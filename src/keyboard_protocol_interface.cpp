#include <QtCore/QDebug>
#include "keyboard_protocol_interface.h"

KeyboardCommandTransfer::KeyboardCommandTransfer()
{
}

KeyboardCommandTransfer::~KeyboardCommandTransfer()
{
}

void KeyboardCommandTransfer::setKeyboardInterface(std::shared_ptr<KeyboardInterface> protocol)
{
    _protocol = protocol;
}

void KeyboardCommandTransfer::exec(std::unique_ptr<KeyboardCommand> cmd)
{
    // qDebug() << "KeyboardCommandTransfer::exec";
    cmd->exec(_protocol);
}

std::shared_ptr<KeyboardInterface> KeyboardCommandTransfer::getProtocol() const
{
    return _protocol;
}

std::unique_ptr<KeyboardCommandTransfer> KeyboardCommandTransferInterface::_transfer(
    std::unique_ptr<KeyboardCommandTransfer>(new KeyboardCommandTransfer()));

KeyboardCommandTransferInterface::KeyboardCommandTransferInterface()
{
}

void KeyboardCommandTransferInterface::setKeyboardCommandTransferImplementation(
    std::shared_ptr<KeyboardInterface> protocol)
{
    _transfer->setKeyboardInterface(protocol);
}

void KeyboardCommandTransferInterface::exec(std::unique_ptr<KeyboardCommand> cmd)
{
    _transfer->exec(std::move(cmd));
}

std::shared_ptr<KeyboardInterface> KeyboardCommandTransferInterface::getProtocol() const
{
    return _transfer->getProtocol();
}
