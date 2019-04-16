#include "ProgressBarLeaf.h"
#include "VersionInfo/VersionInfo.h"

ProgressBarLeaf* ProgressBarLeaf::create(std::string progressBg, std::string progressBar) {
    ProgressBarLeaf *pRet = new(std::nothrow) ProgressBarLeaf();
    if (pRet && pRet->init(progressBg, progressBar)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ProgressBarLeaf::init(std::string progressBg, std::string progressBar) {
    if (!Node::init()) {
        return false;
    }

    background = Sprite::create(progressBg);
    this->addChild(background);
    setContentSize(background->getContentSize());

    _loadProgress = ProgressTimer::create(Sprite::create(progressBar)); //创建一个进程条
    _loadProgress->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loadProgress->setBarChangeRate(Vec2(1, 0)); //设置进程条的变化速率
    _loadProgress->setType(ProgressTimer::Type::BAR); //设置进程条的类型
    _loadProgress->setMidpoint(Vec2(0, 1)); //设置进度的运动方向
    _loadProgress->setPercentage(0.0f); //设置初始值为0
    this->addChild(_loadProgress);

    _label = Label::createWithTTF("", "platform/fonts/simhei.ttf", 26,Size::ZERO, TextHAlignment::CENTER,TextVAlignment::CENTER);
    _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(_label);
    _loadProgress->setPosition(Vec2(background->getPositionX() , background->getPositionY() + (background->getContentSize().height - _loadProgress->getContentSize().height) / 2));
    _label->setPosition(Vec2(_loadProgress->getPosition().x, _loadProgress->getPosition().y + 4.0f));

	if (GD_NEED_OPEN)
	{
		_loadProgress->setPosition(background->getPositionX(), background->getPositionY() + 5);
		_label->setPosition(Vec2(_loadProgress->getPosition().x, _loadProgress->getPosition().y + 4.0f));
	}

    return true;
}

void ProgressBarLeaf::setProgressBarOffsetY(float offsetY) {
    _loadProgress->setPositionY(_loadProgress->getPositionY() + offsetY);
    _label->setPositionY(_loadProgress->getPositionY());
}

void ProgressBarLeaf::setText(std::string text) {
    _label->setString(text);
}

void ProgressBarLeaf::updateProgressBar(std::string text, float percentage) {
    setText(text);
    setPercentage(percentage);
}

float ProgressBarLeaf::getPercentage() {
    return _percentage;
}

//const Size& ProgressBarLeaf::getContentSize() {
//    if (background) {
//        return background->getContentSize();
//    }
//    return Size::ZERO;
//}

void ProgressBarLeaf::setPercentage(float percentage) {
    _percentage = percentage;
    _loadProgress->setPercentage(percentage);
}
