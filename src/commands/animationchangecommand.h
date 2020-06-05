#ifndef ANIMATIONCHANGECOMMAND_H
#define ANIMATIONCHANGECOMMAND_H

#include "keyboard_command.h"

class AnimationChangeCommand : public KeyboardCommand
{
public:
    AnimationChangeCommand(unsigned int animation);

    unsigned int animation();

protected:
    unsigned int _animation;
};

#endif // ANIMATIONCHANGECOMMAND_H


