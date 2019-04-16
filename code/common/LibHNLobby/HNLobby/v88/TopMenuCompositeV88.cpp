#include "TopMenuCompositeV88.h"
#include "PlatformResV88.h"

#include "HNLobby/pattern/CommandManager.h"
#include "../pattern/factory/gameUser/GameUserFactory.h"
#include "../pattern/factory/gameUser/IGameUser.h"
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
#include "HNLobby/v88/GameUserV88.h"
//#include "HNLobby/v88/PopularizeSignManager.h"
#include "../globel.h"
#include "base/ClipboardHelper.h"
#include "HNPlatform/HNPlatformLogic.h"

#define btn_back_tag        "Button_back"
#define btn_copy_tag        "Button_copy"
#define btn_charge_tag      "shopBtn"
#define btn_popularize_tag	"Button_tuiGuang"

static const char* GAME_TIP_PATH = "v88/room/changci/%d.png";

using namespace HN;

TopMenuCompositeV88::TopMenuCompositeV88() :
_isTouch(true)
{
}

TopMenuCompositeV88::~TopMenuCompositeV88(){
	unscheduleUpdate();
}

bool TopMenuCompositeV88::init() {
	if (!Node::init()) {
		return false;
	}

	_spreadLogic.init(this);

	csb = CSLoader::createNode(PLATFORM_TOP_MENU_V88);
	addChild(csb);

    Layout* layout = (Layout *)csb->getChildByName("layout");

	Sprite* background = static_cast<Sprite*>(csb->getChildByName("background"));
	setContentSize(background->getContentSize());
	csb->setContentSize(background->getContentSize());

	// 拷贝ID
	copy = static_cast<Button*>(csb->getChildByName(btn_copy_tag));
    if (copy) {
        copy->setShowEffect(false);
        copy->addTouchEventListener(CC_CALLBACK_2(TopMenuCompositeV88::menuLobbyTopCallBack, this));
    }

	Button* btnAdd = dynamic_cast<Button*>(csb->getChildByName("Button_add"));
	btnAdd->setName(btn_charge_tag);
	btnAdd->addTouchEventListener(CC_CALLBACK_2(TopMenuCompositeV88::menuLobbyTopCallBack, this));

    //推广
    Button*  popularizeBtnCSB = dynamic_cast<Button*>(csb->getChildByName(btn_popularize_tag));
	popularizeBtnCSB->setVisible(false);
	
	auto spr = Sprite::create("v88/main/up/tuiguang.png");
	Button*  popularizeBtn = Button::create("v88/main/up/tuiguang.png", "v88/main/up/tuiguang.png");
	popularizeBtn->setName(btn_popularize_tag);
	popularizeBtn->setContentSize(spr->getContentSize());
    popularizeBtn->setVisible(PlatformConfig::getInstance()->funcConfig.isPopularize);
	popularizeBtn->setPosition(popularizeBtnCSB->getPosition());
	popularizeBtn->setShowEffect(false);
	popularizeBtn->setPressedActionEnabled(false);
	popularizeBtn->addTouchEventListener(CC_CALLBACK_2(TopMenuCompositeV88::menuLobbyTopCallBack, this));
	Tools::addAnimationFrame("v88/animation/main/icon/icon_tuiguang.csb", popularizeBtn, "icon_tuiguang", Vec2(0, -4));
	popularizeBtnCSB->getParent()->addChild(popularizeBtn);

	// 返回
	Button*  btnBack = dynamic_cast<Button*>(csb->getChildByName(btn_back_tag));
	btnBack->setShowEffect(false);
	btnBack->setPressedActionEnabled(true);
	btnBack->addTouchEventListener(CC_CALLBACK_2(TopMenuCompositeV88::menuLobbyTopCallBack, this));
	btnBack->setTouchEnabled(true);
	btnBack->setVisible(true);

	MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;

	//用户id
	l_userID = (Text *)Helper::seekWidgetByName(layout, "Text_userid");
	//l_userID->setVisible(false);
	l_userID->setString(StringUtils::format("%d", LogonResult.dwUserID));

	//m_NameBg = dynamic_cast<Sprite*>(layout->getChildByName("img_nameBg"));
	m_MoneyBg = dynamic_cast<Sprite*>(layout->getChildByName("img_goldBg"));

	// 用户昵称
	l_userName = dynamic_cast<Text*>(layout->getChildByName("Text_nickName"));
	std::string nickName(LogonResult.nickName);
	if (!nickName.empty()) {
		l_userName->setString(ForceGBKToUtf8(nickName.c_str()));
	}
	else {
		l_userName->setString(GBKToUtf8("未知"));
	}
    if (l_userName && l_userID && copy && m_MoneyBg) {
        float maxPosX = l_userName->getPositionX() + (max(l_userName->getContentSize().width, l_userID->getContentSize().width));
        if (maxPosX - copy->getContentSize().width * 1.5f >= m_MoneyBg->getPositionX()) {
            maxPosX = m_MoneyBg->getPositionX() - copy->getContentSize().width * 1.5f;
        }
        copy->setPositionX(maxPosX + copy->getContentSize().width);
    }
	// 用户金币
	l_userMoney = dynamic_cast<Text*>(m_MoneyBg->getChildByName("Text_money"));
	l_userMoney->setString(Tools::fourSeparateMoney(LogonResult.i64Money));

//	m_vOldPos.push_back(l_userID->getPosition());
//	m_vOldPos.push_back(m_NameBg->getPosition());
//	m_vOldPos.push_back(m_MoneyBg->getPosition());
	
	//gametip
//	m_GameTipSprite = Sprite::create();
//	m_GameTipSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
//	m_GameTipSprite->setPosition(Vec2(WIN_SIZE.width/2 - 140, m_vOldPos.at(2).y));
//	layout->addChild(m_GameTipSprite);
//	m_GameTipSprite->setVisible(false);

	auto headImg = dynamic_cast<Sprite*>(layout->getChildByName("img_headBg"));
	headImg->setVisible(false);

	//用户头像
	userHead = UserHead::create(HEAD_FRAME_PURPLE_V88);
	userHead->setScale(0.7f);
	userHead->setName("TOP_USER_HEAD");
	userHead->setPosition(headImg->getPosition());
	userHead->setVisible(true);
    userHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88, "v88/niuniu/5_1.png", 0.76f);
    userHead->setHeadTouchEnabled(true);
	userHead->onHeadClickEvent = [=](UserHead *pSender, Vec2 touchVec) {
//		if (!UserDefault::getInstance()->getBoolForKey(TOUCH_HEAD)) {
//			return;
//		}
		//canTouchHead(false);

		IGameUser* gameUser = GameUserFactory::createWithVersion();
		gameUser->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V88, 0);
		dynamic_cast<GameUserV88*>(gameUser)->setTopMenuDelegate(this);

		setLayerDelegate(gameUser);

		gameUser->updateHeadCallBack = [=]()-> void {
            unsigned int logoID = PlatformLogic()->getInstance()->loginResult.bLogoID;
            if(Tools::headFromWX(logoID))
            {
                Texture2D* texture = new Texture2D;
                Tools::loadWechatImageFromLocal(texture);
                if (texture && userHead) {
                    userHead->setClipUserHeadWithTexTure(texture, "v88/niuniu/5_1.png", 0.76f);
                }
            }
            else
            {
                userHead->setClipUserHeadWithFile(StringUtils::format(PLAYER_HEAD_V88, logoID), "v88/niuniu/5_1.png", 0.76f);
            }
		};
		gameUser->updateNicknameCallBack = [=](string str)->void{
			l_userName->setString(str);
		};
	};
	layout->addChild(userHead);


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
                    userHead->setClipUserHeadWithTexTure(texture, "v88/niuniu/5_1.png", 0.76f);
                }
            }
            else
            {
                std::string head_path = StringUtils::format(PLAYER_HEAD_V88, LogonResult.bLogoID);
                userHead->setClipUserHeadWithFile(head_path, "v88/niuniu/5_1.png", 0.76f);
            }
        });
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    else
    {
        std::string head_path = StringUtils::format(PLAYER_HEAD_V88, LogonResult.bLogoID);
        if(!Tools::headFromWX(PlatformLogic()->loginResult.bLogoID))
        {
            userHead->setClipUserHeadWithFile(head_path, "v88/niuniu/5_1.png", 0.76f);
        }
    }
    
	scheduleUpdate();

	return true;
}

void TopMenuCompositeV88::updateTopMenuShow(MenuType type)
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

void TopMenuCompositeV88::menuLobbyTopCallBack(Ref *pSender, Widget::TouchEventType type) {
	if (Widget::TouchEventType::ENDED != type || _isTouch == false)
		return;
	auto btn = (Button *)pSender;
	_isTouch = false;
	btn->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {
		_isTouch = true;
	}), nullptr));

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

	std::string name(btn->getName());

	if (name.compare(btn_back_tag) == 0) { // 返回
		if (platformDelegate) {
			platformDelegate->handleReturnButton();
		}

	}
	else if (name.compare(btn_copy_tag) == 0){		//copy
		char str[180] = { 0 };
		sprintf(str, "用户名:%s\n用户ID:%s", l_userName->getString().c_str(), l_userID->getString().c_str());
#if(CC_TARGET_PLATFORM  == CC_PLATFORM_ANDROID)
		JniMethodInfo minfo;//定义Jni函数信息结构体
		//getStaticMethodInfo 次函数返回一个bool值表示是否找到此函数
		bool isHave = JniHelper::getStaticMethodInfo(minfo, "com/jlx/MixProject/AppActivity", "paste", "(Ljava/lang/String;)V");
		if (!isHave) {
		}
		else {
			jobject para = minfo.env->NewStringUTF(str);
			minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, para);
			CommandManager::getInstance()->showGamePrompt(GetText("USERINFO_COPYID"));
		}
#elif CC_TARGET_PLATFORM  == CC_PLATFORM_IOS
		ClipboardHelper::copyStr(str);
		CommandManager::getInstance()->showGamePrompt(GetText("USERINFO_COPYID"));
#endif
	}
	else if (name.compare(btn_charge_tag) == 0) { //商店
	 IGameShop* gameShop = GameShopFactory::createWithVersion();
	 gameShop->setMenuDelegate(topMenuDelegate);
	 gameShop->open(this->getParent(), Vec2(0, 0), MODULE_LAYER_ORDER_V88, 0);

    }
    else if (name.compare(btn_popularize_tag) == 0) { //推广系统
        /*auto popularizemanager = PopularizeSignManager::create();
        if (popularizemanager) {
            popularizemanager->openAddToParent((Layer*)this->getParent(), MODULE_LAYER_ORDER_V88, WIN_SIZE/2);
        }*/
    }
	else {  // 其他
		GamePromptLayer::create()->showPrompt(GBKToUtf8("程序猿使劲开发中。"));
	}
}

void TopMenuCompositeV88::walletChanged(LLONG money) {

}

void TopMenuCompositeV88::update(float dt) {
	MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
	if (m_money != LogonResult.i64Money) {
		m_money = LogonResult.i64Money;
		l_userMoney->setString(to_string(m_money));
	}
}

void TopMenuCompositeV88::autoMove() {
    auto moveByY = MoveBy::create(0.3f, Vec2(0, csb->getContentSize().height));
    auto reverseMoveByY = moveByY->reverse();
    csb->runAction(Sequence::create(moveByY, reverseMoveByY, nullptr));
}
void TopMenuCompositeV88::moveRight(bool isRight) {
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

void TopMenuCompositeV88::closeChildLayer() {
    if (layerDelegate) {
        layerDelegate->close();
        layerDelegate = nullptr;
    }
}

void TopMenuCompositeV88::setType(MenuType type) {
	//updateTopMenuShow(type);
}

void TopMenuCompositeV88::getRoomSpriteByRoomName(char name[], char fishName[], char* gameName) {
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

void TopMenuCompositeV88::canTouchHead(bool isTouch) {
	UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, isTouch);
}

void TopMenuCompositeV88::headClickCallback(Ref *pSender, Widget::TouchEventType type)
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
