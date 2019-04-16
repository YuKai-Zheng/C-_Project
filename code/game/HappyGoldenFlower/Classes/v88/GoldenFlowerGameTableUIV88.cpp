#include "GoldenFlowerGameTableUIV88.h"
#include "GoldenFlowerGameTableLogic.h"
//#include "GoldenFlowerGameNotice.h"
#include "GoldenFlowerGameDashboardV88.h"
//#include "GoldenFlowerGameUserMessageBox.h"
#include "GoldenFlowerResourceMgr.h"
//#include "../../TTNN/Classes/v88/BRNNSetLayerV88.h"
//#include "../../TTNN/Classes/GameTable/BRNNActions.h"
#include "GoldenFlowerResourceMgr.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "HNLobbyExport.h"
#include "HNLobby/v66/ExtraReward.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/v66/PlatformResV66.h"

//#include "NSAnimationTool.h"
#include "Sound/HNAudioEngine.h"
#include "Tool/Tools.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "UI/Scanner.h"
//using namespace NewYearActivity;

namespace goldenflower {

static const long long BetConstMoney[5] = {100, 1000, 10000, 100000, 1000000};
static const int playerHeadOlder[6] = { 100, 90, 70, 60, 50,80};

static const int	Dashboard_Zorder	=	10;
//static const int	TableInfo_Zorder	=   10;
static const int	EndBox_Zorder       =   12;
//static const int	Max_Zorder			=   100;

//static const int	MAX_ZORDER			=   100; //最大层??
static const int	MEMBAN_ZORDER		=	300;

static const char* JSON_NODE_POSITION	= "Point8";
static const char* JSON_NODE_DEALER     = "Dealer";

//static const char* TOOL_BAR_PATH = "v66/common/tool/%s";


//static const char* GAME_SOUND_BTN = "music/btnEffect/button_click_sound.mp3";




//static const char* PREPARE_WORD								= "v66/button/green/12.png";
//static const char* PREPARE_BTN								= "v66/button/green/bg.png";

static const char* ANIMATION_VS_CSB			= "v88/animation/common/bipai/bipai_2ren.csb";
static const char* ANIMATION_VS_HIT_CSB		= "v88/animation/common/bipai/bipai_2ren_wenzi.csb";

static const char* NODE_TOTAL_BET_CSB       = "Games/12101106/Node_Bet_Total_V66.csb";

static const int VS_NODE_ZORDER = 200;
static const char* END_LOST_BG = "v66/common/jiesuan/loseBg.png";
static const char* END_WIN_BG = "v66/common/jiesuan/winBg.png";
static const char* HIT_MUSIC_EFFECT_PATH = "v66/musics/hit_effect.wav";

//    static const char* AUDIO_BG_MUSIC_PATH = "music/NewYearActivity/NewYearBG.mp3";

HNGameUIBase* GameTableUIV88::create(BYTE bDeskIndex, bool bAutoCreate) {
    GameTableUIV88* tableUI = new GameTableUIV88();
    if (tableUI->init(bDeskIndex, bAutoCreate)) {
        tableUI->autorelease();
        return tableUI;
    } else {
        CC_SAFE_DELETE(tableUI);
        return nullptr;
    }
}

bool GameTableUIV88::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }

    m_DeskIndex = bDeskIndex;
    m_AutoCreate = bAutoCreate;

    return true;
}


void GameTableUIV88::onEnterTransitionDidFinish() {
    HNGameUIBase::onEnterTransitionDidFinish();
    //        HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_MUSIC_PATH, true);
}

void GameTableUIV88::onEnter() {
    HNGameUIBase::onEnter();

    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("v66/common/card_AMax/cards.plist");

    _remainTime = 0;
    Size size = WIN_SIZE;
    _visibleWidth = size.width;
    _visibleHeight = size.height;
    this->setIgnoreAnchorPointForPosition(false);
    this->setPosition(size.width / 2, size.height / 2);


    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    initPosition();
    initGameData();
    inflateLayout();
    initVSAnimation();
    initHitAnimation();

    //荷官处理??
    _dealer = GameDelearUIV88::create();
    _dealer->setPosition(0, 0);
    addChild(_dealer, VS_NODE_ZORDER - 10);

    std::vector<Vec2> seatPos(PLAY_COUNT);
    std::vector<Vec2> BetFlyToPos(PLAY_COUNT);
    std::vector<Vec2> tablePos(PLAY_COUNT);
    for (int i = 0; i < PLAY_COUNT; i++) {
        auto bet = _tableUI.Nbet[i]->getPosition();
        seatPos[i] = _nodeCardPosition[i];
        BetFlyToPos[i] = _tableUI.lOutline->convertToWorldSpace(bet);
        tablePos[i] = _tableUI.lOutline->convertToWorldSpace(_jettonPosition[i]);
    }

	_colorLayer = LayerColor::create(Color4B(0, 0, 0, 160));
	if (_colorLayer) {
		_colorLayer->setContentSize(Size(300, 26));
		this->addChild(_colorLayer, 1000);
		_colorLayer->setPosition(WIN_SIZE.width / 2 - 150, WIN_SIZE.height / 2);
		auto _nameLabel = Label::createWithTTF(GBKToUtf8("您已下注啦，开奖后再离开哦!"), FONT_TTF_MSYH, FONT_SIZE_NUM_18);
		_nameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		_nameLabel->setPosition(_colorLayer->getContentSize().width / 2, 12);
		_colorLayer->addChild(_nameLabel);
		_colorLayer->setVisible(false);
	}

    _dealer->setTablePoint(tablePos);
    _dealer->setSeatPoint(seatPos);
    _dealer->setBetFlyToPoint(BetFlyToPos);
    _dealer->setDealerPoint(Vec2(_visibleWidth * 0.5f, _visibleHeight * 0.55f));

    //牌桌逻辑
    _tableLogic = new GameTableLogic(this, m_DeskIndex, m_AutoCreate);
    _tableLogic->clearDesk();
    _tableLogic->loadUsers();

    createToolBarUI();
}

void GameTableUIV88::onExit() {
//        HNAudioEngine::getInstance()->stopBackgroundMusic();
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile("v66/common/card_AMax/cards.plist");
    HNGameUIBase::onExit();
}

void GameTableUIV88::createToolBarUI() {
    int zOrder = MEMBAN_ZORDER;
    auto toolbar = ToolBarLayer::create();
    if (toolbar) {
        toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
            if (RoomLogic()->connected()) {
                _tableLogic->sendStandUp();
            } else {
                leaveTheGame();
            }
        });
        toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
            IGameSet* gameSet = GameSetFactory::createWithVersion();
            gameSet->setBgPath("");
            gameSet->open(this, Vec2(0, 0), zOrder+5, 0);
        });
        toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
            auto helpLayer = HN::HelpLayer::create();
            helpLayer->setContentOnView("v66/common/help/zhajinhua.png");
            helpLayer->setName("helpLayer");
            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            helpLayer->setOrignalPosition();
            helpLayer->moveSelfInWithAction();
            addChild(helpLayer, zOrder+5);
        });
        toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
    }
}

void GameTableUIV88::initVSAnimation() {
    _vsNode = CSLoader::createNode(ANIMATION_VS_CSB);
    if (_vsNode) {
        _vsNode->setPosition(WIN_SIZE / 2);
        _vsNode->setVisible(false);
        addChild(_vsNode, VS_NODE_ZORDER);
    }

    //auto action = CSLoader::createTimeline(ANIMATION_VS_CSB);
    //vsNode->runAction(action);
    //action->play("Vs", false);
}

void GameTableUIV88::initHitAnimation() {
    _hitNode = CSLoader::createNode(ANIMATION_VS_HIT_CSB);
    _hitNode->setPosition(WIN_SIZE / 2);
    _hitNode->setVisible(false);
    addChild(_hitNode, VS_NODE_ZORDER + 40);

    //auto action = CSLoader::createTimeline(ANIMATION_VS_HIT_CSB);
    //hitNode->runAction(action);
    //action->play("hit", false);

}

//void GameTableUIV88::onToolBtnClick(Ref* pSender, Control::EventType event) {
//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//    Node* btn = (Node*)pSender;
//    switch (btn->getTag()) {
// //   case em_toolbarBtn: {
//	//						if (0 == _toolbarBg->getName().compare("0")) {
//	//							_toolbarBg->setName("1");
//	//							_toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height)));
//	//							this->getChildByTag(em_toolbarBtn)->setVisible(false);
//	//						}
//	//}
// //   break;
//    case em_toolbarSetBtn: {
//							   if (this->getChildByName("helpLayer")) {
//                                   ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
////								   this->getChildByName("helpLayer")->removeFromParent();
//							   }
//							   if (0 == _toolbarBg->getName().compare("1")) {
//								   _toolbarBg->setName("0");
//
//								   _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//									   CallFunc::create([this]() {
//									   this->getChildByTag(em_toolbarBtn)->setVisible(true);
//								   })));
//							   }
//
//        if (this->getChildByName("setLayer")) {
//            (dynamic_cast<BRNNSetLayerV88*>(this->getChildByName("setLayer")))->closeSet();
//            return;
//        }
//		auto setLayer = BRNNSetLayerV88::create();
//        setLayer->setName("setLayer");
//        setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//		setLayer->showSet(this, VS_NODE_ZORDER+50);
//		//setLayer->setGlobalZOrder(11);
//    }
//    break;
//    case  em_toolbarHelpBtn: {
//        showCardType();
//    }
//    break;
//    case em_toolbarLeaveBtn: {
//        if (RoomLogic()->connected()) {
//            _tableLogic->sendStandUp();
//        } else {
//            leaveTheGame();
//        }
//    }
//    break;
//    case em_toolbarNextOutBtn: {
//        if (btn->getUserData() == (void*)true) {
//            btn->setUserData((void*)false);
//            btn->getChildByName("checkSprite")->setVisible(false);
//            m_bNextOut = false;
//        } else if (btn->getUserData() == (void*)false) {
//            btn->setUserData((void*)true);
//            btn->getChildByName("checkSprite")->setVisible(true);
//            m_bNextOut = true;
//        }
//    }
//    break;
//    }
//}



bool GameTableUIV88::onTouchBegan(Touch *touch, Event *unused_event) {
    Vec2 touchPos = touch->getLocation();
    auto size = Director::getInstance()->getWinSize();

//    if (0 == _toolbarBg->getName().compare("1")) {
//        _toolbarBg->setName("0");
//
//        _toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//        CallFunc::create([this]() {
//            this->getChildByTag(em_toolbarBtn)->setVisible(true);
//        })));
//    }

    //    for (int i = 0; i < PLAY_COUNT; i++)
    //    {
    //        Rect rt = _tableUI.bSeat[i]->getBoundingBox();
    //        rt.origin += WIN_SIZE/2;
    //        if (rt.containsPoint(touchPos))
    //        {
    //            if (nullptr != _players[i])
    //            {
    //
    //            }
    //            return false;
    //        }
    //    }




    //对下注筹码进行操??
    if (_dashboard->_ButtonBeg->getPosition().y > _dashboard->_BetBegPosition.y) {
        _dashboard->setAddBetVisible(false);
        _dashboard->setAddVisible(true);
        return false;
    }

    //if (_tableUI.nPullDownMenu->getPosition().y < size.height - 50)
    //{
    //	Size rt = _tableUI.nPullDownMenu->getContentSize();
    //	_tableUI.nPullDownMenu->stopAllActions();
    //	_tableUI.nPullDownMenu->runAction(MoveTo::create(0.2f, Vec2(0, _visibleHeight)));

    //	return false;
    //}




    return true;
}

void GameTableUIV88::menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (Widget::TouchEventType::ENDED != touchtype) {
        return;
    }

//    bool bRet = PoolManager::getInstance()->getCurrentPool()->contains(this);

    Button* ptr = (Button*)pSender;
    std::string name = ptr->getName();
    if (name.compare("btn_start") == 0) {
        ptr->setVisible(false);
        _tableLogic->sendAgreeGame();
        IStartTimer(0);
    }
    //else if(name.compare("btn_cardtype") == 0)
    //{
    //	showCardType();
    //}
    //else if(name.compare("btn_menu") == 0)
    //{
    //	Size rt = _tableUI.nPullDownMenu->getContentSize();
    //	_tableUI.nPullDownMenu->stopAllActions();
    //	_tableUI.nPullDownMenu->runAction(MoveTo::create(0.2f, Vec2(0, _visibleHeight-rt.height*1.3+2)));
    //}
}

//void GameTableUIV88::EndLayerClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
//{
//	if (Widget::TouchEventType::ENDED != touchtype)
//	{
//		return;
//	}
//
//	for (auto TempCard : _vecCard)
//	{
//		TempCard->removeFromParent();
//	}
//	_vecCard.clear();
//
//	_endLayer->getChildByName("Image_win")->setVisible(false);
//	_endLayer->getChildByName("Image_lose")->setVisible(false);
//	//隐藏结算??
//	_endLayer->getParent()->setVisible(false);
//}

//void GameTableUIV88::seatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
//{
//	if (Widget::TouchEventType::ENDED != touchtype)
//	{
//		return;
//	}
//	return;
//	//for (int i = 0; i < PLAY_COUNT; i++)
//	//{
//	//	if (pSender == _tableUI.bSeat[i])
//	//	{
//	//		_tableLogic->sendUserSit(i);
//	//		break;
//	//	}
//	//}
//}

void GameTableUIV88::compareClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (Widget::TouchEventType::ENDED != touchtype) {
        return;
    }

    for (int i = 0; i < PLAY_COUNT; i++) {
        if (pSender == _tableUI.iCard[i]) {
            _tableLogic->sendCompare(_tableLogic->viewToLogicSeatNo(i));
            break;
        }
    }

    for (int i = 0; i < PLAY_COUNT; i++) {
        _tableUI.iCard[i]->setVisible(false);
        _tableUI.bCompare[i]->setVisible(false);
    }
}

//void GameTableUIV88::pullDownItemCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
//{
//	if (Widget::TouchEventType::ENDED != touchtype)
//	{
//		return;
//	}
//	Size winSize = Director::getInstance()->getWinSize();
//	auto pButton = (Button*)pSender;
//
//	if (pButton == _tableUI.bJumpMenu[0])			//站起子按??
//	{
//		_tableLogic->sendStandUp();
//	}
//	else if (pButton == _tableUI.bJumpMenu[1])			//设置子按??
//	{
//		pButton->setTouchEnabled(false);
//		auto box = GameSetLayer::create();
//		box->setPosition(Vec2(winSize.width, 0));
//		addChild(box, 105);
//		box->runAction(Sequence::create(EaseBackOut::create(MoveBy::create(0.2f, Vec2(-winSize.width, 0))),
//			CallFunc::create(CC_CALLBACK_0(GameTableUIV88::menuTouchReset, this, pButton)), nullptr));
//	}
//	else if (pButton == _tableUI.bJumpMenu[2])			//商城子按??
//	{
//	}
//	else if (pButton == _tableUI.bJumpMenu[3])			//商城子按??
//	{
//		showCardType();
//	}
//
//	Size rt = _tableUI.nPullDownMenu->getContentSize();
//	_tableUI.nPullDownMenu->stopAllActions();
//	_tableUI.nPullDownMenu->runAction(MoveTo::create(0.2f, Vec2(0, _visibleHeight-rt.height*1.3+2)));
//}

////按钮点击功能复位
//void GameTableUIV88::menuTouchReset(Button* btn)
//{
//	btn->setTouchEnabled(true);
//}

void GameTableUIV88::compareCardReq() {
    _tableLogic->compareCardReq();
}

void GameTableUIV88::lookCard() {
    _tableLogic->sendLook();
}

void GameTableUIV88::compareCard() {
    //_tableLogic->sendCompare();
}

void GameTableUIV88::followNote() {
    _tableLogic->sendFollow();
}

void GameTableUIV88::giveUpNote() {
    _tableLogic->sendGiveUp();
}

//加注
void GameTableUIV88::addNote(LLONG money) {
    //_tableLogic->sendAdd(money);
    _dashboard->setAddVisible(false);
    _dashboard->setAddBetVisible(true);
}

//选择下注筹码
void GameTableUIV88::addBet(int multiple) {
    _dashboard->setAddBetVisible(false);
    _tableLogic->sendAddBet(multiple);
}

void GameTableUIV88::initGameData() {
    //memset(_pendData, 0, sizeof(_pendData));
    memset(_players, 0, sizeof(_players));
    memset(_viewSeat, INVALID_SEAT_NO, sizeof(_viewSeat));
    _viewOfWiner = INVALID_SEAT_NO;
}

void GameTableUIV88::initPosition() {
    std::string filename = "goldenflower/position.json";
    if (!FileUtils::getInstance()->isFileExist(filename)) {
        return;
    }
    std::string json = FileUtils::getInstance()->getStringFromFile(filename);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
    if (doc.HasParseError()) {
        return;
    }
    if (doc.IsObject() && doc.HasMember("Position")) {
        const rapidjson::Value &val = doc["Position"];
        if (val.IsArray()) {
            for (rapidjson::SizeType i = 0; i < val.Size(); ++i) {
                const rapidjson::Value &node = val[i];

                //读取玩家下注筹码的位??
                if (node.HasMember(JSON_NODE_POSITION)) {
                    const rapidjson::Value &nd = node[JSON_NODE_POSITION];
                    if (nd.IsArray()) {
                        for (rapidjson::SizeType j = 0; j < nd.Size(); ++j) {
                            const rapidjson::Value &vitem = nd[j];
                            const rapidjson::Value & vx = vitem["x"];
                            const rapidjson::Value & vy = vitem["y"];
                            _jettonPosition[j].x = vx.GetDouble();
                            _jettonPosition[j].y = vy.GetDouble();
                        }
                    }
                    continue;
                }

                //读取荷官位子
                if (node.HasMember(JSON_NODE_DEALER)) {
                    const rapidjson::Value &nd = node[JSON_NODE_DEALER];
                    const rapidjson::Value &ndx = nd["x"];
                    const rapidjson::Value &ndy = nd["y"];
                    _dealerPostion.x = ndx.GetDouble();
                    _dealerPostion.y = ndy.GetDouble();
                }
            }
        }
    }

}

int GameTableUIV88::getPlayerCount() {
    int count = 0;
    for (BYTE i = 0; i < PLAY_COUNT; ++i) {
        if (nullptr == _players[i]) {
            continue;
        }
        count++;
    }
    return count;
}


GameTableUIV88::GameTableUIV88()
    : _dashboard(nullptr)
      //, _endNodeBg(nullptr)
    , _winNode(nullptr)
    , _loseNode(nullptr)
    , _winAction(nullptr)
    , _loseAction(nullptr)
    , _endNode(nullptr)
    , _toolbarBg(nullptr)
    , m_bNextOut(false)
    , _vsNode(nullptr)
    , _hitNode(nullptr)
    , m_Notice(nullptr)
//    , m_leaveTimeOut(false)
{
    memset(_players, 0, sizeof(_players));

    _playerReadyState.clear();
    for (int i = 0; i < PLAY_COUNT; i++) {
        _playerReadyState.push_back(false);
    }
    _canShowReadySign = false;
}

GameTableUIV88::~GameTableUIV88() {
//    this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
    HN_SAFE_DELETE(_tableLogic);
}

PlayerUIV88* GameTableUIV88::getPlayer(int seatNo) {
    return (seatNoIsOk(seatNo) ? _players[seatNo] : nullptr);
}

PlayerUIV88* GameTableUIV88::getPlayer(LLONG userId) {
    for(int i = 0; i< PLAY_COUNT; ++i) {
        if(nullptr != _players[i]) {
            if(userId == _players[i]->getUserID()) {
                return _players[i];
            }
        }
    }

    return nullptr;
}

//void GameTableUIV88::showCardType() {
//
//	if (this->getChildByName("setLayer")) {
//
//		this->getChildByName("setLayer")->removeFromParent();
//	}
//	if (0 == _toolbarBg->getName().compare("1")) {
//		_toolbarBg->setName("0");
//
//		_toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//			CallFunc::create([this]() {
//			this->getChildByTag(em_toolbarBtn)->setVisible(true);
//		})));
//	}
//
//    auto layer = this->getChildByName("helpLayer");
//    if(layer != nullptr) {
//        return;
//    }
//
//
////    auto cardTypeLayer = GameCardTypeV88::create();
////    auto size = cardTypeLayer->getContentSize();
////    cardTypeLayer->setVisible(true);
////    cardTypeLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
////    cardTypeLayer->ignoreAnchorPointForPosition(false);
////    cardTypeLayer->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height /2));
////	this->addChild(cardTypeLayer, VS_NODE_ZORDER +50);
////	//cardTypeLayer->setGlobalZOrder(11);
////    cardTypeLayer->setName("card_type_layer");
////	BRNN::Actions::actionIn(cardTypeLayer);
//    
//    auto helpLayer = HN::HelpLayer::create();
//    helpLayer->setContentOnView("v66/common/help/zhajinhua.png");
//    helpLayer->setName("helpLayer");
//    helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//    helpLayer->setOrignalPosition();
//    helpLayer->moveSelfInWithAction();
//    addChild(helpLayer, VS_NODE_ZORDER + 1000000);
//
//}

/**************************************************************/
void GameTableUIV88::showHandCard(std::vector<THandCard>& cards) {
    _canShowReadySign = false;//开始发牌后就屏蔽准备状态

	auto scanner = Scanner::create();
	this->addChild(scanner, 100000);
	scanner->starPlayerEffect();
	scanner->setPosition(250, 60);

    // all displayed users enter the game
    for(int i = 0; i< PLAY_COUNT; ++i) {
        if(nullptr != _players[i]) {
            BYTE viewNo = _tableLogic->logicToViewSeatNo(i);
            if (seatNoIsOk(viewNo)) {
                _tableUI.ReadySign[viewNo]->setVisible(false);
            }
            _players[i]->enterGame();
        }
    }


	//std::vector<Vec2> seatPos(PLAY_COUNT);
	//std::vector<Vec2> BetFlyToPos(PLAY_COUNT);
	//std::vector<Vec2> tablePos(PLAY_COUNT);
	//for (int i = 0; i < PLAY_COUNT; i++) {
	//	auto bet = _tableUI.Nbet[i]->getPosition();
	//	seatPos[i] = _nodeCardPosition[i];
	//	BetFlyToPos[i] = _tableUI.lOutline->convertToWorldSpace(bet);
	//	tablePos[i] = _tableUI.lOutline->convertToWorldSpace(_jettonPosition[i]);
	//}

	//_dealer->setTablePoint(tablePos);
	//_dealer->setSeatPoint(seatPos);
	//_dealer->setBetFlyToPoint(BetFlyToPos);
	//_dealer->setDealerPoint(Vec2(_visibleWidth * 0.5f, _visibleHeight * 0.55f));

    _dealer->dealHandCard(cards);
}

/************************************************************************/
//seatNo  玩家座位ID  视图index
void GameTableUIV88::showUser(BYTE seatNo, bool bMe, bool sex) {
    if (!seatNoIsOk(seatNo)) {
        return;
    }

    UserInfoStruct* userInfo = _tableLogic->getUserBySeatNo(_tableLogic->viewToLogicSeatNo(seatNo));
	PlayerUIV88* player = PlayerUIV88::create(_tableLogic->getUserId(_tableLogic->viewToLogicSeatNo(seatNo)), seatNo);
    //player->setAnchorPoint(Vec2(0.5f, 0.5f));
    player->setAnchorPoint(seatNo == 0 ? Vec2::ANCHOR_BOTTOM_LEFT : (seatNo == 1 || seatNo == 2) ? Vec2::ANCHOR_MIDDLE_RIGHT : seatNo == 3 ? Vec2::ANCHOR_MIDDLE_TOP : (seatNo == 4 || seatNo == 5) ? Vec2::ANCHOR_MIDDLE_LEFT : Vec2::ANCHOR_MIDDLE);
    player->setPosition(_seatPosition[seatNo]);

    player->setTableUI(this);

	player->setSexAndIcon(userInfo);

    player->note(0);
    //	player->setScale(0.8);

	_tableUI.lOutline->addChild(player, playerHeadOlder[seatNo]);

    _players[seatNo] = player;

    _tableUI.bSeat[seatNo]->setVisible(false);
    //if (bMe)
    //{
    //	for (BYTE i = 0; i < PLAY_COUNT; i++)
    //	{
    //		_tableUI.bSeat[i]->setTouchEnabled(false);
    //	}
    //}
}

void GameTableUIV88::showUserUp(BYTE seatNo, bool bMe) {
    if (seatNoIsOk(seatNo) && nullptr != _players[seatNo]) {

        _players[seatNo]->removeFromParentAndCleanup(true);
        _players[seatNo] = nullptr;
    }
    _tableUI.bSeat[seatNo]->setVisible(true);

    //if (bMe)
    //{
    //	for (BYTE i = 0; i < PLAY_COUNT; i++)
    //	{
    //		// 人走?? 箭头跳起??
    //		if (nullptr == _players[i])
    //		{
    //			_tableUI.bSeat[i]->setTouchEnabled(true);
    //		}

    //	}
    //}
}

void GameTableUIV88::showUserName(BYTE seatNo, CHAR* name) {
    if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr) {
        _players[seatNo]->setName(ForceGBKToUtf8(name));
    }
}

void GameTableUIV88::showUserMoney(BYTE seatNo, LLONG MONEY) {
    if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr) {
        _players[seatNo]->setChip(MONEY);
    }
}

void GameTableUIV88::showDealer(BYTE seatNo) {
    _tableUI.iDealer[seatNo]->setVisible(true);
}

void GameTableUIV88::showReady(bool visible) {
    if (_tableUI.bReady != nullptr) {
        _tableUI.bReady->setVisible(visible);
    }
}

void GameTableUIV88::showReadyPrivate() {
    _canShowReadySign = true;

    for (int i = 0; i < PLAY_COUNT; i++) {
        BYTE logicNo = _tableLogic->viewToLogicSeatNo(i);
        if (seatNoIsOk(logicNo) && _players[logicNo]) {
            if (_playerReadyState.at(i)) {
                _tableUI.ReadySign[i]->setVisible(true);
            }
        }
    }
}

void GameTableUIV88::delayShowReady(bool visible /* = true */) {
    scheduleOnce(schedule_selector(GameTableUIV88::resetNewGameUI), 3.0f);
}

void GameTableUIV88::resetNewGameUI(float dt) {

    //m_bNextOut = false;

    //reset all UI
    showDashboard(false);

    showReadySign(INVALID_SEAT_NO, false);
    showGiveUpCard(INVALID_SEAT_NO, false);
    updateOperateCount(0, 0);

    controlShowOrHideTotalNode(false);

    for (int i = 0; i < PLAY_COUNT; i++) {
        if (seatNoIsOk(i) && nullptr != _players[i]) {
            _players[i]->resetForNewGame();
        }
    }

	//_dealer->setDealerPoint(Vec2(_visibleWidth * 0.5f, _visibleHeight * 0.55f));
    //show ready button
    this->runAction(Sequence::create(DelayTime::create(2.5f), CallFunc::create([this]() {
        showReady(true);
        showReadyPrivate();
    }), nullptr));

}



void GameTableUIV88::showNextGame(bool visible) {
    if (_tableUI.next_game != nullptr) {
        _tableUI.next_game->setVisible(visible);
    }
}

//void GameTableUIV88::showAddRange(LLONG min, LLONG max)
//{
//	_dashboard->setAddRange(min, max);
//}


void GameTableUIV88::showUserNoteMoney(BYTE seatNo, LLONG money/*, int DiZhu*/) {

    //if (0!=money && 0!=DiZhu)
    //{
    //	betNum = money / DiZhu;
    //}

    //_dealer->betMoney(seatNo, money);
    if (money <= 0) {
        return;
    }

    //auto player = getPlayer(seatNo);
    //if(player != nullptr)
    //{
    //	player->note(money);
    //}

    updateNoteMoney(seatNo, money);

}

//更新玩家下注总额，传入的金额为玩家的总下注额  seatNo  传入 作为IDX   使用时需要转换
void GameTableUIV88::showUserTotalBetMoney(BYTE seatNo, long long betMoney) {
    if (betMoney <= 0) {
        betMoney = 0;
    }
    auto player = getPlayer(_tableLogic->logicToViewSeatNo(seatNo));
    if (player != nullptr) {
        player->note(betMoney);
    }
}

void GameTableUIV88::updateNoteMoney(BYTE seatNo, LLONG money) {
    long long betMoney = money;//为展示的筹码总金额
    int moneyType = 0;//筹码类型
    long long betNum = 0;//同类型筹码的个数
    do {
        if (betMoney >= BetConstMoney[4]) {
            //超过100W
            moneyType = 3;
            betNum = betMoney / BetConstMoney[4];
            betMoney %= BetConstMoney[4];
        } else if (betMoney < BetConstMoney[4] && betMoney >= BetConstMoney[3]) {
            //10W到100W
            moneyType = 2;
            betNum = betMoney / BetConstMoney[3];
            betMoney %= BetConstMoney[3];
        } else if (betMoney < BetConstMoney[3] && betMoney >= BetConstMoney[2]) {
            //1W到10W
            moneyType = 1;
            betNum = betMoney / BetConstMoney[2];
            betMoney %= BetConstMoney[2];
        } else if (betMoney < BetConstMoney[2] && betMoney >= BetConstMoney[1]) {
            //1K到1W
            moneyType = 0;
            betNum = betMoney / BetConstMoney[1];
            betMoney %= BetConstMoney[1];
        } else if (betMoney < BetConstMoney[1] && betMoney >= BetConstMoney[0]) {
            //100到1K
            moneyType = 5;
            betNum = betMoney / BetConstMoney[0];
            betMoney %= BetConstMoney[0];
        } else {
            moneyType = 5;
            betNum = betMoney / BetConstMoney[0];
            betMoney = 0;
        }

        for (int i = 0; i < betNum; i++) {
            _dealer->betMoney(seatNo, moneyType);
        }

    } while (betMoney > 0);
}

void GameTableUIV88::showWin(BYTE winnerSeatNo) {
    // recycle all chips to the winner
    _dealer->recycleChips(winnerSeatNo);
}


void GameTableUIV88::startEndBox(S_C_GameEnd *pdata, BYTE *viewSeat, BYTE viewWiner) {
    _endNode->setVisible(true);

    _vecCard.clear();

    showReadySign(INVALID_SEAT_NO, false);
    for (int i = 0; i < PLAY_COUNT; i++) {
        _playerReadyState.at(i) = false;
        
    }

    //记录当局玩家??
    int playerCount = 0;

    //结算??
    Node *WinOrLoseImage = nullptr;

	WinOrLoseImage = dynamic_cast<Node*>(_endNode->getChildByName("Node_win"));
	WinOrLoseImage->setVisible(true);
    
    _endNode->setVisible(true);
//    ImageView* settlebg;
//    settlebg = dynamic_cast<ImageView*>(_endNode->getChildByName("settleBg"));
//    settlebg->setVisible(true);
//    settlebg->setAnchorPoint(Vec2(0.5,1));

	dynamic_cast<Node*>(_endNode->getChildByName("TitleNode"))->setVisible(true);
	
    //如果赢家是自??

	if (0 == viewWiner) {
		_loseNode->setVisible(false);
		_winNode->setVisible(true);
		_winAction->play("shengli", false);
//        settlebg->loadTexture(END_WIN_BG);
		HNAudioEngine::getInstance()->playEffect(GAME_WIN_AUDIO_EFFECT_V66);
	}
	else {
//        settlebg->loadTexture(END_LOST_BG);
		_winNode->setVisible(false);
		_loseNode->setVisible(true);
		_loseAction->play("shibai", false);
		HNAudioEngine::getInstance()->playEffect(GAME_LOSE_AUDIO_EFFECT_V66);
	}




    //////////////////////////////////////////////////////////////////////////
    //结算框信息
    for (int i = 0; i < PLAY_COUNT; i++) {
        //
        if (pdata->i64ChangeMoney[i] != 0) {
            showUserProfit(i, pdata->i64ChangeMoney[i]);
            playerCount++;
            string player = StringUtils::format("player%d", playerCount);
            Node *ImagePlayer = dynamic_cast<Node*>(WinOrLoseImage->getChildByName(player));
            ImagePlayer->setVisible(true);

			string userName;
			auto userInfo = _tableLogic->getUserInfoBySeatNo(i);
			if (userInfo) {
				userName = userInfo->nickName;
			}
			else {
				userName = _tableLogic->_userName[i];
			}
			((Text *)ImagePlayer->getChildByName("nick_name"))->setString(ForceGBKToUtf8(userName.c_str()));

            bool IsWin = (pdata->i64ChangeMoney[i] > 0);
            ((ImageView *)ImagePlayer->getChildByName("win_sign"))->setVisible(IsWin);
            ((ImageView *)ImagePlayer->getChildByName("lose_sign"))->setVisible(!IsWin);

            string money = StringUtils::format("%lld", pdata->i64ChangeMoney[i]);
            ((Text *)ImagePlayer->getChildByName("win_lose_num"))->setString(money);

//                string gameTimes = StringUtils::format("%d", pdata->iOperateCount[i]);
//                ((Text *)ImagePlayer->getChildByName("game_times"))->setString(gameTimes);

            //同步更新玩家背包金币的显示
            int viewIdx = _tableLogic->logicToViewSeatNo(i);
            if (_players[viewIdx]) {
                _players[viewIdx]->setChip(_players[viewIdx]->getChip() + pdata->i64ChangeMoney[i]);
            }


            //显示扑克
            for (int j = 0; j < MAX_CARD_COUNT; j++) {
				PokerCardV88* tmpCard = PokerCardV88::create(pdata->byCard[i][j]);
                Vec2 cardPosition = ((Node *)ImagePlayer->getChildByName("card_position"))->getPosition();
				if (tmpCard)
				{
					tmpCard->setPosition(cardPosition + Vec2(20 * (j - 1), -16));
					tmpCard->setScale(0.48f);
					ImagePlayer->addChild(tmpCard);
					_vecCard.push_back(tmpCard);
				}
  
            }
            //自己的底框
            if (_tableLogic->logicToViewSeatNo(i) == 0 && nullptr != m_myTipImage) {
                m_myTipImage->setPositionY(ImagePlayer->getPositionY());
                m_myTipImage->setVisible(true);
            }
            
        }
    }

//    settlebg->setScaleY(0.23*playerCount+0.2);


    //隐藏不存在的玩家
    for (int i = playerCount; i < PLAY_COUNT; i++) {
        playerCount++;
        string player = StringUtils::format("player%d", playerCount);
        Node *ImagePlayer = dynamic_cast<Node*>(WinOrLoseImage->getChildByName(player));
        ImagePlayer->setVisible(false);
    }

    scheduleOnce(schedule_selector(GameTableUIV88::releaseGameEndBox), 5.f);
}
    
    void GameTableUIV88::playBigWinAnimation(BYTE cardShape) {
        std::string resPath = "";
        std::string actionName = "";
        if (cardShape == SH_SAME_HUA_CONTINUE) {
            resPath = "v66/animation/straightflush/straightflush.csb";
            actionName = "straightflush";
        }
        else if (cardShape == SH_THREE) {
            resPath = "v66/animation/redbag/redbag_bomb.csb";
            actionName = "redbag_bomb";
        }
        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
        addChild(colorLayer, MEMBAN_ZORDER+10);
        colorLayer->runAction(Sequence::create(DelayTime::create(2.0f), RemoveSelf::create(), nullptr));
        auto node = CSLoader::createNode(resPath);
        if (node) {
            node->setPosition(WIN_SIZE/2);
            addChild(node, MEMBAN_ZORDER+10);
            auto action = CSLoader::createTimeline(resPath);
            if (action) {
                node->runAction(action);
                action->play(actionName, false);
            }
        }
    }

void GameTableUIV88::showEndBox(S_C_GameEnd *pdata, BYTE *viewSeat, BYTE viewWiner) {

    memcpy(_viewSeat, viewSeat, sizeof(_viewSeat));
    _pendData = *pdata;

    BYTE _viewOfWiner = viewWiner;

    //big win animation
    if (pdata->byCardShape[_tableLogic->getMySeatNo()] >= SH_SAME_HUA_CONTINUE) {
        _endNode->runAction(Sequence::create(
                                             CallFunc::create([=]() {
            playBigWinAnimation(pdata->byCardShape[_tableLogic->getMySeatNo()]);
        }),
                                             DelayTime::create(2.0f),
                                             CallFunc::create([=]() {
            startEndBox(&_pendData, _viewSeat, _viewOfWiner);
        }),
                                             nullptr));
    }
    else {
        _endNode->runAction(Sequence::create(
                                             CallFunc::create([=]() {
            startEndBox(&_pendData, _viewSeat, _viewOfWiner);
        }),
                                             nullptr));
    }
    
}


void GameTableUIV88::releaseGameEndBox(float dt) {
    //判断玩家输赢
    bool isWin = _endNode->getChildByName("Node_win")->isVisible();
    _endNode->getChildByName("Node_win")->setVisible(false);
    //	_endNode->getChildByName("Node_lose")->setVisible(false);

    for (auto TempCard : _vecCard) {
        TempCard->removeFromParent();
    }
    _vecCard.clear();

//    if (isWin) {
//        ////粒子效果
//        //auto gold = ParticleSystemQuad::create("goldenflower/animation/gold.plist");
//        //gold->setPosition(_seatPosition[0]+Vec2(0,-100));
//        //Sprite *goldSprite = Sprite::create("goldenflower/animation/gold.png");
//        //gold->setTexture(goldSprite->getTexture());
//        //_endNode->addChild(gold, 10);
//
//        //auto star = ParticleSystemQuad::create("goldenflower/animation/star.plist");
//        //star->setPosition(_seatPosition[0] + Vec2(0, -100));
//        //Sprite *starSprite = Sprite::create("goldenflower/animation/star.png");
//        //star->setTexture(starSprite->getTexture());
//        //_endNode->addChild(star, 10);
//
//        //scheduleOnce(schedule_selector(GameTableUIV88::disapearEndBox), 4.0f);
//    } else {
//        //隐藏结算??
//        _endNode->setVisible(false);
//    }
    if (nullptr != m_myTipImage)
    {
        m_myTipImage->setVisible(false);
    }
    
    _endNode->setVisible(false);
    
    _winNode->setVisible(false);
    _loseNode->setVisible(false);
//    dynamic_cast<ImageView*>(_endNode->getChildByName("settleBg"))->setVisible(false);
    
	dynamic_cast<Node*>(_endNode->getChildByName("TitleNode"))->setVisible(false);
//        //下局离开这里执行
//        if (m_bNextOut)
//        {
//            m_bNextOut = false;
//            if (RoomLogic()->connected())
//            {
//                _tableLogic->sendStandUp();
//            }
//            else
//            {
//                leaveTheGame();
//            }
//        }
}

void GameTableUIV88::disapearEndBox(float dt) {
    //隐藏结算??
    _endNode->setVisible(false);
}

void GameTableUIV88::showWatchCard(BYTE seatNo,bool visible) {
    if (INVALID_SEAT_NO == seatNo) {
        for (int i = 0; i < PLAY_COUNT; i++) {
            _tableUI.WatchSign[i]->setVisible(visible);
        }
    } else {
        _tableUI.WatchSign[seatNo]->setVisible(visible);
    }
}

void GameTableUIV88::showGiveUpCard(BYTE seatNo, bool visible) {
    if (INVALID_SEAT_NO == seatNo) {
        for (int i = 0; i < PLAY_COUNT; i++) {
            if (visible) {
                _players[i]->lose();
                _tableUI.WatchSign[i]->setVisible(false);

            }
            _tableUI.GiveUpSign[i]->setVisible(visible);
        }
    } else {
        if (visible) {
            _players[seatNo]->lose();
            _tableUI.WatchSign[seatNo]->setVisible(false);

        }
        _tableUI.GiveUpSign[seatNo]->setVisible(visible);
    }
}


//显示玩家手牌
void GameTableUIV88::showUserHandCard(BYTE seatNo, const std::vector<BYTE>& cards) {
    //LLONG currentUserID = PlatformLogic()->loginResult.dwUserID;
    for (int i = 0; i < PLAY_COUNT; ++i) {
        if (nullptr != _players[i]) {
            _players[i]->enterGame();
        }
    }

    if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr) {
        if(!_players[seatNo]->isPlaying()) {
            return;
        }

        _players[seatNo]->setHandCard(0, cards[0]);
        _players[seatNo]->setHandCard(1, cards[1]);
        _players[seatNo]->setHandCard(2, cards[2]);
    }
}

//翻牌动画，玩家請求看牌后回調中調??
void GameTableUIV88::showUserFlipCard(BYTE seatNo, const std::vector<BYTE>& cards) {
    if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr) {
        if (!_players[seatNo]->isPlaying()) {
            return;
        }

        const int iCardNum = 3;
        for (int i = 0; i < iCardNum; i++) {
            _players[seatNo]->_handCard[i]->setCardValue(0);
            auto orbit = OrbitCamera::create(0.1f, 1, 0, 0, -90, 0, 0);
            auto orbit1 = OrbitCamera::create(0.2f, 1, 0, 90, -90, 0, 0);
            auto seq = Sequence::create(orbit,
                                        CallFuncN::create(CC_CALLBACK_1(PlayerUIV88::changeHandCardValue, _players[seatNo], i, cards[i])),
                                        orbit1,
                                        nullptr
                                       );
            _players[seatNo]->_handCard[i]->runAction(seq);
        }
    }
}

void GameTableUIV88::clearDesk() {
    for (BYTE i = 0; i < PLAY_COUNT; ++i) {
        if (nullptr != _players[i]) {
            _players[i]->hideHandCard(false);
            _players[i]->stopWait();
            _players[i]->stopPlayWin();
        }
        _tableUI.bCompare[i]->setVisible(false);
        _tableUI.bSeat[i]->setVisible(false);
        _tableUI.iDealer[i]->setVisible(false);
    }
    _dealer->clear();
}

void GameTableUIV88::clearInvalidUser() {
    for (BYTE i = 0; i < PLAY_COUNT; ++i) {
        if (nullptr == _players[i]) {
            _tableUI.bCompare[i]->setVisible(false);
            _tableUI.bSeat[i]->setVisible(false);
            _tableUI.iDealer[i]->setVisible(false);
        }
    }
}


void GameTableUIV88::leaveTheGame() {
    HNRoomLogic::getInstance()->closeRoom();
    //GamePlatform::returnPlatform(ROOMLIST);
    PlatformState* state = PlatformState::create();
    state->setType(ROOMLIST);
    StateManager::getInstance()->setState(state);
}

//显示牌桌准备
void GameTableUIV88::showLoading(bool bVisible) {
    if (bVisible) {
        _loadSprite = Sprite::create("goldenflower/table/loading_view.jpg");
        _loadSprite->setVisible(true);
        Size frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
        _loadSprite->setAnchorPoint(Vec2(0, 0));
        _loadSprite->setPosition(Vec2(0, 0));
        this->addChild(_loadSprite, MEMBAN_ZORDER);
    } else {
        auto fadeOut = FadeOut::create(2.0f);
        _loadSprite->runAction(fadeOut);
    }
}

/*
 * rotate player's postion
 * 1.player
 * 2.seat
 * 3.compare button
 */
void GameTableUIV88::rotateSeat(int offset) {
	PlayerUIV88* oldPlayers[PLAY_COUNT] = { 0 };
	PlayerUIV88* newPlayers[PLAY_COUNT] = { 0 };
    Sprite* oldSeat[PLAY_COUNT] = { 0 };
    Sprite* newSeat[PLAY_COUNT] = { 0 };
    ui::ImageView* newCompare[PLAY_COUNT] = {0};
    ui::Button* oldCompare[PLAY_COUNT] = {0};

    memcpy(oldPlayers, _players, sizeof(_players));
    memcpy(oldSeat, _tableUI.bSeat, sizeof(_tableUI.bSeat));
    memcpy(oldCompare, _tableUI.bCompare, sizeof(_tableUI.bCompare));

    //先更新位置，防止消息处理后位置出??
    for (unsigned int i = 0; i < PLAY_COUNT; i++) {
        int index = (i + offset + PLAY_COUNT) % PLAY_COUNT;
        if (_players[i] != nullptr) {
            newPlayers[index] = _players[i];
            newPlayers[index]->setViewSeatNo(index);
            _players[i]->setHandCardPosition();
        }
        newCompare[index] = _tableUI.bCompare[i];
        newSeat[index]  = _tableUI.bSeat[i];
    }
    memcpy(_players, newPlayers, sizeof(_players));
    memcpy(_tableUI.bSeat, newSeat, sizeof(_tableUI.bSeat));
    memcpy(_tableUI.bCompare, newCompare, sizeof(_tableUI.bCompare));

    //play animation with old pointer.
    int rotateTimes = abs(offset);
    for (int i = 0; i < PLAY_COUNT; i++) {
        Vector<FiniteTimeAction*> actionArray;
        Vector<FiniteTimeAction*> compareArray;
        if (offset > 0) {
            //逆时针转
            for (int j = 0; j <= rotateTimes; j++) {
                int newPos = (i + j + PLAY_COUNT) % PLAY_COUNT;
                actionArray.pushBack(MoveTo::create(0.3f, _seatPosition[newPos]));
                compareArray.pushBack(MoveTo::create(0.3f, oldCompare[newPos]->getPosition()));
            }
        } else {
            //顺时针转
            for (int j = 0; j <= rotateTimes; j++) {
                int newPos = (i - j + PLAY_COUNT) % PLAY_COUNT;
                actionArray.pushBack(MoveTo::create(0.2f, _seatPosition[newPos]));
                compareArray.pushBack(MoveTo::create(0.3f, oldCompare[newPos]->getPosition()));
            }
        }
        auto sequence = Sequence::create(actionArray);
        auto compareSequence = Sequence::create(compareArray);
        if (oldPlayers[i] != nullptr) {
            oldPlayers[i]->runAction(sequence->clone());
        }
        oldSeat[i]->runAction(sequence);
        oldCompare[i]->runAction(compareSequence);
    }
}


void GameTableUIV88::alertDialog(const std::string& title, const std::string& message) {

}

//void GameTableUIV88::showNotice(const std::string &message)
//{
//	GameNotice* notice = GameNotice::create(this, message);
//	this->reorderChild(notice, MAX_ZORDER);
//	notice->show();
//}

//游戏过程中禁止离开提醒
void GameTableUIV88::showForbidLeaveNotice() {
    //auto Prompt = GamePromptLayer::create();
    //Prompt->showPrompt(GetText("limit_leave_in_game"));

	//if (_colorLayer) {
	//	_colorLayer->setVisible(true);
	//	schedule(schedule_selector(GameTableUIV88::hideTips), 3.0f);
	//}
	GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
	layer->setGamePromptTip(GetText("limit_leave_in_game"));
}

void GameTableUIV88::hideTips(float delat)
{
	if (_colorLayer) {
		_colorLayer->setVisible(false);
	}
	unschedule(schedule_selector(GameTableUIV88::hideTips));
}

void GameTableUIV88::showUserProfit(BYTE seatNo, LLONG money) {
    auto player = getPlayer(seatNo);
    if(player != nullptr) {
        player->profit(money);
    }
}

//void GameTableUIV88::showTableInfo(const std::string& tableName)
//{
//	char str[50] = { 0 };
//	sprintf(str, "房间:%s", tableName.c_str());
//	//_lRoomInfo = Label::createWithTTF(gFontConfig_18, GBKToUtf8(str));
//	_lRoomInfo = Label::createWithSystemFont(GBKToUtf8(str), "", 18);
//	_lRoomInfo->setAnchorPoint(Vec2(1, 1));
//	auto size = Director::getInstance()->getWinSize();
//	_lRoomInfo->setPosition(Vec2(size.width - 10, size.height - 10));
//	this->addChild(_lRoomInfo, TableInfo_Zorder);
//}

void GameTableUIV88::IStartTimer(int second, BYTE type) {
    _remainTime = second;
//    unschedule(schedule_selector(GameTableUIV88::timerUpdate));

    if (second > 0) {
        m_clockObj->start(second);
//        schedule(schedule_selector(GameTableUIV88::timerUpdate), 1.0f);
        m_clockObj->setTimeCallBack([&]() {
            if (_tableUI.bReady) {
                if (_tableUI.bReady->isVisible()) {
                    if (RoomLogic()->connected()) {
                        _tableLogic->sendStandUp();
                    } else {
                        leaveTheGame();
                    }
                }
            } else {
                leaveTheGame();
            }
        });
    } else {
		m_clockObj->stop();
    }
}

//void GameTableUIV88::timerUpdate(float delat) {
//    _remainTime--;
//    if (_remainTime < 1) {
//        unschedule(schedule_selector(GameTableUIV88::timerUpdate));
//        m_clockObj->stop();
//        
//        if (_tableUI.bReady) {
//            if (_tableUI.bReady->isVisible()) {
//                if (RoomLogic()->connected()) {
//                    _tableLogic->sendStandUp();
//                } else {
//                    leaveTheGame();
//                }
//            }
//        } else {
//            leaveTheGame();
//        }
//
//        return;
//    }
//}

void GameTableUIV88::showWaitTime(BYTE seatNo, BYTE byTime, BYTE byTotalTime) {
    if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr) {
        if (byTime > 0) {
            _players[seatNo]->startWait(byTime, byTotalTime);
        } else {
            _players[seatNo]->stopWait();
        }
    }
}

//void GameTableUIV88::showLimitNote(LLONG note)
//{
//	char tmp[50] = { 0 };
//	sprintf(tmp, "%s:%lld", GBKToUtf8("上限"), note);
//	_tableUI.lLimitNote->setString(tmp);
//}

void GameTableUIV88::showBaseNote(LLONG note) {
    char tmp[50] = { 0 };
    sprintf(tmp, "%lld", note);
    _tableUI.lNote->setString(tmp);
}

void GameTableUIV88::updateOperateCount(int current, int maxLoop) {
    if (_tableUI.lOperateCount) {
        ostringstream ss;
        if (maxLoop <= 0) {
            ss << "" << 0;
        } else {
            ss << current << "/" << maxLoop;
        }
        _tableUI.lOperateCount->setString(ss.str().c_str());
    }
}


//void GameTableUIV88::showLimitPerNote(LLONG note)
//{
//	char tmp[50] = { 0 };
//	sprintf(tmp, "%s:%lld", GBKToUtf8("顶注"), note);
//	_tableUI.lTopNote->setString(tmp);
//}
//
//void GameTableUIV88::showGuoDi(LLONG note)
//{
//	char tmp[50] = { 0 };
//	sprintf(tmp, "%s:%lld", GBKToUtf8("锅底"), note);
//	_tableUI.lGuoDi->setString(tmp);
//}

void GameTableUIV88::controlShowOrHideTotalNode(bool show /* = true */) {
    if (_tableUI.lTotalNode) {
        _tableUI.lTotalNode->setVisible(show);
    }
}

void GameTableUIV88::showTotalNote(LLONG note) {
    if (_tableUI.lTotalNote) {
        char tmp[50] = { 0 };
        sprintf(tmp, "%s", Tools::addComma(note).c_str());
        _tableUI.lTotalNote->setString(tmp);
    }
}

void GameTableUIV88::showTotalNoteFirst(LLONG note) {
    if (note > 0) {
        updateNoteMoney(INVALID_BYTE, note);
    }
}

//void GameTableUIV88::showMyTotalNote(LLONG note)
//{
//   return;
//char tmp[50] = { 0 };
//sprintf(tmp, "%s:%lld", GBKToUtf8("个人总注"), note);
//_tableUI.lMyTotalNote->setString(tmp);
//}

void GameTableUIV88::showReadySign(BYTE seatNo,bool visible) {
    if (INVALID_SEAT_NO == seatNo) {
        for (int i = 0; i < PLAY_COUNT; i++) {
            _tableUI.ReadySign[i]->setVisible(visible);
        }
    } else {
        BYTE logicSeatNo = _tableLogic->viewToLogicSeatNo(seatNo);
        if (!_canShowReadySign && seatNoIsOk(logicSeatNo) && logicSeatNo != _tableLogic->getMySeatNo()) {
            _tableUI.ReadySign[seatNo]->setVisible(false);
            return;
        }
        if (visible) {
            if (seatNoIsOk(logicSeatNo) && _players[logicSeatNo] && _players[logicSeatNo]->isQuit()) {
                //补丁 如果仍然在游戏中，则不显示其他玩家的准备标示
                _tableUI.ReadySign[seatNo]->setVisible(false);
            } else {
                _tableUI.ReadySign[seatNo]->setVisible(visible);
            }
        } else {
            _tableUI.ReadySign[seatNo]->setVisible(visible);
        }

    }
}

void GameTableUIV88::storeReadyState(BYTE seatNo, bool ready) {
    if (seatNoIsOk(seatNo)) {
        _playerReadyState.at(seatNo) = ready;
    }
}

void GameTableUIV88::showUserLookCard(BYTE seatNo, bool isMe) {
    if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr) {
        _players[seatNo]->playAction(PlayerUIV88::Action::eLookCard);
        if(!isMe) {
            //_players[seatNo]->lookHandCard();
            _tableUI.WatchSign[seatNo]->setVisible(true);
        }
    }

}

void GameTableUIV88::showUserGiveUp(BYTE seatNo) {
    if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr) {
        _players[seatNo]->playAction(PlayerUIV88::Action::eGiverUp);
        _players[seatNo]->lose();
        _tableUI.WatchSign[seatNo]->setVisible(false);
        _tableUI.GiveUpSign[seatNo]->setVisible(true);
        _dashboard->setAddBetVisible(false);
    }

    //隐藏比牌提示标志
    for (BYTE i = 0; i < PLAY_COUNT; ++i) {
        if (nullptr != _players[i]) {
            _tableUI.bCompare[i]->setVisible(false);
        }
    }
}

void GameTableUIV88::updateBetButtonValues(int values[4]) {
    if (_dashboard) {
        _dashboard->setBetButtonValue(values);
    }
}

void GameTableUIV88::showUserNote(BYTE seatNo) {
    if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr) {
        _players[seatNo]->playAction(PlayerUIV88::Action::eNote);
    }
}

void GameTableUIV88::showUserCompare(BYTE fromSeatNo, BYTE toSeatNo, BYTE winSeatNo) {
    //if(seatNoIsOk(fromSeatNo) && _players[fromSeatNo] != nullptr)
    //{
    //	_players[fromSeatNo]->playAction(PlayerUI::Action::eCompareCard);
    //}

    //if(seatNoIsOk(toSeatNo) && _players[toSeatNo] != nullptr)
    //{
    //	_players[toSeatNo]->playAction(PlayerUI::Action::eCompareCard);
    //}
}

void GameTableUIV88::showUserFollow(BYTE seatNo) {
    if (seatNoIsOk(seatNo) && _players[seatNo] != nullptr) {
        _players[seatNo]->playAction(PlayerUIV88::Action::eFollow);
    }
}


void GameTableUIV88::showUserOpenCard(BYTE seatNo) {

}

void GameTableUIV88::dealGameTipExit(goldenflower::tag_s2c_tip_exit *msg) {
    if (msg == nullptr) {
        return;
    }
    tipMsg = msg;

    //不退出
    if (tipMsg->bIsExit == 0) {
        //下一局离开
        if (m_bNextOut) {
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
                if (RoomLogic()->connected()) {
                    _tableLogic->sendStandUp();
                } else {
                    leaveTheGame();
                }

            })));
        }

        return;
    }

    //要退出
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
        tipCallback();
    })));

}

void GameTableUIV88::showDashboard(bool visible) {
    visible ? _dashboard->show() : _dashboard->hide();
}


void GameTableUIV88::setLookVisible(bool visible) {
    _dashboard->setLookVisible(visible);
}

void GameTableUIV88::setFollowVisible(bool visible) {
    _dashboard->setFollowVisible(visible);
}

void GameTableUIV88::setAddVisible(bool visible, bool *CanAdd) {
    memcpy(_dashboard->_CanAddOfAll, CanAdd, sizeof(_dashboard->_CanAddOfAll));
    //当前玩家是自己并且可以下注才显示加注按钮
    _dashboard->setAddVisible( visible  &&  (CanAdd[0] || CanAdd[1] || CanAdd[2] || CanAdd[3]) );
}

void GameTableUIV88::setOpenVisible(bool visible) {
    _dashboard->setOpenVisible(visible);
}

void GameTableUIV88::setGiveUpVisible(bool visible) {
    _dashboard->setGiveUpVisible(visible);
}

void GameTableUIV88::boom(Ref* pRef) {
    Layer* tempLayer = (Layer*)pRef;
    auto losercard0 = (PokerCardV88*)tempLayer->getChildByTag(0);
	auto losercard1 = (PokerCardV88*)tempLayer->getChildByTag(1);
	auto losercard2 = (PokerCardV88*)tempLayer->getChildByTag(2);

    if (losercard0 == nullptr || losercard1 == nullptr || losercard2 == nullptr) {
        return;
    }

    losercard0->setGray(true);
    losercard1->setGray(true);
    losercard2->setGray(true);
}

////炸弹效果
//void GameTableUIV88::moveBomb(bool leftWiner)
//{
//	//炸弹
//	ImageView *bomb = ImageView::create("goldenflower/res/tableSrc/bomb.png");
//	int setposition = 0;
//	Size headSize = _tableUI.head_image[0]->getContentSize();
//	//移动的距??
//	int MoveDistance = (_tableUI.head_image[1]->getPosition()).x - (_tableUI.head_image[0]->getPosition()).x;
//	//MoveDistance += headSize.width/2;
//
//	if (leftWiner)
//	{
//		_tableUI.head_image[0]->addChild(bomb, 101);
//		bomb->ignoreAnchorPointForPosition(false);
//		bomb->setPosition(Vec2(headSize.width / 2, headSize.height / 2));
//		setposition = 1;
//	}
//	else
//	{
//		_tableUI.head_image[1]->addChild(bomb, 101);
//		bomb->ignoreAnchorPointForPosition(false);
//		bomb->setPosition(Vec2(headSize.width / 2, headSize.height / 2));
//		setposition = -1;
//	}
//	bomb->setVisible(true);
//
//	//炸弹移动轨迹
//	ccBezierConfig tr0;
//	tr0.endPosition = Vec2(MoveDistance*setposition, 0);
//	tr0.controlPoint_1 = Vec2(0, 260);
//	tr0.controlPoint_2 = Vec2(MoveDistance*setposition, 260);
//	CCActionInterval* bezierForward = CCBezierBy::create(0.8f, tr0);
//
//	auto rotateBy = RotateBy::create(0.8f, 360);
//	auto spawn = Spawn::create(bezierForward, rotateBy, nullptr);
//
//	bomb->runAction(Sequence::create(
//	spawn,
//	RemoveSelf::create(true),
//	CallFunc::create(CC_CALLBACK_0(GameTableUIV88::playBombAnimate, this, 1.6f, leftWiner)),
//		nullptr));
//
//}

void GameTableUIV88::moveHead(bool winnerIsLeft, BYTE winSeatNo) {
    if (winnerIsLeft) {
        auto moveLeftHead = MoveTo::create(0.8f, _seatPosition[winSeatNo]);

        ccBezierConfig tr0;
        tr0.endPosition = Vec2(500, 0);
        tr0.controlPoint_1 = Vec2(0, 260);
        tr0.controlPoint_2 = Vec2(500, 260);
		ActionInterval* bezierForward = CCBezierBy::create(0.5f, tr0);

        auto rotateBy = RotateBy::create(0.8f, 360);

        _tableUI.head_image[0]->runAction(Sequence::create(
                                              moveLeftHead,
        CallFunc::create([=]() {
            _tableUI.head_image[0]->setVisible(false);
        }),
        nullptr));

        auto Spawn = Spawn::create(bezierForward, rotateBy, nullptr);
        _tableUI.head_image[1]->runAction(Sequence::create(
                                              Spawn,
        CallFunc::create([=]() {
            _tableUI.head_image[1]->setVisible(false);
        }),
        nullptr));

    } else {
        auto moveRightHead = MoveTo::create(0.8f, _seatPosition[winSeatNo]);
        ccBezierConfig tr1;
        tr1.endPosition = Vec2(-500, 0);
        tr1.controlPoint_1 = Vec2(0, 260);
        tr1.controlPoint_2 = Vec2(-500, 260);
		ActionInterval* bezierForwardLeft = CCBezierBy::create(0.5f, tr1);
        auto RotateByLeft = RotateBy::create(0.8f, 360);

        _tableUI.head_image[1]->runAction(Sequence::create(
                                              moveRightHead,
        CallFunc::create([=]() {
            _tableUI.head_image[0]->setVisible(false);
        }),
        nullptr));


        auto RightSpawn = Spawn::create(bezierForwardLeft, RotateByLeft,nullptr);
        _tableUI.head_image[0]->runAction(Sequence::create(
                                              RightSpawn,
        CallFunc::create([=]() {
            _tableUI.head_image[1]->setVisible(false);
        }),
        nullptr));
    }
}

void GameTableUIV88::playVSAnimation() {
    if (_vsNode == nullptr) {
        _vsNode = CSLoader::createNode(ANIMATION_VS_CSB);
        _vsNode->setPosition(WIN_SIZE / 2);
        addChild(_vsNode);
    }
    _vsNode->stopAllActions();
    _vsNode->setVisible(true);
    auto action = CSLoader::createTimeline(ANIMATION_VS_CSB);
    if (action) {
        _vsNode->runAction(action);
        action->play("bipai_2ren", false);
    }
}

void GameTableUIV88::playHitAnimation(Vec2 hitPos) {
    if (_hitNode == nullptr) {
        _hitNode = CSLoader::createNode(ANIMATION_VS_HIT_CSB);
        _hitNode->setPosition(WIN_SIZE / 2);
        addChild(_hitNode);
    }
    _hitNode->stopAllActions();
    _hitNode->setPosition(hitPos);
    _hitNode->setVisible(true);
    auto action = CSLoader::createTimeline(ANIMATION_VS_HIT_CSB);
    _hitNode->runAction(action);
    action->play("bipai_wenzi", false);
    this->runAction(Sequence::create(DelayTime::create(0.9f), CallFunc::create([]() {
        HNAudioEngine::getInstance()->playEffect(HIT_MUSIC_EFFECT_PATH);
    }), nullptr));
    
}

void GameTableUIV88::playBombAnimate(float duration, bool leftWin) {
    //	SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
    //	frameCache->addSpriteFramesWithFile("goldenflower/table/bombPlist.plist");
    //
    //	auto animation = Animation::create();
    //	for (int i = 1; i < 17;i++)
    //	{
    //		string szName = StringUtils::format("boom_%02d.png", i);
    //		auto frame = frameCache->getSpriteFrameByName(szName);
    //		animation->addSpriteFrame(frame);
    //	}
    //
    //	animation->setDelayPerUnit(duration / 16.0f);
    //	animation->setRestoreOriginalFrame(true);
    //
    //	auto action = Animate::create(animation);
    //
    //	//爆炸的位??
    //	Vec2 bombPosition;
    //	if (leftWin)
    //	{
    //		bombPosition = _tableUI.head_image[1]->getPosition();
    //	}
    //	else
    //	{
    //		bombPosition = _tableUI.head_image[0]->getPosition();
    //	}
    //
    //	auto sp = Sprite::create();
    //	sp->setScale(0.8f);
    //	sp->setPosition(bombPosition);
    //	sp->runAction(action);
    //	this->addChild(sp, 102);
}

//比牌动画
void GameTableUIV88::showCompareResult(BYTE winSeatNo, BYTE loseSeatNo) {
    auto sex = false; // Lady
    auto audio = sex ? GOLD_PK_MAN : GOLD_PK_LADY;
    HNAudioEngine::getInstance()->playEffect(audio);

    auto winner = this->getPlayer(winSeatNo);
    auto loser = this->getPlayer(loseSeatNo);
    loser->lose();
//        loser->setHandCardBroken();
    _tableUI.WatchSign[loseSeatNo]->setVisible(false);

    //比牌头像发光
	this->getPlayer(winSeatNo)->showBipaiPlayerEffect();
	this->getPlayer(loseSeatNo)->showBipaiPlayerEffect();
    auto winnerLayer = Layer::create();
    auto loserLayer = Layer::create();

    this->addChild(winnerLayer, VS_NODE_ZORDER + 1);
    this->addChild(loserLayer, VS_NODE_ZORDER + 1);

	PokerCardV88* winnerCards[3];
	PokerCardV88* loserCards[3];

    ////////////////////////////////////////////////////////////////////////
    // init handcards
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	_fScaleX = WIN_SIZE.width / DESIGN_SIZE.width;
	_fScaleY = WIN_SIZE.height / DESIGN_SIZE.height;

    float offset = 15;
    winner->hideHandCard(false);
    loser->hideHandCard(false);
    for (int i = 0; i < 3; i++) {
        winnerCards[i] = PokerCardV88::create(0x00);
        winnerCards[i]->setScale(0.86);
        Vec2 tmp = winner->convertToWorldSpace(winner->getAnchorPointInPoints());
        winnerCards[i]->setPosition(tmp.x + offset*i, tmp.y);
        winnerLayer->addChild(winnerCards[i], i);
		winnerLayer->setVisible(false);

		loserCards[i] = PokerCardV88::create(0x00);
        loserCards[i]->setScale(0.86);
        tmp = loser->convertToWorldSpace(loser->getAnchorPointInPoints());
        loserCards[i]->setPosition(tmp.x + offset*i, tmp.y);
        loserLayer->addChild(loserCards[i], i);
		loserLayer->setVisible(false);
    }

    loserCards[0]->setTag(0);
    loserCards[1]->setTag(1);
    loserCards[2]->setTag(2);

    ////////////////////////////////////////////////////////////////////////
    // run action, move to  1 -> 2 -> 3 -> 4 -> 2 -> 1
    Layer* leftLayer;
    Layer* rightLayer;
    PlayerUIV88* leftPlayer;
    PlayerUIV88* rightPlayer;

    //赢家是否在左??
    bool winnerIsLeft = true;

    if (winnerCards[0]->getPosition().x < loserCards[0]->getPosition().x) {
        leftLayer = winnerLayer;
        leftPlayer = winner;

        rightLayer = loserLayer;
        rightPlayer = loser;
    } else {
        leftLayer = loserLayer;
        leftPlayer = loser;
        winnerIsLeft = false;

        rightLayer = winnerLayer;
        rightPlayer = winner;
    }



    //-----------
    winnerLayer = nullptr;
    loserLayer = nullptr;
    winner = nullptr;
    loser = nullptr;
    //-----------
    leftLayer->setGlobalZOrder(VS_NODE_ZORDER + 5);
    rightLayer->setGlobalZOrder(VS_NODE_ZORDER + 5);

//        auto widthOfCards = winnerCards[0]->getContentSize().width;

    // target position 1
    auto leftPosition1 = leftLayer->getPosition();
    auto rightPosition1 = rightLayer->getPosition();

    auto Lp1 = leftPlayer->convertToWorldSpace(leftPlayer->getAnchorPointInPoints());
    auto Lp2 = Vec2(origin.x + visibleSize.width * 1 / 3, origin.y + visibleSize.height / 2);
    auto Lo1 = leftPosition1;
    auto Lo2 = Lp2 - Lp1 + Lo1;
    auto leftPosition2 = Lo2+Vec2(40,0);

    auto Rp1 = rightPlayer->convertToWorldSpace(rightPlayer->getAnchorPointInPoints());
    auto Rp2 = Vec2(origin.x + visibleSize.width * 2 / 3, origin.y + visibleSize.height / 2);
    auto Ro1 = rightPosition1;
    auto Ro2 = Rp2 - Rp1 + Ro1;
    auto rightPosition2 = Ro2+Vec2(-70,0);

    //创建头像
	//Texture2D* texture;
	//texture = (leftPlayer->getHeadSp())->getTexture();
	//Sprite *leftHead = HNSprite::create("v99/commonHead/touxiang.png");
	//leftHead->setTexture(texture);

	//
	//Sprite *rightHead = HNSprite::create("v99/commonHead/touxiang.png");
	//texture = (rightPlayer->getHeadSp())->getTexture();
	//rightHead->setTexture(texture);
	//创建头像
	//UserHead* leftHead = UserHead::create(HEAD_FRAME_BLUE_V88);
	//leftHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88, "hallimg/niuniu/5_1.png", 0.76f);
	//leftHead->setHeadWithInfo(leftPlayer->getUserInfo());

	//UserHead* rightHead = UserHead::create(HEAD_FRAME_BLUE_V88);
	//rightHead->setClipUserHeadWithFile(DEFAULT_PLAYER_HEAD_V88, "hallimg/niuniu/5_1.png", 0.76f);
	//rightHead->setHeadWithInfo(rightPlayer->getUserInfo());

 //   _tableUI.head_image[0]->addChild(leftHead, VS_NODE_ZORDER + 5);
 //   _tableUI.head_image[0]->setLocalZOrder(VS_NODE_ZORDER + 5);
 //   _tableUI.head_image[0]->setVisible(true);
 //   _tableUI.head_image[0]->setScale(0.7f);
 //   _tableUI.head_image[0]->setPosition(_headImagePostion[0]);
 //   _tableUI.head_image[1]->addChild(rightHead, VS_NODE_ZORDER + 5);
 //   _tableUI.head_image[1]->setLocalZOrder(VS_NODE_ZORDER + 5);
 //   _tableUI.head_image[1]->setVisible(true);
 //   _tableUI.head_image[1]->setScale(0.7f);
 //   _tableUI.head_image[1]->setPosition(_headImagePostion[1]);


    Size headSize = _tableUI.head_image[0]->getContentSize();
    //leftHead->setPosition(Vec2(headSize.width / 2, headSize.height / 2));
    //rightHead->setPosition(Vec2(headSize.width  / 2, headSize.height / 2));

    // move
    auto duration = 0.2f;
    auto leftmove12 = MoveTo::create(duration, leftPosition2);
    auto rightmove12 = MoveTo::create(duration, rightPosition2);

    auto leftmove21 = MoveTo::create(duration, leftPosition1);
    auto rightmove21 = MoveTo::create(duration, rightPosition1);

    // left
    float duration2 = 2.5;
    float duration3 = 2.1;
	Vec2 pos = winnerIsLeft ? Vec2(WIN_SIZE.width * 0.746f, WIN_SIZE.height * 0.32f*_fScaleY) : Vec2(WIN_SIZE.width * 0.248f, WIN_SIZE.height * 0.426f*_fScaleY);
    leftLayer->runAction(Sequence::create(
                             leftmove12,
                             Spawn::create(
                                 CallFunc::create(CC_CALLBACK_0(GameTableUIV88::playVSAnimation, this)),
                                 CallFunc::create(CC_CALLBACK_0(GameTableUIV88::playHitAnimation, this, pos)),
                                 nullptr),

                             //CallFunc::create(CC_CALLBACK_0(GameTableUIV88::moveBomb, this, winnerIsLeft)),
                             DelayTime::create(duration3+1),
                            // CallFunc::create(CC_CALLBACK_0(GameTableUIV88::moveHead, this, winnerIsLeft, winSeatNo)),
                             //DelayTime::create(duration2),
                             CallFuncN::create(CC_CALLBACK_1(GameTableUIV88::boom, this)),
                             //DelayTime::create(duration3),
                             leftmove21,

    CallFunc::create([=]() {
        leftPlayer->hideHandCard(true);
        if (!winnerIsLeft) {
            leftPlayer->setHandCardBroken();
        }
        leftLayer->removeAllChildren();
    }),
    nullptr));

    // right
    rightLayer->runAction(Sequence::create(
                              rightmove12,
                              DelayTime::create(duration2+0.4),
                              CallFuncN::create(CC_CALLBACK_1(GameTableUIV88::boom, this)),
                              //DelayTime::create(duration3),
                              rightmove21,

    CallFunc::create([=]() {
        rightPlayer->hideHandCard(true);
        if (winnerIsLeft) {
            rightPlayer->setHandCardBroken();
        }
        rightLayer->removeAllChildren();
    }),
    nullptr));

}


void GameTableUIV88::showCompareOption(const std::vector<bool> seats) {
    if (seats.size() != PLAY_COUNT) {
        return;
    }
    for (int i = 0; i < PLAY_COUNT; i++) {
        bool flippedX = false; // indicate the compare imageview is flipedX
        //auto logicNO = _tableLogic->viewToLogicSeatNo(i);
        if(seats[i] && nullptr!= _players[i] && _players[i]->isPlaying() && !_players[i]->isQuit()) {
            _tableUI.iCard[i]->setVisible(true);

            if(i > 3) {
//                    if(_tableUI.bCompare[i]->isFlippedX() == false){
//                        _tableUI.bCompare[i]->setFlippedX(true);
                flippedX = true;
//                    }
            }
            _tableUI.bCompare[i]->setVisible(true);
            Vec2 pos = _tableUI.bCompare[i]->getPosition();
            // run action
            int factor = flippedX ? -1 : 1;
            auto moveBy = MoveBy::create(0.5, Vec2(factor*9, 0));
            auto sequence = Sequence::create(moveBy, moveBy->reverse(), nullptr);
            auto repeat = RepeatForever::create(sequence);
            _tableUI.bCompare[i]->runAction(repeat);
        }
    }
}


void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
    if (bVisible) {
        if (!m_Notice) {
            m_Notice = GameNotice::create();
            m_Notice->setLocalZOrder(10000);
            m_Notice->setName("GameNotice");
        }
        m_Notice->openNotic(this, 10000, message, nCount, nType);
    }
}

void GameTableUIV88::onGameNotify(tag_s2c_notify* msg) {
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


        //自己才看得到弹窗消息
        if (msg->nUserID == _tableLogic->getUserBySeatNo(_tableLogic->getMySeatNo())->dwUserID) {
            auto image = dynamic_cast<ImageView*>(getChildByName("poolNotice"));

            //已经在显示，要显示另一个时跳过
            if (image->isVisible()) {
                return;
            }

            auto label1 = dynamic_cast<Label*>(image->getChildByName("noticeContentLabel"));

            auto label3 = dynamic_cast<LabelAtlas*>(image->getChildByName("noticeMoneyLabel"));

            if (msg->bGameID == 1) {
                sprintf(str, "%s %s", GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName));
                label1->setString(ForceGBKToUtf8(str));

                sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName));
            } else if (msg->bGameID == 2) {
                sprintf(str, "%s %s", GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName));
                label1->setString(ForceGBKToUtf8(str));

                sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName));
            } else if (msg->bGameID == 3) {
                sprintf(str, "%s %s", GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName));
                label1->setString(ForceGBKToUtf8(str));

                sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName));
            } else {
                return;
            }

            sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName));
            auto label2 = dynamic_cast<Label*>(image->getChildByName("noticeNameLabel"));
            label2->setString(ForceGBKToUtf8(str));

            sprintf(str, "%s", Tools::addComma(msg->i64WinMoney).c_str());
            label3->setString(str);

            //展示
            image->runAction(Sequence::create(Show::create(), ScaleTo::create(0.3f, 1.0f), DelayTime::create(2.2f), ScaleTo::create(0.2, 0.1f), Hide::create(), nullptr));
        }
    } else {
        if (msg->bGameID == 1) {
            sprintf(str, GetText("NN_Catch_Bull_Notify"), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName), msg->bDeskID + 1, ForceGBKToUtf8(msg->cNickName), msg->i64WinMoney);
        } else if (msg->bGameID == 2) {
            sprintf(str, GetText("NN_Catch_Bull_Notify"), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName), msg->bDeskID + 1, ForceGBKToUtf8(msg->cNickName), msg->i64WinMoney);
        } else if (msg->bGameID == 3) {
            sprintf(str, GetText("NN_Catch_Bull_Notify"), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName), msg->bDeskID + 1, ForceGBKToUtf8(msg->cNickName), msg->i64WinMoney);
        } else {
            return;
        }

        showNotice(str, 1, 0);
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

/************************分割??********************************/
void GameTableUIV88::inflateLayout() {
    auto bg = Sprite::create(GAME_BG_COMMON_V66);
    bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    bg->setPosition(WIN_SIZE / 2);
    //bg->setScaleY(WIN_SIZE.height / bg->getContentSize().height);
    addChild(bg);

    //add dashboard
	_dashboard = DashboardV88::create(this);
	_dashboard->setAnchorPoint(Vec2(1, 0));
	_dashboard->setPosition(Vec2(WIN_SIZE.width*1.07, 0));
	_dashboard->setVisible(false);
	addChild(_dashboard, Dashboard_Zorder);



    auto root = CSLoader::createNode("Games/12101106/Node_Root_V66.csb");
    root->setAnchorPoint(Vec2(0.5f, 0.5f));
    root->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
    this->addChild(root, 2);

    _tableUI.next_game = dynamic_cast<ImageView*>(root->getChildByName("next_game"));
    _tableUI.next_game->setVisible(false);

    //计时??

	m_clockObj = TimerObject::create();
    if (m_clockObj) {
        m_clockObj->setPosition(Vec2(WIN_SIZE.width * 0.73f, WIN_SIZE.width/2));
        addChild(m_clockObj, VS_NODE_ZORDER - 10);
    }

    //_tableUI.ImageView_TimerBg = dynamic_cast<ImageView*>(root->getChildByName("timerbg_0"));
    //_tableUI.ImageView_TimerBg->setPosition(Vec2(WIN_SIZE.width * 0.23f, 0));
    //_tableUI.TextAtlas_TimerText = dynamic_cast<TextAtlas*>(_tableUI.ImageView_TimerBg->getChildByName("timer"));
    //_tableUI.ImageView_TimerBg->setVisible(false);


    auto compareImageNode = CSLoader::createNode("Games/12101106/Node_Compare_Image_V66.csb");
    if (compareImageNode) {
        compareImageNode->setPosition(WIN_SIZE / 2);
        addChild(compareImageNode, VS_NODE_ZORDER + 1);
        //
        //比牌头框
        for (int i = 0; i < 2; i++) {
            std::string head = StringUtils::format("head_Image_%d", i);
            _tableUI.head_image[i] = dynamic_cast<Sprite*>(compareImageNode->getChildByName(head));
            _tableUI.head_image[i]->setVisible(false);
            _headImagePostion[i] = _tableUI.head_image[i]->getPosition();
        }
    }



    char tmp[100] = { 0 };

    _nodeCardPosition[0] = Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.25f);
    _nodeCardPosition[1] = Vec2(WIN_SIZE.width - 250, WIN_SIZE.height * 0.40f);
    _nodeCardPosition[2] = Vec2(WIN_SIZE.width - 250, WIN_SIZE.height * 0.67f);
    _nodeCardPosition[3] = Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.7f);
    _nodeCardPosition[4] = Vec2(250, WIN_SIZE.height * 0.67f);
    _nodeCardPosition[5] = Vec2(250, WIN_SIZE.height * 0.40f);


    //牌相关节点信息
    for (int i = 0; i < PLAY_COUNT; i++) {
        sprintf(tmp, "FileNode_%d", i);
        _tableUI.Node_Card[i] = dynamic_cast<Node*>(root->getChildByName(tmp));
        _tableUI.Node_Card[i]->setPosition(_nodeCardPosition[i] - WIN_SIZE/2);
        _tableUI.Node_Card[i]->setVisible(true);
    }

    for (int i = 0; i < PLAY_COUNT; i++) {
        auto node = _tableUI.Node_Card[i];
        if (node == nullptr) {
            continue;
        }

        _tableUI.iCard[i] = dynamic_cast<ImageView*>(node->getChildByName("Image_card"));
        _tableUI.iCard[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::compareClickCallback, this));
        _tableUI.iCard[i]->setOpacity(0);
        _tableUI.iCard[i]->setPosition(Vec2::ZERO);
        _tableUI.iCard[i]->setVisible(true);
        _tableUI.iCard[i]->setTouchEnabled(true);

        _tableUI.ReadySign[i] = dynamic_cast<ImageView*>(node->getChildByName("ready"));
        _tableUI.ReadySign[i]->setPosition(Vec2::ZERO);
        _tableUI.ReadySign[i]->setVisible(false);

        _tableUI.Nbet[i] = dynamic_cast<Node*>(node->getChildByName("Node"));
        _tableUI.Nbet[i]->setPosition(Vec2::ZERO);
        _tableUI.Nbet[i]->setVisible(false);

        _tableUI.WatchSign[i] = dynamic_cast<ImageView*>(node->getChildByName("card_watch"));
        _tableUI.WatchSign[i]->setPosition(Vec2::ZERO);
        _tableUI.WatchSign[i]->setVisible(false);

        _tableUI.GiveUpSign[i] = dynamic_cast<ImageView*>(node->getChildByName("card_giveup"));
        _tableUI.GiveUpSign[i]->setPosition(Vec2::ZERO);
        _tableUI.GiveUpSign[i]->setVisible(false);
    }

    _tableUI.lOutline = dynamic_cast<Node*>(root->getChildByName("Node_Poker"));
    auto layout_buttons = dynamic_cast<Node*>(_tableUI.lOutline->getChildByName("Node_Player"));
    if (layout_buttons) {
        layout_buttons->setLocalZOrder(VS_NODE_ZORDER - 25);
    }
    //准备按钮
	_tableUI.bReady = AUIButton::create("v66/button/yellow1/BG.png", "v66/button/yellow1/BG.png", "v66/button/yellow1/BG.png");
	addChild(_tableUI.bReady, 98);
	_tableUI.bReady->setName("btn_start");
	_tableUI.bReady->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 110));
	_tableUI.bReady->setPressedActionEnabled(true);
	_tableUI.bReady->setTitleOffset(Vec2(4, 4));
	_tableUI.bReady->setTitle("v66/button/yellow1/6.png");
	_tableUI.bReady->setEnabled(true);
	_tableUI.bReady->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::menuClickCallback, this));

    auto baseNode = dynamic_cast<Node*>(_tableUI.lOutline->getChildByName("FileNode_Base"));
    if (baseNode) {
        _tableUI.lNote = dynamic_cast<Text*>(baseNode->getChildByName("Text"));
        baseNode->setPosition(Vec2(WIN_SIZE.width / 2 - 100, WIN_SIZE.height / 2 - 40));
    }

    _tableUI.lTotalNode = CSLoader::createNode(NODE_TOTAL_BET_CSB);
    if (_tableUI.lTotalNode) {
        _tableUI.lTotalNode->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.37f));
        _tableUI.lTotalNode->setVisible(false);
        addChild(_tableUI.lTotalNode, VS_NODE_ZORDER - 9);
        _tableUI.lTotalNote = dynamic_cast<Text*>(_tableUI.lTotalNode->getChildByName("Text"));
        if (_tableUI.lTotalNote) {
            _tableUI.lTotalNote->setPosition(Vec2::ZERO);
        }
    }


    auto loopNode = dynamic_cast<Node*>(_tableUI.lOutline->getChildByName("FileNode_Loop"));
    if (loopNode) {
        _tableUI.lOperateCount = dynamic_cast<Text*>(loopNode->getChildByName("Text"));
        loopNode->setPosition(Vec2(WIN_SIZE.width / 2 - 100, WIN_SIZE.height / 2 - 100));
    }
    updateOperateCount(0, 0);

    _seatPosition[0] = Vec2(-WIN_SIZE.width / 2 + 10, -WIN_SIZE.height / 2 + 10);
    _seatPosition[1] = Vec2(WIN_SIZE.width / 2 - 10, -WIN_SIZE.height * (0.5 - 0.40));
    _seatPosition[2] = Vec2(WIN_SIZE.width / 2 - 10, WIN_SIZE.height * (0.5 - 0.33));
    _seatPosition[3] = Vec2(0, WIN_SIZE.height / 2 - 10);
    _seatPosition[4] = Vec2(-WIN_SIZE.width / 2 + 10, WIN_SIZE.height * (0.5 - 0.33));
    _seatPosition[5] = Vec2(-WIN_SIZE.width / 2 + 10, -WIN_SIZE.height * (0.5 - 0.40));


    for (int i = 0; i < PLAY_COUNT; i++) {
        sprintf(tmp, "image_compare_%d", i);
        _tableUI.bCompare[i] = dynamic_cast<ImageView*>(layout_buttons->getChildByName(tmp));
        _tableUI.bCompare[i]->setVisible(false);
        _tableUI.bCompare[i]->setPosition(_tableUI.Node_Card[i]->getPosition() + ((i == 4 || i == 5) ? Vec2(100, 0) : Vec2(-100, 0)));

        sprintf(tmp, "btn_sit_pos_%d", i);
        _tableUI.bSeat[i] = dynamic_cast<Sprite*>(layout_buttons->getChildByName(tmp));
		_tableUI.bSeat[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        //_tableUI.bSeat[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::seatClickCallback, this));
       // _tableUI.bSeat[i]->setAnchorPoint(i == 0 ? Vec2::ANCHOR_BOTTOM_LEFT : (i == 1 || i == 2) ? Vec2::ANCHOR_MIDDLE_RIGHT : (i == 3) ? Vec2::ANCHOR_MIDDLE_TOP : Vec2::ANCHOR_MIDDLE_LEFT);
        _tableUI.bSeat[i]->setVisible(false);
        _tableUI.bSeat[i]->setLocalZOrder(VS_NODE_ZORDER - 30);

        sprintf(tmp, "image_dealer_%d", i);
		_tableUI.iDealer[i] = dynamic_cast<ImageView*>(layout_buttons->getChildByName(tmp));
		_tableUI.iDealer[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        Size dealerSize = _tableUI.iDealer[i]->getContentSize();
		//_tableUI.iDealer[i]->setPosition(_tableUI.bSeat[i]->getPosition() + root->getPosition() - Vec2(_tableUI.bSeat[i]->getContentSize().width*0.5, _tableUI.bSeat[i]->getContentSize().width*0.2));
        //_tableUI.iDealer[i]->setPosition(_seatPosition[i] + (i == 0 ? Vec2(_tableUI.bSeat[i]->getContentSize().width, _tableUI.bSeat[i]->getContentSize().height) :
        //                                 i == 3 ? Vec2(_tableUI.bSeat[i]->getContentSize().width / 2, 0) :
        //                                 (i == 1 || i == 2) ? Vec2(0, _tableUI.bSeat[i]->getContentSize().height / 2) :
        //                                 (i == 4 || i == 5) ? Vec2(_tableUI.bSeat[i]->getContentSize().width, _tableUI.bSeat[i]->getContentSize().height / 2) : Vec2::ZERO));
        _tableUI.iDealer[i]->setLocalZOrder(VS_NODE_ZORDER - 29);
        _tableUI.iDealer[i]->setVisible(false);
    }


    //胜利结算
    _winNode = CSLoader::createNode("v88/animation/settle/shengli/shengli.csb");
    _winNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _winNode->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height*0.5f));
	/*   Sprite* winSprite = dynamic_cast<Sprite*>(_winNode->getChildByName("tietu_2"));
	   if (winSprite) {
	   winSprite->setVisible(false);
	   }*/
    addChild(_winNode, 200);
    _winNode->setVisible(false);

    _winAction = CSLoader::createTimeline("v88/animation/settle/shengli/shengli.csb");
    _winNode->runAction(_winAction);

    //失败结算
    _loseNode = CSLoader::createNode("v88/animation/settle/shibai/shibai.csb");
    _loseNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _loseNode->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height*0.5f));
	/*   Sprite* loseSprite = dynamic_cast<Sprite*>(_loseNode->getChildByName("tietu2"));
	   if (loseSprite) {
	   loseSprite->setVisible(false);
	   }*/
    addChild(_loseNode, 200);
    _loseNode->setVisible(false);

    _loseAction = CSLoader::createTimeline("v88/animation/settle/shibai/shibai.csb");
    _loseNode->runAction(_loseAction);

    //结算??
    _endNode = CSLoader::createNode("Games/12101106/endBox_V66.csb");
//    _endNode->setZOrder(EndBox_Zorder);
    _endNode->setVisible(false);
    _endNode->getChildByName("Node_win")->setVisible(false);
	_endNode->getChildByName("TitleNode")->setVisible(false);
//    _endNode->getChildByName("settleBg")->setVisible(false);
    _endNode->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 20.0f));
    addChild(_endNode, 199);
    
    auto node = _endNode->getChildByName("Node_win");
    if (node) {
        m_myTipImage = dynamic_cast<ImageView*>(node->getChildByName("Image_1"));
        if (m_myTipImage) {
            m_myTipImage->setVisible(false);
        }
    }

    //    Director::getInstance()->getTextureCache()->addImageAsync("goldenflower/table/cards.pvr.czz", CC_CALLBACK_1(GameTableUIV88::loadingTextureCallback, this, "goldenflower/table/cards.plist"));
}

//    void GameTableUIV88::loadingTextureCallback(Texture2D* textureData, std::string plist)
//    {
//        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist , textureData);
//
//    }

bool GameTableUIV88::seatNoIsOk(BYTE seatNo) {
    return (seatNo < PLAY_COUNT && seatNo >= 0);
}

void GameTableUIV88::getReady() {
    if(_tableUI.bReady->isVisible()) {
        _tableLogic->sendStandUp();
    }
}

Vec2 GameTableUIV88::getCardPosition(BYTE seatNo) {
    return _tableUI.iCard[seatNo]->getPosition();
}

void    GameTableUIV88::tipCallback() {
    if (tipMsg->bIsExit == 1) {
        //准备关闭掉
        if (_tableUI.bReady) {
            _tableUI.bReady->setVisible(false);
        }

        auto layer = GamePromptLayer::create();
//        layer->setCloseExitBtn();
//             char str[128] = { 0 };
//             int sec = 3;
//             sprintf(str, "金币数额低于%lld，%ds后离开游戏！", tipMsg->i64Money, sec);
//             layer->showPrompt(GBKToUtf8(str));
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
            if (RoomLogic()->connected()) {
                _tableLogic->sendStandUp();
            } else {
                leaveTheGame();
            }
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
        if (RoomLogic()->connected()) {
            _tableLogic->sendStandUp();
        } else {
            leaveTheGame();
        }
        return;
    }

//         char str[128] = { 0 };
//         iTag -= 1;
//         sprintf(str, "金币数额低于%lld，%ds后离开游戏！", m_i64TipMoney, iTag);
//         m_TextTip->setText(GBKToUtf8(str));
//         m_TextTip->setTag(iTag);
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


