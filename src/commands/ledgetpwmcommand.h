#ifndef LedGetPwmCommand_H
#define LedGetPwmCommand_H

#include "ledchangecommand.h"
#include <functional>

class LedGetPwmCommand : public LedChangeCommand
{
public:
    LedGetPwmCommand(unsigned int led, std::function<void(unsigned int, unsigned int)> func);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    std::function<void(unsigned int, unsigned int)> _func;
};

#endif // LedGetPwmCommand_H
