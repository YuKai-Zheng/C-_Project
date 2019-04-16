//
//  InvitedCodeLayerV66.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 14/07/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "InvitedCodeLayerV66.h"
#include "HNNetProtocol/HNPlatformMessage.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "../pattern/CommandManager.h"
#include "HNLobby/GamePrompt.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "base/WechatMgr.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "base/WeChatAndroid.h"
#endif

InviteCodeLayerV66::InviteCodeLayerV66() {
	touchListener = nullptr;
	colorLayer = nullptr;
	m_sureBtn = nullptr;
	m_invitedCode = nullptr;
}

InviteCodeLayerV66::~InviteCodeLayerV66() {

}

bool InviteCodeLayerV66::init() {
	if (!HNLayer::init()) {
		return false;
	}
	colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	colorLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	addChild(colorLayer);

	initTouchEventListener();
	return true;
}

void InviteCodeLayerV66::editBoxReturn(ui::EditBox *editBox) {
	//    std::string string = editBox->getText();
}

void InviteCodeLayerV66::open(cocos2d::Node *parent, cocos2d::Vec2 vec, int zorder) {
	if (parent) {
		this->setPosition(vec);
		parent->addChild(this, zorder);
	}
}

void InviteCodeLayerV66::setColorLayerTouchEnabled(bool bTouch) {
	if (!bTouch) {
		touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
			return true;
		};
		touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
			return;
		};
	}
}

void InviteCodeLayerV66::setCodeType(int type){
	m_codeType = type;
	createBasicUI(type);
}

void InviteCodeLayerV66::createBasicUI(int type) {
	auto bg = ImageView::create("v66/module/set/dikuang_1.png");
	if (bg) {
		bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bg->setPosition(WIN_SIZE / 2);
		bg->setTouchEnabled(true);
		addChild(bg);
		//bottom bar
		auto bar = ImageView::create("v66/module/set/dikuang_2.png");
		if (bar) {
			bar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			bar->setPosition(Vec2(bg->getContentSize().width*0.5f, 1));
			bg->addChild(bar);
		}
		//title
		string titleStr = (type == INVITE) ? "v66/module/yaoqingma/1.png" : "v66/common/tanchuang/tishi.png";
		auto title = ImageView::create(titleStr);
		if (title) {
			title->setPosition(Vec2(bg->getContentSize().width*0.5f, bg->getContentSize().height*0.92f));
			bg->addChild(title);
		}
		//sure
		auto sure = AUIButton::create("v66/button/yellow1/BG.png", "v66/button/yellow1/BG_1.png");
		if (sure) {
			string sureTitle = (type == INVITE) ? "v66/button/yellow1/11.png" : "v66/button/yellow1/15.png";
			sure->setTitle(sureTitle);
			sure->setTitleOffset(Vec2(-4, 4));
			float surePosX = (type == INVITE) ? (bg->getContentSize().width*0.5f) : (bg->getContentSize().width*0.5f - 150);
			sure->setPosition(Vec2(surePosX, bg->getContentSize().height*0.08f));
			if (type == INVITE){
				sure->addTouchEventListener(CC_CALLBACK_2(InviteCodeLayerV66::sureButtonCallback, this));
			}
			else{
				sure->addTouchEventListener(CC_CALLBACK_2(InviteCodeLayerV66::confirmExperienceCodeCallback, this));
			}
			m_sureBtn = sure;
			bg->addChild(sure);
		}
		//share
		if (type == EXPERIENCE)
		{
			auto share = AUIButton::create("v66/button/yellow1/BG.png", "v66/button/yellow1/BG_1.png");
			if (share) {
				share->setTitle("v66/button/yellow1/16.png");
				share->setTitleOffset(Vec2(-4, 4));
				share->setPosition(Vec2(bg->getContentSize().width*0.5f + 150, bg->getContentSize().height*0.08f));
				share->addTouchEventListener(CC_CALLBACK_2(InviteCodeLayerV66::shareExperienceCodeCallback, this));
				bg->addChild(share);
			}
		}

		//content
		string contentStr = (type == INVITE) ? "v66/module/yaoqingma/2.png" : "v66/module/tiyanma/1.png";
		auto content = ImageView::create(contentStr);
		if (content) {
			content->setPosition(Vec2(bg->getContentSize().width*0.5f, bg->getContentSize().height*0.5f));
			bg->addChild(content);
			//text field
			auto textField = TextField::create("", DEFAULT_FONT, 28);
			if (type == INVITE)
			{
				textField->setPlaceHolder("请输入邀请码");
			}
			else
			{
				textField->setPlaceHolder("请输入推广人游戏ID");
			}
			

			if (textField) {
				float fHeightPercent = (type == INVITE) ? 0.12f : 0.32f;
				textField->setPosition(Vec2(content->getContentSize().width*0.5f, content->getContentSize().height*fHeightPercent));
				textField->setVisible(false);
				textField->setContentSize(Size(480, textField->getContentSize().height));
				content->addChild(textField);
				m_invitedCode = HNEditBox::createEditBox(textField, this);
				if (m_invitedCode) {
					m_invitedCode->setInput(HNEditBox::InputMode::PHONE_NUMBER);
					m_invitedCode->setPosition(textField->getPosition());
				}
			}
		}
	}
}

void InviteCodeLayerV66::initTouchEventListener() {
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
		return true;
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
		closeSettlementLayer();
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);
}

void InviteCodeLayerV66::closeSettlementLayer() {
	this->removeFromParent();
}

void InviteCodeLayerV66::sureButtonCallback(cocos2d::Ref *pSender, Widget::TouchEventType type) {
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}
	if (m_invitedCode && m_codeType == INVITE) {
		std::string string = m_invitedCode->getText();
		if (string.compare("") == 0) {
			CommandManager::getInstance()->showGamePrompt(GetText("INVITE_CODE_TIP_INPUT"));
		}
		else {
			MSG_GP_S_InviteCode invite;
			memset(&invite, 0, sizeof(MSG_GP_S_InviteCode));
			invite.userID = PlatformLogic()->getInstance()->loginResult.dwUserID;
			strcpy(invite.szInviteCode, string.c_str());

			PlatformLogic()->sendData(MDM_GP_SetInviteCode, ASS_GP_SetInviteCode, &invite, sizeof(MSG_GP_S_InviteCode), HN_SOCKET_CALLBACK(InviteCodeLayerV66::setInvitedCodeSelector, this));
		}
		return;
	}
}

void InviteCodeLayerV66::confirmExperienceCodeCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	if (m_invitedCode && m_codeType == EXPERIENCE)
	{
		std::string string = m_invitedCode->getText();
		if (string.compare("") == 0) {
			CommandManager::getInstance()->showGamePrompt(GetText("EXPERIENCE_CODE_TIP_INPUT"));
		}
		else {
			requestBindExperienceCode(1);
		}
		return;
	}
}

void InviteCodeLayerV66::shareExperienceCodeCallback(cocos2d::Ref *pSender, Widget::TouchEventType type){
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	createSelectShareTypeLayer();
}

bool InviteCodeLayerV66::setInvitedCodeSelector(HN::HNSocketMessage *socketMessage) {
	CCAssert(sizeof(MSG_GP_S_InviteCode) == socketMessage->objectSize, "sizeof(MSG_GP_S_InviteCode) != objectSize");
	if (sizeof(MSG_GP_S_InviteCode) != socketMessage->objectSize)    return false;
	MSG_GP_S_InviteCode* msg = (MSG_GP_S_InviteCode*)socketMessage->object;
	if (msg) {
		if (socketMessage->messageHead.bHandleCode == 1) {
			//成功
			PlatformLogic()->getInstance()->loginResult.i64Bank += msg->songMoney;
			auto prompt = GamePromptLayer::create();
			if (prompt) {
				prompt->showPrompt(GetText("INVITE_CODE_SUCCESS"));
				prompt->setColorLayerTouchEnabled(false);
				prompt->setOkayCallBack([&]() {
					closeSettlementLayer();
				});
			}
		}
		else {
			CommandManager::getInstance()->showGamePrompt(GetText("INVITE_CODE_FAILURE"));
		}
	}

	return true;
}

void InviteCodeLayerV66::requestBindExperienceCode(int iActionType){
	MSG_GP_S_TiYanMa experienceCode;
	memset(&experienceCode, 0, sizeof(MSG_GP_S_TiYanMa));
	experienceCode.userID = PlatformLogic()->getInstance()->loginResult.dwUserID;
	experienceCode.flag = iActionType;
	if (iActionType == 1){
		experienceCode.tiYanMaID = atoi(m_invitedCode->getString().c_str());
	}
	PlatformLogic()->sendData(MDM_GP_SetTiYanMa, ASS_GP_SetTiYanMa, &experienceCode, sizeof(MSG_GP_S_TiYanMa), HN_SOCKET_CALLBACK(InviteCodeLayerV66::experienceCodeSelector, this));
}

bool InviteCodeLayerV66::experienceCodeSelector(HNSocketMessage* socketMessage){
	CCAssert(sizeof(MSG_GP_S_TiYanMa) == socketMessage->objectSize, "sizeof(MSG_GP_S_TiYanMa) != objectSize");
	MSG_GP_S_TiYanMa* msg = (MSG_GP_S_TiYanMa*)socketMessage->object;
	if (msg) {
		if (msg->responseInfo == 0)
		{
			switch (socketMessage->messageHead.bHandleCode)
			{
			case 2:
			{
					  //邀请码错误
					  CommandManager::getInstance()->showGamePrompt(GetText("INVITE_CODE_FAILURE"));
					  break;
			}
			case 3:
			{	//邀请码使用成功
					  auto prompt = GamePromptLayer::create();
					  if (prompt) {
						  prompt->showPrompt(GetText("INVITE_CODE_SUCCESS"));
						  prompt->setColorLayerTouchEnabled(false);
						  prompt->setOkayCallBack([&]() {
							  closeSettlementLayer();
						  });
					  }
					  break;
			}
			case 4:
			{
					  //已使用
					  CommandManager::getInstance()->showGamePrompt(GetText("INVITE_CODE_TIP_USED"));
					  break;
			}
			case 5:
			{
					  //其他账号已使用
					  CommandManager::getInstance()->showGamePrompt(GetText("INVITE_CODE_TIP_OTHER_USED"));
					  break;
			}
			default:
				break;
			}
		}
		else if (msg->responseInfo == 1)
		{
			switch (socketMessage->messageHead.bHandleCode)
			{
			case 1:
			{
					  CommandManager::getInstance()->showGamePrompt("推广用户不存在");
					  break;
			}
			case 2:
			{
					  CommandManager::getInstance()->showGamePrompt("被推广用户不存在");
					  break;
			}
			case 3:
			{		  CommandManager::getInstance()->showGamePrompt("推广用户跟被推广用户不能是同一个");
			break;
			}
			case 4:
			{
					  CommandManager::getInstance()->showGamePrompt("该ID不具备推广权限（请先申请成为推广员）");
					  break;
			}
			case 5:
			{
					  CommandManager::getInstance()->showGamePrompt("推广员不能为自己的小号");
					  break;
			}
			case 6:
			{
					  CommandManager::getInstance()->showGamePrompt("已设置过推广员，请不要重复设置");
					  break;
			}
			case 7:
			{
					  CommandManager::getInstance()->showGamePrompt("设置成功");
					  break;
			}
			case 8:
			{
					  CommandManager::getInstance()->showGamePrompt("已是推广人，不能成为被推广人");
					  break;
			}
			default:
				break;
			}
		}
	}

	return true;
}

void InviteCodeLayerV66::createSelectShareTypeLayer()
{
	auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
	colorLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	addChild(colorLayer);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
		return true;
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
		colorLayer->removeFromParent();
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);

	auto bg = ImageView::create("v66/common/tanchuang/bg.png");
	bg->setPosition(WIN_SIZE / 2);
	bg->setTouchEnabled(true);
	colorLayer->addChild(bg);
	bg->setScale(0.5f);

	auto bgSize = bg->getContentSize();

	auto title = ImageView::create("v66/module/fenxiang/0.png");
	title->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height - 44));
	bg->addChild(title);

	auto friendCircleBtn = Button::create("v66/module/fenxiang/1.png");
	friendCircleBtn->setPosition(Vec2(bgSize.width / 2 - 180, bgSize.height / 2 - 40));
	friendCircleBtn->setTag(0);
	bg->addChild(friendCircleBtn);
	friendCircleBtn->addTouchEventListener(CC_CALLBACK_2(InviteCodeLayerV66::shareCallback, this));

	auto friendBtn = Button::create("v66/module/fenxiang/2.png");
	friendBtn->setPosition(Vec2(bgSize.width / 2 + 180, bgSize.height / 2 - 40));
	friendBtn->setTag(1);
	bg->addChild(friendBtn);
	friendBtn->addTouchEventListener(CC_CALLBACK_2(InviteCodeLayerV66::shareCallback, this));
}

void InviteCodeLayerV66::shareCallback(cocos2d::Ref *pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) {
		return;
	}

	bool bIsInstallWeChat = false;
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	bIsInstallWeChat = WechatMgr::isAppInstalled();
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bIsInstallWeChat = WeChatAndroid::getInstance()->isAppInstalled();
#endif
	if (!bIsInstallWeChat)
	{
		CommandManager::getInstance()->showGamePrompt(GBKToUtf8("请安装最新微信客户端"));
		return;
	}

	auto btn = (Button*)pSender;
	int tag = btn->getTag();

	char title[128] = { 0 };
	if (tag == 0){
		sprintf(title, GetText("EXPERIENCE_CODE_INVITE_TITLE1"), PlatformLogic()->loginResult.dwUserID);
	}
	else{
		sprintf(title, "%s", GetText("EXPERIENCE_CODE_INVITE_TITLE2"));
	}

	char content[256] = { 0 };
	sprintf(content, GetText("EXPERIENCE_CODE_INVITE_CONTENT"), PlatformLogic()->loginResult.dwUserID);

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	WechatMgr::shareContentLinkToWechatOrTimeLineSession(title, content, tag);
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	WeChatAndroid::getInstance()->shareWebpage(tag, title, content, getWebServerUrl() + "/share.aspx");
#endif

	if (btn->getParent())
	{
		btn->getParent()->getParent()->removeFromParent();
	}
}
