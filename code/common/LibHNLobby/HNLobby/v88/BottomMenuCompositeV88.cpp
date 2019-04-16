#include "BottomMenuCompositeV88.h"
#include "HNLobby/PersionalCenter/GameUserDataLayer.h"
#include "HNLobby/ChildLayer/GameOnlineReward.h"
#include "HNLobby/ChildLayer/GameRechargeRebate.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GamePrompt.h"
//#include "UMeng/Cocos2dx/Common/CCUMTypeDef.h"
#include "../pattern/CommandManager.h"
//#include "UMeng/Cocos2dx/Common/CCUMSocialSDK.h"
#include "PlatformResV88.h"
#include "../ChildLayer/GameStoreLayer.h"
#include "../pattern/factory/gameBank/GameBankFactory.h"
#include "../pattern/factory/gameShop/GameShopFactory.h"
#include "../pattern/factory/gameSet/GameSetFactory.h"
#include "../pattern/factory/gameRank/GameRankFactory.h"
#include "Sound/HNAudioEngine.h"
#include "HNLobby/v88/InvitedCodeLayerV88.h"
//USING_NS_UM_SOCIAL;
#include "HNLobby/state/PlatformState.h"

#define BTN_RANK_RES    "v88/main/paihangb.png"

BottomMenuCompositeV88::BottomMenuCompositeV88() {
	l_onlinePeople = nullptr;
	_csbRoot = nullptr;
}

BottomMenuCompositeV88::~BottomMenuCompositeV88() {
	PlatformLogic()->removeEventSelector(MDM_GP_MESSAGE, ASS_GP_ONLINE_USER_COUNT);
    PlatformLogic()->removeEventSelector(MDM_GP_TRANS_SUCCESS_TIP, ASS_GP_TRANS_SUCCESS_TIP);
}


bool BottomMenuCompositeV88::init() {
    if (!Node::init()) {
        return false;
    }

    m_bottomButton.clear();
    m_needTipAnimation = false;
    
    // 玩家信息结构体
    MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;

	_csbRoot = CSLoader::createNode(PLATFORM_BOTTOM_MENU_V88);
	addChild(_csbRoot);

	Layout* layout = (Layout *)_csbRoot->getChildByName("layout");
    setContentSize(layout->getContentSize());
    
	//在线人数
	auto onlineBg = ImageView::create("v88/main/zaixianrenshu.png");
	onlineBg->setScale(0.65f);
	onlineBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	onlineBg->setPosition(Vec2(75, 40));
	layout->addChild(onlineBg);

	l_onlinePeople = Text::create();
	l_onlinePeople->setFontName(FONT_TTF_FZCY);
	l_onlinePeople->setFontSize(30);
	l_onlinePeople->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	l_onlinePeople->setPosition(Vec2(onlineBg->getContentSize().width*0.5f, onlineBg->getContentSize().height*0.2f));
	onlineBg->addChild(l_onlinePeople);
	l_onlinePeople->setString(GBKToUtf8("加载中"));
    
	int nModuleCount = 5;
    char temp[64] = {0};
    vector<Node*> vModuleNode;
	for (int i = 0; i < nModuleCount-1; i++)
    {
        sprintf(temp, "Node_icon_%d", i);
        auto node = layout->getChildByName(temp);
        if(node)
        {
            vModuleNode.push_back(node);
        }
    }
    
    //功能开关
    vector<bool> vIsModuleOpen;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	for (int i = 0; i < nModuleCount; i++)
	{
		vIsModuleOpen.push_back(true);
	}
#else
	vIsModuleOpen.push_back(true);															//设置
	vIsModuleOpen.push_back(PlatformConfig::getInstance()->funcConfig.isOpenBankFunc);		//银行
	vIsModuleOpen.push_back(!Tools::isiOSReview());											//邀请
	vIsModuleOpen.push_back(PlatformConfig::getInstance()->funcConfig.isOpenRechargeFunc);	//商城
	vIsModuleOpen.push_back(PlatformConfig::getInstance()->funcConfig.isOpenRankFunc);		//排行榜
#endif

    
    //功能名字
    vector<string> vModuleImgName;
	vModuleImgName.push_back(btn_set_tag);
    vModuleImgName.push_back(btn_bank_tag);
	vModuleImgName.push_back(btn_invite_tag);
	vModuleImgName.push_back(btn_charge_tag);
    
    for(int i = 0; i < vModuleImgName.size(); i++)
    {
        sprintf(temp, "v88/main/up/%s.png", vModuleImgName.at(i).c_str());
		Button* btn = Button::create(temp, temp);

        btn->setName(vModuleImgName.at(i));
        btn->setVisible(vIsModuleOpen.at(i));
        btn->setShowEffect(false);
        btn->setName(vModuleImgName.at(i));
		btn->setPressedActionEnabled(false);
        btn->addTouchEventListener(CC_CALLBACK_2(BottomMenuCompositeV88::menuLobbyBottomCallBack, this));
        
        m_bottomButton.push_back(btn);
        
		if (i == (vModuleImgName.size() - 1))
		{
			Tools::addAnimationFrame("v88/animation/main/icon/icon_shopping.csb", btn, "icon_shopping", Vec2(btn->getContentSize().width / 4 - 20, 1));
		}

        vModuleNode.at(i)->addChild(btn);
	
		if (vModuleImgName.at(i).compare(btn_bank_tag) == 0)
		{
			auto redSPr = Sprite::create("v88/niuniu/4_3.png");
			redSPr->setName("redLight");
			redSPr->setVisible(false);
			redSPr->setPosition(Vec2(btn->getContentSize().width, btn->getContentSize().height));
			btn->addChild(redSPr);
		}
       
        if(vModuleImgName.at(i).compare(btn_activity_tag) == 0)
        {
            Tools::addAnimationFrame("v88/animation/luckyturntable/luckyturntable_logo/luckyturntable_logo.csb", btn, "luckyturntable_logo", Vec2(0, 0));
        }

    }
    
    Button* rankBtn = Button::create(BTN_RANK_RES);
    if (rankBtn) {
        rankBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        rankBtn->setName(btn_rank_tag);
        rankBtn->addTouchEventListener(CC_CALLBACK_2(BottomMenuCompositeV88::menuLobbyBottomCallBack, this));
        rankBtn->setPosition(Vec2(-WIN_SIZE.width/2, WIN_SIZE.height/2));
        addChild(rankBtn);
        rankBtn->setVisible(vIsModuleOpen.at(vIsModuleOpen.size()-1));
    }
    
	//在线人数
	PlatformLogic()->addEventSelector(MDM_GP_MESSAGE, ASS_GP_ONLINE_USER_COUNT, HN_SOCKET_CALLBACK(BottomMenuCompositeV88::updateOnlineUserCount, this));
	//银行到账通知
    TMSG_GP_BankTransferSuccessTip pData;
    pData.iUserID = PlatformLogic()->getInstance()->loginResult.dwUserID;
    pData.iType = 0;
    pData.iRet = 0;
    PlatformLogic()->sendData(MDM_GP_TRANS_SUCCESS_TIP, ASS_GP_TRANS_SUCCESS_TIP, &pData, sizeof(TMSG_GP_BankTransferSuccessTip), HN_SOCKET_CALLBACK(BottomMenuCompositeV88::updateBankTipTag, this));

	scheduleOnce(schedule_selector(BottomMenuCompositeV88::activityTime), 0.2);

	return true;
}

void BottomMenuCompositeV88::moveOut(bool isOut) {
    int k = isOut ? -1 : 1;
    if (!isOut) {
        this->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, this->getContentSize().height * k * 1.1)), nullptr));
    } else {
        this->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, this->getContentSize().height * k * 1.1)), nullptr));
    }
}

void BottomMenuCompositeV88::walletChanged(LLONG money) {

}

void BottomMenuCompositeV88::openNewYearActivity()
{
	if (platformDelegate) {
		platformDelegate->openNewYearActivity();
	}
}

void BottomMenuCompositeV88::activityTime(float dt)
{
	MSG_GR_C2S_TurnTableRequest pTurnData;

	pTurnData.nUserID = PlatformLogic()->getInstance()->loginResult.dwUserID;
	PlatformLogic()->sendData(MDM_GP_LOGON, ASS_GP_LOGON_ISTURNTABLE_TODAY, &pTurnData, sizeof(pTurnData), HN_SOCKET_CALLBACK(BottomMenuCompositeV88::bankerForwardSelector, this));
}

void BottomMenuCompositeV88::menuLobbyBottomCallBack(Ref *pSender, Widget::TouchEventType type) {
    if (Widget::TouchEventType::ENDED != type/* || !_isTouch*/)
        return;

    auto btn = (Button *)pSender;
    _isTouch = false;
    btn->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {
        _isTouch = true;
    }),
    nullptr));

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

    std::string name(btn->getName());
    if (name.compare(btn_join_tag) == 0) { //快速加入
		
//		UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, true);
//		quickJoinGame();

    } else if (name.compare(btn_bank_tag) == 0) { //银行

        if (m_needTipAnimation) {
            m_needTipAnimation = false;
            TMSG_GP_BankTransferSuccessTip pData;
            pData.iUserID = PlatformLogic()->getInstance()->loginResult.dwUserID;
            pData.iType = 1;
            pData.iRet = 0;
            PlatformLogic()->sendData(MDM_GP_TRANS_SUCCESS_TIP, ASS_GP_TRANS_SUCCESS_TIP, &pData, sizeof(TMSG_GP_BankTransferSuccessTip));

            if (btn) {
                auto  redlight = btn->getChildByName("redLight");
                if (redlight) {
                    redlight->setVisible(false);
                    redlight->stopAllActions();
                }
            }
            
        }
        
        IGameBank* gameBank = GameBankFactory::createWithVersion();
        gameBank->setMenuDelegate(this);
        gameBank->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V88, 0);
		auto  redlight = btn->getChildByName("redLight");
		if (redlight)
		{
			redlight->setVisible(false);
			redlight->stopAllActions();
		}
        //topMenuDelegate->setLayerDelegate(gameBank);

    } else if (name.compare(btn_rank_tag) == 0) {	//排行榜
        IGameRank* gameRank = GameRankFactory::createWithVersion();
        gameRank->open(this, Vec2(0, 0), MODULE_LAYER_ORDER_V88, 0);
    } else if (name.compare(btn_activity_tag) == 0) {	//活动
		MSG_GR_C2S_TurnTableRequest pTurnData;

		pTurnData.nUserID = PlatformLogic()->getInstance()->loginResult.dwUserID;
		PlatformLogic()->sendData(MDM_GP_LOGON, ASS_GP_LOGON_ISTURNTABLE_TODAY, &pTurnData, sizeof(pTurnData), HN_SOCKET_CALLBACK(BottomMenuCompositeV88::bankerForwardSelector, this));

    } else if (name.compare(btn_set_tag) == 0) { //设置
        IGameSet* gameSet = GameSetFactory::createWithVersion();
        gameSet->setBgPath(GAME_BACKGROUND_MUSIC_V88);
        gameSet->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V88, 0);

        //topMenuDelegate->setLayerDelegate(gameSet);

    } else if (name.compare(btn_charge_tag) == 0) { //商店
        IGameShop* gameShop =  GameShopFactory::createWithVersion();
        gameShop->setMenuDelegate(topMenuDelegate);
        gameShop->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V88, 0);

        //topMenuDelegate->setLayerDelegate(gameShop);

	}else if (name.compare(btn_invite_tag) == 0){
		//请求加载
		CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("加载中..."));
		MSG_GP_S_TiYanMa tiyanCode;
		memset(&tiyanCode, 0, sizeof(MSG_GP_S_TiYanMa));
		tiyanCode.userID = PlatformLogic()->getInstance()->loginResult.dwUserID;
		tiyanCode.flag = 0;
		PlatformLogic()->sendData(MDM_GP_SetTiYanMa, ASS_GP_SetTiYanMa, &tiyanCode, sizeof(MSG_GP_S_TiYanMa), HN_SOCKET_CALLBACK(BottomMenuCompositeV88::updateShowTuiJianRenModule, this));

	}else if (name.compare(btn_back_tag) == 0) {//返回
		platformDelegate->handleReturnButton();
	}else {
        CommandManager::getInstance()->showGamePrompt(GBKToUtf8("程序猿使劲开发中!"));
    }
}

bool BottomMenuCompositeV88::bankerForwardSelector(HNSocketMessage* socketMessage)
{
	//结果： 1-今天还没玩过转盘；2-其他账号今天已经玩过转盘；3-今天已经玩过转盘 4-需要绑定手机才能转

	CCAssert(sizeof(MSG_GR_S2C_TurnTableRequest) == socketMessage->objectSize, "sizeof(MSG_GR_S2C_TurnTableRequest) != objectSize");
	if (sizeof(MSG_GR_S2C_TurnTableRequest) != socketMessage->objectSize)    return false;

	MSG_GR_S2C_TurnTableRequest* msg = (MSG_GR_S2C_TurnTableRequest*)socketMessage->object;

	switch (msg->nResult)
	{
	case 1:
	{
			  this->openNewYearActivity();
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
			//  GamePromptLayer::create()->showPrompt(ForceGBKToUtf8("参加每日转盘,需先在个人资料内绑定手机."));
			  break;
	}
	default:
		break;
	}
	return true;
}

bool BottomMenuCompositeV88::updateBankTipTag(HNSocketMessage* socketMessage) {
    if (socketMessage) {
        if (socketMessage->messageHead.bHandleCode == 0) {
            m_needTipAnimation = false;
            return true;
        }
        else {
            m_needTipAnimation = true;
        }
    }
    //金币到账动画
    bool visible = this->isVisible();
    if (!visible || m_bottomButton.size() <= 0) {
        return true;
    }
    
    auto aniPath = "v88/animation/main/account/account.csb";
    
    for (int i = 0; i < m_bottomButton.size(); i++) {
        auto btn = m_bottomButton.at(i);
        if (btn) {
            if (btn->getName().compare(btn_bank_tag) == 0) {
                
                m_needTipAnimation = true;
                
                auto csb = CSLoader::createNode(aniPath);
                csb->setPosition(Vec2(238.0 / 1280 * WIN_SIZE.width, WIN_SIZE.height*0.5f - 15));
                btn->addChild(csb, 100);
                
                auto moeyAni = CSLoader::createTimeline(aniPath);
                btn->runAction(moeyAni);
                moeyAni->play("account", false);
                
                auto  redlight = btn->getChildByName("redLight");
                if (redlight) {
                    redlight->setVisible(true);
                    redlight->runAction(RepeatForever::create(Blink::create(1.0, 1)));
                }
                
                break;
            }
        }
    }
		
    return true;
}

bool BottomMenuCompositeV88::updateOnlineUserCount(HNSocketMessage* socketMessage)
{
	if (socketMessage->objectSize != sizeof(MSG_GR_RS_OnLineUserCount)) return false;

	MSG_GR_RS_OnLineUserCount* msg = (MSG_GR_RS_OnLineUserCount*)(socketMessage->object);

	if (l_onlinePeople && msg->nOnLineUserCount > 0)
	{
		l_onlinePeople->setString(Tools::fourSeparateMoney(msg->nOnLineUserCount));
	}

	return true;
}

bool BottomMenuCompositeV88::updateShowTuiJianRenModule(HNSocketMessage* socketMessage)
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
				  auto invitedLayer = InviteCodeLayerV88::create();
				  if (invitedLayer) {
					  invitedLayer->setCodeType(1);
					  invitedLayer->open(this->getParent(), Vec2::ZERO, MODULE_LAYER_ORDER_V88);
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
					  textTip->setFontName("platform/fonts/simhei.ttf");
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
