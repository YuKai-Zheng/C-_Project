#include "../pattern/Command.h"
#include "HNSocket/HNSocketMessage.h"
#ifndef _CHECK_GIFT_COMMAND_H_
#define _CHECK_GIFT_COMMAND_H_

using namespace HN;

/*
*/
class CheckGiftCommand : public Command {
public:
    virtual void execute()override;
    void checkGiftMessages();
    //查询信息回调
    bool checkGiftMessagesEventSelector(HNSocketMessage *socketMessage);
};

#endif //_CHECK_GIFT_COMMAND_H_
