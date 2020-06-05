#include "threaded_serial_port.h"
#include <QTime>
#include <QtCore/QDebug>

QT_USE_NAMESPACE

ThreadedSerialPort::ThreadedSerialPort(int waitTimeoutMs, QObject *parent)
    : QThread(parent), _waitTimeoutMs(waitTimeoutMs), quit(false)
{
}

ThreadedSerialPort::~ThreadedSerialPort()
{
    close();
}

void ThreadedSerialPort::setPort(QString const &newPortName)
{
    QMutexLocker locker(&mutex);
    _portName = newPortName;
}

void ThreadedSerialPort::openPort()
{
    QMutexLocker locker(&mutex);

    qDebug() << "open serial port " << _portName;

    _serialport->close();
    _serialport->setPortName(_portName);
    _serialport->setBaudRate(QSerialPort::Baud115200);

    if (!_serialport->open(QIODevice::ReadWrite))
    {
        qDebug() << QString("Can't open %1, error (code %2): %3")
                .arg(_portName)
                .arg(_serialport->error())
                .arg(_serialport->errorString());

        emit error(tr("Can't open %1, error (code %2): %3")
                       .arg(_portName)
                       .arg(_serialport->error())
                       .arg(_serialport->errorString()));
    }

    _serialport->setDataTerminalReady(true);
    _serialport->setRequestToSend(true);
}

void ThreadedSerialPort::close()
{
    if (quit)
        return;

    mutex.lock();
    quit = true;
    //cond.wakeOne();
    mutex.unlock();
    wait();
}

void ThreadedSerialPort::transaction(const QString &request)
{
    QMutexLocker locker(&mutex);
    //qDebug() << "new request: " << request;

    this->request = request;

    if (!isRunning())
    {
        start();
    }

    sem.release();
}

void ThreadedSerialPort::run()
{
    qDebug() << "serial port start";

    _serialport = std::unique_ptr<QSerialPort>(new QSerialPort());
    openPort();

    while (!quit)
    {
        exec();
    }

    _serialport->close();
}
