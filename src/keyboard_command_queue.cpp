#include <QtCore/QDebug>
#include <QtCore/QMutexLocker>
#include "keyboard_command_queue.h"

#define MAX_COMSUMER_QUEUE_SIZE 100

KeyboardCommandQueue::KeyboardCommandQueue() : _interruptionRequested(false)
{
}

void KeyboardCommandQueue::enqueue(std::unique_ptr<KeyboardCommand> cmd)
{
    //qDebug() << "enqueue";

    mutex.lock();
    if (_queue.size() > MAX_COMSUMER_QUEUE_SIZE)
        bufferNotFull.wait(&mutex);
    mutex.unlock();

    mutex.lock();
    _queue.push(std::move(cmd));
    bufferNotEmpty.wakeAll();
    mutex.unlock();
}

std::unique_ptr<KeyboardCommand> KeyboardCommandQueue::dequeue()
{
    //qDebug() << "dequeue";

    mutex.lock();
    if (_queue.empty())
        bufferNotEmpty.wait(&mutex);

    // mutex.unlock();
    // mutex.lock();

    if (_interruptionRequested)
    {
        qDebug() << "dequeue _interruptionRequested";
        mutex.unlock();
        return std::unique_ptr<KeyboardCommand>();
    }

    std::unique_ptr<KeyboardCommand> cmd = std::move(_queue.front());
    _queue.pop();

    bufferNotFull.wakeAll();
    mutex.unlock();

    return cmd;
}

bool KeyboardCommandQueue::empty()
{
    QMutexLocker locker(&mutex);
    return _queue.empty();
}

void KeyboardCommandQueue::requestInterruption()
{
    qDebug() << "requestInterruption";

    QMutexLocker locker(&mutex);
    _interruptionRequested = true;
    bufferNotEmpty.wakeAll();
}

std::unique_ptr<KeyboardCommandQueue>
    KeyboardCommandQueueInstance::_queue(std::unique_ptr<KeyboardCommandQueue>(new KeyboardCommandQueue));

KeyboardCommandQueueInstance::KeyboardCommandQueueInstance()
{
}

void KeyboardCommandQueueInstance::enqueue(std::unique_ptr<KeyboardCommand> cmd)
{
    _queue->enqueue(std::move(cmd));
}

std::unique_ptr<KeyboardCommand> KeyboardCommandQueueInstance::dequeue()
{
    return _queue->dequeue();
}

bool KeyboardCommandQueueInstance::empty()
{
    return _queue->empty();
}

void KeyboardCommandQueueInstance::requestInterruption()
{
    _queue->requestInterruption();
}
