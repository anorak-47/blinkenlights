#include "animationchangecommand.h"

AnimationChangeCommand::AnimationChangeCommand(unsigned int animation)
    : _animation(animation)
{

}

unsigned int AnimationChangeCommand::animation()
{
    return _animation;
}
