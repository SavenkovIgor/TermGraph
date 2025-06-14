#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>

#include <expected>
#include <functional>
#include <string>

class WikiUrlChecker : public QObject
{
    Q_OBJECT

public:
    using Result   = std::expected<bool, std::string>;
    using Callback = std::function<void(Result)>;

    explicit WikiUrlChecker(QObject* parent = nullptr);

    void check(const QString& term, Callback callback);

private slots:
    void onFinished();

private:
    QNetworkAccessManager mManager;
    QNetworkReply*        mReply{nullptr};
    Callback              mCallback;
};
