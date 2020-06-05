#include "bootloaderstartcommand.h"

BootloaderStartCommand::BootloaderStartCommand()
{

}

void BootloaderStartCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->enterBootloader();
}
