#include "GoldenFlowerGamePlayerV88.h"
#include "GoldenFlowerPokerCardV88.h"
#include "GoldenFlowerGameTableUIV88.h"
//#include "GoldenFlowerGameUserMessageBox.h"
#include "GoldenFlowerMessageHead.h"
#include "GoldenFlowerResourceMgr.h"
#include "HNLobby/v66/PlatformResV66.h"

#include "HNCommon/HNConverCode.h"
#include "HNLobbyExport.h"
#include "Sound/HNAudioEngine.h"
#include "Tool/Tools.h"
#include "UI/FloatCoin.h"
/************************line*********************************/
namespace goldenflower {
//#define WOMEN_HEAD      "goldenflower/table/women_head%d.png"
//#define MEN_HEAD        "goldenflower/table/men_head%d.png"

static const char* FONT_RES             = FONT_TTF_FZCY;

static const char* BG_BOTTOM_SRC        = "hallimg/niuniu/5.png";
static const char* BG_EDGE_SRC          = "hallimg/niuniu/5.png";
static const char* BG_TOP_SRC           = "hallimg/niuniu/5.png";

static const char* PROGRESS_SRC         = "Games/12101106/goldenflower/table/countdown.png";
static const char* CHIP_BACKGROUND      = "Games/12101106/goldenflower/table/gamingRoomChipBG.png";

static const char* CARD_LOSE_LABEL_RES = "Games/12101106/goldenflower/table/card_lose_label.png";


static const char* ANIMATION_VS_EFFECT_CSB = "v88/animation/common/bipai/bipai_2ren_portraits.csb";

PlayerUIV88* PlayerUIV88::create(LLONG userId, BYTE seatNo) {
    PlayerUIV88* player = new PlayerUIV88(userId, seatNo);
    if(player && player->init()) {
        player->autorelease();
        return player;
    } else {
        CC_SAFE_DELETE(player);
        return nullptr;
    }
}

PlayerUIV88::PlayerUIV88(LLONG userId, BYTE seatNo)
    : _chip(0)
    , _headSp(nullptr)
    , _nameText(nullptr)
    , _chipText(nullptr)
    , _bg(nullptr)
    , _progressTimer(nullptr)
    , _leftTime(0)
    , _userId(userId)
    , _seatNo(seatNo)
    , _isPlaying(false)
    , _isQuit(false)
    , _loseCardLabel(nullptr) {
    memset(_handCard, 0, sizeof(_handCard));
}

PlayerUIV88::~PlayerUIV88() {
    unschedule(schedule_selector(PlayerUIV88::onWaitTimer));
}

bool PlayerUIV88::init() {
    if(!HN::HNLayer::init()) {
        return false;
    }

    //auto listener = EventListenerTouchOneByOne::create();
    //listener->onTouchBegan = CC_CALLBACK_2(PlayerUIV88::onTouchBegan, this);
    //listener->onTouchEnded = CC_CALLBACK_2(PlayerUIV88::onTouchEnded, this);
    //listener->setSwallowTouches(true);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	m_HeadImage = UserHead::create(HEAD_FRAME_IN_GAME_V66);
	Size size = m_HeadImage->getContentSize()*0.85;

    if (_seatNo >= 0 && _seatNo < PLAY_COUNT) {
        if (_seatNo == 0) {
            _bg = HNSprite::create(BG_BOTTOM_SRC);
        } else if (_seatNo == 3) {
            _bg = HNSprite::create(BG_TOP_SRC);
        } else {
            _bg = HNSprite::create(BG_EDGE_SRC);
        }
    } else {
        _bg = HNSprite::create(BG_EDGE_SRC);
    }

    this->addChild(_bg,1);
    this->setAnchorPoint(Vec2(0.5, 0.5));

	_bg->setScale(0.85);
	_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bg->setPosition(Vec2(size.width / 2, size.height / 2));
	_bg->setVisible(false);

	m_HeadImage->setClipUserHeadOffset(Vec2(0, 15));
	m_HeadImage->setAnchorPoint(_bg->getAnchorPoint());
	m_HeadImage->setPosition(_bg->getPosition());
	m_HeadImage->setScale(_bg->getScale());
	m_HeadImage->setLocalZOrder(_bg->getLocalZOrder());
	addChild(m_HeadImage);

	this->setContentSize(size);
	this->setIgnoreAnchorPointForPosition(false);

    auto timerSp = Sprite::create(PROGRESS_SRC);
    _progressTimer = ProgressTimer::create(timerSp);
    this->addChild(_progressTimer, 3);
	_progressTimer->setType(cocos2d::ProgressTimer::Type::RADIAL);
   // _progressTimer->setScaleX(size.width / timerSp->getContentSize().width);
    //_progressTimer->setScaleY(size.height / timerSp->getContentSize().height);
	_progressTimer->setScale(0.8);
    _progressTimer->setPosition(size.width / 2, size.height / 2+15);
    _progressTimer->setPercentage(0);
    _progressTimer->setVisible(false);
    _progressTimer->setReverseProgress(true);
	//_progressTimer->setMidpoint(Vec2(0, 0));
	//_progressTimer->setBarChangeRate(Vec2(1, 0));



	_nameText = Text::create("", FONT_TTF_MSYH, _seatNo == 0 ? 18 : _seatNo == 3 ? 18 : 18);
    _nameText->setAnchorPoint(Vec2(0.5f, 0.5f));
	_nameText->setPosition(Vec2(size.width / 2, size.height*0.32));
    /*_nameText->setPosition(_seatNo == 0 ? Vec2(size.width*0.65, size.height*0.8) :
                           _seatNo == 3 ? Vec2(size.width*0.68, size.height*0.7) :
                           Vec2(size.width*0.5, size.height*0.88));*/
	_nameText->setTextColor(Color4B(255, 255, 255, 255));
    this->addChild(_nameText, 3);


    _chipBackground = HNSprite::create(CHIP_BACKGROUND);
    _chipBackground->setAnchorPoint(Vec2(0.5f, 0.5f));
	_chipBackground->setPosition(_seatNo == 0 ? Vec2(_bg->getContentSize().width + 50, _bg->getContentSize().height*0.85) : Vec2(size.width/2, -10));
    _chipBackground->setScale(0.65f);
    this->addChild(_chipBackground, 4);

    auto chipBgSize = _chipBackground->getContentSize();
    _statusText = Text::create("", FONT_RES, 24);
    _statusText->setAnchorPoint(Vec2(0.5f, 0.5f));
	_statusText->setPosition(Vec2(_chipBackground->getContentSize().width / 2, (_chipBackground->getContentSize().height / 2)));
    _statusText->setTextColor(Color4B(255, 255, 255,255));
    _chipBackground->addChild(_statusText);

    _chipText = Text::create("", FONT_RES, 18);
    _chipText->setAnchorPoint(Vec2(0.5f, 0.5f));
	_chipText->setPosition(Vec2(size.width / 2, size.height*0.21));
	/* _chipText->setPosition(_seatNo == 0 ? Vec2(size.width*0.24, size.height*0.23) :
							_seatNo == 3 ? Vec2(size.width*0.34, size.height*0.18) :
							Vec2(size.width*0.25, size.height*0.17));*/
	_chipText->setTextColor(Color4B(255, 245, 135, 255));
    this->addChild(_chipText, 4);

    for(int i = 0; i < HAND_CARD_COUNT; i++) {
        _handCard[i] = PokerCardV88::create(0x00);
        _handCard[i]->setCardValue(0);
        _handCard[i]->setAnchorPoint(Vec2(0.5, 0.5));
        this->addChild(_handCard[i], i);
    }

    setHandCardPosition();

    _loseCardLabel = Sprite::create(CARD_LOSE_LABEL_RES);
    if (_loseCardLabel) {
        _loseCardLabel->setPosition(_handCard[1]->getPosition());
        _loseCardLabel->setVisible(false);
        addChild(_loseCardLabel, 20);
    }

    hideHandCard(false);

    return true;
}

void PlayerUIV88::setHandCardPosition() {
    Size size = _bg->getContentSize();
    size.width = 78;

    Vec2 pos_card = Vec2::ZERO;
    switch (_seatNo) {
    case 0:
        pos_card =  Vec2(WIN_SIZE.width / 2 - size.width * 0.34f, WIN_SIZE.height * 0.25f - 10);
        break;
    case 1:
        pos_card =  Vec2(-250+124-10, 76);
        break;
    case 2:
        pos_card =  Vec2(-250+124-10, 76);
        break;
    case 3:
        pos_card = Vec2(101-size.width*0.34f, -WIN_SIZE.height * 0.3f + 122 + 10);
        break;
    case 4:
        pos_card =  Vec2(250-10-124+100, 76);
        break;
    case 5:
        pos_card =  Vec2(250-10-124+100, 76);
        break;
    default:
        break;
    }

    //调整牌的位置及大小
    for(int i = 0; i < HAND_CARD_COUNT; i++) {
        Vec2 pos_temp =  pos_card + Vec2(size.width*i*0.34, 0);
        _handCard[i]->setPosition(pos_temp);
    }
}

void PlayerUIV88::setTableUI(GameTableUIV88* tableUI) {
    _TableUI = tableUI;
}

void PlayerUIV88::setName(const char* name) {
    _name = name;
    _nameText->setString(name);
    _nameText->setTextColor(Color4B(240,240,240,255));
}

void PlayerUIV88::setChip(LLONG chip) {
    _chip = chip;
    char tmp[50] = {0};
    sprintf(tmp, "%s", Tools::addComma(_chip).c_str());
    _chipText->setString(tmp);
}

long long PlayerUIV88::getChip() {
    return _chip;
}

void PlayerUIV88::setStatus(const char* status,int value) {
    _statusText->setString(status);
    _statusText->setTextColor(Color4B(201,207,80,255));
}

void PlayerUIV88::resetName(Node* pNode) {
    Text* text = (Text*)pNode;
    text->setString(_name);
    text->setTextColor(Color4B(240, 240, 240,255));
}

//传入 false 隐藏裂纹
void PlayerUIV88::controlHideLoseLabel(bool isHide) {
    if (_loseCardLabel) {
        _loseCardLabel->setVisible(isHide);
    }
}

//	std::string PlayerUIV88::getName() const
//	{
//		return _name;
//	}

std::string PlayerUIV88::getHeadImage() const {
    return _headImage;
}

void PlayerUIV88::setSexAndIcon(UserInfoStruct* user) {
	_userInfo = user;
	m_HeadImage->setHeadWithInfo(user);
}

UserInfoStruct* PlayerUIV88::getUserInfo() {
	return _userInfo;
}

PokerCardV88* PlayerUIV88::getHandCard(int index) {
    return _handCard[index];
}

void PlayerUIV88::note(LLONG money) {
    char tmp[100] = {0};
    sprintf(tmp, "%lld", money);
    //_nameText->setString(tmp);
    //_nameText->setColor(Color3B(201,207,80));
    _statusText->setString(tmp);
    _statusText->setTextColor(Color4B(201,207,80,255));
}

void PlayerUIV88::profit(LLONG money) {
//    char tmp[100] = {0};
//    if(money > 0) {
//        sprintf(tmp, "+%lld", money);
//    } else {
//        sprintf(tmp, "%lld", money);
//    }
//    _statusText->setString(tmp);
//    _statusText->setTextColor(Color4B(201,207,80,255));
    //
    auto coin = FloatCoin::create();
    if (coin && m_HeadImage) {
        coin->openParent(this, m_HeadImage->getPosition(), m_HeadImage->getLocalZOrder());
        coin->updateCoin(money);
    }
}

void PlayerUIV88::playAction(Action action) {
    //auto userid = _tableLogic->getUserId(seatNo);
    auto userinfo = UserInfoModule()->findUser(_userId);
    // true-man, false-lady
    auto sex = userinfo->bBoy;

    std::string audio("");
    std::string filename("");

    switch (action) {
    case PlayerUIV88::eLookCard:
        audio = sex ? GOLD_LOOK_MAN : GOLD_LOOK_LADY;
        filename.append("Games/12101106/goldenflower/table/look_action.png");
        break;
    case PlayerUIV88::eGiverUp:
        audio = sex ? GOLD_GIVEUP_MAN : GOLD_GIVEUP_LADY;
        filename.append("Games/12101106/goldenflower/table/giveup_action.png");
        break;
    case PlayerUIV88::eNote:
        audio = sex ? GOLD_ADD_MAN : GOLD_ADD_LADY;
        filename.append("Games/12101106/goldenflower/table/note_action.png");
        break;
    case PlayerUIV88::eFollow:
        audio = sex ? GOLD_FOLLOW_MAN : GOLD_FOLLOW_LADY;
        filename.append("Games/12101106/goldenflower/table/follow_action.png");
        break;
    default:
        break;
    }

    if (!audio.empty()) {
        HNAudioEngine::getInstance()->playEffect(audio.c_str());
    }

    if (!filename.empty()) {
        auto sp = Sprite::create(filename);
		Vec2 pos = _seatNo == 0 ? Vec2(m_HeadImage->getContentSize().width * 0.35f, m_HeadImage->getContentSize().height * 0.15f) :
			_seatNo == 3 ? Vec2(m_HeadImage->getContentSize().width * 0.35f, m_HeadImage->getContentSize().height * 0.1f) :
			Vec2(m_HeadImage->getContentSize().width * 0.36f, m_HeadImage->getContentSize().height * 0.1f);

		Vec2 offPos = _seatNo == 0 ? Vec2(0, m_HeadImage->getContentSize().height * 0.8f) :
			_seatNo == 3 ? Vec2(0, m_HeadImage->getContentSize().height * 0.53f) :
			Vec2(0, m_HeadImage->getContentSize().height * 0.71f);

        sp->setPosition(pos);
        this->addChild(sp, 100);
        //sp->setGlobalZOrder(3);
        sp->runAction(Sequence::create(MoveBy::create(0.3f, offPos), DelayTime::create(1.5f), RemoveSelf::create(true), nullptr));
    }

}

void PlayerUIV88::setHandCard(int index, BYTE value) {
    if(index == 0 || index == 1 || index == 2) {
        controlHideLoseLabel(false);
        _handCard[index]->setCardValue(value);
        _handCard[index]->setVisible(true);
        _handCard[index]->setRotation(0);

        if(value != 0x00) {
//				int width = _handCard[index]->getContentSize().width;
            this->reorderChild(_handCard[index], 4);
        } else {
//				int width = _handCard[index]->getContentSize().width;
            this->reorderChild(_handCard[index], 0);
        }
    }
}

void PlayerUIV88::changeHandCardValue(Node* pNode,int index, BYTE value) {
    if (index == 0 || index == 1 || index == 2) {
        controlHideLoseLabel(false);
        _handCard[index]->setCardValue(value);
        _handCard[index]->setVisible(true);
        _handCard[index]->setRotation(0);

        if (value != 0x00) {
//				int width = _handCard[index]->getContentSize().width;
            this->reorderChild(_handCard[index], 4);
        } else {
//				int width = _handCard[index]->getContentSize().width;
            this->reorderChild(_handCard[index], 0);
        }
    }
}

// show the handcards for other players
void PlayerUIV88::lookHandCard() {
    controlHideLoseLabel(false);
    _handCard[0]->runAction(RotateBy::create(0.2f, 0));
    _handCard[1]->runAction(RotateTo::create(0.2f, 15));
    _handCard[2]->runAction(RotateTo::create(0.2f, 30));
}

// lose the combat
void PlayerUIV88::lose() {
    _isQuit = true;
    for (int i = 0; i < HAND_CARD_COUNT; i++) {
        _handCard[i]->setCardValue(0x00);
        _handCard[i]->setGray(true);
    }
}

// set hand card broken when lose in card comparison
void PlayerUIV88::setHandCardBroken() {
//		for (int i = 0; i < HAND_CARD_COUNT; i++)
//		{
//			_handCard[i]->setCardValue(0x3E);
//		}
    controlHideLoseLabel(true);
}

void PlayerUIV88::hideHandCard(bool setVisible) {
    controlHideLoseLabel(false);
    for (int i = 0; i < 3; i++) {
        if (_handCard[i]) {
            _handCard[i]->setVisible(setVisible);
        }
    }
}

void PlayerUIV88::startWait(int time, int total) {
    if(time > 1 && total > 1) {
        _leftTime = time;
        _totalTime = total;
        schedule(schedule_selector(PlayerUIV88::onWaitTimer),0.1f);
    }
}

void PlayerUIV88::stopWait() {
    _progressTimer->setVisible(false);
    unschedule(schedule_selector(PlayerUIV88::onWaitTimer));
}

void PlayerUIV88::playWin(int type) {
    char tmp[50] = {0};
    sprintf(tmp,"cardtype-%d.png", type);
    auto sp = Sprite::createWithSpriteFrameName(tmp);
    if(sp == nullptr) {
        return;
    }
    sp->setPosition(_bg->getContentSize().width / 2, _bg->getContentSize().height / 2);
    this->addChild(sp,6,"winSprite");
    sp->runAction(Sequence::create(
                      EaseIn::create(MoveBy::create(0.5f, Vec2(0, 50)), 2.5f),
                      DelayTime::create(8.0f),
                      RemoveSelf::create(true),
                      nullptr));
}

void PlayerUIV88::stopPlayWin() {
    auto sp  = this->getChildByName("winSprite");
    if (nullptr == sp) {
        return ;
    }
    sp->stopAllActions();
    sp->removeFromParent();
}

void PlayerUIV88::onWaitTimer(float delta) {
	_leftTime -= 0.1f;
	if (_leftTime < 0) {
		stopWait();

		_TableUI->getReady();

		_progressTimer->setVisible(false);
		return;
	}

	Color3B color(248, 220, 83);//GREEN
	_progressTimer->setVisible(true);
	if (0 == _seatNo) {
		_progressTimer->setVisible(false);
	}

	float ratio = _leftTime / _totalTime;
	//int value = ratio * 510;
	//   if(value > 255) {
	//       color.r  = 475 - value;
	//   } else {
	//       color.r = 220;
	//       //颜色变淡效果
	//       //color.g = value;
	//   }

	_progressTimer->setColor(color);
	_progressTimer->setPercentage(ratio * 100);
}

//bool PlayerUIV88::onTouchBegan(Touch *touch, Event *unused_event)
//{
//	if(_bg == nullptr)
//	{
//		return false;
//	}
//	//Rect rect = _bg->getBoundingBox();
//	//Vec2 pos = this->convertToNodeSpace(touch->getLocation());
//	//if(rect.containsPoint(pos))
//	//{
//	//	return true;
//	//}
//	//else
//	//{
//		return false;
//	//}
//}

//void PlayerUIV88::onTouchEnded(Touch *touch, Event *unused_event)
//{
//	//if (_bg != nullptr)
//	//{
//	//	Rect rect = _bg->getBoundingBox();
//	//	Vec2 pos = this->convertToNodeSpace(touch->getLocation());
//	//	if (rect.containsPoint(pos))
//	//	{
//	//		auto playerInfo = GameUserMessageBox::createMessageBox(_TableUI, _userId, _headImage);
//	//		playerInfo->setHidePoint(this->getPosition());
//	//		_TableUI->reorderChild(playerInfo, this->getLocalZOrder() + 100);
//	//		if (playerInfo != nullptr)
//	//		{
//	//			playerInfo->show();
//	//		}
//	//	}
//	//}
//}

void PlayerUIV88::setViewSeatNo(BYTE seatNo) {
    _seatNo = seatNo;
}

bool PlayerUIV88::isPlaying() {
    return _isPlaying;
}

//    void PlayerUIV88::resetIsQuit()
//    {
//        _isQuit = false;
//    }

void PlayerUIV88::enterGame() {
    _isPlaying = true;
    _isQuit = false;
}

//未离开的玩家准备开始新游戏
void PlayerUIV88::resetForNewGame() {
    stopWait();
    hideHandCard(false);
    note(0);
    controlHideLoseLabel(false);
    _isQuit = false;
}


//比牌光效
void PlayerUIV88::showBipaiPlayerEffect() {
	auto _hitNode = CSLoader::createNode(ANIMATION_VS_EFFECT_CSB);
	_hitNode->setAnchorPoint(Vec2(0.5, 0.5));
	Size size = m_HeadImage->getContentSize()*0.85;
	_hitNode->setPosition(Vec2(m_HeadImage->getPosition().x, m_HeadImage->getPosition().y - 0.24*size.height));
	_hitNode->setScale(0.85);
	this->addChild(_hitNode, 20);
	if (_hitNode)
	{
		auto action = CSLoader::createTimeline(ANIMATION_VS_EFFECT_CSB);
		_hitNode->runAction(action);
		action->play("bipai_2ren_portraits", false);
	}

}




bool PlayerUIV88::isQuit() {
    return _isQuit;
}

LLONG PlayerUIV88::getUserID() {
    return _userId;
}

HNSprite* PlayerUIV88::getHeadSp() {
	return _headSp;
}

}



