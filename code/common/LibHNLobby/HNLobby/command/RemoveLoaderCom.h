#ifndef _REMOVE_LOADER_COMMAND_H_
#define _REMOVE_LOADER_COMMAND_H_
#include "../pattern/Command.h"

/*
*/
class RemoveLoaderCom : public Command {
public:
    virtual void execute()override;
};

#endif //_REMOVE_LOADER_COMMAND_H_
