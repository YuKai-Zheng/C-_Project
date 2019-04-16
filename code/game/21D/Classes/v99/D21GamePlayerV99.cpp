#include "D21GamePlayerV99.h"
#include "D21GameUserMessageBox.h"
#include "D21MessageHead.h"

#include "HNLobbyExport.h"
#include "HNLobby\globel.h"


namespace D21
{
	static const int BUTTON_TAG_CHECK	=	100	;		//100时表示看牌
	static const int  BUTTON_TAG_CALL	=	101	;		//101时表示跟注
	static const int  BUTTON_TAG_ALLIN	=	102	;		//102时表示全下

	static const int  BUTTON_TAG_BET	=	201	;		//201时表示下注
	static const int  BUTTON_TAG_RAISE	=	202	;		//202时表示加注

	static const int  BUTTON_TAG_FLOD	=	301	;		//301时表示弃牌

	static Color3B USER_NAME_COLOR(240, 240, 240);		// 玩家名称颜色

	static Color3B USER_CHIPTEXT_COLOR(122,162,116);	// 玩家筹码文本颜色

	static Color3B USER_STATUS_COLOR(201,207,80);		// 玩家状态颜色


	static const char* D21_PLAYER_V99_CSB = "Games/12100404/PlayerHead_%d.csb";

	static const char* WOMEN_HEAD =  "d21/table/women_head.png";
	static const char* MEN_HEAD =  "d21/table/men_head.png";
	static const char* BG_SRC  = "d21/table/seat.png";
	static const char* BG_LINE_SRC  = "";
	static const char* PROGRESS_SRC = "v99/goldenflower/table/countdown.png";

	//PlayerUIV99* PlayerUIV99::create(LLONG userId, BYTE bViewSeatNo)
	//{
	//	Node* player = Node::create();
	//	if(player /*&& ((PlayerUIV99*)player)->init(userId, bViewSeatNo)*/)
	//	{
	//		//player->autorelease();
	//		return player;
	//	}
	//	CC_SAFE_DELETE(player);
	//	return nullptr;
	//}

	bool PlayerUIV99::init()
	{
		if (!HN::HNNode::init())
		{
			return false;
		}

		return true;
	}

	bool PlayerUIV99::initUI(LLONG userId, BYTE bViewSeatNo)
	{
		//if(!HN::HNNode::init())
		//{
		//	return false;
		//}

		auto pUser = UserInfoModule()->getInstance()->findUser(userId);
		if (!pUser)
			return false;

		_userId = userId;
		_viewSeatNo = bViewSeatNo;

		char str[80] = { 0 };
		sprintf(str, D21_PLAYER_V99_CSB, bViewSeatNo);
		_root = CSLoader::createNode(str);
		this->addChild(_root);

		_bg = dynamic_cast<ImageView*>(_root->getChildByName("Image_HeadFrame"));
		_headSp = dynamic_cast<ImageView*>(_bg->getChildByName("Image_Head"));
		sprintf(str, "v99/commonHead/%d.png", pUser->bLogoID);
		_headSp->loadTexture(str);

		Size bgSize = _bg->getContentSize();

// 		auto timerSp = Sprite::create(PROGRESS_SRC);
// 		_progressTimer = ProgressTimer::create(timerSp);
// 		_progressTimer->setScaleX(bgSize.width / timerSp->getContentSize().width);
// 		_progressTimer->setScaleY(bgSize.height / timerSp->getContentSize().height);
// 		_progressTimer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
// 		_progressTimer->setPosition(_bg->getPosition());
// 		_root->addChild(_progressTimer, _bg->getLocalZOrder() - 1);
// 		_progressTimer->setPercentage(0);
// 		_progressTimer->setVisible(false);
// 		_progressTimer->setReverseProgress(true);

		_readyLogo = dynamic_cast<ImageView*>(_root->getChildByName("ready_label"));
		_readyLogo->setVisible(false);
		_zhuangLogo = dynamic_cast<ImageView*>(_root->getChildByName("Image_Banker"));
		_zhuangLogo->setVisible(false);

		_toutleSp = dynamic_cast<ImageView*>(_root->getChildByName("Image_BetBg"));
		_chipToutleText = dynamic_cast<Text*>(_root->getChildByName("Text_BetMoney"));
		_chipToutleText->setString("");

		_nameText = dynamic_cast<Text*>(_root->getChildByName("Text_NickName"));
		_chipText = dynamic_cast<Text*>(_root->getChildByName("AtlasLabel_Gold"));
		_nameText->setString(ForceGBKToUtf8(pUser->nickName));
		_chipText->setString(Tools::addComma(pUser->i64Money));

		return true;
	}

	PlayerUIV99::PlayerUIV99()
		: _chip(0)
		, _userId(0)
		, _leftTime(0)
		, _bg(nullptr)
		, _headSp(nullptr)
		, _nameText(nullptr)
		, _chipText(nullptr)
		, _progressTimer(nullptr)
	{
		memset(_handCard, 0, sizeof(_handCard));
		memset(_handCardValue, 0, sizeof(_handCardValue));	
	}

	PlayerUIV99::~PlayerUIV99()
	{
		unschedule(schedule_selector(PlayerUIV99::onWaitTimer));
	}

	//显示准备icon
	void PlayerUIV99::showReady(bool bShow)
	{
		_readyLogo->setVisible(bShow);
	}

	//显示庄icon
	void PlayerUIV99::showBanker(bool bShow, bool bAniamtion)
	{
		if (bAniamtion)
		{
			Vec2 beginPos = WIN_SIZE / 2;
			Vec2 converPos = this->convertToNodeSpace(beginPos);
			Vec2 endPos = _zhuangLogo->getPosition();

			auto node = CSLoader::createNode("v99/animation/banker/bankerglow.csb");
			node->setPosition(converPos);
			addChild(node);

			std::function<void(void)> func = [=](){

				_zhuangLogo->setVisible(bShow);
				_zhuangLogo->setPosition(converPos);
				_zhuangLogo->runAction(Sequence::create(MoveTo::create(0.5f, endPos), CallFunc::create([=](){

					auto bankerNode = CSLoader::createNode("v99/animation/banker/banker.csb");
					bankerNode->setPosition(endPos);
					addChild(bankerNode);

					auto bankerAction = CSLoader::createTimeline("v99/animation/banker/banker.csb");
					bankerAction->play("banker", false);
					bankerNode->runAction(bankerAction);
				}), nullptr));
			};

			auto action = CSLoader::createTimeline("v99/animation/banker/bankerglow.csb");
			action->play("bankerglow", false);
			action->addFrameEndCallFunc(15, "glow", func);
			node->runAction(action);
		}
		else
		{
			_zhuangLogo->setVisible(bShow);
		}
	}

	void PlayerUIV99::setUserMoney(LLONG i64Money)
	{
		_chip = i64Money;
		if (_chip == -1)
		{
			_chipText->setString("");
		}
		else
		{
			_chipText->setString(Tools::addComma(i64Money));
			_chipText->runAction(Sequence::create(ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5f, 1.0f), nullptr));
		}	
	}

	LLONG PlayerUIV99::getUserMoney()
	{
		return _chip;
	}

	void PlayerUIV99::setStatus(const char* status, int value)
	{
		_nameText->setString(status);
		_nameText->setColor(USER_STATUS_COLOR);
		return ;				//yanghuanSearch
		//_nameText->runAction(Sequence::create(DelayTime::create(2.0f), CallFuncN::create(CC_CALLBACK_1(PlayerUIV99::resetName,this)) ,nullptr));
		
	}

	std::string PlayerUIV99::getName()
	{
		return _name;
	}

	void PlayerUIV99::updateBetMoney(LLONG i64Money, LLONG i64TotalMoney)
	{
		_chipToutleText->setString(Tools::addComma(i64TotalMoney));
		_chipToutleText->runAction(Sequence::create(ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5f, 1.0f), nullptr));
	}

	void PlayerUIV99::setHandCard(int index, BYTE value)
	{
		_handCard[index]->setCardValue(value);
		_handCard[index]->setVisible(true);
		if(value != 0x00)
		{
			this->reorderChild(_handCard[index], 0);
		}
		else
		{
			this->reorderChild(_handCard[index], 0);
		}	
	}

	void PlayerUIV99::hideHandCard()
	{
		for (int i = 0; i < 5; i++)
		{
			if (nullptr != _handCard[0])
			{
				_handCard[i]->setVisible(false);
			}
		}
	}

	void PlayerUIV99::startWait(int time, int total)
	{
		if(time > 1 && total > 1)
		{
			_leftTime = time;
			_totalTime = total;
			schedule(schedule_selector(PlayerUIV99::onWaitTimer),0.1f);
		}

	}

	void PlayerUIV99::stopWait()
	{
		_progressTimer->setVisible(false);
		unschedule(schedule_selector(PlayerUIV99::onWaitTimer));
	}

	void PlayerUIV99::playWin(int type)
	{
		char tmp[50] = {0};
		sprintf(tmp,"cardtype-%d.png", type);
		auto sp = Sprite::createWithSpriteFrameName(tmp);
		if(sp == nullptr)
		{
			return;
		}
		sp->setPosition(_bg->getContentSize().width / 2, _bg->getContentSize().height / 2);
		this->addChild(sp,6,"winSprite");
		sp->runAction(Sequence::create(
			EaseOut::create(MoveBy::create(0.5f, Vec2(0, 50)), 2.5f),
			DelayTime::create(8.0f),
			RemoveSelf::create(true),
			nullptr));
	}

	void PlayerUIV99::stopPlayWin()
	{
		auto sp  = this->getChildByName("winSprite");
		if (nullptr == sp)
		{
			return ;
		}
		sp->stopAllActions();
		sp->removeFromParent();
	}

	void PlayerUIV99::onWaitTimer(float delta)
	{
		_leftTime -= 0.1f;
		if(_leftTime < 0)
		{
			stopWait();
			_progressTimer->setVisible(false);
			return;
		}

		Color3B color(0,255,0);//GREEN
		_progressTimer->setVisible(true);

		float ratio = _leftTime / _totalTime;
		int value = ratio * 510;
		if(value > 255)
		{
			color.r  = 510 - value;
		}
		else
		{
			color.r = 255;
			color.g = value;
		}

		_progressTimer->setColor(color);
		_progressTimer->setPercentage(ratio * 100);
	}
}
