//
//  RobotInfo.hpp
//  LibHNUI
//
//  Created by mac on 2018/8/10.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef RobotInfo_hpp
#define RobotInfo_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "UserHead.h"
using namespace cocos2d;
using namespace ui;
using namespace cocostudio;
using namespace cocos2d::extension;
namespace HN {
    class RobotInfo : public Node {
    public:
        RobotInfo();
        virtual ~RobotInfo();
        CREATE_FUNC(RobotInfo);
        
        virtual bool init() override;
        virtual void onExit() override;
        void createRobotData(int id,long long money,char* name,int headId,char* szWXHeadUrl);
        void updateRobotData(int id,long long money,char* name,int headId,char* szWXHeadUrl);
        void createRobotUI();
        long getMyUserId();
    private:
        int   _UserId;
        long long _myMoney;
        int    _headMyId;
        char*  _myName;
        char* _myWXHeadUrl;
        
        Text*     _myMoneyLabel;
        Text*     _myNameLabel;
        Node*     _effectNode;
        UserHead * userHead;
        
    };
    
}
#endif /* RobotInfo_hpp */
