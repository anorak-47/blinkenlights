#ifndef ANIMATIONRUNCOMMAND_H
#define ANIMATIONRUNCOMMAND_H

#include "animationchangecommand.h"

class AnimationRunCommand : public AnimationChangeCommand
{
public:
    AnimationRunCommand(unsigned int animation, bool run);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

    bool running() const;
    void setRunning(bool running);

protected:
    bool _running;
};

#endif // ANIMATIONRUNCOMMAND_H
