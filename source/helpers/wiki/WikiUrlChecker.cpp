#include "WikiUrlChecker.h"

#include <QNetworkRequest>
#include <QUrl>

WikiUrlChecker::WikiUrlChecker(QObject* parent)
    : QObject(parent)
{}

WikiUrlChecker::~WikiUrlChecker()
{
    if (mReply) {
        // Disconnect signals to avoid calling onFinished after destruction
        disconnect(mReply, nullptr, this, nullptr);
        mReply->abort();
        mReply->deleteLater();
        mReply = nullptr;
        // Notify the callback that the operation was aborted due to destruction
        if (mCallback) {
            mCallback(std::unexpected("WikiUrlChecker destroyed before operation completed."));
        }
    }
}

void WikiUrlChecker::checkPageExist(const QString& term, Callback callback)
{
    if (mReply) {
        // Instead of silent failure, immediately invoke the callback with an error
        if (callback) {
            callback(std::unexpected("WikiUrlChecker is busy: another check is already in progress."));
        }
        return;
    }

    mCallback = std::move(callback);

    QString encodedTerm = term;
    encodedTerm.replace(' ', '_');
    QUrl url(QString("https://en.wikipedia.org/wiki/%1").arg(encodedTerm));
    QNetworkRequest request(url);

    mReply = mManager.head(request);
    connect(mReply, &QNetworkReply::finished, this, &WikiUrlChecker::onFinished);
}

void WikiUrlChecker::onFinished()
{
    auto* reply = mReply;
    mReply = nullptr;

    Result result;

    if (reply->error() == QNetworkReply::ContentNotFoundError) {
        result = false;
    } else if (reply->error() != QNetworkReply::NoError) {
        result = std::unexpected(reply->errorString().toStdString());
    } else {
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        result = (status == 200);
    }

    reply->deleteLater();

    if (mCallback) {
        mCallback(std::move(result));
    }
}

bool WikiUrlChecker::isBusy() const
{
    return mReply != nullptr;
}
