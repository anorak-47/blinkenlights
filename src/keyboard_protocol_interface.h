#pragma once
#include "keyboard_command.h"
#include "keyboard_interface.h"
#include <memory>
#include <queue>

class KeyboardCommandTransfer
{
public:
    KeyboardCommandTransfer();
    virtual ~KeyboardCommandTransfer();

    void setKeyboardInterface(std::shared_ptr<KeyboardInterface> protocol);
    void exec(std::unique_ptr<KeyboardCommand> cmd);

    std::shared_ptr<KeyboardInterface> getProtocol() const;

private:
    std::shared_ptr<KeyboardInterface> _protocol;
};

class KeyboardCommandTransferInterface
{
public:
    KeyboardCommandTransferInterface();

    void setKeyboardCommandTransferImplementation(std::shared_ptr<KeyboardInterface> protocol);
    void exec(std::unique_ptr<KeyboardCommand> cmd);

    std::shared_ptr<KeyboardInterface> getProtocol() const;

private:
    static std::unique_ptr<KeyboardCommandTransfer> _transfer;
};
