#ifndef ANIMATIONSTATECOMMAND_H
#define ANIMATIONSTATECOMMAND_H

#include "keyboard_command.h"
#include <functional>

class AnimationStateCommand : public KeyboardCommand
{
public:
    AnimationStateCommand(unsigned int animation, std::function<void(unsigned int animation, bool running, unsigned int fps, QColor const &color1, QColor const &color2)> func);

    unsigned int animation();

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    unsigned int _animation;
    std::function<void(unsigned int animation, bool running, unsigned int fps, QColor const &color1, QColor const &color2)> _func;
};

#endif // ANIMATIONSTATECOMMAND_H
