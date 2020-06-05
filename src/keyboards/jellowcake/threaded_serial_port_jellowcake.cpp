#include "threaded_serial_port_jellowcake.h"
#include <QtCore/QTime>
#include <QtCore/QDebug>

QT_USE_NAMESPACE

ThreadedSerialPortJellowcake::ThreadedSerialPortJellowcake(int waitTimeoutMs, QObject *parent)
    : ThreadedSerialPort(waitTimeoutMs, parent)
{
}

void ThreadedSerialPortJellowcake::exec()
{
    if (sem.available())
    {
        sem.acquire();

        mutex.lock();
        currentWaitTimeout = _waitTimeoutMs;
        currentRequest = request;
        mutex.unlock();

        // qDebug() << "write request: " << currentRequest;
        QByteArray requestData = currentRequest.toLocal8Bit();
        _serialport->write(requestData);
        if (_serialport->waitForBytesWritten(currentWaitTimeout))
        {
            QByteArray responseData = _serialport->readAll();
            partialResponse = responseData;
            // qDebug() << "partial response: " << partialResponse;

            while (!partialResponse.endsWith(">OK\n") && !partialResponse.endsWith(">ERR\n") &&
                   !partialResponse.contains(">NC\n"))
            {
                while (_serialport->waitForReadyRead(5))
                    responseData += _serialport->readAll();

                partialResponse = QString(responseData);
                // qDebug() << "more partial response: " << partialResponse;
            }

            QString response(responseData);
            // qDebug() << "response: " << response;
            emit this->response(response);
        }
        else
        {
            emit timeout(tr("Wait read response timeout %1").arg(QTime::currentTime().toString()));
        }

        partialResponse.clear();
    }

    else if (_serialport->waitForReadyRead(1))
    {
        partialResponse += _serialport->readAll();

        if (partialResponse.endsWith("\n"))
        {
            qDebug() << "debug output: " << partialResponse;
            emit output(partialResponse);
            partialResponse.clear();
        }
    }
}
