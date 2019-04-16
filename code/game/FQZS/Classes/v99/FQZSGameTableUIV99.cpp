#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
#include "../../../../code/common/LibHNUI/UI/UIGoldRain.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"

#include "FQZSGameTableUIV99.h"
#include "FQZSGameTableLogic.h"

#include "HNLobbyExport.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "Sound/HNAudioEngine.h"
#include "Tool/Tools.h"
#include "HNLobby/v99/PlatformResV99.h"

#include "Tool/Tools.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "HNLobby/v66/ExtraReward.h"
#include "HNLobby/v99/PlatformResV99.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"

//#include "../v99/BRNNSetLayerV99.h"

namespace FQZS {

#define WINSIZE Director::getInstance()->getWinSize()
#define NOTICE_ZORDER 100
#define TOOLBAR_ZORDER 500

//////////////////////////////////////////////////////////////////////////
//常量

//押注按钮名字
static char* Bet_Button_Names[Bet_Area_Num] = {
    "Button_fq",//飞禽
    "Button_yz",//燕子
    "Button_gz",//鸽子
    "Button_kq",//孔雀
    "Button_ly",//老鹰
    "Button_zs",//走兽
    "Button_sz",//狮子
    "Button_xm",//熊猫
    "Button_hz",//猴子
    "Button_tz",//兔子
    "Button_sy",//鲨鱼
};

static char Bet_Button_ID[Bet_Area_Num] = {
    K_FQ,//0
    K_YZ,
    K_GZ,
    K_KQ,
    K_LY,
    //
    K_ZS,//5
    K_SZ,
    K_XM,
    K_HZ,
    K_TZ,
    //
    K_SY,//10
};

//押注按钮Tag 11个值
enum {
    //押注类型按钮
    tag_fq = 0,
    tag_yz,
    tag_gz,
    tag_kq,
    tag_ly,
    tag_zs,
    tag_sz,
    tag_xm,
    tag_hz,
    tag_tz,
    tag_sy,
    //押注金额按钮
    tag_level_1 = 15,
    tag_level_2,
    tag_level_3,
    tag_level_4,
    tag_level_5,
    //取消、续注按钮
    tag_clear = 20,
    tag_rebet,
    //走势图
    tag_left = 25,
    tag_right,
};
//押注类型按钮Tag数组
static int Bet_Button_Tag[Bet_Area_Num] = {
    tag_fq,
    tag_yz,
    tag_gz,
    tag_kq,
    tag_ly,
    tag_zs,
    tag_sz,
    tag_xm,
    tag_hz,
    tag_tz,
    tag_sy,
};
//押注金额按钮Tag数组
static int Bet_Money_Tag[5] = {
    tag_level_1,
    tag_level_2,
    tag_level_3,
    tag_level_4,
    tag_level_5,
};

//押注金额
static long long g_i64BetValue[Bet_Count] = {
    1000,
    10000,
    100000,
    1000000,
    5000000
};

//转盘变速类型
enum LoopType {
    Loop_Start,
    Loop_Max_Speed,
    Loop_Slow_Speed,
    Loop_End
};


//////////////////////////////////////////////////////////////////////////
//CSB文件
static const char* FQZS_RESULT_LABEL_CSB_V99 = "Games/31000500/Result.csb";
static const char* FQZS_BET_AREA_CSB_V99 = "Games/31000500/Bet_Area.csb";
static const char* FQZS_CLEAR_RESET_AREA_CSB_V99 = "Games/31000500/ClearResetArea.csb";
static const char* FQZS_BET_BUTTONS_CSB_V99 = "Games/31000500/Bet_Buttons.csb";
static const char* FQZS_RESULT_LOOP_CSB_V99 = "Games/31000500/Result_Loop.csb";
static const char* FQZS_STATE_CSB_V99 = "Games/31000500/State.csb";
static const char* FQZS_STATE_LABEL_CSB_V99 = "Games/31000500/GameState.csb";
static const char* FQZS_POOL_CSB_V99 = "Games/31000500/Pool.csb";

//Animation
static const char* FQZS_ANIMATION_YZ_CSB_V99 = "common/animation/FQZS_animal/swallow/swallow.csb";
static const char* FQZS_ANIMATION_GZ_CSB_V99 = "common/animation/FQZS_animal/dove/dove.csb";
static const char* FQZS_ANIMATION_KQ_CSB_V99 = "common/animation/FQZS_animal/peacock/peacock.csb";
static const char* FQZS_ANIMATION_LY_CSB_V99 = "common/animation/FQZS_animal/eagle/eagle.csb";
static const char* FQZS_ANIMATION_SZ_CSB_V99 = "common/animation/FQZS_animal/lion/lion.csb";
static const char* FQZS_ANIMATION_XM_CSB_V99 = "common/animation/FQZS_animal/panda/panda.csb";
static const char* FQZS_ANIMATION_HZ_CSB_V99 = "common/animation/FQZS_animal/monkey/monkey.csb";
static const char* FQZS_ANIMATION_TZ_CSB_V99 = "common/animation/FQZS_animal/rabbit/rabbit.csb";
static const char* FQZS_ANIMATION_SY_CSB_V99 = "common/animation/FQZS_animal/shark/blueshark.csb";
static const char* FQZS_ANIMATION_JS_CSB_V99 = "common/animation/FQZS_animal/shark/goldenshark.csb";
static const char* FQZS_ANIMATION_BET_CSB_V99 = "common/animation/FQZS_text/bet/FQZS_bet.csb";
static const char* FQZS_ANIMATION_TIP_START_CIRCLE_CSB_V99 = "common/animation/FQZS_text/lottery/lottery.csb";


//////////////////////////////////////////////////////////////////////////
//字体
static const char* FQZS_FONT_TTF_V99					= "platform/fonts/simhei.ttf";


//////////////////////////////////////////////////////////////////////////
//图片资源
//background
static const char* FQZS_GAME_BG_V99						= "v99/common/BG/FQZS_BG.png";
//tool
static const char* FQZS_TOOL_BAR_PATH_V99				= "v99/cow/exit/%s";
//转盘光环
static const char* FQZS_LOOP_LIGHT_RES_V99				= "Games/31000500/FQZS_New/Common/loop_light.png";
//转盘动物资源
static const char* FQZS_SHOW_LOOP_ANIMAL_RES_V99		= "Games/31000500/FQZS_New/Result_Loop/SL_%d.png";


//////////////////////////////////////////////////////////////////////////
//音效
static const char*  GAME_SOUND_BTN_V99					= "music/btnEffect/button_click_sound.mp3";

static const char*  AUDIO_BG_FREE_V99					= "Games/31000500/music/back1.wav";
static const char*  AUDIO_BG_BET_V99					= "Games/31000500/music/back2.wav";
static const char*  AUDIO_BG_OPEN_V99					= "Games/31000500/music/back3.wav";

static const char*  AUDIO_TIP_BET_V99					= "v66/music/BJL/tipBet.mp3";
static const char*  AUDIO_TIP_STOP_BET_V99				= "v66/music/BJL/tipStopBet.mp3";
static const char*  AUDIO_BET_MONEY_V99					= "v66/music/BJL/down_coin.mp3";

//转盘音效
static const char*  AUDIO_LOOP_START_V99				= "Games/31000500/music/loop/start_loop.wav";
static const char*  AUDIO_LOOP_MAX_SPEED_V99			= "Games/31000500/music/loop/max_speed_loop.wav";
static const char*  AUDIO_LOOP_SLOW_SPEED_V99			= "Games/31000500/music/loop/slow_speed_loop.wav";
static const char*  AUDIO_LOOP_END_V99					= "Games/31000500/music/loop/end_loop.wav";

//飞禽 燕子 鸽子 孔雀 老鹰 0-5
//走兽 狮子 熊猫 猴子 兔子 6-10
//银鲨 金鲨 通杀 通赔 11-14
static const char* AUDIO_KIND_RES_V99 = "Games/31000500/music/Animal_%d.mp3";//通用

//
static const int AutoHideBetButtons_Tag_V99 = 101;//超时自动隐藏
static const int FQZS_ANIMATION_BET_NODE_TAG_V99 = 103;//请下注动画节点TAG


//////////////////////////////////////////////////////////////////////////

HNGameUIBase* GameTableUIV99::create(BYTE bDeskIndex, bool bAutoCreate) {
	GameTableUIV99* tableUI = new GameTableUIV99();
    if (tableUI && tableUI->init(bDeskIndex, bAutoCreate)) {
        tableUI->autorelease();
    } else {
        delete tableUI;
        tableUI = nullptr;
    }
    return tableUI;
}

GameTableUIV99::GameTableUIV99()
//    : m_iBetMoneyType(-1)
{
    //    TextureCache::getInstance()->addImage(NN_GOLD_IMAGE);
}

GameTableUIV99::~GameTableUIV99() {
	this->unschedule(schedule_selector(GameTableUIV99::updateClock));
	this->unschedule(schedule_selector(GameTableUIV99::updateTip));
	this->unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
	this->unschedule(schedule_selector(GameTableUIV99::updateResultAnimation));
	this->unschedule(schedule_selector(GameTableUIV99::showGameStateLabelAnimation));
	this->unschedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree));

//	this->unschedule(schedule_selector(GameTableUIV99::autoRemoveNianShouButton));

    //this->unschedule(schedule_selector(GameTableUIV99::sendCardTimer));
//    this->unschedule(schedule_selector(GameTableUIV99::showOpenBtn));
    CC_SAFE_DELETE(_GameTableLogic);

    resetData();
}

bool GameTableUIV99::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }

    //初始化人数和变量
    resetData();
    _GameTableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate, PLAYER_COUNT);

    _fScaleX = WIN_SIZE.width / DESIGN_SIZE.width;
    _fScaleY = WIN_SIZE.height / DESIGN_SIZE.height;

    //触摸
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto myListener = EventListenerTouchOneByOne::create();
	myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV99::onTouchBegan, this);
    myListener->setSwallowTouches(false);
    dispatcher->addEventListenerWithSceneGraphPriority(myListener,this);

    //bg
	_tableBg = Sprite::create(FQZS_GAME_BG_V99);
    addChild(_tableBg);
    _tableBg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
    _tableBg->setScaleX(_fScaleX);
    _tableBg->setScaleY(_fScaleY);


    {
        for (int i = 0; i < Open_Animal_Num; i++) {
            m_openRecordAnimal.at(i) = Open_Kind[0];
        }
    }

    createPool();//彩金池

    createReadyUI();

    createBetArea();
    createResultLoop();
    createState();

    createClock();//阶段倒计时
    createGameState();//阶段文字展示
    createBetButtons();//下注按钮
    createResult();//结算结果

    createToolBarUI();

    _GameTableLogic->sendGameInfo();

    return true;
}

void GameTableUIV99::createToolBarUI() {
	//char resStr[128] = { 0 };

	//sprintf(resStr, FQZS_TOOL_BAR_PATH_V99, "tool_show_hide.png");
	//auto toobarBtn1 = Button::create(resStr, resStr);
	//toobarBtn1->addClickEventListener([=](Ref*) {
	//	showToolBar(true);
	//});
	//toobarBtn1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//toobarBtn1->setPosition(Vec2(10, WIN_SIZE.height - 10));
	//toobarBtn1->setTag(em_toolbarBtn);
	//this->addChild(toobarBtn1, TOOLBAR_ZORDER);
	//toobarBtn1->setVisible(true);

	//sprintf(resStr, FQZS_TOOL_BAR_PATH_V99, "tool_bg.png");
	//sprintf(resStr, FQZS_TOOL_BAR_PATH_V99, "tool_bg_three.png");
	//_toolbarBg = Sprite::create(resStr);
	//_toolbarBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	//_toolbarBg->setPosition(10, WIN_SIZE.height + _toolbarBg->getContentSize().height);
	//_toolbarBg->setName("0");//0为在上面 1在下面
	//this->addChild(_toolbarBg, TOOLBAR_ZORDER+1);     //whao

	//Size toolbarSize = _toolbarBg->getContentSize();

	//sprintf(resStr, FQZS_TOOL_BAR_PATH_V99, "tool_setting.png");
	//auto setPic = Sprite::create(resStr);
	//setPic->setPosition(toolbarSize.width / 2, toolbarSize.height * (5.0f/6.0f));
	//_toolbarBg->addChild(setPic);
	//auto setBtn = ControlButton::create();
	//setBtn->setPreferredSize(setPic->getContentSize());
	//setBtn->setPosition(setPic->getPosition());
	//setBtn->setTag(em_toolbarSetBtn);
	//setBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
	//_toolbarBg->addChild(setBtn);

// 	sprintf(resStr, FQZS_TOOL_BAR_PATH_V99, "tool_help.png");
// 	auto helpPic = Sprite::create(resStr);
// 	helpPic->setPosition(toolbarSize.width / 2, toolbarSize.height * (3.0f/8.0f));
// 	_toolbarBg->addChild(helpPic);
// 	auto helpBtn = ControlButton::create();
// 	helpBtn->setPreferredSize(helpPic->getContentSize());
// 	helpBtn->setPosition(helpPic->getPosition());
// 	helpBtn->setTag(em_toolbarHelpBtn);
// 	helpBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
// 	_toolbarBg->addChild(helpBtn);

	//sprintf(resStr, FQZS_TOOL_BAR_PATH_V99, "tool_exit.png");
	//auto leavePic = Sprite::create(resStr);
	//leavePic->setPosition(toolbarSize.width / 2, toolbarSize.height * (3.0f / 6.0f));
	//_toolbarBg->addChild(leavePic);
	//auto leaveBtn = ControlButton::create();
	//leaveBtn->setPreferredSize(leavePic->getContentSize());
	//leaveBtn->setPosition(leavePic->getPosition());
	//leaveBtn->setTag(em_toolbarLeaveBtn);
	//leaveBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
	//_toolbarBg->addChild(leaveBtn);

	//sprintf(resStr, FQZS_TOOL_BAR_PATH_V99, "tool_next_out.png");
	//auto nextRoundOutPic = Sprite::create(resStr);
	//nextRoundOutPic->setPosition(toolbarSize.width / 2, toolbarSize.height * (1.0f / 6.0f));
	//_toolbarBg->addChild(nextRoundOutPic);
	//auto nextRoundOutBtn = ControlButton::create();
	//nextRoundOutBtn->setPreferredSize(nextRoundOutPic->getContentSize());
	//nextRoundOutBtn->setPosition(nextRoundOutPic->getPosition());
	//nextRoundOutBtn->setTag(em_toolbarNextOutBtn);
	//nextRoundOutBtn->setUserData((void*)false);
	//nextRoundOutBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
	//_toolbarBg->addChild(nextRoundOutBtn);

	//sprintf(resStr, FQZS_TOOL_BAR_PATH_V99, "tool_check.png");
	//auto checkSprite = Sprite::create(resStr);
	//checkSprite->setPosition(Vec2(25, nextRoundOutPic->getContentSize().height / 2 + 4));
	//checkSprite->setName("checkSprite");
	//checkSprite->setVisible(false);
	//nextRoundOutBtn->addChild(checkSprite);
	int zOrder = TOOLBAR_ZORDER;
	auto toolbar = ToolBarLayer::create();
	if (toolbar) {
		toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
			if (!RoomLogic()->connected()) {
				leaveTheGame();
			}
			else {
				if (m_GameStatus != GS_FREE) {
					// 游戏不在空闲时期，不允许离开
					bool bHasNote = false;
					for (int i = 0; i < GAME_NOTE_COUNT; i++) {
						if (m_i64OwnNotes[i] > 0) {
							bHasNote = true;
						}
					}

					if (bHasNote) {
						//auto layer = GamePromptLayer::create();
						//layer->showPrompt(GetText("limit_leave_in_game"));
						GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
						layer->setGamePromptTip(GetText("limit_leave_in_game"));

						return;
					}
				}
				_GameTableLogic->sendUserUp();
			}
		});
		toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
			IGameSet* gameSet = GameSetFactory::createWithVersion();
			gameSet->setBgPath(AUDIO_BG_FREE_V99);
			gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
		});
		toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
			//auto helpLayer = HN::HelpLayer::create();
			//helpLayer->setContentOnView("v66/common/help/BB.png");
			//helpLayer->setName("helpLayer");
			//helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			//helpLayer->setOrignalPosition();
			//helpLayer->moveSelfInWithAction();
			//addChild(helpLayer, zOrder + 5);
		});
		toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
	}
}

//void GameTableUIV99::createPlayerHeadUI(int playerCount) {
//  if (playerCount <= 0) return;

//  for (int i = 0; i < playerCount; i++) {
//      bool bMe = (i == 0) ? true : false;
//bool bTop = (i == PLAYER_COUNT / 2) ? true : false;
//      auto playerHeadLayer = CPlayerHead::create();
//      playerHeadLayer->initUI(bMe, bTop);
//      addChild(playerHeadLayer);
//      playerHeadLayer->setPosition(Vec2(0, 0));
//      playerHeadLayer->setVisible(false);

//      m_vPlayerHead.push_back(playerHeadLayer);
//  }
//}

//vector<Vec2> GameTableUIV99::getPlayerHeadPos() {
//    std::vector<Vec2> vHeadPos;
//	vHeadPos.push_back(Vec2(20, 2));
//	vHeadPos.push_back(Vec2(WIN_SIZE.width - 10 - edgeSize.width, WIN_SIZE.height / 2 - edgeSize.height));
//	vHeadPos.push_back(Vec2(WIN_SIZE.width - 10 - edgeSize.width, WIN_SIZE.height / 2 + edgeSize.height / 4));
//	vHeadPos.push_back(Vec2(WIN_SIZE.width / 2 - topSize.width / 2, WIN_SIZE.height - topSize.height - 2));
//	vHeadPos.push_back(Vec2(10, WIN_SIZE.height / 2 + edgeSize.height / 4));
//	vHeadPos.push_back(Vec2(10, WIN_SIZE.height / 2 - edgeSize.height));
//
//    return vHeadPos;
//}

//void GameTableUIV99::resetPlayerHeadUI() {
//	for (int i = 0; i < PLAYER_COUNT; i++) {
//        m_vPlayerHead.at(i)->setPosition(getPlayerHeadPos().at(i));
//    }
//}

void GameTableUIV99::createClock() {
// 	m_ClockBg = ImageView::create(FQZS_CLOCK_BG_V99);
//     addChild(m_ClockBg);
//     if (_resultLoop.loopNode) {
// 		m_ClockBg->setPosition(Vec2(_resultLoop.loopNode->getPositionX() + (m_ClockBg->getContentSize().width) * 3, _resultLoop.loopNode->getPositionY() + 30));
//     } else {
//         //m_ClockBg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 110.0f));
// 		m_ClockBg->setPosition(Vec2(_resultLoop.loopNode->getPositionX() + (m_ClockBg->getContentSize().width) * 3, _resultLoop.loopNode->getPositionY()));
//     }
// 
// 	m_ClockTimeLabel = LabelAtlas::create("00", FQZS_CLOCK_NUM_V99, 25, 34, '0');
//     m_ClockBg->addChild(m_ClockTimeLabel);
//     m_ClockTimeLabel->setVisible(false);
//     m_ClockTimeLabel->setAnchorPoint(Vec2(0.5, 0.5));
//     m_ClockTimeLabel->setPosition(m_ClockBg->getContentSize() / 2);
	Vec2 pos = Vec2(WIN_SIZE.width/2 , WIN_SIZE.height - 10 - 240 + 30);
	if (_resultLoop.loopNode)
	{
		pos = Vec2(_resultLoop.loopNode->getPositionX() + 250, _resultLoop.loopNode->getPositionY() + 30);
	}

	//m_Clock = BRNN::ClockV99::create();
	//m_Clock->setPosition(pos);
	//m_Clock->setVisible(false);
	//addChild(m_Clock);
	Vec2 vPos(_resultLoop.loopNode->getPositionX() , _resultLoop.loopNode->getPositionY());
	m_clockObj = TimerObject::create();
	if (m_clockObj) {
		vPos += WIN_SIZE / 2;
		m_clockObj->setPosition(vPos);
		m_clockObj->setScale(0.9f);
		m_clockObj->setVisible(false);
		this->addChild(m_clockObj, TOOLBAR_ZORDER-1);
	}
}

void GameTableUIV99::createReadyUI() {
    ////准备按钮
    //m_PrepareBtn = Button::create(NN_PREPARE_BTN);
    //addChild(m_PrepareBtn);
    //m_PrepareBtn->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 110));
    //m_PrepareBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::prepareClickCallback, this));

    //Size readyBtnSize = m_PrepareBtn->getContentSize();
    //auto readyBtnImage = ImageView::create(NN_PREPARE_WORD);
    //readyBtnImage->setPosition(Vec2(readyBtnSize.width / 2, readyBtnSize.height / 2 + 4));
    //m_PrepareBtn->addChild(readyBtnImage);

    //for (int i = 0; i < PLAYER_COUNT; i++) {
//       auto readyImage = ImageView::create(NN_PREPARE_IMAGE);
//       addChild(readyImage);
    //	readyImage->setAnchorPoint(getStateTipAnchor(i));
//       readyImage->setPosition(getStateTipPos(i));
//       readyImage->setVisible(false);

//       m_vPrepareImage.push_back(readyImage);
//   }

    //m_vOkImage.resize(PLAYER_COUNT);
    //for (int i = 0; i < PLAYER_COUNT; i++) {
//       auto okImage = ImageView::create(NN_OK_IMAGE);
//       addChild(okImage, 55);
//       int iViewID = _GameTableLogic->logicToViewSeatNo(i);
    //	okImage->setPosition(getCardPosVector(PLAYER_COUNT).at(iViewID).at(2));
//       okImage->setVisible(false);

//       m_vOkImage.at(iViewID) = okImage;
//   }

    ////叫庄按钮
    //m_CatchBankerBtn = Button::create(NN_CATCH_BTN_BG);
    //addChild(m_CatchBankerBtn);
    //m_CatchBankerBtn->setPosition(Vec2(WIN_SIZE.width / 2 - 80, WIN_SIZE.height / 2 - 100));
    //m_CatchBankerBtn->setTag(1);//1:叫
    //m_CatchBankerBtn->setVisible(false);
    //m_CatchBankerBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::catchBankerClickCallback, this));

    //Size catchBtnSize = m_CatchBankerBtn->getContentSize();
    //auto catchImage = ImageView::create(NN_CATCH_WORD);
    //catchImage->setPosition(Vec2(catchBtnSize.width / 2, catchBtnSize.height / 2));
    //m_CatchBankerBtn->addChild(catchImage);

    ////不叫按钮
    //m_NotCatchBtn = Button::create(NN_CATCH_BTN_BG);
    //addChild(m_NotCatchBtn);
    //m_NotCatchBtn->setPosition(Vec2(WIN_SIZE.width / 2 + 80, WIN_SIZE.height / 2 - 100));
    //m_NotCatchBtn->setTag(0);//0:不叫
    //m_NotCatchBtn->setVisible(false);
    //m_NotCatchBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::catchBankerClickCallback, this));

    //Size giveupBtnSize = m_NotCatchBtn->getContentSize();
    //auto giveupImage = ImageView::create(NN_GIVE_UP_WORD);
    //giveupImage->setPosition(Vec2(giveupBtnSize.width / 2, giveupBtnSize.height / 2));
    //m_NotCatchBtn->addChild(giveupImage);


//   //阶段标志
//   m_StageImage = ImageView::create(NN_STATE_IMAGE_CATCH);
//   addChild(m_StageImage);
    //m_StageImage->setAnchorPoint(getStateTipAnchor(0));
    //m_StageImage->setPosition(getStateTipPos(0));
//   m_StageImage->setVisible(false);

    //广播通知
// 		_NoticeBG = Sprite::create(Notice_Bg);
// 		_NoticeBG->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.75f));
// 		_NoticeBG->setCascadeOpacityEnabled(true);
// 		this->addChild(_NoticeBG, NOTICE_ZORDER);
// 		_NoticeBG->setVisible(false);
//
// 		_notic = GameNotice::create();
// 		_notic->setPosition(Vec2(_NoticeBG->getContentSize() / 2));
// 		_notic->setCallBack([this](){
// 			_NoticeBG->setVisible(false);
// 		});
// 		_NoticeBG->addChild(_notic);
}

//下注操作区域
void GameTableUIV99::createBetArea() {
    ostringstream ss;
    if (!_betArea.areaNode) {
		_betArea.areaNode = CSLoader::createNode(FQZS_BET_AREA_CSB_V99);
        _betArea.areaNode->setPosition(Vec2(WIN_SIZE.width / 2, 0));
        addChild(_betArea.areaNode);
    }
    //取消、续投区域
    if (!_betArea.clearResetNode) {
		_betArea.clearResetNode = CSLoader::createNode(FQZS_CLEAR_RESET_AREA_CSB_V99);
        if (_betArea.clearResetNode) {
            Vec2 pos = Vec2(WIN_SIZE.width - 20, WIN_SIZE.height - 201);

            if (_goldPool.poolNode) {
                Sprite* bg = dynamic_cast<Sprite*>(_goldPool.poolNode->getChildByName("GoldPool"));
                if (bg) {
                    pos = Vec2(WIN_SIZE.width - 20, WIN_SIZE.height - bg->getContentSize().height);
                }
            }

            _betArea.clearResetNode->setPosition(pos);
            addChild(_betArea.clearResetNode);
            //
            if (!_betArea.clearBtn) {
                _betArea.clearBtn = dynamic_cast<Button*>(_betArea.clearResetNode->getChildByName("Button_clear"));
                if (_betArea.clearBtn) {
                    _betArea.clearBtn->setTag(tag_clear);
					_betArea.clearBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::clearRebetCallBack, this));
                }
            }
            if (!_betArea.resetBtn) {
                _betArea.resetBtn = dynamic_cast<Button*>(_betArea.clearResetNode->getChildByName("Button_rebet"));
                if (_betArea.resetBtn) {
                    _betArea.resetBtn->setTag(tag_rebet);
					_betArea.resetBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::clearRebetCallBack, this));
                }
            }
        }

    }
    if (!_betArea.myGoldSprite) {
        _betArea.myGoldSprite = dynamic_cast<Sprite*>(_betArea.areaNode->getChildByName("MyCoin"));
        if (_betArea.myGoldSprite) {
            if (!_betArea.myGold) {
                _betArea.myGold = dynamic_cast<Text*>(_betArea.myGoldSprite->getChildByName("my"));
                ss << Tools::addComma(0);
                _betArea.myGold->setString(ss.str().c_str());
                ss.str("");
            }
        }
    }
    if (!_betArea.myWinSprite) {
        _betArea.myWinSprite = dynamic_cast<Sprite*>(_betArea.areaNode->getChildByName("MyWin"));
        if (_betArea.myWinSprite) {
            if (!_betArea.myWin) {
                _betArea.myWin = dynamic_cast<Text*>(_betArea.myWinSprite->getChildByName("my"));
                ss << Tools::addComma(0);
                _betArea.myWin->setString(ss.str().c_str());
                ss.str("");
            }
        }
    }
    //下注的按钮
    if (_betArea.betButtons.empty()) {
        //需要初始化
        for (int i = 0; i < Bet_Area_Num; i++) {
            Button* btn = dynamic_cast<Button*>(_betArea.areaNode->getChildByName(Bet_Button_Names[i]));
            if (btn) {
                btn->setTag(Bet_Button_Tag[i]);
				btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::betAreaCallBack, this));
                _betArea.betButtons.push_back(btn);
            }
        }
    }
    if (_betArea.btnLights.empty()) {
        //需要初始化
        for (int i = 0; i < Bet_Area_Num; i++) {
            if (_betArea.betButtons.at(i)) {
                Sprite* light = dynamic_cast<Sprite*>(_betArea.betButtons.at(i)->getChildByName("light"));
                if (light) {
                    light->setVisible(false);
                    light->setTag(Bet_Button_Tag[i]);	//按钮的光效与按钮本身使用同一个tag值
                    _betArea.btnLights.push_back(light);
                }
            }
        }
    }
    if (_betArea.myCoinText.empty()) {
        //需要初始化
        for (int i = 0; i < Bet_Area_Num; i++) {
            if (_betArea.betButtons.at(i)) {
                Text* my = dynamic_cast<Text*>(_betArea.betButtons.at(i)->getChildByName("my"));
                if (my) {
                    ss << 0;
                    my->setString(ss.str().c_str());
                    ss.str("");
                    _betArea.myCoinText.push_back(my);
                }
            }
        }
    }
    if (_betArea.allCoinText.empty()) {
        //需要初始化
        for (int i = 0; i < Bet_Area_Num; i++) {
            if (_betArea.betButtons.at(i)) {
                Text* all = dynamic_cast<Text*>(_betArea.betButtons.at(i)->getChildByName("all"));
                if (all) {
                    ss << 0;
                    all->setString(ss.str().c_str());
                    ss.str("");
                    _betArea.allCoinText.push_back(all);
                }
            }
        }
    }
    //
    initBetButtonsPosition();
    //进入游戏后，非下注阶段下注按钮禁止响应
    controlBetButtonEnable(false);
}

//动态设置押注按钮位置
void GameTableUIV99::initBetButtonsPosition() {
    const float edgeOff = 10.0f;	//左右边距
    const float upOff = 5.0f;		//距离底框距离
    float space = 0.0f;				//水平方向间距
    const float upSpace = 10;		//垂直方向控件间距
    Size aSize = Size::ZERO;//动物
    Size kSize = Size::ZERO;//类型
    Size sSize = Size::ZERO;//鲨鱼
    if (_betArea.myWinSprite) {
        _betArea.myWinSprite->setPosition(Vec2(-(WIN_SIZE.width / 2 - edgeOff), upOff));
        //if (_betArea.clearBtn)
        //{
        //	_betArea.clearBtn->setPosition(Vec2(-_betArea.clearBtn->getContentSize().width,
        //		_betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height / 2));
        //}
    }
    if (_betArea.myGoldSprite) {
        _betArea.myGoldSprite->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff, upOff));
        //if (_betArea.resetBtn)
        //{
        //	_betArea.resetBtn->setPosition(Vec2(_betArea.resetBtn->getContentSize().width,
        //		_betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height / 2));
        //}
    }
    if (!_betArea.betButtons.empty()) {
        for (int i = 0; i < Bet_Area_Num; i++) {
            if (_betArea.betButtons.at(i)) {
                if (_betArea.betButtons.at(i)->getTag() == tag_sy) {
                    sSize = _betArea.betButtons.at(i)->getContentSize();
                    m_betAreaKindSize[1] = sSize;
                    continue;
                }
                if (_betArea.betButtons.at(i)->getTag() == tag_fq || _betArea.betButtons.at(i)->getTag() == tag_zs) {
                    kSize = _betArea.betButtons.at(i)->getContentSize();
                    m_betAreaKindSize[0] = kSize;
                    continue;
                }
                aSize = _betArea.betButtons.at(i)->getContentSize();
            }
        }
        //计算水平方向间距
        space = (WIN_SIZE.width - edgeOff * 2 - aSize.width * 8 - sSize.width) / 8;
        //
        for (int i = 0; i < Bet_Area_Num; i++) {
            if (_betArea.betButtons.at(i)) {
                switch (_betArea.betButtons.at(i)->getTag()) {
                case tag_fq://飞禽
                    if (_betArea.myWinSprite) {
                        m_betAreaPosition[0] = Vec2(-(WIN_SIZE.width / 2 - edgeOff),
                                                    _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + aSize.height + upSpace);
						m_betAreaPosition.at(0).y += 20;
                        _betArea.betButtons.at(i)->setPosition(m_betAreaPosition.at(0));
                    }
                    break;
                case tag_yz://燕子
                    if (_betArea.myWinSprite) {
                        _betArea.betButtons.at(i)->setPosition(Vec2(-(WIN_SIZE.width / 2 - edgeOff),
                                                               _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace));
                    }
                    break;
                case tag_gz://鸽子
                    if (_betArea.myWinSprite) {
                        _betArea.betButtons.at(i)->setPosition(Vec2(-(WIN_SIZE.width / 2 - aSize.width * 1.5f - space - edgeOff),
                                                               _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace));
                    }
                    break;
                case tag_kq://孔雀
                    if (_betArea.myWinSprite) {
                        _betArea.betButtons.at(i)->setPosition(Vec2(-(WIN_SIZE.width / 2 - aSize.width * 2.5f - 2 * space - edgeOff),
                                                               _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace));
                    }
                    break;
                case tag_ly://老鹰
                    if (_betArea.myWinSprite) {
                        _betArea.betButtons.at(i)->setPosition(Vec2(-(WIN_SIZE.width / 2 - aSize.width * 3.5f - 3 * space - edgeOff),
                                                               _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace));
                    }
                    break;
                case tag_zs://走兽
                    if (_betArea.myGoldSprite) {
                        m_betAreaPosition[1] = Vec2(WIN_SIZE.width / 2 - edgeOff,
                                                    _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + aSize.height + upSpace);
						m_betAreaPosition.at(1).y += 20;
                        _betArea.betButtons.at(i)->setPosition(m_betAreaPosition.at(1));
                    }
                    break;
                case tag_sz://狮子
                    if (_betArea.myGoldSprite) {
                        _betArea.betButtons.at(i)->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff - aSize.width * 3.5f - 3 * space,
                                                               _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + upSpace));
                    }
                    break;
                case tag_xm://熊猫
                    if (_betArea.myGoldSprite) {
                        _betArea.betButtons.at(i)->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff - aSize.width * 2.5f - 2 * space,
                                                               _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + upSpace));
                    }
                    break;
                case tag_hz://猴子
                    if (_betArea.myGoldSprite) {
                        _betArea.betButtons.at(i)->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff - aSize.width * 1.5f - space,
                                                               _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + upSpace));
                    }
                    break;
                case tag_tz://兔子
                    if (_betArea.myGoldSprite) {
                        _betArea.betButtons.at(i)->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff,
                                                               _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + upSpace));
                    }
                    break;
                case tag_sy://鲨鱼
                    if (_betArea.myWinSprite) {
                        m_betAreaPosition[2] = Vec2(0,
                                                    _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace);
                        _betArea.betButtons.at(i)->setPosition(m_betAreaPosition.at(2));
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
}

//选择下注金额操作界面
void GameTableUIV99::createBetButtons() {
    if (!_betButton.node) {
        ostringstream ss;
		_betButton.node = CSLoader::createNode(FQZS_BET_BUTTONS_CSB_V99);
        if (_betButton.node) {
            Vec2 pos = Vec2(WIN_SIZE.width / 2, 30 + 5);
            Sprite* bg = dynamic_cast<Sprite*>(_betButton.node->getChildByName("bg"));
            if (bg) {
                pos = Vec2(WIN_SIZE.width / 2, bg->getContentSize().height / 2 + 10);
            }
            _betButton.node->setPosition(pos);
            addChild(_betButton.node);
            if (_betButton.buttons.empty()) {
                for (int i = 0; i < Bet_Count; i++) {
                    ss << "B_" << (i + 1);
                    Button* btn = dynamic_cast<Button*>(_betButton.node->getChildByName(ss.str().c_str()));
                    if (btn) {
                        btn->setTag(Bet_Money_Tag[i]);
						btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::betButtonsCallBack, this));
						btn->setScale(0.7f);
                        _betButton.buttons.push_back(btn);
                    }
                    ss.str("");
                }
            }
            if (_betButton.btnLights.empty()) {
                for (int i = 0; i < Bet_Count; i++) {
                    if (!_betButton.buttons.empty() && i < _betButton.buttons.size()) {
                        if (_betButton.buttons.at(i)) {
                            Sprite* light = dynamic_cast<Sprite*>(_betButton.buttons.at(i)->getChildByName("light"));
                            if (light) {
                                light->setTag(Bet_Money_Tag[i]);
                                light->setVisible(false);
                                _betButton.btnLights.push_back(light);
                            }
                        }
                    }
                }
            }
        }
    }
}

//彩金池
void GameTableUIV99::createPool() {
    if (!_goldPool.poolNode) {
		_goldPool.poolNode = CSLoader::createNode(FQZS_POOL_CSB_V99);
        if (_goldPool.poolNode) {
			_goldPool.poolNode->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height - 10 - 240 + 86));
            addChild(_goldPool.poolNode);
            if (!_goldPool.poolMoney) {
                _goldPool.poolMoney = dynamic_cast<Text*>(_goldPool.poolNode->getChildByName("gold"));
                ostringstream ss;
                ss << Tools::addComma(_goldPool.i64Money);
                _goldPool.poolMoney->setString(ss.str().c_str());
                ss.str("");
            }
        }
    }
}

//结果展示
void GameTableUIV99::createResult() {
    if (!_resultLabel.labelNode) {
		_resultLabel.labelNode = CSLoader::createNode(FQZS_RESULT_LABEL_CSB_V99);
        if (_resultLabel.labelNode) {
            Vec2 pos = WIN_SIZE / 2;
            if (_resultLoop.loopNode) {
                pos.x = _resultLoop.loopNode->getPositionX();
                pos.y = _resultLoop.loopNode->getPositionY();// - _betArea.myGoldSprite->getContentSize().height / 2 - 85.0f;
            }
            _resultLabel.labelNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
            _resultLabel.labelNode->setPosition(pos);
            addChild(_resultLabel.labelNode);
            //
            if (!_resultLabel.font) {
                _resultLabel.font = dynamic_cast<TextBMFont*>(_resultLabel.labelNode->getChildByName("Result"));
                if (_resultLabel.font) {
                    ostringstream ss;
                    ss << "+" << Tools::addComma(0);
                    _resultLabel.font->setString(ss.str().c_str());
                    ss.str("");
                }
            }
            _resultLabel.labelNode->setVisible(false);
        }
    }
}

//动物椭圆区域
void GameTableUIV99::createResultLoop() {
	float fTopSpace = 10.0f;
	float fCircleHeightHalf = 240.0f;

    char ss[20];
    if (!_resultLoop.loopNode) {
		_resultLoop.loopNode = CSLoader::createNode(FQZS_RESULT_LOOP_CSB_V99);
        if (_resultLoop.loopNode) {
			_resultLoop.loopNode->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height - 10 - 240));
            addChild(_resultLoop.loopNode);
        }
    }
    if (_resultLoop.animals.empty()) {
        for (int i = 0; i < Open_Area; i++) {
            memset(ss, 0, sizeof(ss));
            sprintf(ss, "A_%02d", (i + 1));
            Sprite* ani = dynamic_cast<Sprite*>(_resultLoop.loopNode->getChildByName(ss));
            if (ani) {
                Vec2 pos = ani->getPosition();
                if (_resultLoop.loopNode) {
                    pos.x += _resultLoop.loopNode->getPositionX();
                    pos.y += _resultLoop.loopNode->getPositionY();
                }
                _resultLoop.animals.push_back(ani);
                m_loopAnimalPosition.push_back(pos);
            }
        }
        //
        if (!m_loopAnimalPosition.empty()) {
            for (int i = 2; i >= 0; i--) {
				Sprite* sprite = Sprite::create(FQZS_LOOP_LIGHT_RES_V99);
                sprite->setPosition(m_loopAnimalPosition.at(i));
                sprite->setTag(i);
                sprite->setOpacity(80 * i + 75);
                sprite->setVisible(false);
                m_loopAnimationSprite.at(i) = sprite;
                addChild(m_loopAnimationSprite.at(i));
            }
        }
    }

}

//游戏历史开局情况
void GameTableUIV99::createState() {
    if (!_openRecord.node) {
		_openRecord.node = CSLoader::createNode(FQZS_STATE_CSB_V99);
        if (_openRecord.node) {
            if (_resultLoop.loopNode) {
                _openRecord.node->setPosition(Vec2(_resultLoop.loopNode->getPositionX(), _resultLoop.loopNode->getPositionY() - 75.0f));
            } else {
                _openRecord.node->setPosition(WIN_SIZE / 2);
            }
            addChild(_openRecord.node);
//             if (!_openRecord.arrowLeft) {
//                 _openRecord.arrowLeft = dynamic_cast<Button*>(_openRecord.node->getChildByName("Button_Left"));
//                 if (_openRecord.arrowLeft) {
//                     _openRecord.arrowLeft->setTag(tag_left);
//                     _openRecord.arrowLeft->setVisible(false);
// 					_openRecord.arrowLeft->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::leftRightArrowCallBack, this));
//                 }
//             }
//             if (!_openRecord.arrowRight) {
//                 _openRecord.arrowRight = dynamic_cast<Button*>(_openRecord.node->getChildByName("Button_Right"));
//                 if (_openRecord.arrowRight) {
//                     _openRecord.arrowRight->setTag(tag_right);
//                     _openRecord.arrowRight->setVisible(false);
// 					_openRecord.arrowRight->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::leftRightArrowCallBack, this));
//                 }
//             }
            //添加精灵，缩放80%
			Sprite* recordBg = dynamic_cast<Sprite*>(_openRecord.node->getChildByName("bg"));
			Size bgSize = recordBg->getContentSize();

            Vec2 posStart = Vec2(-bgSize.height/2, -2);
            Vec2 posEnd = Vec2(bgSize.height/2, -2);
//             if (_openRecord.arrowLeft) {
//                 posStart.x = _openRecord.arrowLeft->getPositionX() + _openRecord.arrowLeft->getContentSize().width - 5.0f;
//                 posStart.y = _openRecord.arrowLeft->getPositionY();
//             }
//             if (_openRecord.arrowRight) {
//                 posEnd.x = _openRecord.arrowRight->getPositionX() - _openRecord.arrowRight->getContentSize().width;
//                 posEnd.y = _openRecord.arrowRight->getPositionY();
//             }
            char res[100];
            memset(res, 0, sizeof(res));
            float width = 55.0f;	//精灵宽
            float space = 0.0f;		//精灵间距
            float scaleX = 0.8f;
			sprintf(res, FQZS_SHOW_LOOP_ANIMAL_RES_V99, 1);
            Sprite* spr = Sprite::create(res);
            if (spr) {
                //scaleX = width / spr->getContentSize().width;
				scaleX = 0.8f;
            }
            space = (posEnd.x - posStart.x - width * 10) / 9.0f;
            space = space > 0 ? space : 0;
            for (int i = 0; i < Open_Animal_Num; i++) {
                memset(res, 0, sizeof(res));
                if (!m_openRecordAnimal.empty()) {
					sprintf(res, FQZS_SHOW_LOOP_ANIMAL_RES_V99, Open_Kind[m_openRecordAnimal.at(i)]);
                } else {
					sprintf(res, FQZS_SHOW_LOOP_ANIMAL_RES_V99, 1);
                }
                Sprite* sprite = Sprite::create(res);
                if (sprite) {
                    sprite->setScale(scaleX);
                    sprite->setVisible(false);
                    _openRecord.animals.push_back(sprite);
                    _openRecord.node->addChild(_openRecord.animals.at(i));
                    _openRecord.animals.at(i)->setPosition(Vec2(posStart.x + space * i + width * (i + 0.5),
                                                           posStart.y));
                }
            }
        }
    }
}

//游戏状态label
void GameTableUIV99::createGameState() {
    if (!m_gameStateNode) {
        unschedule(schedule_selector(GameTableUIV99::showGameStateLabelAnimation));
        m_gameStateNode = CSLoader::createNode(FQZS_STATE_LABEL_CSB_V99);
        if (m_gameStateNode) {
            Vec2 pos = WIN_SIZE / 2;
            if (_resultLoop.loopNode) {
                pos = _resultLoop.loopNode->getPosition();
				pos.x += 100;
                pos.y += 30;
            }
            m_gameStateNode->setPosition(pos);
            this->addChild(m_gameStateNode);
			if (!m_gameStateSprite) {
                unschedule(schedule_selector(GameTableUIV99::showGameStateLabelAnimation));
				m_gameStateSprite = dynamic_cast<Sprite*>(m_gameStateNode->getChildByName("gameSateSp"));
				if (m_gameStateSprite) {
                     //空现阶段  
					m_gameStateSprite->setTexture("v99/zi/23.png");
        
                    //
                    schedule(schedule_selector(GameTableUIV99::showGameStateLabelAnimation), 1.0f);
                }
            }
        }
    }
}

void GameTableUIV99::leaveTheGame() {
    HNRoomLogic::getInstance()->closeRoom();
    //GamePlatform::returnPlatform(ROOMLIST);
    PlatformState* state = PlatformState::create();
    //state->setType(GAMELIST_ROOM);
	state->setType(ROOMLIST);
    StateManager::getInstance()->setState(state);
}

////进入或者短线重连恢复游戏桌子场景
void GameTableUIV99::resumeTheGameFree(GameStation_Base* pcene, BYTE gameStation) {
    m_GameStatus = pcene->bGameStation;
    updateGameStateLabelContent();
    controlBetButtonEnable(m_GameStatus == GS_XIAZHU);
    m_i64SingleNoteLimit = pcene->i64SingleNoteLimit;
    memcpy(m_iMultiplying, pcene->imultiplying, sizeof(pcene->imultiplying));
    auto leftTime = pcene->itime;
    memcpy(m_iHistory, pcene->iHistory, sizeof(pcene->iHistory));

    //刷新身上金额
    updateGoldLabel(_GameTableLogic->getMyMoney());

    //彩金
    updatePoolMoney(pcene->i64Winning);
    
    //刷新纪录
    updateStateImage(m_iHistory);

    m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_FREE, leftTime);
}

void GameTableUIV99::resumeTheGameBet(GameStation_Bet* pcene, BYTE gameStation) {
    m_GameStatus = pcene->bGameStation;
    updateGameStateLabelContent();
    controlBetButtonEnable(m_GameStatus == GS_XIAZHU);
//    playBetMusic();
    m_i64SingleNoteLimit = pcene->i64SingleNoteLimit;
    memcpy(m_iMultiplying, pcene->imultiplying, sizeof(pcene->imultiplying));
    auto leftTime = pcene->itime;
    memcpy(m_iHistory, pcene->iHistory, sizeof(pcene->iHistory));
    memcpy(m_i64OwnNotes, pcene->i64OwnNotes, sizeof(pcene->i64OwnNotes));
    memcpy(m_i64AreaNotes, pcene->i64AreaNotes, sizeof(pcene->i64AreaNotes));

    //刷新身上金额
    updateGoldLabel(_GameTableLogic->getMyMoney());

    //彩金
    updatePoolMoney(pcene->i64Winning);
    
    //刷新纪录
    updateStateImage(m_iHistory);

    //自己下的每个区域
    if (_betArea.myCoinText.size() > 0) {
        for (int i = 0; i < GAME_NOTE_COUNT; i++) {
            string moneyString = Tools::addComma(pcene->i64OwnNotes[i]);
            _betArea.myCoinText.at(i)->setString(moneyString);
        }
    }

    //所有区域下注值
    if (_betArea.allCoinText.size() > 0) {
        for (int i = 0; i < GAME_NOTE_COUNT; i++) {
            string moneyString = Tools::addComma(pcene->i64AreaNotes[i]);
            _betArea.allCoinText.at(i)->setString(moneyString);
        }
    }

    m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_BET, leftTime);

    //转盘上随机显示一个光环，其最后结束的位置就是下局转盘开始时动物的起始位置
    schedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree), 1.5f);
}

void GameTableUIV99::resumeTheGameKaiJiang(GameStation_Kaijiang* pcene, BYTE gameStation) {
    m_GameStatus = pcene->bGameStation;
    updateGameStateLabelContent();
    controlBetButtonEnable(m_GameStatus == GS_XIAZHU);
    m_i64SingleNoteLimit = pcene->i64SingleNoteLimit;
    memcpy(m_iMultiplying, pcene->imultiplying, sizeof(pcene->imultiplying));
    auto leftTime = pcene->itime;
    memcpy(m_iHistory, pcene->iHistory, sizeof(pcene->iHistory));

    memcpy(m_i64OwnNotes, pcene->i64OwnNotes, sizeof(pcene->i64OwnNotes));
    memcpy(m_i64AreaNotes, pcene->i64AreaNotes, sizeof(pcene->i64AreaNotes));

    //开的动物

    //刷新身上金额
    updateGoldLabel(_GameTableLogic->getMyMoney());

    //彩金
    updatePoolMoney(pcene->i64Winning);
    
    //刷新纪录
    updateStateImage(m_iHistory);

    //自己下的每个区域
    if (_betArea.myCoinText.size() > 0) {
        for (int i = 0; i < GAME_NOTE_COUNT; i++) {
            string moneyString = Tools::addComma(pcene->i64OwnNotes[i]);
            _betArea.myCoinText.at(i)->setString(moneyString);
        }
    }

    //所有区域下注值
    if (_betArea.allCoinText.size() > 0) {
        for (int i = 0; i < GAME_NOTE_COUNT; i++) {
            string moneyString = Tools::addComma(pcene->i64AreaNotes[i]);
            _betArea.allCoinText.at(i)->setString(moneyString);
        }
    }

    m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_KAIJIANG, leftTime);
}

void GameTableUIV99::resumeTheGameOver(GameStation_Base* pcene, BYTE gameStation) {
    m_GameStatus = pcene->bGameStation;
    m_i64SingleNoteLimit = pcene->i64SingleNoteLimit;
    memcpy(m_iMultiplying, pcene->imultiplying, sizeof(pcene->imultiplying));
    auto leftTime = pcene->itime;
    memcpy(m_iHistory, pcene->iHistory, sizeof(pcene->iHistory));

    //刷新身上金额
    updateGoldLabel(_GameTableLogic->getMyMoney());

    //彩金
    updatePoolMoney(pcene->i64Winning);
    
    //刷新纪录
    updateStateImage(m_iHistory);

// 	m_bDoTimeOutCallback = false;
// 	setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_OVER, leftTime);
}

//////////////////////////////////////////////////////////////////////////
//
//void GameTableUIV99::onBtnClick(Ref* pSender,Control::EventType event) {
//	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN_V99);
//
//    Node* btn = (Node*)pSender;
//    switch (btn->getTag()) {
//    case em_toolbarBtn: {
//		showToolBar(true);
//    }
//    break;
//    case em_toolbarSetBtn: {
//		//if (this->getChildByName("setLayer")) {
//		//	(dynamic_cast<BRNNSetLayerV99*>(this->getChildByName("setLayer")))->closeSet();
//		//	return;
//		//}
//		//auto setLayer = BRNNSetLayerV99::create();
//		//setLayer->setName("setLayer");
//		//setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//		//setLayer->showSet(this, TOOLBAR_ZORDER);
//
//		showToolBar(false);
//    }
//    break;
//    case  em_toolbarHelpBtn: {
//		showToolBar(false);
//    }
//    break;
//    case em_toolbarLeaveBtn: {
//        if (!RoomLogic()->connected()) {
//            leaveTheGame();
//        } else {
//            if (m_GameStatus != GS_FREE) {
//                // 游戏不在空闲时期，不允许离开
//                bool bHasNote = false;
//                for (int i = 0; i < GAME_NOTE_COUNT; i++) {
//                    if (m_i64OwnNotes[i] > 0) {
//                        bHasNote = true;
//                    }
//                }
//
//                if (bHasNote) {
//                    auto layer = GamePromptLayer::create();
//                    layer->showPrompt(GBKToUtf8("游戏过程中不允许离开！"));
//
//                    return;
//                }
//            }
//            _GameTableLogic->sendUserUp();
//        }
//    }
//    break;
//    case em_toolbarNextOutBtn: {
//        if (btn->getUserData() == (void*)true) {
//            btn->setUserData((void*)false);
//            btn->getChildByName("checkSprite")->setVisible(false);
//            m_bNextOut = false;
//        } else if(btn->getUserData() == (void*)false) {
//            btn->setUserData((void*)true);
//            btn->getChildByName("checkSprite")->setVisible(true);
//            m_bNextOut = true;
//        }
//    }
//    break;
//    }
//}

void GameTableUIV99::showToolBar(bool bShow) {
	//if (bShow && 0 == _toolbarBg->getName().compare("0")) {
	//	_toolbarBg->setName("1");
	//	_toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height - 10)));
	//	this->getChildByTag(em_toolbarBtn)->setVisible(false);
	//}
	//else if (!bShow && 0 == _toolbarBg->getName().compare("1")) {
	//	_toolbarBg->setName("0");
	//	_toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 10)),
	//		CallFunc::create([this]() {
	//		this->getChildByTag(em_toolbarBtn)->setVisible(true);
	//	})));
	//}
}

bool GameTableUIV99::onTouchBegan(Touch* touch, Event* event) {
    //Point point = touch->getLocation();
    //point = _tableBg->convertToNodeSpace(point);

    //if(0 == _toolbarBg->getName().compare("1")) {
    //    _toolbarBg->setName("0");

    //    _toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 10)),
    //    CallFunc::create([this]() {
    //        this->getChildByTag(em_toolbarBtn)->setVisible(true);
    //    })));
    //}

    return true;
}

void GameTableUIV99::onEnter() {
    HNGameUIBase::onEnter();
	HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_FREE_V99, true);
}

void GameTableUIV99::onExit() {
    HNGameUIBase::onExit();
}

void GameTableUIV99::onExitTransitionDidStart() {
    HNGameUIBase::onExitTransitionDidStart();
    HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameTableUIV99::addUser(BYTE seatNo, bool bMe) {
    //UserInfoStruct * user = _GameTableLogic->getUserBySeatNo(seatNo);

    //auto viewSeatNo = _GameTableLogic->logicToViewSeatNo(seatNo);

    //m_vPlayerHead.at(viewSeatNo)->setVisible(true);
    //m_vPlayerHead.at(viewSeatNo)->setHead(user->bLogoID);
    //m_vPlayerHead.at(viewSeatNo)->setNick(user->nickName);
    //m_vPlayerHead.at(viewSeatNo)->setGold(user->i64Money);
}

void GameTableUIV99::updateClock(float dt) {
    if (m_fTimeCount <= 0) {
        if (m_GameStatus == GS_XIAZHU) {
            playStopBetMusic();//停止下注
            this->unschedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree));
        }
        //m_ClockBg->setVisible(false);
        this->unschedule(schedule_selector(GameTableUIV99::updateClock));

        timeOutCallback(m_bTimerChairID, m_iTimerID);

		//m_Clock->stop();
		//m_Clock->setVisible(false);

        return;
    }

//     char s[10] = { 0 };
//     sprintf(s, "%.2d", static_cast<int>(m_fTimeCount));
//     m_ClockTimeLabel->setString(s);
//     m_ClockBg->setVisible(true);

     m_fTimeCount -= floor(dt);
 
    if (m_fTimeCount < 3) {
		HNAudioEngine::getInstance()->playEffect(GAME_CLOCK_TIME_WARNING);
    }
}

void GameTableUIV99::setTimer(BYTE bChairID, int iTimerID, float fTime) {

	m_fTimeCount = fTime - 1;
	//m_Clock->start(fTime);
	//m_Clock->setVisible(true);
	this->schedule(schedule_selector(GameTableUIV99::updateClock), 1.0f);
//     m_bTimerChairID = bChairID;
//     m_iTimerID = iTimerID;
//     m_fClockTime = fTime;
//     m_fTimeCount = fTime - 1;
//     m_bDoTimeOutCallback = true;
// 
//     //马上显示一次
//     if (m_ClockBg) {
//         switch (iTimerID) {
//         case CLIENT_TIMER_FREE:
//         case CLIENT_TIMER_BET:
//             //m_ClockBg->setPosition(getTimerPosition(true));
//             break;
//         case CLIENT_TIMER_KAIJIANG:
//         case CLIENT_TIMER_KAIJIANG_RESULT:
//         default:
//             break;
//         }
//         m_ClockBg->setVisible(true);
//         if (m_ClockTimeLabel) {
//             m_ClockTimeLabel->setVisible(true);
//         }
//     }
// 
//     char s[10] = { 0 };
//     sprintf(s, "%.2d", static_cast<int>(m_fClockTime));
//     m_ClockTimeLabel->setString(s);
// 
//     this->schedule(schedule_selector(GameTableUIV99::updateClock), 1.0f);

	if (m_clockObj && fTime >= 1) {
		m_clockObj->stop();
		m_clockObj->setVisible(true);
		m_clockObj->start(fTime);
	}
}

void GameTableUIV99::killTimer() {
	//m_Clock->stop();
	//m_Clock->setVisible(false);
//     m_ClockBg->setVisible(false);
    this->unschedule(schedule_selector(GameTableUIV99::updateClock));
	if (m_clockObj) {
		m_clockObj->stop();
		m_clockObj->setVisible(false);
	}
}

void GameTableUIV99::timeOutCallback(BYTE bChairID, int iTimerID) {
    if (_GameTableLogic->getMySeatNo() != bChairID) return;

    if (m_bDoTimeOutCallback == false) return;

    //准备超时
    if (iTimerID == CLIENT_TIMER_FREE) {
// 		killTimer();
// 		//killTimer(CLIENT_TIMER_PREPARE);
//         if (!RoomLogic()->connected()) {
//             leaveTheGame();
//         } else {
//             _GameTableLogic->sendUserUp();
//         }
    }
    //else if (iTimerID == CLIENT_TIMER_CATCH_BANKER) {
    //	killTimer();
//       catchBankerClickCallback(m_NotCatchBtn, Widget::TouchEventType::ENDED);
//   }
    //else if (iTimerID == CLIENT_TIMER_SET_MONEY) {
    //	killTimer();
    //	//如果超时，则默认使用最小金币值
    //	autoUpdateSetMoneyUI(m_i64BetArr[0]);
//   }
    //else if (iTimerID == CLIENT_TIMER_SET_BOMB) {
    //	killTimer();
    //	autoUpdateSetBombUI(NULL, rand() % 10);
//   }
    //else if (iTimerID == CLIENT_TIMER_OPEN_LE)
    //{
    //	killTimer();
    //	sendOpenLEButtonCallBack(_waitEnvelopeNode.Open_Button, Widget::TouchEventType::ENDED);
    //}
}

void GameTableUIV99::updateUIForResume() {
    //if (m_CatchBankerBtn)
    //{
    //	m_CatchBankerBtn->setVisible(false);
    //}
    //if (m_NotCatchBtn)
    //{
    //	m_NotCatchBtn->setVisible(false);
    //}
    //if (m_StageImage)
    //{
    //	m_StageImage->setVisible(false);
    //}
    //if (m_bPrepareTime)
    //{
    //	m_PrepareBtn->setVisible(false);
    //}
}
    
    void GameTableUIV99::updatePoolMoney(long long poolMoney) {
        if (poolMoney < 0) {
            return;
        }
        if (_goldPool.poolMoney) {
            _goldPool.i64Money = poolMoney;
            string poolMoneyStr = Tools::addComma(_goldPool.i64Money);
            _goldPool.poolMoney->setString(poolMoneyStr);
        }
    }

void GameTableUIV99::updateTip(float dt) {
    int iTag = m_TextTip->getTag();
    if (iTag == 0) {
        unschedule(schedule_selector(GameTableUIV99::updateTip));
        _GameTableLogic->sendUserUp();
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
    m_TextTip->setText(str);
    m_TextTip->setTag(iTag);
}

void GameTableUIV99::setBtnTouch(Button* btn, bool bTouch) {

    btn->setTouchEnabled(bTouch);
    auto image = dynamic_cast<ImageView*>(btn->getChildByName("openImage"));
    if (bTouch) {
        btn->setColor(Color3B(255, 255, 255));
        image->setColor(Color3B(255, 255, 255));
    } else {
        btn->setColor(Color3B(180, 180, 180));
        image->setColor(Color3B(180, 180, 180));
    }
}

void GameTableUIV99::tipCallback() {
    if (tipMsg.bIsExit == 1) {
        //准备关闭掉
        //m_PrepareBtn->setVisible(false);

        _layer = GamePromptLayer::create();
        //layer->setCloseExitBtn();
//         char str[128] = { 0 };
//         int sec = 3;
//         sprintf(str, "金币数额低于%lld，%ds后离开游戏！", tipMsg.i64Money, sec);
//         layer->showPrompt(GBKToUtf8(str));
        int sec = 3;
        char s[128] {0};
        std::string str;
        str.append(GBKToUtf8("金币数额低于"));
        sprintf(s, "%lld", tipMsg.i64Money);
        str.append(s);
        sprintf(s, ",%d", sec);
        str.append(s);
        str.append(GBKToUtf8("s后离开游戏！"));
		_layer->showPrompt(str);
		_layer->setOkayCallBack([&]() {
			unschedule(schedule_selector(GameTableUIV99::updateTip));
			_GameTableLogic->sendUserUp();
		});

		_layer->setColorLayerTouchEnabled(false);

        m_i64TipMoney = tipMsg.i64Money;
        m_TextTip = _layer->getPromptText();
        m_TextTip->setTag(sec);
        schedule(schedule_selector(GameTableUIV99::updateTip), 1.0f);
    } else {

    }
}

void GameTableUIV99::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
    if (bVisible) {
        if (!m_Notice) {
            m_Notice = GameNotice::create();
            m_Notice->setLocalZOrder(NOTICE_ZORDER);
            m_Notice->setName("GameNotice");
        }
        m_Notice->openNotic(this, 10000, message, nCount, nType);
    }
}

//////////////////////////////////////////////////////////////////////////
//下注操作区域按钮回调
void GameTableUIV99::betAreaCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    //HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
    playBetMoneyEffect();
    if (m_iBetMoneyType < 0 || m_iBetMoneyType >= Bet_Count) {
        return;//下注前没有选择金额
    }
    auto btn = (Button*)pSender;
    hideBetAreaSelectState();
    if (btn) {
        m_iBetType = btn->getTag();
        if (m_iBetType >= K_FQ && m_iBetType <= K_SY) {
            if (m_iBetMoneyType >= 0 && m_iBetMoneyType < Bet_Count) {
                _GameTableLogic->sendBet(m_iBetType, g_i64BetValue[m_iBetMoneyType]);
            }
        }
    }
}

//下注金额选择按钮回调
void GameTableUIV99::betButtonsCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN_V99);

    if (m_GameStatus != GS_XIAZHU) {
        controlBetButtonEnable(false);
        return;
    }

    auto btn = (Button*)pSender;

    if (btn) {
        hideBetButtonSelectState();

        int iTag = btn->getTag();
        int iMoneyType = iTag - 15;
        m_iBetMoneyType = iMoneyType;
        if (m_iBetMoneyType < Bet_Count) {

            if (!_betButton.btnLights.empty() && m_iBetMoneyType < _betButton.btnLights.size()) {
                if (_betButton.btnLights.at(m_iBetMoneyType)) {
                    if (_betButton.btnLights.at(m_iBetMoneyType)->getTag() == iTag) {
                        _betButton.btnLights.at(m_iBetMoneyType)->setVisible(true);
                    }
                }
            }
        }
    }
}

//取消、续注按钮回调
void GameTableUIV99::clearRebetCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN_V99);

    //showResultButtonAnimation(K_SZ);

    if (m_GameStatus != GS_XIAZHU) {
        controlBetButtonEnable(false);
        return;
    }

    auto btn = (Button*)pSender;

    if (btn) {

        int iTag = btn->getTag();

        if (iTag == tag_clear) {
            _GameTableLogic->sendBet(k_QK, 0);
            //其他区域恢复
            //m_betButtonTag = INVALID_BYTE;
            //controlBetMoneyArea(INVALID_BYTE, false);
            hideBetAreaSelectState();
        } else if (iTag == tag_rebet) {
            _GameTableLogic->sendBet(k_XT, 0);
        } else {

        }
    }
}

//左右按钮回调
void GameTableUIV99::leftRightArrowCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    auto btn = (Button*)pSender;
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN_V99);
    if (btn) {
        if (btn->getTag() == tag_left) {
            //向左按钮回调

        } else if (btn->getTag() == tag_right) {
            //向右按钮回调

        }
    }
}


////控制选择下注区域的私有方法，默认隐藏
//void GameTableUIV99::controlBetMoneyArea(BYTE areaKind, bool isShow)
//{
//	if (_betButton.node)
//	{
//		auto bg = dynamic_cast<Sprite*>(_betButton.node->getChildByName("bg"));
//		float width = (bg == nullptr ? 639.0f : bg->getContentSize().width);
//		if (areaKind == tag_sy)
//		{
//			//鲨鱼
//			_betButton.node->setPosition(Vec2(WIN_SIZE.width / 2 - m_betAreaPosition.at(2).x,
//				m_betAreaPosition.at(2).y + 45 + m_betAreaKindSize.at(1).height));
//		}
//		else if (areaKind >= tag_fq && areaKind <= tag_ly)
//		{
//			//所有飞禽
//			_betButton.node->setPosition(Vec2((WIN_SIZE.width / 2 + m_betAreaPosition.at(0).x) + 10 + width / 2 + m_betAreaKindSize.at(0).width,
//				m_betAreaPosition.at(0).y + 45));
//		}
//		else if (areaKind = tag_zs && areaKind <= tag_tz)
//		{
//			//所有走兽
//			_betButton.node->setPosition(Vec2(WIN_SIZE.width - (WIN_SIZE.width / 2 - m_betAreaPosition.at(1).x) - 10 - width / 2 - m_betAreaKindSize.at(0).width,
//				m_betAreaPosition.at(1).y + 45));
//		}
//		if (areaKind >= tag_fq && areaKind <= tag_sy)
//		{
//			//此时应该播放动画
//			//显示则先取消点击效果，然后放大，然后恢复效果
//			//隐藏则先取消点击效果，然后缩小，然后隐藏
//			if (_betButton.node)
//			{
//				Sequence* seq;
//				if (isShow)
//				{
//					seq = Sequence::create(CallFunc::create([=]{
//						controlBetValueButtonsEnable(false);//不管显示还是隐藏，都应该先禁止点击效果，动画播放之后才恢复
//					}), FadeIn::create(0.1f), Show::create(), ScaleTo::create(0.25f, 1.2f), DelayTime::create(0.05f), ScaleTo::create(0.15f, 1.0f), CallFunc::create([=]{
//						controlBetValueButtonsEnable(isShow);
//					}), nullptr);
//				}
//				else
//				{
//					seq = Sequence::create(CallFunc::create([=]{
//						controlBetValueButtonsEnable(false);//不管显示还是隐藏，都应该先禁止点击效果，动画播放之后才恢复
//					}), ScaleTo::create(0.3f, 1.1f), DelayTime::create(0.05f), ScaleTo::create(0.1f, 0.6f), FadeOut::create(0.1f), nullptr);
//				}
//				if (seq)
//				{
//					_betButton.node->runAction(seq);
//				}
//
//			}
//		}
//		else
//		{
//			_betButton.node->setVisible(false);
//		}
//	}
//}

//控制下注金额按钮的响应
void GameTableUIV99::controlBetValueButtonsEnable(bool canTouch /* = true */) {
    if (!_betButton.buttons.empty()) {
        for (int i = 0; i < _betButton.buttons.size(); i++) {
            if (_betButton.buttons.at(i)) {
                _betButton.buttons.at(i)->setTouchEnabled(canTouch);
            }
        }
    }
}

//控制下注按钮是否允许响应
void GameTableUIV99::controlBetButtonEnable(bool canTouch) {
    //十一种动物
    if (!_betArea.betButtons.empty()) {
        for (int i = 0; i < _betArea.betButtons.size(); i++) {
            if (_betArea.betButtons.at(i)) {
                //if (!canTouch)
                //{
                //	_betArea.betButtons.at(i)->setScale(1.0f);
                //}
                _betArea.betButtons.at(i)->setTouchEnabled(canTouch);
            }
        }
    }
    //五个下注金额
    if (!_betButton.buttons.empty()) {
        for (int i = 0; i < _betButton.buttons.size(); i++) {
            if (_betButton.buttons.at(i)) {
                //if (!canTouch)
                //{
                //_betButton.buttons.at(i)->setScale(1.0f);
                //}
                _betButton.buttons.at(i)->setTouchEnabled(canTouch);
            }
        }
    }
    if (!canTouch) {
        hideBetButtonSelectState();
    }
    controlClearButtonEnable(canTouch);
    controlRebetButtonEnable(canTouch && m_iPrevHasNote);
}

//隐藏金额按钮全部选中效果
void GameTableUIV99::hideBetButtonSelectState() {
    if (!_betButton.btnLights.empty()) {
        for (int i = 0; i < _betButton.btnLights.size(); i++) {
            if (_betButton.btnLights.at(i)) {
                _betButton.btnLights.at(i)->setVisible(false);
            }
        }
    }
}

//控制清空按钮
void GameTableUIV99::controlClearButtonEnable(bool canTouch /* = true */) {
    if (_betArea.clearBtn) {
        _betArea.clearBtn->setBright(canTouch);
        _betArea.clearBtn->setTouchEnabled(canTouch);
    }
}

//控制续投按钮
void GameTableUIV99::controlRebetButtonEnable(bool canTouch /* = true */) {
    if (_betArea.resetBtn) {
        _betArea.resetBtn->setBright(canTouch);
        _betArea.resetBtn->setTouchEnabled(canTouch);
    }
}

void GameTableUIV99::hideBetAreaSelectState() {
    //选择之后隐藏
    if (!_betArea.btnLights.empty()) {
        for (int i = 0; i < Bet_Area_Num; i++) {
            if (_betArea.btnLights.at(i)) {
                _betArea.btnLights.at(i)->stopAllActions();
                _betArea.btnLights.at(i)->setVisible(false);
            }
        }
    }
}

void GameTableUIV99::resetTextValue() {
    //自己下的每个区域
    if (_betArea.myCoinText.size() > 0) {
        for (int i = 0; i < GAME_NOTE_COUNT; i++) {
            _betArea.myCoinText.at(i)->setString("0");
        }
    }

    //所有区域下注值
    if (_betArea.allCoinText.size() > 0) {
        for (int i = 0; i < GAME_NOTE_COUNT; i++) {
            _betArea.allCoinText.at(i)->setString("0");
        }
    }
}

////下注区域超时则自动隐藏
//void GameTableUIV99::autoHideBetButtons()
//{
//	if (m_GameStatus != GS_XIAZHU)
//	{
//		this->stopActionByTag(AutoHideBetButtons_Tag);
//	}
//	//允许自动隐藏
//	this->stopActionByTag(AutoHideBetButtons_Tag);
//	auto toolbarSeq = Sequence::create(DelayTime::create(3.0f), CallFunc::create([this]() {
//		//同一个连续按两次，隐藏，不发送
//		m_betButtonTag = INVALID_BYTE;
//		//controlBetMoneyArea(INVALID_BYTE, false);
//		hideBetAreaSelectState();
//	}), nullptr);
//	toolbarSeq->setTag(AutoHideBetButtons_Tag);
//	this->runAction(toolbarSeq);
//}

//////////////////////////////////////////////////////////////////////////
//动画
void GameTableUIV99::showRunLoopAnimation(BYTE endIndex, BYTE loopTime, float delayTime) {
    if (endIndex < 0 || endIndex >= Open_Area) {
        //转盘上的序号是 0-27
        return;
    }
    if (m_loopAnimationSprite.empty()) {
        for (int i = 2; i >= 0; i--) {
			Sprite* sprite = Sprite::create(FQZS_LOOP_LIGHT_RES_V99);
            sprite->setPosition(m_loopAnimalPosition.at(i));
            sprite->setTag(i);
            sprite->setOpacity(80 * i + 75);
            sprite->setVisible(false);
            m_loopAnimationSprite.at(i) = sprite;
            addChild(m_loopAnimationSprite.at(i));
        }
    }
    //
    m_loopAnimationEndIndex = endIndex;
    int startIndex = 1;
    if (!m_loopAnimationSprite.empty()) {
        startIndex = m_loopAnimationSprite.at(2) == nullptr ? 1 : (m_loopAnimationSprite.at(2)->getTag());//若精灵在，以上一局第一个精灵的位置作为起点，否则0号位置作为起点
    }
    m_loopAnimationRemainTime = loopTime * Open_Area + ((endIndex - startIndex) <= 0 ? Open_Area : 0) + (endIndex - startIndex);//精灵要移动的数量，超过一圈要做修正
    m_loopAnimationMaxTime = m_loopAnimationRemainTime;//

    schedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition), 0.16);//前10个，慢速，0.16s一帧，共1.6s
    playLoopEffectAudio(Loop_Start);
}

//至少会转动4圈，有116-140个
void GameTableUIV99::updateLoopSpritePosition(float dt) {
    vector<Vec2> spriteNewPos;	//下一帧三个精灵应该在的位置
    m_loopAnimationRemainTime--;
    if (m_loopAnimationRemainTime < 0) {
        showOpenAnimalAnimation(m_loopAnimationEndIndex);
        if (m_loopAnimationSprite.at(0)) {
            m_loopAnimationSprite.at(0)->stopAllActions();
            m_loopAnimationSprite.at(0)->setVisible(false);
        }
        if (m_loopAnimationSprite.at(1)) {
            m_loopAnimationSprite.at(1)->stopAllActions();
            m_loopAnimationSprite.at(1)->setVisible(false);
        }
        //////////////////////////////////////////////////////////////////////////
        m_loopAnimationRemainTime = 0;
        m_loopAnimationMaxTime = 0;
        //m_loopAnimationEndIndex = INVALID_BYTE;
        unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
        return;
    }
    if (!m_loopAnimationSprite.empty()) {
        for (int i = 0; i < 3; i++) {
            if (m_loopAnimationSprite.at(i)) {
                int tag = (m_loopAnimationSprite.at(i)->getTag() + 1) % Open_Area;
                m_loopAnimationSprite.at(i)->setTag(tag);//更新tag
                spriteNewPos.push_back(m_loopAnimalPosition.at(tag));
            }
        }
        //
        for (int i = 0; i < 3; i++) {
            if (m_loopAnimationSprite.at(i)) {
                Sequence* seq = Sequence::create(CallFunc::create([=] {
                    if (m_loopAnimationSprite.at(i)) {
                        m_loopAnimationSprite.at(i)->setVisible(false);
                    }
                }), CallFunc::create([=] {
                    if (m_loopAnimationSprite.at(i)) {
                        m_loopAnimationSprite.at(i)->setPosition(spriteNewPos.at(i));
                        m_loopAnimationSprite.at(i)->setVisible(true);
                    }
                }), nullptr);
                //
                m_loopAnimationSprite.at(i)->runAction(seq);
            }
        }
        //
        if (m_loopAnimationRemainTime <= 0) {
            showOpenAnimalAnimation(m_loopAnimationEndIndex);
            if (m_loopAnimationSprite.at(0)) {
                m_loopAnimationSprite.at(0)->stopAllActions();
                m_loopAnimationSprite.at(0)->setVisible(false);
            }
            if (m_loopAnimationSprite.at(1)) {
                m_loopAnimationSprite.at(1)->stopAllActions();
                m_loopAnimationSprite.at(1)->setVisible(false);
            }
            //////////////////////////////////////////////////////////////////////////
            m_loopAnimationRemainTime = 0;
            m_loopAnimationMaxTime = 0;
            //m_loopAnimationEndIndex = INVALID_BYTE;
            unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
            return;
        }
    }
    //
    if (m_loopAnimationRemainTime == m_loopAnimationMaxTime - 9) {
        playLoopEffectAudio(Loop_Max_Speed);
        unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
        schedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition), (7.5f - 5.0f) * 0.9f / (m_loopAnimationMaxTime - 20));
    } else if (m_loopAnimationRemainTime == 15) {
        unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
        schedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition), 0.1f);//共1.2s
    } else if (m_loopAnimationRemainTime == 5) {
        playLoopEffectAudio(Loop_Slow_Speed);
        unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
        schedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition), 0.24f); //共1.25s
    } else if (m_loopAnimationRemainTime <= 0) {
        playLoopEffectAudio(Loop_End);
        m_loopAnimationRemainTime = 0;
        unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
    }
}

//下注状态光圈随机闪烁，其最后停止的位置即为下局转盘开始的位置
void GameTableUIV99::updateRandomBlinkLightFree(float dt) {
    if (m_GameStatus != GS_XIAZHU) {
        unschedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree));
        return;
    }
    m_loopAnimationEndIndex = rand() % Open_Area;
    updateBlinkOpenAnimalLight();
}

//开奖后展示开奖动物相关结果动画，如下注区域闪烁，开的动物光环闪烁
void GameTableUIV99::showResultAnimalAnimation(BYTE animalKind, float showTime) {
    m_openResultAnimalKind.push_back(animalKind);
    float singleTime = 0.8f;
    schedule(schedule_selector(GameTableUIV99::updateResultAnimation), singleTime, (unsigned int)(floor(showTime / singleTime)), 0.1f);

}

void GameTableUIV99::updateResultAnimation(float dt) {
    if (m_openResultAnimalKind.empty()) {
        unschedule(schedule_selector(GameTableUIV99::updateResultAnimation));
        stopBlinkAnimalLight();
        return;
    }
    updateBlinkOpenAnimalLight();

    for (int i = 0; i < m_openResultAnimalKind.size(); i++) {
        if (m_openResultAnimalKind.at(i) >= K_YZ && m_openResultAnimalKind.at(i) <= K_LY) {
            //飞禽类则飞禽也要展示
            m_openResultAnimalIndex.push_back(0);
            //
            BYTE index = getBetButtonIndexByKindID(m_openResultAnimalKind.at(i));
            if (index != INVALID_BYTE) {
                //其他类型
                m_openResultAnimalIndex.push_back(index);
            }
        } else if (m_openResultAnimalKind.at(i) >= K_SZ && m_openResultAnimalKind.at(i) <= K_TZ) {
            //走兽类则走兽也要展示
            m_openResultAnimalIndex.push_back(5);
            //
            BYTE index = getBetButtonIndexByKindID(m_openResultAnimalKind.at(i));
            if (index != INVALID_BYTE) {
                //其他类型
                m_openResultAnimalIndex.push_back(index);
            }
        } else if (m_openResultAnimalKind.at(i) == K_TP) {
            //通赔
            for (int i = 0; i < Bet_Area_Num; i++) {
                m_openResultAnimalIndex.push_back(i);
            }
        } else {
            //鲨鱼
            BYTE index = getBetButtonIndexByKindID(m_openResultAnimalKind.at(i));
            if (index != INVALID_BYTE) {
                //其他类型
                m_openResultAnimalIndex.push_back(index);
            }
        }
    }
    //
    if (!m_openResultAnimalIndex.empty()) {
        for (int i = 0; i < m_openResultAnimalIndex.size(); i++) {
            if (!_betArea.btnLights.empty() && m_openResultAnimalIndex.at(i) < _betArea.btnLights.size()) {
                if (_betArea.btnLights.at(m_openResultAnimalIndex.at(i))) {
                    Sequence* seq = Sequence::create(CallFunc::create([=] {
                        if (!m_openResultAnimalIndex.empty())
                            _betArea.btnLights.at(m_openResultAnimalIndex.at(i))->setVisible(false);
                    }), DelayTime::create(0.2f), CallFunc::create([=] {
                        if (!m_openResultAnimalIndex.empty())
                            _betArea.btnLights.at(m_openResultAnimalIndex.at(i))->setVisible(true);
                    }), DelayTime::create(0.28f), CallFunc::create([=] {
                        if (!m_openResultAnimalIndex.empty())
                            _betArea.btnLights.at(m_openResultAnimalIndex.at(i))->setVisible(false);
                    }), nullptr);
                    //
                    if (!m_openResultAnimalIndex.empty())
                        _betArea.btnLights.at(m_openResultAnimalIndex.at(i))->runAction(seq);
                }
            }
        }
    } else {
        unschedule(schedule_selector(GameTableUIV99::updateResultAnimation));
        stopBlinkAnimalLight();
    }
}

//开启的动画位置的光环交替闪烁
void GameTableUIV99::updateBlinkOpenAnimalLight() {
    if (!m_loopAnimationSprite.empty()) {
        for (int i = 0; i < m_loopAnimationSprite.size(); i++) {
            if (m_loopAnimationSprite.at(i)) {
                int idx = m_loopAnimationEndIndex - 2 + i;
                while (idx < 0 || idx >= Open_Area) {
                    idx = (idx + Open_Area) % Open_Area;//防止存在异常
                }

                if (!m_loopAnimalPosition.empty() && idx >= 0 && idx < m_loopAnimalPosition.size()) {
                    m_loopAnimationSprite.at(i)->setVisible(false);
                    m_loopAnimationSprite.at(i)->setTag(idx);
                    m_loopAnimationSprite.at(i)->setPosition(m_loopAnimalPosition.at(idx));
                }
            }
        }
    }

    if (!m_loopAnimationSprite.empty()) {
        if (m_loopAnimationSprite.size() >= 3) {
            if (m_loopAnimationSprite.at(0)) {
                m_loopAnimationSprite.at(0)->stopAllActions();
                m_loopAnimationSprite.at(0)->setVisible(false);
            }
            if (m_loopAnimationSprite.at(1)) {
                m_loopAnimationSprite.at(1)->stopAllActions();
                m_loopAnimationSprite.at(1)->setVisible(false);
            }
            //动画
            if (m_loopAnimationSprite.at(2)) {
                m_loopAnimationSprite.at(2)->stopAllActions();
                Sequence* seq = Sequence::create(Hide::create(), DelayTime::create(0.2f), Show::create(), DelayTime::create(0.28f), Hide::create(), nullptr);
                if (seq) {
                    m_loopAnimationSprite.at(2)->runAction(seq);
                }
            }
        }
    }

}

//更新阶段文字
void GameTableUIV99::updateGameStateLabelContent() {
    if (m_gameStateNode) {
		string pathUrl;
        switch (m_GameStatus) {
        case GS_FREE:
			pathUrl = "v99/zi/23.png";
            break;
        case GS_XIAZHU:
			pathUrl = "v99/zi/24.png";
            break;
        case GS_KAIJIANG:
			pathUrl = "v99/zi/22.png";
            break;
		case GS_GAME_FINISH:
			pathUrl = "";
			break;
        default:
			pathUrl = "v99/zi/23.png";
            break;
        }
		if (m_gameStateSprite) {
			m_gameStateSprite->setVisible(false);
			m_gameStateSprite->setTexture(pathUrl);
			m_gameStateSprite->setVisible(true);
        }
    }
}

//阶段文字动画显示
void GameTableUIV99::showGameStateLabelAnimation(float dt) {
    if (m_gameStateNode) {
        Sequence* seq = Sequence::create(CallFunc::create([this] {
            m_gameStateNode->setVisible(false);
        }), DelayTime::create(0.3f), CallFunc::create([this] {
            m_gameStateNode->setVisible(true);
        }), DelayTime::create(0.7f), CallFunc::create([this] {
            m_gameStateNode->setVisible(false);
        }), nullptr);
        m_gameStateNode->runAction(seq);
    }
}

//隐藏三个转盘上动物光效
void GameTableUIV99::stopBlinkAnimalLight() {
    this->unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
    if (!m_loopAnimationSprite.empty()) {
        for (int i = 0; i < m_loopAnimationSprite.size(); i++) {
            if (m_loopAnimationSprite.at(i)) {
                m_loopAnimationSprite.at(i)->stopAllActions();
                m_loopAnimationSprite.at(i)->setVisible(false);
            }
        }
    }
}

void GameTableUIV99::showOpenAnimalAnimation(BYTE index) {
    if (index < 0 || index >= Open_Area) {
        return;
    }
    playOpenEffectAudio(Area_Kind[index]);
    if (Area_Kind[index] == K_TS) {
        //通杀
        tongpeiTongshaAnimation(false);
    } else if (Area_Kind[index] == K_TP) {
        //通赔
        tongpeiTongshaAnimation(true);
    } else if (Area_Kind[index] == K_YZ) {
		playAnimalsAnimation(FQZS_ANIMATION_YZ_CSB_V99, "swallow");
    } else if (Area_Kind[index] == K_GZ) {
		playAnimalsAnimation(FQZS_ANIMATION_GZ_CSB_V99, "dove");
    } else if (Area_Kind[index] == K_KQ) {
		playAnimalsAnimation(FQZS_ANIMATION_KQ_CSB_V99, "peacock");
    } else if (Area_Kind[index] == K_LY) {
		playAnimalsAnimation(FQZS_ANIMATION_LY_CSB_V99, "eagle");
    } else if (Area_Kind[index] == K_SZ) {
		playAnimalsAnimation(FQZS_ANIMATION_SZ_CSB_V99, "lion");
    } else if (Area_Kind[index] == K_XM) {
		playAnimalsAnimation(FQZS_ANIMATION_XM_CSB_V99, "panda");
    } else if (Area_Kind[index] == K_HZ) {
		playAnimalsAnimation(FQZS_ANIMATION_HZ_CSB_V99, "monkey");
    } else if (Area_Kind[index] == K_TZ) {
		playAnimalsAnimation(FQZS_ANIMATION_TZ_CSB_V99, "rabbit");
    } else if (Area_Kind[index] == K_SY) {
		playAnimalsAnimation(FQZS_ANIMATION_SY_CSB_V99, "blueshark");
    } else if (Area_Kind[index] == K_JS) {
		playAnimalsAnimation(FQZS_ANIMATION_JS_CSB_V99, "goldenshark");
    }
}

//通用播放开奖动物的动画
void GameTableUIV99::playAnimalsAnimation(std::string res, std::string tagName) {
    stopAnimalsAnimation();
    m_animalAnimationNode = CSLoader::createNode(res);
    if (m_animalAnimationNode) {
        m_animalAnimationNode->setPosition(WIN_SIZE / 2);
        addChild(m_animalAnimationNode);

        auto action = CSLoader::createTimeline(res);
        m_animalAnimationNode->runAction(action);
        action->play(tagName, false);
    }
}

//停止并移除通用动画
void GameTableUIV99::stopAnimalsAnimation() {
    if (m_animalAnimationNode) {
        m_animalAnimationNode->stopAllActions();
        m_animalAnimationNode->removeFromParent();
    }
    m_animalAnimationNode = nullptr;
}

//通杀通赔  true 通赔
void GameTableUIV99::tongpeiTongshaAnimation(bool flag) {
    auto end = flag != true ? 19 : 18;
    auto iconPath = flag != true ? "common/animation/tongsha/0.png" : "common/animation/tongpei/0.png";
    auto icon = Sprite::create(iconPath);
    if (icon) {
        icon->setName("AllAnimation");
        Vec2 pos = WIN_SIZE / 2;
        if (_resultLoop.loopNode) {
            pos = _resultLoop.loopNode->getPosition();
        }
        icon->setPosition(pos);
        addChild(icon);
        std::string path;
        auto animation = Animation::create();
        for (int i = 0; i < end; i++) {
            char szName[100] = { 0 };
            std::string path = flag != true ? "common/animation/tongsha/%d.png" : "common/animation/tongpei/%d.png";
            sprintf(szName, path.c_str(), i);
            animation->addSpriteFrameWithFile(szName);
        }
        // should last 2.8 seconds. And there are 14 frames.
        animation->setDelayPerUnit(1.5f / end);
        animation->setRestoreOriginalFrame(true);

        auto action = Animate::create(animation);
        icon->runAction(action);
        icon->runAction(Sequence::create(action, CallFunc::create([&]() {
            this->getChildByName("AllAnimation")->removeFromParent();
        }), nullptr));
    }
}

void GameTableUIV99::showResultAnimation(long long changeMoney) {
    ostringstream ss;
    bool isNegative = (changeMoney >= 0 ? false : true);
    ss << (isNegative ? "" : "+") << Tools::addComma(changeMoney);
    long long newMoney = _GameTableLogic->getMyMoney();
    if (_resultLabel.labelNode) {
        Vec2 startPos = _resultLabel.labelNode->getPosition();
        Vec2 endPos = _resultLabel.labelNode->getPosition();
        endPos.y += (isNegative ? -30 : 30);
        _resultLabel.labelNode->setVisible(true);
        if (_resultLabel.font) {
            _resultLabel.font->setColor(isNegative ? Color3B::GREEN : Color3B::RED);
            _resultLabel.font->setString(ss.str().c_str());
			HNAudioEngine::getInstance()->playEffect((isNegative ? GAME_LOSE_AUDIO_V99 : GAME_WIN_AUDIO_V99));
            //
            Sequence* seq = Sequence::create(MoveTo::create(1.0f, endPos),
                                             DelayTime::create(1.0f),
            CallFunc::create([=] {
                _resultLabel.labelNode->setVisible(false);
                _resultLabel.labelNode->setPosition(startPos);
                updateGoldLabel(newMoney);
            }), nullptr);

            _resultLabel.labelNode->runAction(seq);
        }
    }
}

//传入新的开局记录数组，先将数组的值保存，然后再更新UI显示
void GameTableUIV99::updateStateImage(int iHistory[]) {

    vector<int> kinds;
    for (int i = 0; i < Open_Animal_Num; i++) {
        if (iHistory[i] == 0)
            continue;
        kinds.push_back(iHistory[i]);
    }

    if (kinds.empty()) {
        return;
    }
    for (int i = 0; i < Open_Animal_Num; i++) {
        if (i < kinds.size()) {
            m_openRecordAnimal.at(i) = kinds.at(i);
        }
    }
    //
    if (_openRecord.animals.empty()) {
        createState();
    }
    char res[100];
    memset(res, 0, sizeof(res));
    for (int i = 0; i < kinds.size(); i++) {
        if (!m_openRecordAnimal.empty()) {
			sprintf(res, FQZS_SHOW_LOOP_ANIMAL_RES_V99, m_openRecordAnimal.at(i));
        } else {
			sprintf(res, FQZS_SHOW_LOOP_ANIMAL_RES_V99, 1);
        }
        if (_openRecord.animals.at(i)) {
            //i不是最后一个
            if (i != kinds.size() - 1) {
                _openRecord.animals.at(i)->setVisible(true);
                _openRecord.animals.at(i)->setTexture(res);
            } else {
                _openRecord.animals.at(i)->setVisible(false);
                _openRecord.animals.at(i)->setTexture(res);
                animationMoveOpenItemToStateLabel(i);
            }
        }
    }
}

//将开奖的icon移动到走势图最后的位置，endIdx为最后一个
void GameTableUIV99::animationMoveOpenItemToStateLabel(int endIdx) {
    Vec2 startPos = Vec2::ZERO;
    Vec2 endPos = Vec2::ZERO;

    if (!_openRecord.animals.empty() && endIdx < _openRecord.animals.size()) {
        if (_openRecord.animals.at(endIdx)) {
            endPos = _openRecord.animals.at(endIdx)->getPosition();
        }
    }
    if (m_loopAnimationEndIndex != -1) {
        if (!m_loopAnimalPosition.empty()) {
            startPos = m_loopAnimalPosition.at(m_loopAnimationEndIndex);
        }
        if (_openRecord.node) {
            startPos.x -= _openRecord.node->getPositionX();
            startPos.y -= _openRecord.node->getPositionY();
        }
    } else {
        startPos = endPos;
    }
    //animation
    if (!_openRecord.animals.empty() && endIdx < _openRecord.animals.size()) {
        if (_openRecord.animals.at(endIdx)) {
            float scaleValue = _openRecord.animals.at(endIdx)->getScale();
            Sequence* seq = Sequence::create(
                                FadeIn::create(0.3f),
                                DelayTime::create(0.3f),
            CallFunc::create([=] {
                if (!_openRecord.animals.empty() && endIdx < _openRecord.animals.size()) {
                    if (_openRecord.animals.at(endIdx)) {
                        _openRecord.animals.at(endIdx)->setPosition(startPos);
                        _openRecord.animals.at(endIdx)->setVisible(true);
                    }
                }
            }),
            MoveTo::create(0.5f, endPos),
            ScaleTo::create(0.3f, 1.1f),
            DelayTime::create(0.1f),
            ScaleTo::create(0.1f, scaleValue), nullptr);

            _openRecord.animals.at(endIdx)->runAction(seq);
        }
    }
}

//请下注 动画
void GameTableUIV99::animationTipBetBegin() {
    Vec2 pos = WIN_SIZE / 2;
    if (_openRecord.node) {
        pos = _openRecord.node->getPosition();
    }
	if (getChildByTag(FQZS_ANIMATION_BET_NODE_TAG_V99)) {
		getChildByTag(FQZS_ANIMATION_BET_NODE_TAG_V99)->stopAllActions();
		getChildByTag(FQZS_ANIMATION_BET_NODE_TAG_V99)->removeFromParent();
    }

	auto node = CSLoader::createNode(FQZS_ANIMATION_BET_CSB_V99);
    if (node) {
        node->setPosition(pos);
		node->setTag(FQZS_ANIMATION_BET_NODE_TAG_V99);
        addChild(node);
        //
		auto action = CSLoader::createTimeline(FQZS_ANIMATION_BET_CSB_V99);
        node->runAction(action);
        action->play("bet", false);
    }
}

//请开奖动画，播放完动画后转圈开奖
void GameTableUIV99::animationTipStartCircle() {
	auto node = CSLoader::createNode(FQZS_ANIMATION_TIP_START_CIRCLE_CSB_V99);
    if (node) {
        Vec2 pos = WIN_SIZE / 2;
        if (_openRecord.node) {
            pos = _openRecord.node->getPosition();
        }
        node->setPosition(pos);
        addChild(node);
    }
    //
	auto action = CSLoader::createTimeline(FQZS_ANIMATION_TIP_START_CIRCLE_CSB_V99);
    if (action) {
        node->runAction(action);
        action->play("lottery", false);
    }
}

void GameTableUIV99::playFreeMusic() {
//    HNAudioEngine::getInstance()->stopBackgroundMusic();
//    HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_FREE, true);
}

void GameTableUIV99::playBetMusic() {
//    HNAudioEngine::getInstance()->stopBackgroundMusic();
//    HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_BET, true);
	HNAudioEngine::getInstance()->playEffect(AUDIO_TIP_BET_V99);
}

void GameTableUIV99::playStopBetMusic() {
	HNAudioEngine::getInstance()->playEffect(AUDIO_TIP_STOP_BET_V99);
}

void GameTableUIV99::playBetMoneyEffect() {
	HNAudioEngine::getInstance()->playEffect(AUDIO_BET_MONEY_V99);
}

void GameTableUIV99::playOpenMusic() {
//    HNAudioEngine::getInstance()->stopBackgroundMusic();
//    HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_OPEN, true);
}

void GameTableUIV99::playOpenEffectAudio(BYTE kind) {
    if (kind == INVALID_BYTE) {
        return;
    }
    char res[100];
    memset(res, 0, sizeof(res));
	sprintf(res, AUDIO_KIND_RES_V99, kind);
    HNAudioEngine::getInstance()->playEffect(res);
}

void GameTableUIV99::playLoopEffectAudio(BYTE type) {
    switch (type) {
    case Loop_Start:
		HNAudioEngine::getInstance()->playEffect(AUDIO_LOOP_START_V99);
        break;
    case Loop_Max_Speed:
		HNAudioEngine::getInstance()->playEffect(AUDIO_LOOP_MAX_SPEED_V99);
        break;
    case Loop_Slow_Speed:
		HNAudioEngine::getInstance()->playEffect(AUDIO_LOOP_SLOW_SPEED_V99);
        break;
    case Loop_End:
		HNAudioEngine::getInstance()->playEffect(AUDIO_LOOP_END_V99);
        break;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
//传入服务端开的动物类型，返回押注区域对应按钮的索引编号
BYTE GameTableUIV99::getBetButtonIndexByKindID(BYTE kindID) {
    BYTE index = INVALID_BYTE;
    for (int i = 0; i < Bet_Area_Num; i++) {
        if (Bet_Button_ID[i] == kindID) {
            index = i;
            break;
        }
    }
    return index;
}

//传入服务端转盘结束的位置，将该位置转换成对应的动物类型
BYTE GameTableUIV99::getAnimalKindByLoopIndex(BYTE endIndex) {
    BYTE kindID = INVALID_BYTE;
    if (endIndex < 0 || endIndex >= Open_Area) {
        return kindID;
    }
    kindID = Area_Kind[endIndex];
    return kindID;
}

void GameTableUIV99::updateMyWinLabel(long long money) {
    if (_betArea.myWin) {
        ostringstream ss;
        m_allMyWinGold += money;
        ss << Tools::addComma(m_allMyWinGold);
        _betArea.myWin->setString(ss.str().c_str());
        ss.str("");
    }
}

void GameTableUIV99::updateGoldLabel(long long money) {
    if (_betArea.myGold && money >= 0) {
        string moneyStr = Tools::addComma(money);
        _betArea.myGold->setString(moneyStr);
    }
}

////庄家发红包按钮回调
//void GameTableUIV99::sendLEButtonCallBack(Ref* pSender, Widget::TouchEventType type)
//{
//	if (type != Widget::TouchEventType::ENDED)
//	{
//		return;
//	}
//	auto btn = (Button*)pSender;
//	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//	//
//	if (btn && _envelope_Value.bChairID != INVALID_BYTE)
//	{
//		btn->setVisible(false);
//		showOpenListForBanker();
//	}
//}
//
////闲家拆红包
//void GameTableUIV99::sendOpenLEButtonCallBack(Ref* pSender, Widget::TouchEventType type)
//{
//	if (type != Widget::TouchEventType::ENDED)
//	{
//		return;
//	}
//    //killTimer();
//    auto btn = (Button*)pSender;
//	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//	if (btn)
//	{
//		//通知服务端闲家红包已经拆开
//		_GameTableLogic->sendHasOpenLE(_GameTableLogic->getMySeatNo());
//		showOpenLE(btn);
//	}
//}
//
//void GameTableUIV99::prepareClickCallback(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED) {
//        return;
//    }
//
//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//    if (m_GameStatus != GS_FREE) {
//        return;
//    }
//
//    m_PrepareBtn->setVisible(false);
//
//    _GameTableLogic->sendPrepare(_GameTableLogic->getMySeatNo());
//	for (int i = 0; i < PLAYER_COUNT; i++)
//	{
//		if (m_vSettlementValueLabel.at(i))
//		{
//			m_vSettlementValueLabel.at(i)->setVisible(false);
//		}
//	}
//
//    killTimer();
//	//resetUINewGame();
//}
//
//void GameTableUIV99::catchBankerClickCallback(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED) {
//        return;
//    }
//
//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//    if (_GameTableLogic->getMySeatNo() != m_bNextUser)	return;
//
//    if (m_GameStatus != GS_CATCH_BANKER)	return;
//
//
//    Button* pBtn = (Button*)pSender;
//    BYTE tag =static_cast<BYTE>(pBtn->getTag());
//
//    m_CatchBankerBtn->setVisible(false);
//    m_NotCatchBtn->setVisible(false);
//
//    _GameTableLogic->sendCatchData(_GameTableLogic->getMySeatNo(), tag);
//
//    killTimer();
//}
//
//void GameTableUIV99::betMoneyClickCallback(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED) {
//        return;
//    }
//
//	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//	if (m_GameStatus != GS_SET_LE_MONEY)	return;
//
//    Button* btn = (Button*)pSender;
//    int tag = btn->getTag();
//
//	autoUpdateSetMoneyUI(m_i64BetArr[tag]);
//
//    //_GameTableLogic->sendBet(_GameTableLogic->getMySeatNo(), m_i64BetArr[tag]);
//
//    //killTimer();
//    m_bDoTimeOutCallback = false;
//}
//
//void GameTableUIV99::sliderButtonCallBack(Ref* pSender, ui::Slider::EventType type)
//{
//	Slider* pSlider = static_cast<Slider*>(pSender);
//
//	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
//	{
//		//滑动条有响应事件，此时若有动画则停止提示动画
//		showBombOperationAnimation(false);
//		//此处雷的传入参数实际无效，该参数只在协议里生效
//		autoUpdateSetBombUI(pSlider, 0);
//	}
//}

////庄家操作超时，客户端自动设置红包雷数字
//void GameTableUIV99::autoUpdateSetBombUI(Slider* sliderNode, int bomb)
//{
//	ostringstream ss;
//	float percent;
//	int bombValue;
//	if (sliderNode == NULL)
//	{
//		bombValue = bomb;
//		if (_setEnvelopeNode.Bomb_Number_Slider_Operation)
//		{
//			sliderNode = _setEnvelopeNode.Bomb_Number_Slider_Operation;
//		}
//		else
//		{
//			sliderNode = nullptr;
//			return;
//		}
//	}
//	else
//	{
//		//操作进度条百分比
//		percent = sliderNode->getPercent();
//		for (int i = 0; i < 10; i++)
//		{
//			if (percent < Slider_Operation_Percent[0])
//			{
//				bombValue = 0;
//				break;
//			}
//			if (i == 9)
//			{
//				bombValue = 9;
//				break;
//			}
//			if (percent >= Slider_Operation_Percent[i] && percent <= Slider_Operation_Percent[i + 1])
//			{
//				bombValue = (percent < (Slider_Operation_Percent[i] + 5)) ? i : (i + 1);
//				break;
//			}
//
//		}
//	}
//
//	if (bombValue >= 0 && bombValue < 10)
//	{
//		sliderNode->setPercent(Slider_Operation_Percent[bombValue]);
//		if (_setEnvelopeNode.Bomb_Number_Slider)
//		{
//			_setEnvelopeNode.Bomb_Number_Slider->setPercent(Slider_State_Percent[bombValue]);
//		}
//	}
//
//
//	ss << bombValue;
//	_envelope_Value.iBombValue = bombValue;
//
//	if (_setEnvelopeNode.Bomb_Number_Text)
//	{
//		ss.str("");
//		ss << GetText("LE_BOMB_VALUE") << _envelope_Value.iBombValue;
//		_setEnvelopeNode.Bomb_Number_Text->setString(ss.str().c_str());
//	}
//	ss.str("");
//}

////庄家滑动设置雷提示动画，只播放一次
//void GameTableUIV99::showBombOperationAnimation(bool isShow)
//{
//	if (_setEnvelopeNode.Bomb_Hand)
//	{
//		if (!isShow)
//		{
//			//停止动画
//			_setEnvelopeNode.Bomb_Hand->stopActionByTag(LE_ANIMATION_HAND_TAG);
//            _setEnvelopeNode.Bomb_Hand->setVisible(false);
//			return;
//		}
//		else
//		{
//			Vec2 sliderPos = _setEnvelopeNode.Bomb_Number_Slider_Operation->getPosition();
//			float width = _setEnvelopeNode.Bomb_Number_Slider_Operation->getContentSize().width;
//			Vec2 orignalPos = Vec2(sliderPos.x - width * (50 - Slider_Operation_Percent[0]) / 100.0f, sliderPos.y);
//			Vec2 endPos = Vec2(sliderPos.x - width * (50 - Slider_Operation_Percent[1]) / 100.0f, sliderPos.y);
//			_setEnvelopeNode.Bomb_Hand->setPosition(orignalPos);
//			_setEnvelopeNode.Bomb_Hand->setVisible(true);
//            Sequence* seq = Sequence::create(MoveTo::create(0.8f, endPos), FadeOut::create(0.1f), CallFunc::create([=]{
//                _setEnvelopeNode.Bomb_Hand->setPosition(orignalPos);
//            }), FadeIn::create(0.1f), MoveTo::create(0.8f, endPos), Hide::create(), nullptr);
//			seq->setTag(LE_ANIMATION_HAND_TAG);
//			_setEnvelopeNode.Bomb_Hand->runAction(seq);
//		}
//	}
//}

//void GameTableUIV99::updateBombValue()
//{
//	if (_setEnvelopeNode.Bomb_Node)
//	{
//		_setEnvelopeNode.Bomb_Node->setVisible(true);
//	}
//	if (_setEnvelopeNode.Slider_Node)
//	{
//		_setEnvelopeNode.Slider_Node->setVisible(true);
//		showBombOperationAnimation(true);
//	}
//}

//void GameTableUIV99::showSendLEButton()
//{
//	if (_setEnvelopeNode.Send_LE_Button)
//	{
//		_setEnvelopeNode.Send_LE_Button->setVisible(true);
//	}
//}
//
//void GameTableUIV99::hideSendLEButton()
//{
//	if (_setEnvelopeNode.Send_LE_Button)
//	{
//		_setEnvelopeNode.Send_LE_Button->setVisible(false);
//	}
//}

//更新庄家界面设置的红包金额的值
void GameTableUIV99::updateMoneyValue(long long value) {
    //if (_setEnvelopeNode.Envelope_Node)
    //{
    //	_setEnvelopeNode.Envelope_Node->setVisible(true);
    //}
    //if (_setEnvelopeNode.Money_Value_Text)
    //{
    //	ostringstream ss;
//       ss << GetText("LE_MONEY_VALUE") << Tools::addComma(value);
    //	_setEnvelopeNode.Money_Value_Text->setString(ss.str().c_str());
    //}
    ////_GameTableLogic->sendBet(_GameTableLogic->getMySeatNo(), value);
    ////{
    ////记录庄家设置的红包信息参数
    //_envelope_Value.bChairID = _GameTableLogic->getMySeatNo();
    //_envelope_Value.i64Money = value;
    ////}
    //_GameTableLogic->sendLEMoney(_GameTableLogic->getMySeatNo(), value);
    ////金额设置完可以设置雷数字
    //updateBombValue();
    ////设置雷数字的时候可以允许显示塞进红包按钮
    //showSendLEButton();
}

//更新庄家界面到红包金额设定好的状态
void GameTableUIV99::autoUpdateSetMoneyUI(long long money) {
    //if (!m_vBetBtn.empty())
    //{
    //	for (int i = 0; i < 4; i++)
    //	{
    //		if (m_vBetBtn.at(i))
    //		{
    //			m_vBetBtn.at(i)->setVisible(false);
    //		}
    //	}
    //}

    //updateMoneyValue(money);
}

//显示庄家设置完红包等待拆红包的界面
void GameTableUIV99::showOpenListForBanker() {
    //if (_setEnvelopeNode.Envelope_Node)
    //{
    //	_setEnvelopeNode.Envelope_Node->setVisible(false);
    //}
    //if (_setEnvelopeNode.Slider_Node)
    //{
    //	_setEnvelopeNode.Slider_Node->setVisible(false);
    //}
    //_GameTableLogic->sendLEBomb(_GameTableLogic->getMySeatNo(), _envelope_Value.iBombValue);
    ////
    //showBankerListUI();
}

//庄家发完红包后显示闲家拆红包列表
void GameTableUIV99::showBankerListUI() {
    //if (_openEnvelopeList.EnvelopeNode)
    //{
    //	ostringstream ss;
    //	_openEnvelopeList.EnvelopeNode->setVisible(true);
    //	UserInfoStruct* user = _GameTableLogic->getUserBySeatNo(_envelope_Value.bChairID);
    //	if (user)
    //	{
    //		if (_openEnvelopeList.Head_Icon)
    //		{
    //			_openEnvelopeList.Head_Icon->getContentSprite()->setTexture(StringUtils::format(PLAYER_HEAD, user->bLogoID));
    //		}
    //		if (_openEnvelopeList.Player_Name)
    //		{
    //			std::string name = ForceGBKToUtf8(user->nickName);
    //			ss << name << " 的红包";
    //			_openEnvelopeList.Player_Name->setString(ss.str());
    //			ss.str("");
    //		}
    //	}
    //	if (_openEnvelopeList.Player_LE_Money)
    //	{
//           ss << Tools::addComma(_envelope_Value.i64Money);
    //		_openEnvelopeList.Player_LE_Money->setString(ss.str().c_str());
    //		ss.str("");
    //	}
    //	if (_openEnvelopeList.LE_Count_State)
    //	{
    //		ss << _envelope_Value.openCount << "/5";
    //		_openEnvelopeList.LE_Count_State->setString(ss.str().c_str());
    //		ss.str("");
    //	}
    //	if (_openEnvelopeList.Bomb_Node_Text)
    //	{
    //		ss << GetText("LE_BOMB_VALUE") << _envelope_Value.iBombValue;
    //		_openEnvelopeList.Bomb_Node_Text->setString(ss.str().c_str());
    //		ss.str("");
    //	}
    //}
}

//玩家坐下(非自己)
void GameTableUIV99::userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    addUser(userSit->bDeskStation, userSit->bDeskStation == _GameTableLogic->getMySeatNo());
}

//玩家站起
void GameTableUIV99::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    if (m_GameStatus == GS_FREE) {
        BYTE bViewChairID = _GameTableLogic->logicToViewSeatNo(userSit->bDeskStation);
        //m_vPlayerHead.at(bViewChairID)->setVisible(false);
        //m_vPrepareImage.at(bViewChairID)->setVisible(false);

        if (m_bGameEnd) {

            //m_vPlayerPokerPointImage.at(bViewChairID)->setVisible(false);
            //if (m_vSettlementValueLabel.at(bViewChairID))
            //{
            //	m_vSettlementValueLabel.at(bViewChairID)->setVisible(false);
            //}
        }
    }
}

void GameTableUIV99::onGameBegin(GameBeginStruct* msg) {
    //重置上轮游戏ui
    resetNewGame();

    memcpy(m_iMultiplying, msg->imultiplying, sizeof(msg->imultiplying));
    m_i64SingleNoteLimit = msg->i64SingleNoteLimit;
    m_iFreeTime = msg->itime;

    //刷新身上金额
    updateGoldLabel(_GameTableLogic->getMyMoney());

    //定时器
    m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_FREE, m_iFreeTime);
}

void GameTableUIV99::onGameBeginBet(GameNote* msg) {
    m_iBetTime = msg->iTime;
    m_iPrevHasNote = msg->iPrevHasNote;

    //定时器
    m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_BET, m_iBetTime);

    m_GameStatus = GS_XIAZHU;
    controlBetButtonEnable();
    updateGameStateLabelContent();
    playBetMusic();
    animationTipBetBegin();

    //转盘上随机显示一个光环，其最后结束的位置就是下局转盘开始时动物的起始位置
    schedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree), 1.5f);

}

void GameTableUIV99::onGameBetResult(GameXiaZhuResponse* msg) {
    //刷新余额
    if (msg->bDeskStation == _GameTableLogic->getMySeatNo()) {
        //刷新身上金额
        updateGoldLabel(_GameTableLogic->getMyMoney());
    }

    //自己下的每个区域
    if (_betArea.myCoinText.size() > 0) {
        for (int i = 0; i < GAME_NOTE_COUNT; i++) {
            string moneyString = Tools::addComma(msg->iOwnNotes[i]);
            _betArea.myCoinText.at(i)->setString(moneyString);

            m_i64OwnNotes[i] = msg->iOwnNotes[i];
        }
    }

    //所有区域下注值
    if (_betArea.allCoinText.size() > 0) {
        for (int i = 0; i < GAME_NOTE_COUNT; i++) {
            string moneyString = Tools::addComma(msg->iNotebyregion[i]);
            _betArea.allCoinText.at(i)->setString(moneyString);

            m_i64AreaNotes[i] = msg->iNotebyregion[i];
        }
    }
    //下注类型
    //msg->iType;

    //自己下的总注
    //msg->iOwnAllCountNote;

    //最近一笔主值
    //msg->iNoteNum;
}

void GameTableUIV99::onGameStartCircle(GamePlay* msg) {
    //停止下注声音提示
//    playStopBetMusic();
    //下注阶段的ui关闭
    hideBetAreaSelectState();
    //controlBetMoneyArea(INVALID_BYTE, false);
    controlBetButtonEnable(false);

    //////////要刷新的东西
    m_iCircleTime = msg->iTime;
    //起点
    //msg->iStarPos;
    int endPos = msg->iEndPos;

    //先播放开奖动画提示，然后转圈效果展示
    Sequence* seq = Sequence::create(CallFunc::create([=] {
        animationTipStartCircle();
    }),
    DelayTime::create(2.1f),
    CallFunc::create([=] {
        //终点位置  圈数  总时长
        showRunLoopAnimation(endPos, 3 + rand()%2, 7.5f);
    }), nullptr);

    this->runAction(seq);

    //开的动物
    //msg->iEndAnimal;

    //是否射灯
    //msg->iIsShedeng;

    //更新彩金池金额
    if (_goldPool.poolMoney) {
        _goldPool.i64Money = msg->i64Winnings;
        string poolMoneyStr = Tools::addComma(msg->i64Winnings);
        _goldPool.poolMoney->setString(poolMoneyStr);
    }

    m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_KAIJIANG, m_iCircleTime);

    m_GameStatus = GS_KAIJIANG;
    updateGameStateLabelContent();
    //playOpenMusic();
}

void GameTableUIV99::onGameShowResult(GamePlayResult* msg) {
    //终点
    m_loopAnimationEndIndex = msg->iEndPos;
    //if (BYTE kindID = getAnimalKindByLoopIndex(msg->iEndPos) != INVALID_BYTE)
    //{
    //	showResultButtonAnimation(kindID, 4.0f);
    //}

    if (msg->iEndAnimal != INVALID_BYTE) {
        //展示出开奖的动物
        showResultAnimalAnimation(msg->iEndAnimal, 4.5f);
    }


    //开的动物
    //msg->iEndAnimal;

    //是否射灯
    //msg->iIsShedeng;

    //彩金
    //msg->i64Winnings;

    m_iShowResultTime = msg->iTime;

// 	m_bDoTimeOutCallback = false;
// 	setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_KAIJIANG_RESULT, m_iShowResultTime);

    m_GameStatus = GS_KAIJIANG;
}

//计算结果
void GameTableUIV99::onGameCalcuResult(GameEndMsg* msg) {
    //玩家输赢的钱
    showResultAnimation(msg->i64RealWinMoney);
    ////刷新身上金额
    //updateGoldLabel(_GameTableLogic->getMyMoney());

    //输赢游戏总的输赢记录
    updateMyWinLabel(msg->i64RealWinMoney);

    //刷新纪录
    updateStateImage(msg->iHistory);

    //赢多少
    msg->i64RealWinMoney;
}

void GameTableUIV99::onGameTipExit(le_s2c_tip_exit* msg) {
    tipMsg.bIsExit = msg->bIsExit;
    tipMsg.i64Money = msg->i64Money;

    //不退出
    if (tipMsg.bIsExit == 0) {
        //下一局离开
        if (m_bNextOut) {
            m_canPrepareForNew = false;
            this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
                _GameTableLogic->sendUserUp();
            })));
        } else {
            m_canPrepareForNew = true;
            //m_PrepareBtn->setVisible(false && m_canPrepareForNew);
            //m_PrepareBtn->setTouchEnabled(false && m_canPrepareForNew);
        }
        return;
    } else {
//        tipMsg->bIsExit;
    }

    //要退出
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
        tipCallback();
    })));
}

void GameTableUIV99::onGameNotify(tag_s2c_notify* msg) {
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
        if (msg->nUserID == _GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo())->dwUserID) {
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


    void GameTableUIV99::onHandleNewMessage(int iType, const std::string &message, int iCount)
    {
        //全服公告
        if (iType == 0)
        {
            showNotice(message, 100, iType);
        }
        else if (iType == -11)//银商
        {
            showNotice(message, iCount, iType);
        }
    }

void GameTableUIV99::initData() {
    //m_vGameStatePosition.clear();
}

void GameTableUIV99::resetNewGame() {
    m_GameStatus = GS_FREE;
    updateGameStateLabelContent();
    playFreeMusic();

    m_bGameEnd = true;
    m_canPrepareForNew = false;
    //m_betButtonTag = INVALID_BYTE;

    m_iBetType = -1;
    m_iBetMoneyType = -1;

    m_iPrevHasNote = 0;	//重置上局下注信息，等待服务端重新通知

    memset(m_i64OwnNotes, 0, sizeof(m_i64OwnNotes));
    memset(m_i64AreaNotes, 0, sizeof(m_i64AreaNotes));
    //////////////
    stopAnimalsAnimation();
    this->unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
    this->unschedule(schedule_selector(GameTableUIV99::updateResultAnimation));
    this->unschedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree));
    m_openResultAnimalIndex.clear();
    m_openResultAnimalKind.clear();

    //controlBetMoneyArea(INVALID_BYTE, false);
    hideBetAreaSelectState();
    resetTextValue();
}

void GameTableUIV99::resetUINewGame() {
    //memset(m_i64BetArr, 0, sizeof(m_i64BetArr));
    //恢复设置红包UI
    //createEnvelope();
    //_envelope_Value.bChairID = INVALID_BYTE;
    //_envelope_Value.i64Money = 0;
    //_envelope_Value.iBombValue = 0;
    //_envelope_Value.openCount = 0;

    //if (m_ClockBg)
    //{
    //	m_ClockBg->setPosition(getTimerPosition());
    //}
    //for (int i = 0; i < PLAYER_COUNT; i++)
    //{
    //	if (m_vSettlementValueLabel.at(i))
    //	{
    //		m_vSettlementValueLabel.at(i)->setString("+0");
    //		m_vSettlementValueLabel.at(i)->setColor(Color3B::RED);
    //		m_vSettlementValueLabel.at(i)->setVisible(false);
    //	}
    //	m_vPlayerPrepare.at(i) = 0;
    //	//if (m_vPlayerHead.at(i))
    //	//{
    //	//	m_vPlayerHead.at(i)->setBankerVisible(false);
    //	//}
    //}
}

void GameTableUIV99::resetData() {
    //ui
    _fScaleX = 0;
    _fScaleY = 0;
    _tableSize = Size(0, 0);

    _tableBg = nullptr;
    _tableBgRight = nullptr;

    //_toolbarBg = nullptr;
//     m_ClockBg = nullptr;
//     m_ClockTimeLabel = nullptr;
	//m_Clock = nullptr;

    m_Notice = nullptr;
	m_clockObj = nullptr;

    //数据
    m_bPrepareTime = 0;
    m_bCatchBankerTime = 0;
    m_bSetMoneyTime = 0;
    m_bSetBombTime = 0;
    m_bOpenLETime = 0;
    m_bShowResultTime = 0;

    m_fClockTime = 0;
    m_fTimeCount = 0;
    m_bTimerChairID = INVALID_BYTE;
    m_iTimerID = 0;
    m_iSendRoundNum = 0;
    m_iSendViewID = 0;

    //上局是否有下注
    m_iPrevHasNote = 0;
    m_iBetType = -1;
    m_iBetMoneyType = -1;

    m_GameStatus = GS_FREE;
    m_bGameEnd = false;
    m_bNextOut = false;
    m_bCurUser = INVALID_BYTE;
    m_bNextUser = INVALID_BYTE;

    m_canPrepareForNew = false;
    //m_bBankerID = INVALID_BYTE;
    //m_vPlayerPrepare.clear();

    //BetArea
    _betArea.clearResetNode = nullptr;
    _betArea.myGold = nullptr;
    _betArea.myWin = nullptr;
    _betArea.myGoldSprite = nullptr;
    _betArea.myWinSprite = nullptr;
    _betArea.clearBtn = nullptr;
    _betArea.resetBtn = nullptr;
    _betArea.betButtons.clear();
    _betArea.allCoinText.clear();
    _betArea.myCoinText.clear();
    _betArea.btnLights.clear();
    _betArea.areaNode = nullptr;

    //ResultLoop
    _resultLoop.loopNode = nullptr;
    _resultLoop.animals.clear();

    _resultLabel.labelNode = nullptr;
    _resultLabel.font = nullptr;

    //MoneyPool
    _goldPool.poolNode = nullptr;
    _goldPool.poolMoney = nullptr;
    _goldPool.i64Money = 0;

    //BetButtons
    _betButton.node = nullptr;
    _betButton.buttons.clear();
    _betButton.btnLights.clear();

    //OpenRecord
    _openRecord.node = nullptr;
    _openRecord.arrowLeft = nullptr;
    _openRecord.arrowRight = nullptr;
    _openRecord.animals.clear();

    //game state label
    m_gameStateNode = nullptr;
	m_gameStateSprite = nullptr;

    //游戏总的输赢
    m_allMyWinGold = 0;
    //m_betButtonTag = INVALID_BYTE;

    //animation
    m_animalAnimationNode = nullptr;

    //
    m_betAreaKindSize.clear();
    m_betAreaPosition.clear();
    m_betAreaKindSize.resize(2);
    m_betAreaPosition.resize(3);

    //
    m_loopAnimalPosition.clear();
    m_loopAnimationSprite.clear();
    m_loopAnimationSprite.resize(3);
    m_loopAnimationRemainTime = 0;
    m_loopAnimationMaxTime = 0;
    m_loopAnimationEndIndex = -1;

    //
    m_openRecordAnimal.clear();
    m_openRecordAnimal.resize(Open_Animal_Num);

    //
    m_openResultAnimalKind.clear();
    m_openResultAnimalIndex.clear();
}

////年兽触发
//void GameTableUIV99::onGameShowNianShou() {
//    //播动画
//    showNianShouAnimation();
//}
//
////赶年兽结果
//void GameTableUIV99::onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) {
//    //过滤只有自己看的到
//    if (nUserID != _GameTableLogic->getUserId(_GameTableLogic->getMySeatNo())) {
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
//void GameTableUIV99::onGameMatchGainFirecraker(int nUserID) {
//    //该id获得一个中型鞭炮
//    //只有自己显示
//    if (_GameTableLogic->getUserId(_GameTableLogic->getMySeatNo()) == nUserID) {
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
//void GameTableUIV99::showNianShouAnimation() {
//    if (this->getChildByName("DriveNianShouBtn")) {
//        return;
//    }
//    auto btn = NewYearActivity::NSAnimationTool::createNSButton("DriveNianShouBtn");
//    if (btn) {
//        btn->setPosition(NewYearActivityInGame::NianShouButtonPosition);
//        btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::driveNianShouClickCallback, this));
//        addChild(btn, NewYearActivityInGame::NianShouZOrder);
//
//        auto daijiNode = NewYearActivity::NSAnimationTool::createNSDaijiNodeAni("daiji", true);
//        if (daijiNode) {
//            daijiNode->setPosition(Vec2(btn->getContentSize().width*0.5f, btn->getContentSize().height*0.5f));
//            btn->addChild(daijiNode);
//        }
//        scheduleOnce(schedule_selector(GameTableUIV99::autoRemoveNianShouButton), 5.0f);
//    }
//}
//
//void GameTableUIV99::driveNianShouClickCallback(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED)
//        return;
//
////        unschedule(schedule_selector(GameTableUIV99::autoRemoveNianShouButton));
//
//    Button* btn = (Button*)pSender;
//    btn->setTouchEnabled(false);
//
//    //发包
//    int nUserID = _GameTableLogic->getUserId(_GameTableLogic->getMySeatNo());
//    _GameTableLogic->sendDriveNianShou(nUserID);
//}
//
//void GameTableUIV99::removeNianShouButton() {
//    auto btn = this->getChildByName("DriveNianShouBtn");
//    if (btn) {
//        btn->stopAllActions();
//        btn->removeFromParentAndCleanup(true);
//    }
//}
//
//void GameTableUIV99::autoRemoveNianShouButton(float dt) {
//    removeNianShouButton();
//}

void GameTableUIV99::onLuckyMoneyNotify(LuckyMoneyNotify *msg)
{
	if (nullptr == msg)
	{
		return;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto notify = CSLoader::createNode("Reward/ui.csb");
	notify->setName("RewardNotice");
	auto btnOpenReward = dynamic_cast<Button*>(notify->getChildByName("btnOpen"));
	btnOpenReward->addTouchEventListener(std::bind([&](Ref* psender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::ENDED != type)
		{
			return;
		}

		RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_OPEN_LUCKYMONEY, 0, 0);
		this->removeChildByName("RewardNotice");
	}, std::placeholders::_1, std::placeholders::_2));

	addChild(notify, 200);
	notify->setPosition(Vec2(visibleSize) / 2);

	runAction(Sequence::create(DelayTime::create(msg->nStayTime), CallFunc::create([&]() {
		this->removeChildByName("RewardNotice");
	}), NULL));
}

void GameTableUIV99::onLuckyMoneyResult(LuckyMoneyResult *msg)
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
		result->setPosition(Vec2(visibleSize) / 2);
		result->setName("rewardResult");
		result->getChildByName("show")->runAction(action);
		action->gotoFrameAndPlay(0, false);

		auto txtReward = dynamic_cast<Text*>(result->getChildByName("txtReward"));
		txtReward->setString("");

		auto luckyMoney = msg->i64LuckyMoney;
		auto retainTime = msg->nShowTime;
		//auto i64MyMoney = msg->i64MyMoney;
		runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
			txtReward->setString(Tools::addComma(luckyMoney));
			//updateGoldLabel(i64MyMoney);
			//_playerHead->setPlayerMoney(msg->i64MyMoney);
		}), DelayTime::create(retainTime), CallFunc::create([&]() {
			this->removeChildByName("rewardResult");
		}), NULL));
	}
}

}
