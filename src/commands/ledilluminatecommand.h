#ifndef LedIlluminateCommand_H_
#define LedIlluminateCommand_H_

#include "ledchangecommand.h"

class LedIlluminateCommand : public LedChangeCommand
{
public:
    LedIlluminateCommand(unsigned int led, bool illuminate);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

    bool illuminated() const;
    void setIlluminated(bool illuminated);

protected:
    bool _illuminated;
};

#endif // LedIlluminateCommand_H_
