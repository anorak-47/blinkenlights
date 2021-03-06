#pragma once
#include "keyboard_command.h"
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>
#include <memory>
#include <queue>

class KeyboardCommandQueue
{
public:
    KeyboardCommandQueue();

    void enqueue(std::unique_ptr<KeyboardCommand> cmd);
    std::unique_ptr<KeyboardCommand> dequeue();
    bool empty();

    void requestInterruption();

private:
    bool _interruptionRequested;
    QWaitCondition bufferNotEmpty;
    QWaitCondition bufferNotFull;
    QMutex mutex;

    std::queue<std::unique_ptr<KeyboardCommand>> _queue;
};

class KeyboardCommandQueueInstance
{
public:
    KeyboardCommandQueueInstance();

    void enqueue(std::unique_ptr<KeyboardCommand> cmd);
    std::unique_ptr<KeyboardCommand> dequeue();
    bool empty();

    void requestInterruption();

private:
    static std::unique_ptr<KeyboardCommandQueue> _queue;
};
