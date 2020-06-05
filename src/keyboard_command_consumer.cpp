#include "keyboard_command_consumer.h"
#include "keyboard_command_queue.h"
#include "keyboard_protocol_interface.h"

KeyboardCommandConsumer::KeyboardCommandConsumer(QObject *parent) : QThread(parent)
{
}

void KeyboardCommandConsumer::run()
{
    forever
    {
        std::unique_ptr<KeyboardCommand> cmd = std::move(KeyboardCommandQueueInstance().dequeue());

        if (isInterruptionRequested())
            return;

        KeyboardCommandTransferInterface().exec(std::move(cmd));
    }
}
