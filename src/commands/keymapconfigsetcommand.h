#ifndef KeymapConfigSetCommand_H
#define KeymapConfigSetCommand_H

#include "keyboard_command.h"

class KeymapConfigSetCommand : public KeyboardCommand
{
public:
    KeymapConfigSetCommand(unsigned int config);

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;

    unsigned int config();

protected:
    unsigned int _config;
};

#endif // KeymapConfigSetCommand_H
