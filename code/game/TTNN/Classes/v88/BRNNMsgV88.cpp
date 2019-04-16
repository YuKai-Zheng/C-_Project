#include "BRNNMsgV88.h"
#include "FontSize.h"

namespace BRNN {
#define MSG_MOVE_TIME   0.5


MsgTipV88* MsgTipV88::create(std::string msg) {
	MsgTipV88* obj = new MsgTipV88();
	if (obj && obj->init(msg))
		obj->autorelease();
	else
		CC_SAFE_DELETE(obj);
	return obj;
}

bool MsgTipV88::init(std::string msg) {
	if(!HNNode::init())
		return false;

//    float msgLength = msg.size();
//    int textNum = msg.size()/3 + 1;
//    float lengthPerText = 23;
//    float allLength = textNum*lengthPerText;

	/*auto MsgTipV88Bg = Sprite::create("msg/MsgTipV88bg.png");
	   MsgTipV88Bg->setScaleX(allLength/MsgTipV88Bg->getContentSize().width);
	   MsgTipV88Bg->setScaleY(lengthPerText*1.5/MsgTipV88Bg->getContentSize().height);
	   this->addChild(MsgTipV88Bg);*/

	auto msgLabel = Label::createWithTTF(msg, DEFAULT_FONT, 23);
	//msgLabel->setColor(Color3B(0,0,0));
	this->addChild(msgLabel, 1);

	auto MsgTipV88Bg = ui::Scale9Sprite::create(BRNN_MSGTIPBG_V88);
	MsgTipV88Bg->setContentSize(Size(msgLabel->getContentSize().width+20, msgLabel->getContentSize().height+10));
	this->addChild(MsgTipV88Bg);

	auto winSize = Director::getInstance()->getWinSize();
	this->setPosition(winSize.width / 2, winSize.height);

	auto scene = Director::getInstance()->getRunningScene();
	scene->addChild(this, 1000);

	this->runAction(Sequence::create(MoveBy::create(MSG_MOVE_TIME, Point(0, -50)),
	                                 DelayTime::create(4*MSG_MOVE_TIME),
	                                 MoveBy::create(MSG_MOVE_TIME, Point(0, 50)),
	                                 RemoveSelf::create(),
	                                 nullptr));
	return true;
}
}
