#include "BJLGameTableUIV88.h"
#include "BJLResultCardV88.h"
#include "Sound/HNAudioEngine.h"

namespace BJL {
#define  ZORDER_CARDBOARD	60

static const char* BJL_GAME_MUSIC_FAPAI = "v66/music/BJL/fapai.mp3";
static const char* BJL_GAME_MUSIC_POINT = "v66/music/BJL/point%d_%d.mp3";
static const char* BJL_GAME_MUSIC_RESULT = "v66/music/BJL/result%d.mp3";
static const char* BJL_GAME_MUSIC_CATCH_ONE = "v66/music/BJL/catchOne%d.mp3";//博牌

static const char* BJL_GAME_HEGUAN = "v88/animation/baccarat/heguan/heguan.csb";

static const int PAI_JIANJU  = 25;     //牌间距

ResultCardV88::~ResultCardV88() {
}

ResultCardV88::ResultCardV88(GameTableUIV88* gameTableUi)
    : _bSendCardIndex(0) {
    _gameTableUi = gameTableUi;

    auto size = gameTableUi->getContentSize();
    _csLoader = CSLoader::createNode("csb/v66/BJL/cardbroad_v88.csb");

    auto panel = _csLoader->getChildByName("Panel_2");
    _panel = panel;

    _csLoader->setPosition(Vec2(WIN_SIZE.width/2 - 185, WIN_SIZE.height - 330));
    gameTableUi->addChild(_csLoader, ZORDER_CARDBOARD);

    Vec2 panelPos = panel->getPosition();

    char chs[30] = {0};
    for (BYTE i = 0 ; i < 2 ; i++  ) {
        sprintf(chs, "cardsss_%d", i);
        cardTest[i] = dynamic_cast<ImageView*>(_csLoader->getChildByName(chs));
    }

    cardsss = dynamic_cast<ImageView*>(_csLoader->getChildByName("cardsss"));

    // 庄家、闲家点数
    leisurePoint = dynamic_cast<TextAtlas*>(_csLoader->getChildByName("AtlasLabel_point1"));
    dealerPoint = dynamic_cast<TextAtlas*>(_csLoader->getChildByName("AtlasLabel_point2"));

    // 比赛结果
    leisure_win = dynamic_cast<ImageView*>(_csLoader->getChildByName("leisure_win"));
    dealer_win = dynamic_cast<ImageView*>(_csLoader->getChildByName("dealer_win"));
    dealer_leisure = dynamic_cast<ImageView*>(_csLoader->getChildByName("equal"));

    for (BYTE i = 0; i < 2; i++) {
        for (BYTE j = 0; j < 3; j++) {
            _spriteCard[i][j] = PokerCardV88::create();
            _spriteCard[i][j]->setAnchorPoint(Vec2(0.5, 0.5));
            _panel->addChild(_spriteCard[i][j], 99999999);
            _spriteCard[i][j]->setScale(0.25f);
            auto des_pos = cardTest[i]->getPosition();
            des_pos.x += j * PAI_JIANJU;
            destinationPositon[i][j] = des_pos;
        }
    }

    for (BYTE i = 0; i < 2; i++) {
        spArr[i] = nullptr;
    }

    //荷官
    ImageView* girl = (ImageView*)_csLoader->getChildByName("Image_Girl");
    girl->setVisible(false);
    Vec2 girlPos = girl->getPosition();

    auto csb = CSLoader::createNode(BJL_GAME_HEGUAN);
    csb->setPosition(Vec2(WIN_SIZE.width / 2 - 185 + girlPos.x, WIN_SIZE.height - 408 + girlPos.y));
    _gameTableUi->addChild(csb, ZORDER_CARDBOARD);

    _antAction = CSLoader::createTimeline(BJL_GAME_HEGUAN);
    _gameTableUi->runAction(_antAction);
}


void ResultCardV88::dispatchCardAnimation(PokerCardV88* PokerCardv88, BYTE state) {
    auto orbit = OrbitCamera::create(0.1f, 1, 0, 0, -90, 0, 0);
    auto orbit1 = OrbitCamera::create(0.2f, 1, 0, 90, -90, 0, 0);
    auto des_pos = destinationPositon[(state + 1) % 2 ][(state - 1) / 2];
    auto seq = Sequence::create(
                   Show::create(),
                   //Spawn::create(ScaleTo::create(0.5f, 1.0f), MoveTo::create(0.5f, des_pos)),
                   //EaseOut::create(EaseSineOut::create(MoveTo::create(0.5f, des_pos)), 0.3f),
                   Spawn::create(EaseOut::create(EaseSineOut::create(MoveTo::create(0.5f, des_pos)), 0.3f), ScaleTo::create(0.5f, 0.85f), nullptr),
                   orbit,
				   CallFuncN::create(CC_CALLBACK_1(ResultCardV88::cardFlipCallback, this)),
                   orbit1,
                   nullptr
               );
	PokerCardv88->runAction(seq);
}

void ResultCardV88::cardFlipCallback(Node* sender) {
    auto card = (PokerCardV88*)sender;
    card->setCardFrameName(card->getCardFileName());
}

bool ResultCardV88::dispatchCard() {
    if (_bSendCardIndex >= 6) {
        //TODO   添加结果
        calculatePoint(true);
        return false;
    }
    ++_bSendCardIndex;

    // 发牌，从闲家开始发
    if (0 == _cards[(_bSendCardIndex + 1)%2][(_bSendCardIndex - 1) / 2]) {
        return dispatchCard();
    } else {
        //博牌
        if (_bSendCardIndex == 5 || _bSendCardIndex == 6) {
            if (_cards[(_bSendCardIndex + 1) % 2][(_bSendCardIndex - 1) / 2] != 0) {
                char str[64] = { 0 };
                sprintf(str, BJL_GAME_MUSIC_CATCH_ONE, (_bSendCardIndex + 1) % 2);
                HNAudioEngine::getInstance()->playEffect(str);
            }
        }

        HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_FAPAI);
        dispatchCardAnimation(_spriteCard[(_bSendCardIndex + 1)%2][(_bSendCardIndex - 1) / 2],_bSendCardIndex);
    }
    return true;
}

void ResultCardV88::showCardBorad() {
// 		auto winSize  = _gameTableUi->getContentSize();
// 		auto size = _csLoader->getContentSize();
// 		auto seq = Sequence::create(
// 			Show::create(),
// 			EaseSineOut::create(MoveTo::create(0.5f, Vec2(winSize.width / 2 , winSize.height - size.height / 2))),
// 			nullptr
// 			);
// 		_csLoader->runAction(seq);
}

void ResultCardV88::showCardBoradWithOutAnimation() {
// 		auto winSize  = _gameTableUi->getContentSize();
// 		auto size = _csLoader->getContentSize();
// 		_csLoader->setVisible(true);
// 		_csLoader->setPosition( Vec2(winSize.width / 2 , winSize.height - size.height / 2));
}

bool ResultCardV88::dispatchCardWithoutAnimation() {
    //TODO   添加结果
    calculatePoint(false);
    //发牌 == 直接显示牌
    for (BYTE i = 0 ; i < 3 ; i++) {
        if (0 != _cards[0][i]) {
            auto des_pos = destinationPositon[0][i];
            _spriteCard[0][i]->setCardFrameName(_spriteCard[0][i]->getCardFileName());
            _spriteCard[0][i]->setPosition(des_pos);
            _spriteCard[0][i]->setVisible(true);
            _spriteCard[0][i]->setScale(0.85f);
        }
        if (0 != _cards[1][i]) {
            auto des_pos = destinationPositon[1][i];
            _spriteCard[1][i]->setCardFrameName(_spriteCard[1][i]->getCardFileName());
            _spriteCard[1][i]->setPosition(des_pos);
            _spriteCard[1][i]->setVisible(true);
			_spriteCard[1][i]->setScale(0.85f);
        }
    }
    return true;
}

void ResultCardV88::calculatePoint(bool bPlay) {
    // 闲家
    char str[50];
    sprintf(str,"%d",_leisureCardtype[1]);

	spArr[0] = LabelAtlas::create(str, "v66/BJL/bjl_number.png", 20, 26, '0');
	_csLoader->addChild(spArr[0]);
	spArr[0]->setAnchorPoint(Vec2(0, 0.5));
	spArr[0]->setPosition(Vec2(leisurePoint->getPositionX(), leisurePoint->getPositionY()));
	spArr[0]->setVisible(false);

    auto leisurePointSprite = ImageView::create("v66/BJL/bjl_dian.png");
	spArr[0]->addChild(leisurePointSprite);
    leisurePointSprite->setPosition(Vec2(38, 12));

    if (_leisureCardtype[1] >= 8) {
        auto tianWanSprite = ImageView::create("v66/BJL/bjl_king.png");
		spArr[0]->addChild(tianWanSprite);
        tianWanSprite->setPosition(Vec2(100, 15));
    }


    // 庄家
    sprintf(str,"%d",_dealerCardtype[1]);

	spArr[1] = LabelAtlas::create(str, "v66/BJL/bjl_number.png", 20, 26, '0');
	_csLoader->addChild(spArr[1]);
	spArr[1]->setAnchorPoint(Vec2(0, 0.5));
	spArr[1]->setPosition(Vec2(dealerPoint->getPositionX(), dealerPoint->getPositionY()));
	spArr[1]->setVisible(false);

    auto dealerPointSprite = ImageView::create("v66/BJL/bjl_dian.png");
	spArr[1]->addChild(dealerPointSprite);
    dealerPointSprite->setPosition(Vec2(38, 12));

    if (_dealerCardtype[1] >= 8) {
        auto tianWanSprite = ImageView::create("v66/BJL/bjl_king.png");
		spArr[1]->addChild(tianWanSprite);
        tianWanSprite->setPosition(Vec2(100, 15));
    }

    leisure_win->setLocalZOrder(_spriteCard[1][2]->getLocalZOrder() + 1);
    dealer_win->setLocalZOrder(_spriteCard[1][2]->getLocalZOrder() + 1);


    if (bPlay) {
        playResult();
    } else {
        spArr[0]->setVisible(true);
        spArr[1]->setVisible(true);

        if (_leisureCardtype[1] > _dealerCardtype[1]) {
            char str[64] = { 0 };
            sprintf(str, BJL_GAME_MUSIC_RESULT, 0);
            HNAudioEngine::getInstance()->playEffect(str);
            leisure_win->setVisible(true);
        } else if (_leisureCardtype[1] < _dealerCardtype[1]) {
            char str[64] = { 0 };
            sprintf(str, BJL_GAME_MUSIC_RESULT, 1);
            HNAudioEngine::getInstance()->playEffect(str);
            dealer_win->setVisible(true);
        } else {
            char str[64] = { 0 };
            sprintf(str, BJL_GAME_MUSIC_RESULT, 2);
            HNAudioEngine::getInstance()->playEffect(str);
            dealer_leisure->setVisible(true);
        }
    }

}

void ResultCardV88::playResult() {
    auto func0 = CCCallFunc::create([&]() {
        char str[64] = { 0 };
        sprintf(str, BJL_GAME_MUSIC_POINT, 0, _leisureCardtype[1]);
        HNAudioEngine::getInstance()->playEffect(str);
        spArr[0]->setVisible(true);
    });

    auto func1 = CCCallFunc::create([&]() {
        char str[64] = { 0 };
        sprintf(str, BJL_GAME_MUSIC_POINT, 1, _dealerCardtype[1]);
        HNAudioEngine::getInstance()->playEffect(str);
        spArr[1]->setVisible(true);
    });


    auto func = CCCallFunc::create([&]() {
        if (_leisureCardtype[1] > _dealerCardtype[1]) {
            char str[64] = { 0 };
            sprintf(str, BJL_GAME_MUSIC_RESULT, 0);
            HNAudioEngine::getInstance()->playEffect(str);
            leisure_win->setVisible(true);
        } else if (_leisureCardtype[1] < _dealerCardtype[1]) {
            char str[64] = { 0 };
            sprintf(str, BJL_GAME_MUSIC_RESULT, 1);
            HNAudioEngine::getInstance()->playEffect(str);
            dealer_win->setVisible(true);
        } else {
            char str[64] = { 0 };
            sprintf(str, BJL_GAME_MUSIC_RESULT, 2);
            HNAudioEngine::getInstance()->playEffect(str);
            dealer_leisure->setVisible(true);
        }
    });

    _gameTableUi->runAction(Sequence::create(func0, DelayTime::create(1.5f), func1, DelayTime::create(1.5f), func, nullptr));
}



void ResultCardV88::hideCardBorad() {
    auto winSize = _gameTableUi->getContentSize();
    auto seq = Sequence::create(
                   EaseSineIn::create(MoveTo::create(0.5f, Vec2(winSize.width / 2 , winSize.height))),
                   Hide::create(),
                   nullptr
               );
    _csLoader->runAction(seq);
}


void ResultCardV88::initCard(BYTE card[2][3], BYTE dealerCardNum, BYTE leisureCardNum, int dealerCardtype[5], int leisureCardtype[5]) {
    memcpy(_dealerCardtype,  dealerCardtype, sizeof(int) * 5);
    memcpy(_leisureCardtype, leisureCardtype, sizeof(int) * 5);
    memcpy(_cards, card, sizeof(_cards));

    _dealerCardNum  = dealerCardNum;
    _leisureCardNum = leisureCardNum;

    for (int i = 0; i < 2; i++) {
        for (int j = 0 ; j < 3 ; j++ ) {
            _spriteCard[i][j]->setCardValue(card[i][j]);
			if (PRESSED_TEXTURE) {
				_spriteCard[i][j]->setCardFrameName("v66/common/card_Common/00.png");
			}
			else {
				_spriteCard[i][j]->setCardFrameName("00.png");
			}
			_spriteCard[i][j]->setPosition(cardsss->getPosition());
			_spriteCard[i][j]->setVisible(false);
        }
    }
}

void ResultCardV88::initNewGame() {
    _bSendCardIndex = 0;
    leisure_win->setVisible(false);
    dealer_win->setVisible(false);
    leisurePoint->setVisible(false);
    dealerPoint->setVisible(false);
    dealer_leisure->setVisible(false);
    for ( BYTE i =  0 ; i < 2; i++) {
        if (nullptr != spArr[i]) {
            spArr[i]->removeFromParent();
            spArr[i] = nullptr;
        }
    }

    for (int i = 0; i < 3; i++) {
		_spriteCard[0][i]->setVisible(false);
		if (PRESSED_TEXTURE) {
			_spriteCard[0][i]->setCardFrameName("v66/common/card_Common/00.png");
		}
		else {
			_spriteCard[0][i]->setCardFrameName("00.png");
        }
        _spriteCard[0][i]->setScale(0.25f);
        _spriteCard[1][i]->setVisible(false);
		if (PRESSED_TEXTURE) {
			_spriteCard[1][i]->setCardFrameName("v66/common/card_Common/00.png");
		}
		else {
			_spriteCard[1][i]->setCardFrameName("00.png");
		}
        _spriteCard[1][i]->setScale(0.25f);
    }
}


void ResultCardV88::playAnimation(int index, bool loop) {
    switch (index) {
    case eye_one:
        _antAction->play("danyanzha", loop);
        break;
    case eye_two:
        _antAction->play("zhayan", loop);
        break;
    case hand_plese:
        _antAction->play("please", loop);
        break;
    case mouth_kiss: {
// 						   CCCallFunc* fun0 = CCCallFunc::create([&](){
// 							   _antAction->play("kiss", loop);
// 						   });
//
// 						   CCCallFunc* fun1 = CCCallFunc::create([&](){
// 							   _antAction->play("kiss", loop);
// 						   });
// 						   CCCallFunc* fun2 = CCCallFunc::create([&](){
// 							   _antAction->play("kiss", loop);
// 						   });
// 						   _gameTableUi->runAction(Sequence::create(fun0, DelayTime::create(1.0f), fun1, DelayTime::create(1.0f), fun2, nullptr));
// 						   break;
        _antAction->play("kiss", loop);
        break;
    }
    case hand_click:
        _antAction->play("qiaoji", loop);
        break;
    default:
        break;
    }
}

bool ResultCardV88::isAnimationPlaying() {
    return _antAction->isPlaying();
}

BYTE ResultCardV88::getCardFileValue(const WORD& card) {
    BYTE value = BYTE( 0x000F & card ) - 1;
    BYTE color = BYTE( 0x00F0 & card );
    //大小王
    if (4 == color || 0x0E == value) {
        value = 0x4E;
    } else if (4 == color || 0x0F == value) {
        value = 0x4F;
    } else {
        color = color>>4;
        color %= 4;
        //花色校准
        if (0 == color) {
            color = 3;
        } else {
            color -= 1;
        }
        color = color<<4;
        value = color | value;
        if (0x00 == value) {
            value = 0x3D;
        }
    }
    return value;
}
}
