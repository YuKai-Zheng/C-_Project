#include "TopMenuCompositeV66.h"
#include "PlatformResV66.h"

#include "HNLobby/pattern/CommandManager.h"

#include "../pattern/factory/gameShop/GameShopFactory.h"

#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/ChildLayer/GameBankLayer.h"
#include "HNLobby/ChildLayer/GameStoreLayer.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/ChildLayer/GameSignLayer.h"
#include "HNLobby/ChildLayer/ServiceLayer.h"
#include "HNLobby/ChildLayer/GameSetLayer.h"
#include "HNLobby/ChildLayer/GameExitChangeLayer.h"
#include "HNLobby/ChildLayer/GameGiftShop.h"
#include "HNLobby/ChildLayer/GameRankingList.h"
#include "HNLobby/ChildLayer/NoticeList.h"
#include "HNLobby/GameNotice.h"
#include "../pattern/factory/gameBank/GameBankFactory.h"
#include "../pattern/factory/gameSet/GameSetFactory.h"
#include "../pattern/factory/gameUser/GameUserFactory.h"
#include "../pattern/factory/gameRank/GameRankFactory.h"
#include "../pattern/factory/gameUser/IGameUser.h"
#include "HNLobby/v66/GameUserV66.h"
#include "HNLobby/v66/ExchangeManager.h"

#include "../globel.h"
#include "base/ClipboardHelper.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "XHChatQQ/QQMgr.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/ClipboardHelper.h"
#endif

#define btn_exit_tag        "Button_exit"
#define btn_shop_tag        "Button_shop"
#define btn_bank_tag        "Button_bank"
#define btn_setup_tag       "Button_setup"
#define btn_customer_tag    "Button_customer"
#define btn_rank_tag        "btn_rank"

#define TIPS_IMAGE      "hallimg/main/tips.png"

using namespace HN;

TopMenuCompositeV66::TopMenuCompositeV66() :
_isTouch(true)
{
}

TopMenuCompositeV66::~TopMenuCompositeV66(){
	unscheduleUpdate();
}

bool TopMenuCompositeV66::init() {
	if (!Node::init()) {
		return false;
	}

	_spreadLogic.init(this);

	m_needTipAnimation = false;
	m_bottomButton.clear();

	csb = CSLoader::createNode(PLATFORM_TOP_MENU_V66);
	addChild(csb);

    Layout* layout = (Layout *)csb->getChildByName("layout");
    
    auto top_menu_bk = dynamic_cast<ImageView*>(layout->getChildByName("Image_bk"));
	auto user_position_img = dynamic_cast<Node*>(Tools::getNodeByName(csb, "Node_img_position"));
    
	MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
    
    // 拷贝ID
    copy = dynamic_cast<Button*>(top_menu_bk->getChildByName("Button_copy"));
    if (copy) {
        copy->addTouchEventListener(CC_CALLBACK_2(TopMenuCompositeV66::menuLobbyTopCallBack, this));
        
    }
    
    // 用户昵称
    l_userName = dynamic_cast<Text*>(top_menu_bk->getChildByName("Text_nickName"));
    std::string nickName(LogonResult.nickName);
    if (!nickName.empty())
	{
        l_userName->setString(ForceGBKToUtf8(nickName.c_str()));
    }
    else
	{
        l_userName->setString(GBKToUtf8("未知"));
    }
    
    //用户ID
    l_userID = (Text *)Helper::seekWidgetByName(top_menu_bk, "Text_userid");
    l_userID->setString(StringUtils::format("ID:%d", LogonResult.dwUserID));
    
    // 用户金币
    auto m_MoneyBg = dynamic_cast<ImageView*>(top_menu_bk->getChildByName("Image_4"));
    if (m_MoneyBg)
	{
		m_MoneyBg->setPositionX(WIN_SIZE.width - 40);
        l_userMoney = dynamic_cast<Text*>(m_MoneyBg->getChildByName("Text_money"));
        l_userMoney->setString(Tools::goldDisplayByType(LogonResult.i64Money));
    }

	//兑换
	auto pExchange = dynamic_cast<Button*>(top_menu_bk->getChildByName("Button_exchange"));
	if (pExchange)
	{
		pExchange->setPositionX(WIN_SIZE.width - 50 - m_MoneyBg->getSize().width - (pExchange->getSize().width/2));
		pExchange->addTouchEventListener(CC_CALLBACK_2(TopMenuCompositeV66::menuLobbyTopCallBack, this));
	}
    
    auto m_DimondBg = dynamic_cast<ImageView*>(top_menu_bk->getChildByName("Image_dimond"));
    if (m_DimondBg) 
	{
        l_userDimond = dynamic_cast<Text*>(m_DimondBg->getChildByName("Text_money"));
        l_userDimond->setString("0");
    }
    
    auto exitBtn =dynamic_cast<Button *>(top_menu_bk->getChildByName("Button_exit"));
	exitBtn->setAlphaTouchEnable(true);
     if (exitBtn)
	 {
         exitBtn->addTouchEventListener(CC_CALLBACK_2(TopMenuCompositeV66::menuLobbyTopCallBack, this));
     }
	 /*

	 //排行榜界面入口
	 auto rankBtn = dynamic_cast<Button *>(top_menu_bk->getChildByName("btn_rank"));
	 if (rankBtn) {
		 string jsonpath(string("v66/module/paihangbang/ani/btn_paihangbang.json"));
		 string atlaspath(string("v66/module/paihangbang/ani/btn_paihangbang.atlas"));
		 auto skeletonNode = spine::SkeletonAnimation::createWithJsonFile(jsonpath, atlaspath);
		 if (skeletonNode) {
			 skeletonNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

			 Vec2 pp(40, -120);
			 skeletonNode->setScale(0.8);
			 skeletonNode->setPosition(pp);
			 skeletonNode->setAnimation(0, "btn_paihangbang", true);
			 rankBtn->addChild(skeletonNode);
		 }

		 rankBtn->addTouchEventListener(CC_CALLBACK_2(TopMenuCompositeV66::menuLobbyTopCallBack, this));
	 }

	 */
        
    //用户头像
	float fcale3 = 1.0f;
    userHead = UserHead::create(HEAD_FRAME_PURPLE_V66);
    userHead->setScale(fcale3);
    userHead->setName("TOP_USER_HEAD");
    userHead->setPosition(Vec2(230,50));
    userHead->setVisible(true);

	float  fscale = 0.35f;
    userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V66, "hallimg/niuniu/5_1.png", fscale);
    userHead->setHeadTouchEnabled(true);

    userHead->onHeadClickEvent = [=](UserHead *pSender, Vec2 touchVec) {
        IGameUser* gameUser = GameUserFactory::createWithVersion();
        gameUser->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V66, 0);
        dynamic_cast<GameUserV66*>(gameUser)->setTopMenuDelegate(this);
        
        setLayerDelegate(gameUser);
        
        gameUser->updateHeadCallBack = [=]()-> void {
            unsigned int logoID = PlatformLogic()->getInstance()->loginResult.bLogoID;
            if (Tools::headFromWX(logoID))
            {
                Texture2D* texture = new Texture2D;
                Tools::loadWechatImageFromLocal(texture);
                if (texture && userHead) {
                    userHead->setClipUserHeadWithTexTure(texture);
                }
            }
            else
            {
                userHead->setClipUserHeadWithFile(StringUtils::format(PLAYER_HEAD_V66, logoID), "hallimg/niuniu/5_1.png", 0.35f);
            }
        };
        gameUser->updateNicknameCallBack = [=](string str)->void{
            l_userName->setString(str);
        };
    };

	Vec2 v2 = userHead->getAnchorPoint();
	Vec2 vPos = userHead->getPosition();
	userHead->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	userHead->setPosition(Vec2::ZERO);
	user_position_img->addChild(userHead);

	Tools::PlayFrameAnimation(userHead,"csb/v66/Node_txk.csb");
	/*
    auto node_head = CSLoader::createNode("csb/v66/Node_txk.csb");
    if (node_head) {
		node_head->setScale(1.28f);
        node_head->setPosition(userHead->getContentSize()/2);
        userHead->addChild(node_head);
        
        auto action = CSLoader::createTimeline("csb/v66/Node_txk.csb");
        if (action) {
            node_head->runAction(action);
            action->gotoFrameAndPlay(0);
        }
    }
	*/
    bool isWechatLogin = UserDefault::getInstance()->getBoolForKey(WX_LOGIN);
    if (isWechatLogin) {
        std::string imgHeadUrl = UserDefault::getInstance()->getStringForKey(WX_HEAD_IMG_URL);
        
        cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
        request->setUrl(imgHeadUrl);
        request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
        request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
            if (response->isSucceed()) {
                std::vector<char>* buffer = response->getResponseData();
                char *buff = (char *)malloc(buffer->size());
                std::copy(buffer->begin(), buffer->end(), buff);
                
                if (buffer->size() > 0) {
                    auto fileName = FileUtils::getInstance()->getWritablePath() + WX_HEAD_IMG_NAME;
                    FILE *fp = fopen(fileName.c_str(), "wb+");
                    fwrite(buff, 1, buffer->size(), fp);
                    fclose(fp);
                }
            }
            
            
            if (Tools::needLoadWechatImage()) {
                Texture2D* texture = new Texture2D;
                Tools::loadWechatImageFromLocal(texture);
                if (texture && userHead) {
                    userHead->setClipUserHeadWithTexTure(texture, "hallimg/niuniu/5_1.png", 0.60f);
                }
            }
            else
            {
                std::string head_path = StringUtils::format(PLAYER_HEAD_V66, LogonResult.bLogoID);
                userHead->setClipUserHeadWithFile(head_path, "hallimg/niuniu/5_1.png", 0.35f);
            }
        });
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    else
    {
        std::string head_path = StringUtils::format(PLAYER_HEAD_V66, LogonResult.bLogoID);
        if (!Tools::headFromWX(PlatformLogic()->loginResult.bLogoID))
        {
			float fscale1 = 0.35f;
            userHead->setClipUserHeadWithFile(head_path, "hallimg/niuniu/5_1.png", fscale1);
        }
    }
    
  
    scheduleUpdate();
  
#if 0
    auto effectParentNode = dynamic_cast<Node*>(imgParent->getChildByName("Node_effect"));
    if (effectParentNode) {
        auto node_action = CSLoader::createNode("csb/v66/Node_topEffect.csb");
        if (node_action) {
            node_action->setPosition(Vec2(0,0));
            effectParentNode->addChild(node_action);
            
            auto action = CSLoader::createTimeline("csb/v66/Node_topEffect.csb");
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
#endif

#if  0
    auto effectParentNode2 = dynamic_cast<Node*>(exitBtn->getChildByName("Node_effect2"));
    if (effectParentNode2) {
        auto node_action = CSLoader::createNode("csb/v66/Node_topEffect2.csb");
        if (node_action) {
            node_action->setPosition(Vec2(0,0));
            effectParentNode2->addChild(node_action);
            
            auto action = CSLoader::createTimeline("csb/v66/Node_topEffect2.csb");
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
	layout->setContentSize(Size(WIN_SIZE.width, layout->getContentSize().height));
	//if (rankBtn)
	//{
		//rankBtn->setPositionX(WIN_SIZE.width - 430);
	//}
    //if (WIN_SIZE.width > 1280) {
    //    imgParent->setPositionX(imgParent->getPositionX() + (640 - WIN_SIZE.width / 2) + 50);
    //    exitBtn->setPositionX(exitBtn->getPositionX() + WIN_SIZE.width / 2 - 700);
    //}
    
    
	return true;
}


void TopMenuCompositeV66::update(float dt) {

    MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
    if (m_money != LogonResult.i64Money) {
        m_money = LogonResult.i64Money;
        l_userMoney->setString(Tools::goldDisplayByType(LogonResult.i64Money));
    }
}


bool TopMenuCompositeV66::updateBankTipTag(HNSocketMessage* socketMessage) {
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

	auto aniPath = "v66/animation/main/account/account.csb";

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

void TopMenuCompositeV66::menuLobbyTopCallBack(Ref *pSender, Widget::TouchEventType type)
{
	if (Widget::TouchEventType::ENDED != type || _isTouch == false)
		return;
	auto btn = (Button *)pSender;
	_isTouch = false;
	btn->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {
		_isTouch = true;
	}), nullptr));

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	std::string name(btn->getName());

	if (name.compare(btn_exit_tag) == 0) { // 返回
		if (platformDelegate) {
			platformDelegate->handleReturnButton();
		}
	}
    else if (name.compare("Button_copy") == 0) 
	{ //复制id
        char str[180] = { 0 };
        sprintf(str, "%s",l_userID->getString().c_str());
#if(CC_TARGET_PLATFORM  == CC_PLATFORM_ANDROID)
        JniMethodInfo minfo;//定义Jni函数信息结构体
        //getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
        bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/jlx/MixProject/AppActivity", "paste", "(Ljava/lang/String;)V");
        if (!isHave) {
        }
        else {
            jobject para = minfo.env->NewStringUTF(str);
            minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, para);
			//GamePromptLayer::create()->showPrompt(GetText("USERINFO_COPYID"));
            //CommandManager::getInstance()->showGamePrompt(GetText("USERINFO_COPYID"));
        }
#elif CC_TARGET_PLATFORM  == CC_PLATFORM_IOS
        ClipboardHelper::copyStr(str);
		//GamePromptLayer::create()->showPrompt(GetText("USERINFO_COPYID"));
        //CommandManager::getInstance()->showGamePrompt(GetText("USERINFO_COPYID"));
#endif
		GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
		layer->setGamePromptTip(GetText("USERINFO_COPYID"), 0, -WIN_SIZE.height / 2);
    }
	else if (name.compare(btn_setup_tag) == 0) 
	{ //设置
		IGameSet* gameSet = GameSetFactory::createWithVersion();
		gameSet->setBgPath(GAME_BACKGROUND_MUSIC_V66);
		gameSet->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V66, 0);

		//topMenuDelegate->setLayerDelegate(gameSet);

	}
    else if (name.compare(btn_rank_tag) == 0)
    {
        // 排行榜
		/*
        auto gameRank = GameRankFactory::createWithVersion();
        CC_ASSERT(gameRank != nullptr);
        
        if (gameRank != nullptr)
        {
            gameRank->open(this, cocos2d::Vec2::ZERO, 1001, 1000);
        }
		*/
    }
	else if (name.compare("Button_exchange") == 0)
	{
		auto exchangeManager = ExchangeManager::create();
		if (exchangeManager)
		{
			exchangeManager->openAddToParent((Layer*)this->getParent(), MODULE_LAYER_ORDER_V66, WIN_SIZE / 2);
		}
	}
	else
	{  // 其他
		GamePromptLayer::create()->showPrompt(GBKToUtf8("程序猿使劲开发中。"));
	}
}

void TopMenuCompositeV66::walletChanged(LLONG money) {

}

void TopMenuCompositeV66::autoMove() {
    auto moveByY = MoveBy::create(0.3f, Vec2(0, csb->getContentSize().height));
    auto reverseMoveByY = moveByY->reverse();
    csb->runAction(Sequence::create(moveByY, reverseMoveByY, nullptr));
}
void TopMenuCompositeV66::moveRight(bool isRight) {
	float width = WIN_SIZE.width;
	float height = csb->getContentSize().height;
	if (isRight) {//右移动
		auto moveByY = MoveBy::create(0.3f, Vec2(0, height));
		auto reverseMoveByY = moveByY->reverse();
		auto moveByX = MoveBy::create(0.1f, Vec2(width, 0));
		csb->runAction(Sequence::create(moveByY, moveByX, reverseMoveByY, nullptr));
	}
	else {//左移
		auto moveByY = MoveBy::create(0.3f, Vec2(0, height));
		auto reverseMoveByY = moveByY->reverse();
		auto moveByX = MoveBy::create(0.1f, Vec2(-width, 0));
		csb->runAction(Sequence::create(moveByY, moveByX, reverseMoveByY, nullptr));
	}
}

void TopMenuCompositeV66::closeChildLayer() {
    if (layerDelegate) {
        layerDelegate->close();
        layerDelegate = nullptr;
    }
}

void TopMenuCompositeV66::setType(MenuType type) {
	//updateTopMenuShow(type);
}

void TopMenuCompositeV66::getRoomSpriteByRoomName(char name[], char fishName[], char* gameName) {
	char str[100] = { 0 };
	char bStr[100] = { 0 };

	string tmpName = ForceGBKToUtf8(gameName);

	string s0 = GBKToUtf8("新手场");
	string s1 = GBKToUtf8("初级场");
	string s2 = GBKToUtf8("中级场");
	string s3 = GBKToUtf8("高级场");
	string s4 = GBKToUtf8("超级场");
	string s5 = GBKToUtf8("至尊场");

	if (std::strcmp(tmpName.c_str(), s0.c_str()) == 0) {
		sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/xinshouchang.png");
		sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/xinshouchang.png");
	}
	else if (std::strcmp(tmpName.c_str(), s1.c_str()) == 0) {
		sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/chujichang.png");
		sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/chujichang.png");
	}
	else if (std::strcmp(tmpName.c_str(), s2.c_str()) == 0) {
		sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/zhongjichang.png");
		sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/zhongjichang.png");
	}
	else if (std::strcmp(tmpName.c_str(), s3.c_str()) == 0) {
		sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/gaojichang.png");
		sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/gaojichang.png");
	}
	else if (std::strcmp(tmpName.c_str(), s4.c_str()) == 0) {
		sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/chaojichang.png");
		sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/chaojichang.png");
	}
	else if (std::strcmp(tmpName.c_str(), s5.c_str()) == 0) {
		sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/zhizunchang.png");
		sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/zhizunchang.png");
	}
	else {
		sprintf(str, "%s", "platform/lobbyUi/PlatformUi_TOP/xinshouchang.png");
		sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishBiLi/xinshouchang.png");
	}
	strcpy(name, str);
	strcpy(fishName, bStr);
}

void TopMenuCompositeV66::canTouchHead(bool isTouch) {
	UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, isTouch);
}
