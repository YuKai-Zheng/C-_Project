#include "InitialCompositeV100.h"
#include "VersionInfo/VersionInfo.h"
#include "Tool/WireframeOnOff.h"
#include "../pattern/factory/background/BackgroundFactory.h"
#include "../pattern/factory/progressBar/ProgressBarFactory.h"
using namespace HN;

bool InitialCompositeV100::init() {
    if (!Node::init()) {
        return false;
    }

    std::string filename = "platform/lodingUI/gd_Loding.png";
    setBackGroundImage(filename);
    //
    std::string path = "platform/lodingUI/loading3.png";
    IBackground*  healthy = BackgroundFactory::create(path);
    healthy->setPosition(Vec2(VISIBLE_SIZE.width / 2.0f, healthy->getContentSize().height));
    addChild(healthy);

    progressBarLeaf = ProgressBarFactory::create("platform/lodingUI/loading2.png", "platform/lodingUI/gd_bar.png");
    addChild(progressBarLeaf);
    progressBarLeaf->setPosition(healthy->getPosition() + Vec2(0, progressBarLeaf->getContentSize().height));
    progressBarLeaf->setProgressBarOffsetY(-10);
    //
    return true;
}

void InitialCompositeV100::updateProgressBar(std::string text, float percent) {
    progressBarLeaf->updateProgressBar(text, percent);

    if (!TESTCASE_ONOFF) {
        Director::getInstance()->getScheduler()->schedule(schedule_selector(InitialCompositeV100::waitting), this, 1.5f, 0, 0.0f, false);
    }
}

void InitialCompositeV100::waitting(float dt) {
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(InitialCompositeV100::waitting), this);
    m_logic->gotoMenu();
}

float InitialCompositeV100::getPercentage() {
    return progressBarLeaf->getPercentage();
}

void InitialCompositeV100::setLogic(InitialLogic* logic) {
    m_logic = logic;
}

void InitialCompositeV100::setBackGroundImage(const std::string &name) {
    auto loadingBG = BackgroundFactory::create(name.c_str());
    loadingBG->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2)); // visibleOrigin +
    float _xScale = VISIBLE_SIZE.width / loadingBG->getContentSize().width;
    float _yScale = VISIBLE_SIZE.height / loadingBG->getContentSize().height;
    loadingBG->setScaleX(_xScale);
    loadingBG->setScaleY(_yScale);
    this->addChild(loadingBG, -1);
}
