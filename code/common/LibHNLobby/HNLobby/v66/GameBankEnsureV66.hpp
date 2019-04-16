//
//  GameBankEnsureV66.hpp
//  LibHNLobby
//
//  Created by mac on 2018/8/22.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef GameBankEnsureV66_hpp
#define GameBankEnsureV66_hpp

#include "HNLobby/PlatformDefine.h"
#include "HNLobby/Shop/ShopManager.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"
#include "../pattern/factory/gameBank/IGameBank.h"
#include<string.h>
#include "UI/Base/AUIButton.h"
#include "UI/HNEditBox.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace network;
using namespace std;

class GameBankEnsureV66 : public Layer, public ui::EditBoxDelegate  {
public:
    GameBankEnsureV66();
    virtual ~GameBankEnsureV66();
    
    virtual bool init() override;
    CREATE_FUNC(GameBankEnsureV66);
    
    void open(Node* parent, Vec2 vec, int zorder, int tag);
    void close();
    typedef std::function<void()> bankCallback;
    bankCallback _bCallBack;
    
    virtual void editBoxReturn(ui::EditBox* editBox)override;
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox)override;
    bool bankerEventSelector(HNSocketMessage* socketMessage);
 private:
    char szBankPassWord[50];
    std::string savePassword;
    HNEditBox * mima;
};


#endif /* GameBankEnsureV66_hpp */
