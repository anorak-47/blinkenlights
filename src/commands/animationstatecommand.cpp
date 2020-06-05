#include "animationstatecommand.h"
#include <QtCore/QDebug>

AnimationStateCommand::AnimationStateCommand(unsigned int animation, std::function<void(unsigned int animation, bool running, unsigned int fps, QColor const &color1, QColor const &color2)> func)
    : _animation(animation), _func(func)
{

}

unsigned int AnimationStateCommand::animation()
{
    return _animation;
}

void AnimationStateCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    qDebug() << __PRETTY_FUNCTION__;

    unsigned int currentAnimation = protocol->getAnimationCurrent();
    QColor color1 = protocol->getAnimationColor(currentAnimation, 0);
    QColor color2 = protocol->getAnimationColor(currentAnimation, 1);
    bool running = protocol->getAnimationRunning(currentAnimation);
    unsigned int fps = protocol->getAnimationFPS(currentAnimation);

    _func(currentAnimation, running, fps, color1, color2);
}
