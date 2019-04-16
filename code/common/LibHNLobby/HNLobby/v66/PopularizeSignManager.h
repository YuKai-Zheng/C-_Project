//
//  PopularizeSignManager.h
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef PopularizeSignManager_h
#define PopularizeSignManager_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"
#include "HNSocket/HNSocketMessage.h"

USING_NS_CC;

//推广申请界面
class PopularizeSignManager : public Layer {
public:
    PopularizeSignManager();
    ~PopularizeSignManager();
    CREATE_FUNC(PopularizeSignManager);
    virtual bool init() override;
    
    void openAddToParent(Layer* parent, int zOrder, Vec2 pos);
    
private:
    void resetData();
    void createUI();
    
    void closeLayer();
    
    void requestState();
    bool signPopularizeSelector(HN::HNSocketMessage *socketMessage);
    
private:
    EventListenerTouchOneByOne*		touchListener;
    
};


#endif /* PopularizeSignManager_h */
