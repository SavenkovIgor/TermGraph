#include "WikiUrlChecker.h"

#include <QNetworkRequest>
#include <QUrl>

WikiUrlChecker::WikiUrlChecker(QObject* parent)
    : QObject(parent)
{}

void WikiUrlChecker::check(const QString& term, Callback callback)
{
    if (mReply) {
        return; // busy
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

    if (reply->error() != QNetworkReply::NoError) {
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
