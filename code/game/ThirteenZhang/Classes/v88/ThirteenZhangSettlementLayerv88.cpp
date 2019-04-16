#include "ThirteenZhangSettlementLayerV88.h"
#include "ThirteenZhangWrapperV88.h"
#include "ThirteenZhangSoundHelper.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "Tool/Tools.h"
#include "HNCommon/HNConverCode.h"

//USING_NS_CC;
//using namespace ui;

#define WIN_SIZE		Director::getInstance()->getWinSize()

namespace ThirteenZhang {
//////////////////////////////////////////////////////////////////////////

//    const static char* Win_Music = "music/win.mp3";
//    const static char* Lose_Music = "music/lose.mp3";
const static char* Lose_Img = "v66/common/jiesuan/lost.png";
const static char* Win_Img = "v66/common/jiesuan/win.png";
const static char* Ping_Img = "v66/common/jiesuan/ping.png";

SettlementLayerV88::SettlementLayerV88()
    : _callback(nullptr)
//    , _clock(nullptr)
//    , _win(nullptr)
//    , _lose(nullptr)
    , _countdownTime(8)
    , _winAction(nullptr)
    , _winNode(nullptr)
    , _loseAction(nullptr)
    , _loseNode(nullptr)
      //, _zhuang(nullptr)
{
    memset(_results, 0x0, sizeof(_results));
    memset(_nickNames, 0x0, sizeof(_nickNames));
    memset(_textAtlasScores, 0x0, sizeof(_textAtlasScores));
}

SettlementLayerV88::~SettlementLayerV88() {

}

bool SettlementLayerV88::init() {
    if (!HNLayer::init()) {
        return false;
    }
    this->setIgnoreAnchorPointForPosition(false);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [&](Touch* touch, Event* event) {
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    quicklyShade(100);

    //胜利结算
    _winNode = CSLoader::createNode("v88/animation/settle/shengli/shengli.csb");
    _winNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _winNode->setPosition(WIN_SIZE / 2);
    Sprite* winSprite = dynamic_cast<Sprite*>(_winNode->getChildByName("tietu_2"));
    if (winSprite) {
        winSprite->setVisible(false);
    }
    addChild(_winNode, 131);
    _winNode->setVisible(false);

    _winAction = CSLoader::createTimeline("v88/animation/settle/shengli/shengli.csb");
    _winNode->runAction(_winAction);

    //失败结算
    _loseNode = CSLoader::createNode("v88/animation/settle/shibai/shibai.csb");
    _loseNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loseNode->setPosition(WIN_SIZE / 2);
    Sprite* loseSprite = dynamic_cast<Sprite*>(_loseNode->getChildByName("tietu2"));
    if (loseSprite) {
        loseSprite->setVisible(false);
    }
    addChild(_loseNode, 131);
    _loseNode->setVisible(false);

    _loseAction = CSLoader::createTimeline("v88/animation/settle/shibai/shibai.csb");
    _loseNode->runAction(_loseAction);



    auto settlementNode = CSLoader::createNode("Games/12100004/SettlementBox.csb");
    HelperWrapperV88 search(settlementNode);
    bgWinImage = search.find<ImageView>("Image_1");
//    bgWinImage->setVisible(false);
//    bgWinImage->loadTexture("v66/common/jiesuan/jiesuan2/1.png");
    //
    bgLostImage = search.find<ImageView>("Image_2");
    bgLostImage->setVisible(false);
    
    m_myTipImage = search.find<ImageView>("Image_3");
    if (nullptr != m_myTipImage)
    {
        m_myTipImage->setVisible(false);
    }
    
    settlementNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    settlementNode->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 20.0f));
    addChild(settlementNode, 130);
    //

    char buf[64];
    for (int i = 0; i < 4; i++) {
        sprintf(buf, "Text_NickName_%d", i);
        _nickNames[i] = search.find<Text>(buf);
        _nickNames[i]->setString("");
    }

    for (int i = 0; i < 4; i++) {
        sprintf(buf, "resultImg_%d", i);
        _results[i] = search.find<ImageView>(buf);
        _results[i]->loadTexture(Win_Img);
        _results[i]->setVisible(false);

    }

    for (int i = 0; i < 4; i++) {
        sprintf(buf, "BitmapFontLabel_Score_%d", i);
        _textAtlasScores[i] = search.find<TextBMFont>(buf);
        _textAtlasScores[i]->setString("");
    }


//    auto nickname = search.find<Sprite>("nick_name_61");
//    nickname->setVisible(false);
//    auto win_lose = search.find<Sprite>("win_lose_62");
//    win_lose->setVisible(false);
//    auto score = search.find<Sprite>("score_63");
//    score->setVisible(false);
//        _win->setVisible(false);
//        _lose->setVisible(false);

    /* auto btnClose = search.find<Button>("Button_Close");
     btnClose->setVisible(false);*/
//        btnClose->addClickEventListener(CC_CALLBACK_1(SettlementLayerV88::onCloseCallBack, this));
//        search.load(btnClose);
//        _clock = search.find<Text>("AtlasLabel_Clock");

    return true;
}

void SettlementLayerV88::loadParameter(const std::vector<SettlementParameter>& parameters) {
    bool hasPlayAction = false;
    for (size_t i = 0; i < parameters.size(); i++) {
        if (parameters[i].self) {
            //如果自己有参加游戏，以自己的为准，否则显示胜利的动画
            if (parameters[i].score >= 0) {
                if (!hasPlayAction) {
                    _loseAction->stop();
                    _loseNode->setVisible(false);
                    _winNode->setVisible(true);
                    hasPlayAction = true;
//                    bgWinImage->setVisible(true);
//                    bgLostImage->setVisible(false);
                    break;
                }
            } else {
                if (!hasPlayAction) {
                    _winAction->stop();
                    _winNode->setVisible(false);
                    _loseNode->setVisible(true);
                    hasPlayAction = true;
//                    bgWinImage->setVisible(false);
//                    bgLostImage->setVisible(true);
                    break;
                }
            }
        }
    }

    for (size_t i = 0; i < parameters.size(); i++) {
        if (parameters[i].self) {
            if (parameters[i].score > 0) {
                if (hasPlayAction) {
                    _winNode->setVisible(true);
                    _loseNode->setVisible(false);
                    _winAction->play("shengli", false);

                    hasPlayAction = true;
//                    bgWinImage->setVisible(true);
//                    bgLostImage->setVisible(false);
                }
                SoundHepler::getInstance()->playerEffects(GAME_WIN_AUDIO_EFFECT_V66);
            } else {
                if (hasPlayAction) {
                    _winNode->setVisible(false);
                    _loseNode->setVisible(true);
                    _loseAction->play("shibai", false);
                    hasPlayAction = true;
//                    bgWinImage->setVisible(false);
//                    bgLostImage->setVisible(true);
                }
                SoundHepler::getInstance()->playerEffects(GAME_LOSE_AUDIO_EFFECT_V66);
            }
        } else {
            if (!hasPlayAction) {
                _winNode->setVisible(true);
                _loseNode->setVisible(false);
                _winAction->play("shengli", false);
//                bgWinImage->setVisible(true);
//                bgLostImage->setVisible(false);
                hasPlayAction = true;
            }
        }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        _nickNames[i]->setString(ForceGBKToUtf8(parameters[i].nickName.c_str()));
#else
        _nickNames[i]->setString(GBKToUtf8(parameters[i].nickName.c_str()));
#endif

        if (parameters[i].self) {
            auto img = _results[i];
            if (img && nullptr != m_myTipImage) {
                m_myTipImage->setPositionY(img->getPositionY());
                m_myTipImage->setVisible(true);
            }
        }
        
        char buf[64];
        if (parameters[i].score > 0) {
            _results[i]->loadTexture(Win_Img);
            sprintf(buf, "+%s", Tools::addComma(parameters[i].score).c_str());
            _results[i]->setVisible(true);

            _textAtlasScores[i]->setColor(parameters[i].self == true ? Color3B::RED : Color3B::WHITE);
            _nickNames[i]->setColor(parameters[i].self == true ? Color3B::RED : Color3B::WHITE);
            _results[i]->setColor(parameters[i].self == true ? Color3B::RED : Color3B::WHITE);
        } else if (parameters[i].score < 0) {
            _results[i]->loadTexture(Lose_Img);
            sprintf(buf, "%s", Tools::addComma(parameters[i].score).c_str());
            _results[i]->setVisible(true);
            _textAtlasScores[i]->setColor(parameters[i].self == true ? Color3B::GREEN : Color3B::WHITE);
            _nickNames[i]->setColor(parameters[i].self == true ? Color3B::GREEN : Color3B::WHITE);
            _results[i]->setColor(parameters[i].self == true ? Color3B::GREEN : Color3B::WHITE);
        } else {
            sprintf(buf, "%s", Tools::addComma(parameters[i].score).c_str());
            _results[i]->loadTexture(Ping_Img);
            _results[i]->setVisible(true);
            _textAtlasScores[i]->setColor(parameters[i].self == true ? Color3B::YELLOW : Color3B::WHITE);
            _nickNames[i]->setColor(parameters[i].self == true ? Color3B::YELLOW : Color3B::WHITE);
            _results[i]->setColor(parameters[i].self == true ? Color3B::YELLOW : Color3B::WHITE);
        }

        _textAtlasScores[i]->setString(buf);
    }
}

void SettlementLayerV88::startTimer(int time) {
    _countdownTime = time;
    this->unschedule(CC_SCHEDULE_SELECTOR(SettlementLayerV88::scheduleCountdownTimer));
    this->schedule(CC_SCHEDULE_SELECTOR(SettlementLayerV88::scheduleCountdownTimer), 0.5f);//2.5s
}

void SettlementLayerV88::setCloseCallback(const ccSettlementTimer& callback) {
    _callback = callback;
}

void SettlementLayerV88::close() {
    this->unschedule(CC_SCHEDULE_SELECTOR(SettlementLayerV88::scheduleCountdownTimer));

    if (nullptr != _callback) _callback();

    this->runAction(RemoveSelf::create());
}

void SettlementLayerV88::onCloseCallBack(Ref* pSender) {
    SoundHepler::getInstance()->playButtonSoundEffect();
    this->close();
}

void SettlementLayerV88::scheduleCountdownTimer(float dt) {
    if (_countdownTime < 0) {
        this->close();
    }
//        else
//        {
//            char buf[64];
//            sprintf(buf, "%d", _countdownTime);
//            _clock->setString(buf);
//        }
    _countdownTime--;
}
}
