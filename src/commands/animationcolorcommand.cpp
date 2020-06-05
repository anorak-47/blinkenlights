#include "animationcolorcommand.h"
#include <QtCore/QDebug>

AnimationColorCommand::AnimationColorCommand(unsigned int animation, unsigned int colorid, const QColor &color)
    : AnimationChangeCommand(animation), _colorid(colorid), _color(color)
{

}

QColor AnimationColorCommand::color() const
{
    return _color;
}

void AnimationColorCommand::setColor(const QColor &color)
{
    _color = color;
}

unsigned int AnimationColorCommand::colorid() const
{
    return _colorid;
}

void AnimationColorCommand::setColorid(unsigned int colorid)
{
    _colorid = colorid;
}

void AnimationColorCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    protocol->setAnimationColor(_animation, _colorid, _color);
}
