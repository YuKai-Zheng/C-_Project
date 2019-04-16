#include "D21GamePlayer.h"
#include "D21GameUserMessageBox.h"
#include "D21MessageHead.h"

#include "HNLobbyExport.h"


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

	static const char* WOMEN_HEAD =  "d21/table/women_head.png";
	static const char* MEN_HEAD =  "d21/table/men_head.png";
	static const char* BG_SRC  = "d21/table/seat.png";
	static const char* BG_LINE_SRC  = "";
	static const char* PROGRESS_SRC  = "";

	PlayerUI* PlayerUI::create(LLONG userId)
	{
		PlayerUI* player = new PlayerUI();
		if(player && player->init(userId))
		{
			player->autorelease();
			return player;
		}
		CC_SAFE_DELETE(player);
		return nullptr;
	}

	bool PlayerUI::init(LLONG userId)
	{
		if(!HN::HNLayer::init())
		{
			return false;
		}
		_userId = userId;

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(PlayerUI::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(PlayerUI::onTouchEnded, this);
		listener->setSwallowTouches(true);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		_bg = HNSprite::create(BG_SRC);
		this->addChild(_bg,1);
		_bg->setOpacity(50);

		Size size = _bg->getContentSize();
		this->setContentSize(size);
		this->ignoreAnchorPointForPosition(false);

		_bg->setAnchorPoint(Vec2::ZERO);
		_bg->setPosition(Vec2::ZERO);

		_nameText = Text::create("", "Arial", 14);
		_nameText->setAnchorPoint(Vec2(0.5f, 0.5f));
		_nameText->setPosition(Vec2(size.width / 2, size.height / 4 * 3 + 20));
		_nameText->setColor(USER_NAME_COLOR);
		this->addChild(_nameText,3);

		_chipText = Text::create("", "Arial", 16);
		_chipText->setAnchorPoint(Vec2(0.5f, 0.5f));
		_chipText->setPosition(Vec2(size.width / 2 , size.height / 4 - 20));
		_chipText->setColor(USER_CHIPTEXT_COLOR);
		this->addChild(_chipText,3);

		return true;
	}

	PlayerUI::PlayerUI()
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

	PlayerUI::~PlayerUI()
	{
		unschedule(schedule_selector(PlayerUI::onWaitTimer));
	}

	void PlayerUI::setUserId(LLONG userId)
	{
		_userId = userId;
	}

	void PlayerUI::setUserName(BYTE seatNo, const std::string& name)
	{
		_name = name;
		_nameText->setString(name);
		_nameText->setColor(USER_NAME_COLOR);
	}

	void PlayerUI::setSex(bool men)
	{
		Size size = _bg->getContentSize();
		_headSp = HNSprite::create(men ? MEN_HEAD: WOMEN_HEAD);
		_headSp->setAnchorPoint(Vec2(0.5f, 0.5f));
		_headSp->ignoreAnchorPointForPosition(false);
		_headSp->setPosition(size.width / 2, size.height / 2);
		this->addChild(_headSp, 3);
	}

	void PlayerUI::romveHead()
	{
		if(nullptr == _headSp) return;
		this->removeChild(_headSp);
	}

	void PlayerUI::setChip(LLONG chip)
	{
		_chip = chip;
		if (_chip == -1)
		{
			_chipText->setString("");
		}
		else
		{
			char tmp[128] = {0};
			sprintf(tmp, "%lld", _chip);
			_chipText->setString(tmp);
			_chipText->runAction(Sequence::create(ScaleTo::create(0.5f, 1.5f), ScaleTo::create(0.5f, 1.0f), nullptr));
		}	
	}

	void PlayerUI::setStatus(const char* status, int value)
	{
		_nameText->setString(status);
		_nameText->setColor(USER_STATUS_COLOR);
		return ;				//yanghuanSearch
		_nameText->runAction(Sequence::create(DelayTime::create(2.0f), CallFuncN::create(CC_CALLBACK_1(PlayerUI::resetName,this)) ,nullptr));
		
	}

	void PlayerUI::resetName(Node* pNode)
	{
		Text* text = (Text*)pNode;
		text->setString(_name);
		//text->setColor(Color3B(240,240,240));
		text->setColor(USER_STATUS_COLOR);
	}

	//std::string PlayerUI::getName() const
	//{
	//	return _name;
	//}

	LLONG PlayerUI::getUserMoney()
	{
		return _chip;
	}

	void PlayerUI::setHandCard(int index, BYTE value)
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

	void PlayerUI::hideHandCard()
	{
		for (int i = 0; i < 5; i++)
		{
			if (nullptr != _handCard[0])
			{
				_handCard[i]->setVisible(false);
			}
		}
	}

	void PlayerUI::startWait(int time, int total)
	{
		if(time > 1 && total > 1)
		{
			_leftTime = time;
			_totalTime = total;
			schedule(schedule_selector(PlayerUI::onWaitTimer),0.1f);
		}

	}

	void PlayerUI::stopWait()
	{
		_progressTimer->setVisible(false);
		unschedule(schedule_selector(PlayerUI::onWaitTimer));
	}

	void PlayerUI::playWin(int type)
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

	void PlayerUI::stopPlayWin()
	{
		auto sp  = this->getChildByName("winSprite");
		if (nullptr == sp)
		{
			return ;
		}
		sp->stopAllActions();
		sp->removeFromParent();
	}

	void PlayerUI::onWaitTimer(float delta)
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

	bool PlayerUI::onTouchBegan(Touch *touch, Event *unused_event)
	{
		if(_bg == nullptr)
		{
			return false;
		}
		Rect rect = _bg->getBoundingBox();
		Vec2 pos = this->convertToNodeSpace(touch->getLocation());
		if(rect.containsPoint(pos))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void PlayerUI::onTouchEnded(Touch *touch, Event *unused_event)
	{
		if(_bg != nullptr)
		{
			Rect rect = _bg->getBoundingBox();
			if(!rect.containsPoint(touch->getLocation()))
			{
				auto playerInfo = GameUserMessageBox::createMessageBox(this->getParent()->getParent(), _userId);
				playerInfo->setHidePoint(this->getPosition());
				playerInfo->setScale(1.5f);
				this->getParent()->reorderChild(playerInfo, 500);
				if(playerInfo != nullptr)
				{
					playerInfo->show();
				}
			}
		}
	}

}
