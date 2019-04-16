//
//  PlayerHeadV99.h
//  MixProject
//
//  Created by 黄百超 on 12/03/2017.
//
//

#ifndef BBPlayerHeadV99_h
#define BBPlayerHeadV99_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"

USING_NS_CC;

namespace BB {
    
    class BBPlayerHeadV99 : public Layer {
        
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
        
    public:
        CREATE_FUNC(BBPlayerHeadV99);
        BBPlayerHeadV99();
        ~BBPlayerHeadV99();
        
        virtual bool init() override;
        
        void setPlayerIcon(int iconIndex);
        void setPlayerName(char playerName[61]);
        void setPlayerMoney(long long money);
        
        void setHeadPosition(Vec2 pos);
        
    private:
        void initDefaultUI();
        
    };
    
}

#endif /* BBPlayerHeadV99_h */
