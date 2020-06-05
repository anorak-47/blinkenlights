#ifndef KeymapConfigSaveCommand_H
#define KeymapConfigSaveCommand_H

#include "keyboard_command.h"

class KeymapConfigSaveCommand : public KeyboardCommand
{
public:
    KeymapConfigSaveCommand();

    virtual void exec(std::shared_ptr<KeyboardInterface> &protocol) override;
};

#endif // KeymapConfigSaveCommand_H
