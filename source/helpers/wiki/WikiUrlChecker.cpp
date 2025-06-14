#include "source/helpers/wiki/WikiUrlChecker.h"

#include <QNetworkRequest>
#include <QUrl>

WikiUrlChecker::WikiUrlChecker(QObject* parent)
    : QObject(parent)
{
    connect(&mTimer, &QTimer::timeout, this, &WikiUrlChecker::processQueue);
    mTimer.setInterval(mRequestIntervalMs);
    mTimer.setSingleShot(false);
}

WikiUrlChecker::~WikiUrlChecker()
{
    if (mCurrentRequest) {
        mCurrentRequest->reply->abort();
        mCurrentRequest->reply->deleteLater();
        mCurrentRequest->callback(std::unexpected("WikiUrlChecker destroyed before operation completed."));
        mCurrentRequest.reset();
    }
    // Notify all pending requests that the checker is destroyed
    while (!mQueue.empty()) {
        auto& req = mQueue.front();
        if (req.callback) {
            req.callback(std::unexpected("WikiUrlChecker destroyed before operation completed."));
        }
        mQueue.pop();
    }
}

void WikiUrlChecker::checkPageExists(const QString& term, Callback callback) { addToQueue(term, std::move(callback)); }

void WikiUrlChecker::processQueue()
{
    if (isBusy() || mQueue.empty()) {
        return;
    }
    mCurrentRequest = std::move(mQueue.front());
    mQueue.pop();

    QUrl url(QString("https://en.wikipedia.org/wiki/%1").arg(mCurrentRequest->term));

    QNetworkRequest request(url);
    mCurrentRequest->reply = mManager.head(request);
    connect(mCurrentRequest->reply, &QNetworkReply::finished, this, &WikiUrlChecker::onFinished);
}

void WikiUrlChecker::onFinished()
{
    auto* reply = mCurrentRequest->reply;
    CheckResult result;

    if (reply->error() == QNetworkReply::ContentNotFoundError) {
        result = false;
    } else if (reply->error() != QNetworkReply::NoError) {
        result = std::unexpected("Failed to check Wikipedia page '" +
                                mCurrentRequest->term.toStdString() +
                                "': " + reply->errorString().toStdString());
    } else {
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        result     = (status == 200);
    }

    reply->deleteLater();

    if (mCurrentRequest->callback) {
        mCurrentRequest->callback(std::move(result));
    }
    mCurrentRequest.reset();

    // After finishing, process the next item if available
    stopTimerIfNeeded();
}

void WikiUrlChecker::startTimerIfNeeded()
{
    if (mTimer.isActive()) {
        return;
    }

    if (!mQueue.empty()) {
        mTimer.start();
    }
}

void WikiUrlChecker::stopTimerIfNeeded()
{
    if (mQueue.empty() && !isBusy()) {
        mTimer.stop();
    }
}

size_t WikiUrlChecker::queueSize() const
{
    return mQueue.size();
}

void WikiUrlChecker::addToQueue(const QString& term, Callback&& callback)
{
    mQueue.push(Request{term, std::move(callback)});
    startTimerIfNeeded();
}
