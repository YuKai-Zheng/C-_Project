#include "HNLobby/v66/GameGiftV66.h"
#include "HNLobby/PlatformDefine.h"
#include "Sound/HNAudioEngine.h"
#include "FontSize.h"
#include "PlatformResV66.h"
#include "VersionInfo/VersionInfo.h"
#include "UI/Base/AUIButton.h"
#include "../GamePrompt.h"
#include "../GameData.h"

#include "HNLobby/PlatformConfig.h"

using namespace HN;

GameGiftV66::GameGiftV66() {
    //m_bgPath = "";
	m_bMusicOnOff = false;
	m_bEffectOnOff = false;
}

GameGiftV66::~GameGiftV66() {
}


bool GameGiftV66::init() {
    if (!HNLayer::init()) {
        return false;
    }

    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);//阻止触摸向下传递
    myListener->onTouchBegan = [](Touch *touch, Event *unused_event){
        return true;
    };
    myListener->onTouchEnded = [&](Touch *touch, Event *unused_event){
        close();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
    

	_rootNode = CSLoader::createNode("hallimg/main/node_fuli.csb");
	addChild(_rootNode);
	_rootNode->setPosition(WIN_SIZE / 2);
	(_rootNode->getChildByName("Panel_9"))->setScaleX(WIN_SIZE.width / 1280);

	auto closeBtn = (Button*)_rootNode->getChildByName("bg")->getChildByName("btn_close");
	closeBtn->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type){
		if (type != Widget::TouchEventType::ENDED) return;
		close();
	});  
	 
	LLONG nDonateCoin = GameData::getInstance()->getDonateCoin();
	auto btn_get = (Button*)_rootNode->getChildByName("bg")->getChildByName("btn_get");
	auto img_get = (Sprite*)btn_get->getChildByName("Sprite_1");
	auto text = (Text*)_rootNode->getChildByName("bg")->getChildByName("Text_1"); 
	if (nDonateCoin>0)
	{
		img_get->setTexture("hallimg/main/category/images/hallpic/txt_lingqu.png");
		btn_get->loadTextureNormal("csb/newplatform/bank/img/btn_yellow.png");
		btn_get->addTouchEventListener(CC_CALLBACK_2(GameGiftV66::GetMoneyCallBack, this));

		char tempStr[128];
		sprintf(tempStr, "恭喜你获得返点福利大礼包，\n%lld金币", nDonateCoin);
		text->setString(GBKToUtf8(string(tempStr).c_str()));
		text->setTextHorizontalAlignment(TextHAlignment::CENTER);
	}
	else
	{
		img_get->setTexture("hallimg/main/category/images/hallpic/txt_yilingqu.png");
		btn_get->loadTextureNormal("csb/newplatform/bank/img/btn_fei.png");
		text->setString(GBKToUtf8("为了回馈玩家，每周三登陆游戏即可领取返点福利大礼包，充值越多，游戏时长越久，礼包越大哦~"));
		text->setTextHorizontalAlignment(TextHAlignment::LEFT);
	}


    return true;
}


void GameGiftV66::GetMoneyCallBack(Ref* pSender, Widget::TouchEventType event) {
	GameGiftInfo srInfo;
	srInfo.lMoney = GameData::getInstance()->getDonateCoin();
	srInfo.dwUserID = PlatformLogic()->loginResult.dwUserID;
	PlatformLogic()->sendData(MDM_GP_BANK, ASS_GP_GET_GIFT, &srInfo, sizeof(srInfo), HN_SOCKET_CALLBACK(GameGiftV66::getEventSelector, this));
}


void GameGiftV66::open(Node* parent, Vec2 vec, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    setPosition(vec);
    parent->addChild(this, zorder, tag);
    
    //setScale(0.2);
    //runAction(ScaleTo::create(0.2f, 1.0f));
}

void GameGiftV66::close() {
    //runAction(Sequence::create(ScaleTo::create(0.2f, 0.2f), RemoveSelf::create(), nullptr));
	removeFromParent();
}

void GameGiftV66::setBgPath(std::string path) {
 /*   if (path.compare("") != 0) {
        m_bgPath = path;
    }*/
}

bool GameGiftV66::getEventSelector(HNSocketMessage* socketMessage) {
	UINT bAssistantID = socketMessage->messageHead.bAssistantID;
	UINT bHandleCode = socketMessage->messageHead.bHandleCode;
	// 打开银行
	if (ASS_GP_GET_GIFT == bAssistantID) {
		switch (bHandleCode) {
		case HC_GP_GETGIFT_SUC: 
		{ 
			//1		//领取成功
			auto btn_get = (Button*)_rootNode->getChildByName("bg")->getChildByName("btn_get");
			auto img_get = (Sprite*)btn_get->getChildByName("Sprite_1");
			auto text = (Text*)_rootNode->getChildByName("bg")->getChildByName("Text_1");
			GameData::getInstance()->setDonateCoin(-1);
			img_get->setTexture("hallimg/main/category/images/hallpic/txt_yilingqu.png");
			btn_get->loadTextureNormal("csb/newplatform/bank/img/btn_fei.png");
			text->setString(GBKToUtf8("为了回馈玩家，每周三登陆游戏即可领取返点福利大礼包，充值越多，游戏时长越久，礼包越大哦~"));
			text->setTextHorizontalAlignment(TextHAlignment::LEFT);
		}
		break;
		//case HC_GP_GETGIFT_ERR: 
		//{ 
		//	//-1		//领取失败
	
		//}

		//break;
		default:
			break;
		}
	}

	return true;
}