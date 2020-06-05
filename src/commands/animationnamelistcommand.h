#ifndef AnimationNameListCommand_H
#define AnimationNameListCommand_H

#include "keyboard_command.h"
#include <functional>

class AnimationNameListCommand : public KeyboardCommand
{
public:
    AnimationNameListCommand(std::function<void(QStringList names)> func);

    void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    std::function<void(QStringList names)> _func;
};

#endif // AnimationNameListCommand_H
