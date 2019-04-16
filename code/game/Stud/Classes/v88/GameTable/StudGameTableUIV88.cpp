#include "StudGameTableUIV88.h"

#include "json/rapidjson.h"
#include "json/document.h"
#include "HNLobbyExport.h"
#include "HNLobby/v66/ExtraReward.h"
#include "StudGameTableLogic.h"

//#include "../../TTNN/Classes/GameTable/BRNNSetLayer.h"
//#include "../../TTNN/Classes/GameTable/BRNNUserInfo.h"
//#include "../../TTNN/Classes/GameTable/BRNNHelpLayer.h"
#include "HNLobby/globel.h"
#include "HNLobby/v66/PlatformResV66.h"
#include <math.h>
#include "Tool/Tools.h"
//#include "../v88/BRNNSetLayerV88.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "UI/Scanner.h"


namespace STUD {
const static int						MEMBAN_ZORDER					= 300;
//static const char*					BACKGROUND							= "v88/niuniu/0.png";
static const char*					GAME_BACKGROUND				= "Games/12100202/bgLeft.png";
static const char*					NN_PREPARE_IMAGE					= "v66/zi/1.png";

#define WINSIZE Director::getInstance()->getWinSize()
#define TOOLBAR_WIDTH 115.0

#define DISTANCE 200.0

#define ANGLE -110
#define TOOLBALL_KIDS_COUNT 3
#define TOOLBALL_OFFSET_Y 5.0f
#define PI 3.1415926

string GameTableUIV88::LongToString(LLONG number) {
    char s[30];
    sprintf(s, "%lld", number);
    string str(s);

    return str;
}

GameTableUIV88::GameTableUIV88() :
    _tableLogic(nullptr),
    _sentStart(0),
    _sentCounter(0),
    _handcardCounter(0),
    _sentTotal(0),
    _pot(0),
    _baseNote(0),
    _tableMenuOutline(nullptr),
    _chipPool(nullptr),
    _hint(nullptr),
    _isOpenHelp(false),
    _sendingCard(false),
    toolBar_flag(true),
//    _clockMusicId(0),

    m_bNextOut(false)
    , m_Notice(nullptr) {
    _vecCardPos.clear();
    _vecAreadyPos.clear();
    _vecAreadySprite.clear();

    memset(&_dashboardCanvas, 0, sizeof(_dashboardCanvas));
    memset(&_tableCanvas, 0, sizeof(_tableCanvas));
    memset(_players, 0, sizeof(_players));
    memset(_cardSuite, 0, sizeof(_cardSuite));
}

GameTableUIV88::~GameTableUIV88() {
//    this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
    HN_SAFE_DELETE(_tableLogic);
    m_Notice = nullptr;
}

HNGameUIBase* GameTableUIV88::create(BYTE deskId, bool autoCreate) {
    GameTableUIV88* tableUI = new GameTableUIV88();
    if (tableUI->init(deskId, autoCreate)) {
        tableUI->autorelease();
        return tableUI;
    } else {
        CC_SAFE_DELETE(tableUI);
        return nullptr;
    }
}

void GameTableUIV88::onExit() {
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("v66/common/card_AMax/cards.plist");
//    HNAudioEngine::getInstance()->stopEffectWithID(_clockMusicId);
    HNGameUIBase::onExit();
}

void GameTableUIV88::onEnter() {
    HNGameUIBase::onEnter();

    _loginResult = PlatformLogic()->getInstance()->loginResult;

    auto bg = Sprite::create(GAME_BG_COMMON_V66);
    if (bg) {
        bg->setPosition(WIN_SIZE / 2);
        addChild(bg);
    }
//    setBackGroundImage(GAME_BG_COMMON_V88);
    loadCardPosition();
    createToolBarUI();

    loadAreadySprite();
    createClock();

    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("v66/common/card_AMax/cards.plist");

    Size winSize = Director::getInstance()->getWinSize();

    //触摸
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
    myListener->setSwallowTouches(false);
    dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

    // initial table outline
    auto node = CSLoader::createNode("csb/v66/stud/table/table.csb");
    _tableCanvas.outline = (Layout*)node->getChildByName("table_panel");

    _tableCanvas.seats[0] = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableCanvas.outline, "me_btn"));
    _tableCanvas.seats[1] = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableCanvas.outline, "player_btn"));

    _tableCanvas.topTotalBg = dynamic_cast<Sprite*>(_tableCanvas.outline->getChildByName("Sprite_top"));
    _tableCanvas.betArea = dynamic_cast<Sprite*>(_tableCanvas.outline->getChildByName("betArea"));
    _tableCanvas.bottomTotalBg = dynamic_cast<Sprite*>(_tableCanvas.outline->getChildByName("Sprite_bottom"));

    _tableCanvas.fapaiqi = dynamic_cast<Sprite*>(_tableCanvas.outline->getChildByName("fapaiqi"));
    _tableCanvas.fapaiqi->setLocalZOrder(1000000);

    _tableCanvas.topTotal = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_tableCanvas.outline, "1"));
    _tableCanvas.bottomTotal = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_tableCanvas.outline, "0"));

    _tableCanvas.totalMoney = dynamic_cast<Text*>(node->getChildByName("totalMoney"));

    this->addChild(node, 1);
    _tableCanvas.outline->setAnchorPoint(Vec2(0.5, 0.5));
    _tableCanvas.outline->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

    // dealer cards anchor
    _tableCanvas.dealer = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableCanvas.outline, "dealer_card_btn"));
    _tableCanvas.dealer->setVisible(false);

    // dashboard
    auto DashboardNode = CSLoader::createNode("csb/v66/stud/table/table_dashboard.csb");
    dashboard_panel = (Layout*)DashboardNode->getChildByName("dashboard_panel");
    //DashboardNode->setPositionY();    //auto tableDashboard = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("stud/table/table_dashboard.json");
    this->addChild(DashboardNode);
    dashboard_panel->setAnchorPoint(Vec2(0.5, 0));
    dashboard_panel->setPosition(Vec2(winSize.width / 2, 0));

    auto noteBg = dynamic_cast<Sprite*>(dashboard_panel->getChildByName("note_bg"));
    _dashboardCanvas.bet[0] = dynamic_cast<Button*>(noteBg->getChildByName("bet1_btn"));
    _dashboardCanvas.bet[1] = dynamic_cast<Button*>(noteBg->getChildByName("bet2_btn"));
    _dashboardCanvas.bet[2] = dynamic_cast<Button*>(noteBg->getChildByName("bet3_btn"));

	noteBg->setVisible(false);

    _dashboardCanvas.start = AUIButton::create("v66/button/yellow1/BG.png", "v66/button/yellow1/BG.png", "v66/button/yellow1/BG.png");
    _dashboardCanvas.start->setName("start_btn");
    _dashboardCanvas.start->setPosition(Vec2(winSize.width / 2.0f,winSize.height *0.4f));
    _dashboardCanvas.start->setPressedActionEnabled(true);
    _dashboardCanvas.start->setTitleOffset(Vec2(4, 4));
    _dashboardCanvas.start->setTitle("v66/button/yellow1/6.png");
//	_dashboardCanvas.start->setGray(true);
    _dashboardCanvas.start->setEnabled(true);
    this->addChild(_dashboardCanvas.start,98);

    _dashboardCanvas.bet[0]->setShowEffect(false);
    _dashboardCanvas.bet[1]->setShowEffect(false);
    _dashboardCanvas.bet[2]->setShowEffect(false);

    char*buttonName[4] = { "fold_btn", "showha_btn", "call_btn", "jiazhu_btn" };
    char*buttonTitle[4] = { "1","3", "4", "5" };
    for (int i = 0; i < 4; i++) {
        char title[50] = { "v66/button/yellow1/" };
        string bg = "v66/button/yellow1/BG.png";
        sprintf(title, "%s%s.png", title, buttonTitle[i]);
        auto menuBtn = AUIButton::create(bg, bg);
        menuBtn->setScale(0.8f);
        menuBtn->setName(buttonName[i]);
        menuBtn->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
        //menuBtn->setPosition(Vec2(winSize.width - (3 - i)*(menuBtn->getContentSize().width * 0.8f + 10.0f) - 10.0f, 10.0f));
		menuBtn->setPosition(Vec2(winSize.width - (3 - i)*(menuBtn->getContentSize().width* 0.8f + 10.0f) - 160.0f, 10.0f));
        menuBtn->setPressedActionEnabled(true);
        menuBtn->setTitle(title);
        menuBtn->setTitleOffset(Vec2(2, 2));
        //		menuBtn->setGray(true);
        menuBtn->setEnabled(true);
        menuBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::menuClickCallback, this));
        dashboard_panel->addChild(menuBtn);
    }

    _dashboardCanvas.bet[0]->setPressedActionEnabled(true);
    _dashboardCanvas.bet[1]->setPressedActionEnabled(true);
    _dashboardCanvas.bet[2]->setPressedActionEnabled(true);
    _dashboardCanvas.start->setPressedActionEnabled(true);

    _dashboardCanvas.bet[0]->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::menuClickCallback, this));
    _dashboardCanvas.bet[1]->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::menuClickCallback, this));
    _dashboardCanvas.bet[2]->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::menuClickCallback, this));
    _dashboardCanvas.start->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::startClickCallback, this));

    _dashboardCanvas.bet[0]->setTitleText(LongToString(0));
    _dashboardCanvas.bet[1]->setTitleText(LongToString(0));
    _dashboardCanvas.bet[2]->setTitleText(LongToString(0));

    setBetAndOtherBtnOpen(false);

	auto limitBg = Sprite::create("Games/12100202/1.png");
	limitBg->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	limitBg->setPosition(Vec2(WIN_SIZE.width - 10, WIN_SIZE.height - 10));
	addChild(limitBg);

	_tableCanvas.limit = Text::create("0", FONT_TTF_FZCY, 30);
	_tableCanvas.limit->setAnchorPoint(Vec2(0, 0.5));
	_tableCanvas.limit->setContentSize(Size(300, 30));
	_tableCanvas.limit->setColor(Color3B::WHITE);
	_tableCanvas.limit->setPosition(Vec2(WIN_SIZE.width - 10, WIN_SIZE.height - 10));
	_tableCanvas.limit->setPosition(Vec2(138.0f / 1280 * winSize.width, limitBg->getContentSize().height - _tableCanvas.limit->getContentSize().height *0.85f));
	limitBg->addChild(_tableCanvas.limit);

	_tableCanvas.base = Text::create("0", FONT_TTF_FZCY, 30);
	_tableCanvas.base->setAnchorPoint(Vec2(0, 0.5));
	_tableCanvas.base->setContentSize(Size(300, 30));
	_tableCanvas.base->setColor(Color3B::WHITE);
	_tableCanvas.base->setPosition(Vec2(120.0f / 1280 * winSize.width, limitBg->getContentSize().height - _tableCanvas.base->getContentSize().height *1.85f));
	limitBg->addChild(_tableCanvas.base);

	_tableCanvas.totalMoney = Text::create("0", FONT_TTF_FZCY, 30);
	_tableCanvas.totalMoney->setAnchorPoint(Vec2(0, 0.5));
	_tableCanvas.totalMoney->setContentSize(Size(300, 30));
	_tableCanvas.totalMoney->setColor(Color3B::WHITE);
	_tableCanvas.totalMoney->setPosition(Vec2(86.0f / 1280 * winSize.width, limitBg->getContentSize().height - _tableCanvas.base->getContentSize().height *2.8f));
	limitBg->addChild(_tableCanvas.totalMoney);

    // init game table logic
    _tableLogic = new GameTableLogic(this, m_deskId, m_autoCreate);
    _tableLogic->enterDesk();
    _tableLogic->m_gameState = GAME_FREE;
}

bool GameTableUIV88::init(BYTE deskId, bool autoCreate) {
    if (!HNLayer::init()) {
        return false;
    }
    m_deskId = deskId;
    m_autoCreate = autoCreate;
    return true;
}
void GameTableUIV88::loadBackGround() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float _xScale = visibleSize.width / 1280;
    float _yScale = visibleSize.height / 720;

    auto bgLeft = Sprite::create(GAME_BACKGROUND);
    auto bgSize = bgLeft->getContentSize();
    bgLeft->setScaleX(_xScale);
    bgLeft->setScaleY(_yScale);
    bgLeft->setName("backGround");
    bgLeft->setPosition(Vec2(WINSIZE.width / 4 * 1, WINSIZE.height*0.5f));
    this->addChild(bgLeft, -1);

    auto bgRigtht = Sprite::create(GAME_BACKGROUND);
    bgRigtht->setFlippedX(true);
    bgRigtht->setScaleX(_xScale);
    bgRigtht->setScaleY(_yScale);
    bgRigtht->setPosition(Vec2(WINSIZE.width / 4 * 3, WINSIZE.height*0.5f));
    this->addChild(bgRigtht, -1);
}

void GameTableUIV88::createClock() {
    m_timerObj = TimerObject::create();
    if (m_timerObj) {
        addChild(m_timerObj, 66);
        m_timerObj->setPosition(Vec2(WINSIZE.width / 2.0f, WINSIZE.height /2.0f + 25));
//        m_timerObj->setTimeCallBack([&]() {
////            if (_players[this->getTag()]->isMe()) {
//            auto mySeat = _tableLogic->getMySeatNo();
//            if (INVALID_DESKSTATION != mySeat && PLAY_COUNT > mySeat) {
//                _tableLogic->sendUserUp();
//            }
////            }
//        });
    }
}

    void GameTableUIV88::startClock(BYTE seatID, int time) {
        if (m_timerObj) {
//            m_timerObj->setTag(seatID);
            m_timerObj->start(time);
        }
    }
    
    void GameTableUIV88::stopClock() {
        if (m_timerObj) {
            m_timerObj->stop();
        }
    }

    float GameTableUIV88::readTimer() {
        if (m_timerObj) {
            return m_timerObj->readTimer();
        }
        return 0;
    }
    
//void    GameTableUIV88::hideClock() {
//    m_ClockBg->setVisible(false);
//    m_ClockTimeLabel->setVisible(false);
//    HNAudioEngine::getInstance()->stopEffectWithID(_clockMusicId);
//    m_pClock->setVisible(false);
//}

void GameTableUIV88::createToolBarUI() {
    int zOrder = 100;
    auto toolbar = ToolBarLayer::create();
    if (toolbar) {
        toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
            if (_tableLogic->m_gameState != GAME_FREE) {
                //GamePromptLayer::create()->showPrompt(GetText("limit_leave_in_game"));
				GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
				layer->setGamePromptTip(GetText("limit_leave_in_game"));
            } else {
                _tableLogic->sendUserUp();
            }
        });
        toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
            IGameSet* gameSet = GameSetFactory::createWithVersion();
            gameSet->setBgPath("");
            gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
        });
        toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
            auto helpLayer = HN::HelpLayer::create();
            helpLayer->setContentOnView("v66/common/help/suoha.png");
            helpLayer->setName("helpLayer");
            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            helpLayer->setOrignalPosition();
            helpLayer->moveSelfInWithAction();
            addChild(helpLayer, zOrder + 5);
        });
        toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
    }
}

void GameTableUIV88::loadAreadySprite() {
    Point pos[2] = { Point(WINSIZE.width*0.5f, WINSIZE.height*0.3f), Point(WINSIZE.width*0.5f, WINSIZE.height*0.7f) };
    for (int i = 0; i < 2; i++) {
        auto spr = Sprite::create(NN_PREPARE_IMAGE);
        spr->setVisible(false);
        spr->setPosition(pos[i]);
        this->addChild(spr,99);
        _vecAreadySprite.push_back(spr);
    }
}

bool GameTableUIV88::onTouchBegan(Touch* touch, Event* event) {

//    if (0 == _toolbarBg->getName().compare("1")) {
//
//		_toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//			CallFunc::create([this]() {
//			_toolbarBg->setName("0");
//			this->getChildByTag(em_toolbarBtn)->setVisible(true);
//		})));
//        return true;
//    }

    return true;
}

//void GameTableUIV88::onBtnClick(Ref* pSender, Control::EventType event) {
//    Node* btn = (Node*)pSender;
//    switch (btn->getTag()) {
//    case em_toolbarBtn: {
//							if (0 == _toolbarBg->getName().compare("0")) {
//								_toolbarBg->setName("1");
//								_toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height)), nullptr));
//								this->getChildByTag(em_toolbarBtn)->setVisible(false);         //whao
//							}
//
//    }
//    break;
//    case em_toolbarSetBtn: {
//							   if (this->getChildByName("BRNNSetLayerV88") == nullptr)
//							   {
//								   auto setLayer = BRNNSetLayerV88::create();
//								   setLayer->setName("BRNNSetLayerV88");
//								   setLayer->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
//								   setLayer->showSet(this, 99999);
//
//								   _toolbarBg->runAction(Sequence::create(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 0.5f)), CallFunc::create([this](){
//									   _toolbarBg->setName("0");
//									   this->getChildByTag(em_toolbarBtn)->setVisible(true);
//								   }), nullptr));
//							   }
//        if (this->getChildByName("helpLayer")) {
//            ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
//        }
//    }
//    break;
//    case  em_toolbarHelpBtn: {
//								hintClickCallback(btn, Widget::TouchEventType::ENDED);
//								_toolbarBg->runAction(Sequence::create(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 0.5f)), CallFunc::create([this](){
//									_toolbarBg->setName("0");
//									this->getChildByTag(em_toolbarBtn)->setVisible(true);
//								}), nullptr));
//    }
//    break;
//    case em_toolbarLeaveBtn: {
//        if (_tableLogic->m_gameState != GAME_FREE) {
//            GamePromptLayer::create()->showPrompt(GetText("GAME_PLAYING_NOT_EXIT"));
//			_toolbarBg->runAction(Sequence::create(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 0.5f)), CallFunc::create([this](){
//				_toolbarBg->setName("0");
//				this->getChildByTag(em_toolbarBtn)->setVisible(true);
//			}), nullptr));
//        } else {
//            _tableLogic->sendUserUp();
//        }
//    }
//    break;
//    case em_toolbarNextOutBtn: {
//        if (btn->getUserData() == (void*)true) {
//            btn->setUserData((void*)false);
//            auto spr = static_cast<Sprite*>(_toolbarBg->getChildByTag(em_toolbarNextOutLight));
//            if (spr)
//                spr->setTexture("Games/12100004/xialaanniu/27.png");
//            m_bNextOut = false;
//        } else if (btn->getUserData() == (void*)false) {
//            btn->setUserData((void*)true);
//            auto spr = static_cast<Sprite*>(_toolbarBg->getChildByTag(em_toolbarNextOutLight));
//            if (spr)
//                spr->setTexture("Games/12100004/xialaanniu/28.png");
//            m_bNextOut = true;
//        }
//    }
//    break;
//    default:
//        break;
//    }
//}

void GameTableUIV88::nextTurnLeavlDealCallBack(float dt) {
    if (m_bNextOut) {
        _tableLogic->sendUserUp();
    }
}

    void GameTableUIV88::onHandleNewMessage(int iType, const std::string &message, int iCount)
    {
        //全服公告
        if (iType == 0)
        {
            showNotice(message, 100, iType);
        }
        else
        {
            showNotice(message, iCount, iType);
        }
    }

//void GameTableUIV88::clockHide(bool visible) {
////    m_ClockBg->setVisible(visible);
//}

void GameTableUIV88::nextTurnLeavl() {
    scheduleOnce(schedule_selector(GameTableUIV88::nextTurnLeavlDealCallBack), 3.5);
}

void GameTableUIV88::aControlBtnClick(Ref* pSender, Control::EventType event) {
    if (pSender == _dashboardCanvas.call) {
        _tableLogic->sendFollow();
    } else if (pSender == _dashboardCanvas.allIn) {
        _tableLogic->sendShuttle();
    } else if (pSender == _dashboardCanvas.fold) {
        _tableLogic->sendGiveUp();
    }
}

void GameTableUIV88::startClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (Widget::TouchEventType::ENDED == touchtype) {
        //_tableCanvas.start->setVisible(false);
        _tableLogic->sendAgreeGame();
    }
}

void GameTableUIV88::setClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (Widget::TouchEventType::ENDED == touchtype) {
        auto setLayer = GameSetLayer::create();
        auto winSize = Director::getInstance()->getWinSize();
        setLayer->setPosition(Vec2(0, 0));
        addChild(setLayer, 100);
    }
}


//void GameTableUIV88::hintClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
//    if (Widget::TouchEventType::ENDED == touchtype) {
//        auto winSize = Director::getInstance()->getWinSize();
//
//        if (_isOpenHelp == false) {
////            auto hintLayer = LayerColor::create(Color4B::BLACK);
////            hintLayer->setOpacity(10);
////            this->addChild(hintLayer, 100);
////
////            auto helpNode = CSLoader::createNode("csb/v66/stud/table/help_node.csb");
////            auto bg = static_cast<ImageView*>(helpNode->getChildByName("Image_1"));
////            bg->setTouchEnabled(true);
////            bg->setSwallowTouches(true);
////            helpNode->setPosition(Vec2(WINSIZE.width*0.5f, WINSIZE.height*0.5f));
////            helpNode->setScale(0.0);
////            hintLayer->addChild(helpNode, 99999);
////
////            //_hint = static_cast<Sprite*>(helpNode->getChildByName("help_suoha"));
////
////            helpNode->runAction(Sequence::create(ScaleTo::create(0.3f, 1.0f),nullptr));
////            auto listener = EventListenerTouchOneByOne::create();
////            listener->onTouchBegan = [=](Touch* touch, Event* event) {
////                helpNode->runAction(Sequence::create(ScaleTo::create(0.3f, 0.0f), CallFunc::create([=]() {
////                    hintLayer->removeFromParent();
////                }), nullptr));
////                return false;
////            };
////
////            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, hintLayer);
//            auto helpLayer = HN::HelpLayer::create();
//            helpLayer->setContentOnView("v66/common/help/suoha.png");
//            helpLayer->setName("helpLayer");
//            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//            helpLayer->setOrignalPosition();
//            helpLayer->moveSelfInWithAction();
//            addChild(helpLayer, 60);
//        }
//    }
//}


void GameTableUIV88::showLoading() {
    auto loadSprite = Sprite::create("table/loading_view.jpg");

    Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    loadSprite->setAnchorPoint(Vec2(0, 0));
    loadSprite->setPosition(Vec2(0, 0));
    this->addChild(loadSprite, MEMBAN_ZORDER);

    auto fadeOut = FadeOut::create(2.0f);
    loadSprite->runAction(Sequence::create(
                              fadeOut,
                              RemoveSelf::create(true),
                              nullptr));
}


//////////////////////////////////////////////////////////////
// new

void GameTableUIV88::loadUser(BYTE seatId, LLONG userId) {
    if (_players[seatId] != nullptr && _players[seatId]->getUserId() != userId) {
        _players[seatId]->removeFromParent();
        _players[seatId] = nullptr;
    }
    int direction = userId != _loginResult.dwUserID ? PlayerUIV88::TOP : PlayerUIV88::BOTTOM;  // 1为上方玩家 0位自己 根据这个初始化不同CSB

    if (_players[seatId] == nullptr) {
        auto winSize = Director::getInstance()->getWinSize();

		// create a new users
		auto user = _tableLogic->getUserBySeatNo(seatId);
		_players[seatId] = PlayerUIV88::create(userId, direction); // 2016 7 28 临时处理 补进度 判断是不是自己 不是自己玩家UI位置偏  wzx

//        auto size = _players[seatId]->getSize();
        if (userId != _loginResult.dwUserID) {
            _players[seatId]->setPosition(Vec2(WINSIZE.width*0.5f - _players[seatId]->getSize().width * 1.25f /1280 *WINSIZE.width, WINSIZE.height - _players[seatId]->getSize().height));
            _players[seatId]->changeOtherBackGround(1);
        } else {
            _players[seatId]->setPosition(Vec2(0, 0));
        }
        addChild(_players[seatId], _tableCanvas.outline->getParent()->getLocalZOrder());
        _players[seatId]->setTableUI(this);

        // hide the seat button
        _tableCanvas.seats[seatId]->setVisible(false);
    } else {
        // update userinfo if the user exist
//        auto tp = PlayerUIV88::create(userId, direction);
        _players[seatId]->resetPot();
        _players[seatId]->reloadMoney();
    }
    
    UserInfoStruct * user = _tableLogic->getUserBySeatNo(seatId);
	if (_players[seatId] && user) {
		_players[seatId]->setHead(user);
	}

    // if i'm sitting, disable all seats
    if (_players[seatId]->isMe()) {
        for (auto s : _tableCanvas.seats) {
            s->setEnabled(false);
        }
    }
}

void GameTableUIV88::getReady() {
    if (_dashboardCanvas.start->isVisible()) {
        _tableLogic->sendUserUp();
    }
}

void GameTableUIV88::showUserUp(BYTE seatId) {
    if (_players[seatId]->isMe()) {
        for (auto s : _tableCanvas.seats) {
            s->setEnabled(true);
        }
    }
//    _players[seatId]->stopTimer();
    _players[seatId]->removeThis();
    _players[seatId] = nullptr;
    _tableCanvas.seats[seatId]->setVisible(true);
    setAreadySpriteVisible(false, seatId);
//    clockHide(true);
    stopClock();
}

void GameTableUIV88::dealUserUpResp(float detal) {

}

void GameTableUIV88::seatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (Widget::TouchEventType::ENDED == touchtype) {
        for (int i = 0; i < PLAY_COUNT; i++) {
            if (pSender == _tableCanvas.seats[i]) {
                _tableLogic->sendUserSit(i);
                break;
            }
        }
    }
}

void GameTableUIV88::startTimer(BYTE seatId, BYTE leftTime) {
    if (_players[seatId] != nullptr) {
        startClock(seatId, leftTime);
        if (m_timerObj) {
            if (_players[seatId]->isMe()) {
                m_timerObj->setTimeCallBack([&]() {
                    if (_tableLogic->m_gameState == GAME_FREE) {
                        _tableLogic->sendUserUp();
                    }
                });
            }
        }
    }
}

void GameTableUIV88::stopTimer(BYTE seatId) {
    if (_players[seatId] != nullptr) {
        if (_tableLogic->m_gameState == GAME_FREE) {
            if (_players[seatId]->isMe()) {
                stopClock();
            }
        }
        else {
            stopClock();
        }
    }
}

//void GameTableUIV88::refreshMyTimer(float delta) {
//    auto mySeat = _tableLogic->getMySeatNo();
//    if (INVALID_DESKSTATION != mySeat && PLAY_COUNT > mySeat && _players[mySeat]->readTimer() <= 0) {
//        unschedule(schedule_selector(GameTableUIV88::refreshMyTimer));
//        _tableLogic->sendUserUp();
//    }
//}

void GameTableUIV88::initGameUi(GameBeginStruct* buff) {
    _pot = 0;
    _baseNote = buff->iBaseNote;

    string str;

    // init dashboard
    setBetBtnText(buff->iBaseNote);
    setBetBtnEnabled(false);
    setBetAndOtherBtnOpen(false);


    // reset pot
    for (int i = 0; i < PLAY_COUNT; i++) {
        if (_players[i] != nullptr) {
            _players[i]->resetPot();
            _players[i]->setButton(false);
        }
    }
}
void GameTableUIV88::recordDeskInfo(LLONG zhu[]) {
    for (int i = 0; i < PLAY_COUNT; i++) {
        if (i != _tableLogic->getMySeatNo()) {
            _tableCanvas.topTotal->setString(to_string(zhu[i]));
        } else {
            _tableCanvas.bottomTotal->setString(to_string(zhu[i]));
        }
    }
}

    void GameTableUIV88::resetNote()
    {
        for (int i = 0; i < PLAY_COUNT; i++)
        {
            _players[i]->setTotalBetMoney(0);
        }
        
        _tableCanvas.totalMoney->setString("0");
        _pot = 0;
    }

void GameTableUIV88::updateNote(BYTE seatId, LLONG money) {
    _players[seatId]->bet(money);
    throwChip(seatId, money);

    _pot += money;
    string str = ForceGBKToUtf8("");
    str.append(LongToString(_pot));


    dynamic_cast<Text*>(ui::Helper::seekWidgetByName(_tableCanvas.outline, to_string(int(_tableLogic->logicToViewSeatNo(seatId)))))->setString(to_string(_players[seatId]->getTotalBetMoney()));

    if (money > 0) {
        _tableCanvas.totalMoney->setString(str);
        //auto font = _tablecanvas.pot->gettitlefontname();
        //auto fontweight = _tablecanvas.pot->gettitlefontsize();
        //auto fontcolor = _tablecanvas.pot->getcolor();
        //auto potanimate = text::create(str, font, fontweight);

        //potanimate->setcolor(fontcolor);
        //potanimate->setposition(vec2(_tablecanvas.pot->getpositionx() + 2, _tablecanvas.pot->getpositiony()-3));
        //potanimate->setanchorpoint(_tablecanvas.pot->getanchorpoint());
        //_tablecanvas.pot->getparent()->addchild(potanimate, _tablecanvas.pot->getlocalzorder() + 10);
        //_tablecanvas.pot->settitletext("");
        //_tablecanvas.pot->settitletext(str);
        //potanimate->setscale(1.1f);
        //potanimate->runaction(sequence::create(
        //	//scaleto::create(0.3f, 1.3f),
        //	fadeout::create(0.2f),
        //	removeself::create(true),
        //	//callfunc::create([=](){ _tablecanvas.pot->settitletext(str);}),
        //	nullptr));
    }
}

void GameTableUIV88::userPlayerCardTypeEffect(BYTE type,float dt) {
    switch (type) {
    case SH_ERROR_KIND: { // 0 错误
        HNLOG(" 688  userPlayerCardTypeEffect value error !!!");
        break;
    }
    case SH_OTHER: // 1 散牌
    case SH_DOUBLE: //2 对子
    case SH_TWO_DOUBLE: // 3 两对
    case SH_THREE: // 4 三条
    case SH_SMALL_CONTINUE: // 5 最小顺子
    case SH_CONTINUE: // 6 顺子
    case SH_SAME_HUA: // 7 同花
    case SH_HU_LU: // 8 葫芦
    case SH_TIE_ZHI: // 9 铁枝
    case SH_SMALL_SAME_HUA_CONTINUE: // 10 同花最小顺子
    case SH_SAME_HUA_CONTINUE: { // 同花顺
        char  str[50] = { 0 };
        sprintf(str, "music/Stud/%d.mp3", type);
        //HNAudioEngine::getInstance()->playEffect(str);

        this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([&]() {
            HNAudioEngine::getInstance()->playEffect(str);
        }),nullptr));
        break;
    }
    default:
        break;
    }
}


void GameTableUIV88::playNoteAudio(BYTE seatId, BYTE audioType) {
    _players[seatId]->playNoteAudio(audioType);
}


void GameTableUIV88::updateHandcard(SendOneTurnCardStruct* buff) {
    if (buff == nullptr)
        return;

    // data
    for (int i = 0; i < buff->iFirstSendCardCount; i++) {
        for (int seat = 0; seat < PLAY_COUNT; seat++) {
            int value;
            if (_players[seat]->isMe()) {
                value = buff->iCardList[seat][i];
            } else {
                value = i == 0 ? 0x00 : buff->iCardList[seat][i];
            }

            _players[seat]->setHandCard(i, value, false);
        }
    }

    // animate
    for (int i = 0; i < buff->iFirstSendCardCount; i++) {
        auto tmp = new SendOneTurnCard();
        tmp->bStartPos = buff->iStartPos;
        ::memcpy(tmp->iTurnCard, buff->iCardList[i], sizeof(tmp->iTurnCard));
        _sendCardQueue.push_back(tmp);
    }

    if (!_sendingCard && !_sendCardQueue.empty()) {
        _sendingCard = true;
        _sentStart = _sendCardQueue.front()->bStartPos;
        _sentTotal = PLAY_COUNT;
        _sentCounter = 0;
        scheduleOnce(schedule_selector(GameTableUIV88::dealcards), 0.2f);
    }
}

void GameTableUIV88::updateHandcard(SendOneTurnCard* buff) {
    if (buff == nullptr)
        return;

    // data
    for (int seat = 0; seat < PLAY_COUNT; seat++) {
        _players[seat]->setHandCard(_handcardCounter, buff->iTurnCard[seat], false);
    }

    // animate
    _sendCardQueue.push_back(buff);
    if (!_sendingCard && !_sendCardQueue.empty()) {
        _sendingCard = true;
        _sentStart = _sendCardQueue.front()->bStartPos;
        _sentTotal = PLAY_COUNT;
        _sentCounter = 0;
        scheduleOnce(schedule_selector(GameTableUIV88::dealcards), 0.2f);
    }
}
//初始化卡牌坐标
void GameTableUIV88::loadCardPosition() {
    float pianyi = 30.f;
//    float xOff = pianyi / 1280.0f * WINSIZE.width;
    Point pos[2] = { Point(WINSIZE.width*0.5f, WINSIZE.height*0.2), Point(WINSIZE.width*0.5f, WINSIZE.height*0.8) };
    for (int i = 0; i < 2; i++) {
        _vecCardPos.push_back(pos[i]);
    }
}

void GameTableUIV88::userMoneyNotEnouth(tag_s2c_tip_exit* msg) {
	if (_tableLogic->getMySeatNo() != msg->bChairID) {
        if (msg->bIsExit == 1) {
            //金币不足的不是自己，需要踢人的操作
            auto node = CSLoader::createNode("v88/animation/cow/gameover/gameover.csb");
            if (node) {
                node->setPosition(_players[msg->bChairID]->getPosition() + Vec2(100, 0));
                addChild(node, 101);
                auto action = CSLoader::createTimeline("v88/animation/cow/gameover/gameover.csb");
                if (action) {
                    node->runAction(action);
                    action->play("gameover", false);
                    //
                    runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
                        if (_players[msg->bChairID] != nullptr) {
                            _players[msg->bChairID]->setVisible(false);
                            updateNote(msg->bChairID, 0);
                        }
                    }), nullptr));
                    
                }
            }
        }
    }
    else {
        tipMsg = msg;
        //不退出
        if (tipMsg->bIsExit == 1) {
            //要退出
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
                tipCallback();
            })));
        }
    }
}

void GameTableUIV88::playScanner()
{
	auto scanner = Scanner::create();
	this->addChild(scanner, 100000000);
	scanner->starPlayerEffect();
	scanner->setPosition(250, 220);
}

void GameTableUIV88::tipCallback() {
    if (tipMsg->bIsExit == 1) {

        auto layer = GamePromptLayer::create();
//        layer->setCloseExitBtn();
//         char str[128] = { 0 };
//         int sec = 3;
//         sprintf(str, "金币数额低于%lld，%ds后离开游戏！", tipMsg->i64Money, sec);
//         layer->showPrompt(GBKToUtf8(str));
        int sec = 3;
        char s[128] {0};
        std::string str;
        str.append(GBKToUtf8("金币数额低于"));
        sprintf(s, "%lld", tipMsg->i64Money);
        str.append(s);
        sprintf(s, ",%d", sec);
        str.append(s);
        str.append(GBKToUtf8("s后离开游戏！"));
        layer->showPrompt(str);

        layer->setOkayCallBack([&]() {
            unschedule(schedule_selector(GameTableUIV88::updateTip));
            _tableLogic->sendUserUp();
        });
        layer->setColorLayerTouchEnabled(false);

        m_i64TipMoney = tipMsg->i64Money;
        m_TextTip = layer->getPromptText();
        m_TextTip->setTag(sec);
        schedule(schedule_selector(GameTableUIV88::updateTip), 1.0f);
    } else {

    }
}

void GameTableUIV88::updateTip(float dt) {
    int iTag = m_TextTip->getTag();
    if (iTag == 0) {
        unschedule(schedule_selector(GameTableUIV88::updateTip));
        _tableLogic->sendUserUp();
        return;
    }

//     char str[128] = { 0 };
//     iTag -= 1;
//     sprintf(str, "金币数额低于%lld，%ds后离开游戏！", m_i64TipMoney, iTag);
//     m_TextTip->setString(GBKToUtf8(str));
//     m_TextTip->setTag(iTag);

    iTag -= 1;

    char s[128] {0};
    std::string str;
    str.append(GBKToUtf8("金币数额低于"));
    sprintf(s, "%lld", m_i64TipMoney);
    str.append(s);
    sprintf(s, ",%d", iTag);
    str.append(s);
    str.append(GBKToUtf8("s后离开游戏！"));
    m_TextTip->setString(str);
    m_TextTip->setTag(iTag);
}

void GameTableUIV88::dealcards(float delta) {
    if (_sentCounter < _sentTotal) {
        auto card = _cardSuite[TOTAL_CARDS - 2 * _handcardCounter - _sentCounter % 2 - 1];
        auto receiver = _players[(_sentStart + _sentCounter) % PLAY_COUNT];
        auto isRight = isSittingRight((_sentStart + _sentCounter) % PLAY_COUNT);

        auto position1 = Vec2(card->getPosition().x, card->getPosition().y - 20);
        //auto position2 = this->convertToNodeSpace(receiver->getHandCardAnchor()->getPosition());
        auto cardAnchor = receiver->getHandCardAnchor();
        auto position2 = card->getParent()->convertToNodeSpace(receiver->getHandCardAnchor()->getParent()->convertToWorldSpace(receiver->getHandCardAnchor()->getPosition()));
        position2.x += HANDCARD_OFFSET_V88*_handcardCounter*WINSIZE.width / 900;
        auto curDeskStation = UserInfoModule()->getInstance()->findUser(receiver->getUserId())->bDeskStation;

        auto move2 = MoveTo::create(0.2f, position2);
        auto rotate1 = RotateTo::create(0.0f, 0);
		auto rotate2 = RotateTo::create(cardAnchor->getRotation(), 1.0f);
        auto secal = ScaleTo::create(0.2f, cardAnchor->getScale());

        //auto spawn1=Spawn::create(move1, rotate1, nullptr);
		auto spawn2 = Spawn::create(move2, secal, rotate2,nullptr);

        card->runAction(Sequence::create(
                            //spawn1,
                            rotate1,
                            spawn2,
        CallFunc::create([=]() {
            card->setVisible(false);
            card->setRotation(0);

            HNAudioEngine::getInstance()->playEffect(STUDAUDIO_BOARD_CARD);
            if (isRight) {
                receiver->showHandcardLeft(_handcardCounter + 1);
            }

            HNLOG("_handcardCounter: %d, _sentCounter: %d", _handcardCounter, _sentCounter);
            receiver->setHandCardVisible(_handcardCounter, true);

            _sentCounter++;
            _handcardCounter += (_sentCounter + 1) % 2;

            if (_sentCounter >= _sentTotal) {
                if (!_sendCardQueue.empty())
                    _sendCardQueue.pop_front();

                if (!_sendCardQueue.empty()) {
                    _sendingCard = true;
                    _sentStart = _sendCardQueue.front()->bStartPos;
                    _sentTotal = PLAY_COUNT;
                    _sentCounter = 0;
                } else {
                    _sendingCard = false;
                }
            }
            scheduleOnce(schedule_selector(GameTableUIV88::dealcards), 0);

        }),
        nullptr));
    }
}

void GameTableUIV88::HidTopPlayerOneCard() {
    for (int i = 0; i < PLAY_COUNT; i++) {
        if (_players[i]->getDirecTion() == PlayerUIV88::TOP) {
            _players[i]->setHandCard(0, 0x00, true);
        }
    }
}

void GameTableUIV88::updateHandcard(BYTE handcardCount, BYTE handcards[PLAY_COUNT][HANDCARD_COUNT]) {
    _handcardCounter = handcardCount;

    for (int i = 0; i < HANDCARD_COUNT; i++) {
        for (int seat = 0; seat < PLAY_COUNT; seat++) {
            if (i != 0 && handcards[seat][i] == 0x00)
                _players[seat]->setHandCard(i, handcards[seat][i], false);
            else
                _players[seat]->setHandCard(i, handcards[seat][i], i < handcardCount);
        }
    }

    for (int seat = 0; seat < PLAY_COUNT; seat++) {
        _players[seat]->showHandcardLeft(_handcardCounter);
        if (isSittingRight(seat)) {
        }
    }

}

vector<vector<Vec2>> GameTableUIV88::getCardPosVector() {
    vector<vector<Vec2>> vPos;
    _vecPockerCards.resize(2);
    for (int i = 0; i < 2; i++) {
        _vecPockerCards.at(i).resize(5);
    }
    float fMyCardInstance = 45;
    float fOtherCardInstance = 25;

    float fMyCardStartX = WINSIZE.width / 2 - (4 * fMyCardInstance + 103) / 2 + 103 / 2;
    float fOtherCardStartX = WINSIZE.width / 2 - (4 * fOtherCardInstance + 65) / 2 + 65 / 2;

    for (int i = 0; i < 2; i++) {
        vector<Vec2> v;
        v.clear();
        for (int j = 0; j < 5; j++) {
            if (i == 0) {
                v.push_back(Vec2(fMyCardStartX + fMyCardInstance * j, 140));
                auto card = PokerCardV88::create(0x00);
                card->setVisible(false);
                card->setScale(1.3);
                card->setPosition(Vec2(fMyCardStartX + fMyCardInstance * j, 140));
                this->addChild(card,100);
                _vecPockerCards.at(i).push_back(card);
            } else {
                v.push_back(Vec2(fOtherCardStartX + fOtherCardInstance * j, WINSIZE.height - 135));
                auto card = PokerCardV88::create(0x00);
                card->setVisible(false);
                card->setScale(1.1);
                card->setPosition(Vec2(fMyCardStartX + fMyCardInstance * j, 140));
                this->addChild(card, 100);
                _vecPockerCards.at(i).push_back(card);
            }
        }
        vPos.push_back(v);
    }
    return vPos;
}
// hide>=0: number of cards will not show(total 52)
// hide <0: all cards will be recycled & not show
void GameTableUIV88::shuffleCards(int hide) {
    if (hide >= 0 && hide < HANDCARD_COUNT) {
        ////////////////////////////////////////////////////////////////
        // shuffle cards

        // radius
        auto winSize = Director::getInstance()->getWinSize();

        float radius = 350;

        // Total angle
        float alpha = 30;
        float y_adjustment = 350;

        float beta = (180 - alpha) / 2;

        // angle value TO radians value
        auto r_alpha = alpha * M_PI / 180;
        auto r_beta = beta * M_PI / 180;
        auto r_alphaSplit = r_alpha / (TOTAL_CARDS - 1);

        int delaycounter = 0;
        auto dealerPosition = _tableCanvas.dealer->getPosition();

        for (int i = 0; i < TOTAL_CARDS; i++, delaycounter++) {
            auto x_offset = radius*cos(r_alphaSplit*i + r_beta);

            auto y_offset = (-1)*radius*sin(r_alphaSplit*i + r_beta) + y_adjustment;
            auto rotateAngle = (r_alphaSplit*i + r_beta) * 180 / M_PI - 90;

            // init the card if not exist
            if (_cardSuite[i] == nullptr) {
                _cardSuite[i] = PokerCardV88::create(0x00);
                _tableCanvas.dealer->getParent()->addChild(_cardSuite[i], i + 10);
                //_cardSuite[i]->setScale(0.5f*winSize.width / 800, 0.5f*winSize.height / 480);
                _cardSuite[i]->setScale(0.3);
                _cardSuite[i]->setVisible(false);
            } else {
                _cardSuite[i]->setScale(0.3);
            }
            _cardSuite[i]->setPosition(Vec2(dealerPosition.x, dealerPosition.y));
            //_cardSuite[i]->setPosition(Vec2(dealerPosition.x + x_offset, dealerPosition.y + y_offset));
            _cardSuite[i]->runAction(Sequence::create(
                                         //RotateBy::create(0.01f, rotateAngle),
                                         DelayTime::create(0.01f*delaycounter),
            CallFunc::create([=]() {
                _cardSuite[i]->setVisible(i < (TOTAL_CARDS - hide * 2));
            })
            , nullptr));
        }
    } else {
        ////////////////////////////////////////////////////////////////
        // recycle cards

        auto delaycounter = 0;
        for (int i = TOTAL_CARDS - 1; i >= 0; i--) {
            // init the card if not exist
            if (_cardSuite[i] == nullptr) {
                _cardSuite[i] = PokerCardV88::create(0x00);
                _tableCanvas.dealer->getParent()->addChild(_cardSuite[i], i);
                _cardSuite[i]->setScale(0.3);
                _cardSuite[i]->setVisible(false);
            }

            if (_cardSuite[i]->isVisible() == true) {
                _cardSuite[i]->runAction(Sequence::create(
                                             DelayTime::create(0.005f*delaycounter++),
                CallFunc::create([=]() {
                    _cardSuite[i]->setVisible(false);
                    _cardSuite[i]->setRotation(0);
                }),
                nullptr));
            } else {
                _cardSuite[i]->setRotation(0);
            }

        }
    }
}

//恢复庄玩家信息
void GameTableUIV88::recoveryUserDataCallBack(BYTE seatId,bool isVisible) {
    _players[seatId]->recoveryData(isVisible);
}

void GameTableUIV88::showMaxCardMark(bool visible, BYTE buttonSeat) {
    for (int i = 0; i < PLAY_COUNT; i++) {
        _players[i]->setButton((i == buttonSeat ? true : false) & visible);
    }
}

//void GameTableUIV88::setStartSeat(BYTE startSeat)
//{
//	_tableCanvas.startSeat = startSeat;
//}

// dashboard
void GameTableUIV88::setBetBtnText(LLONG baseNote) {
    _baseNote = baseNote;

    LLONG bet[3] = { 0 };
    bet[0] = (LLONG)(baseNote*1.0);
    bet[1] = (LLONG)(baseNote*2.0);
    bet[2] = (LLONG)(baseNote*4.0);
    if (bet[0] <= 0) {
        bet[0] = 1;
        bet[1] = 2;
        bet[2] = 4;
    }

    char title[128] = { 0 };

    for (int i = 0; i < 3; i++) {
        if (bet[i] < 10000) {//千
            memset(title, 0, sizeof(title));
            sprintf(title, GetText("QIAN"), bet[i] / 1000);
            _dashboardCanvas.bet[i]->setTitleText(title);
        }
        else {//万
            memset(title, 0, sizeof(title));
            sprintf(title, GetText("WANG"), bet[i] / 10000);
            _dashboardCanvas.bet[i]->setTitleText(title);
        }
    }
}

void GameTableUIV88::setBetAndOtherBtnOpen(bool isEnable) {
    auto bg = static_cast<Sprite*>(dashboard_panel->getChildByName("note_bg"));
    if (bg == nullptr)
        return;
    auto fold_btn = static_cast<Button*>(dashboard_panel->getChildByName("fold_btn"));
    auto showha_btn = static_cast<Button*>(dashboard_panel->getChildByName("showha_btn"));
    auto call_btn = static_cast<Button*>(dashboard_panel->getChildByName("call_btn"));
    auto jiazhu_btn = static_cast<Button*>(dashboard_panel->getChildByName("jiazhu_btn"));

    if (fold_btn == nullptr || showha_btn == nullptr || call_btn == nullptr || jiazhu_btn == nullptr) {
        return;
    }

    fold_btn->setVisible(isEnable);
    showha_btn->setVisible(isEnable);
    jiazhu_btn->setVisible(isEnable);
    call_btn->setVisible(isEnable);
    //_dashboardCanvas.bet[0]->setVisible(isEnable);
    //_dashboardCanvas.bet[1]->setVisible(isEnable);
    //_dashboardCanvas.bet[2]->setVisible(isEnable);
    //	_tableCanvas.fapaiqi->setVisible(isEnable);
}

void GameTableUIV88::setMesaVisible(bool visible) {
    // _tableCanvas.pot->setVisible(visible);
    _tableCanvas.limit->setVisible(visible);
    _tableCanvas.base->setVisible(visible);
    //  _tableCanvas.totalMoney->setVisible(visible);
    _tableCanvas.topTotal->setVisible(visible);
    _tableCanvas.topTotalBg->setVisible(visible);
    _tableCanvas.bottomTotal->setVisible(visible);
    _tableCanvas.bottomTotalBg->setVisible(visible);
}

void GameTableUIV88::setBetBtnEnabled(bool isEnable) {
    setBetBtnEnabled(isEnable, isEnable, isEnable, isEnable);
}

// Dashboard
void GameTableUIV88::setBetBtnEnabled(bool follow, bool allin, bool note, bool quit) {
    auto bg = static_cast<Sprite*>(dashboard_panel->getChildByName("note_bg"));
    if (bg == nullptr)
        return;
    auto fold_btn = static_cast<AUIButton*>(dashboard_panel->getChildByName("fold_btn"));
    auto showha_btn = static_cast<AUIButton*>(dashboard_panel->getChildByName("showha_btn"));
    auto call_btn = static_cast<AUIButton*>(dashboard_panel->getChildByName("call_btn"));
    auto jiazhu_btn = static_cast<AUIButton*>(dashboard_panel->getChildByName("jiazhu_btn"));

    if (fold_btn == nullptr || showha_btn == nullptr || call_btn == nullptr || jiazhu_btn == nullptr) {
        return;
    }

    call_btn->setBright(follow);
    showha_btn->setBright(allin);
    fold_btn->setBright(quit);
    jiazhu_btn->setBright(note);

    call_btn->setEnabled(follow);
    showha_btn->setEnabled(allin);
    fold_btn->setEnabled(quit);
    bg->setVisible(note);
    _dashboardCanvas.bet[0]->setBright(note);
    _dashboardCanvas.bet[1]->setBright(note);
    _dashboardCanvas.bet[2]->setBright(note);
}

// dashboard menu
void GameTableUIV88::menuClickCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType touchtype) {
    if (touchtype == cocos2d::ui::Widget::TouchEventType::ENDED) {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        (false);
        auto btn = static_cast<Button*>(pSender);
        auto name = btn->getName();
        if (pSender == _dashboardCanvas.bet[0]) {
            // 1* base note
            _tableLogic->sendNote(1 * _baseNote);
        } else if (pSender == _dashboardCanvas.bet[1]) {
            // 2* base note
            _tableLogic->sendNote(2 * _baseNote);
        } else if (pSender == _dashboardCanvas.bet[2]) {
            // 4* base note
            _tableLogic->sendNote(4 * _baseNote);
        } else if (name == "jiazhu_btn") {
            auto noteBg = dynamic_cast<Sprite*>(dashboard_panel->getChildByName("note_bg"));
            noteBg->setVisible(!noteBg->isVisible());
            return;
        } else if (name == "showha_btn") {
            _tableLogic->sendShuttle();
        } else if (name == "call_btn") {
            _tableLogic->sendFollow();
        } else if (name == "fold_btn") {
            _tableLogic->sendGiveUp();
        }
        setBetBtnEnabled(false);
    }
}

void GameTableUIV88::setStartBtnVisible(bool visible) {
    _dashboardCanvas.start->setVisible(visible);
}
//
void GameTableUIV88::setAreadySpriteVisible(bool visible,BYTE bDeskStation) {
    _vecAreadySprite.at(_tableLogic->logicToViewSeatNo(bDeskStation))->setVisible(visible);
}

void GameTableUIV88::setAllAreadySpriteVisible(bool visible) {
    for (auto spr : _vecAreadySprite) {
        spr->setVisible(false);
    }
}

//更新历史输赢和单笔输赢
void GameTableUIV88::upDataHistoyAndLastMoney(LLONG history, LLONG last) {
    _tableCanvas.base->setString(StringUtils::format("%lld", last));
    _tableCanvas.limit->setString(StringUtils::format("%lld", history));
}

void GameTableUIV88::onGameNotify(NN::tag_s2c_notify* msg) {
    if (msg->i64WinMoney < 0) return;
    char str[128] = { 0 };

    if (msg->bDeskID == INVALID_BYTE) {
        //广播
        if (msg->bGameID == 1) {
            sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
        } else if (msg->bGameID == 2) {
            sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
        } else if (msg->bGameID == 3) {
            sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
        } else {
            return;
        }
        showNotice(str, 1, 1);
    }
}

bool GameTableUIV88::isSittingRight(BYTE seat) {
    if (seat<0 || seat>PLAY_COUNT - 1) {
        return false;
    }

    // only 2 seats total
    auto anotherSeat = PLAY_COUNT - seat - 1;

    return (_tableCanvas.seats[seat]->getPosition().x > _tableCanvas.seats[anotherSeat]->getPosition().x);
}

void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
    if (bVisible) {
        if (!m_Notice) {
            m_Notice = GameNotice::create();
            m_Notice->setLocalZOrder(200);
            m_Notice->setName("GameNotice");
        }
        m_Notice->openNotic(this, 10000, message, nCount, nType);
    }
}

void GameTableUIV88::throwChip(BYTE seat, LLONG money) {
    if (seat<0 || seat>PLAY_COUNT - 1) {
        return;
    }

    while (money > 0) {
        while (money > 2 * _baseNote) {
            while (money > 4 * _baseNote) {
                while (money > 8 * _baseNote) {
                    throwChipBase(seat, 8 * _baseNote);
                    money -= 8 * _baseNote;
                }
                if (money > 4 * _baseNote) {
                    throwChipBase(seat, 4 * _baseNote);
                    money -= 4 * _baseNote;
                }
            }
            if (money > 2 * _baseNote) {
                throwChipBase(seat, 2 * _baseNote);
                money -= 2 * _baseNote;
            }
        }
        if (money > 0) {
            throwChipBase(seat, _baseNote);
            money -= _baseNote;
        }
    }
}

void GameTableUIV88::throwChipBase(BYTE seat, LLONG money) {
    if (seat<0 || seat>PLAY_COUNT - 1) {
        return;
    }

    if (_chipPool == nullptr) {
        _chipPool = Layer::create();
//        auto table = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableCanvas.outline, "table_img"));
        this->addChild(_chipPool, _players[0]->getLocalZOrder() + 1);
    }

    auto chip = getChip(money, _baseNote);
	chip->setScale(0.45f);
    auto position1 = _chipPool->convertToNodeSpace(_players[seat]->getParent()->convertToWorldSpace(_players[seat]->getPosition()));
    chip->setPosition(position1);
    _chipPool->addChild(chip);

    auto winSize = Director::getInstance()->getWinSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto chipSize = chip->getContentSize();
    auto width = _tableCanvas.betArea->getContentSize().width - chipSize.width;
    auto height = _tableCanvas.betArea->getContentSize().height - chipSize.height;

    auto rect = _tableCanvas.betArea->getBoundingBox();
    rect.origin = _tableCanvas.betArea->getParent()->convertToWorldSpace(rect.origin);
    rect.origin.x += chipSize.width / 2;
    rect.origin.y += chipSize.width / 2;
    rect.size.width -= chipSize.width;
    rect.size.height -= chipSize.height;

    Vec2 position2;
    bool flag;
    do {
        position2 = rect.origin;
        auto xOffset = fmod(rand()*(rand() % 2 ? -1 : 1), width);
        auto yOffset = fmod(rand()*(rand() % 2 ? -1 : 1), height);

        position2.x += xOffset;
        position2.y += yOffset;

        flag = rect.containsPoint(position2);
    } while (!flag);

    chip->runAction(MoveTo::create(0.3f, Vec2(position2.x , position2.y )));
    HNAudioEngine::getInstance()->playEffect(STUDAUDIO_CHIP);
}

// return chip01: money = 1* base
// return chip02: money = 2* base
// return chip03: money = 4* base
// return chip04: money = 8* base
Sprite* GameTableUIV88::getChip(LLONG money, LLONG base) {
    if (money >= 8 * base)
        return Sprite::create("v66/chouma/10.png");
    else if (money >= 4 * base)
        return Sprite::create("v66/chouma/9.png");
    else if (money >= 2 * base)
        return Sprite::create("v66/chouma/8.png");
    else
        return Sprite::create("v66/chouma/7.png");
}

void GameTableUIV88::recycleChipsById(LLONG userId) {
    for (BYTE seat = 0; seat < PLAY_COUNT; seat++) {
        if (nullptr == _players[seat]) continue;
        if (_players[seat]->getUserId() == userId) {
            recycleChips(seat);
            break;
        }
    }
}

void GameTableUIV88::recycleChips(BYTE seat) {
    if (seat<0 || seat>PLAY_COUNT - 1 || _chipPool == nullptr) {
        return;
    }

    auto winnerpos = _chipPool->convertToNodeSpace(_players[seat]->getParent()->convertToWorldSpace(_players[seat]->getPosition()));

    for (auto chip : _chipPool->getChildren()) {
        chip->runAction(Sequence::create(
                            MoveTo::create(0.5f, winnerpos),
                            RemoveSelf::create(true),
                            nullptr));
    }
    HNAudioEngine::getInstance()->playEffect(STUDAUDIO_PRIZE);
    return;
}

////年兽触发
//void GameTableUIV88::onGameShowNianShou() {
//    //播动画
//    showNianShouAnimation();
//}
//
////赶年兽结果
//void GameTableUIV88::onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) {
//    //过滤只有自己看的到
//    if (nUserID != _tableLogic->getUserId(_tableLogic->getMySeatNo())) {
//        return;
//    }
//
//    removeNianShouButton();
//
//    Vec2 pos = NewYearActivityInGame::NianShouButtonPosition;
//
//    //有鞭炮赶年兽奖励
//    if (i64RewardMoney > 0) {
//        NewYearActivity::NSAnimationTool::playTNTAudio(i64RewardMoney);
//        auto daijiNode = NewYearActivity::NSAnimationTool::createNSGanNodeAni("hit", false);
//        if (daijiNode) {
//            daijiNode->setPosition(Vec2(pos));
//            this->addChild(daijiNode, NewYearActivityInGame::NianShouZOrder);
//        }
//
//        NewYearActivity::NSAnimationTool::createRewardLabel(i64RewardMoney, this, Vec2(pos.x, pos.y + 50));
//    } else { //无鞭炮
//        NewYearActivity::NSAnimationTool::createTipOfNoProp(this, pos);
//    }
//}
//
////对局获得鞭炮
//void GameTableUIV88::onGameMatchGainFirecraker(int nUserID) {
//    //该id获得一个中型鞭炮
//    //只有自己显示
//    if (_tableLogic->getUserId(_tableLogic->getMySeatNo()) == nUserID) {
//        ImageView* pTipImage = ImageView::create("common/gainFirecrakerTip.png");
//        pTipImage->setScale(0.1);
//        pTipImage->setPosition(Vec2(250, 140));
//        addChild(pTipImage, NewYearActivityInGame::NianShouZOrder);
//
//        Spawn* spawn = Spawn::create(ScaleTo::create(0.5f, 1.0f), FadeIn::create(0.5f), nullptr);
//        Sequence* seq = Sequence::create(spawn, DelayTime::create(2.0f), RemoveSelf::create(), nullptr);
//        pTipImage->runAction(seq);
//    }
//}
//
//void GameTableUIV88::showNianShouAnimation() {
//    if (this->getChildByName("DriveNianShouBtn")) {
//        return;
//    }
//    auto btn = NewYearActivity::NSAnimationTool::createNSButton("DriveNianShouBtn");
//    if (btn) {
//        btn->setPosition(NewYearActivityInGame::NianShouButtonPosition);
//        btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::driveNianShouClickCallback, this));
//        addChild(btn, NewYearActivityInGame::NianShouZOrder);
//
//        auto daijiNode = NewYearActivity::NSAnimationTool::createNSDaijiNodeAni("daiji", true);
//        if (daijiNode) {
//            daijiNode->setPosition(Vec2(btn->getContentSize().width*0.5f, btn->getContentSize().height*0.5f));
//            btn->addChild(daijiNode);
//        }
//        scheduleOnce(schedule_selector(GameTableUIV88::autoRemoveNianShouButton), 5.0f);
//    }
//}
//
//void GameTableUIV88::driveNianShouClickCallback(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED)
//        return;
//
////        unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
//
//    Button* btn = (Button*)pSender;
//    btn->setTouchEnabled(false);
//
//    //发包
//    int nUserID = _tableLogic->getUserId(_tableLogic->getMySeatNo());
//    _tableLogic->sendDriveNianShou(nUserID);
//}
//
//void GameTableUIV88::removeNianShouButton() {
//    auto btn = this->getChildByName("DriveNianShouBtn");
//    if (btn) {
//        btn->stopAllActions();
//        btn->removeFromParentAndCleanup(true);
//    }
//}
//
//void GameTableUIV88::autoRemoveNianShouButton(float dt) {
//    removeNianShouButton();
//}
    
    void GameTableUIV88::onLuckyMoneyNotify(LuckyMoneyNotify *msg)
    {
        if (nullptr == msg)
        {
            return;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto notify = CSLoader::createNode("Reward/ui.csb");
        notify->setName("RewardNotice");
        auto btnOpenReward = dynamic_cast<Button*>(notify->getChildByName("btnOpen"));
        btnOpenReward->addTouchEventListener(std::bind([&](Ref* psender, Widget::TouchEventType type){
            if (Widget::TouchEventType::ENDED != type)
            {
                return;
            }
            
            RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_OPEN_LUCKYMONEY, 0, 0);
            this->removeChildByName("RewardNotice");
        }, std::placeholders::_1, std::placeholders::_2));
        
        addChild(notify, 200);
        notify->setPosition(Vec2(visibleSize)/2);
        
        runAction(Sequence::create(DelayTime::create(msg->nStayTime), CallFunc::create([&](){
            this->removeChildByName("RewardNotice");
        }), NULL));
    }
    
    void GameTableUIV88::onLuckyMoneyResult(LuckyMoneyResult *msg)
    {
        if (nullptr == msg)
        {
            return;
        }
        
        if (msg->nType != 1)
        {
            GamePromptLayer::create()->showPrompt("手慢无！下次快点哦。");
            return;
        }
        
        auto result = CSLoader::createNode("Reward/reward.csb");
        auto action = CSLoader::createTimeline("Reward/reward.csb");
        if (nullptr != result && nullptr != action)
        {
            addChild(result, 200);
            auto visibleSize = Director::getInstance()->getVisibleSize();
            result->setPosition(Vec2(visibleSize)/2);
            result->setName("rewardResult");
            result->getChildByName("show")->runAction(action);
            action->gotoFrameAndPlay(0, false);
            
            auto txtReward = dynamic_cast<Text*>(result->getChildByName("txtReward"));
            txtReward->setString("");
            
            auto luckyMoney = msg->i64LuckyMoney;
            auto retainTime = msg->nShowTime;
            runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                txtReward->setString(Tools::addComma(luckyMoney));
            }), DelayTime::create(retainTime), CallFunc::create([&](){
                this->removeChildByName("rewardResult");
            }), NULL));
        }
    }
}
