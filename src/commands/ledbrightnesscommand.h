#ifndef LedBrightnessCommand_H
#define LedBrightnessCommand_H

#include "ledchangecommand.h"

class LedBrightnessCommand : public LedChangeCommand
{
public:
    LedBrightnessCommand(unsigned int led, unsigned int brightness);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

    unsigned int brightness() const;
    void setBrightness(unsigned int brightness);

protected:
    unsigned int _brightness;
};

#endif // LedBrightnessCommand_H
