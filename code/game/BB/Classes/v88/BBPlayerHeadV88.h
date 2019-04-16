//
//  PlayerHeadV88.h
//  MixProject
//
//  Created by 黄百超 on 12/03/2017.
//
//

#ifndef BBPlayerHeadV88_h
#define BBPlayerHeadV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "UI/UserHead.h"

USING_NS_CC;

namespace BB {
    
    class BBPlayerHeadV88 : public Layer {
        
    private:
        struct Head {
            Node* node;
            ImageView* bg;
            Sprite* icon;
            Text* name;
            Text* money;
            Head() {
                node = nullptr;
                bg = nullptr;
                icon = nullptr;
                name = nullptr;
                money = nullptr;
            }
        } _head;
        UserHead*	m_HeadImage;
        
    public:
        CREATE_FUNC(BBPlayerHeadV88);
        BBPlayerHeadV88();
        ~BBPlayerHeadV88();
        
        virtual bool init() override;
        
        void setPlayerIcon(UserInfoStruct* user);
        void setPlayerName(char playerName[61]);
        void setPlayerMoney(long long money);
        
        void setHeadPosition(Vec2 pos);
        
    private:
        void initDefaultUI();
        
    };
    
}

#endif /* BBPlayerHeadV88_h */
