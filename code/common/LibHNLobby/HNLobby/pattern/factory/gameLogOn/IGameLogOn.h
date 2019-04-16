//
//  IGameLogOn.h
//  LibHNLobby
//
//  Created by 黄百超 on 21/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef IGameLogOn_h
#define IGameLogOn_h

#include "HNUIExport.h"
#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include <functional>
USING_NS_CC;

class IGameLogOn : public HNLayer {
public:
    typedef std::function<void()> LogOnCallBack;
    typedef std::function<void(Ref * pSender, cocos2d::ui::Widget::TouchEventType type)> WeChatLogOnCallBack;
    typedef std::function<void()> LocalLogOnCallBack;
    
public:
    LogOnCallBack	onLogOnCallBack;
    WeChatLogOnCallBack wechatLogOnCallBack;
    LocalLogOnCallBack localLogonCallBack;
};

#endif /* IGameLogOn_h */
