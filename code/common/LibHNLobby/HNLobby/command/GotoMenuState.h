#include "../pattern/Command.h"
#ifndef _GOTO_MENU_COMMAND_H_
#define _GOTO_MENU_COMMAND_H_

/*
*/
class GotoMenuState : public Command {
public:
    virtual void execute()override;
};

#endif //_GOTO_MENU_COMMAND_H_
