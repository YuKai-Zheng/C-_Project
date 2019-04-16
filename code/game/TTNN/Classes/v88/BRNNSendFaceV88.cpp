//#include "BRNNSendFaceV88.h"
//#include "BRNNMsgV88.h"
//
//namespace BRNN {
//SendFaceV88* SendFaceV88::create(bool bEnableSend) {
//	SendFaceV88* ret = new SendFaceV88(bEnableSend);
//	if (ret && ret->init())
//		ret->autorelease();
//	else
//		CC_SAFE_DELETE(ret);
//	return ret;
//}
//
//SendFaceV88::SendFaceV88(bool bEnableSend)
//	: _bEnableSend(bEnableSend)
//	,_scrollview(nullptr)
//	,onSpeakCallBack(nullptr) {
//}
//
//SendFaceV88::~SendFaceV88() {
//}
//
//bool SendFaceV88::init() {
//	if (!MakeBlack::init()) return false;
//
//	auto tmp = Sprite::create(StringUtils::format(BRNN_EMOTION_SE,0));
//	Size layerSize = Size(tmp->getContentSize().width*7, tmp->getContentSize().height*5);
//	this->setContentSize(layerSize);
//	tmp = NULL;
//
//	auto bg = ui::Scale9Sprite::create(BRNN_SOMEBG);
//	bg->setContentSize(Size(layerSize.width+50, layerSize.height+30));
//	bg->setPosition(layerSize.width/2, layerSize.height/2);
//	this->addChild(bg);
//
//	auto slayer = Layer::create();
//	slayer->setContentSize(Size(layerSize.width, layerSize.height*1.2));
//	slayer->setAnchorPoint(Vec2(0, 1));
//	slayer->setPosition(0, layerSize.height);
//	slayer->setTag(100);
//
//	int faceCnt = 0;
//	for(int i=0; i<5; i++) {
//		for(int j=0; j<6; j++) {
//			if(faceCnt > 26)
//				break;
//
//			auto pic = Sprite::create(StringUtils::format(BRNN_EMOTION_SE, faceCnt));
//			pic->setPosition(layerSize.width*(2*j+1)/12, slayer->getContentSize().height*(2*(4-i)+1)/10);
//			pic->setTag(faceCnt);
//			slayer->addChild(pic);
//
//			faceCnt++;
//		}
//	}
//
//	_scrollview = cocos2d::extension::ScrollView::create(layerSize);
//	_scrollview->setAnchorPoint(Vec2(0, 0));
//	_scrollview->setPosition(0, 0);
//	_scrollview->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
//	_scrollview->setContainer(slayer);
//	_scrollview->setContentOffset(Vec2(0, -layerSize.height*1.0/5));
//	this->addChild(_scrollview);
//
//	return true;
//}
//
//void SendFaceV88::onTouchEnded(Touch* touch, Event* event) {
//	if(_bSelfMoved)
//		return;
//
//	Point point = touch->getLocation();
//	auto slayer = _scrollview->getChildByTag(100);
//	if(slayer && !_bMoved) {
//		point = slayer->convertToNodeSpace(point);
//		Vector<Node*> childrenList;
//		childrenList.clear();
//		childrenList = slayer->getChildren();
//		for(auto child : childrenList) {
//			if(child) {
//				if(child->getBoundingBox().containsPoint(point)) {
//					if(_bEnableSend) {
//						//this->removeFromParent();
//						this->moveSelfWithAction();
//						if(onSpeakCallBack)
//							onSpeakCallBack(StringUtils::format("--%d--", child->getTag()));
//					} else {
//						MsgTipV88::create(GBKToUtf8("只有坐下的玩家才能发送表情"));
//					}
//				}
//			}
//		}
//	}
//}
//}
