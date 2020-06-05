#ifndef LedChangeCommand_H
#define LedChangeCommand_H

#include "keyboard_command.h"

class LedChangeCommand : public KeyboardCommand
{
public:
    LedChangeCommand(unsigned int led);

    unsigned int led();

protected:
    unsigned int _led;
};

#endif // LedChangeCommand_H
