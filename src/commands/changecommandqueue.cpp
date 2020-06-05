#include "changecommandqueue.h"
#include <QtCore/QDebug>
#include <QtCore/QMutexLocker>

#define MAX_COMSUMER_QUEUE_SIZE 100

ChangeCommandQueue::ChangeCommandQueue() :
    _interruptionRequested(false)
{

}

void ChangeCommandQueue::enqueue(std::unique_ptr<KeyboardCommand> cmd)
{
    qDebug() << "enqueue";

    mutex.lock();
    if (_queue.size() > MAX_COMSUMER_QUEUE_SIZE)
        bufferNotFull.wait(&mutex);
    mutex.unlock();

    mutex.lock();
    _queue.push(std::move(cmd));
    bufferNotEmpty.wakeAll();
    mutex.unlock();
}

std::unique_ptr<KeyboardCommand> ChangeCommandQueue::dequeue()
{
    qDebug() << "dequeue";

    mutex.lock();
    if (_queue.empty())
        bufferNotEmpty.wait(&mutex);

    //mutex.unlock();
    //mutex.lock();

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

bool ChangeCommandQueue::empty()
{
    QMutexLocker locker(&mutex);
    return _queue.empty();
}

void ChangeCommandQueue::requestInterruption()
{
    qDebug() << "requestInterruption";

    QMutexLocker locker(&mutex);
    _interruptionRequested = true;
    bufferNotEmpty.wakeAll();
}

std::unique_ptr<ChangeCommandQueue> ChangeCommandQueueInstance::_queue(std::unique_ptr<ChangeCommandQueue>(new ChangeCommandQueue));

ChangeCommandQueueInstance::ChangeCommandQueueInstance()
{

}

void ChangeCommandQueueInstance::enqueue(std::unique_ptr<KeyboardCommand> cmd)
{
    _queue->enqueue(std::move(cmd));
}

std::unique_ptr<KeyboardCommand> ChangeCommandQueueInstance::dequeue()
{
    return _queue->dequeue();
}

bool ChangeCommandQueueInstance::empty()
{
    return _queue->empty();
}

void ChangeCommandQueueInstance::requestInterruption()
{
    _queue->requestInterruption();
}
