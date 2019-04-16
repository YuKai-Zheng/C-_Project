//#include "BRNNGiveGiftV88.h"
//#include "BRNNMsgV88.h"
//#include "BRNNMessageHead.h"
//
//namespace BRNN {
//#define GIFT_ACTION_TIME        0.2
//#define GIFT_FONT                       ""
//#define GIFT_FONTSIZE           25
//
//GiveGiftV88* GiveGiftV88::create(UserInfoStruct* user) {
//	GiveGiftV88* ret = new GiveGiftV88();
//	if (ret && ret->init(user))
//		ret->autorelease();
//	else
//		CC_SAFE_DELETE(ret);
//	return ret;
//}
//
//GiveGiftV88::GiveGiftV88()
//	: _user(nullptr) {
//	_giftPriceArr[0] = 1000;
//	_giftPriceArr[1] = 1000;
//	_giftPriceArr[2] = 10000;
//	_giftPriceArr[3] = 100000;
//	_giftPriceArr[4] = 500000;
//	_giftPriceArr[5] = 5000000;
//	_vecGiftSprite.clear();
//}
//
//GiveGiftV88::~GiveGiftV88() {
//}
//
//bool GiveGiftV88::init(UserInfoStruct* user) {
//	if (!MakeBlackV88::init()) return false;
//
//	_user = user;
//
//	auto bg = Sprite::create(BRNN_GIFT_BG);
//	auto bgSize = bg->getContentSize();
//	this->setContentSize(bgSize);
//
//	bg->setPosition(bgSize.width/2, bgSize.height/2);
//	this->addChild(bg);
//
//	if(_user) {
//		//head
//		int headId = _user->bLogoID;
//		if(headId < 0 || headId > 4)
//			headId = 0;
//		auto head = Sprite::create(StringUtils::format(PLAYER_HEAD_V88, headId));
//		if(head) {
//			head->setPosition(bgSize.width*98.0/664, bgSize.height*250.0/346);
//			this->addChild(head);
//		}
//
//		auto name = Label::createWithTTF(GBKToUtf8(_user->nickName), GIFT_FONT, GIFT_FONTSIZE);
//		name->setAnchorPoint(Vec2(0, 0.5));
//		name->setPosition(bgSize.width*330.0/664, bgSize.height*300.0/346);
//		this->addChild(name);
//		auto id = Label::createWithTTF(StringUtils::format("%d", _user->dwUserID), GIFT_FONT, GIFT_FONTSIZE);
//		id->setAnchorPoint(Vec2(0, 0.5));
//		id->setPosition(bgSize.width*330.0/664, bgSize.height*248.0/346);
//		this->addChild(id);
//		auto money = Label::createWithTTF(StringUtils::format("%lld", _user->i64Money), GIFT_FONT, GIFT_FONTSIZE);
//		money->setAnchorPoint(Vec2(0, 0.5));
//		money->setPosition(bgSize.width*330.0/664, bgSize.height*188.0/346);
//		this->addChild(money);
//	}
//
//	for(int i=0; i<6; i++) {
//		auto gift = Sprite::create(StringUtils::format(BRNN_GIFT, i));
//		gift->setPosition(bgSize.width*(72.0+103.0*i)/664, bgSize.height*92.0/346);
//		this->addChild(gift);
//		_vecGiftSprite.pushBack(gift);
//
//		auto price = Label::createWithTTF(StringUtils::format("%d", _giftPriceArr[i]), "", 22);
//		price->setPosition(gift->getPositionX(), bgSize.height*45.0/346);
//		this->addChild(price);
//	}
//
//	return true;
//}
//
//void GiveGiftV88::onTouchEnded(Touch* touch, Event* event) {
//	if(_bSelfMoved)
//		return;
//
//	Point point = touch->getLocation();
//	point = this->convertToNodeSpace(point);
//	log("point x, y = %f, %f", point.x, point.y);
//	for(int i=0; i<_vecGiftSprite.size(); i++) {
//		if(_vecGiftSprite.at(i)->getBoundingBox().containsPoint(point)) {
//			if(_user->dwUserID == PlatformLogic()->loginResult.dwUserID) {
//				MsgTipV88::create(GBKToUtf8("礼物是用来送人的"));
//				return;
//			}
//			if(PlatformLogic()->loginResult.i64Money < _giftPriceArr[i]) {
//				MsgTipV88::create(GBKToUtf8("您的筹码不够"));
//				return;
//			}
//
//			MSG_PROP_GIVE gift;
//			gift.dwUserID = PlatformLogic()->loginResult.dwUserID;
//			gift.dwTargetID = _user->dwUserID;
//			gift.dwCost = _giftPriceArr[i];
//			gift.nCount = 1;
//			gift.nPropID = i;
//			HNRoomLogic::getInstance()->sendData(MDM_GR_PROP, ASS_PROP_GIVE, &gift, sizeof(gift));
//
//		}
//	}
//}
//}
