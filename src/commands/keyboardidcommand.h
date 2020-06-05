#ifndef KeyboardIdCommand_H
#define KeyboardIdCommand_H

#include "keyboard_command.h"
#include <functional>

class KeyboardIdCommand : public KeyboardCommand
{
public:
    KeyboardIdCommand(std::function<void(QStringList names)> func);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    std::function<void(QStringList names)> _func;
};

#endif // KeyboardIdCommand_H
