#pragma once
#include "threaded_serial_port.h"

class ThreadedSerialPortJellowcake : public ThreadedSerialPort
{
    Q_OBJECT

public:
    explicit ThreadedSerialPortJellowcake(int waitTimeoutMs = 500, QObject *parent = nullptr);
    void exec() final;
};
