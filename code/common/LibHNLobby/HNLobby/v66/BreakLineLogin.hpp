//
//  BreakLineLogin.hpp
//  LibHNLobby
//
//  Created by mac on 2018/10/31.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef BreakLineLogin_hpp
#define BreakLineLogin_hpp

#include "HNLobby/PlatformDefine.h"
#include "HNNetExport.h"
#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"
#include "../pattern/factory/gameBank/IGameBank.h"
#include<string.h>

USING_NS_CC;
using namespace ui;
using namespace cocostudio;
using namespace network;
using namespace std;

class BreakLineLogin : public Layer {
public:
    BreakLineLogin();
    virtual ~BreakLineLogin();
    
    virtual bool init() override;
    CREATE_FUNC(BreakLineLogin);
    
    void open(Node* parent, Vec2 vec, int zorder, int tag);
    void close();
    
private:
   
};


#endif /* BreakLineLogin_hpp */
