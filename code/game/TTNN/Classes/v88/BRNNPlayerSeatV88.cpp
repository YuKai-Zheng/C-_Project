
#include "BRNNPlayerSeatV88.h"
#include "HNLobby/v66/PlatformResV66.h"
//#include <afx.h>
//#include <afx.h>

int framesV88[27] = { 2, 8, 9, 2, 2, 8, 6, 9, 3, 4, 12, 4, 4, 14, 5, 4, 3, 4, 2, 2, 2, 7, 5, 9, 4, 8, 3 };

namespace BRNN {
PlayerSeatV88::PlayerSeatV88()
    : _emotionSprite(nullptr)
    , _bSetGirl(false) {
    this->initData();
}

PlayerSeatV88::~PlayerSeatV88() {
}

PlayerSeatV88* PlayerSeatV88::create(INT seatIndex /* = 6*/, UserInfoStruct* user /* = nullptr*/) {
    PlayerSeatV88* ret = new PlayerSeatV88();
    if (ret && ret->init(seatIndex, user)) {
        ret->autorelease();
    } else {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void PlayerSeatV88::initData() {
    _girl = nullptr;
    _seatBg = nullptr;
    _nameLabel = nullptr;
    _moneyLabel = nullptr;
    _user = nullptr;
    //blackQueue = nullptr;
}

bool PlayerSeatV88::init(INT seatIndex, UserInfoStruct* user) {
    if (!HNGameUIBase::init()) {
        return false;
    }

	this->setIgnoreAnchorPointForPosition(false);

    isBanker = false;
    _seatIndex = seatIndex;
    _user = user;

	float fScale = 0.76f;

	//MAX_NON_BANKER_SEAT_NUM_V88 12人
	if (seatIndex > 12){
		isBanker = true;
		m_HeadImage = UserHead::create(BRNN_BANKER_BG_V88);
		m_HeadImage->setClipUserHeadOffset(Vec2(0, 6));

		fScale = 0.60;

		auto bankLogo = Sprite::create("hallimg/niuniu/zhuang_1.png");
		bankLogo->setPosition(Vec2(0, 0));
		addChild(bankLogo);
	}
	else{
		m_HeadImage = UserHead::create(HEAD_FRAME_IN_GAME_V66);
		m_HeadImage->setClipUserHeadOffset(Vec2(0, 15));
	}

	Size size = m_HeadImage->getContentSize()*fScale;
	setContentSize(size);

	m_HeadImage->setScale(fScale);
	m_HeadImage->setPosition(size/2);
    m_HeadImage->setHeadTouchEnabled(true);
	addChild(m_HeadImage);

	//空位置坐下
	m_HeadImage->onHeadClickEvent = [&](UserHead* sender, Vec2 touchVec){
		if (!getUser() && _seatIndex <= 12)
		{
			_GameTableLogic->sendChangeSeat(_seatIndex);
		}
	};

    setUser(_user);

    return true;
}

void PlayerSeatV88::setGirl() {
    if(_girl) {
        _girl->removeFromParent();
        _girl = nullptr;
    }
    //_girl = Sprite::create(BRNN_TABLERES_GIRL_V88);
    //_girl->setPosition(_size.width / 2, _size.height / 2);
    //this->addChild(_girl);
    _bSetGirl = true;
}

void PlayerSeatV88::setUser(UserInfoStruct* user) {
    _user = user;
	if (!_user) {
		m_HeadImage->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD);
        this->setVisible(false);
		return;
	}
    this->setVisible(true);
    _bSetGirl = false;
    
    m_HeadImage->setHeadWithInfo(user);
    
	//name
    if(!_nameLabel) {
		_nameLabel = Label::createWithTTF("", FONT_TTF_MSYH, FONT_SIZE_NUM_18);
        _nameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _nameLabel->setPosition(getContentSize().width / 2, 46);
		if (isBanker)
		{
			_nameLabel = Label::createWithTTF("", FONT_TTF_MSYH, 22);
		}
        this->addChild(_nameLabel,2);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	_nameLabel->setString(ForceGBKToUtf8(_user->nickName));
#else
	_nameLabel->setString(GBKToUtf8(_user->nickName));
#endif

	//money
    if(!_moneyLabel) {
		_moneyLabel = Label::createWithTTF("", FONT_TTF_FZCY, FONT_SIZE_NUM_18);
		_moneyLabel->setColor(FONT_MONEY_COLOR_V66);
		_moneyLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		if (isBanker)
		{
			_moneyLabel = Label::createWithTTF("", FONT_TTF_FZCY, 22);
			_moneyLabel->setColor(FONT_CLOLOR_YELLOW_V66);
			_moneyLabel->setPosition(getContentSize().width+10, 15);
		}
		else
		{
			_moneyLabel->setPosition(getContentSize().width / 2, 27);
		}

	
		this->addChild(_moneyLabel, 2);
    }
	_moneyLabel->setString(Tools::addComma(_user->i64Money));

	//
    if (isBanker) {
        if (_nameLabel) {
			_nameLabel->setPosition((getContentSize().width) / 2 + 115, getContentSize().height / 2 +5);
        }
        if (_moneyLabel) {
            _moneyLabel->setPosition(getContentSize().width / 2 + 115, getContentSize().height / 2 - 25);
        }
    }
}

void PlayerSeatV88::updateUser() {
    if(!_user) { //这个_user在下面还要用的
        if (!_bSetGirl) {
            userExit();
        }
        return;
    }
    _user = UserInfoModule()->findUser(_user->dwUserID);
    if (_moneyLabel)
        _moneyLabel->setString(Tools::addComma(_user->i64Money));
}

void PlayerSeatV88::setBankerBg(std::string  bgPath)
{

}

void PlayerSeatV88::userExit() {
    if (_seatBg) {
        _seatBg->removeFromParent();
    }
    m_HeadImage->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD);
    if (_nameLabel) {
        _nameLabel->removeFromParent();
    }
    if (_moneyLabel) {
        _moneyLabel->removeFromParent();
    }
    if (_girl) {
        _girl->removeFromParent();
    }
    this->setVisible(false);
    this->initData();
}

//////////////////////////////////////////////////////////////////////////
//void PlayerSeatV88::showEmotion(int emotionId) {
//    removeEmotionSprite();
//
//    _emotionSprite = Sprite::create(StringUtils::format(BRNN_EMOTION_SE, emotionId));
//    if(_emotionSprite) {
//        _emotionSprite->setPosition(this->getPosition());
//        this->getParent()->addChild(_emotionSprite, 1000);
//
//        std::string path = StringUtils::format(BRNN_EMOTION_DIR, emotionId);
//        std::string f = StringUtils::format("%d.png", emotionId);
//        ActionInterval* action = nullptr;
//        if(getEmotionFrame(emotionId) <= 5) {
//            action = Sequence::create(
//                         Repeat::create(ActionsV88::FrameAnimation((path+"%d"+f).c_str(), getEmotionFrame(emotionId), 0.3f, false), 3.0f),
//                         RemoveSelf::create(),
//            CallFunc::create([=]() {
//                _emotionSprite = nullptr;
//            }),
//            nullptr);
//        } else {
//            action = Sequence::create(
//                         ActionsV88::FrameAnimation((path+"%d"+f).c_str(), getEmotionFrame(emotionId), 0.3f, true),
//            CallFunc::create([=]() {
//                _emotionSprite = nullptr;
//            }),
//            nullptr);
//        }
//        _emotionSprite->runAction(action);
//    }
//}

//void PlayerSeatV88::showWords(std::string words) {
//    if(this->getParent()->getChildByTag(-101)) {
//        this->getParent()->getChildByTag(-101)->stopAllActions();
//        this->getParent()->removeChildByTag(-101);
//    }
//
//    ui::Scale9Sprite* chatBox = nullptr;
//    if(_seatIndex > 5) {
//        chatBox = ui::Scale9Sprite::create(BRNN_CHAT_BLACK_RIGHT, Rect(0, 0, 72, 43), Rect(26, 10, 20, 18));
//        chatBox->setAnchorPoint(Vec2(1, 0.5));
//    } else {
//        chatBox = ui::Scale9Sprite::create(BRNN_CHAT_BLACK_LEFT, Rect(0, 0, 72, 43), Rect(26, 10, 20, 18));
//        chatBox->setAnchorPoint(Vec2(0, 0.5));
//    }
//    chatBox->setTag(-101);
//    chatBox->setPosition(this->getPosition());
//    this->getParent()->addChild(chatBox, 1000);
//
//    auto wLabel = Label::createWithTTF(words, DEFAULT_FONT, 23);
//    chatBox->setContentSize(Size(wLabel->getContentSize().width+60, wLabel->getContentSize().height+30));
//    wLabel->setAnchorPoint(Vec2(0, 0.5));
//    wLabel->setPosition(30, chatBox->getContentSize().height*2/3);
//    wLabel->setColor(Color3B(0,0,0));
//    chatBox->addChild(wLabel);
//
//    chatBox->runAction(Sequence::create(DelayTime::create(3), RemoveSelf::create(), nullptr));
//}

void PlayerSeatV88::removeEmotionSprite() {
    if(nullptr != _emotionSprite) {
        _emotionSprite->stopAllActions();
        _emotionSprite->removeFromParent();
        _emotionSprite = nullptr;
    }
}

int PlayerSeatV88::getEmotionFrame(int idx) {
    int len = (sizeof(framesV88) / sizeof(framesV88[0]));
    if (len>idx+1) {
        return 1;
    } else {
        return framesV88[idx];
    }
}
}
