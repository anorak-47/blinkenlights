#ifndef SleepLedCommand_H
#define SleepLedCommand_H

#include "keyboard_command.h"

class SleepLedCommand : public KeyboardCommand
{
public:
    SleepLedCommand(bool on);

    bool getOn() const;
    void setOn(bool value);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

protected:
    bool _on;
};

#endif // SleepLedCommand_H
