#include "D21GameUserMessageBoxV99.h"
#include "HNLobbyExport.h"
#include "HNNetExport.h"
#include <string>

namespace D21
{
	static const INT USERDATA_BOX		= 10;    //玩家资料框

	static const char* MESSAGE_BOX_BG	= "d21/userinfo_messagebox/table_userinfo_bg.png";
	static const char* HEAD_BOX			= "d21/userinfo_messagebox/head_box.png";
	static const char* MEN_HEAD			= "d21/userinfo_messagebox/men_head.png";
	static const char* WOMEN_HEAD		= "d21/userinfo_messagebox/women_head.png";

	static const char* MEN_ICON			= "d21/userinfo_messagebox/boy.png";
	static const char* WOMEN_ICON		= "d21/userinfo_messagebox/girl.png";
	static const char* COIN_ICON		= "d21/userinfo_messagebox/jinbi.png";
	static const char* UDES_BOX			= "d21/userinfo_messagebox/Udes_box.png";


	GameUserMessageBoxV99* GameUserMessageBoxV99::createMessageBox(cocos2d::Node* parent, int userID)
	{
		auto pRet = new GameUserMessageBoxV99();
		if (pRet && pRet->init(parent, userID))
		{
			pRet->autorelease();
			return pRet;
		}
		delete pRet;
		pRet = nullptr;
		return pRet;
	}

	bool GameUserMessageBoxV99::init(cocos2d::Node* parent, int userID)
	{
		if (!HNDialogBase::init(parent)) return false;

		Size winSize = Director::getInstance()->getWinSize();

		//创建一个点击事件
		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);//阻止触摸向下传递
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto sp = this->getChildByTag(USERDATA_BOX);
			if(nullptr != sp)
			{
				Rect rect = Rect(0, 0, sp->getContentSize().width, sp->getContentSize().height);        
				auto pos = sp->convertToNodeSpace(touch->getLocation());
				if (!rect.containsPoint(pos))
				{
					hide();
				}
				return true;
			}
			else
			{
				return false;
			}
			
		};
		//把点击监听添加到监听序列中,因为只创建了一个监听,一个监听只能绑定到一个对象上,所以其他对象要使用监听则克隆
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

		//玩家信息结构体
		UserInfoStruct* LogonResult = UserInfoModule()->findUser(userID);
		if (nullptr == LogonResult)
		{
			return true;
		}

		// 玩家资料框
		auto userDataBox = Sprite::create(MESSAGE_BOX_BG);
		userDataBox->setPosition(Vec2(winSize.width/2, winSize.height/2));
		addChild(userDataBox, 10, USERDATA_BOX);
		_userDataBoxRect = userDataBox->getBoundingBox();

		// 玩家头像
		auto userIcon = Sprite::create(LogonResult->bBoy ? MEN_HEAD : WOMEN_HEAD);
		userIcon->setPosition(Vec2(userDataBox->getContentSize().width*0.13, userDataBox->getContentSize().height*0.8));
		userIcon->setScale(1.15f);
		userDataBox->addChild(userIcon, 2);

		// 玩家头像背景框
		auto headBox = Sprite::create(HEAD_BOX);
		headBox->setPosition(Vec2(userDataBox->getContentSize().width*0.13, userDataBox->getContentSize().height*0.8));
		userDataBox->addChild(headBox, 3);


		char str[128];

		// 玩家昵称
		sprintf(str, "%s", LogonResult->nickName);
		auto uNickName = createLabel(gFontConfig_22, GBKToUtf8(str), colorGold);
		uNickName->setAnchorPoint(Vec2(0, 0.5));
		//uNickName->setPosition(Vec2(userDataBox->getContentSize().width*0.4f, userDataBox->getContentSize().height*0.89f));
		uNickName->setPosition(Vec2(userDataBox->getContentSize().width*0.38f, userDataBox->getContentSize().height*0.9f));
		userDataBox->addChild(uNickName, 2);

		// 玩家性别
		Sprite* uSex = nullptr;
		if (LogonResult->bBoy) uSex = Sprite::create(MEN_ICON);
		else uSex = Sprite::create(WOMEN_ICON);
		//uSex->setScale(0.6);
		uSex->setAnchorPoint(Vec2(0, 0.5));
		//uSex->setPosition(Vec2(userDataBox->getContentSize().width*0.76f, userDataBox->getContentSize().height*0.89f));
		uSex->setPosition(Vec2(userDataBox->getContentSize().width*0.3f, userDataBox->getContentSize().height*0.9f));
		userDataBox->addChild(uSex, 2);

		// 玩家金币
		Sprite* uCoin = nullptr;
		uCoin = Sprite::create(COIN_ICON);
		//uCoin->setScale(0.6);
		uCoin->setAnchorPoint(Vec2(0, 0.5));
		uCoin->setPosition(Vec2(userDataBox->getContentSize().width*0.3f, userDataBox->getContentSize().height*0.75f));
		userDataBox->addChild(uCoin, 2);

		sprintf(str, "%lld", LogonResult->i64Money);
		auto UserMoney = createLabel(gFontConfig_20, str, colorGold);
		UserMoney->setAnchorPoint(Vec2(0, 0.5));
		UserMoney->setPosition(Vec2(userDataBox->getContentSize().width*0.38f, userDataBox->getContentSize().height*0.75f));
		userDataBox->addChild(UserMoney, 2);

		Label* displyWord = nullptr;

		//// 玩家分数
		//displyWord = createLabel(gFontConfig_20, GBKToUtf8("分数"), colorGold);
		//displyWord->setAnchorPoint(Vec2(0, 0.5));
		//displyWord->setPosition(Vec2(userDataBox->getContentSize().width*0.3f, userDataBox->getContentSize().height*0.7f));
		//userDataBox->addChild(displyWord, 2);
		//sprintf(str, "%d", LogonResult->dwPoint);
		//auto uSCR = createLabel(gFontConfig_20, str, colorGold);
		//uSCR->setAnchorPoint(Vec2(0, 0.5));
		//uSCR->setPosition(Vec2(userDataBox->getContentSize().width*0.4f, userDataBox->getContentSize().height*0.7f));
		//userDataBox->addChild(uSCR, 2);

		// 玩家经验
		displyWord = createLabel(gFontConfig_20, GBKToUtf8("经验"), colorWhite);
		displyWord->setAnchorPoint(Vec2(0, 0.5));
		displyWord->setPosition(Vec2(userDataBox->getContentSize().width*0.3f, userDataBox->getContentSize().height*0.55f));
		userDataBox->addChild(displyWord, 2);
		sprintf(str, "%d", LogonResult->dwPoint);
		auto uEXP = createLabel(gFontConfig_20, str, colorWhite);
		uEXP->setAnchorPoint(Vec2(0, 0.5));
		uEXP->setPosition(Vec2(userDataBox->getContentSize().width*0.4f, userDataBox->getContentSize().height*0.55f));
		userDataBox->addChild(uEXP, 2);

		// 玩家胜局
		displyWord = createLabel(gFontConfig_20, GBKToUtf8("胜局"), colorWhite);
		displyWord->setAnchorPoint(Vec2(0, 0.5));
		displyWord->setPosition(Vec2(userDataBox->getContentSize().width*0.65f, userDataBox->getContentSize().height*0.55f));
		userDataBox->addChild(displyWord, 2);

		sprintf(str, "%d", LogonResult->uWinCount);
		auto uWinNum = createLabel(gFontConfig_20, str, colorWhite);
		uWinNum->setAnchorPoint(Vec2(0, 0.5));
		uWinNum->setPosition(Vec2(userDataBox->getContentSize().width*0.76f, userDataBox->getContentSize().height*0.55f));
		userDataBox->addChild(uWinNum, 2);

		// 玩家负局
		displyWord = createLabel(gFontConfig_20, GBKToUtf8("负局"), colorWhite);
		displyWord->setAnchorPoint(Vec2(0, 0.5));
		displyWord->setPosition(Vec2(userDataBox->getContentSize().width*0.3f, userDataBox->getContentSize().height*0.45f));
		userDataBox->addChild(displyWord, 2);

		sprintf(str, "%d", LogonResult->uLostCount);
		auto uLostNum = createLabel(gFontConfig_20, str, colorWhite);
		uLostNum->setAnchorPoint(Vec2(0, 0.5));
		uLostNum->setPosition(Vec2(userDataBox->getContentSize().width*0.4f, userDataBox->getContentSize().height*0.45f));
		userDataBox->addChild(uLostNum, 2);

		// 玩家和局
		displyWord = createLabel(gFontConfig_20, GBKToUtf8("和局"), colorWhite);
		displyWord->setAnchorPoint(Vec2(0, 0.5));
		displyWord->setPosition(Vec2(userDataBox->getContentSize().width*0.65f, userDataBox->getContentSize().height*0.45f));
		userDataBox->addChild(displyWord, 2);

		sprintf(str, "%d", LogonResult->uMidCount);
		auto uMidNum = createLabel(gFontConfig_20, str, colorWhite);
		uMidNum->setAnchorPoint(Vec2(0, 0.5));
		uMidNum->setPosition(Vec2(userDataBox->getContentSize().width*0.76f, userDataBox->getContentSize().height*0.45f));
		userDataBox->addChild(uMidNum, 2);

		// 个性签名
		displyWord = createLabel(gFontConfig_20, GBKToUtf8("个性签名"), colorWhite);
		displyWord->setAnchorPoint(Vec2(0, 0.5));
		displyWord->setPosition(Vec2(userDataBox->getContentSize().width*0.02f, userDataBox->getContentSize().height*0.38f));
		userDataBox->addChild(displyWord, 2);

		auto uDes_Box = Sprite::create(UDES_BOX);
		uDes_Box->setPosition(Vec2(userDataBox->getContentSize().width / 2, userDataBox->getContentSize().height*0.18));
		userDataBox->addChild(uDes_Box, 2); 

		sprintf(str, "%s", LogonResult->szSignDescr);
		//sprintf(str, "%s", "我是中国人");
		auto uDesign = createLabel(gFontConfig_18, GBKToUtf8(str), colorWhite);
		uDesign->setAnchorPoint(Vec2(0, 0.5));
		uDesign->setPosition(Vec2(userDataBox->getContentSize().width*0.1f, userDataBox->getContentSize().height*0.28f));
		userDataBox->addChild(uDesign, 2);

		return true;
	}

		
}

