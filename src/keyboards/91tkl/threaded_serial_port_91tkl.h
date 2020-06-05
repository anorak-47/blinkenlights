#pragma once
#include "threaded_serial_port.h"

class ThreadedSerialPort91tkl : public ThreadedSerialPort
{
    Q_OBJECT

public:
    explicit ThreadedSerialPort91tkl(int waitTimeoutMs = 500, QObject *parent = nullptr);
    virtual void exec() final;
};
