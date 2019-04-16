//
//  IGameRegister.h
//  LibHNLobby
//
//  Created by 黄百超 on 22/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef IGameRegister_h
#define IGameRegister_h

#include "HNUIExport.h"
#include "cocos2d.h"

USING_NS_CC;

class IGameRegister : public HNLayer {
public:
    typedef std::function<void(const std::string& name, const std::string& psd, BYTE gender, const std::string nickName, const std::string inviteCode)> RegisterCallBack;
    typedef std::function<void ()> CloseCallBack;
    
public:
    RegisterCallBack onRegisterCallBack;
    CloseCallBack onCloseCallBack;
    
public:
    virtual void initRegisterUI() {}
    
};

#endif /* IGameRegister_h */
