#ifndef KeymapConfigGetCommand_H
#define KeymapConfigGetCommand_H

#include "keyboard_command.h"
#include <functional>

class KeymapConfigGetCommand : public KeyboardCommand
{
public:
    KeymapConfigGetCommand(std::function<void(unsigned int)> func);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    std::function<void(unsigned int)> _func;
};

#endif // KeymapConfigGetCommand_H
