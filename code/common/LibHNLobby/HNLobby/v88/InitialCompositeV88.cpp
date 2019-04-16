//
//  InitialCompositeV88.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 14/03/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "InitialCompositeV88.h"
#include "VersionInfo/VersionInfo.h"
#include "Tool/WireframeOnOff.h"
#include "../pattern/factory/background/BackgroundFactory.h"
#include "../pattern/factory/background/IBackground.h"
#include "../pattern/factory/progressBar/ProgressBarFactory.h"
#include "PlatformResV88.h"

using namespace HN;

bool InitialCompositeV88::init() {
    if (!Node::init()) {
        return false;
    }
    
    srand(time(NULL));

//    if (Application::getInstance()->getTargetPlatform() == cocos2d::Application::Platform::OS_IPAD) {
//        setBackGroundImage(LOADING_BG_V88);
//    }
//    else {
//        setBackGroundImage(LOADING_BG_IPHONE_V88);
//    }
    auto bg = Sprite::create(LOADING_BG_V88);
    bg->setPosition(WIN_SIZE / 2);
    addChild(bg);
    
//    std::string path = "v88/loading/loading3.png";
//    IBackground*  healthy = BackgroundFactory::create(path);
//    healthy->setPosition(Vec2(VISIBLE_SIZE.width / 2.0f, healthy->getContentSize().height));
//    addChild(healthy);
    
    auto progressBarLeaf = ProgressBarFactory::create(LOADING_BAR_BG_V88, LOADING_BAR_PRE_V88);
    addChild(progressBarLeaf);
    progressBarLeaf->setPosition(Vec2(WIN_SIZE.width / 2, progressBarLeaf->getContentSize().height));
    
    progressBarDelegate = progressBarLeaf;
    //
    return true;
}

void InitialCompositeV88::updateProgressBar(std::string text, float percent) {
    progressBarDelegate->updateProgressBar(text, percent);
    if (!TESTCASE_ONOFF) {
        Director::getInstance()->getScheduler()->schedule(schedule_selector(InitialCompositeV88::waitting), this, 1.5f, 0, 0.0f, false);
    }
}

void InitialCompositeV88::waitting(float dt) {
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(InitialCompositeV88::waitting), this);
    m_logic->gotoMenu();
}

float InitialCompositeV88::getPercentage() {
    return progressBarDelegate->getPercentage();
}

void InitialCompositeV88::setLogic(InitialLogic* logic) {
    m_logic = logic;
}

void InitialCompositeV88::setBackGroundImage(const std::string &name) {
    auto loadingBG = BackgroundFactory::create(name.c_str());
    loadingBG->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2)); // visibleOrigin +
    float _xScale = VISIBLE_SIZE.width / loadingBG->getContentSize().width;
    float _yScale = VISIBLE_SIZE.height / loadingBG->getContentSize().height;
    loadingBG->setScaleX(_xScale);
    loadingBG->setScaleY(_yScale);
    this->addChild(loadingBG, -1);
}
