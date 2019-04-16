#include "BRNNBetObjV88.h"
#include "BRNNResV88.h"
#include "HNCommon/HNConverCode.h"
#include "HNLobby/v66/PlatformResV66.h"

namespace BRNN {

BetObjV88::BetObjV88(INT betType, bool bClick, bool bSelect)
    : _betType(betType), _bClick(bClick), _bSelect(bSelect),
      _effectPic(nullptr), _betPic(nullptr) {}

BetObjV88::~BetObjV88() {}

BetObjV88 *BetObjV88::create(INT betType, bool bClick /* = false*/,
                       bool bSelect /* = false*/) {
    BetObjV88 *obj = new BetObjV88(betType, bClick, bSelect);
    if (obj && obj->init() && 0 <= betType && betType <= 4)
        obj->autorelease();
    else
        CC_SAFE_DELETE(obj);
    return obj;
}

bool BetObjV88::init() {
    if (!HNLayer::init()) {
        return false;
    }

    this->setIgnoreAnchorPointForPosition(false);
    auto tmp =
		Sprite::create(StringUtils::format(BRNN_GAME_TABLERES_BET_NORMAL_V88, 7));
    this->setContentSize(tmp->getContentSize());
    tmp = nullptr;

	_effectPic = Sprite::create(BRNN_GAME_TABLERES_BET_SELECT_V88);
    //_effectPic->setScale(0.8);
    _effectPic->setPosition(this->getContentSize().width / 2,
                            0);

    _effectPic->setVisible(false);
    this->addChild(_effectPic);

    if (_bClick) {
        _betPic = Sprite::create(
			StringUtils::format(BRNN_GAME_TABLERES_BET_NORMAL_V88, _betType+7));
        if (_bSelect)
            _effectPic->setVisible(true);
		_betPic->setColor(Color3B(255, 255, 255));
	
    } else {
        /*_betPic = Sprite::create(
                      StringUtils::format(BRNN_GAME_TABLERES_BET_DISABLE_V88, _betType));*/
		_betPic = Sprite::create(
			StringUtils::format(BRNN_GAME_TABLERES_BET_JINZHI_V88, _betType + 7));
		//_betPic->setColor(Color3B(155, 155, 155));
	
    }

    _betPic->setPosition(this->getContentSize().width / 2, 0);
    this->addChild(_betPic);
	string ss;
	 if (_betType==1)
	{
		ss = "1万";
	}
	else if (_betType == 2)
	{
		ss = "10万";
	}
	else if (_betType == 3)
	{
		ss = "100万";
	}
	else if (_betType == 4)
	{
		ss = "500万";
	}
	else
	{
		ss = "1千";
	}
	auto tmpTextValue = Text::create("", FONT_TTF_MSYH, 20);
	tmpTextValue->setString(ForceGBKToUtf8(ss.c_str()));
	_betPic->addChild(tmpTextValue);
	tmpTextValue->setPosition(Vec2(_betPic->getContentSize().width / 2, _betPic->getContentSize().height / 2));
		
    return true;
}




void BetObjV88::setClick(bool bClick) {
    _bClick = bClick;
    // this->setSelect(false);
    if (_bClick) {
        _betPic->setTexture(
			StringUtils::format(BRNN_GAME_TABLERES_BET_NORMAL_V88, _betType+7));
		_betPic->setColor(Color3B(255, 255, 255));
		
    } else {
        /*_betPic->setTexture(
            StringUtils::format(BRNN_GAME_TABLERES_BET_DISABLE_V88, _betType));*/
		_betPic->setTexture(
			StringUtils::format(BRNN_GAME_TABLERES_BET_JINZHI_V88, _betType + 7));
    }
}

void BetObjV88::setSelect(bool bSelect) {
    _bSelect = bSelect;
    _effectPic->setVisible(_bSelect);
	_effectPic->setScale(1.0f);
// 	if (_bSelect == true)
// 	{
// 		_betPic->setScale(1.2f);
// 	}
    if (bSelect) {
        //_effectPic->runAction(RepeatForever::create(Blink::create(0.5f, 1)));
		_betPic->setScale(1.0f);
    } else {
        //_effectPic->stopAllActions();
		_betPic->setScale(1.0f);
    }
}
    
    Size BetObjV88::getBetSize() {
        if (_betPic) {
            return _betPic->getContentSize();
        }
        else {
            return Size(104, 109);
        }
    }
    
}
