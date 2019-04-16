//
//  GameRewardEffect.hpp
//  LibHNLobby
//
//  Created by mac on 2018/5/7.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef GameRewardEffect_hpp
#define GameRewardEffect_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
using namespace cocos2d;
class GameRewardEffect:public Node
{
public:
    CREATE_FUNC(GameRewardEffect);
    static void createEffect(Node * parentNode);
    GameRewardEffect();
    virtual ~GameRewardEffect();
    virtual bool init() override;
    
private:
    Sprite * tipImg;
    Node * effectNode;
    
    
};



#endif /* GameRewardEffect_hpp */
