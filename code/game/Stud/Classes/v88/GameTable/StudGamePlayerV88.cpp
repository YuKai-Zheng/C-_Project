#include "StudGamePlayerV88.h"


#include "StudPokerCardV88.h"
#include "StudGameTableUIV88.h"
#include "StudGameUserMessageBoxV88.h"

#include "Tool/Tools.h"
#include "HNCommon/HNConverCode.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "Sound/HNAudioEngine.h"
#include "UI/FloatCoin.h"

#define WINSIZE Director::getInstance()->getWinSize()

namespace STUD {
PlayerUIV88::PlayerUIV88():
    _bankroll(0),
    _pot(0),
    _userId(0),
    _gender(GENDER::FEMALE),
    _isme(false) {
    memset(&_canvas, 0, sizeof(_canvas));
    memset(_handCard, 0, sizeof(_handCard));
}


PlayerUIV88* PlayerUIV88::create(LLONG userId,INT direction) {
    PlayerUIV88* player = new PlayerUIV88();
    if(player->init(userId,direction)) {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return nullptr;
}

PlayerUIV88::~PlayerUIV88() {
//    unschedule(schedule_selector(PlayerUIV88::refreshTimer));
}

bool PlayerUIV88::init(LLONG userId, INT direction) {
    if (!HNNode::init()) {
        return false;
    }
    _userId = userId;
    _direction = direction;
    auto userinfo	= UserInfoModule()->findUser(userId);
    switch (direction) {
    case BOTTOM: { //自己
        _playerNode = CSLoader::createNode("csb/v66/stud/player/player.csb");
        break;
    }
    case TOP: { //上方
        _playerNode = CSLoader::createNode("csb/v66/stud/player/topPlayer.csb");
        break;
    }
    default:
        break;
    }

    _canvas.outline = (Layout*)_playerNode->getChildByName("player_panel");
    _canvas.outline	-> setAnchorPoint(Vec2(0.5, 0.5));
    _playerNode->setPosition(Vec2(_canvas.outline->getContentSize().width*0.5f, _canvas.outline->getContentSize().height*0.5f));
    _layoutSize = _canvas.outline->getContentSize();
    //_canvas.outline	= cocostudio::GUIReader::getInstance()->widgetFromJsonFile("stud/player/player.json");
    this->addChild(_playerNode);

    // is me or not
    _isme = (PlatformLogic()->loginResult.dwUserID == userinfo->dwUserID);
    //auto ismeImg = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_canvas.outline, "isme_img"));
    //ismeImg->setVisible(_isme);

    // background
    _canvas.background = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_canvas.outline, "background_btn"));
    _canvas.background	-> setEnabled(false);
    //_canvas.background->setBright(!_isme);

    // init hand cards to poker 0x00, the poker back-image
    _canvas.handcardAnchor = (ImageView*)_playerNode->getChildByName("handcard_img");
    if (direction == BOTTOM) {
        _canvas.handcardAnchor->setPositionX(WINSIZE.width*0.5f - _layoutSize.width*0.65f - 20.0 / 1280.0 * WINSIZE.width );
    } else {

    }
    _canvas.handcardAnchor->setVisible(false);

    // timer
    //_canvas.timerBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_canvas.outline, "timer_btn"));
    //_canvas.timerBtn -> setEnabled(false);

    //  ial button to invisiable
    _canvas.button = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_canvas.background, "button_img"));
    _canvas.button -> setVisible(false);

    // set player gender
    _gender = userinfo->bBoy?MALE:FEMALE;

    // set player avata
    auto avatafile = _gender==GENDER::MALE?MEN_AVATA_V88:LADY_AVATA_V88;
	_canvas.avata = dynamic_cast<Sprite*>(_canvas.background->getChildByName("avata_img"));
	_canvas.avata->setVisible(false);
	_canvas.headbg = static_cast<Sprite*>(_canvas.background->getChildByName("Sprite_1"));
	_canvas.headbg->setVisible(false);
    
	m_HeadImage = UserHead::create(HEAD_FRAME_IN_GAME_V66);
	m_HeadImage->setClipUserHeadOffset(Vec2(0, 15));
	m_HeadImage->setPosition(_canvas.headbg->getPosition());
	m_HeadImage->setLocalZOrder(_canvas.headbg->getLocalZOrder());
	m_HeadImage->setScale(_canvas.headbg->getScale());
    _canvas.background->addChild(m_HeadImage);
    

    // set player name
    auto namestr = userinfo->nickName;
    _canvas.name = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_canvas.outline, "name_label"));
    _canvas.name->setString(ForceGBKToUtf8(namestr));

    // set total money the player owned
    _bankroll = userinfo->i64Money;
    _canvas.bank = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_canvas.outline, "bank_label"));
    _canvas.bank->setString(ForceGBKToUtf8(Tools::fourSeparateMoney(userinfo->i64Money).c_str()));

    // init the pot to 0, the money i betted
    _pot = 0;
    _canvas.pot = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_canvas.outline, "pot_label"));
    _canvas.pot->setString("");



    auto winSize = Director::getInstance()->getWinSize();
    INT OFFSET = HANDCARD_OFFSET_V88;
    for(int i=0; i<HANDCARD_COUNT; i++) {
        _handCard[i] = PokerCardV88::create(0x00);
        if (direction == TOP) {
            OFFSET = 25;
            _handCard[i]->setScale(_canvas.handcardAnchor->getScale());
        } else if (direction == BOTTOM) {
            _handCard[i]->setScale(_canvas.handcardAnchor->getScale() * 1.2);
            OFFSET = 26;
        }
        auto offset = Vec2(OFFSET*i*1280 / 900, 0);
        _handCard[i]->setPosition(_canvas.handcardAnchor->getPosition()+offset);
        _canvas.handcardAnchor->getParent()->addChild(_handCard[i], i);
    }

    setHandCardVisible(false);

    // touch event for user info
    //auto listener = EventListenerTouchOneByOne::create();
    //listener->onTouchBegan = CC_CALLBACK_2(PlayerUIV88::onTouchBegan, this);
    //listener->onTouchEnded = CC_CALLBACK_2(PlayerUIV88::onTouchEnded, this);
    //	listener->setSwallowTouches(true);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
//
    return true;
}

void PlayerUIV88::setHead(UserInfoStruct *user) {
    if (!user) {
        m_HeadImage->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V66);
        return;
    }
    m_HeadImage->setHeadWithInfo(user);
}

void PlayerUIV88::setTableUI(GameTableUIV88* tableUI) {
    _TableUI = tableUI;
}

    void PlayerUIV88::setTotalBetMoney(LLONG pot) {
        _pot = pot;
    }
    
void PlayerUIV88::reloadMoney() {
    auto userinfo=UserInfoModule()->findUser(_userId);

    char buff[30];
    auto moneyChange = userinfo->i64Money - _bankroll;
    string moneyChangeStr = moneyChange > 0 ? ";" : ":";
    string musicWinAndLosePath = moneyChange > 0 ? GAME_WIN_AUDIO_EFFECT_V66 : GAME_LOSE_AUDIO_EFFECT_V66;
    string fontPath = moneyChangeStr != ";" ? "v66/common/fnt_number/blue1.png" : "v66/common/fnt_number/yellow3.png";

    sprintf(buff, "%lld", moneyChange < 0 ? -moneyChange : moneyChange);
    moneyChangeStr.append(buff);

    _bankroll = userinfo->i64Money;
    char money[30];
    sprintf(money, "%lld", _bankroll);
    string moneystr = ForceGBKToUtf8("");
    moneystr.append(money);

    if(moneyChange!=0) {
        // animate
        auto font	= _canvas.bank->getFontName();
        auto fontWeight	= _canvas.bank->getFontSize() + 10;
        auto fontcolor	= moneyChange < 0 ? Color3B::RED: Color3B::GREEN;
        //auto potAnimate		= Text::create(moneyChangeStr, font, fontWeight);
        LabelAtlas* pLabel5 = LabelAtlas::create(moneyChangeStr, fontPath, 22, 28, '0');



        //auto offset = Vec2(_canvas.bank->getContentSize().width, 0);
        auto offset = Vec2(0, 0);

        //potAnimate	-> setColor(fontcolor);
        //potAnimate	-> setPosition(_canvas.bank->getPosition() + offset);
        //potAnimate	-> setAnchorPoint(_canvas.bank->getAnchorPoint());
        //_canvas.bank-> getParent()->addChild(potAnimate, _canvas.bank->getLocalZOrder()+10);

        //potAnimate->runAction(Sequence::create(
        //	MoveBy::create(1.8f, Vec2(0, 20)),
        //	RemoveSelf::create(true),
        //	CallFunc::create([=](){ _canvas.bank->setString(GBKToUtf8(Tools::convertNumberOnlySignificantDigits(_bankroll).c_str())); }),
        //	nullptr));

        pLabel5->setPosition(_canvas.bank->getPosition() + offset);
        pLabel5->setAnchorPoint(Vec2(0.5, 0.5));
        _canvas.bank->getParent()->addChild(pLabel5, _canvas.bank->getLocalZOrder() + 10);

        if (isMe()) {
            HNAudioEngine::getInstance()->playEffect(musicWinAndLosePath.c_str());
        }
        pLabel5->setVisible(false);
        pLabel5->runAction(Sequence::create(
//                               MoveBy::create(1.8f, Vec2(0, 20)),
//                               RemoveSelf::create(true),
                                            DelayTime::create(1.8f),
        CallFunc::create([=]() {
            _canvas.bank->setString(ForceGBKToUtf8(Tools::fourSeparateMoney(_bankroll).c_str()));
        }),
        nullptr));
        //
        auto coin = FloatCoin::create();
        if (coin && pLabel5) {
            coin->openParent(_canvas.bank->getParent(), pLabel5->getPosition() + Vec2(0, 80), pLabel5->getLocalZOrder());
            coin->updateCoin(moneyChange);
        }
    }
}

void PlayerUIV88::recoveryData(bool isVisible) {
    _canvas.button->setVisible(isVisible);
}


void PlayerUIV88::removeThis() {
    scheduleOnce(schedule_selector(PlayerUIV88::removeThis), 1.0f);
}

void PlayerUIV88::removeThis(float dealt) {
    this->removeFromParentAndCleanup(true);
}

void PlayerUIV88::resetPot() {
    _pot=0;
    _canvas.pot->setString("");
}

void PlayerUIV88::changeOtherBackGround(int key) {
    if (key == 1) {
        _canvas.headbg->setTexture("hallimg/niuniu/5.png");
    } else if (key == 2) {
        _canvas.headbg->setTexture("hallimg/niuniu/5.png");
    }
}

//筹码币
void PlayerUIV88::bet(LLONG money) {
    _pot+=money;

    char moneyc[30] = {0};
    sprintf(moneyc, "%lld", _pot);
    string moneystr="";
    moneystr.append(moneyc);

    if(money>0) {
        HNAudioEngine::getInstance()->playEffect(STUDAUDIO_CHIP);

        //auto font			= _canvas.pot->getFontName();
        //auto fontWeight		= _canvas.pot->getFontSize();
        //auto fontcolor		= _canvas.pot->getColor();
        //auto potAnimate		= Text::create(moneystr, font, fontWeight);

        //potAnimate->setColor(fontcolor);
        //potAnimate	-> setPosition(_canvas.pot->getPosition());
        //potAnimate	-> setAnchorPoint(_canvas.pot->getAnchorPoint());
        //_canvas.pot	-> getParent()->addChild(potAnimate, _canvas.pot->getLocalZOrder()+10);
        //_canvas.pot	-> setString("");

        //_canvas.pot->setString(moneystr);
        //potAnimate->setScale(1.1f);
        //potAnimate->runAction(Sequence::create(
        //		FadeOut::create(0.2f),
        //		RemoveSelf::create(true),
        //		CallFunc::create([=](){ _canvas.pot->setText(moneystr);}),
        //	nullptr));
    }
}

bool PlayerUIV88::isMe() {
    return _isme;
}

bool PlayerUIV88::isMan() {
    return _gender==GENDER::MALE? true: false;
}

// hide all hand cards(true)
void PlayerUIV88::setHandCardVisible(bool isVisible) {
    for(auto card: _handCard) {
        card->setVisible(isVisible);
    }
}

void PlayerUIV88::setHandCardVisible(int index, bool isVisible) {
    if(index < HANDCARD_COUNT && index >= 0)
        _handCard[index]->setVisible(isVisible);
}


void PlayerUIV88::setButton(bool isButton) {
    _canvas.button->setVisible(isButton);
}


void PlayerUIV88::setHandCard(int index, BYTE value, bool visible) {
    _handCard[index]->setCardValue(value);
    _handCard[index]->setVisible(visible);
    _handCard[index]->setRotation(0);
}


void PlayerUIV88::playAction(PLAYER_ACTION action) {
    std::string filename;

    filename.append("stud/table/note_action.png");

    if(!filename.empty()) {
        auto sp = Sprite::create(filename);
        sp->setPosition(_canvas.avata->getContentSize().width / 2, _canvas.avata->getContentSize().height / 2);
        addChild(sp, 10);

        //HNAudioEngine::getInstance()->playEffect(audio.c_str());
        sp->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, 50)), DelayTime::create(1.5f), RemoveSelf::create(true), nullptr));
    }
}


//void PlayerUIV88::startTimer(int leftTime, int totalTime) {
//    if(leftTime > 1) {
//        _leftTime = leftTime;
////        _totalTime = totalTime;
//        _TableUI->startClock(_leftTime);
////        schedule(schedule_selector(PlayerUIV88::refreshTimer),1.0f);
//    }
//}
//
//void PlayerUIV88::stopTimer() {
//    _leftTime=0;
//    _TableUI->stopClock();
////    _TableUI->clockHide(bool visibility)
////    unschedule(schedule_selector(PlayerUIV88::refreshTimer));
//}
//
//float PlayerUIV88::readTimer() {
////    return _leftTime;
//    return _TableUI->readTimer();
//}

//void PlayerUIV88::refreshTimer(float delta) {
//    _leftTime -= 1;
//
////    CCLOG("refreshTimer=_leftTime = %f ", _leftTime);
//    if (_leftTime <= 0) {
//        stopTimer();
//        return;
//    }
//    _TableUI->upDateClockTimer(_leftTime);
//}

bool PlayerUIV88::onTouchBegan(Touch *touch, Event *events) {
    if(_canvas.avata != nullptr) {

        Rect rect(0,0, _canvas.avata->getContentSize().width, _canvas.avata->getContentSize().height);
        //= _canvas.avata->getBoundingBox();
        if(rect.containsPoint(_canvas.avata->convertToNodeSpace(touch->getLocation()))) {
            auto playerInfo		= GameUserMessageBoxV88::createMessageBox(this, _userId);
            auto visibleSize	= Director::getInstance()->getVisibleSize();
            auto showpoint		= Vec2(visibleSize.width/2, visibleSize.height/2);

            playerInfo->setShowPoint(this->convertToNodeSpace(showpoint));
            playerInfo->setHidePoint(Vec2(0, 0));

            if(playerInfo != nullptr) {
                playerInfo->show();
            }
            return true;
        }
    }
    return false;
}

void PlayerUIV88::onTouchEnded(Touch *touch, Event *events) {
}


LLONG PlayerUIV88::getUserId() {
    return _userId;
}

void PlayerUIV88::showHandcardLeft(BYTE count) {

    _canvas.handcardAnchor->setVisible(false);
    //if(count<1)
    //{
    //	return;
    //}

    //auto intervalAnchor = _canvas.handcardAnchor->getPosition().x;
    //auto offset = HANDCARD_OFFSET * (count-1);
    //offset += intervalAnchor;

    ////auto intervalHandcard = _handCard[0]->getPosition().x - _canvas.background->getPosition().x;

    //auto offsetX = _canvas.background->getPosition().x - offset;

    //auto offVec=Vec2(offsetX - _handCard[0]->getPosition().x, 0);
    //for(int i=0; i<HANDCARD_COUNT; i++)
    //{
    //	_handCard[i]->setPosition(_handCard[i]->getPosition() + offVec);
    //}
}

//#define TYPE_GIVE_UP					0x00			//放弃
//#define TYPE_NOTE						0x01			//下注
//#define TYPE_ADD						0x02			//加注
//#define TYPE_FOLLOW					0x03			//跟注
//#define TYPE_SHUTTLE					0x04			//梭哈
void PlayerUIV88::playNoteAudio(BYTE audioType) {
    string audioString = "music/Stud/";
    //audioString.append(_gender == GENDER::MALE ? "male/": "female/");

    switch (audioType) {
    case 0x00:
        // give up	- TYPE_GIVE_UP  //放弃
        audioString += STUDAUDIO_FOLD;
        break;
    case 0x01:
        // note		- TYPE_NOTE
        audioString += STUDAUDIO_BET;
        break;
    case 0x02:
        // add		- TYPE_ADD //加注
        audioString += STUDAUDIO_RAISE;
        break;
    case 0x03:
        // follow	- TYPE_FOLLOW 跟注
        audioString += STUDAUDIO_CALL;
        break;
    case 0x04:
        // shuttle	- TYPE_SHUTTLE //梭哈
        audioString += STUDAUDIO_ALLIN;
        break;

    default:
        break;
    }

    HNAudioEngine::getInstance()->playEffect(audioString.c_str());
}
}
