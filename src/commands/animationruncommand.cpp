#include "animationruncommand.h"
#include <QtCore/QDebug>

AnimationRunCommand::AnimationRunCommand(unsigned int animation, bool run)
    : AnimationChangeCommand(animation), _running(run)
{

}

bool AnimationRunCommand::running() const
{
    return _running;
}

void AnimationRunCommand::setRunning(bool running)
{
    _running = running;
}

void AnimationRunCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setAnimationRunning(_animation, _running);
}
