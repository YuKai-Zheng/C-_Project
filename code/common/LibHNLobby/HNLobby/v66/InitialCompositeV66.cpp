//
//  InitialCompositeV66.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 14/03/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "InitialCompositeV66.h"
#include "VersionInfo/VersionInfo.h"
#include "Tool/WireframeOnOff.h"
#include "../pattern/factory/background/BackgroundFactory.h"
#include "../pattern/factory/background/IBackground.h"
#include "../pattern/factory/progressBar/ProgressBarFactory.h"
#include "PlatformResV66.h"

using namespace HN;

bool InitialCompositeV66::init() {
    if (!Node::init()) {
        return false;
    }
    
    srand(time(NULL));

//    if (Application::getInstance()->getTargetPlatform() == cocos2d::Application::Platform::OS_IPAD) {
//        setBackGroundImage(LOADING_BG_V66);
//    }
//    else {
//        setBackGroundImage(LOADING_BG_IPHONE_V66);
//    }
   // auto bg = Sprite::create(LOADING_BG_V66);
   // if (bg != nullptr)
   // {
       // bg->setPosition(WIN_SIZE / 2);
        //addChild(bg);
    //}
//    IBackground*  healthy = BackgroundFactory::create(path);
//    healthy->setPosition(Vec2(VISIBLE_SIZE.width / 2.0f, healthy->getContentSize().height));
//    addChild(healthy);
    setBackGroundImage(LOADING_BG_V66);
    auto progressBarLeaf = ProgressBarFactory::create(LOADING_BAR_BG_V66, LOADING_BAR_PRE_V66);
    addChild(progressBarLeaf);
    progressBarLeaf->setPosition(Vec2(WIN_SIZE.width / 2, progressBarLeaf->getContentSize().height));
    
    progressBarDelegate = progressBarLeaf;

	Tools::PlaySkeletonAnimation(this, "csb/newplatform/login/denglu/person_atlas_spien/", "dl_chaifen_renwu", -1, Vec2(510.0f, 330.0f));
    //
    return true;
}

void InitialCompositeV66::updateProgressBar(std::string text, float percent) {
    progressBarDelegate->updateProgressBar(text, percent);
    //if (!TESTCASE_ONOFF) {
    //    Director::getInstance()->getScheduler()->schedule(schedule_selector(InitialCompositeV66::waitting), this, 1.5f, 0, 0.0f, false);
    //}
}

void InitialCompositeV66::waitting(float dt) {
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(InitialCompositeV66::waitting), this);
    m_logic->gotoMenu();
}

float InitialCompositeV66::getPercentage() {
    return progressBarDelegate->getPercentage();
}

void InitialCompositeV66::setLogic(InitialLogic* logic) {
    m_logic = logic;
}

void InitialCompositeV66::setBackGroundImage(const std::string &name) {
    auto loadingBG = BackgroundFactory::create(name.c_str());
    loadingBG->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2)); // visibleOrigin +
    //float _xScale = VISIBLE_SIZE.width / loadingBG->getContentSize().width;
    //float _yScale = VISIBLE_SIZE.height / loadingBG->getContentSize().height;
    //loadingBG->setScaleX(_xScale);
    //loadingBG->setScaleY(_yScale);
    this->addChild(loadingBG, -1);
}
