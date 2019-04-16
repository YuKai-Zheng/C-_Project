#ifndef _ADD_LOADER_COMMAND_H_
#define _ADD_LOADER_COMMAND_H_
#include "../pattern/Command.h"

#include <string>
#include "cocos2d.h"
USING_NS_CC;

/*
*/
class AddLoaderCom : public Command {
public:
    virtual void execute()override;

    void setTitle(std::string text);
    void setTTFConfig(TTFConfig& ttfConfig);
private:
    std::string m_title;
    TTFConfig m_ttfconfig;
};

#endif //_ADD_LOADER_COMMAND_H_
