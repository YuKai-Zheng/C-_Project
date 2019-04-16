//
//  IGameLand.h
//  LibHNLobby
//
//  Created by 黄百超 on 21/02/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef IGameLand_h
#define IGameLand_h

#include "HNUIExport.h"
#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include <functional>
USING_NS_CC;

class IGameLand : public HNLayer {
public:
    typedef std::function<void (const std::string& name, const std::string& psd)> LoginCallBack;
    typedef std::function<void (Ref * pSender, cocos2d::ui::Widget::TouchEventType type)> YouKeLoginCallBack;
    typedef std::function<void ()> RegistCallBack;
    typedef std::function<void ()> FindPwCallBack;
    typedef std::function<void ()> BackCallBack;

public:
    LoginCallBack	onLoginCallBack;
    YouKeLoginCallBack	youKeLoginCallBack;
    RegistCallBack  onRegistCallBack;
    FindPwCallBack  onFindPwCallBack;
    BackCallBack onBackCallBack;

};

#endif /* IGameLand_h */
