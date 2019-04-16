#include "BottomMenuCompositeV66.h"
#include "HNLobby/PersionalCenter/GameUserDataLayer.h"
#include "HNLobby/ChildLayer/GameOnlineReward.h"
#include "HNLobby/ChildLayer/GameRechargeRebate.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"
//#include "UMeng/Cocos2dx/Common/CCUMTypeDef.h"
#include "../pattern/CommandManager.h"
//#include "UMeng/Cocos2dx/Common/CCUMSocialSDK.h"
#include "PlatformResV66.h"
#include "../ChildLayer/GameStoreLayer.h"
#include "../pattern/factory/gameBank/GameBankFactory.h"
#include "../pattern/factory/gameShop/GameShopFactory.h"
#include "../pattern/factory/gameSet/GameSetFactory.h"
#include "../pattern/factory/gameGift/GameGiftFactory.h"
#include "../pattern/factory/gameRank/GameRankFactory.h"
#include "Sound/HNAudioEngine.h"
#include "HNLobby/v66/InvitedCodeLayerV66.h"
#include "../pattern/factory/gameUser/IGameUser.h"
#include "../pattern/factory/gameUser/GameUserFactory.h"
#include "HNLobby/v66/GameUserV66.h"
//USING_NS_UM_SOCIAL;
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/GameData.h"

#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>

#include "HNLobby/v66/PopularizeSignManager.h"
#include "HNLobby/v66/WelfareManager.h"
#include "HNLobby/v66/RechargeManager.h"

#include "GameBankEnsureV66.hpp"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/ClipboardHelper.h"
#endif

#define BTN_RANK_RES    "hallimg/main/paihangb.png"

const char* BTN_QUICK_JOIN_ANIMAION = "v66/animation/main/button/anniu.csb";
const char*	BTN_BANK_ANIMATION = "v66/animation/main/button/bank.csb";
const char*	BTN_RANK_ANIMATION = "v66/animation/main/button/charts.csb";
const char*	BTN_ACTIVITY_ANIMATION = "v66/animation/luckyturntable/luckyturntable_logo/luckyturntable_logo.csb";
const char*	BTN_SET_ANIMATION = "v66/animation/main/button/setup.csb";
const char*	BTN_CHARGE_ANIMATION = "v66/animation/main/button/mall.csb";


static const char* GAME_TIP_PATH = "v66/room/changci/%d.png";


#define btn_copy_tag        "Button_copy"
#define btn_popularize_tag   "Button_tuijian"
#define btn_charge_tag      "shopBtn"

#define btn_bank_tag        "Button_bank"
#define btn_setup_tag       "Button_set"
#define btn_join_tag       "Button_join"
#define btn_customer_tag    "Button_customer"
#define btn_shop_tag        "Button_shop"




BottomMenuCompositeV66::BottomMenuCompositeV66() {
	l_onlinePeople = nullptr;
	_csbRoot = nullptr;

}

BottomMenuCompositeV66::~BottomMenuCompositeV66() {
	PlatformLogic()->removeEventSelector(MDM_GP_MESSAGE, ASS_GP_ONLINE_USER_COUNT);
//    PlatformLogic()->removeEventSelector(MDM_GP_TRANS_SUCCESS_TIP, ASS_GP_TRANS_SUCCESS_TIP);
}


bool BottomMenuCompositeV66::init() {
    if (!Node::init()) {
        return false;
    }
    _isTouch=true;
    _isQuickTouch = true;
    
   //_csbRoot = CSLoader::createNode(PLATFORM_BOTTOM_MENU_V66);
    _csbRoot = CSLoader::createNode("csb/v66/BottomMenu.csb");
	addChild(_csbRoot);
    
    Layout* layout = (Layout *)_csbRoot->getChildByName("layout");
    setContentSize(layout->getContentSize());
    
    //Layout* layoutLeft = (Layout *)layout->getChildByName("bg")->getChildByName("Panel_left");
    //Layout* layoutRight = (Layout *)layout->getChildByName("bg")->getChildByName("Panel_right");
    
    vector<bool> vIsModuleOpen;  
    vIsModuleOpen.push_back(true);
    vIsModuleOpen.push_back(true);
    vIsModuleOpen.push_back(true);
    vIsModuleOpen.push_back(true);
    vIsModuleOpen.push_back(true);
	vIsModuleOpen.push_back(true);
	vIsModuleOpen.push_back(true);
    
	//注意插入的顺序;
    vector<string> vModuleImgName;   
    vModuleImgName.push_back(btn_setup_tag);   
	vModuleImgName.push_back(btn_bank_tag);
    vModuleImgName.push_back(btn_customer_tag);

	vModuleImgName.push_back(btn_join_tag);
    vModuleImgName.push_back(btn_shop_tag);
	vModuleImgName.push_back("Button_rank");
	vModuleImgName.push_back("Button_gift");
	
   
	for (int i = 0; i < vModuleImgName.size(); i++)
	{
		auto btn = (Button*)layout->getChildByName("bg")->getChildByName(vModuleImgName.at(i));
		//if (i <= 2) {
		//	btn = (Button*)layoutLeft->getChildByName(vModuleImgName.at(i));
		//}

		if (nullptr == btn)
		{
			log("TopMenu Button %s Not Found!", vModuleImgName.at(i).data());
			continue;
		}

		btn->setName(vModuleImgName.at(i));
		if (!vIsModuleOpen.at(i))
		{
			btn->setBright(false);
			btn->setTouchEnabled(false);
		}

		btn->setVisible(vIsModuleOpen.at(i));
		btn->setShowEffect(false);
		btn->setName(vModuleImgName.at(i));
		btn->setPressedActionEnabled(false);
		btn->addTouchEventListener(CC_CALLBACK_2(BottomMenuCompositeV66::menuLobbyBottomCallBack, this));

		m_bottomButton.push_back(btn);

		if ( vModuleImgName.at(i).compare("Button_gift") == 0 )
		{
			Tools::PlayFrameAnimation(btn,"csb/v66/RoundBtnEffect.csb",Vec2(-42.7,10.0f));
		}

		if (vModuleImgName.at(i).compare("Button_rank") == 0)
		{
			Tools::PlayFrameAnimation(btn, "csb/v66/RoundBtnEffect.csb", Vec2(-38.7, 10.0f));
		}

		if (vModuleImgName.at(i).compare(btn_join_tag) == 0)
		{
			/*auto node_effect = CSLoader::createNode("csb/newplatform/animationnode/node_begineffect.csb");
			if (node_effect) {
				node_effect->setPosition(btn->getContentSize() / 2);
				btn->addChild(node_effect);

				auto action = CSLoader::createTimeline("csb/newplatform/animationnode/node_begineffect.csb");
				if (action) {
					node_effect->runAction(action);
					action->gotoFrameAndPlay(0);
				}
			}*/
		}

		if (vModuleImgName.at(i).compare(btn_shop_tag) == 0)
		{
			/*
			auto node_effect = CSLoader::createNode("csb/newplatform/animationnode/node_shangcheng.csb");
			if (node_effect) {
				node_effect->setPosition(btn->getContentSize() / 2);
				btn->addChild(node_effect);

				auto action = CSLoader::createTimeline("csb/newplatform/animationnode/node_shangcheng.csb");
				if (action) {
					node_effect->runAction(action);
					action->gotoFrameAndPlay(0);
				}
			}
			*/
			Tools::PlaySkeletonAnimation(btn, "v66/animation/main/btnshopatlas/", "btn_chongzhizhongxin",0,Vec2(136,-5.7));
		}

		if (vModuleImgName.at(i).compare("Button_gift") == 0)
		{/*
			auto node_effect = CSLoader::createNode("csb/v66/node_gifteffect.csb");
			if (node_effect) {
				node_effect->setPosition(btn->getContentSize() / 2);
				btn->addChild(node_effect);

				auto action = CSLoader::createTimeline("csb/v66/node_gifteffect.csb");
				if (action) {
					node_effect->runAction(action);
					action->gotoFrameAndPlay(0);
				}
			}*/
		}

		if (vModuleImgName.at(i).compare("Button_rank") == 0)
		{
			//排行榜界面入口
			auto rankBtn = (Button*)layout->getChildByName("bg")->getChildByName(vModuleImgName.at(i));
			if (rankBtn) {
				//string jsonpath(string("v66/module/paihangbang/ani/btn_paihangbang.json"));
				//string atlaspath(string("v66/module/paihangbang/ani/btn_paihangbang.atlas"));
				//auto skeletonNode = spine::SkeletonAnimation::createWithJsonFile(jsonpath, atlaspath);
				//if (skeletonNode) {
					//skeletonNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

					//Vec2 pp(40, -120);
					//skeletonNode->setScale(0.8);
					//skeletonNode->setPosition(pp);
					//skeletonNode->setAnimation(0, "btn_paihangbang", true);
					//rankBtn->addChild(skeletonNode);
				//}

				rankBtn->addTouchEventListener(CC_CALLBACK_2(BottomMenuCompositeV66::menuLobbyBottomCallBack, this));
			}
		}

        
		//delete by qiuyx
        //if (vModuleImgName.at(i).compare(btn_join_tag) == 0) {
        //    auto static_Pos = btn->getPosition();
        //    btn->runAction(RepeatForever::create(Sequence::create(RotateBy::create(4.0f,360), Spawn::create(RotateBy::create(1.0f,90),MoveTo::create(0.5f,Vec2(static_Pos.x,static_Pos.y + 50)),NULL) ,Spawn::create(RotateBy::create(1.0f,90),MoveTo::create(0.5f,Vec2(static_Pos.x,static_Pos.y)),NULL) ,NULL)));
        //}
    }
    
#if 0  //delete by qiuyx 大厅不要这些特效了
    auto effectParentNode = dynamic_cast<Node*>(layoutLeft->getChildByName("Node_effectLeft"));
    if (effectParentNode) {
        auto node_action = CSLoader::createNode("csb/v66/Node_bottomEffect.csb");
        if (node_action) {
            node_action->setPosition(Vec2(0,0));
            effectParentNode->addChild(node_action);
            
            auto action = CSLoader::createTimeline("csb/v66/Node_bottomEffect.csb");
            if (action) {
                node_action->runAction(action);
                action->gotoFrameAndPlay(0,false);
                action->setLastFrameCallFunc([=](){
                    effectParentNode->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=](){
                        
                        action->gotoFrameAndPlay(0,false);
                    }), NULL));
                });
            }
        }
        
    }
    auto effectParentNode2 = dynamic_cast<Node*>(layoutRight->getChildByName("Node_effectRight"));
    if (effectParentNode2) {
        auto node_action = CSLoader::createNode("csb/v66/Node_bottomEffect2.csb");
        if (node_action) {
            node_action->setPosition(Vec2(0,0));
            effectParentNode2->addChild(node_action);
            
            auto action = CSLoader::createTimeline("csb/v66/Node_bottomEffect2.csb");
            if (action) {
                node_action->runAction(action);
                action->gotoFrameAndPlay(0,false);
                action->setLastFrameCallFunc([=](){
                    effectParentNode2->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=](){
                        
                        action->gotoFrameAndPlay(0,false);
                    }), NULL));
                });
            }
        }
        
    }
#endif

	//适配处理，调整按钮位置及缩放背景图
	auto bg_0 = (Layout*)layout->getChildByName("bg_0");
	auto bgParent = (Layout*)layout->getChildByName("bg");
	if (bgParent != nullptr && bg_0 != nullptr)
	{
		float fScale = WIN_SIZE.width / 1600;
	
		bg_0->setScaleX(fScale);
		auto sizecon = bgParent->getContentSize();
		bgParent->setContentSize(Size(WIN_SIZE.width, sizecon.height));

		float fPositionXRightBegin = WIN_SIZE.width - 108.0;//128是游戏种类按钮底图的宽度;
		float fSpaceRight = 70;
		std::vector<Node*> listRightBtn;
		listRightBtn.push_back(bgParent->getChildByName("Button_join"));
		listRightBtn.push_back(bgParent->getChildByName(btn_shop_tag));
		listRightBtn.push_back(bgParent->getChildByName("Button_rank"));
		listRightBtn.push_back(bgParent->getChildByName("Button_gift"));

		float fPositionXLeftBegin = 25;
		float fSpaceLeft = 70;
		std::vector<Node*> listLeftBtn;
		listLeftBtn.push_back(bgParent->getChildByName(btn_setup_tag));
		listLeftBtn.push_back(bgParent->getChildByName(btn_bank_tag));
		listLeftBtn.push_back(bgParent->getChildByName(btn_customer_tag));

		Size sSize1 = Tools::GetNodesContentSize(listRightBtn);
		Size sSize2 = Tools::GetNodesContentSize(listLeftBtn);
		float fWidth1 = sSize1.width + sSize2.width + 108.0;//所有按钮的宽度;
		float fScale1 = Tools::CalcScaleValue(1600, WIN_SIZE.width, fWidth1);

		Tools::AutomaticLayout(listRightBtn, fPositionXRightBegin, fSpaceRight*fScale1, Tools::EALT_HORIZONTAL, Tools::ELDT_MINUS_DIRECTION,Tools::ELAT_EQUIDISTANCE);

		Tools::AutomaticLayout(listLeftBtn, fPositionXLeftBegin, fSpaceLeft*fScale1, Tools::EALT_HORIZONTAL, Tools::ELDT_DUE_DIRECTION, Tools::ELAT_EQUIDISTANCE);
	}
	
   
    scheduleOnce(schedule_selector(BottomMenuCompositeV66::activityTime), 0.2);
	scheduleUpdate();
    
	return true;
}

void BottomMenuCompositeV66::moveOut(bool isOut) {
    int k = isOut ? -1 : 1;
    if (!isOut) {
        this->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, this->getContentSize().height * k * 1.1)), nullptr));
    } else {
        this->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, this->getContentSize().height * k * 1.1)), nullptr));
    }
}

void BottomMenuCompositeV66::walletChanged(LLONG money) {

}

void BottomMenuCompositeV66::openNewYearActivity()
{
	if (platformDelegate) {
		platformDelegate->openNewYearActivity();
	}
}

void BottomMenuCompositeV66::activityTime(float dt)
{
	MSG_GR_C2S_TurnTableRequest pTurnData;

	pTurnData.nUserID = PlatformLogic()->getInstance()->loginResult.dwUserID;
	PlatformLogic()->sendData(MDM_GP_LOGON, ASS_GP_LOGON_ISTURNTABLE_TODAY, &pTurnData, sizeof(pTurnData), HN_SOCKET_CALLBACK(BottomMenuCompositeV66::turnplateForwardSelector, this));
}

void BottomMenuCompositeV66::menuLobbyBottomCallBack(Ref *pSender, Widget::TouchEventType type) 
{
	auto btn = (Button *)pSender;
	if (Widget::TouchEventType::ENDED != type /*|| !_isTouch*/)
	{
		if (Widget::TouchEventType::BEGAN == type)
		{
			btn->setScale(0.9);
		}
		else if (Widget::TouchEventType::CANCELED == type)
		{
			btn->setScale(1.0);
		}

		return;
	}
    
	btn->setScale(1.0);
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

    std::string name(btn->getName());
    if (name.compare(btn_join_tag) == 0)
	{ //快速加入
        if (_isQuickTouch)
		{
            UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN,true);
            quickJoinGame();
            _isQuickTouch = false;
            runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([&](){
                _isQuickTouch = true;
                }), nullptr));
        }
        
    }  
	else if (name.compare(btn_bank_tag) == 0)
	{ //银行       
        setOpenBankFunc();
    } 
	else if (name.compare(btn_shop_tag) == 0)
	{ //商店
        /*IGameShop* gameShop =  GameShopFactory::createWithVersion();
        gameShop->setMenuDelegate(topMenuDelegate);
        gameShop->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V66, 0);*/
		auto rechargeManager = RechargeManager::create();
		if (rechargeManager)
		{
			rechargeManager->openAddToParent((Layer*)this->getParent(), MODULE_LAYER_ORDER_V66, WIN_SIZE / 2);
		}
	} 
	else if (name.compare(btn_customer_tag) == 0)
	{
		/*
        MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
        string url1 = StringUtils::format("http://live.game5186.net/live.aspx?UserID=%d",LogonResult.dwUserID);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        Application::getInstance()->openURL(url1);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        Application::getInstance()->openURL(url1);
#endif
		*/
		auto popularizemanager = PopularizeSignManager::create();
		if (popularizemanager)
		{
			popularizemanager->openAddToParent((Layer*)this->getParent(), MODULE_LAYER_ORDER_V66, WIN_SIZE / 2);
		}

	}
	else if (name.compare(btn_setup_tag) == 0)
	{ //设置
       IGameSet* gameSet = GameSetFactory::createWithVersion();
       gameSet->setBgPath(GAME_BACKGROUND_MUSIC_V66);
       gameSet->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V66, 0);
	}
	else if (name.compare("Button_gift") == 0)
	{	//福利
		/*
		IGameGift* gameGift = GameGiftFactory::createWithVersion();
		//gameGift->setBgPath(GAME_BACKGROUND_MUSIC_V66);
		gameGift->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V66, 0);*/

		/*auto popularizemanager = PopularizeSignManager::create();
		if (popularizemanager)
		{
			popularizemanager->openAddToParent((Layer*)this->getParent(), MODULE_LAYER_ORDER_V66, WIN_SIZE / 2);
		}*/

		auto welfareManager = WelfareManager::create();
		if (welfareManager)
		{
			welfareManager->openAddToParent((Layer*)this->getParent(), MODULE_LAYER_ORDER_V66, WIN_SIZE / 2);
		}
	}
	else if (name.compare("Button_rank") == 0)
	{
		// 排行榜
		auto gameRank = GameRankFactory::createWithVersion();
		CC_ASSERT(gameRank != nullptr);

		if (gameRank != nullptr)
		{
			gameRank->open(this, cocos2d::Vec2::ZERO, 1001, 1000);
		}
	}
   //else if (name.compare(btn_back_tag) == 0) {//返回
		//platformDelegate->handleReturnButton();
	//}
    else
	{
        CommandManager::getInstance()->showGamePrompt(GBKToUtf8("程序猿使劲开发中!"));
    }
}
void BottomMenuCompositeV66::setOpenBankFunc()
{
    if (!UserDefault::getInstance()->getBoolForKey("IS_OPEN_BANK")) {
        GameData::getInstance()->saveUserBankPassWord("");
        auto ensurelayer = GameBankEnsureV66::create();
        if (ensurelayer) {
            ensurelayer->open(this->getParent(),Vec2::ZERO,MODULE_LAYER_ORDER_V66,0);
            ensurelayer->_bCallBack = [=](){
                UserDefault::getInstance()->setBoolForKey("IS_OPEN_BANK",true);
                IGameBank* gameBank = GameBankFactory::createWithVersion();
                gameBank->setMenuDelegate(this);
                gameBank->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V66, 0);
            };
        }
    }
    else
    {
        IGameBank* gameBank = GameBankFactory::createWithVersion();
        gameBank->setMenuDelegate(this);
        gameBank->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V66, 0);
        
    }
}
bool BottomMenuCompositeV66::turnplateForwardSelector(HNSocketMessage* socketMessage)
{
	//结果： 1-今天还没玩过转盘；2-其他账号今天已经玩过转盘；3-今天已经玩过转盘 4-需要绑定手机才能转

	CCAssert(sizeof(MSG_GR_S2C_TurnTableRequest) == socketMessage->objectSize, "sizeof(MSG_GR_S2C_TurnTableRequest) != objectSize");
	if (sizeof(MSG_GR_S2C_TurnTableRequest) != socketMessage->objectSize)    return false;

	MSG_GR_S2C_TurnTableRequest* msg = (MSG_GR_S2C_TurnTableRequest*)socketMessage->object;
	//msg->nResult = 2;
	switch (msg->nResult)
	{
	case 1:
	{
		this->openNewYearActivity();

		//bool ret = GameCreator()->startGameClient(11118888, 1, true);

		/*ComRoomInfo* roomInfo = RoomInfoModule()->getRoom(0);
		if (roomInfo) {
			static HNRoomLogicBase* _roomLogic = new HNRoomLogicBase(new IHNRoomLogicBase());
			_roomLogic->start();
			_roomLogic->requestLogin(roomInfo->uRoomID);
			RoomLogic()->setSelectedRoom(roomInfo);
		}*/

		
		break;
	}
	case 2:
	{
			 // GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("其他账号已经进行过转盘游戏啦."));
			  break;
	}
	case 3:
	{
			  //GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("已经进行过转盘游戏啦."));
			  break;
	}
	case 4:
	{
			//test;
			/*
			this->openNewYearActivity();
			bool ret = GameCreator()->startGameClient(11118888, 1, true);
			if (!ret) {
				CommandManager::getInstance()->showGamePrompt(GBKToUtf8("游戏启动失败。"));
			}
			else
			{
				//启动成功置为false防止游戏退到roomlist有执行快速加入逻辑
				UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, false);
			}
			*/
			//  GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("参加每日转盘,需先在个人资料内绑定手机."));
			  break;
	}
	default:
		break;
	}
	return true;
}

bool BottomMenuCompositeV66::updateOnlineUserCount(HNSocketMessage* socketMessage)
{
	if (socketMessage->objectSize != sizeof(MSG_GR_RS_OnLineUserCount)) return false;

	MSG_GR_RS_OnLineUserCount* msg = (MSG_GR_RS_OnLineUserCount*)(socketMessage->object);

	if (l_onlinePeople && msg->nOnLineUserCount > 0)
	{
		l_onlinePeople->setString(Tools::fourSeparateMoney(msg->nOnLineUserCount));
	}

	return true;
}
void BottomMenuCompositeV66::updateTopMenuShow(MenuType type)
{
	if (type == MenuType::room_type)
	{
		l_userID->setVisible(false);

		m_NameBg->setPosition(m_vOldPos.at(0));
		copy->setPositionX(m_NameBg->getPositionX() + m_NameBg->getContentSize().width + 30.0f);
		l_userName->setString(StringUtils::format("%d", PlatformLogic()->loginResult.dwUserID));

		m_MoneyBg->setPosition(Vec2(m_vOldPos.at(1).x + 160, m_vOldPos.at(1).y));

		//记录的游戏id
		int iGameID = UserDefault::getInstance()->getIntegerForKey(QUICK_JOIN_GAME_ID);
		std::string gameSpriteStr = StringUtils::format(GAME_TIP_PATH, iGameID);
		if (FileUtils::getInstance()->isFileExist(gameSpriteStr))
		{
			m_GameTipSprite->setVisible(true);
			m_GameTipSprite->setTexture(gameSpriteStr);
		}
	}
	else if (type == MenuType::game_list_type)
	{
		l_userID->setVisible(true);
		l_userID->setPosition(m_vOldPos.at(0));

		m_NameBg->setPosition(m_vOldPos.at(1));
		copy->setPositionX(l_userID->getPositionX() + l_userID->getContentSize().width*0.5f + 80.0f);
		std::string nickName(PlatformLogic()->loginResult.nickName);
		l_userName->setString(ForceGBKToUtf8(nickName.c_str()));

		m_MoneyBg->setPosition(m_vOldPos.at(2));

		m_GameTipSprite->setVisible(false);
	}
	else if (type == MenuType::func_type)
	{
		m_GameTipSprite->setVisible(false);
	}
	else{

	}
}

void BottomMenuCompositeV66::update(float dt) {
	MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
	/*if (m_money != LogonResult.i64Money) {
		m_money = LogonResult.i64Money;
		l_userMoney->setString(to_string(m_money));
	}*/
}


bool BottomMenuCompositeV66::updateShowTuiJianRenModule(HNSocketMessage* socketMessage)
{
	CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
	CCAssert(sizeof(MSG_GP_S_TiYanMa) == socketMessage->objectSize, "sizeof(MSG_GP_S_TiYanMa) != objectSize");
	MSG_GP_S_TiYanMa* msg = (MSG_GP_S_TiYanMa*)socketMessage->object;

	if (msg)
	{
		switch (socketMessage->messageHead.bHandleCode)
		{
		case 0:
		{
				  auto invitedLayer = InviteCodeLayerV66::create();
				  if (invitedLayer) {
					  invitedLayer->setCodeType(1);
					  invitedLayer->open(this->getParent(), Vec2::ZERO, MODULE_LAYER_ORDER_V66);
				  }
				  break;
		}
		case 1:
		{
				  Layout* layout = (Layout *)_csbRoot->getChildByName("layout");
				  Node* node = (Node*)layout->getChildByName("Node_icon_2");
				  if (node)
				  {
					  char str[128] = { 0 };
					  int bindID = msg->responseInfo;
					  sprintf(str, "您已绑定推广人，ID：%d", bindID);
					  auto textTip = Text::create();
					  textTip->setString(GBKToUtf8(str));
					  textTip->setFontName(DEFAULT_FONT);
					  textTip->setFontSize(30);
					  textTip->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
					  float fPosX = node->getPositionX();
					  textTip->setPosition(Vec2(fPosX, 75));
					  layout->addChild(textTip);
					  textTip->runAction(Sequence::create(DelayTime::create(2.0f), FadeOut::create(0.5), RemoveSelf::create(), nullptr));
				  }
				  break;
		}
		default:
			break;
		}
	}
	return true;
}

void BottomMenuCompositeV66::headClickCallback(Ref *pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	if (!UserDefault::getInstance()->getBoolForKey(TOUCH_HEAD)) {
		return;
	}
	canTouchHead(false);

	ImageView* head = (ImageView*)pSender;

	IGameUser* gameUser = GameUserFactory::createWithVersion();
	gameUser->open(this->getParent(), Vec2(0, -WIN_SIZE.height), CHILD_LAYER_ZORDER, CHILD_LAYER_BANK_TAG);
	gameUser->updateHeadCallBack = [&]() -> void {
		head->loadTexture(StringUtils::format("common/head/%d.png", PlatformLogic()->getInstance()->loginResult.bLogoID));
	};
	gameUser->updateNicknameCallBack = [&](string str)->void{
		l_userName->setString(str);
	};

	this->layerDelegate = gameUser;
}

void BottomMenuCompositeV66::canTouchHead(bool isTouch) {
	UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, isTouch);
}
void BottomMenuCompositeV66::setQuickBtn(bool show)
{
    //auto fastJoin =static_cast<Button *>(_csbRoot->getChildByName("Button_join"));
    //fastJoin->setVisible(show);
    
}
