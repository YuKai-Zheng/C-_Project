//
//  JackpotModel.hpp
//  LibHNUI
//
//  Created by mac on 2018/8/27.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef JackpotModel_hpp
#define JackpotModel_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;
using namespace ui;
namespace HN {
    
    class JackpotModel : public Node {
    public:
        CREATE_FUNC(JackpotModel);
        JackpotModel();
        ~JackpotModel();
        virtual bool init() override;
        virtual void onExit() override;
        
    public:
        void setJackPotData();
        void updateMoney(float dt);
        void setNextUpdate();
        void setTextScale(float scale);
        void setGameData(int _type,int _gameId,int _roomId);//1 代表分类的奖池综合 2 游戏 3房间
        long long getGameJackpot();
        long long getCurJackpot();
    private:
        long long m_currentJackpot;
        long long m_nextJackpot;
        long long m_minJackpot;
        long long m_maxJackpot;
        
        TextAtlas* m_jackpotText;
        
        bool isAdd;
        bool isSub;
        int m_myGameType;
        int m_myGameId;
        int m_roomId;
        
        int minAdd;
        int maxAdd;
    };
    
}

#endif /* JackpotModel_hpp */
