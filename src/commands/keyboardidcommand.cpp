#include "keyboardidcommand.h"
#include <QtCore/QDebug>

KeyboardIdCommand::KeyboardIdCommand(std::function<void(QStringList names)> func)
    : _func(func)
{

}

void KeyboardIdCommand::exec(std::shared_ptr<KeyboardInterface> &protocol)
{
    qDebug() << __PRETTY_FUNCTION__;
    //QStringList names = protocol->getKeyboardInfo();
    protocol->getKeyboardInfo();
    //_func(names);
}
