#include "InitialCompositeV1.h"
#include "VersionInfo/VersionInfo.h"
#include "Tool/WireframeOnOff.h"
#include "../pattern/factory/background/BackgroundFactory.h"
#include "../pattern/factory/background/IBackground.h"
#include "../pattern/factory/progressBar/ProgressBarFactory.h"
using namespace HN;

bool InitialCompositeV1::init() {
    if (!Node::init()) {
        return false;
    }

    srand(time(NULL));
    int _bgIdx = rand() % 2;
    char strPath[100] = { 0 };
	if (GD_NEED_OPEN)
	{
		sprintf(strPath, "%s", "platform/lodingUI/gd_Loding.png");
	}
	else
	{
		sprintf(strPath, "platform/lodingUI/loadingBg%d.png", _bgIdx);
	}
	//bg
    setBackGroundImage(strPath);

	//健康公告
	IBackground*  healthy = BackgroundFactory::create("platform/lodingUI/loading3.png");
    healthy->setPosition(Vec2(VISIBLE_SIZE.width / 2.0f, healthy->getContentSize().height));
    addChild(healthy);

	//loadingbar
	if (GD_NEED_OPEN)
	{
		sprintf(strPath, "%s", "platform/lodingUI/gd_bar.png");
	}
	else
	{
		sprintf(strPath, "%s", "platform/lodingUI/loading1.png");
	}
    auto progressBarLeaf = ProgressBarFactory::create("platform/lodingUI/loading2.png", strPath);
    addChild(progressBarLeaf);
    progressBarLeaf->setPosition(healthy->getPosition() + Vec2(0, progressBarLeaf->getContentSize().height));

    progressBarDelegate = progressBarLeaf;

    return true;
}

void InitialCompositeV1::updateProgressBar(std::string text, float percent) {
    progressBarDelegate->updateProgressBar(text, percent);
    if (!TESTCASE_ONOFF) {
		if (GD_NEED_OPEN)
		{
			Director::getInstance()->getScheduler()->schedule(schedule_selector(InitialCompositeV1::waitting), this, 10.0f, 0, 0.0f, false);
		}
		else
		{
			Director::getInstance()->getScheduler()->schedule(schedule_selector(InitialCompositeV1::waitting), this, 1.5f, 0, 0.0f, false);
		}
    }
}

void InitialCompositeV1::waitting(float dt) {
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(InitialCompositeV1::waitting), this);
    m_logic->gotoMenu();
}

float InitialCompositeV1::getPercentage() {
    return progressBarDelegate->getPercentage();
}

void InitialCompositeV1::setLogic(InitialLogic* logic) {
    m_logic = logic;
}

void InitialCompositeV1::setBackGroundImage(const std::string &name) {
    auto loadingBG = BackgroundFactory::create(name.c_str());
    loadingBG->setPosition(Vec2(VISIBLE_SIZE.width / 2, VISIBLE_SIZE.height / 2)); // visibleOrigin +
    float _xScale = VISIBLE_SIZE.width / loadingBG->getContentSize().width;
    float _yScale = VISIBLE_SIZE.height / loadingBG->getContentSize().height;
    loadingBG->setScaleX(_xScale);
    loadingBG->setScaleY(_yScale);
    this->addChild(loadingBG, -1);
}
