//
//  PopularizeSignManager.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 09/09/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "PopularizeSignManager.h"
#include "PopularizeSignArticle.h"
#include "PopularizeSignLayer.h"
#include "PopularizeOfMy.h"
#include "PopularizeSignResult.h"

#include "HNLobby/GamePrompt.h"
#include "HNPlatform/HNPlatformLogic.h"

PopularizeSignManager::PopularizeSignManager() {
    
}

PopularizeSignManager::~PopularizeSignManager() {
    
}

bool PopularizeSignManager::init() {
    if (!Layer::init()) {
        return false;
    }
    resetData();
    
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
    addChild(colorLayer);
    
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        return true;
    };
    touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
        closeLayer();
    };
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);
    
    this->setContentSize(WIN_SIZE);
    this->setIgnoreAnchorPointForPosition(false);
    
    createUI();
    
    requestState();
    
    return true;
}

void PopularizeSignManager::openAddToParent(Layer *parent, int zOrder, cocos2d::Vec2 pos) {
    if (parent) {
        this->setPosition(pos);
        parent->addChild(this, zOrder);
    }
}

void PopularizeSignManager::resetData() {
    touchListener = nullptr;
}

void PopularizeSignManager::createUI() {
    
}

void PopularizeSignManager::closeLayer() {
    this->removeFromParent();
}

void PopularizeSignManager::requestState() {
    MSG_GP_TuiGuang_ShenQing shenqing;
    shenqing.iUserID = PlatformLogic()->loginResult.dwUserID;
    shenqing.iType = 0;     //状态查询
    PlatformLogic()->sendData(MDM_GP_TUIGUANG_SHENQING, ASS_GP_TUIGUANG_SHENQING, &shenqing, sizeof(MSG_GP_TuiGuang_ShenQing), HN_SOCKET_CALLBACK(PopularizeSignManager::signPopularizeSelector, this));
}

bool PopularizeSignManager::signPopularizeSelector(HN::HNSocketMessage *socketMessage) {
//    @ret=1--设置成功
//    @ret=2--用户ID不存在
//    @ret=3--审核中
//    @ret=4--已经通过审核
//    @ret=5--系统拒绝
//    @ret=6--未申请
    CCAssert(sizeof(MSG_GP_TuiGuang_ShenQing) == socketMessage->objectSize, "sizeof(MSG_GP_TuiGuang_ShenQing) != objectSize");
    if (sizeof(MSG_GP_TuiGuang_ShenQing) != socketMessage->objectSize)    return false;
    MSG_GP_TuiGuang_ShenQing* msg = (MSG_GP_TuiGuang_ShenQing*)socketMessage->object;
    if (msg == nullptr) {
        return true;
    }
    if (socketMessage) {
        switch (socketMessage->messageHead.bHandleCode) {
            case 3: {
                //审核中
                auto signLayer = PopularizeSignLayer::create();
                if (signLayer) {
                    signLayer->openAddToParent(this, WIN_SIZE/2);
                    signLayer->setIsSigning(true);
                }
            }
                break;
            case 4: {
                //审核通过
                auto myLayer = PopularizeOfMy::create();
                if (myLayer) {
                    myLayer->openAddToParent(this, WIN_SIZE/2);
                }
            }
                break;
            case 5: {
                //系统拒绝
                auto resultLayer = PopularizeSignResult::create();
                if (resultLayer) {
                    resultLayer->openAddToParent(this, WIN_SIZE/2);
                    if (msg) {
                        resultLayer->setRefuseInfo(msg->szMessageInfo);
                    }
                }
            }
                break;
            case 6: {
                //未申请
                auto signLayer = PopularizeSignLayer::create();
                if (signLayer) {
                    signLayer->openAddToParent(this, WIN_SIZE/2);
                }
            }
                break;
            default:
                closeLayer();
                break;
        }
    }
    
    return true;
}

