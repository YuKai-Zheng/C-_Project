//
//  GamdLogOnCompositeV88.h
//  LibHNLobby
//
//  Created by 黄百超 on 21/06/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef GameLogOnCompositeV88_h
#define GameLogOnCompositeV88_h

#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "UI/Base/AUIButton.h"
#include "../pattern/factory/gameLogOn/IGameLogOn.h"
#include "UI/HNEditBox.h"
using namespace HN;
USING_NS_CC;

using namespace cocostudio;
using namespace ui;

class GameLogOnCompositeV88 : public IGameLogOn {
    
public:
    CREATE_FUNC(GameLogOnCompositeV88);
    GameLogOnCompositeV88();
    virtual ~GameLogOnCompositeV88();
    
    virtual bool init() override;
    void onExitTransitionDidStart() override;
    void onEnter() override;
    
private:
    void userLogonCallback(Ref* pSender, Widget::TouchEventType type);
    void wechatLogonCallBack(Ref* pSender, Widget::TouchEventType type);
    void localLogonCallBack(Ref* pSender, Widget::TouchEventType type);

private:
    struct LOGON_NODE {
        Node*       node;
        AUIButton*  userLogonBtn;
        AUIButton*  wechatLogonBtn;
        AUIButton*  localLogonBtn;
    } _logon_node;
    
};

#endif /* GameLogOnCompositeV88_h */
