//#include "../../TTNN/Classes/GameTable/BRNNSetLayer.h"
//#include "../../TTNN/Classes/GameTable/BRNNOtherPlayer.h"
//#include "../../TTNN/Classes/GameTable/BRNNSettlementLayer.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNLobby/v66/ExtraReward.h"

#include "HNLobby/FontConfig.h"
#include "HNLobby/GamePlatform.h"
#include "HNLobby/GamePrompt.h"

#include "BJLGameTableUIV88.h"
#include "BJLGameTableLogic.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/StateManager.h"
#include "Tool/Tools.h"
#include "Sound/HNAudioEngine.h"
#include "../v88/BRNNOtherPlayerV88.h"
#include "BJLNoticeV88.h"
//#include "../v88/BRNNSetLayerV88.h"
#include "../v88/BRNNSettlementLayerV88.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"

namespace BJL {
#define  ZORDER_GAMETABLE	40
#define  ZORDER_DASHBOARD	50			//控制面板
#define  ZORDER_CARDBOARD   60
#define  ZORDER_MAX			200


#define	 RECORD_COUNT		14
#define  POINT_COUNT		19

//font
static const char* FONT_PATH = FONT_TTF_FZCY;

//CSB
static const char* LIST_CSB = "csb/v66/BJL/applyBankerList_v88.csb";
static const char* DASH_BROAD_CSB = "csb/v66/BJL/dashbroad_v88.csb";
static const char* GAME_BANKER_CSB = "csb/v66/BJL/gamebanker_v88.csb";
static const char* GAME_TABLE_CSB = "csb/v66/BJL/gametable_v88.csb";
static const char* GAME_RENDCHART_CSB = "csb/v66/BJL/rendChart_v88.csb";
//music
static const char* BJL_GAME_MUSIC_LOSE			= "v66/music/audio_lose_v88.mp3";
static const char* BJL_GAME_MUSIC_WIN			= "v66/music/audio_win_v88.mp3";

static const char* BJL_GAME_MUSIC_TIP_BET		= "v66/music/BJL/tipBet.mp3";
static const char* BJL_GAME_MUSIC_TIP_STOP_BET	= "v66/music/BJL/tipStopBet.mp3";
static const char* BJL_GAME_MUSIC_BET			= "v66/music/BJL/down_coin.mp3";

static const char* BJL_GAME_MUSIC_BET_BACK		= "v66/music/NN/betBackGround.mp3";

static const char* GAME_SOUND_BTN				= "music/btnEffect/button_click_sound.mp3";

//res
//static const char* NN_TOOL_BAR_PATH            = "Games/12100004/xialaanniu/%s";
//static const char* NN_TOOL_BAR_PATH_BTN    = "Games/12100004/%s";

//static const char* BJL_BG									= "v66/BJL/game/BJLStudio/Layergame/deskBg.png";
static const char* BJL_BET_AREA_PATH				= "v66/BJL/betArea%d.png";
static const char* RECORD_FLAG						= "v66/BJL/flag.png";
//static const char* RECORD_POINT_BG					= "v66/BJL/game/BJLStudio/Layergame/pointRecordBg.png";

static const char* BET_BTN_PATH					= "v66/chouma/%d.png";
static const char* CARD_RES						= "v66/common/card_Common/cards.plist";


static const char*	BJL_NOSEAT_TITLE			= "v66/common/button/1.png";

static const char*	BJL_MONEY_TOATAL_BG			= "v66/BJL/leijishuying.png";

#define TOOLBALL_OFFSET_Y_V99 8.0f

GameTableUIV88::~GameTableUIV88() {
//	this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));

    _logic->stop();

    CC_SAFE_DELETE(_logic);
    CC_SAFE_DELETE(_resultCard);
    CC_SAFE_DELETE(_gameView);
    CC_SAFE_DELETE(_dealerList);

    //delete image
    for (int i = 0; i < NUMOFBETAREA; i++) {
        if (m_vImage.at(i)) {
            CC_SAFE_DELETE(m_vImage.at(i));
        }
    }

    for (int i = 0; i < vConinSprite.size(); i++) {
        auto pData = vConinSprite.at(i)->getUserData();
        if (pData) {
            BET_TYPEV88* data = (BET_TYPEV88*)pData;
            CC_SAFE_DELETE(data);
        }
    }

    //移除
    char str[64] = { 0 };
    for (int i = 7; i < 12; i++) {
        sprintf(str, BET_BTN_PATH, i);
        Director::getInstance()->getTextureCache()->removeTextureForKey(str);
    }

    m_Notice = nullptr;

	unschedule(schedule_selector(GameTableUIV88::animationUpdate));
}

GameTableUIV88::GameTableUIV88()
    :_currentDealerLimit(0)
    , m_Notice(nullptr)
    , _resultCard(nullptr)
    , m_bNextOut(false)
    , m_bAutoBet(false)
    , _t(0)
    , m_iRecordIndex(0) {
    m_vPrevBetArea1.clear();
    m_vPrevBetMoney1.clear();
    m_vPrevBetArea2.clear();
    m_vPrevBetMoney2.clear();
}

HNGameUIBase* GameTableUIV88::create(BYTE bDeskIndex, bool bAutoCreate) {
	GameTableUIV88* ptr = new GameTableUIV88();
    if (nullptr != ptr) {
        ptr->init(bDeskIndex,bAutoCreate);
        ptr->autorelease();
        return ptr;
    }
    CC_SAFE_DELETE(ptr);
    return nullptr;
}

bool GameTableUIV88::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }
    m_DeskIndex = bDeskIndex;
    m_AutoCreate = bAutoCreate;

    return true;
}

bool GameTableUIV88::initUI() {
    auto winSize = Director::getInstance()->getWinSize();

    //庄家信息初始化
    auto bankerLoader = CSLoader::createNode(GAME_BANKER_CSB);
    bankerLoader->setPosition(Vec2(WIN_SIZE.width / 2 - 420, WIN_SIZE.height - 105));
    addChild(bankerLoader, ZORDER_GAMETABLE - 1);

    auto bankerLayout = (Layout*)bankerLoader->getChildByName("Panel_1");
    _gameView->Label_BankNum = (Text*)bankerLayout->getChildByName("Text_BankNum");
    _gameView->Label_BankNum->setString("");
    _gameView->Label_BankToatal = (Text*)bankerLayout->getChildByName("Text_ToaTalNum");
    _gameView->Label_BankToatal->setString("");


    _gameView->ImageView_HeadBg = (ImageView*)bankerLayout->getChildByName("dealerbg_0");
	_gameView->ImageView_HeadBg->setVisible(false);
	Vec2 headPos = _gameView->ImageView_HeadBg->getPosition();
	Vec2 pos = Vec2(headPos.x, headPos.y + 15);

	_gameView->ImageView_BankHead = UserHead::create(HEAD_FRAME_IN_GAME_V66);
	_gameView->ImageView_BankHead->setClipUserHeadOffset(Vec2(0, 15));
	_gameView->ImageView_BankHead->setScale(0.8);
	_gameView->ImageView_BankHead->setPosition(pos);
	bankerLayout->addChild(_gameView->ImageView_BankHead);
	
	_gameView->Label_BankName = Text::create();
	_gameView->Label_BankName->setFontSize(18);
	_gameView->Label_BankName->setFontName(FONT_TTF_MSYH);
	_gameView->Label_BankName->setPosition(Vec2(pos.x, pos.y - 30));
	bankerLayout->addChild(_gameView->Label_BankName);

	_gameView->Label_BankMoney = Text::create();
	_gameView->Label_BankMoney->setFontSize(18);
	_gameView->Label_BankMoney->setFontName(FONT_TTF_FZCY);
	_gameView->Label_BankMoney->setPosition(Vec2(pos.x, pos.y - 53));
	_gameView->Label_BankMoney->setColor(FONT_MONEY_COLOR_V66);
	bankerLayout->addChild(_gameView->Label_BankMoney);

    //点记录bg
    //auto pointBg = ImageView::create(RECORD_POINT_BG);
    //addChild(pointBg, ZORDER_CARDBOARD - 1);
    //pointBg->setAnchorPoint(Vec2(1, 1));
    //pointBg->setPosition(Vec2(WIN_SIZE.width - 5, WIN_SIZE.height));

    //_gameView->Image_PointRecordBg = pointBg;

    return true;
}

void GameTableUIV88::createBetAreaUI() {
    //csb
    auto gametable_loader = CSLoader::createNode(GAME_TABLE_CSB);

	auto bgSpr = Sprite::create(GAME_BG_HEGUAN_V66);

    auto gametable_layout = (Layout*)gametable_loader->getChildByName("Panel_50");
    gametable_layout->setAnchorPoint(Vec2(0.5f, 0.5f));
	auto bgY = WIN_SIZE.height > 1280 ? bgSpr->getContentSize().height - WIN_SIZE.height + WIN_SIZE.height*0.5f : WIN_SIZE.height *0.43f;
	gametable_layout->setPosition(Vec2(WIN_SIZE.width / 2, bgY));
    addChild(gametable_loader, ZORDER_GAMETABLE);

    _gameView->gametable_layout = gametable_layout;

    //下注区域
    char str[50];
    for (BYTE i = 0; i < 8; i++) {
        sprintf(str, "area__%d", i);
        _gameView->Button_BetArea[i] = (Button*)gametable_layout->getChildByName(str);
		_gameView->Button_BetArea[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onBetAreaClickCallback, this));
        _gameView->Button_BetArea[i]->setSwallowTouches(false);

        sprintf(str, "betbg__%d", i);
        _gameView->ImageView_BetWarning[i] = (ImageView*)gametable_layout->getChildByName(str);

        sprintf(str, "winbg__%d", i);
        _gameView->ImageView_BetWin[i] = (ImageView*)gametable_layout->getChildByName(str);

        sprintf(str, "allmoney__%d", i);
        _gameView->TextAtlas_TotalBet[i] = (TextAtlas*)gametable_layout->getChildByName(str);
        _gameView->TextAtlas_TotalBet[i]->setString("0");

        sprintf(str, "mymoney___%d", i);
        _gameView->TextAtlas_UserBet[i] = (TextAtlas*)gametable_layout->getChildByName(str);
        _gameView->TextAtlas_UserBet[i]->setString("0");

        Vec2 leftTopPoint = _gameView->Button_BetArea[i]->getPosition();
        leftTopPoint.x -= _gameView->Button_BetArea[i]->getContentSize().width / 2;
        leftTopPoint.y += _gameView->Button_BetArea[i]->getContentSize().height / 2;

        //初始化下注区域
        _gameView->pnoteview[i].init(_gameView->Button_BetArea[i], _gameView->TextAtlas_TotalBet[i], _gameView->TextAtlas_UserBet[i], i, leftTopPoint);
    }
    getAreaBetPixData();

    auto clocknode = _gameView->gametable_layout->getChildByName("clockNode");
    auto clocKpos = clocknode->getPosition();

    //auto clockSprite = Sprite::create(BRNN_TABLERES_JISHIQI_V99);
    //clockSprite->setPosition(clocKpos);
    //clockSprite->setVisible(false);
    //_gameView->gametable_layout->addChild(clockSprite);
    //auto size = clockSprite->getContentSize();

    _gameView->m_timerObj = TimerObject::create();
    if (_gameView->m_timerObj) {
        _gameView->gametable_layout->addChild(_gameView->m_timerObj, 66);
        _gameView->m_timerObj->setPosition(clocKpos);
        _gameView->m_timerObj->setTimeCallBack([&]() {
            hideXiaZhuBTN();
            hideClock();
        });
    }
    
//    _gameView->ImageView_TimerBg = ImageView::create("hallimg/niuniu/clock.png");
//    _gameView->ImageView_TimerBg->setScale(0.7);
//    _gameView->ImageView_TimerBg->setAnchorPoint(Vec2(0.5, 0.5));
//    _gameView->ImageView_TimerBg->setVisible(false);

//    _gameView->TextAtlas_TimerText = LabelAtlas::create("00", "v66/fontpng/atlas_yellow.png", 32, 40, '0');
//    _gameView->TextAtlas_TimerText->setScale(0.6);
//    _gameView->TextAtlas_TimerText->setVisible(false);
//    _gameView->TextAtlas_TimerText->setAnchorPoint(Vec2(0.5, 0.5));
//    _gameView->TextAtlas_TimerText->setPosition(Vec2(_gameView->ImageView_TimerBg->getPosition().x + 2.0f, _gameView->ImageView_TimerBg->getPosition().y + 2.0f));
//    _gameView->gametable_layout->addChild(_gameView->TextAtlas_TimerText, 67);

//    //特效
//    _gameView->m_pClock = CSLoader::createNode("v66/animation/common/alarm/alarm.csb");
//    _gameView->m_pClock->setVisible(false);
//    _gameView->m_pClock->setScale(0.7);
//    _gameView->m_pClock->setPosition(Vec2(_gameView->m_pClock->getPosition().x + 4.0f, _gameView->m_pClock->getPosition().y - 4.0f));
//    _gameView->gametable_layout->addChild(_gameView->m_pClock, 68);
//
//    _gameView->m_pClockAnimation = CSLoader::createTimeline("v66/animation/common/alarm/alarm.csb");
//    _gameView->m_pClock->runAction(_gameView->m_pClockAnimation);

    //状态提示图片
    _gameView->ImageView_betHint = (ImageView*)gametable_loader->getChildByName("current_state__0");
	_gameView->ImageView_betHint->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2  + 40));
    _gameView->ImageView_WaitHint = (ImageView*)gametable_loader->getChildByName("current_state__1");
	_gameView->ImageView_WaitHint->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 40));
    _gameView->ImageView_betHint->setVisible(false);
    _gameView->ImageView_WaitHint->setVisible(false);
}

void GameTableUIV88::createResultRecordUI() {
	auto root = CSLoader::createNode(GAME_RENDCHART_CSB);
	auto layout = static_cast<Layout*>(root->getChildByName("Panel_1"));
	root->setPosition(Vec2(WIN_SIZE.width - layout->getContentSize().width *0.5f, WIN_SIZE.height));
	this->addChild(root);
	auto recordBg = (ImageView*)layout->getChildByName("Image_RecordBg");
    Size recordSize = recordBg->getContentSize();

    float fStartX = recordSize.width / 2 - 174.7;
    float fStartY = recordSize.height / 2 + 37.5;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < RECORD_COUNT; j++) {
            auto image = ImageView::create(RECORD_FLAG);
            recordBg->addChild(image);
            image->setPosition(Vec2(fStartX + j*25.0, fStartY - i * 31));
            image->setVisible(false);
            _vRecordYes.pushBack(image);
        }
    }
    for (int i = 0; i < 3; i++) {
        auto label = Label::createWithTTF("0%", FONT_PATH, 16);
        recordBg->addChild(label);
        label->setPosition(Vec2(recordSize.width / 2 + 186, recordSize.height / 2 + 38 - i * 32));
        label->setVisible(false);

        _vRecordNum.pushBack(label);
    }

	auto prevRecordBtn = (ImageView*)layout->getChildByName("Image_LeftArrow");
    prevRecordBtn->setVisible(false);
    prevRecordBtn->setTouchEnabled(true);
    prevRecordBtn->setTag(-1);
	prevRecordBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::recordBtnClickCallback, this));
    _gameView->Image_LeftArrow = prevRecordBtn;

	auto nextRecordBtn = (ImageView*)layout->getChildByName("Image_RightArrow");
    nextRecordBtn->setVisible(false);
    nextRecordBtn->setTouchEnabled(true);
    nextRecordBtn->setTag(1);
	nextRecordBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::recordBtnClickCallback, this));
    _gameView->Image_RightArrow = nextRecordBtn;
}

void GameTableUIV88::createPointRecordUI() {
    float fStartX = 51;
    float fStartY = 63;
    float width = 21.25;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < POINT_COUNT; j++) {
            auto label = Label::createWithTTF("0", FONT_PATH, 16);
            _gameView->Image_PointRecordBg->addChild(label);
            label->setPosition(Vec2(fStartX + j*width, fStartY - i*(width+4)));
            label->setVisible(false);

            _vPointNum.pushBack(label);
        }
    }


    vector<Vec2> vPos;
    vPos.push_back(Vec2(108, 123));
	vPos.push_back(Vec2(360, 123));
    vPos.push_back(Vec2(0, 0));
    vPos.push_back(Vec2(108, 151));
	vPos.push_back(Vec2(360, 151));
    vPos.push_back(Vec2(0, 0));
    vPos.push_back(Vec2(108, 95));
    vPos.push_back(Vec2(0, 0));


    for (int i = 0; i < 8; i++) {
        auto label = Label::createWithTTF("0", FONT_PATH, 18);
        _gameView->Image_PointRecordBg->addChild(label);
        label->setAnchorPoint(Vec2(0, 0.5));
        label->setPosition(vPos.at(i));
        label->setVisible(vPos.at(i).x > 0);

        _vCanBetNum.pushBack(label);
    }
}

void GameTableUIV88::createMyControlUI() {
    //自己控制面板
    auto dashbroad_loader = CSLoader::createNode(DASH_BROAD_CSB);

    auto dashbroad_layout = (Layout*)dashbroad_loader->getChildByName("Panel_1");
    dashbroad_layout->setPosition(Vec2(0, 0));
    this->addChild(dashbroad_loader, ZORDER_DASHBOARD);
    //保存下来
    _gameView->dashbroad_layout = dashbroad_layout;

    //bg
    auto dash_bg_imgview = (ImageView*)dashbroad_layout->getChildByName("dashbroad_0");
    _gameView->dash_bg_imgview = dash_bg_imgview;

    //玩家信息
    string nickName = PlatformLogic()->loginResult.nickName;
    auto nameBg = (ImageView*)dashbroad_layout->getChildByName("own_name_bg");
    _ownName = (Text*)nameBg->getChildByName("Text_name");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _ownName->setString(ForceGBKToUtf8(nickName.c_str()));
#else
    _ownName->setString(GBKToUtf8(nickName.c_str()));
#endif
    auto moneyBg = (ImageView*)dashbroad_layout->getChildByName("own_money_bg");
    _ownMoney = (Text*)moneyBg->getChildByName("Text_money");

    //玩家累计
    auto userToatalBg = ImageView::create(BJL_MONEY_TOATAL_BG);
    addChild(userToatalBg, ZORDER_DASHBOARD);
    userToatalBg->setAnchorPoint(Vec2(0, 0.5));
    userToatalBg->setPosition(Vec2(5, 160));

    _gameView->Label_UserToatal = Text::create();
    _gameView->Label_UserToatal->setFontSize(20);
    userToatalBg->addChild(_gameView->Label_UserToatal);
    _gameView->Label_UserToatal->setAnchorPoint(Vec2(0, 0.5));
    _gameView->Label_UserToatal->setPosition(Vec2(95, userToatalBg->getContentSize().height /2));
    _gameView->Label_UserToatal->setString("");


    //下注按钮
    char str[128] = { 0 };
    for (BYTE i = 0; i < CHOUMANUM; i++) {
        sprintf(str, "jettonbtn__%d", i);
        _gameView->Button_Bet[i] = (Button*)dashbroad_layout->getChildByName(str);
		_gameView->Button_Bet[i]->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onBetClickCallback, this));
        _gameView->Button_Bet[i]->setPressedActionEnabled(true);

        sprintf(str, "jettonbg__%d", i);
        _gameView->ImageView_BetBg[i] = (ImageView*)dashbroad_layout->getChildByName(str);
        _gameView->ImageView_BetBg[i]->setVisible(false);
    }

    ///////////////////////
    //重复下注
    _gameView->Button_PrevBet = (Button*)dashbroad_loader->getChildByName("Button_PrevBet");
	_gameView->Button_PrevBet->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onRepeatBetClickCallback, this));
    _gameView->Button_PrevBet->setPressedActionEnabled(true);
    _gameView->Button_PrevBet->setVisible(true);

    //取消重复下注
    _gameView->Button_CancelPrevBet = (Button*)dashbroad_loader->getChildByName("Button_CancelPrevBet");
	_gameView->Button_CancelPrevBet->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onRepeatBetClickCallback, this));
    _gameView->Button_CancelPrevBet->setPressedActionEnabled(true);
    _gameView->Button_CancelPrevBet->setVisible(false);

    //上庄按钮
    _gameView->Button_applyNT = (Button*)dashbroad_loader->getChildByName("Button_Apply");
	_gameView->Button_applyNT->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onApplyNTClickCallback, this));
    _gameView->Button_applyNT->setPressedActionEnabled(true);
    _gameView->Button_applyNT->setVisible(true);

    //上庄列表按钮
    _gameView->Button_getListNT = (Button*)dashbroad_loader->getChildByName("Button_List");
	_gameView->Button_getListNT->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onApplyNTClickCallback, this));
    _gameView->Button_getListNT->setPressedActionEnabled(true);
    _gameView->Button_getListNT->setVisible(false);

    /////////////////list
    auto list_loader = CSLoader::createNode(LIST_CSB);
    auto btnPos = _gameView->Button_applyNT->getPosition();
    _gameView->dashbroad_layout->addChild(list_loader, ZORDER_DASHBOARD + 1);
    //上庄列表
    _gameView->ImageView_ListBg = (ImageView*)list_loader->getChildByName("Image_ListBg");
    _gameView->ImageView_ListBg->setVisible(false);
    _gameView->ImageView_ListBg->setUserData((void*)false);
    list_loader->setPosition(Vec2(btnPos.x, btnPos.y + _gameView->Button_applyNT->getContentSize().height / 2.0f + _gameView->ImageView_ListBg ->getContentSize().height *0.5f+ 10));

    _gameView->ListView_NTList = (ListView*)_gameView->ImageView_ListBg->getChildByName("ListView_dealerList");
    _gameView->ImageView_ListItem = (ImageView*)_gameView->ListView_NTList->getChildByName("Image_item");
    _gameView->ImageView_ListItem->setLocalZOrder(100);

    //初始化上庄列表控制器
    _dealerList->initDealerList(_gameView->ListView_NTList, _gameView->ImageView_ListItem);

    //下庄按钮
    _gameView->Button_applyNoNT = (Button*)_gameView->ImageView_ListBg->getChildByName("Button_ApplyDown");
	_gameView->Button_applyNoNT->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onApplyNTClickCallback, this));
    _gameView->Button_applyNoNT->setPressedActionEnabled(true);
    _gameView->Button_applyNoNT->setVisible(false);

    //无座按钮/////////
    _gameView->Button_NoSeat = AUIButton::create("v66/common/button/1.png", "v66/common/button/1_1.png", "v66/common/button/1.png");
    addChild(_gameView->Button_NoSeat, ZORDER_GAMETABLE - 1);
    //_gameView->Button_NoSeat->setTitle("v66/BJL/game/BJLStudio/Layergame/titleImage.png");
	_gameView->Button_NoSeat->setTitleOffset(Vec2(4, 4));
    _gameView->Button_NoSeat->setAnchorPoint(Vec2(0.5, 0.5));
    _gameView->Button_NoSeat->setPosition(Vec2(WIN_SIZE.width - 95, WIN_SIZE.height - 200));
    _gameView->Button_NoSeat->setPressedActionEnabled(true);
    _gameView->Button_NoSeat->setScale(0.8);
	_gameView->Button_NoSeat->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onNoSeatClickCallback, this));

	//帮助
	//_gameView->Button_help = AUIButton::create("v66/common/button/2.png", "v66/common/button/2_1.png", "v66/common/button/2.png");
	//addChild(_gameView->Button_help, ZORDER_GAMETABLE - 1);
	////_gameView->Button_NoSeat->setTitle("v66/BJL/game/BJLStudio/Layergame/titleImage.png");
	//_gameView->Button_help->setTitleOffset(Vec2(4, 4));
	//_gameView->Button_help->setAnchorPoint(Vec2(0.5, 0.5));
	//_gameView->Button_help->setPosition(Vec2(WIN_SIZE.width - 95, WIN_SIZE.height - 300));
	//_gameView->Button_help->setPressedActionEnabled(true);
	//_gameView->Button_help->setScale(0.8);
	//_gameView->Button_help->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::onNoSeatClickCallback, this));
}

void GameTableUIV88::luziscrollview_scroll(cocos2d::Ref* pobj, ui::ScrollView::EventType type) {
}

//void GameTableUIV88::onBtnClick(Ref* pSender, Control::EventType event) {
//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//    Node* btn = (Node*)pSender;
//    switch (btn->getTag()) {
//    case em_toolbarBtn: {
//        showToolBar(true);
//        break;
//    }
//    case em_toolbarSetBtn: {
//        if (this->getChildByName("setLayer")) {
//            (dynamic_cast<BRNNSetLayer*>(this->getChildByName("setLayer")))->closeSet();
//            return;
//        }
//        auto setLayer = BRNNSetLayerV88::create();
//        setLayer->setName("setLayer");
//        setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//        setLayer->showSet(this, ZORDER_MAX + 50);
//
//        showToolBar(false);
//        break;
//    }
//
//    case  em_toolbarHelpBtn: {
//        IShowNoticeMessage(GBKToUtf8("开发中"));
//        showToolBar(false);
//        break;
//    }
//
//    case em_toolbarLeaveBtn: {
//        if (GS_NOTE_STATE == _logic->getGameStatus() || GS_SEND_CARD == _logic->getGameStatus()) {
//            bool hasBet = false;
//            for (int i = 0; i < NUMOFBETAREA; i++) {
//                if (_currentMyZhu[i] > 0) {
//                    hasBet = true;
//                    break;
//                }
//            }
//            if (hasBet || (_logic->getMySeatNo() == _logic->getBankerDeskStation())) {
//                IShowNoticeMessage(GBKToUtf8("正在游戏中，禁止退出。"));
//                return;
//            }
//        }
//
//        _logic->requestStandUp();
//        break;
//    }
//    case em_toolbarNextOutBtn: {
//								   if (btn->getUserData() == (void*)true) {
//									   btn->setUserData((void*)false);
//									   auto spr = static_cast<Sprite*>(_toolbarBg->getChildByTag(em_toolbarNextOutLight));
//									   if (spr)
//										   spr->setTexture("Games/12100004/xialaanniu/27.png");
//									   m_bNextOut = false;
//								   }
//								   else if (btn->getUserData() == (void*)false) {
//									   btn->setUserData((void*)true);
//									   auto spr = static_cast<Sprite*>(_toolbarBg->getChildByTag(em_toolbarNextOutLight));
//									   if (spr)
//										   spr->setTexture("Games/12100004/xialaanniu/28.png");
//									   m_bNextOut = true;
//								   }
//        break;
//    }
//
//    }
//}

void  GameTableUIV88::onTouchEnded(Touch* touch, Event* event) {
    Point point = touch->getLocation();
    point = _tableBg->convertToNodeSpace(point);

//    showToolBar(false);

    //if (_tableBg->getChildByName("recordBg")->getPositionX() == WIN_SIZE.width / 2)
    //{
    //	auto move = MoveTo::create(0.2, Vec2(-455, WIN_SIZE.height / 2));
    //	_tableBg->getChildByName("recordBg")->runAction(Sequence::createWithTwoActions(move, CallFunc::create([this](){
    //		this->getChildByName("outBtnPic")->setVisible(true);
    //		this->getChildByName("outBtn")->setVisible(true);
    //	})));
    //}

    //_gameView->ImageView_ListBg->setVisible(false);
}

void GameTableUIV88::recordBtnClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    auto btn = (ImageView*)pSender;

    if (touchtype == Widget::TouchEventType::BEGAN) {
        btn->runAction(ScaleTo::create(0.2, 1.2));
    }

    if (touchtype != Widget::TouchEventType::ENDED) return;

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
    btn->runAction(ScaleTo::create(0.2, 1.0));

    int tag = btn->getTag();

    int page = _logic->getRecords().size() / RECORD_COUNT;
    int tempIndex = m_iRecordIndex + tag;
    if (tempIndex < 0 && tempIndex > page) {
        return;
    }

    m_iRecordIndex = tempIndex;

    refreshRecord(m_iRecordIndex);
}

void GameTableUIV88::onBetAreaClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return ;
    }

    if (GS_NOTE_STATE != _logic->getGameStatus()) {
        IShowNoticeMessage(GBKToUtf8("未到下注时间!"));
        return;
    }

    if(_betIndex < 0 || _betIndex > (CHOUMANUM - 1)) {
        IShowNoticeMessage(GBKToUtf8("请选择下注筹码!"));
        return;
    }

    //区域
    int iIndex = 0;
    for (int i = 0; i < 8; i++) {
        if (pSender == _gameView->Button_BetArea[i]) {
            iIndex = i;
            break;
        }
    }

    Button* btn = (Button*)pSender;
    Vec2 point = btn->getTouchEndPosition();
    Vec2 pos = btn->convertToNodeSpace(point);
    Size size = _gameView->Button_BetArea[iIndex]->getContentSize();
    Color4B pColor = Tools::getPixelColor(m_vPixData.at(iIndex), pos, size);

    //透明区域
    bool bValid = false;
    for (int i = 0; i < 4; i++) {
        if (pColor.r != 0 || pColor.g != 0 || pColor.b != 0 || pColor.a != 0) {
            bValid = true;
            break;
        }
    }

    //非有效区域跳出
    if (!bValid) {
        return;
    }

    // 请求下注
    _logic->requestBet(_betIndex, iIndex);

    //取消重复下注
    if (m_bAutoBet) {
        m_bAutoBet = false;
        _gameView->Button_PrevBet->setVisible(true);
        _gameView->Button_CancelPrevBet->setVisible(false);
    }
}

void GameTableUIV88::onBetClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (touchtype!=Widget::TouchEventType::ENDED) {
        return;
    }

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    if (_logic->_gameStatus != GS_NOTE_STATE) {
        IShowNoticeMessage(GBKToUtf8("未到下注时间！"));
        return;
    }

    if (_betIndex >= 0 && _betIndex < CHOUMANUM) {
        _gameView->ImageView_BetBg[_betIndex]->stopAllActions();
        _gameView->ImageView_BetBg[_betIndex]->setVisible(false);
    }

    for (int i = 0; i < CHOUMANUM; i++) {
        if (_gameView->Button_Bet[i] == pSender) {
            _betIndex = i;
            break;
        }
    }

    _gameView->ImageView_BetBg[_betIndex]->setVisible(true);
    //_gameView->ImageView_BetBg[_betIndex]->runAction(RepeatForever::create(Blink::create(0.5f, 1)));
}

void GameTableUIV88::onRepeatBetClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) return;

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    if (pSender == _gameView->Button_PrevBet) {
        if (_logic->getMySeatNo() == _logic->getBankerDeskStation()) {
            IShowNoticeMessage(GBKToUtf8("庄家不能做此操作！"));
            return;
        }

//         if (_logic->_gameStatus == GS_NOTE_STATE) {
//             IShowNoticeMessage(GBKToUtf8("下局生效！"));
//         }
        if (m_vPrevBetArea2.size() == 0 && m_vPrevBetMoney2.size() == 0) {
            if (m_vPrevBetArea1.size() == 0 && m_vPrevBetMoney1.size() == 0) {
                IShowNoticeMessage(GBKToUtf8("您上局没有投注！"));
                return;
            }
        }

        m_bAutoBet = true;

        _gameView->Button_PrevBet->setVisible(false);
        _gameView->Button_CancelPrevBet->setVisible(true);
    } else if (pSender == _gameView->Button_CancelPrevBet) {
        m_bAutoBet = false;

        _gameView->Button_PrevBet->setVisible(true);
        _gameView->Button_CancelPrevBet->setVisible(false);
    }
}

void GameTableUIV88::onApplyNTClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    // 申请上庄
    if (pSender == _gameView->Button_applyNT) {
        _logic->requestBeNT(true);
    }

    // 申请下庄
    else if(pSender == _gameView->Button_applyNoNT) {
        _logic->requestBeNT(false);
        _gameView->ImageView_ListBg->setVisible(false);
        _gameView->ImageView_ListBg->setUserData((void*)false);
    }

    if (pSender == _gameView->Button_getListNT) {
        _gameView->ImageView_ListBg->stopAllActions();
        if (_gameView->ImageView_ListBg->getUserData() == (void*)false) {
            _gameView->ImageView_ListBg->setVisible(true);
            _gameView->ImageView_ListBg->setUserData((void*)true);
            _gameView->ImageView_ListBg->runAction(CCSequence::createWithTwoActions(DelayTime::create(10.0f), CallFunc::create([this]() {
                _gameView->ImageView_ListBg->setVisible(false);
                _gameView->ImageView_ListBg->setUserData((void*)false);
            })));
        } else {
            _gameView->ImageView_ListBg->setVisible(false);
            _gameView->ImageView_ListBg->setUserData((void*)false);
        }
    }
}

//帮助
void onHelpBtnCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
{
	if (touchtype != Widget::TouchEventType::ENDED) {
		return;
	}
}

void GameTableUIV88::onNoSeatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype) {
    if (touchtype != Widget::TouchEventType::ENDED) {
        return;
    }

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    auto deskUserList = _logic->getDeskUserList();
    std::vector<UserInfoStruct *> uList;
    deskUserList->findGameUsers(uList);
    std::vector<char *> vecOthers;
    vecOthers.clear();

    int myUserID = _logic->getUserBySeatNo(_logic->getMySeatNo())->dwUserID;
    int bankUserID = 0;
    if (_logic->getBankerDeskStation() && _logic->getBankerDeskStation() != INVALID_DESKSTATION) {
        bankUserID = _logic->getUserBySeatNo(_logic->getBankerDeskStation())->dwUserID;
    }

    for (int i = 0; i < uList.size(); i++) {
        if (uList.at(i)->dwUserID != myUserID && uList.at(i)->dwUserID != bankUserID) {
            vecOthers.push_back(uList.at(i)->nickName);
        }
    }

    if (vecOthers.size() > 0) {
        auto otherLayer = BRNN::OtherPlayerV88::create(vecOthers);
        otherLayer->setTitleImage(BJL_NOSEAT_TITLE);
    }
}

void GameTableUIV88::stopBetAnimation() {
    _betIndex = -1;
    for (BYTE i = 0; i < CHOUMANUM; i++) {
        _gameView->ImageView_BetBg[i]->stopAllActions();
        _gameView->ImageView_BetBg[i]->setVisible(false);
    }
}


void GameTableUIV88::leaveTheGame() {
	//离开游戏的时候恢复游戏背景音乐
	HNAudioEngine::getInstance()->stopBackgroundMusic();

	//退出当前房间
	RoomLogic()->closeRoom();
	//GamePlatform::returnPlatform(ROOMLIST);
	PlatformState* state = PlatformState::create();
	state->setType(GAMELIST);
	StateManager::getInstance()->setState(state);
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

void GameTableUIV88::betAnimationCallback(cocos2d::Node* pSender) {
    void* pData = pSender->getUserData();
    BET_TYPEV88* data = (BET_TYPEV88*)pData;
    auto pos = pSender->getPosition();
    if (data->whichJetton < 7  && data->whichJetton >= 0) {
        CurJtInfo cji;
        int jts[] = {1000, 10000, 100000, 1000000, 5000000, 10000000, 0};
        cji.val=jts[data->whichJetton];
        cji.xpos=data->whichJetton;
        _gameView->pnoteview[data->whichType].dropone(cji,0);
    }
    //CC_SAFE_DELETE(data);
    return ;
}


void GameTableUIV88::createToolBarUI() {
    int zOrder = 70;
    auto toolbar = ToolBarLayer::create();
    if (toolbar) {
        toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
            if (GS_NOTE_STATE == _logic->getGameStatus() || GS_SEND_CARD == _logic->getGameStatus()) {
                bool hasBet = false;
                for (int i = 0; i < NUMOFBETAREA; i++) {
                    if (_currentMyZhu[i] > 0) {
                        hasBet = true;
                        break;
                    }
                }
                if (hasBet || (_logic->getMySeatNo() == _logic->getBankerDeskStation())) {
                    //IShowNoticeMessage(GetText("limit_leave_in_game"));
					GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
					layer->setGamePromptTip(GetText("limit_leave_in_game"));
                    return;
                }
            }
            _logic->requestStandUp();
        });
        toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
            IGameSet* gameSet = GameSetFactory::createWithVersion();
            gameSet->setBgPath(BJL_GAME_MUSIC_BET_BACK);
            gameSet->open(this, Vec2(0, 0), zOrder+5, 0);
        });
        toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
            auto helpLayer = HN::HelpLayer::create();
            helpLayer->setContentOnView("v66/common/help/longhudou.png");
            helpLayer->setName("helpLayer");
            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            helpLayer->setOrignalPosition();
            helpLayer->moveSelfInWithAction();
            addChild(helpLayer, zOrder+5);
        });
        toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
    }
}

//void GameTableUIV88::showToolBar(bool bShow) {
//    if (bShow && 0 == _toolbarBg->getName().compare("0")) {
//        _toolbarBg->setName("1");
//		_toolbarBg->runAction(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)));
//        this->getChildByTag(em_toolbarBtn)->setVisible(false);
//    } else if (!bShow && 0 == _toolbarBg->getName().compare("1")) {
//        _toolbarBg->setName("0");
//		_toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//        CallFunc::create([this]() {
//            this->getChildByTag(em_toolbarBtn)->setVisible(true);
//        })));
//    }
//}

void GameTableUIV88::getAreaBetPixData() {
    char str[64] = { 0 };
    for (int i = 0; i < NUMOFBETAREA; i++) {
        sprintf(str, BJL_BET_AREA_PATH, i);

        Size size = Size(0, 0);
        FileUtils::getInstance()->skipPressedTexture(true);
        auto image = Tools::getImageData(str, size);
        FileUtils::getInstance()->skipPressedTexture(false);
        m_vImage.push_back(image);

        unsigned char* data = image->getData();
        m_vPixData.push_back(data);
    }
}

void GameTableUIV88::loadRes() {
    _gameView=new GameViewV88();
    _dealerList = new T_DEALER_LIST();
    _betIndex = -1;

    char str[64] = { 0 };
    for (int i = 7; i < 12; i++) {
        sprintf(str, BET_BTN_PATH, i);
        Director::getInstance()->getTextureCache()->addImage(str);
    }
}

void GameTableUIV88::initTheNewGameData() {
    //_resultCard->hideCardBorad();
    for (BYTE i = 0; i < NUMOFBETAREA ; ++i) {
        _gameView->TextAtlas_TotalBet[i]->setString("0");
        _gameView->TextAtlas_TotalBet[i]->setVisible(false);   //下注的金币数
        _gameView->TextAtlas_UserBet[i]->setString("0");
        _gameView->TextAtlas_UserBet[i]->setVisible(false);   //自己下注的金币数

        _gameView->ImageView_BetWarning[i]->setVisible(false);   //赢牌区域动画
        _gameView->ImageView_BetWin[i]->setVisible(false);   //赢牌区域动画

        _gameView->pnoteview[i].hide();
    }
	memset(_currentMyZhu, 0, sizeof(_currentMyZhu));
    _betIndex = -1;
    _resultCard->initNewGame();

    // 中间区域
    IShowWinArea(false);
}


//开始游戏
void GameTableUIV88::OnHandleBegin(S_C_GameBegin * pGameBeginData) {
    //重新初始化数据
    initTheNewGameData();
}

void GameTableUIV88::IShowNoticeMessage(const std::string& message) {
    //HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_WARNING);

    auto notice = BJLGameNoticeV88::create(_gameView->gametable_layout, message, _gameView->dash_bg_imgview);
    _gameView->gametable_layout->reorderChild(notice , ZORDER_MAX);
    notice->show();
}

void GameTableUIV88::IApplyNT(bool isNT) {
    _gameView->Button_applyNT->setVisible(!isNT);
    _gameView->Button_getListNT->setVisible(isNT);
    _gameView->Button_applyNoNT->setVisible(isNT);
}

//赢的区域 0=闲 1-庄 2-和
void GameTableUIV88::refreshRecord(int iIdex) {
    if (iIdex != -1) {
        m_iRecordIndex = iIdex;
    }

    auto vLuzi = _logic->getRecords();
    int iPage = vLuzi.size() / RECORD_COUNT;
    int iRemain = vLuzi.size() % RECORD_COUNT;

    if (m_iRecordIndex < 0 || m_iRecordIndex > iPage) {
        return;
    }

    if (vLuzi.size() <= 0) {
        return;
    }


    //标志刷新
    int beginIndex = m_iRecordIndex * RECORD_COUNT;;
    int endIndex = 0;
    if (m_iRecordIndex == iPage) {
        endIndex = beginIndex + iRemain;
    } else {
        endIndex = beginIndex + RECORD_COUNT;
    }

    //重置
    for (int i = 0; i < RECORD_COUNT * 3; i++) {
        _vRecordYes.at(i)->setVisible(false);
    }

    for (int i = beginIndex; i < endIndex; i++) {
        _vRecordYes.at(i - beginIndex)->setVisible(vLuzi.at(i).byWinResult == 1);
        _vRecordYes.at(i - beginIndex + RECORD_COUNT)->setVisible(vLuzi.at(i).byWinResult == 0);
        _vRecordYes.at(i - beginIndex + RECORD_COUNT * 2)->setVisible(vLuzi.at(i).byWinResult == 2);
    }

    //概率刷新
    float toatalCount = vLuzi.size();

    if (toatalCount == 0) {
        for (int i = 0; i < 3; i++) {
            _vRecordNum.at(i)->setString("0%");
            _vRecordNum.at(i)->setVisible(true);
        }
        return;
    }

    float zeroCount = 0;
    float oneCount = 0;
    float twoCount = 0;
    for (int i = 0; i < vLuzi.size(); i++) {
        if (vLuzi.at(i).byWinResult == 0) {
            zeroCount++;
        } else if (vLuzi.at(i).byWinResult == 1) {
            oneCount++;
        } else if (vLuzi.at(i).byWinResult == 2) {
            twoCount++;
        }
    }

    int num1 = oneCount / toatalCount * 100;
    char str[20] = { 0 };
    sprintf(str, "%d%%", num1);
    _vRecordNum.at(0)->setString(str);
    _vRecordNum.at(0)->setVisible(true);

    int num2 = zeroCount / toatalCount * 100;
    sprintf(str, "%d%%", num2);
    _vRecordNum.at(1)->setString(str);
    _vRecordNum.at(1)->setVisible(true);

    int num3 = 100 - num1 - num2;
    sprintf(str, "%d%%", num3);
    _vRecordNum.at(2)->setString(str);
    _vRecordNum.at(2)->setVisible(true);

    //左右箭头按钮
    _gameView->Image_LeftArrow->setVisible(m_iRecordIndex > 0);
    _gameView->Image_RightArrow->setVisible(m_iRecordIndex < iPage);
}

void GameTableUIV88::refreshPoint() {
    vector <LuziData> vData = _logic->getRecords();
//
//    int iDataCount = vData.size();
//
//    if (iDataCount <= 0) {
//        return;
//    }
//
//    //重置
//    for (int i = 0; i < POINT_COUNT * 2; i++) {
//        _vPointNum.at(i)->setVisible(false);
//    }
//
//// 		int page = iDataCount / POINT_COUNT;
//// 		if (iDataCount % POINT_COUNT == 0 && page > 0)//整数页
//// 		{
//// 			page--;
//// 		}
////
//// 		int iBeginPos = (iDataCount <= POINT_COUNT) ? 0 : (page*POINT_COUNT);
//    int iBeginPos = (iDataCount <= POINT_COUNT) ? 0 : (iDataCount - POINT_COUNT);
//
//    int number = 0;
//    char str[16] = {0};
//    for (int i = iBeginPos; i < iDataCount; i++) {
//        _vPointNum.at(i - iBeginPos)->setVisible(true);
//        number = vData.at(i).byZPoint;
//        sprintf(str, "%d", number);
//        _vPointNum.at(i - iBeginPos)->setString(str);
//
//        _vPointNum.at(i - iBeginPos + POINT_COUNT)->setVisible(true);
//        number = vData.at(i).byXpoint;
//        sprintf(str, "%d", number);
//        _vPointNum.at(i - iBeginPos + POINT_COUNT)->setString(str);
//    }
}

void GameTableUIV88::refreshCanBetNum(long long i64CanBetMoney[]) {
    //char str[64] = { 0 };
    //for (int i = 0; i < 8; i++) {
    //    auto moneyStr = Tools::addComma(i64CanBetMoney[i]);
    //    _vCanBetNum.at(i)->setString(moneyStr);
    //}
}


void GameTableUIV88::refreshBankerTotalWinNum(long long i64BankerWin) {
    string s;
    if (i64BankerWin < 0) {
        string str = Tools::addComma(-i64BankerWin);
        s = "-" + str;
    } else {
        s = Tools::addComma(i64BankerWin);
    }
    _gameView->Label_BankToatal->setString(s);
}

//
void GameTableUIV88::refreshMyTotalWinNum(long long i64UserWin) {
    string s;
    if (i64UserWin < 0) {
        string str = Tools::addComma(-i64UserWin);
        s = "-" + str;
    } else {
        s = Tools::addComma(i64UserWin);
    }
    _gameView->Label_UserToatal->setString(s);
}

void GameTableUIV88::IUpdateNTList() {
    std::vector<std::string> names = _logic->requestNTList();

    std::vector<T_DEALER> ntList;
    for(auto item: names) {
        T_DEALER name;
        name.dealerName = item;
        ntList.push_back(name);
    }
    _dealerList->resetDealerList(ntList);
}

void GameTableUIV88::IUpdateNTInfo(int userId, const std::string name, LLONG money, int logoID, int bankCount) {
    // 庄家名字
    _gameView->Label_BankName->setString(name);
	
	auto user = _logic->getUserByUserID(userId);
	_gameView->ImageView_BankHead->setHeadWithInfo(user);

	char str[50] = { 0 };
	// 庄家筹码
	if (money > 0) {
		_gameView->Label_BankMoney->setVisible(true);
		_gameView->Label_BankMoney->setString(Tools::addComma(money));
	}
	else {
		_gameView->Label_BankMoney->setVisible(false);
	}

	sprintf(str, "%d", bankCount);
	_gameView->Label_BankNum->setString(str);
}

// 显示自己下注额
void GameTableUIV88::IDisplayMyBetMoney(int noteArea, LLONG money) {
    CCAssert(noteArea <= 7 || noteArea >= 0, "noteArea error.");
    _currentMyZhu[noteArea] += money;
    char str[50] = { 0 };
    sprintf(str, "%lld", _currentMyZhu[noteArea]);
    _gameView->TextAtlas_UserBet[noteArea]->setString(str);
    _gameView->TextAtlas_UserBet[noteArea]->setVisible(true);


    //记录自己的下注
    m_vPrevBetArea1.push_back(noteArea);
    m_vPrevBetMoney1.push_back(money);
}

void GameTableUIV88::IDisplayOtherBetMoney(int noteArea, LLONG money)
{
	_gameView->TextAtlas_TotalBet[noteArea]->setString(to_string(money));
	_gameView->TextAtlas_TotalBet[noteArea]->setVisible(true);
}

//保存下注过程
void GameTableUIV88::saveBetProcess() {
    m_vPrevBetArea2.clear();
    m_vPrevBetMoney2.clear();
    if (m_vPrevBetArea1.size() > 0) {
        m_vPrevBetArea2.assign(m_vPrevBetArea1.begin(), m_vPrevBetArea1.end());
        m_vPrevBetMoney2.assign(m_vPrevBetMoney1.begin(), m_vPrevBetMoney1.end());
        m_vPrevBetArea1.clear();
        m_vPrevBetMoney1.clear();
    }
}

// 更新玩家金币
void GameTableUIV88::IUpdateUserMoney(LLONG money) {
    _ownMoney->setString(Tools::addComma(money));
}

// 显示玩家下注动画
void GameTableUIV88::IPlayNoteAnimation(BYTE noteType, int noteArea, BYTE bDeskStation) {
    HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_BET);
    if (bDeskStation == _logic->getMySeatNo()) {
        showMyBetAnimation(noteType, noteArea);
    } else {
        showOtherBetAnimation(noteType, noteArea);
    }

}

void GameTableUIV88::ISetBetEnable(bool enabled[], int count) {
    for (int i = 0; i < CHOUMANUM; i++) {
// 			if(!enabled[i])
// 			{
// 				_gameView->ImageView_BetBg[i]->stopAllActions();
// 			}
        if (_logic->_gameStatus == GS_SEND_CARD) {
            _gameView->ImageView_BetBg[i]->setVisible(false);
        }
        _gameView->Button_Bet[i]->setEnabled(enabled[i]);
        _gameView->Button_Bet[i]->setBright(enabled[i]);

        if (_logic->getMySeatNo() == _logic->getBankerDeskStation()) {
            _gameView->Button_Bet[i]->setEnabled(false);
            _gameView->Button_Bet[i]->setBright(false);
        }
    }
}


void GameTableUIV88::IStartTimer(int second) {
    _t = second;
    if (_gameView->m_timerObj) {
        if(_t > 0) {
            _gameView->m_timerObj->start(_t);
        } else {
            hideClock();
        }
    }
}

void GameTableUIV88::hideClock() {
    if (_gameView->m_timerObj) {
        _gameView->m_timerObj->stop();
    }
    _t = 0;
}

//void GameTableUIV88::timerUpdate(float delat) {
//
//    if (_countForOneSec++ >= 60) {
//        _countForOneSec = 0.0;
//        _t = _t - 1;
//        if (_t == 5)
//        {
//            //_clockMusicId = HNAudioEngine::getInstance()->playEffect("v66/music/alarm_v88.mp3");
//        }
//    }
//    char str[15];
//    sprintf(str, "%d", _t);
//    _gameView->TextAtlas_TimerText->setString(str);
//
//
//    if (_t <= 5)
//    {
//        //播放动画
//        //_gameView->ImageView_TimerBg->setVisible(false);
//        //_gameView->TextAtlas_TimerText->setVisible(false);
//    //    _gameView->m_pClock->setVisible(true);
//        //_gameView->m_pClockAnimation->play("alarm", false);
//    }
//    if (_t < 0) {
//        hideXiaZhuBTN();
//        //HNAudioEngine::getInstance()->stopEffectWithID(_clockMusicId);
//        hideClock();
//        return;
//    }
//}


void GameTableUIV88::IAreaBet(int area, LLONG betMoney) {
    int rd = rand() % 7;
    CurJtInfo data;
    data.val  = betMoney;
    data.xpos = rd;
    _gameView->pnoteview[area].dropone(data, 0);
}

void GameTableUIV88::IInitCard(BYTE cards[][3], int playerCount, int NTCount, int playerCardType[], int NTCardType[]) {
    _resultCard->initCard(cards, NTCount, playerCount, NTCardType, playerCardType);
}

void GameTableUIV88::IShowCards(bool animate) {
    if(animate) {

    } else {
        _resultCard->showCardBoradWithOutAnimation();
    }
}

void GameTableUIV88::ISendCard(bool animate) {
    if(animate) {
        _resultCard->initNewGame();
        //_resultCard->showCardBorad();

//        this->unschedule(schedule_selector(GameTableUIV88::timerUpdate));
        hideClock();
		this->schedule(schedule_selector(GameTableUIV88::dispatchCard), 1);
    } else {
        _resultCard->dispatchCardWithoutAnimation();
    }
}

void GameTableUIV88::IShowBetHint(bool visible, bool resume) {
    if(visible) {
        if (_logic->_gameStatus == GS_NOTE_STATE && !resume) {
            HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_TIP_BET);
            HNAudioEngine::getInstance()->playBackgroundMusic(BJL_GAME_MUSIC_BET_BACK);
            _resultCard->playAnimation(hand_plese);
        }
        _gameView->ImageView_betHint->runAction(RepeatForever::create(Blink::create(0.5f, 1)));
    } else {
        if (_logic->_gameStatus == GS_SEND_CARD && !resume) {
            HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_TIP_STOP_BET);
            HNAudioEngine::getInstance()->stopBackgroundMusic(BJL_GAME_MUSIC_BET_BACK);
            _resultCard->playAnimation(hand_click);
        }
        _gameView->ImageView_betHint->stopAllActions();
        _gameView->ImageView_betHint->setVisible(false);
    }
}

void GameTableUIV88::IShowWaitHint(bool visible) {
    if (visible) {
        _gameView->ImageView_WaitHint->runAction(RepeatForever::create(Blink::create(0.5f, 1)));

        if (m_bNextOut) {
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(5.0f), CallFunc::create([&]() {
                _logic->requestStandUp();
            })));
        }
    } else {
        _gameView->ImageView_WaitHint->stopAllActions();
        _gameView->ImageView_WaitHint->setVisible(false);
    }
}

void GameTableUIV88::IGameStart() {
    HNAudioEngine::getInstance()->playEffect("v66/music/gamestart.mp3");

    //重复上局下注
    if (m_bAutoBet) {
        LLONG betValue[] = { 1000, 10000, 100000, 1000000, 5000000, 10000000, 0 };

        for (int i = 0; i < m_vPrevBetArea2.size(); i++) {
            int iBetType = 0;
            for (int j = 0; j < 5; j++) {
                if (betValue[j] == m_vPrevBetMoney2.at(i)) {
                    iBetType = j;
                    break;
                }
            }

            _logic->requestBet(iBetType, m_vPrevBetArea2.at(i));
        }
    } else {
        m_vPrevBetArea2.clear();
        m_vPrevBetMoney2.clear();
    }
}

void GameTableUIV88::IShowWinArea(bool visible) {
    if(visible) {
        int* winArea = _logic->getWinArea();
        int maxWinIndex = -1;
        int maxWinRate = 0;
        for (BYTE i = 0; i < NUMOFBETAREA ; ++i) {
            if (*(winArea + i) > 1) {
                _gameView->ImageView_BetWin[i]->setVisible(true);
                _gameView->ImageView_BetWin[i]->runAction(RepeatForever::create(Blink::create(0.5f, 1)));
                maxWinIndex = i;
                maxWinRate = *(winArea + i);
            } else {
                _gameView->ImageView_BetWin[i]->stopAllActions();
                _gameView->ImageView_BetWin[i]->setVisible(false);
            }
        }
        //
        if (maxWinIndex >= 0 && maxWinRate > 0 && _currentMyZhu[maxWinIndex] > 0) {
            //展示开奖最大区域
            playBigWinAnimation(BJL_ID, maxWinIndex, maxWinRate, maxWinRate*_currentMyZhu[maxWinIndex]);
        }
        
        cleanBetAnimation();
    } else {
        for (BYTE i = 0; i < NUMOFBETAREA ; ++i) {
            _gameView->ImageView_BetWin[i]->stopAllActions();
            _gameView->ImageView_BetWin[i]->setVisible(false);
        }
    }
}

void GameTableUIV88::IShowOpenBoard(bool visible) {
    if(visible) {

    } else {
        //_resultCard->hideCardBorad();
        _resultCard->initNewGame();
    }

}

void GameTableUIV88::showMyBetAnimation(BYTE whichBTN, BYTE whichArea) {
    //创建动画
    //int whichBTN = pos;
    if (whichBTN < CHOUMANUM) {
        int count = 1;
        if (whichBTN > 1) {
            int bet[] = { 1000, 10000, 100000, 1000000, 5000000, 10000000, 0 };
            count = bet[whichBTN]/bet[whichBTN-1];
            whichBTN--;
        }
		whichBTN = whichBTN + 7;
        for (int i = 0; i < count; i++) {
            char str[128] = { 0 };
            sprintf(str, BET_BTN_PATH, whichBTN);
            auto sp = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(str));
            if (nullptr != sp) {
                _gameView->gametable_layout->addChild(sp, 5);
                sp->setScale(0.5f);
                sp->setVisible(true);
                auto pos1 = _gameView->Button_Bet[whichBTN]->getPosition();
                sp->setPosition(pos1);
                BET_TYPEV88 *data = new BET_TYPEV88();
                data->whichPeople = 1;
                data->whichType = whichArea;
                data->whichJetton = whichBTN;
                sp->setUserData((void*)data);
                auto seq = Sequence::create(
                               Spawn::create(
                                   EaseOut::create(EaseSineOut::create(MoveTo::create(0.4f, _gameView->pnoteview[whichArea].getRandPositionInTheRect())), 1),
                                   nullptr
                               ),
							   CallFuncN::create(CC_CALLBACK_1(GameTableUIV88::betAnimationCallback, this)),
                               nullptr
                           );
                sp->runAction(seq);
                vConinSprite.pushBack(sp);
            }
        }
    }
}

void GameTableUIV88::showOtherBetAnimation(BYTE whichBTN, BYTE whichArea) {
    //创建动画
    //int whichBTN = pos;
    if (whichBTN < CHOUMANUM) {
        int count = 1;
// 			if (whichBTN > 1)
// 			{
// 				whichBTN--;
// 				count = 10;
// 			}
		whichBTN = whichBTN + 7;

        for (int i = 0; i < count; i++) {
            char str[128] = { 0 };
            sprintf(str, BET_BTN_PATH, whichBTN);			
            auto sp = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(str));
            if (nullptr != sp) {
                _gameView->gametable_layout->addChild(sp, 5);
                sp->setScale(0.5f);
                sp->setVisible(true);

                Vec2 noSeatBtnPos = _gameView->Button_NoSeat->getPosition();
                Vec2 startPos = Vec2(noSeatBtnPos.x - 75, noSeatBtnPos.y - 25);
                int width = 150;
                int height = 50;

                Vec2 endPos;
                endPos.x = rand() % width + startPos.x;
                endPos.y = rand() % height + startPos.y;

                sp->setPosition(endPos);
                BET_TYPEV88 *data = new BET_TYPEV88();
                data->whichPeople = 2;
                data->whichType = whichArea;
                data->whichJetton = whichBTN;
                sp->setUserData((void*)data);
                auto seq = Sequence::create(
                               Spawn::create(
                                   EaseOut::create(EaseSineOut::create(MoveTo::create(0.4f, _gameView->pnoteview[whichArea].getRandPositionInTheRect())), 1),
                                   nullptr
                               ),
							   CallFuncN::create(CC_CALLBACK_1(GameTableUIV88::betAnimationCallback, this)),
                               nullptr
                           );
                sp->runAction(seq);
                vConinSprite.pushBack(sp);
            }
        }
    }
}

void GameTableUIV88::showBankerBetAnimation(BYTE whichBTN, BYTE whichArea, BYTE whichPeoPle, float delay) {
    if (whichBTN < CHOUMANUM) {
        int count = 1;

        for (int i = 0; i < count; i++) {
            char str[128] = { 0 };
			whichBTN = whichBTN + 7;
            sprintf(str, BET_BTN_PATH, whichBTN);
            auto sp = Sprite::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(str));
            if (nullptr != sp) {
                _gameView->gametable_layout->addChild(sp, 5);
                sp->setScale(0.5f);
                sp->setVisible(false);

                Vec2 bankerPos = Vec2(WIN_SIZE.width / 2 - 340, WIN_SIZE.height - 100);
                Vec2 startPos = Vec2(bankerPos.x - 75, bankerPos.y - 25);
                int width = 150;
                int height = 50;

                Vec2 endPos;
                endPos.x = rand() % width + startPos.x;
                endPos.y = rand() % height + startPos.y;

                sp->setPosition(endPos);
                BET_TYPEV88 *data = new BET_TYPEV88();
                data->whichPeople = whichPeoPle;
                data->whichType = whichArea;
                data->whichJetton = whichBTN;
                sp->setUserData((void*)data);
                auto seq = Sequence::create(
                               DelayTime::create(delay),
                CallFuncN::create([](Node* pSender) {
                    pSender->setVisible(true);
                }),
                Spawn::create(
                    EaseOut::create(EaseSineOut::create(MoveTo::create(0.4f, _gameView->pnoteview[whichArea].getRandPositionInTheRect())), 1),
                    nullptr
                ),
                //CallFuncN::create(CC_CALLBACK_1(GameTableUIV88::betAnimationCallback, this)),
                nullptr
                           );
                sp->runAction(seq);
                vConinSprite.pushBack(sp);
            }
        }
    }
}

//结算
void GameTableUIV88::showSettlement(S_C_GameResult* pGameResult) {
    //显示结算框
    showSettlementDialog(pGameResult);

    for (int i = 0; i < 8; i++) {
        _gameView->TextAtlas_TotalBet[i]->setString("0");
        _gameView->TextAtlas_TotalBet[i]->setVisible(false);   //下注的金币数
        _gameView->TextAtlas_UserBet[i]->setString("0");
        _gameView->TextAtlas_UserBet[i]->setVisible(false);   //自己下注的金币数

    }

// 		this->runAction(Sequence::create(DelayTime::create(3.5f), CallFunc::create([&](){
// 			initTheNewGameData();
// 		}), nullptr));
}

//重置庄家列表
void GameTableUIV88::resetDealerList(BYTE dealerStation[], SHORT count) {
    char str[62] = {0};
    std::vector<T_DEALER> dealerlist;
    for (BYTE i = 0 ; i < count ; i++) {
        SHORT a = dealerStation[i];
        auto user = _logic->getUserBySeatNo(dealerStation[i]);
        if (nullptr != user) {
            strcpy(str, GBKToUtf8(user->nickName));
            T_DEALER tdealer;
            tdealer.dealerName = str;
            if (tdealer.dealerName.empty()) {
                continue;
            }
            dealerlist.push_back(tdealer);
        }
    }
    _dealerList->resetDealerList(dealerlist);
}

void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
    if (bVisible) {
        if (!m_Notice) {
            m_Notice = GameNotice::create();
            m_Notice->setLocalZOrder(ZORDER_MAX - 1);
            m_Notice->setName("GameNotice");
        }
        m_Notice->openNotic(this, 10000, message, nCount, nType);
    }
}

//筹码是否在赢的区域
bool GameTableUIV88::inWinArea(int area) {
    auto pData = _logic->getWinArea();
    bool bIn = false;
    for (int i = 0; i < 8; i++) {
        if (pData[i]>0 && i == area) {
            bIn = true;
            break;
        }
    }

    return bIn;
}

void GameTableUIV88::cleanBetAnimation() {
    //几个位置
    Vec2 bankerPos = Vec2(WIN_SIZE.width / 2 - 340, WIN_SIZE.height - 100);
    Vec2 otherPos = _gameView->Button_NoSeat->getPosition();
    Vec2 myPos = Vec2(100, 170);

    float time = 2;
    bool has = false;

    //庄家收筹码
    for (int i = 0; i < vConinSprite.size(); i++) {
        void* pData = vConinSprite.at(i)->getUserData();
        BET_TYPEV88* data = (BET_TYPEV88*)pData;
        Vec2 pos;

        //开和 庄闲返还
        if (inWinArea(6) && (data->whichType == 0 || data->whichType == 3)) {
            continue;
        }

        if (!inWinArea(data->whichType)) {
            if (!has) {
                runAction(Sequence::create(DelayTime::create(time), CallFunc::create([]() {
                    HNAudioEngine::getInstance()->playEffect("music/NN/clear_coin.mp3");
                }), nullptr));
            }

            pos = bankerPos;

            auto seq = Sequence::create(
                           DelayTime::create(time),
                           Spawn::create(
                               EaseOut::create(EaseSineOut::create(MoveTo::create(0.4f, pos)), 1),
                               nullptr
                           ),
                           RemoveSelf::create(),
                           nullptr
                       );

            vConinSprite.at(i)->runAction(seq);

            has = true;
        }
    }

    if (has) {
        time += 1;
        has = false;
    }

    //庄家吐筹码
    int betCount = vConinSprite.size();
    for (int i = 0; i < betCount; i++) {
        void* pData = vConinSprite.at(i)->getUserData();
        BET_TYPEV88* data = (BET_TYPEV88*)pData;

        //开和 庄闲不吐
        if (inWinArea(6) && (data->whichType == 0 || data->whichType == 3)) {
            continue;
        }

        if (inWinArea(data->whichType)) {
            if (!has) {
                runAction(Sequence::create(DelayTime::create(time), CallFunc::create([]() {
                    HNAudioEngine::getInstance()->playEffect("music/NN/clear_coin.mp3");
                }), nullptr));
            }

            showBankerBetAnimation(data->whichJetton, data->whichType, data->whichPeople, time);

            has = true;
        }
    }

    if (has) {
        time += 1;
        has = false;
    }

    //闲家收筹码
    for (int i = 0; i < vConinSprite.size(); i++) {
        void* pData = vConinSprite.at(i)->getUserData();
        BET_TYPEV88* data = (BET_TYPEV88*)pData;
        Vec2 pos;

        if (data->whichPeople == 1) {
            pos = myPos;
        } else if (data->whichPeople == 2) {
            pos = otherPos;
        }

        if (!has) {
            runAction(Sequence::create(DelayTime::create(time), CallFunc::create([]() {
                HNAudioEngine::getInstance()->playEffect("music/NN/clear_coin.mp3");
            }), nullptr));
        }

        auto seq = Sequence::create(
                       DelayTime::create(time),
                       Spawn::create(
                           EaseOut::create(EaseSineOut::create(MoveTo::create(0.4f, pos)), 1),
                           nullptr
                       ),
                       RemoveSelf::create(),
                       nullptr
                   );

        vConinSprite.at(i)->runAction(seq);

        has = true;

    }

    vConinSprite.clear();
}

void GameTableUIV88::showSettlementDialog(S_C_GameResult* pData) {
    CCAssert(pData != nullptr, "nullptr.");

    //
    auto coin = FloatCoin::create();
    if (coin) {
        coin->openParent(this, Vec2(150, 100), ZORDER_MAX);
        coin->updateCoin(pData->i64UserScoreSum);
    }

    
    //结算框
    auto gameResult = BRNN::BRNNSettlementLayerV88::create();
    this->addChild(gameResult, ZORDER_MAX + 1);
    gameResult->setPosition(WIN_SIZE / 2);

    char str[64] = { 0 };
    sprintf(str, "%lld", pData->i64NtScoreSum);
    gameResult->setBankerMoney(str);
    sprintf(str, "%lld", pData->i64UserScoreSum);
    gameResult->setMyselfMoney(str);

    if (pData->i64UserScoreSum > 0) {
        HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_WIN);
    } else if (pData->i64UserScoreSum < 0) {
        HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_LOSE);
    } else {

    }
}

void GameTableUIV88::hideXiaZhuBTN() {
    stopBetAnimation();
    for(BYTE i=0; i < CHOUMANUM ; ++i) {	//下注按钮
        _gameView->ImageView_BetBg[i]->setVisible(false);
        _gameView->Button_Bet[i]->setEnabled(false);
    }
}

void GameTableUIV88::animationUpdate(float dt) {
    int iAniIndex = rand() % 4;
    if (!_resultCard->isAnimationPlaying()) {
        if (iAniIndex == 0) {
            _resultCard->playAnimation(eye_one);
        } else if (iAniIndex == 1) {
            _resultCard->playAnimation(eye_two);
        } else if (iAniIndex == 2 || iAniIndex == 3) {
            _resultCard->playAnimation(mouth_kiss);
        }
    }
}
    
    void GameTableUIV88::playBigWinAnimation(int gameID, int openLogo, int winRate, LLONG money) {
        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
        addChild(colorLayer, ZORDER_MAX+5);
        colorLayer->runAction(Sequence::create(DelayTime::create(2.5f), RemoveSelf::create(), nullptr));
        auto node = CSLoader::createNode("v88/animation/common/jackpotting/jackpotting.csb");
        if (node) {
            node->setPosition(WIN_SIZE/2);
            addChild(node, ZORDER_MAX+5);
            auto action = CSLoader::createTimeline("v88/animation/common/jackpotting/jackpotting.csb");
            if (action) {
                node->runAction(action);
                action->play("jackpotting", false);
            }
            //
            char gameNamePath[64] = {0};
            sprintf(gameNamePath, "v66/common/bigwin/%d.png", gameID);
            std::string logoName = "";
            if (openLogo == 2) {
                logoName = "双虎";
            }
            else if (openLogo == 5) {
                logoName = "双龙";
            }
            else if (openLogo == 6) {
                logoName = "和";
            }
            else if (openLogo == 7) {
                logoName = "同点和";
            }
            char tipString[256] = {0};
            sprintf(tipString, GetText("BIG_WIN_TIP_BJL"), logoName.c_str(), winRate);
            auto tmpNode = node->getChildByName("Node_1");
            if (tmpNode) {
                auto kuang = tmpNode->getChildByName("kuang");
                if (kuang) {
                    //game name
                    ImageView* gameName = ImageView::create(gameNamePath);
                    if (gameName) {
                        gameName->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                        gameName->setPosition(Vec2(kuang->getContentSize().width/2, kuang->getContentSize().height*0.73f));
                        kuang->addChild(gameName);
                    }
                    //money value
                    TextAtlas* moneyAtals = dynamic_cast<TextAtlas*>(kuang->getChildByName("AtlasLabel_2"));
                    if (moneyAtals) {
                        moneyAtals->setProperty(to_string(money), "v66/common/zi/111.png", 55, 78, "0");
                    }
                    //tip
                    Text* tip = Text::create(tipString, FONT_TTF_MSYH, 32);
                    if (tip) {
                        tip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                        tip->setPosition(Vec2(kuang->getContentSize().width/2, kuang->getContentSize().height*0.24f));
                        tip->setTextVerticalAlignment(TextVAlignment::CENTER);
                        tip->setTextHorizontalAlignment(TextHAlignment::CENTER);
                        tip->ignoreContentAdaptWithSize(false);
                        tip->setContentSize(Size(kuang->getContentSize().width*0.77f, kuang->getContentSize().height*0.26f));
                        kuang->addChild(tip);
                    }
                }
            }
        }
    }

void GameTableUIV88::dispatchCard(float delta) {
    if (!_resultCard->dispatchCard()) {
		this->unschedule(schedule_selector(GameTableUIV88::dispatchCard));
    }
}

void GameTableUIV88::onExit() {
    auto cache = SpriteFrameCache::getInstance();
    cache->removeSpriteFramesFromFile(CARD_RES);
//    HNAudioEngine::getInstance()->stopEffectWithID(_clockMusicId);
    HNGameUIBase::onExit();
}
void GameTableUIV88::onEnter() {
    HNGameUIBase::onEnter();

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile(CARD_RES);

    _tableBg = ImageView::create(GAME_BG_HEGUAN_V66);
    _tableBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _tableBg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height)); 
	addChild(_tableBg);

    //触摸
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan = [](Touch* touch, Event* event)->bool{
        return true;
    };
	myListener->onTouchEnded = CC_CALLBACK_2(GameTableUIV88::onTouchEnded, this);
    myListener->setSwallowTouches(false);
    dispatcher->addEventListenerWithSceneGraphPriority(myListener, _tableBg);

    loadRes();
    initUI();
	_resultCard = new ResultCardV88(this);
    createBetAreaUI();
    createMyControlUI();
    createResultRecordUI();
    //createPointRecordUI();
    createToolBarUI();
    initTheNewGameData();
	schedule(schedule_selector(GameTableUIV88::animationUpdate), 6.0f);

    _logic = new GameTableLogic(this, m_DeskIndex, m_AutoCreate);
    _logic->sendGameInfo();
    
    runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([&] () {
        auto scanner = Scanner::create();
        this->addChild(scanner, 100000);
        scanner->starPlayerEffect();
        scanner->setPosition(60, 200);
    }), nullptr));
}

void GameTableUIV88::SetImageTimeMoveNewPostion() {
    //auto postion = _gameView->ImageView_TimerBg->getPosition();

    //_gameView->ImageView_TimerBg->setPosition(Vec2(postion.x - 80, postion.y + 80));
    // _gameView->ImageView_TimerBg->runAction(CCEaseExponentialOut::create(MoveBy::create(1.0f,Vec2(0, -260))));
}

void GameTableUIV88::SetImageTimeMoveOldPostion() {
    //auto postion = _gameView->ImageView_TimerBg->getPosition();

    //_gameView->ImageView_TimerBg->runAction(CCEaseExponentialOut::create(MoveBy::create(1.0f,Vec2(0, 260))));
    //_gameView->ImageView_TimerBg->setPosition(Vec2(postion.x + 80, postion.y - 80));

}

void GameTableUIV88::updateTip(float dt) {
    int iTag = m_TextTip->getTag();
    if (iTag == 0) {
		unschedule(schedule_selector(GameTableUIV88::updateTip));
        _logic->sendUserUp();
        //leaveTheGame();
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

void GameTableUIV88::IShowTipExit(long long i64Money) {
    auto layer = GamePromptLayer::create();
//    layer->setCloseExitBtn();
//     char str[128] = { 0 };
//     int sec = 3;
//     sprintf(str, "金币数额低于%lld，%ds后离开游戏！", i64Money, sec);
//     layer->showPrompt(GBKToUtf8(str));
    int sec = 3;
    char s[128] {0};
    std::string str;
    str.append(GBKToUtf8("金币数额低于"));
    sprintf(s, "%lld", i64Money);
    str.append(s);
    sprintf(s, ",%d", sec);
    str.append(s);
    str.append(GBKToUtf8("s后离开游戏！"));
    layer->showPrompt(str);

    layer->setOkayCallBack([&]() {
        _logic->sendUserUp();
        // leaveTheGame();
    });
    layer->setColorLayerTouchEnabled(false);

    m_i64TipMoney = i64Money;
    m_TextTip = layer->getPromptText();
    m_TextTip->setTag(sec);
	schedule(schedule_selector(GameTableUIV88::updateTip), 1.0f);
    // 			this->runAction(Sequence::createWithTwoActions(CCDelayTime::create(3), CallFunc::create([&](){
    // 				_GameTableLogic->sendUserUp();
    // 				leaveTheGame();
    // 			})));

    //把重复下注关了,防止下注快，发离开消息不成功
    m_bAutoBet = false;
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


////年兽触发
//void GameTableUIV88::onGameShowNianShou() {
//    //播动画
//    showNianShouAnimation();
//}
//
////赶年兽结果
//void GameTableUIV88::onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) {
//    //过滤只有自己看的到
//    if (nUserID != _logic->getUserId(_logic->getMySeatNo())) {
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
//    if (_logic->getUserId(_logic->getMySeatNo()) == nUserID) {
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
//		btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::driveNianShouClickCallback, this));
//        addChild(btn, NewYearActivityInGame::NianShouZOrder);
//
//        auto daijiNode = NewYearActivity::NSAnimationTool::createNSDaijiNodeAni("daiji", true);
//        if (daijiNode) {
//            daijiNode->setPosition(Vec2(btn->getContentSize().width*0.5f, btn->getContentSize().height*0.5f));
//            btn->addChild(daijiNode);
//        }
//		scheduleOnce(schedule_selector(GameTableUIV88::autoRemoveNianShouButton), 5.0f);
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
//    int nUserID = _logic->getUserId(_logic->getMySeatNo());
//    _logic->sendDriveNianShou(nUserID);
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
