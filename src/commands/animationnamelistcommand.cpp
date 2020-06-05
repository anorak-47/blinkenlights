#include "animationnamelistcommand.h"
#include <QtCore/QDebug>

AnimationNameListCommand::AnimationNameListCommand(std::function<void(QStringList names)> func)
    : _func(func)
{

}

void AnimationNameListCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    qDebug() << __PRETTY_FUNCTION__;
    QStringList names = protocol->getAnimationNames();
    _func(names);
}
