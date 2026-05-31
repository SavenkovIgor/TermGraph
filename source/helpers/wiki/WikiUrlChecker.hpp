#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QTimer>

#include <expected>
#include <functional>
#include <string>
#include <queue>

class WikiUrlChecker : public QObject
{
    Q_OBJECT

public:
    using CheckResult = std::expected<bool, std::string>;
    using Callback    = std::function<void(CheckResult)>;

    explicit WikiUrlChecker(QObject* parent = nullptr);
    ~WikiUrlChecker() override;

    void   checkPageExists(const QString& term, Callback callback);
    bool   isBusy() const { return mCurrentRequest.has_value(); }
    size_t queueSize() const;

private slots:
    void onFinished();
    void processQueue();

private:
    struct Request
    {
        QString        term;
        Callback       callback;
        QNetworkReply* reply = nullptr;
    };
    std::queue<Request>    mQueue;
    std::optional<Request> mCurrentRequest;

    QTimer               mTimer;
    constexpr static int mRequestIntervalMs = 1000;

    QNetworkAccessManager mManager;

    void startTimerIfNeeded();
    void stopTimerIfNeeded();

    void addToQueue(const QString& term, Callback&& callback);
};
