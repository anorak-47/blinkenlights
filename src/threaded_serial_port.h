#pragma once
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QSemaphore>
#include <QtSerialPort/QSerialPort>
#include <memory>

class ThreadedSerialPort : public QThread
{
    Q_OBJECT

public:
    explicit ThreadedSerialPort(int waitTimeoutMs = 500, QObject *parent = nullptr);
    virtual ~ThreadedSerialPort();

    void setPort(QString const &newPortName);
    void close();

    void transaction(const QString &request);
    virtual void run() final;

    virtual void exec() = 0;

signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);
    void output(const QString &s);

protected:
    void openPort();

    QString _portName;
    int _waitTimeoutMs;

    QString request;
    bool quit;

    std::unique_ptr<QSerialPort> _serialport;

    QMutex mutex;
    //QWaitCondition cond;
    QSemaphore sem;

    int currentWaitTimeout;
    QString currentRequest;
    QString partialResponse;
};
