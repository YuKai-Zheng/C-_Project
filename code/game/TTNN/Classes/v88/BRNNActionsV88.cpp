#include "BRNNActionsV88.h"

namespace BRNN {
ActionInterval * ActionsV88::FrameAnimation(const char * pFile, int frameCount, float frameTime, bool bMoveSelf /* = false*/) {
	Sprite * objectImage = Sprite::create(StringUtils::format(pFile, 1));
	Vector<SpriteFrame*> vecFrame;
	for (int i = 1; i <= frameCount; i++) {
		SpriteFrame* frame = SpriteFrame::create(StringUtils::format(pFile, i).c_str(),
		                                         Rect(0, 0, objectImage->getContentSize().width, objectImage->getContentSize().height));
		vecFrame.pushBack(frame);
	}
	Animation * flashMove = Animation::createWithSpriteFrames(vecFrame, frameTime);
	if (bMoveSelf)
		return Sequence::create(Animate::create(flashMove), RemoveSelf::create(), nullptr);
	return Sequence::create(Animate::create(flashMove), nullptr);
}

ActionInterval* ActionsV88::LoomingAction(float fDuration, unsigned int times) {
	auto a1 = FadeIn::create(fDuration);
	auto a2 = FadeOut::create(fDuration);
	return Repeat::create(Sequence::create(a1, a2, nullptr), times);
}

ActionInterval* ActionsV88::SmallToBig(float fDuration /* = 0.3*/) {
	return ScaleTo::create(fDuration, 1);
}

ActionInterval* ActionsV88::SmallAndRemove(float fDuration /* = 0.3*/) {
	return Sequence::create(ScaleTo::create(fDuration, 0), RemoveSelf::create(), nullptr);
}

void ActionsV88::actionIn(Node* target) {
	auto winSize = Director::getInstance()->getWinSize();
	//int actionId = rand()%6;
	int actionId = 0;
	switch (actionId) {
	case 0: { //小
		target->setScale(0.0f);
		target->runAction(ScaleTo::create(MAKEBLACK_IN_ACTION_TIME, 1.0f));
	}
	break;
	case 1: { //大
		target->setScale(3);
		target->runAction(ScaleTo::create(MAKEBLACK_IN_ACTION_TIME, 1.0f));
	}
	break;
	case 2: { //上
		target->runAction(Sequence::create(Place::create(Point(target->getPositionX(), winSize.height + target->getContentSize().height / 2)),
		                                   MoveBy::create(MAKEBLACK_IN_ACTION_TIME, Point(0, -(winSize.height / 2 + target->getContentSize().height / 2))),
		                                   nullptr));
	}
	break;
	case 3: { //下
		target->runAction(Sequence::create(Place::create(Point(target->getPositionX(), -target->getContentSize().height / 2)),
		                                   MoveBy::create(MAKEBLACK_IN_ACTION_TIME, Point(0, winSize.height / 2 + target->getContentSize().height / 2)),
		                                   nullptr));
	}
	break;
	case 4: { //左
		target->runAction(Sequence::create(Place::create(Point(-target->getContentSize().width / 2, target->getPositionY())),
		                                   MoveBy::create(MAKEBLACK_IN_ACTION_TIME, Point(winSize.width / 2 + target->getContentSize().width / 2, 0.0f)),
		                                   nullptr));
	}
	break;
	case 5: { //右
		target->runAction(Sequence::create(Place::create(Point(winSize.width + target->getContentSize().width / 2, target->getPositionY())),
		                                   MoveBy::create(MAKEBLACK_IN_ACTION_TIME, Point(-(winSize.width / 2 + target->getContentSize().width / 2), 0.0f)),
		                                   nullptr));
	}
	break;
	}
}

void ActionsV88::actionOut(Node* target) {
	//int actionId = rand()%6;
	int actionId = 0;
	switch (actionId) {
	case 0: { //小
		target->runAction(Sequence::create(ScaleTo::create(MAKEBLACK_OUT_ACTION_TIME, 0.0f), RemoveSelf::create(), nullptr));
	}
	break;
	case 1: { //大
		target->runAction(Sequence::create(ScaleTo::create(MAKEBLACK_IN_ACTION_TIME, 3.0f), RemoveSelf::create(), nullptr));
	}
	break;
	case 2: { //上
		target->runAction(Sequence::create(MoveBy::create(MAKEBLACK_IN_ACTION_TIME, Point(0.0f, target->getPositionY() + target->getContentSize().height / 2)),
		                                   RemoveSelf::create(),
		                                   nullptr));
	}
	break;
	case 3: { //下
		target->runAction(Sequence::create(MoveBy::create(MAKEBLACK_IN_ACTION_TIME, Point(0.0f, -(target->getPositionY() + target->getContentSize().height / 2))),
		                                   RemoveSelf::create(),
		                                   nullptr));
	}
	break;
	case 4: { //左
		target->runAction(Sequence::create(MoveBy::create(MAKEBLACK_IN_ACTION_TIME, Point(-(target->getPositionX() + target->getContentSize().width / 2), 0.0f)),
		                                   RemoveSelf::create(),
		                                   nullptr));
	}
	break;
	case 5: { //右
		target->runAction(Sequence::create(MoveBy::create(MAKEBLACK_IN_ACTION_TIME, Point(target->getPositionX() + target->getContentSize().width / 2, 0.0f)),
		                                   RemoveSelf::create(),
		                                   nullptr));
	}
	break;
	}
}

}
