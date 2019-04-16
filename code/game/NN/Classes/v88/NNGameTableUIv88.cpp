#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
//#include "../../TTNN/Classes/v88/BRNNHelpLayerV88.h"
//#include "../../TTNN/Classes/v88/BRNNSetLayerV88.h"
//#include "../../TTNN/Classes/GameTable/BRNNActions.h"
#include "../../../../code/common/LibHNUI/UI/UIGoldRain.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"

#include "NNGameTableUIv88.h"
#include "NNGameTableLogic.h"

#include "NNPokerCardv88.h"
#include "NNPlayerHeadv88.h"

#include "HNUIExport.h"
#include "HNLobbyExport.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/v66/ExtraReward.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "HNLobby/GameData.h"
namespace NN {
#define WINSIZE Director::getInstance()->getWinSize()
#define NOTICE_ZORDER 100
#define DISTANCE 200.0
#define PI 3.1415926

//font
static const char* NN_FONT_TTF									= FONT_TTF_FZCY;

//static const char* Notice_Bg									= "goldenToad/notic/control.png";
//static const char* Notice_Bg_Stencil							= "goldenToad/notic/Notice_Bg.png";

//tool
//static const char* NN_GAME_BG									= "hallimg/niuniu/0.png";
//static const char* NN_MARK_BG                                    = "hallimg/niuniu/waterMark%d.png";
//static const char* NN_TOOL_BAR_PATH								= "v66/common/tool/%s";
//clock
static const char* NN_CLOCK_BG									= "Games/12100004/13zhang/zupaijiemian/8.png";
static const char* NN_CLOCK_NUM									= "v66/common/fnt_number/yellow1.png";
//static const char* NN_RESULT_NUM								= "v66/common/fnt_number/blue0.png";

static const char* NN_PREPARE_WORD								= "v66/button/yellow1/6.png";
static const char* NN_PREPARE_BTN								= "v66/button/yellow1/BG.png";

static const char* NN_PREPARE_IMAGE								= "v66/zi/1.png";
static const char* NN_OK_IMAGE									= "v66/zi/19.png";

static const char* NN_STATE_IMAGE_CATCH							= "v66/zi/2.png";
static const char* NN_STATE_IMAGE_BET							= "v66/zi/16.png";

static const char* NN_CATCH_BTN_BG								= "v66/button/yellow1/BG.png";
static const char* NN_GIVE_UP_WORD								= "v66/button/yellow1/7.png";
static const char* NN_CATCH_WORD								= "v66/button/yellow1/8.png";

static const char* NN_BET_BG									= "v66/button/chouma/111.png";
static const char* NN_WIN_BG									= "v66/button/chouma/111.png";
static const char* NN_LOSE_BG									= "v66/button/chouma/222.png";

static const char* NN_OPEN_CARD_WORD							= "v66/button/green1/5.png";
static const char* NN_OPEN_CARD_BTN								= "v66/button/green1/BG.png";
static const char* NN_CANCEL_AUTO_WORD							= "v66/button/blue1/5.png";
static const char* NN_AUTO_WORD									= "v66/button/blue1/4.png";
static const char* NN_AUTO_BTN									= "v66/button/blue1/BG.png";

static const char* NN_TONGBINIU_DESKTOP_BG                      = "hallimg/niuniu/2.png";

static const char* NN_RESULT_PATH								= "hallimg/niuniu/number/card_result%d.png";

//tip
//static const char* NN_TIP_PREPARE								= "v66/common/time_tip/prepare_time_tip.png";
//static const char* NN_TIP_BANKER								= "v66/common/time_tip/banker_time_tip.png";
//static const char* NN_TIP_BET									= "v66/common/time_tip/bet_time_tip.png";
//static const char* NN_TIP_SEND									= "v66/common/time_tip/send_time_tip.png";
//static const char* NN_TIP_OPEN									= "v66/common/time_tip/open_time_tip.png";

//奖池
static const char* NN_POOL_BG0									= "hallimg/niuniu/tt_wuhuaniu.png";
static const char* NN_POOL_BG1									= "hallimg/niuniu/tt_db.png";
static const char* NN_POOL_NOTICE								= "hallimg/niuniu/tt_wuhuaniu.png";
//static const char* NN_POOL_IMAGE_PATH							= "NN/cow/mul/rewardPool/poolImage%d.png";
//static const char* NN_MONEY_WORD								= "NN/cow/mul/rewardPool/moneyWord.png";

//sound
//static const char* NN_SOUND_BACK								= "music/NN/back.mp3";
//static const char* NN_SOUND_TIP_CATCH							= "music/NN/tipCatch.mp3";
static const char* NN_SOUND_CATCH								= "hallimg/niuniu/sound/catch.mp3";
static const char* NN_SOUND_NO_CATCH							= "hallimg/niuniu/sound/noCatch.mp3";
//static const char* NN_SOUND_TIP_BET								= "music/NN/tipBet.mp3";
//static const char* NN_SOUND_SEND								= "music/NN/send_card.mp3";
static const char* NN_SOUND_BULL								= "hallimg/niuniu/sound/niu%d.mp3";
static const char* NN_SOUND_READY								= "hallimg/niuniu/sound/ready.mp3";
//static const char* NN_SOUND_CLOCK								= "music/NN/clockTimer.mp3";
static const char* NN_SOUND_CLEAR_COIN							= "music/NN/clear_coin.wav";
static const char* NN_SOUND_BET_CION                            = "hallimg/niuniu/sound/selectBet.wav";
//static const char* NN_SOUND_WIN									= "music/win.mp3";
//static const char* NN_SOUND_LOSE								= "music/lose.mp3";

//static const char* GAME_BACKGROUND_MUSIC						= "music/platformBackGround.mp3";
static const char* GAME_SOUND_BTN								= "music/btnEffect/button_click_sound.mp3";


static const char* NN_GOLD_IMAGE								= "v66/goldenToad/ExchangeScoreUi/Res/golden.png";
static const char* NN_COW_RUN									= "v88/animation/wuhuaniu/wuhuaniu.csb";
static const char* NN_RESULIT_ANI_1_9                           = "v88/animation/cow/shuzi.csb";

static const char* NN_QIANGZHUANG_ANI                            = "v88/animation/qiangzhuang/qiangzhuang.csb";
//static const char* NN_QIANGZHUANG_IMG                            = "hallimg/niuniu/zhuang_1.png";

//static const char* NN_STAR_ANI                                   = "v66/animation/gamestart/gamestart.csb";



HNGameUIBase* GameTableUIV88::create(BYTE bDeskIndex, bool bAutoCreate) {
    GameTableUIV88* tableUI = new GameTableUIV88();
    if (tableUI && tableUI->init(bDeskIndex, bAutoCreate)) {
        tableUI->autorelease();
    } else {
        delete tableUI;
        tableUI = nullptr;
    }
    return tableUI;
}

GameTableUIV88::GameTableUIV88() {
    //TextureCache::getInstance()->addImage(NN_GOLD_IMAGE);
    m_i64Jackpot = 0;
    m_backGroundMusicPath = "";
}

GameTableUIV88::~GameTableUIV88() {
    //TextureCache::getInstance()->removeTextureForKey(NN_GOLD_IMAGE);
   // this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
//    this->unschedule(schedule_selector(GameTableUIV88::updateClock));
    this->unschedule(schedule_selector(GameTableUIV88::updateTip));
    this->unschedule(schedule_selector(GameTableUIV88::sendCardTimer));
    this->unschedule(schedule_selector(GameTableUIV88::showOpenBtn));
    CC_SAFE_DELETE(_GameTableLogic);

	resetData();
}

bool GameTableUIV88::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }

    //初始化人数和变量
    setGamePlayerCount();
    resetData();

	_bDeskIndex = bDeskIndex;
	_bAutoCreate = bAutoCreate;

    return true;
}

void GameTableUIV88::createToolBarUI() {
    int zOrder = 60;
    auto toolbar = ToolBarLayer::create();
    if (toolbar) {
        toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
            if (!RoomLogic()->connected()) {
                leaveTheGame();
            }
            else {
                if (m_vPlayerState.at(_GameTableLogic->getMySeatNo()) != PS_WATCH) {
                    // 游戏不在空闲时期，不允许离开
                    //auto layer = GamePromptLayer::create();
                    //layer->showPrompt(GetText("limit_leave_in_game"));
					GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
					layer->setGamePromptTip(GetText("limit_leave_in_game"));
                    return;
                }
                _GameTableLogic->sendUserUp();
            }
        });
        toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
            IGameSet* gameSet = GameSetFactory::createWithVersion();
            gameSet->setBgPath(m_backGroundMusicPath.c_str());
            gameSet->open(this, Vec2(0, 0), zOrder+5, 0);
        });
        toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
            auto helpLayer = HN::HelpLayer::create();
            if (m_iPlayerCount == PLAYER_COUNT_SIX) {
                helpLayer->setContentOnView("v66/common/help/niuniu1.png");
            }
            else {
                helpLayer->setContentOnView("v66/common/help/niuniu.png");
            }
            helpLayer->setName("helpLayer");
            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            helpLayer->setOrignalPosition();
            helpLayer->moveSelfInWithAction();
            addChild(helpLayer, zOrder+5);
        });
        toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
    }
}


//void GameTableUIV88::onBtnClick(Ref* pSender, Control::EventType event) {
//	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//	Node* btn = (Node*)pSender;
//	switch (btn->getTag()) {
//	//case em_toolbarBtn: {
//	//						if (0 == _toolbarBg->getName().compare("0")) {
//	//							_toolbarBg->setName("1");
//	//							_toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height)));
//	//							this->getChildByTag(em_toolbarBtn)->setVisible(false);         //whao
//	//						}
//	//}
//		break;
//	case em_toolbarSetBtn: {
//							   if (this->getChildByName("helpLayer")) {
//                                   ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
////								   this->getChildByName("helpLayer")->removeFromParent();
//							   }
//							   _toolbarBg->setName("0");
//							   _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//								   CallFunc::create([this]() {
//								   this->getChildByTag(em_toolbarBtn)->setVisible(true);
//							   })));
//							   if (this->getChildByName("setLayer")) {
//								   //(dynamic_cast<BRNNSetLayerV88*>(this->getChildByName("setLayer")))->closeSet();
//								   BRNN::Actions::actionOut(this->getChildByName("setLayer"));
//								   return;
//							   }
//							   auto setLayer = BRNNSetLayerV88::create();
//							   setLayer->setName("setLayer");
//							   setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//							   setLayer->showSet(this, 60);
//	}
//		break;
//	case  em_toolbarHelpBtn: {
//
//								 if (this->getChildByName("setLayer")) {
//
//									 this->getChildByName("setLayer")->removeFromParent();
//								 }
//								 _toolbarBg->setName("0");
//								 _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//									 CallFunc::create([this]() {
//									 this->getChildByTag(em_toolbarBtn)->setVisible(true);
//								 })));
//								 if (this->getChildByName("helpLayer")) {
//                                     ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
//									 //(dynamic_cast< BRNN::BRNNHelpLayer*>(this->getChildByName("helpLayer")))->actionOut();
////									 BRNN::Actions::actionOut(this->getChildByName("helpLayer"));
//									 return;
//								 }
//        
//        auto helpLayer = HN::HelpLayer::create();
//        if (m_iPlayerCount == PLAYER_COUNT_SIX) {
//            helpLayer->setContentOnView("v66/common/help/niuniu1.png");
//        }
//        else {
//                helpLayer->setContentOnView("v66/common/help/niuniu.png");
//        }
//        helpLayer->setName("helpLayer");
//        helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//        helpLayer->setOrignalPosition();
//        helpLayer->moveSelfInWithAction();
//        addChild(helpLayer, 60);
//        
//	}
//		break;
//	case em_toolbarLeaveBtn: {
//								 if (!RoomLogic()->connected()) {
//									 leaveTheGame();
//								 }
//								 else {
//									 if (m_vPlayerState.at(_GameTableLogic->getMySeatNo()) != PS_WATCH) {
//										 // 游戏不在空闲时期，不允许离开
//										 auto layer = GamePromptLayer::create();
//										 layer->showPrompt(GBKToUtf8("游戏过程中不允许离开！"));
//										 return;
//									 }
//									 _GameTableLogic->sendUserUp();
//								 }
//	}
//		break;
//	case em_toolbarNextOutBtn: {
//								   if (btn->getUserData() == (void*)true) {
//									   btn->setUserData((void*)false);
//									   btn->getChildByName("checkSprite")->setVisible(false);
//									   m_bNextOut = false;
//								   }
//								   else if (btn->getUserData() == (void*)false) {
//									   btn->setUserData((void*)true);
//									   btn->getChildByName("checkSprite")->setVisible(true);
//									   m_bNextOut = true;
//								   }
//	}
//		break;
//	}
//}



//void GameTableUIV88::toolBarClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype)
//{
//	if (touchtype != Widget::TouchEventType::ENDED)
//		return;
//	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//	Node* btn = (Node*)pSender;
//	switch (btn->getTag()) {
//	case em_toolbarBtn: {
//							if (toolBar_flag == true)
//							{
//								auto btn = static_cast<Button*>(this->getChildByTag(em_toolbarBtn));
//								if (btn)
//									btn->setTouchEnabled(false);
//								_vecMenuButtonKids.at(0)->runAction(Sequence::create(DelayTime::create(0.1 * 0),
//									MoveTo::create(0.1, _vecToolbarPoints.at(0))
//									, MoveTo::create(0.1, _vecToolbarPoints2.at(0))
//									, MoveTo::create(0.1, _vecToolbarPoints.at(0)), nullptr));
//								_vecMenuButtonKids.at(1)->runAction(Sequence::create(DelayTime::create(0.1 * 1),
//									MoveTo::create(0.1, _vecToolbarPoints.at(1))
//									, MoveTo::create(0.1, _vecToolbarPoints2.at(1))
//									, MoveTo::create(0.1, _vecToolbarPoints.at(1)), nullptr));
//								_vecMenuButtonKids.at(2)->runAction(Sequence::create(DelayTime::create(0.1 * 2),
//									MoveTo::create(0.1, _vecToolbarPoints.at(2))
//									, MoveTo::create(0.1, _vecToolbarPoints2.at(2))
//									, MoveTo::create(0.1, _vecToolbarPoints.at(2)), nullptr));
//								_vecMenuButtonKids.at(3)->runAction(Sequence::create(DelayTime::create(0.1 * 3)
//									, MoveTo::create(0.1, _vecToolbarPoints.at(3))
//									, MoveTo::create(0.1, _vecToolbarPoints2.at(3))
//									, MoveTo::create(0.1, _vecToolbarPoints.at(3)), CallFunc::create([this](){
//									auto btn = static_cast<Button*>(this->getChildByTag(em_toolbarBtn));
//									if (btn)
//									{
//										btn->setTouchEnabled(true);
//										btn->setColor(Color3B::GRAY);
//									}
//									toolBar_flag = !toolBar_flag;
//								}), nullptr));
//							}
//							else
//							{
//								_vecMenuButtonKids.at(0)->runAction(Sequence::create(MoveTo::create(0.05, Vec2(0, 0)), nullptr));
//								_vecMenuButtonKids.at(1)->runAction(Sequence::create(MoveTo::create(0.05, Vec2(0, 0)), nullptr));
//								_vecMenuButtonKids.at(2)->runAction(Sequence::create(MoveTo::create(0.05, Vec2(0, 0)), nullptr));
//								_vecMenuButtonKids.at(3)->runAction(Sequence::create(MoveTo::create(0.05, Vec2(0, 0)), CallFunc::create([&](){
//									auto btn = this->getChildByTag(em_toolbarBtn);
//									if (btn)
//									{
//										btn->setColor(Color3B::WHITE);
//									}
//									toolBar_flag = !toolBar_flag;
//								}), nullptr));
//							}
//							
//							break;
//	}
//	
//		case  em_toolbarHelpBtn: {
//		    if (this->getChildByName("helpLayer")) {
//                ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
//		        //(dynamic_cast< BRNN::BRNNHelpLayer*>(this->getChildByName("helpLayer")))->actionOut();
////		        BRNN::Actions::actionOut(this->getChildByName("helpLayer"));
//		        return;
//		    }
//            
//            auto helpLayer = HN::HelpLayer::create();
//            if (m_iPlayerCount == PLAYER_COUNT_SIX) {
//                helpLayer->setContentOnView("v66/common/help/niuniu1.png");
//            }
//            else {
//                helpLayer->setContentOnView("v66/common/help/niuniu.png");
//            }
//            helpLayer->setName("helpLayer");
//            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//            helpLayer->setOrignalPosition();
//            helpLayer->moveSelfInWithAction();
//            addChild(helpLayer, 60);
//		}
//		break;
//		case   em_toolbarSetBtn:
//		{
//								   if (this->getChildByName("setLayer")) {
//									   (dynamic_cast<BRNNSetLayerV88*>(this->getChildByName("setLayer")))->closeSet();
//									   return;
//								   }
//								   auto setLayer = BRNNSetLayerV88::create();
//								   setLayer->setName("setLayer");
//								   setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//								   setLayer->showSet(this, 60);
//
//		}
//	case em_toolbarLeaveBtn: {
//								 if (!RoomLogic()->connected()) {
//									 leaveTheGame();
//								 }
//								 else {
//									 if (m_GameStatus != GS_FREE && m_vPlayerState.at(_GameTableLogic->getMySeatNo()) != PS_WATCH) {
//										 // 游戏不在空闲时期，不允许离开
//										 auto layer = GamePromptLayer::create();
//										 layer->showPrompt(GBKToUtf8("游戏过程中不允许离开！"));
//										 return;
//									 }
//									 _GameTableLogic->sendUserUp();
//								 }
//	}
//		break;
//	case em_toolbarNextOutBtn: {
//								   if (btn->getUserData() == (void*)true) {
//									   btn->setUserData((void*)false);
//									   btn->getChildByTag(em_toolbarNextOutLight)->setVisible(true);
//									   m_bNextOut = true;
//								   }
//								   else if (btn->getUserData() == (void*)false) {
//									   btn->setUserData((void*)true);
//									   btn->getChildByTag(em_toolbarNextOutLight)->setVisible(false);
//									   m_bNextOut = false;
//								   }
//	}
//		break;
//	
//	default:
//		break;
//	}
//
//}

void GameTableUIV88::toolBarFlag(float dt)
{
	toolBar_flag = !toolBar_flag;
}


void GameTableUIV88::createPlayerHeadUI(int playerCount) {
    if (playerCount <= 0) return;

    for (int i = 0; i < playerCount; i++) {
        bool bMe = (i == 0) ? true : false;
        bool bTop = (i == m_iPlayerCount / 2) ? true : false;
		auto playerHeadLayer = CPlayerHeadV88::create();
		playerHeadLayer->setIgnoreAnchorPointForPosition(false);
        playerHeadLayer->initUI(bMe, bTop);
		playerHeadLayer->setAnchorPoint(Vec2(0.5, 0.5));
		addChild(playerHeadLayer);
        playerHeadLayer->setPosition(Vec2(0, 0));
        playerHeadLayer->setVisible(false);
		//playerHeadLayer->setScale(0.85);
		
		m_vPlayerHead.push_back(playerHeadLayer);
    }
}

vector<Vec2> GameTableUIV88::getPlayerHeadPos() {
	Size temp = m_vPlayerHead.at(0)->getContentSize();
	Size mySize = temp;
	Size topSize = temp;
	Size edgeSize = temp;
    //二人头像坐标
    std::vector<Vec2> vHeadPos;

    if (m_iPlayerCount == 2) {
		vHeadPos.push_back(Vec2(WIN_SIZE.width / 4 - topSize.width / 2+20, 10 + topSize.width / 2));
		vHeadPos.push_back(Vec2(WIN_SIZE.width / 2 - topSize.width / 2, WIN_SIZE.height - topSize.width / 2-130));
    } else if (m_iPlayerCount == 4) {
		vHeadPos.push_back(Vec2(WIN_SIZE.width / 4 - topSize.width / 2, 10 + topSize.width / 2));
        vHeadPos.push_back(Vec2(WIN_SIZE.width - edgeSize.width/2-150, WIN_SIZE.height / 2 ));
		vHeadPos.push_back(Vec2(WIN_SIZE.width / 2 - topSize.width / 2, WIN_SIZE.height - topSize.width / 2 - 130));
		vHeadPos.push_back(Vec2(edgeSize.height / 2+10, WIN_SIZE.height / 2 ));
    } else if (m_iPlayerCount == 6) {
		vHeadPos.push_back(Vec2(WIN_SIZE.width / 4 - topSize.width / 2+20, 10 + topSize.width / 2));
        vHeadPos.push_back(Vec2(WIN_SIZE.width - edgeSize.width/2-150, WIN_SIZE.height / 2 - edgeSize.height/2));
        vHeadPos.push_back(Vec2(WIN_SIZE.width -  edgeSize.width/2-150, WIN_SIZE.height / 2 + edgeSize.height / 2));
		vHeadPos.push_back(Vec2(WIN_SIZE.width / 2 - topSize.width / 2, WIN_SIZE.height - topSize.width / 2 - 130));
		vHeadPos.push_back(Vec2(edgeSize.width / 2 + 10, WIN_SIZE.height / 2 + edgeSize.height / 2));
		vHeadPos.push_back(Vec2(edgeSize.width / 2 + 10, WIN_SIZE.height / 2 - edgeSize.height / 2));
    }

    return vHeadPos;
}

void GameTableUIV88::resetPlayerHeadUI() {
    for (int i = 0; i < m_iPlayerCount; i++) {
        m_vPlayerHead.at(i)->setPosition(getPlayerHeadPos().at(i));
    }
}

void GameTableUIV88::createClock() {
	_clockObj = TimerObject::create();
	_clockObj->setPosition(WIN_SIZE / 2);
    _clockObj->setTimeCallBack([&]() {
        timeOutCallback(m_bTimerChairID, m_iTimerID);
    });
	this->addChild(_clockObj);
}

void GameTableUIV88::createReadyUI() {
    //准备按钮
    m_PrepareBtn = Button::create("hallimg/niuniu/start.png");
    addChild(m_PrepareBtn,59);
    m_PrepareBtn->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 110));
    m_PrepareBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::prepareClickCallback, this));

   // Size readyBtnSize = m_PrepareBtn->getContentSize();
   // auto readyBtnImage = ImageView::create(NN_PREPARE_WORD);
   // readyBtnImage->setPosition(Vec2(readyBtnSize.width / 2, readyBtnSize.height / 2 + 4));
   // m_PrepareBtn->addChild(readyBtnImage);

    vector<Vec2> vPos;
    if (m_iPlayerCount == 2) {
        vPos.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 60));
        vPos.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 60));
    } else if (m_iPlayerCount == 4) {
        vPos.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 60));
        vPos.push_back(Vec2(WIN_SIZE.width / 2 + 250, WIN_SIZE.height / 2));
        vPos.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 60));
        vPos.push_back(Vec2(WIN_SIZE.width / 2 - 250, WIN_SIZE.height / 2));
    } else if (m_iPlayerCount == 6) {
        vPos.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 60));
        vPos.push_back(Vec2(WIN_SIZE.width / 2 + 240, WIN_SIZE.height / 2 - 60));
        vPos.push_back(Vec2(WIN_SIZE.width / 2 + 240, WIN_SIZE.height / 2 + 60));
        vPos.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 60));
        vPos.push_back(Vec2(WIN_SIZE.width / 2 - 240, WIN_SIZE.height / 2 + 60));
        vPos.push_back(Vec2(WIN_SIZE.width / 2 - 240, WIN_SIZE.height / 2 - 60));

	
    }

    for (int i = 0; i < m_iPlayerCount; i++) {
        auto readyImage = ImageView::create(NN_PREPARE_IMAGE);
        addChild(readyImage);
        readyImage->setPosition(vPos.at(i));
        readyImage->setVisible(false);

        m_vPrepareImage.push_back(readyImage);
    }

    m_vOkImage.resize(m_iPlayerCount);
    for (int i = 0; i < m_iPlayerCount; i++) {
        auto okImage = ImageView::create(NN_OK_IMAGE);
        addChild(okImage, 55);
        okImage->setPosition(getCardPosVector(m_iPlayerCount).at(i).at(2));
        okImage->setVisible(false);

        m_vOkImage.at(i) = okImage;
    }

    //叫庄按钮
    m_CatchBankerBtn = Button::create(NN_CATCH_BTN_BG);
    addChild(m_CatchBankerBtn);
    m_CatchBankerBtn->setPosition(Vec2(WIN_SIZE.width / 2 - 80-50, WIN_SIZE.height / 2 - 130));
    m_CatchBankerBtn->setTag(1);//1:叫
    m_CatchBankerBtn->setVisible(false);
    m_CatchBankerBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::catchBankerClickCallback, this));

    Size catchBtnSize = m_CatchBankerBtn->getContentSize();
    auto catchImage = ImageView::create(NN_CATCH_WORD);
    catchImage->setPosition(Vec2(catchBtnSize.width / 2, catchBtnSize.height / 2));
    m_CatchBankerBtn->addChild(catchImage);
	m_CatchBankerBtn->setScale(0.8);

    //不叫按钮
    m_NotCatchBtn = Button::create(NN_CATCH_BTN_BG);
    addChild(m_NotCatchBtn);
    m_NotCatchBtn->setPosition(Vec2(WIN_SIZE.width / 2 + 80+50, WIN_SIZE.height / 2 - 100-30));
    m_NotCatchBtn->setTag(0);//0:不叫
    m_NotCatchBtn->setVisible(false);
    m_NotCatchBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::catchBankerClickCallback, this));

    Size giveupBtnSize = m_NotCatchBtn->getContentSize();
    auto giveupImage = ImageView::create(NN_GIVE_UP_WORD);
    giveupImage->setPosition(Vec2(giveupBtnSize.width / 2, giveupBtnSize.height / 2));
    m_NotCatchBtn->addChild(giveupImage);
	m_NotCatchBtn->setScale(0.8);

    //摊牌按钮
    m_OpenCardBtn = Button::create(NN_OPEN_CARD_BTN);
    addChild(m_OpenCardBtn);
    m_OpenCardBtn->setPosition(Vec2(WIN_SIZE.width / 2 + 245, 85));
    //m_OpenCardBtn->setVisible(false);
    m_OpenCardBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::openCardClickCallback, this));

	Size openBtnSize = m_OpenCardBtn->getContentSize();
    auto openImage = ImageView::create(NN_OPEN_CARD_WORD);
    m_OpenCardBtn->addChild(openImage);
    openImage->setName("openImage");
    openImage->setPosition(Vec2(openBtnSize.width / 2, openBtnSize.height / 2 + 4));

    setBtnTouch(m_OpenCardBtn, false);

    //托管
    auto autoBtn = Button::create(NN_AUTO_BTN);
    addChild(autoBtn);
    autoBtn->setPosition(Vec2(WIN_SIZE.width / 2 + 450, 85));
    autoBtn->setVisible(m_iPlayerCount == 6);
    autoBtn->setTag(1);//1:托管0:取消
    autoBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::autoClickback, this));

	Size autoBtnSize = autoBtn->getContentSize();
    auto autoImage = ImageView::create(NN_AUTO_WORD);
    autoBtn->addChild(autoImage);
    autoImage->setName("autoImage");
    autoImage->setPosition(Vec2(autoBtnSize.width / 2, autoBtnSize.height / 2 + 4));


    //阶段标志
    m_StageImage = ImageView::create(NN_STATE_IMAGE_CATCH);
    addChild(m_StageImage);
    m_StageImage->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 100));
    m_StageImage->setVisible(false);

    //广播通知
// 		_NoticeBG = Sprite::create(Notice_Bg);
// 		_NoticeBG->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.75f));
// 		_NoticeBG->setCascadeOpacityEnabled(true);
// 		this->addChild(_NoticeBG, NOTICE_ZORDER);
// 		_NoticeBG->setVisible(false);
//
// 		_notic = GameNotice::create();
// 		_notic->setPosition(Vec2(_NoticeBG->getContentSize() / 2));
// 		_notic->setOkayCallBack([this](){
// 			_NoticeBG->setVisible(false);
// 		});
// 		_NoticeBG->addChild(_notic);
}

Button* GameTableUIV88::createNum(int iType) { //0:bet 1:win 2:lose
    auto btn = Button::create(NN_BET_BG);
    btn->setPressedActionEnabled(true);
    if (iType == 0) {
        btn->loadTextures(NN_BET_BG, NN_BET_BG, NN_BET_BG);
    } else if (iType == 1) {
        btn->loadTextures(NN_WIN_BG, NN_WIN_BG, NN_WIN_BG);
    } else if (iType == 2) {
        btn->loadTextures(NN_LOSE_BG, NN_LOSE_BG, NN_LOSE_BG);
    }


    Size btnSize = btn->getContentSize();

    auto label = Label::createWithTTF("", NN_FONT_TTF, 20, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    btn->addChild(label);
    label->setName("numLabel");
    label->setAnchorPoint(Vec2(0.5, 0.5));
	label->setColor(Color3B(129, 59, 17));
    label->setPosition(Vec2(btnSize.width / 2, btnSize.height / 2));

    return btn;
}

void GameTableUIV88::createBetBtn() {
    //清除
    char str[20] = { 0 };
    for (int i = 0; i < 4; i++) {
        sprintf(str, "betBtn%d", i);
        auto btn = this->getChildByName(str);
        if (btn) {
            this->removeChildByName(str);
        }
    }
    m_vBetBtn.clear();

    //文本长度
    sprintf(str, "%lld", m_i64BetArr[3]);
    auto label = Label::createWithTTF(str, NN_FONT_TTF, 20, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    float labelWidth = label->getContentSize().width;


    float btnWidth = createNum(0)->getContentSize().width;
// 		if (btnWidth <= labelWidth -15)
// 		{
// 			btnWidth = labelWidth + 15;
// 		}
    float space = 15;
    float startPosX = WIN_SIZE.width / 2 - space / 2 - space - 2 * btnWidth;
    for (int i = 0; i < 4; i++) {
        auto btn = createNum(0);
        addChild(btn);
        btn->setAnchorPoint(Vec2(0, 0.5));
        btn->setPosition(Vec2(startPosX + (btnWidth + space)*i, WIN_SIZE.height / 2 - 150));
        btn->setTag(i);
        sprintf(str, "betBtn%d", i);
        btn->setName(str);
        btn->setScaleX(btnWidth / btn->getContentSize().width);
        btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::betMoneyClickCallback, this));
        btn->setTouchEnabled(true);

        m_vBetBtn.push_back(btn);

        auto label = dynamic_cast<Label*>(btn->getChildByName("numLabel"));
        auto moneyStr = Tools::addComma(m_i64BetArr[i]);
        label->setString(moneyStr);
    }
}

void GameTableUIV88::createBetValue(BYTE bViewChairID) {
    char str[20] = { 0 };
    sprintf(str, "betValue%c", bViewChairID);
    auto oldBtn = getChildByName(str);
    if (oldBtn) {
        oldBtn->removeFromParent();
    }

    vector<Vec2> vPosTwo;
    if (m_iPlayerCount == 2) {
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 8*3));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 8*5));
    } else if (m_iPlayerCount == 4) {
		vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 8 * 3));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 200, WIN_SIZE.height / 2));
		vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 8 * 5));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 200, WIN_SIZE.height / 2));
    } else {
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 80));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 100, WIN_SIZE.height / 2 - 100));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 100, WIN_SIZE.height / 2 + 100));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 80));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 100, WIN_SIZE.height / 2 + 100));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 100, WIN_SIZE.height / 2 - 100));

		vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 8 * 3));
		vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 200, WIN_SIZE.height / 8 * 3));
		vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 200, WIN_SIZE.height / 8 * 5));
		vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 8 * 5));
		vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 200, WIN_SIZE.height / 8 * 5));
		vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 200, WIN_SIZE.height / 8 * 3));
    }

    auto btn = createNum(0);
    addChild(btn);
    btn->setAnchorPoint(Vec2(0.5, 0.5));
    btn->setPosition(vPosTwo.at(bViewChairID));
    btn->setTouchEnabled(false);
    btn->setName(str);

    BYTE bChairID = _GameTableLogic->viewToLogicSeatNo(bViewChairID);
    auto label = dynamic_cast<Label*>(btn->getChildByName("numLabel"));
    auto moneyStr = Tools::addComma(m_vPlayerBetValue.at(bChairID));
    label->setString(moneyStr);

// 		float labelWidth = label->getContentSize().width;
// 		if (labelWidth + 5 > btn->getContentSize().width)
// 		{
// 			btn->setScaleX((labelWidth + 10) / btn->getContentSize().width);
// 		}

    m_vBetValueBtn.at(bViewChairID) = btn;
}

void GameTableUIV88::createPoker(int iPlayerCount) {
    Vec2 pos = WIN_SIZE/2;
    auto faPaiQi = dynamic_cast<Sprite*>(getChildByName("FaPaiQi"));
    if(faPaiQi)
    {
        pos = faPaiQi->getPosition();
    }
    
	vector<PokerCardV88*> vPoker;
    for (int i = 0; i < iPlayerCount; i++) {
        vPoker.clear();
        for (int j = 0; j < 5; j++) {
            auto poker = PokerCardV88::create(pos);
            addChild(poker, 50 + j);
            poker->setVisible(false);

            vPoker.push_back(poker);
        }

        m_vPlayerPoker.push_back(vPoker);
    }
}

vector<Vec2> GameTableUIV88::getPokerPointPos(int iPlayerCount) {
    vector<Vec2> vPos;
	auto poker = m_vPlayerPoker.at(0);
    if (iPlayerCount == 2) {
        vPos.push_back(Vec2(WIN_SIZE.width / 2, 108));
        vPos.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height - 130));
    } else if (iPlayerCount == 4) {
        vPos.push_back(Vec2(WIN_SIZE.width / 2, 108));
        vPos.push_back(Vec2(WIN_SIZE.width - 150 - 100, WIN_SIZE.height / 2 - 28));
		vPos.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height - 130));
        vPos.push_back(Vec2(150 + 100, WIN_SIZE.height / 2 - 28));
    } else if (iPlayerCount == 6) {
        vPos.push_back(Vec2(WIN_SIZE.width / 2, 108));
        vPos.push_back(Vec2(WIN_SIZE.width - 150 - 100, WIN_SIZE.height / 2 - 76 - 28 - 20));
        vPos.push_back(Vec2(WIN_SIZE.width - 150 - 100, WIN_SIZE.height / 2 + 114 - 28 - 20));
		vPos.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height - 130));
        vPos.push_back(Vec2(150 + 100, WIN_SIZE.height / 2 + 114 - 28 - 20));
        vPos.push_back(Vec2(150 + 100, WIN_SIZE.height / 2 - 76 - 28 - 20));
    }

    return vPos;
}

void GameTableUIV88::createPokerPointImage(int iPlayerCount) {
    //vector<Vec2> vPos = getPokerPointPos(iPlayerCount);
	auto vPos = getCardPosVector(m_iPlayerCount);

    char str[64] = { 0 };
    sprintf(str, NN_RESULT_PATH, 0);

    for (int i = 0; i < iPlayerCount; i++) {
       auto image = ImageView::create(str);
		addChild(image, 55);
		image->setPosition(Vec2(vPos.at(i).at(2).x, vPos.at(i).at(2).y - 50));
		image->setVisible(false);
        if (i != 0) {
            //image->setScale(0.8);
        } else {
            //image->setScale(1.5);
        }
		m_vPlayerPokerPointImage.push_back(image);
    }

	
}

void GameTableUIV88::createResultValue(BYTE bViewChairID, long long i64Money) {
    char str[20] = { 0 };
    sprintf(str, "resultValue%c", bViewChairID);
    auto oldBtn = getChildByName(str);
    if (oldBtn) {
        oldBtn->removeFromParent();
    }

    vector<Vec2> vPosTwo;
    if (m_iPlayerCount == 2) {
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 60));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 60));
    } else if (m_iPlayerCount == 4) {
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 60));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 200, WIN_SIZE.height / 2));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 60));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 200, WIN_SIZE.height / 2));
    } else {
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 80));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 100, WIN_SIZE.height / 2 - 100));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 100, WIN_SIZE.height / 2 + 100));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 80));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 100, WIN_SIZE.height / 2 + 100));
// 			vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 100, WIN_SIZE.height / 2 - 100));

        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 60));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 200, WIN_SIZE.height / 2 - 60));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 + 200, WIN_SIZE.height / 2 + 60));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 60));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 200, WIN_SIZE.height / 2 + 60));
        vPosTwo.push_back(Vec2(WIN_SIZE.width / 2 - 200, WIN_SIZE.height / 2 - 60));
    }

    auto btn = createNum(0);
    addChild(btn);
    btn->setAnchorPoint(Vec2(0.5, 0.5));
    btn->setPosition(vPosTwo.at(bViewChairID));
    btn->setTouchEnabled(false);
    btn->setName(str);

    auto label = dynamic_cast<Label*>(btn->getChildByName("numLabel"));
    if (i64Money > 0) {
        btn->loadTextures(NN_WIN_BG, NN_WIN_BG, NN_WIN_BG);
    } else {
        btn->loadTextures(NN_LOSE_BG, NN_LOSE_BG, NN_LOSE_BG);
    }
    auto moneyStr = Tools::addComma(i64Money);
    label->setString(moneyStr);

    float labelWidth = label->getContentSize().width;
// 		if (labelWidth + 15 > btn->getContentSize().width)
// 		{
// 			btn->setScaleX((labelWidth + 15) / btn->getContentSize().width);
// 		}

    m_vPlayerResultValueBtn.at(bViewChairID) = btn;
}

void GameTableUIV88::createRewardPool()
{
	auto poolBtn = Button::create(NN_POOL_BG0);
	addChild(poolBtn, 59);//比侧边栏60低
	poolBtn->setAnchorPoint(Vec2(0.5, 0.5));
	//poolBtn->setPosition(Vec2(WIN_SIZE.width - 110 - 30, WIN_SIZE.height - 50));
    poolBtn->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height - 40));
	poolBtn->setPressedActionEnabled(true);
	poolBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::poolBtnClickCallback, this));
    
    auto nodeCsb = CSLoader::createNode("csb/v66/Node_niuniuJackpotLight.csb");
    if (nodeCsb) {
        nodeCsb->setPosition(Vec2(poolBtn->getContentSize().width/2,poolBtn->getContentSize().height/2+8));
        poolBtn->addChild(nodeCsb);
        auto nodeAction = CSLoader::createTimeline("csb/v66/Node_niuniuJackpotLight.csb");
        nodeCsb->runAction(nodeAction);
        nodeAction->gotoFrameAndPlay(0);
    }
    
	auto poolMoneyLabel = Label::createWithTTF("", NN_FONT_TTF, 22, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	poolMoneyLabel->setName("poolMoneyLabel");
	addChild(poolMoneyLabel, 59);
	poolMoneyLabel->setAnchorPoint(Vec2(0.5, 0.5));
	poolMoneyLabel->setPosition(Vec2(poolBtn->getPosition().x , poolBtn->getPosition().y - 30));
	poolMoneyLabel->setString("0");

    m_i64Jackpot = GameData::getInstance()->getRoomJackpotData();
    schedule(schedule_selector(GameTableUIV88::updateJackpot), 0.33f);
  
	auto poolInfoImage = ImageView::create(NN_POOL_BG1);
	poolInfoImage->setName("poolInfoImage");
	addChild(poolInfoImage, 59);
	poolInfoImage->setAnchorPoint(Vec2(0.5, 0.5));
	poolInfoImage->setPosition(Vec2(poolBtn->getPosition().x - 10, poolBtn->getPosition().y - 207));
	poolInfoImage->setVisible(false);

	Size sizebg = poolInfoImage->getContentSize();
	auto personalContributionLabel = Label::createWithTTF("", NN_FONT_TTF, 18, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	personalContributionLabel->setName("poolMyMoneyLabel");
	poolInfoImage->addChild(personalContributionLabel);
	personalContributionLabel->setAnchorPoint(Vec2(0, 0));
	personalContributionLabel->setPosition(Vec2(sizebg.width*0.65, 0.8*sizebg.height));
	personalContributionLabel->setString("0");

	float posX = 15;
	float posY = 205;
	float instanceX0 = 80;
	float instanceX1 = 100;
	float instanceX2 = 80;

	float instanceY0 = 35;
	

	//char poolStr[64] = { 0 };
	//for (int j = 0; j < 3; j++)
	//{
	//	sprintf(poolStr, NN_POOL_IMAGE_PATH, j);
	//	ImageView* image = ImageView::create(poolStr);
	//	poolInfoImage->addChild(image);
	//	image->setAnchorPoint(Vec2(0, 1));
	//	float instanceX = 0;
	//	if (j == 1)
	//	{
	//		instanceX = instanceX1;
	//	}
	//	else if (j == 2)
	//	{
	//		instanceX = instanceX2;
	//	}
	//	image->setPosition(Vec2(posX + j*instanceX, posY));
	//}

	m_vPoolLabel.resize(5);
	for (int i = 0; i < 5; i++)
	{
		m_vPoolLabel.at(i).resize(3);
		for (int j = 0; j < 3; j++)
		{
			Label* label = Label::createWithTTF("", NN_FONT_TTF, 15, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
			poolInfoImage->addChild(label);
			label->setAnchorPoint(Vec2(0, 1));
			float instanceX = 0;
			if (j == 1)
			{
				instanceX = instanceX1;
			}
			else if (j == 2)
			{
				instanceX = instanceX2;
			}
			label->setPosition(Vec2(posX + j*instanceX, posY - 35 - i* instanceY0));
			label->setString("0");
			label->setVisible(false);

			m_vPoolLabel.at(i).at(j) = label;
		}
	}

	//弹出消息
	auto poolNotice = ImageView::create(NN_POOL_NOTICE);
	poolNotice->setName("poolNotice");
	addChild(poolNotice, 9999);
	poolNotice->setAnchorPoint(Vec2(0.5, 0.5));
	poolNotice->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));

	auto noticeContentLabel = Label::createWithTTF("", NN_FONT_TTF, 25, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	noticeContentLabel->setName("noticeContentLabel");
	poolNotice->addChild(noticeContentLabel);
	noticeContentLabel->setAnchorPoint(Vec2(0.5, 1));
	noticeContentLabel->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 10));
	noticeContentLabel->setString("");

	auto noticeNameLabel = Label::createWithTTF("", NN_FONT_TTF, 25, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::CENTER);
	noticeNameLabel->setName("noticeNameLabel");
	poolNotice->addChild(noticeNameLabel);
	noticeNameLabel->setAnchorPoint(Vec2(1, 1));
	noticeNameLabel->setPosition(Vec2(WIN_SIZE.width / 2 + 20, WIN_SIZE.height / 2 - 40));
	noticeNameLabel->setString("");

	auto noticeMoneyLabel = LabelAtlas::create("123456789", FNT_YELLOW_ONE_V66, 25, 34, '0');
	noticeMoneyLabel->setName("noticeMoneyLabel");
	poolNotice->addChild(noticeMoneyLabel);
	noticeMoneyLabel->setAnchorPoint(Vec2(0, 1));
	noticeMoneyLabel->setPosition(Vec2(WIN_SIZE.width / 2 + 5, WIN_SIZE.height / 2 - 40));
	noticeMoneyLabel->setString("");

	poolNotice->setVisible(false);
}

void GameTableUIV88::leaveTheGame() {
    HNAudioEngine::getInstance()->stopBackgroundMusic();
    RoomLogic()->closeRoom();
    PlatformState* state = PlatformState::create();
    state->setType(ROOMLIST);
    StateManager::getInstance()->setState(state);
}

//进入或者短线重连恢复游戏桌子场景
void GameTableUIV88::resumeTheCatchBaker(tag_s2c_station_catch_banker* pMsg, BYTE bGameStation) {

    m_GameStatus = bGameStation;

    m_bPrepareTime = pMsg->bPrepareTime;
    m_bCatchBankerTime = pMsg->bCatchBankerTime;
    m_bBetTime = pMsg->bBetTime;
    m_bSendCardTime = pMsg->bSendCardTime;
    memcpy(m_bCardTypeMul, pMsg->bCardTypeMul, sizeof(m_bCardTypeMul));

    m_bCurUser = pMsg->bCurChairID;
    m_bNextUser = pMsg->bNextChairID;

    //上阶段图标隐藏
    m_PrepareBtn->setVisible(false);
    for (int i = 0; i < m_iPlayerCount; i++) {
        m_vPrepareImage.at(i)->setVisible(false);
    }

    if (m_bCurUser == INVALID_DESKNO) { //第一个操作者
        //进入叫庄阶段了
        m_GameStatus = GS_CATCH_BANKER;
    } else {
        //叫庄中图标隐藏
    }

    //叫庄中图标显示
    m_StageImage->loadTexture(NN_STATE_IMAGE_CATCH);
    m_StageImage->setVisible(pMsg->bNextChairID != _GameTableLogic->getMySeatNo());
    m_StageImage->setPosition(getStateTipPos(m_iPlayerCount, m_bNextUser));

    //叫庄按钮
    m_CatchBankerBtn->setVisible(_GameTableLogic->getMySeatNo() == m_bNextUser);
    m_NotCatchBtn->setVisible(_GameTableLogic->getMySeatNo() == m_bNextUser);

    //setTimer(m_bNextUser, CLIENT_TIMER_CATCH_BANKER, pMsg->bLeftTime);
    m_bTimerChairID = m_bNextUser;
    m_iTimerID = CLIENT_TIMER_CATCH_BANKER;
	_clockObj->start(pMsg->bLeftTime);
}

void GameTableUIV88::resumeTheGameBet(tag_s2c_station_bet* pMsg, BYTE bGameStation) {
    m_GameStatus = bGameStation;

    m_bPrepareTime = pMsg->bPrepareTime;
    m_bCatchBankerTime = pMsg->bCatchBankerTime;
    m_bBetTime = pMsg->bBetTime;
    m_bSendCardTime = pMsg->bSendCardTime;
    memcpy(m_bCardTypeMul, pMsg->bCardTypeMul, sizeof(m_bCardTypeMul));

    m_bBankerID = pMsg->bBankerID;
    for (int i = 0; i < 4; i++) {
        m_i64BetArr[i] = pMsg->i64BetValue[i];
    }
    for (int i = 0; i < m_iPlayerCount; i++) {
        m_vPlayerBetValue.at(i) = pMsg->i64BetMoney[i];
    }


    m_PrepareBtn->setVisible(false);
    //庄家标志
    int iBankerViewID = _GameTableLogic->logicToViewSeatNo(m_bBankerID);
    m_vPlayerHead.at(iBankerViewID)->setBankerVisible(true);

    //投注阶段图标
    m_StageImage->loadTexture(NN_STATE_IMAGE_BET);
    m_StageImage->setVisible(_GameTableLogic->getMySeatNo() == m_bBankerID);
    m_StageImage->setPosition(getStateTipPos(m_iPlayerCount, m_bBankerID));

    //押注
    bool bMeIsBanker = _GameTableLogic->getMySeatNo() == m_bBankerID;

    for (int i = 0; i < m_iPlayerCount; i++) {
        if (i != m_bBankerID && m_vPlayerState.at(i) == PS_PLAY) {
            if (bMeIsBanker) {
                if (m_vPlayerBetValue.at(i) > 0) {
                    createBetValue(_GameTableLogic->logicToViewSeatNo(i));
                }
            } else {
                if (i == _GameTableLogic->getMySeatNo()) {
                    if (m_vPlayerBetValue.at(i) <= 0) {
                        createBetBtn();
                    } else {
                        createBetValue(_GameTableLogic->logicToViewSeatNo(i));
                    }
                } else {
                    if (m_vPlayerBetValue.at(i) > 0) {
                        createBetValue(_GameTableLogic->logicToViewSeatNo(i));
                    }
                }
            }
        }
    }

   // setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_BET, pMsg->bLeftTime);
    m_bTimerChairID = _GameTableLogic->getMySeatNo();
    m_iTimerID = CLIENT_TIMER_BET;
	_clockObj->start(pMsg->bLeftTime);
}

void GameTableUIV88::resumeTheGameSendCard(tag_s2c_station_send* pMsg, BYTE bGameStation) {
    m_GameStatus = bGameStation;

    m_bPrepareTime = pMsg->bPrepareTime;
    m_bCatchBankerTime = pMsg->bCatchBankerTime;
    m_bBetTime = pMsg->bBetTime;
    m_bSendCardTime = pMsg->bSendCardTime;
    memcpy(m_bCardTypeMul, pMsg->bCardTypeMul, sizeof(m_bCardTypeMul));

    m_bBankerID = pMsg->bBankerID;
    for (int i = 0; i < m_iPlayerCount; i++) {
        m_vPlayerBetValue.at(i) = pMsg->i64BetMoney[i];
    }
    for (int i = 0; i < 5; i++) {
        m_vPlayerPokerValue.at(_GameTableLogic->getMySeatNo()).at(i) = pMsg->bCard[i];
    }

    m_PrepareBtn->setVisible(false);
    if (m_iPlayerCount != 6) {
        //庄家标志
        int iBankerViewID = _GameTableLogic->logicToViewSeatNo(m_bBankerID);
        m_vPlayerHead.at(iBankerViewID)->setBankerVisible(true);

        for (int i = 0; i < m_iPlayerCount; i++) {
            if (i != m_bBankerID && m_vPlayerBetValue.at(i) > 0 && m_vPlayerState.at(i) == PS_PLAY) {
                createBetValue(_GameTableLogic->logicToViewSeatNo(i));
            }
        }
    } else {
        for (int i = 0; i < m_iPlayerCount; i++) {
			if (m_vPlayerBetValue.at(i) > 0 && m_vPlayerState.at(i) == PS_PLAY) {
                createBetValue(_GameTableLogic->logicToViewSeatNo(i));
            }
        }
    }

    sendCardWithNoAction();
    m_bTimerChairID = _GameTableLogic->getMySeatNo();
    m_iTimerID = CLIENT_TIMER_OPEN_CARD;
	_clockObj->start(pMsg->bLeftTime);
    //setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_OPEN_CARD, pMsg->bLeftTime);
}


bool GameTableUIV88::onTouchBegan(Touch* touch, Event* event) {
//	Point point = touch->getLocation();
//	point = _tableBg->convertToNodeSpace(point);
//
//	if (0 == _toolbarBg->getName().compare("1")) {
//		_toolbarBg->setName("0");
//
//		_toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//			CallFunc::create([this]() {
//			this->getChildByTag(em_toolbarBtn)->setVisible(true);
//		})));
//	}
//
//	this->getChildByTag(em_toolbarBtn)->setVisible(true);
	return true;
}

void GameTableUIV88::onEnter() {
    HNGameUIBase::onEnter();
    
    if (!m_backGroundMusicPath.empty()) {
        HNAudioEngine::getInstance()->playBackgroundMusic(m_backGroundMusicPath.c_str(),true);
    }
	_fScaleX = WIN_SIZE.width / DESIGN_SIZE.width;
	_fScaleY = WIN_SIZE.height / DESIGN_SIZE.height;

	//触摸
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto myListener = EventListenerTouchOneByOne::create();
	myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
	myListener->setSwallowTouches(false);
	dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

	//bg
	_tableBg = Sprite::create(GAME_BG_COMMON_V66);
	addChild(_tableBg);
	_tableBg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
	//_tableBg->setScaleX(_fScaleX);
	//_tableBg->setScaleY(_fScaleY);

    auto topBg = Sprite::create("hallimg/niuniu/top.png");
    if (topBg) {
        topBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        topBg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height - 10));
        addChild(topBg);
        
        auto node_action = CSLoader::createNode("hallimg/niuniu/Node_niuniuFire.csb");
        if (node_action) {
            node_action->setPosition(Vec2(topBg->getContentSize().width/2,60));
            topBg->addChild(node_action);
            
            auto action = CSLoader::createTimeline("hallimg/niuniu/Node_niuniuFire.csb");
            if (action) {
                node_action->runAction(action);
                action->gotoFrameAndPlay(0);
            }
        }
    }
    
//    auto tongbiNiuBgImg = ImageView::create(NN_TONGBINIU_DESKTOP_BG);
//    addChild(tongbiNiuBgImg);
//    tongbiNiuBgImg->setAnchorPoint(Vec2(0, 0));
//    tongbiNiuBgImg->setPosition(Vec2(WIN_SIZE.width / 2 - tongbiNiuBgImg->getContentSize().width / 2, WIN_SIZE.height / 2 - tongbiNiuBgImg->getContentSize().height / 2));
//    tongbiNiuBgImg->setVisible(false);

//    //waterMark
//    char markStr[128] = { 0 };
//    if (m_iPlayerCount == 2 || m_iPlayerCount == 4) {
//        sprintf(markStr, NN_MARK_BG, 4);
//        tongbiNiuBgImg->setVisible(false);
//    }
//    else {
//        sprintf(markStr, NN_MARK_BG, m_iPlayerCount);
//        tongbiNiuBgImg->setVisible(true);
//    }

	//   auto waterMarkImage = Sprite::create(markStr);
	//   addChild(waterMarkImage);
	//waterMarkImage->setAnchorPoint(Vec2(0.5, 0.5));
	//waterMarkImage->setPosition(WIN_SIZE/2);
	// 		_tableBg = Sprite::create(NN_GAME_BG);
	// 		addChild(_tableBg);
	// 		_tableBg->setPosition(Vec2(WIN_SIZE.width / 4, WIN_SIZE.height / 2));
	// 		_tableBg->setScaleX(_fScaleX);
	// 		_tableBg->setScaleY(_fScaleY);
	//
	// 		_tableBgRight = Sprite::create(NN_GAME_BG);
	// 		addChild(_tableBgRight);
	// 		_tableBgRight->setPosition(Vec2(WIN_SIZE.width / 4 * 3, WIN_SIZE.height / 2));
	// 		_tableBgRight->setFlippedX(true);
	// 		_tableBgRight->setScaleX(_fScaleX);
	// 		_tableBgRight->setScaleY(_fScaleY);

	auto faPaiQi = Sprite::create("hallimg/niuniu/Card_maker.png");
	faPaiQi->setPosition(Vec2(WIN_SIZE.width / 2 + 300, WIN_SIZE.height / 2 + 150));
	faPaiQi->setName("FaPaiQi");
	addChild(faPaiQi);

	createClock();
	createRewardPool();
	createToolBarUI();
	createPlayerHeadUI(m_iPlayerCount);
	resetPlayerHeadUI();
	createReadyUI();

	createPoker(m_iPlayerCount);
	createPokerPointImage(m_iPlayerCount);
	playScannnerT = 0;



	_GameTableLogic = new GameTableLogic(this, _bDeskIndex, _bAutoCreate, m_iPlayerCount);
	_GameTableLogic->enterGame();
}

void GameTableUIV88::onExit() {
    HttpClient::getInstance()->destroyInstance();
    
    HNGameUIBase::onExit();
    unschedule(schedule_selector(GameTableUIV88::updateJackpot));
    
}

void GameTableUIV88::addUser(BYTE seatNo, bool bMe) {
    UserInfoStruct * user = _GameTableLogic->getUserBySeatNo(seatNo);

    auto viewSeatNo = _GameTableLogic->logicToViewSeatNo(seatNo);

    m_vPlayerHead.at(viewSeatNo)->setVisible(true);
    m_vPlayerHead.at(viewSeatNo)->setHead(user);
    m_vPlayerHead.at(viewSeatNo)->setNick(user->nickName);
    m_vPlayerHead.at(viewSeatNo)->setGold(user->i64Money);
}

void GameTableUIV88::setGamePlayerCount() {
    UINT gameID = RoomLogic()->getSelectedRoom()->uNameID;
//	UINT gameID = UserDefault::getInstance()->getIntegerForKey("Enter_Game_ID");
    m_backGroundMusicPath = "";
    if (gameID == GAME_TWO_NAME_ID) {
        m_iPlayerCount = 2;
        m_backGroundMusicPath = "v66/music/niuniu/erren.mp3";
    } else if (gameID == GAME_FOUR_NAME_ID) {
        m_iPlayerCount = 4;
        m_backGroundMusicPath = "v66/music/niuniu/siren.mp3";
    } else if (gameID == GAME_SIX_NAME_ID) {
        m_iPlayerCount = 6;
        m_backGroundMusicPath = "v66/music/niuniu/tongbi.mp3";
    } else {

    }
   
    
}

//void GameTableUIV88::updateClock(float dt) {
//    if (m_fTimeCount < 0) {
//        m_ClockBg->setVisible(false);
//        this->unschedule(schedule_selector(GameTableUIV88::updateClock));
//
//        timeOutCallback(m_bTimerChairID, m_iTimerID);
//        //showTimeTip(0, false);
//        return;
//    }
//
//    char s[10] = { 0 };
//    sprintf(s, "%.2d", static_cast<int>(m_fTimeCount));
//    m_ClockTimeLabel->setString(s);
//    m_ClockBg->setVisible(true);
//
//    m_fTimeCount -= floor(dt);
//
//    if (m_fTimeCount < 3) {
//       // HNAudioEngine::getInstance()->playEffect(NN_SOUND_CLOCK);
//    }
//}

//void GameTableUIV88::setTimer(BYTE bChairID, int iTimerID, float fTime) {
//    m_bTimerChairID = bChairID;
//    m_iTimerID = iTimerID;
//    m_fClockTime = fTime;
//    m_fTimeCount = fTime - 1;
//    m_bDoTimeOutCallback = true;
//
//    //马上显示一次
//    m_ClockBg->setVisible(true);
//    char s[10] = { 0 };
//    sprintf(s, "%.2d", static_cast<int>(m_fClockTime));
//    m_ClockTimeLabel->setString(s);
//
//    this->schedule(schedule_selector(GameTableUIV88::updateClock), 1.0f);
//
//    //showTimeTip(iTimerID);
//}

void GameTableUIV88::killTimer() {
	_clockObj->stop();
}

void GameTableUIV88::timeOutCallback(BYTE bChairID, int iTimerID) {
    if (_GameTableLogic->getMySeatNo() != bChairID) return;

    if (m_bDoTimeOutCallback == false) return;

    //准备超时
    if (iTimerID == CLIENT_TIMER_PREPARE) {
        if (!RoomLogic()->connected()) {
            leaveTheGame();
        } else {
            _GameTableLogic->sendUserUp();
        }
		return;
    }
	
	if (m_vPlayerState.at(_GameTableLogic->getMySeatNo()) == PS_WATCH) return;

	if (iTimerID == CLIENT_TIMER_CATCH_BANKER) {
        catchBankerClickCallback(m_NotCatchBtn, Widget::TouchEventType::ENDED);
    } else if (iTimerID == CLIENT_TIMER_BET) {
        if (bChairID == m_bBankerID || m_vPlayerBetValue.at(bChairID) > 0) return;

        betMoneyClickCallback(m_vBetBtn[0], Widget::TouchEventType::ENDED);
    } else if (iTimerID == CLIENT_TIMER_OPEN_CARD) {
        openCardClickCallback(m_OpenCardBtn, Widget::TouchEventType::ENDED);
    } else {

    }
}

vector<vector<Vec2>> GameTableUIV88::getCardPosVector(int iPlayerCount) {
    vector<vector<Vec2>> vPos;

    float fMyCardInstance = 42;
    float fOtherCardInstance = 27;

    float fMyCardStartX = WIN_SIZE.width / 2 - (4 * fMyCardInstance + 103) / 2 + 103 /2;
    float fOtherCardStartX = WIN_SIZE.width / 2  - (4 * fOtherCardInstance + 65) / 2 + 65 /2;
	float offsetTopX = WIN_SIZE.width*0.65 - (4 * fOtherCardInstance + 65) / 2 + 65 / 2;
	float offsetTopY = WIN_SIZE.height - WIN_SIZE.height / 8;

    if (iPlayerCount == 2) {
        for (int i = 0; i < 2; i++) {
            vector<Vec2> v;
            v.clear();
            for (int j = 0; j < 5; j++) {
                if (i == 0) {
                    v.push_back(Vec2(fMyCardStartX + fMyCardInstance * j, 140));
                } else {
					v.push_back(Vec2(offsetTopX + fOtherCardInstance * j, offsetTopY));
                }
            }
            vPos.push_back(v);
        }
    } else if (iPlayerCount == 4) {
        for (int i = 0; i < 4; i++) {
            vector<Vec2> v;
            v.clear();
            for (int j = 0; j < 5; j++) {
                if (i == 0) {
                    v.push_back(Vec2(fMyCardStartX + fMyCardInstance * j, 140));
                } else if (i == 1) {
                    v.push_back(Vec2(fOtherCardStartX + WIN_SIZE.width /2 - 250 + fOtherCardInstance*j, WIN_SIZE.height / 2));
                } else if (i == 2) {
					v.push_back(Vec2(offsetTopX + fOtherCardInstance * j, offsetTopY));
                } else if (i == 3) {
                    v.push_back(Vec2(fOtherCardStartX - WIN_SIZE.width /2 + 250 + fOtherCardInstance*j, WIN_SIZE.height / 2));
                }
            }
            vPos.push_back(v);
        }
    } else if (iPlayerCount == 6) {
        for (int i = 0; i < 6; i++) {
            vector<Vec2> v;
            v.clear();
            for (int j = 0; j < 5; j++) {
                if (i == 0) {
                    v.push_back(Vec2(fMyCardStartX + fMyCardInstance * j, 140));
                } else if (i == 1) {
                    v.push_back(Vec2(fOtherCardStartX + WIN_SIZE.width / 2 - 250 + fOtherCardInstance*j, WIN_SIZE.height / 2 - 76 - 20));
                } else if (i == 2) {
                    v.push_back(Vec2(fOtherCardStartX + WIN_SIZE.width / 2 - 250 + fOtherCardInstance*j, WIN_SIZE.height / 2 + 76 + 38 - 20));
                } else if (i == 3) {
					v.push_back(Vec2(offsetTopX + fOtherCardInstance * j, offsetTopY));
                } else if (i == 4) {
                    v.push_back(Vec2(fOtherCardStartX - WIN_SIZE.width / 2 + 250 + fOtherCardInstance*j, WIN_SIZE.height / 2 + 76 + 38 - 20));
                } else if (i == 5) {
                    v.push_back(Vec2(fOtherCardStartX - WIN_SIZE.width / 2 + 250 + fOtherCardInstance*j, WIN_SIZE.height / 2 - 76 - 20));
                }
            }
            vPos.push_back(v);
        }
    }



    return vPos;
}

void GameTableUIV88::sendCard() {


    //通比特殊处理
    if (m_iPlayerCount == 6) {
        m_bBankerID = 0;
    }
    
    m_iSendRoundNum = 0;
    int iBankViewID = _GameTableLogic->logicToViewSeatNo(m_bBankerID);
    m_iSendViewID = (iBankViewID - 1 + m_iPlayerCount)%m_iPlayerCount;
    m_iLastViewID = 0;
    for(int i = 0; i < m_iPlayerCount; i++)
    {
        BYTE bChairID = (m_bBankerID - (i + 1) + m_iPlayerCount)%m_iPlayerCount;
        if(m_vPlayerState.at(bChairID) == PS_PLAY)
        {
            m_iLastViewID = _GameTableLogic->logicToViewSeatNo(bChairID);
            break;
        }
    }
	if (m_iPlayerCount == 6)
	{
		this->schedule(schedule_selector(GameTableUIV88::sendCardTimer), 0.08);
	}
	else if (m_iPlayerCount == 4)
	{
		this->schedule(schedule_selector(GameTableUIV88::sendCardTimer), 0.12);
	}
	else
	{
		this->schedule(schedule_selector(GameTableUIV88::sendCardTimer), 0.2);
	}


}

void GameTableUIV88::sendCardWithNoAction() {
    auto vPos = getCardPosVector(m_iPlayerCount);

    for (int i = 0; i < m_iPlayerCount; i++) {
		
			int iViewiD = _GameTableLogic->logicToViewSeatNo(i);
			bool bVisible = (m_vPlayerState.at(i) == PS_WATCH) ? false : true;
			for (int j = 0; j < 5; j++) {
				m_vPlayerPoker.at(iViewiD).at(j)->setVisible(bVisible);
				m_vPlayerPoker.at(iViewiD).at(j)->setCardValue(m_vPlayerPokerValue.at(i).at(j));
				m_vPlayerPoker.at(iViewiD).at(j)->ChangePoke();
				m_vPlayerPoker.at(iViewiD).at(j)->setPosition(vPos.at(iViewiD).at(j));
				if (iViewiD == 0) {
					m_vPlayerPoker.at(iViewiD).at(j)->setScale(1.0);
					m_vPlayerPoker.at(iViewiD).at(j)->setCardTouchEnable(true);
				}
				else {
					m_vPlayerPoker.at(iViewiD).at(j)->setScale(0.8);
				}
			
		}
    }

    //m_OpenCardBtn->setVisible(true);
	if (m_vPlayerState.at(_GameTableLogic->getMySeatNo()) == PS_PLAY)
	{
		setBtnTouch(m_OpenCardBtn, true);
	}
}

void GameTableUIV88::sendCardTimer(float dt) {
    //HNAudioEngine::getInstance()->playEffect(NN_SOUND_SEND);
    
    //找出下一位
    for(int i = 0; i < m_iPlayerCount; i++)
    {
        m_iSendViewID = (m_iSendViewID + 1) % m_iPlayerCount;
        BYTE bSendChairID = _GameTableLogic->viewToLogicSeatNo(m_iSendViewID);

        if(m_vPlayerState.at(bSendChairID) != PS_WATCH)
        {
            break;
        }
    }
    
    //发牌
    auto vPos = getCardPosVector(m_iPlayerCount);
    auto curVec2 = vPos.at(m_iSendViewID).at(m_iSendRoundNum);
    m_vPlayerPoker.at(m_iSendViewID).at(m_iSendRoundNum)->setVisible(true);
    if(m_iSendViewID == 0)
    {
        m_vPlayerPoker.at(m_iSendViewID).at(m_iSendRoundNum)->FaMingPai(curVec2);
    }
    else{
        m_vPlayerPoker.at(m_iSendViewID).at(m_iSendRoundNum)->FaAnPai(curVec2);
    }
    
    //是否自己发完
    if(m_iSendRoundNum == 4 && 0 == m_iSendViewID && m_vPlayerState.at(_GameTableLogic->getMySeatNo()) == PS_PLAY)
    {
        for (int i = 0; i < 5; i++) {
            m_vPlayerPoker.at(0).at(i)->setCardTouchEnable(true);
        }
        
        scheduleOnce(schedule_selector(GameTableUIV88::showOpenBtn), 1.0f);
    }
    
    //轮数+1
    if(m_iSendViewID == m_iLastViewID)
    {
        m_iSendRoundNum++;
    }
    
    //5张发完
    if (m_iSendRoundNum == 5) {
        
        //结束
        if(m_iSendViewID == m_iLastViewID)
        {
            unschedule(schedule_selector(GameTableUIV88::sendCardTimer));
            return;
        }
    }
}

void GameTableUIV88::showOpenCard(BYTE bChairID, BYTE bPoint) {
    float fMyCardInstance = 42;
    float fOtherCardInstance = 27;

    int iViewChairID = _GameTableLogic->logicToViewSeatNo(bChairID);

    auto vPos = getCardPosVector(m_iPlayerCount);


    if (bPoint == 0) {
        for (int i = 0; i < 5; i++) {
            m_vPlayerPoker.at(iViewChairID).at(i)->stopAllActions();
            m_vPlayerPoker.at(iViewChairID).at(i)->setPosition(vPos.at(iViewChairID).at(i));
        }
        return;
    }

    BYTE bSpecialViewID = m_iPlayerCount/2;

    float instance = 0;
    if (iViewChairID == 0) {
        instance = fMyCardInstance;

        for (int i = 0; i < 3; i++) {
            m_vPlayerPoker.at(iViewChairID).at(i)->stopAllActions();
            m_vPlayerPoker.at(iViewChairID).at(i)->setPosition(vPos.at(iViewChairID).at(i + 1));
        }

        for (int i = 3; i < 5; i++) {
            m_vPlayerPoker.at(iViewChairID).at(i)->stopAllActions();
            m_vPlayerPoker.at(iViewChairID).at(i)->setPosition(vPos.at(iViewChairID).at(4).x - instance*2.5 + (i - 3)*instance, vPos.at(iViewChairID).at(2).y + 43);
            m_vPlayerPoker.at(iViewChairID).at(i)->setLocalZOrder(50 - (5 - i));
        }
    } else if (iViewChairID == bSpecialViewID) {
        instance = fOtherCardInstance;

        for (int i = 0; i < 3; i++) {
            m_vPlayerPoker.at(iViewChairID).at(i)->setPosition(Vec2(vPos.at(iViewChairID).at(i + 1).x, vPos.at(iViewChairID).at(i + 1).y - 32));
        }

        for (int i = 3; i < 5; i++) {
            m_vPlayerPoker.at(iViewChairID).at(i)->setPosition(vPos.at(iViewChairID).at(4).x - instance*2.5 + (i - 3)*instance, vPos.at(iViewChairID).at(2).y);
            m_vPlayerPoker.at(iViewChairID).at(i)->setLocalZOrder(50 - (5 - i));
        }

        m_vPlayerPokerPointImage.at(iViewChairID)->setPositionY(m_vPlayerPokerPointImage.at(iViewChairID)->getPositionY() - 35);
    } else {
        instance = fOtherCardInstance;

        for (int i = 0; i < 3; i++) {
            m_vPlayerPoker.at(iViewChairID).at(i)->setPosition(vPos.at(iViewChairID).at(i + 1));
        }

        for (int i = 3; i < 5; i++) {
            m_vPlayerPoker.at(iViewChairID).at(i)->setPosition(vPos.at(iViewChairID).at(4).x - instance*2.5 + (i - 3)*instance, vPos.at(iViewChairID).at(2).y + 32);
            m_vPlayerPoker.at(iViewChairID).at(i)->setLocalZOrder(50 - (5 - i));
        }
    }
}

Vec2 GameTableUIV88::getStateTipPos(int iPlayerCount, BYTE bDeskStation) {
    BYTE bViewChairID = _GameTableLogic->logicToViewSeatNo(bDeskStation);

    if (iPlayerCount == 2) {
        if (bViewChairID == 0) {
            return Vec2(WIN_SIZE.width /2, WIN_SIZE.height /2 - 100);
        } else if (bViewChairID == 1) {
            return Vec2(WIN_SIZE.width /2, WIN_SIZE.height /2 + 100);
        }
    } else if (iPlayerCount == 4) {
        if (bViewChairID == 0) {
            return Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 100);
        } else if (bViewChairID == 1) {
            return Vec2(WIN_SIZE.width /2 + 360, WIN_SIZE.height / 2);
        } else if (bViewChairID == 2) {
            return Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 100);
        } else if (bViewChairID == 3) {
            return Vec2(WIN_SIZE.width / 2 - 360, WIN_SIZE.height / 2);
        }
    } else if ( iPlayerCount == 6) {
		return Vec2::ZERO;
    }
	return Vec2::ZERO;
}

// 	void GameTableUIV88::showTimeTip(int iTimerID, bool bShow)
// 	{
// 		if (getChildByName("time_tip"))
// 		{
// 			getChildByName("time_tip")->removeFromParentAndCleanup(true);
// 		}
//
// 		if (!bShow)
// 		{
// 			return;
// 		}
//
// 		auto tipImage = ImageView::create(NN_TIP_PREPARE);
// 		tipImage->setName("time_tip");
// 		addChild(tipImage);
// 		tipImage->setPosition(Vec2(WIN_SIZE.width /2, WIN_SIZE.height / 2 - 60));
//
// 		if (iTimerID == CLIENT_TIMER_PREPARE)
// 		{
// 			tipImage->loadTexture(NN_TIP_PREPARE);
// 		}
// 		else if (iTimerID == CLIENT_TIMER_CATCH_BANKER)
// 		{
// 			tipImage->loadTexture(NN_TIP_BANKER);
// 		}
// 		else if (iTimerID == CLIENT_TIMER_BET)
// 		{
// 			tipImage->loadTexture(NN_TIP_BET);
// 		}
// 		else if (iTimerID == CLIENT_TIMER_OPEN_CARD)
// 		{
// 			tipImage->loadTexture(NN_TIP_OPEN);
// 		}
// 	}

void GameTableUIV88::updateTip(float dt) {
    int iTag = m_TextTip->getTag();
    if (iTag == 0) {
        unschedule(schedule_selector(GameTableUIV88::updateTip));
        _GameTableLogic->sendUserUp();
        return;
    }

	//	char str[128] = { 0 };
	//	iTag -= 1;
	//	sprintf(str, "金币数额低于%lld，%ds后离开游戏！", m_i64TipMoney, iTag);
	//	m_TextTip->setText(GBKToUtf8(str));
	//	m_TextTip->setTag(iTag);
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

void GameTableUIV88::setBtnTouch(Button* btn, bool bTouch) {
    if (m_iPlayerCount == PLAYER_COUNT_TWO || m_iPlayerCount == PLAYER_COUNT_FOUR) {
        btn->setVisible(bTouch);
        return;
    }

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

void GameTableUIV88::showOpenBtn(float dt) {
    //m_OpenCardBtn->setVisible(true);
    setBtnTouch(m_OpenCardBtn, true);
    if (m_bAuto) {
        openCardClickCallback(m_OpenCardBtn, Widget::TouchEventType::ENDED);
    }
}

void GameTableUIV88::tipCallback() {
    if (tipMsg->bIsExit == 1) {
        //准备关闭掉
        m_PrepareBtn->setVisible(false);

        auto layer = GamePromptLayer::create();
//        layer->setCloseExitBtn();
		//char str[128] = { 0 };
		//int sec = 3;
		//sprintf(str, "金币数额低于%lld，%ds后离开游戏！", tipMsg->i64Money, sec);
		//layer->showPrompt(GBKToUtf8(str));
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
            _GameTableLogic->sendUserUp();
        });
        layer->setColorLayerTouchEnabled(false);

        m_i64TipMoney = tipMsg->i64Money;
        m_TextTip = layer->getPromptText();
        m_TextTip->setTag(sec);
        schedule(schedule_selector(GameTableUIV88::updateTip), 1.0f);
    } else {

    }
}

void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
    if (bVisible) {
		if (!m_Notice)
		{
			m_Notice = GameNotice::create();
			m_Notice->setLocalZOrder(NOTICE_ZORDER);
			m_Notice->setName("GameNotice");
		}
		m_Notice->openNotic(this, 10000, message, nCount, nType);
    }
}

void GameTableUIV88::resetUI() {
    Vec2 pos = WIN_SIZE/2;
    auto faPaiQi = dynamic_cast<Sprite*>(getChildByName("FaPaiQi"));
    if(faPaiQi)
    {
        pos = faPaiQi->getPosition();
    }
    
    //新的一局重置一些ui
	auto vPos = getCardPosVector(m_iPlayerCount);
    for (int i = 0; i < m_iPlayerCount; i++) {
        for (int j = 0; j < 5; j++) {
            m_vPlayerPoker.at(i).at(j)->setPosition(pos);
            m_vPlayerPoker.at(i).at(j)->setCardValue(0);
            m_vPlayerPoker.at(i).at(j)->ChangePoke();
            m_vPlayerPoker.at(i).at(j)->setVisible(false);
            m_vPlayerPoker.at(i).at(j)->setScale(0.2);
            m_vPlayerPoker.at(i).at(j)->setLocalZOrder(50 + j);
            m_vPlayerPoker.at(i).at(j)->setCardTouchEnable(false);

            auto btn = dynamic_cast<ImageView*>(m_vPlayerPoker.at(i).at(j)->getChildByName("ClickBtn"));
            btn->setTag(1);
	
        }


		m_vPlayerPokerPointImage.at(i)->setPositionY(vPos.at(i).at(0).y-50);
        m_vPlayerPokerPointImage.at(i)->setVisible(false);
        m_vPlayerHead.at(i)->setBankerVisible(false);	
		
		if (m_vPlayerResultValueBtn.at(i))
		{
			m_vPlayerResultValueBtn.at(i)->setVisible(false);
		}
		auto nodeP= this->getChildByTag(em_niuniuNote + 11 * i);
		if (nodeP)
		{
			nodeP->removeFromParent();
		}
    }

	if (this->getChildByTag(em_zhuangTag))
	{
		this->getChildByTag(em_zhuangTag)->removeFromParent();

	}

	//for (int i = 0; i < m_vResultEffect.size(); i++)
	//{
	//	m_vResultEffect.at(i)->removeFromParent();
	//}
	//m_vResultEffect.clear();
}

void GameTableUIV88::setCoinMove() {
    int randMax = 30;
    int randNum = rand() % randMax;
    if (randNum < 10) {
        randNum = 10;
    }

    vector<int> vKey;
    vector<long long> vValue;
    for (int i = 0; i < m_iPlayerCount; i++) {
        if (m_i64PlayerResultValue[i] > 0) {
            vKey.push_back(i);
            vValue.push_back(m_i64PlayerResultValue[i]);
        }
    }

    for (int i = 0; i < vKey.size() - 1; i++) {
        int index = i;
        for (int j = i + 1; j < vKey.size(); j++) {
            if (vValue.at(j) < vValue.at(index)) {
                index = j;
            }
        }
        if (index != i) {
            swap(vKey.at(i), vKey.at(index));
            swap(vValue.at(i), vValue.at(index));
        }
    }

    for (int i = vKey.size() - 1; i >= 0; i--) {
        if (i == 0) {
            vValue.at(i) = randNum;
        } else {
            vValue.at(i) = vValue.at(i) / vValue.at(0) * randNum;
        }
    }

    for (int i = 0; i < vKey.size(); i++) {
        showCoinMove(vKey.at(i), vValue.at(i));
    }

}

void GameTableUIV88::showCoinMove(int chairID, long long goldNum) {
    int viewID = _GameTableLogic->logicToViewSeatNo(chairID);

    int widthRange = 120;
    int heightRange = 120;

    int randWidth = 0;
    int randHeight = 0;
    int symbol = 1;
    int tag = 10000;
    


    for (int i = 0; i < goldNum; i++) {
        randWidth = rand() % widthRange;
        randHeight = rand() % heightRange;
        auto image = Sprite::create(NN_GOLD_IMAGE);
        addChild(image, tag +i);
        image->setPosition(Vec2(WIN_SIZE.width / 2 + randWidth*symbol, WIN_SIZE.height / 2 + randHeight*symbol));
        symbol = -symbol;

        Vec2 pos = Vec2(getPlayerHeadPos().at(viewID).x + 100, getPlayerHeadPos().at(viewID).y + 70);
        image->runAction(Sequence::create(DelayTime::create(0.2f), MoveTo::create(0.5f, pos), RemoveSelf::create(), nullptr));
    }

    this->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([&]() {
        HNAudioEngine::getInstance()->playEffect(NN_SOUND_CLEAR_COIN);
    }), nullptr));
}

void GameTableUIV88::showGoldRain() {
    if (m_iPlayerCount == PLAYER_COUNT_FOUR) {
        for (int i = 0; i < m_iPlayerCount; i++) {
            if (i == m_bBankerID) {
                if (m_i64PlayerResultValue[i] < 0) {
                    return;
                }
            } else {
                if (m_i64PlayerResultValue[i] > 0) {
                    return;
                }
            }
        }

        UIGoldRain::ShowGoldRain(Director::getInstance()->getRunningScene());
    }
}

void GameTableUIV88::showCowRun(long long wuhuaMoney) {
	m_CsbNode = CSLoader::createNode(NN_COW_RUN);
	m_CsbNode->setPosition(WIN_SIZE/2);
	addChild(m_CsbNode, 100000);

	auto antAction = CSLoader::createTimeline(NN_COW_RUN);
	m_CsbNode->runAction(antAction);
	//antAction->play("wuhuaniu", false);
    antAction->gotoFrameAndPlay(0,false);
	m_NickLabel = Label::createWithTTF("", FONT_TTF_FZCY, 50, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
	m_NickLabel->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 80));
	string str = Tools::addComma(wuhuaMoney);
	m_NickLabel->setString(GBKToUtf8(str.c_str()));
	addChild(m_NickLabel, 100001);
	m_NickLabel->setVisible(false);

	   CallFunc* func1 = CallFunc::create([&]() {
		   m_NickLabel->setVisible(true);
			
     });

		 this->runAction(Sequence::create(DelayTime::create(2.0f), func1, nullptr));
		 m_NickLabel->runAction(Sequence::create(DelayTime::create(3.0f), RemoveSelf::create(), nullptr));

	//m_CsbNode->runAction(Sequence::create(MoveBy::create(2.0f, Vec2(WIN_SIZE.width+550, 0)), RemoveSelf::create(), nullptr));
//     Animation* animation = Animation::create();
// 
//     SpriteFrame* frame;
// 
//     char str[128] = { 0 };
// 
//     for (int i = 0; i < 6; i++) {
//         sprintf(str, "common/animation/cowRun/cow%d.png", i);
//         frame = SpriteFrame::create(str, Rect(0, 0, 700, 400));
//         animation->addSpriteFrame(frame);
//     }
//     animation->setLoops(4);
//     animation->setRestoreOriginalFrame(true);
//     animation->setDelayPerUnit(0.6f/6.0f);
// 
//     Animate* animate = Animate::create(animation);
// 
//     m_AnimateImage = Sprite::create("common/animation/cowRun/cow0.png");
//     addChild(m_AnimateImage, 100000);
//     m_AnimateImage->setPosition(Vec2(-400, WIN_SIZE.height / 2));
//     m_AnimateImage->setVisible(false);
// 
//     Spawn* spawn = Spawn::create(animate, MoveBy::create(2.0f, Vec2(WIN_SIZE.width+550, 0)), nullptr);
// 
//     CallFunc* func1 = CallFunc::create([&]() {
//         m_AnimateImage->setVisible(true);
//     });
// 
//     CallFunc* func2 = CallFunc::create([&]() {
//         m_AnimateImage->setVisible(false);
//     });
// 
//     Sequence* seq = Sequence::create(func1, spawn, func2, nullptr);
// 
//     m_AnimateImage->runAction(seq);
}

void GameTableUIV88::showBankerAnimation(Vec2 pos) {
	Vec2 bankerPos;
	Size temp = m_vPlayerHead.at(0)->getContentSize();
	bankerPos = Vec2(getPlayerHeadPos().at(m_iBankerViewID).x - temp.width / 3+5,
		getPlayerHeadPos().at(m_iBankerViewID).y-8);

	auto zhuangEffectNode = CSLoader::createNode(NN_QIANGZHUANG_ANI);
	addChild(zhuangEffectNode);
	zhuangEffectNode->setAnchorPoint(Vec2(0.5, 0.5));
	zhuangEffectNode->setTag(em_zhuangTag);
	zhuangEffectNode->setScale(0.8);
	zhuangEffectNode->setPosition(bankerPos);
	auto antActionZhuang = CSLoader::createTimeline(NN_QIANGZHUANG_ANI);

	//zhuangEffectNode->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
	if (zhuangEffectNode) {
		zhuangEffectNode->runAction(antActionZhuang);
		antActionZhuang->play("qiangzhuang", false);
		antActionZhuang->gotoFrameAndPlay(0, 19, false);
	}

	//auto secondNode = CSLoader::createNode(NN_QIANGZHUANG_ANI_2);
	//addChild(secondNode);
	//auto antActionS = CSLoader::createTimeline(NN_QIANGZHUANG_ANI_2);
	//secondNode->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
	//if (secondNode) {
	//	secondNode->runAction(antActionS);
	//	antActionS->play("zhuang_2", false);
	//}



	//if (bankV.x == 0.5 && bankV.y == 0.5)
	//{
	//	bankerPos = Vec2(getPlayerHeadPos().at(m_iBankerViewID).x - 50,
	//		getPlayerHeadPos().at(m_iBankerViewID).y);
	//}

	//if (bankV.x == 0.0 && bankV.y == 1.0)
	//{
	//	bankerPos = Vec2(getPlayerHeadPos().at(m_iBankerViewID).x,
	//		getPlayerHeadPos().at(m_iBankerViewID).y-77);

	//}

	//auto move = Spawn::create(MoveTo::create(0.3f, bankerPos), ScaleTo::create(0.3f, 0.1f), nullptr);
	//secondNode->runAction(Sequence::create(DelayTime::create(1.0f), move, RemoveSelf::create(), nullptr));
	//
	//CallFunc* func3 = CallFunc::create([&]() {
	//	auto endNode = CSLoader::createNode(NN_QIANGZHUANG_ANI_3);
	//	
	//	Vec2 bankV = m_vPlayerHead.at(m_iBankerViewID)->getAnchorPoint();
	//	Size bankSize = m_vPlayerHead.at(m_iBankerViewID)->getContentSize();
	//	Vec2 bankerPos;
	//	addChild(endNode, 50);
	//
	//	bankerPos = Vec2(getPlayerHeadPos().at(m_iBankerViewID).x,
	//		getPlayerHeadPos().at(m_iBankerViewID).y);
	//	if (bankV.x == 0.5 && bankV.y == 0.5)
	//	{
	//		bankerPos = Vec2(getPlayerHeadPos().at(m_iBankerViewID).x - bankSize.width/2+15,
	//			getPlayerHeadPos().at(m_iBankerViewID).y - bankSize.height / 2+15);
	//	}
	//
	//	if (bankV.x == 0.0 && bankV.y == 1.0)
	//	{
	//		bankerPos = Vec2(getPlayerHeadPos().at(m_iBankerViewID).x,
	//			getPlayerHeadPos().at(m_iBankerViewID).y - bankSize.height+30);

	//	}

	//	endNode->setPosition(bankerPos.x + 11, bankerPos.y + 62);
	//	endNode->setTag(em_zhuangTag);
	//	endNode->setScale(0.98);
	//	auto antActionEnd = CSLoader::createTimeline(NN_QIANGZHUANG_ANI_3);
	//	if (endNode) {
	//		endNode->runAction(antActionEnd);
	//		antActionEnd->play("zhuang_3", false);
	//		antActionEnd->gotoFrameAndPlay(0, 19, false);
	//	}
	//});
	//this->runAction(Sequence::create(DelayTime::create(0.7f), func3, nullptr));
	

}

void GameTableUIV88::prepareClickCallback(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    if (m_GameStatus != GS_FREE) {
        return;
    }

    m_PrepareBtn->setVisible(false);
    m_OpenCardBtn->setVisible(true);
    setBtnTouch(m_OpenCardBtn, false);

    _GameTableLogic->sendPrepare(_GameTableLogic->getMySeatNo());

    killTimer();
}

void GameTableUIV88::catchBankerClickCallback(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    if (_GameTableLogic->getMySeatNo() != m_bNextUser)	return;

    if (m_GameStatus != GS_CATCH_BANKER)	return;


    Button* pBtn = (Button*)pSender;
    BYTE tag =static_cast<BYTE>(pBtn->getTag());

    m_CatchBankerBtn->setVisible(false);
    m_NotCatchBtn->setVisible(false);

    _GameTableLogic->sendCatchData(_GameTableLogic->getMySeatNo(), tag);

    killTimer();
}

void GameTableUIV88::betMoneyClickCallback(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

	HNAudioEngine::getInstance()->playEffect(NN_SOUND_BET_CION);

    if (m_GameStatus != GS_BET)	return;

    Button* btn = (Button*)pSender;
    int tag = btn->getTag();

    for (int i = 0; i < 4; i++) {
        m_vBetBtn.at(i)->setVisible(false);

    }

    _GameTableLogic->sendBet(_GameTableLogic->getMySeatNo(), m_i64BetArr[tag]);

    //killTimer();
    m_bDoTimeOutCallback = false;
}

void GameTableUIV88::openCardClickCallback(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    if (m_GameStatus != GS_SEND_CARD) return;

    //m_OpenCardBtn->setVisible(false);
    setBtnTouch(m_OpenCardBtn, false);

    _GameTableLogic->sendOpenCard(_GameTableLogic->getMySeatNo());

    //killTimer();
    m_bDoTimeOutCallback = false;
}

void GameTableUIV88::autoClickback(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }

    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

    Button* btn = (Button*)pSender;
    int tag = btn->getTag();
    ImageView* image  = dynamic_cast<ImageView*>(btn->getChildByName("autoImage"));


    if (tag == 0) { //点击取消
        image->loadTexture(NN_AUTO_WORD);
        btn->setTag(1);
        m_bAuto = false;
    } else if (tag == 1) {
        image->loadTexture(NN_CANCEL_AUTO_WORD);
        btn->setTag(0);
        m_bAuto = true;

        if (m_PrepareBtn->isVisible()) {
            prepareClickCallback(m_PrepareBtn, Widget::TouchEventType::ENDED);
        }

// 			if (m_OpenCardBtn->isVisible() && m_OpenCardBtn->isEnabled())
// 			{
// 				openCardClickCallback(m_OpenCardBtn, Widget::TouchEventType::ENDED);
// 			}
    }
}

void GameTableUIV88::poolBtnClickCallback(Ref* pSender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED) return;

	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);

	auto image = dynamic_cast<ImageView*>(getChildByName("poolInfoImage"));
	bool bVisible = image->isVisible();
	if (bVisible)
	{
		image->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.2, 0.1), Hide::create()));
	}
	else
	{
		image->runAction(Sequence::createWithTwoActions(Show::create(), ScaleTo::create(0.2, 1)));
	}
}

//玩家坐下(非自己)
void GameTableUIV88::userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    addUser(userSit->bDeskStation, userSit->bDeskStation == _GameTableLogic->getMySeatNo());
}

//玩家站起
void GameTableUIV88::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    if (m_vPlayerState.at(userSit->bDeskStation) == PS_WATCH) {
        BYTE bViewChairID = _GameTableLogic->logicToViewSeatNo(userSit->bDeskStation);
        m_vPlayerHead.at(bViewChairID)->setVisible(false);
        m_vPrepareImage.at(bViewChairID)->setVisible(false);

        if (m_bGameEnd) {
            for (int j = 0; j < 5; j++) {
                m_vPlayerPoker.at(bViewChairID).at(j)->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
                m_vPlayerPoker.at(bViewChairID).at(j)->setCardValue(0);
                m_vPlayerPoker.at(bViewChairID).at(j)->ChangePoke();
                m_vPlayerPoker.at(bViewChairID).at(j)->setVisible(false);
                m_vPlayerPoker.at(bViewChairID).at(j)->setScale(0.2);
                m_vPlayerPoker.at(bViewChairID).at(j)->setLocalZOrder(50 + j);
            }

			
			auto p = this->getChildByTag(em_niuniuNote + 11 * bViewChairID);
			if (p)
			{
				p->removeFromParent();

			}

            m_vPlayerPokerPointImage.at(bViewChairID)->setVisible(false);
			if (m_vPlayerResultValueBtn.at(bViewChairID))
			{
				m_vPlayerResultValueBtn.at(bViewChairID)->setVisible(false);
			}

			if (m_iBankerViewID == bViewChairID)
			{
				if (this->getChildByTag(em_zhuangTag))
				{
					this->getChildByTag(em_zhuangTag)->removeFromParent();
				}
			}
			
			
        }
    }
}

void GameTableUIV88::onGameConfig(tag_s2c_config* msg) {
    //数据填充
    m_bPrepareTime = msg->bPrepareTime;
    m_bCatchBankerTime = msg->bCatchBankerTime;
    m_bBetTime = msg->bBetTime;
    m_bSendCardTime = msg->bSendCardTime;

    memcpy(m_bCardTypeMul, msg->bCardTypeMul, sizeof(msg->bCardTypeMul));

    if (m_GameStatus == GS_FREE) {
        m_bTimerChairID = _GameTableLogic->getMySeatNo();
        m_iTimerID = CLIENT_TIMER_PREPARE;
		_clockObj->start(m_bPrepareTime);
        //setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_PREPARE, m_bPrepareTime);
    }
}

void GameTableUIV88::onGamePrepare(tag_s2c_prepare* msg) {
    m_vPlayerPrepare.at(msg->bChairID) = 1;

    auto viewChairID = _GameTableLogic->logicToViewSeatNo(msg->bChairID);
    if (m_bGameEnd && m_vPlayerResultValueBtn.at(viewChairID)) {
        m_vPlayerResultValueBtn.at(viewChairID)->setVisible(false);
    }
    m_vPrepareImage.at(viewChairID)->setVisible(true);
    if (msg->bChairID == _GameTableLogic->getMySeatNo()) {
        HNAudioEngine::getInstance()->playEffect(NN_SOUND_READY);
        m_PrepareBtn->setVisible(false);
        killTimer();
    }
}

void GameTableUIV88::onGameCatchBanker(tag_s2c_catch_banker* msg) {
    //新一局
	playScannnerT = playScannnerT + 1;
	if (m_vPlayerState.at(_GameTableLogic->getMySeatNo()) != PS_WATCH && playScannnerT==1){
		auto scanner = Scanner::create();
		this->addChild(scanner, 100000);
		scanner->starPlayerEffect();
		scanner->setPosition(50, 50);
	}

    m_bGameEnd = false;
    resetUI();

    //上阶段图标隐藏
    for (int i = 0; i < m_iPlayerCount; i++) {
        m_vPrepareImage.at(i)->setVisible(false);
    }
    killTimer();
	catchBankerMsg = msg;
	//data
	m_bCurUser = msg->bCurChairID;
	m_bNextUser = msg->bNextChairID;
	//msg->bCurIsCatch

	if (m_bCurUser == INVALID_DESKNO) { //第一个操作者
		//进入叫庄阶段了
		//HNAudioEngine::getInstance()->playEffect(NN_SOUND_TIP_CATCH);
		m_GameStatus = GS_CATCH_BANKER;
	}
	else {
		if (msg->bCurIsCatch == 0) {
			HNAudioEngine::getInstance()->playEffect(NN_SOUND_NO_CATCH);
		}
		else if (msg->bCurIsCatch == 1) {
			HNAudioEngine::getInstance()->playEffect(NN_SOUND_CATCH);
		}
	}

	//叫庄中图标显示
	m_StageImage->loadTexture(NN_STATE_IMAGE_CATCH);
	m_StageImage->setVisible(msg->bNextChairID != _GameTableLogic->getMySeatNo());
	m_StageImage->setPosition(getStateTipPos(m_iPlayerCount, m_bNextUser));

	//叫庄按钮
	m_CatchBankerBtn->setVisible(_GameTableLogic->getMySeatNo() == m_bNextUser);
	m_NotCatchBtn->setVisible(_GameTableLogic->getMySeatNo() == m_bNextUser);
	//setTimer(m_bNextUser, CLIENT_TIMER_CATCH_BANKER, m_bCatchBankerTime);
    m_bTimerChairID = m_bNextUser;
    m_iTimerID = CLIENT_TIMER_CATCH_BANKER;
	_clockObj->start(m_bCatchBankerTime);

}


void GameTableUIV88::onGameConfirmBanker(tag_s2c_confirm_banker_two* msg) {
    m_CatchBankerBtn->setVisible(false);
    m_NotCatchBtn->setVisible(false);
    m_StageImage->setVisible(false);
    killTimer();

    m_bBankerID = msg->bBankerID;
    memcpy(m_i64BetArr, msg->i64BetArr, sizeof(msg->i64BetArr));

    int viewChairID = _GameTableLogic->logicToViewSeatNo(msg->bBankerID);
    m_iBankerViewID = viewChairID;
    //庄家头像
    showBankerCallback = [&]() {
        m_vPlayerHead.at(m_iBankerViewID)->setBankerVisible(false);
    };

    Vec2 bankerPos = Vec2(getPlayerHeadPos().at(viewChairID).x + m_vPlayerHead.at(viewChairID)->getBankerPos().x,
                          getPlayerHeadPos().at(viewChairID).y + m_vPlayerHead.at(viewChairID)->getBankerPos().y);
    showBankerAnimation(bankerPos);


    //投注阶段图标
    m_StageImage->loadTexture(NN_STATE_IMAGE_BET);
    m_StageImage->setVisible(_GameTableLogic->getMySeatNo() == m_bBankerID);
    m_StageImage->setPosition(getStateTipPos(m_iPlayerCount, m_bBankerID));

    //动态创建下注按钮
    if (_GameTableLogic->getMySeatNo() != msg->bBankerID && m_vPlayerState.at(_GameTableLogic->getMySeatNo()) == PS_PLAY) {
			createBetBtn();
    }

    runAction(Sequence::create(CallFunc::create([&]() {
        HNAudioEngine::getInstance()->playEffect(NN_SOUND_CATCH);
    }), DelayTime::create(0.8f), CallFunc::create([&]() {
        //HNAudioEngine::getInstance()->playEffect(NN_SOUND_TIP_BET);
    }),nullptr));

    //setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_BET, m_bBetTime);
    m_bTimerChairID = _GameTableLogic->getMySeatNo();
    m_iTimerID = CLIENT_TIMER_BET;
	_clockObj->start(m_bBetTime);

    m_GameStatus = GS_BET;
}

void GameTableUIV88::onGameBet(tag_s2c_bet_two* msg) {
    if (m_iPlayerCount == 6) {
        //新一局 六人没抢庄
		auto scanner = Scanner::create();
		this->addChild(scanner, 100000);
		scanner->starPlayerEffect();
		scanner->setPosition(50, 50);
        m_bGameEnd = false;
        resetUI();

        for (int i = 0; i < m_iPlayerCount; i++) {
            m_vPrepareImage.at(i)->setVisible(false);
        }
    } else {
        if (msg->bBetChairID == _GameTableLogic->getMySeatNo()) {
            for (int i = 0; i < 4; i++) {
                m_vBetBtn.at(i)->setVisible(false);
            }
        }
    }
    //killTimer();
    //押注值
    m_vPlayerBetValue.at(msg->bBetChairID) = msg->i64BetMoney;

    BYTE bViewChairID = _GameTableLogic->logicToViewSeatNo(msg->bBetChairID);
	if (m_vPlayerState.at(msg->bBetChairID) == PS_PLAY)
	{
		createBetValue(bViewChairID);
	}
}

void GameTableUIV88::onGameSendCard(tag_s2c_send_card* msg) {
    m_StageImage->setVisible(false);

    int iMyChairID = _GameTableLogic->getMySeatNo();
    int iMyViewChairID = _GameTableLogic->logicToViewSeatNo(iMyChairID);

    //填充牌值 保存
    for (int i = 0; i < 5; i++) {
        m_vPlayerPoker.at(iMyViewChairID).at(i)->setCardValue(msg->bCards[i]);

        m_vPlayerPokerValue.at(iMyChairID).at(i) = msg->bCards[i];
    }

    //发牌
    sendCard();

    m_GameStatus = GS_SEND_CARD;
    m_bTimerChairID = iMyChairID;
    m_iTimerID = CLIENT_TIMER_OPEN_CARD;
	_clockObj->start(m_bSendCardTime);
   // setTimer(iMyChairID, CLIENT_TIMER_OPEN_CARD, m_bSendCardTime);
}

void GameTableUIV88::onGameOpenOk(tag_s2c_open_card_ok* msg) {
    int iViewChairID = _GameTableLogic->logicToViewSeatNo(msg->bChairID);
    m_vOkImage.at(iViewChairID)->setVisible(true);
}

void GameTableUIV88::onGameOpenCard(tag_s2c_open_card* msg) {
    m_vPlayerPokerPoint.push_back(msg->bCardPoint);

    int iViewChairID = _GameTableLogic->logicToViewSeatNo(msg->bChairID);

    //保存新排序牌(其他玩家会开牌）
    for (int i = 0; i < 5; i++) {
        m_vPlayerPoker.at(iViewChairID).at(i)->setCardValue(msg->bCard[i]);
        m_vPlayerPoker.at(iViewChairID).at(i)->ChangePoke();
        m_vPlayerPokerValue.at(msg->bChairID).at(i) = msg->bCard[i];
    }
    showOpenCard(msg->bChairID, msg->bCardPoint);
    //自己牌点击
    if (iViewChairID == 0) {
        for (int i = 0; i < 5; i++) {
            m_vPlayerPoker.at(0).at(i)->setCardTouchEnable(false);
        }
    }

    char str[64] = { 0 };
    sprintf(str, NN_RESULT_PATH, msg->bCardPoint);
    m_vPlayerPokerPointImage.at(iViewChairID)->loadTexture(str);
    m_vPlayerPokerPointImage.at(iViewChairID)->setVisible(true);
	m_vPlayerPokerPointImage.at(iViewChairID)->setScale(1.0f);
	m_vPlayerPokerPointImage.at(iViewChairID)->runAction(Sequence::create(ScaleTo::create(0.2f, 1.5f, 1.5f), DelayTime::create(0.2f), ScaleTo::create(0.1f, 1.0f, 1.0f), nullptr));

    if (msg->bChairID == _GameTableLogic->getMySeatNo()) {
        sprintf(str, NN_SOUND_BULL, msg->bCardPoint);
        HNAudioEngine::getInstance()->playEffect(str);
        m_isMyNN = msg->bCardPoint == 10;
    }

	if (msg->bCardPoint > 0)
	{
		auto niuniu1_9Node = CSLoader::createNode(NN_RESULIT_ANI_1_9);
		addChild(niuniu1_9Node, 54);
		niuniu1_9Node->setPosition((Vec2(m_vPlayerPokerPointImage.at(iViewChairID)->getPositionX(), m_vPlayerPokerPointImage.at(iViewChairID)->getPositionY())));
		niuniu1_9Node->setScale(1.3);
		niuniu1_9Node->setTag(11 * iViewChairID + em_niuniuNote);
		auto antAction = CSLoader::createTimeline(NN_RESULIT_ANI_1_9);
		if (niuniu1_9Node)
		{
			niuniu1_9Node->runAction(antAction);
			antAction->play("shuziniu", false);
		}
		//m_vResultEffect.push_back(niuniu1_9Node);

	}	

    //有五花牛
    if (msg->bCardPoint == 11) {
        m_bHasCowRun = true;
    }
}



void GameTableUIV88::onGameResult(tag_s2c_result* msg) {
    //隐藏ok
    for (int i = 0; i < m_iPlayerCount; i++) {
        m_vOkImage.at(i)->setVisible(false);
    }


    BYTE bViewChairID = _GameTableLogic->logicToViewSeatNo(msg->bChairID);

    if (m_iPlayerCount == 6) {
		if (m_vBetValueBtn.at(bViewChairID))
		{
			m_vBetValueBtn.at(bViewChairID)->setVisible(false);
		}
    } else {
        if (msg->bChairID != m_bBankerID) {
			if (m_vBetValueBtn.at(bViewChairID))
			{
				m_vBetValueBtn.at(bViewChairID)->setVisible(false);
			}
        }
        //animation
        if (m_iPlayerCount == 4) {
            //四人牛牛
            if (msg->bChairID == _GameTableLogic->getMySeatNo()) {
                if (msg->i64ResultMoney > 0 && m_isMyNN) {
                    //
                    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
                    addChild(colorLayer, NOTICE_ZORDER);
                    colorLayer->runAction(Sequence::create(DelayTime::create(2.0f), RemoveSelf::create(), nullptr));
                    auto node = CSLoader::createNode("v88/animation/cow/inpocket/inpocket.csb");
                    if (node) {
                        node->setPosition(WIN_SIZE/2);
                        addChild(node, NOTICE_ZORDER);
                        auto action = CSLoader::createTimeline("v88/animation/cow/inpocket/inpocket.csb");
                        if (action) {
                            node->runAction(action);
                            action->play("inpocket", false);
                        }
                    }
                }
            }
        }
    }
	if (m_vPlayerState.at(msg->bChairID) == PS_PLAY)
	{
		createResultValue(bViewChairID, msg->i64ResultMoney);
	}

    //保存结果
    m_i64PlayerResultValue[msg->bChairID] = msg->i64ResultMoney;

    //数据齐全
    bool bAllData = true;

    for (int i = 0; i < m_iPlayerCount; i++) {
        if (m_i64PlayerResultValue[i] == 0 && m_vPlayerState.at(i) == PS_PLAY) {
            bAllData = false;
        }
    }
//     if (m_iPlayerCount == PLAYER_COUNT_FOUR && bAllData && _GameTableLogic->getMySeatNo() == m_bBankerID) {
//         //播放通杀金币雨动画
//         showGoldRain();
//     }
    //五花牛特效
	if (m_bHasCowRun && bViewChairID==0) {
        //showCowRun();
    }

    if (bAllData) {
        setCoinMove();
        //
        for (int i = 0; i < m_iPlayerCount; i++) {
            if (m_vPlayerState.at(i) == PS_PLAY) {
                //
                auto coin = FloatCoin::create();
                if (coin) {
                    int viewID = _GameTableLogic->logicToViewSeatNo(i);
                    coin->openParent(this, getPlayerHeadPos().at(viewID), 10000);
                    coin->updateCoin(m_i64PlayerResultValue[i]);
                }
            }
        }
    }

    //刷新头像金币
// 		long long i64Money = _GameTableLogic->getUserBySeatNo(msg->bChairID)->i64Money;
// 		m_vPlayerHead.at(bViewChairID)->setGold(i64Money);
	if (m_vPlayerState.at(msg->bChairID) == PS_PLAY)
	{
		long long i64Money = m_vPlayerHead.at(bViewChairID)->getGold();
		m_vPlayerHead.at(bViewChairID)->setGold(i64Money + msg->i64ResultMoney);
        
	}

    //新开始清空数据
    if (bAllData) {
        //输赢音效
        if (m_i64PlayerResultValue[_GameTableLogic->getMySeatNo()] > 0) {
            HNAudioEngine::getInstance()->playEffect(GAME_WIN_AUDIO_EFFECT_V66);
        } else {
            HNAudioEngine::getInstance()->playEffect(GAME_LOSE_AUDIO_EFFECT_V66);
        }

        resetNewGame();
        m_bTimerChairID = _GameTableLogic->getMySeatNo();
        m_iTimerID = CLIENT_TIMER_PREPARE;
		_clockObj->start(m_bPrepareTime);
       // setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_PREPARE, m_bPrepareTime);
    }
}

void GameTableUIV88::onGameTipExit(tag_s2c_tip_exit* msg) {
    if (_GameTableLogic->getMySeatNo() != msg->bChairID) {
        if (msg->bIsExit == 1 && m_iPlayerCount != 6) {
            //2、4人踢人动画
            //金币不足的不是自己，需要踢人的操作
            auto node = CSLoader::createNode("v88/animation/cow/gameover/gameover.csb");
            if (node) {
                BYTE bViewChairID = _GameTableLogic->logicToViewSeatNo(msg->bChairID);
                Vec2 offsetPos = Vec2(100, 0);
                if (bViewChairID == 1 && m_iPlayerCount == 4) {
                    offsetPos = Vec2(-100, 0);
                    node->setScaleX(-1);
                }
                node->setPosition(m_vPlayerHead.at(bViewChairID)->getPosition() + offsetPos);
                addChild(node);
                auto action = CSLoader::createTimeline("v88/animation/cow/gameover/gameover.csb");
                if (action) {
                    node->runAction(action);
                    action->play("gameover", false);
                    BYTE chairID = msg->bChairID;
                    //
                    runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
                        m_vPlayerHead.at(bViewChairID)->setVisible(false);
                        m_vPlayerHead.at(bViewChairID)->setBankerVisible(false);
                        m_vPrepareImage.at(bViewChairID)->setVisible(false);
                        for (int j = 0; j < 5; j++) {
                            m_vPlayerPoker.at(bViewChairID).at(j)->setVisible(false);
                            auto nodeP= this->getChildByTag(em_niuniuNote + 11 * chairID);
                            if (nodeP) {
                                nodeP->setVisible(false);
                            }
                        }
                        m_vPlayerPokerPointImage.at(bViewChairID)->setVisible(false);
                        if (m_iBankerViewID == bViewChairID) {
                            if (this->getChildByTag(em_zhuangTag)) {
                                this->getChildByTag(em_zhuangTag)->setVisible(false);
                            }
                        }
                    }), nullptr));
                    
                }
            }
        }
    }
    else {
        tipMsg = msg;
        //不退出
        if (tipMsg->bIsExit == 0) {
            //下一局离开
            if (m_bNextOut) {
                this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
                    _GameTableLogic->sendUserUp();
                })));
            } else {
                //新位置开始按钮
                m_PrepareBtn->setPosition(m_OpenCardBtn->getPosition());
                
                if (m_iPlayerCount == 2 || m_iPlayerCount == 4) {
                    m_PrepareBtn->setVisible(true);
                    m_OpenCardBtn->setVisible(false);
                } else if (m_iPlayerCount == 6) {
                    if (m_bAuto) {
                        m_PrepareBtn->setVisible(false);
                        m_OpenCardBtn->setVisible(true);
                        setBtnTouch(m_OpenCardBtn, false);
                        
                        this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
                            prepareClickCallback(m_PrepareBtn, Widget::TouchEventType::ENDED);
                        })));
                    } else {
                        m_PrepareBtn->setVisible(true);
                        m_OpenCardBtn->setVisible(false);
                    }
                }
            }
            return;
        }
        
        //要退出
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
            tipCallback();
        })));
    }
}

void GameTableUIV88::onGameNotify(tag_s2c_notify* msg) {
    if (msg->i64WinMoney < 0) return;
	char str[128] = { 0 };

	if (msg->bDeskID == INVALID_BYTE)
	{
		//广播
		if (msg->bGameID == 1) {
			sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
		}
		else if (msg->bGameID == 2) {
			sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
		}
		else if (msg->bGameID == 3) {
			sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
		}
		else {
			return;
		}
		showNotice(str, 1, 1);


		//自己才看得到弹窗消息
		if (msg->nUserID == _GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo())->dwUserID)
		{

			showCowRun(msg->i64WinMoney);


			/*sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName));
			auto label2 = dynamic_cast<Label*>(image->getChildByName("noticeNameLabel"));
			label2->setString(ForceGBKToUtf8(str));*/


		/*	auto label3 = dynamic_cast<LabelAtlas*>(image->getChildByName("noticeMoneyLabel"));
			sprintf(str, "%lld", Tools::addComma(msg->i64WinMoney).c_str());
			label3->setString(str);
			auto image = dynamic_cast<ImageView*>(getChildByName("poolNotice"));*/

			////已经在显示，要显示另一个时跳过
			//if (image->isVisible())
			//{
			//	return;
			//}

			//auto label1 = dynamic_cast<Label*>(image->getChildByName("noticeContentLabel"));

			

			//if (msg->bGameID == 1) {
			//	sprintf(str, "%s %s", GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName));
			//	label1->setString(ForceGBKToUtf8(str));

			//	sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName));
			//}
			//else if (msg->bGameID == 2) {
			//	sprintf(str, "%s %s", GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName));
			//	label1->setString(ForceGBKToUtf8(str));

			//	sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName));
			//}
			//else if (msg->bGameID == 3) {
			//	sprintf(str, "%s %s", GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName));
			//	label1->setString(ForceGBKToUtf8(str));

			//	sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName));
			//}
			//else {
			//	return;
			//}


			////展示
			//image->runAction(Sequence::create(Show::create(), ScaleTo::create(0.3f, 1.0f), DelayTime::create(2.2f), ScaleTo::create(0.2, 0.1f), Hide::create(), nullptr));
		}
	}
	else
	{
		if (msg->bGameID == 1) {
			sprintf(str, GetText("NN_Catch_Bull_Notify"), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName), msg->bDeskID + 1, ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
		}
		else if (msg->bGameID == 2) {
			sprintf(str, GetText("NN_Catch_Bull_Notify"), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName), msg->bDeskID + 1, ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
		}
		else if (msg->bGameID == 3) {
			sprintf(str, GetText("NN_Catch_Bull_Notify"), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName), msg->bDeskID + 1, ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
		}
		else {
			return;
		}

		showNotice(str, 1, 0);
	}
}

void GameTableUIV88::onGamePoolMoney(tag_s2c_reward_pool* msg)
{
	/*long long i64PoolMoney = msg->i64RewardPoolMoney;
	auto label = dynamic_cast<Label*>(getChildByName("poolMoneyLabel"));
	string str = Tools::addComma(i64PoolMoney);
	label->setString(GBKToUtf8(str.c_str()));*/
}
void GameTableUIV88::updateJackpot(float dt)
{
    m_i64Jackpot += random(50,100);
    auto label = dynamic_cast<Label*>(getChildByName("poolMoneyLabel"));
    if (label) {
        string str = Tools::addComma(m_i64Jackpot);
        label->setString(GBKToUtf8(str.c_str()));
    }
    
}
//获奖记录
void GameTableUIV88::onGameRewardGetRecord(tag_s2c_reward_get_record* msg)
{
	//记录几条了
	if (msg->bCount == 1)
	{
		m_PoolRecord.push_back(*msg);
	}
	else
	{
		m_PoolRecord.push_front(*msg);
	}
	
	if (m_PoolRecord.size() > 5)
	{
		m_PoolRecord.pop_front();
	}
	
	list<tag_s2c_reward_get_record>::iterator iter = m_PoolRecord.begin();

	for (int i = 0; i < 5; i++)
	{
		if (i < m_PoolRecord.size())
		{
			if (iter == m_PoolRecord.end())
			{
				continue;
			}
			char str1[124] = { 0 };
			memcpy(str1, iter->nickName, sizeof(iter->nickName));
			m_vPoolLabel.at(i).at(0)->setString(ForceGBKToUtf8(str1));
			m_vPoolLabel.at(i).at(0)->setVisible(true);

			char str2[124] = { 0 };
			memcpy(str2, iter->byCard, sizeof(iter->byCard));
			m_vPoolLabel.at(i).at(1)->setString(str2);
			m_vPoolLabel.at(i).at(1)->setVisible(true);

			string str3 = Tools::addComma(iter->i64RewardMoney);
			m_vPoolLabel.at(i).at(2)->setString(GBKToUtf8(str3.c_str()));
			m_vPoolLabel.at(i).at(2)->setVisible(true);

			iter++;
		}
		else
		{
			m_vPoolLabel.at(i).at(0)->setVisible(false);
			m_vPoolLabel.at(i).at(1)->setVisible(false);
			m_vPoolLabel.at(i).at(2)->setVisible(false);
		}
	}
}

//个人贡献
void GameTableUIV88::onGamePersonalContribution(tag_s2c_personal_contribution* msg)
{
	long long i64PersonalContribution = msg->i64PersonalContribution;

	auto image = dynamic_cast<ImageView*>(getChildByName("poolInfoImage"));

	auto label = dynamic_cast<Label*>(image->getChildByName("poolMyMoneyLabel"));
	string str = Tools::addComma(i64PersonalContribution);
	label->setString(GBKToUtf8(str.c_str()));

}

//自己状态
void GameTableUIV88::onGameUpdatPlayerState(tag_s2c_player_state* msg)
{
	m_vPlayerState.at(msg->bChairID) = msg->bPlayerState;
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

void GameTableUIV88::resetNewGame() {
    m_GameStatus = GS_FREE;
    m_bGameEnd = true;
    m_bHasCowRun = false;
    m_vPlayerPrepare.clear();
    //m_vPlayerResultValueBtn.clear();

    memset(m_i64BetArr, 0, sizeof(m_i64BetArr));
    m_vPlayerBetValue.clear();
    m_vPlayerPokerValue.clear();
    m_vPlayerPokerPoint.clear();
    memset(m_i64PlayerResultValue, 0, sizeof(m_i64PlayerResultValue));
	m_vPlayerState.clear();
	readyCount.clear();

    m_vPlayerPrepare.resize(m_iPlayerCount);
    m_vPlayerBetValue.resize(m_iPlayerCount);
    m_vPlayerPokerValue.resize(m_iPlayerCount);
    for (int i = 0; i < m_iPlayerCount; i++) {
        m_vPlayerPokerValue.at(i).resize(5);
    }
    m_vPlayerResultValueBtn.resize(m_iPlayerCount);
	for (int i = 0; i < m_iPlayerCount; i++)
	{
		m_vPlayerState.push_back(PS_WATCH);
	}

	playScannnerT = 0;
}

void GameTableUIV88::resetData() {
	//ui
	_fScaleX = 0;
	_fScaleY = 0;
	_tableSize = Size(0, 0);

	_tableBg = nullptr;
	_tableBgRight = nullptr;

	_toolbarBg = nullptr;
//    m_ClockBg = nullptr;
//    m_ClockTimeLabel = nullptr;

	m_StageImage = nullptr;

	m_PrepareBtn = nullptr;
	m_CatchBankerBtn = nullptr;
	m_NotCatchBtn = nullptr;
	m_OpenCardBtn = nullptr;
//    tongbiNiuBgImg = nullptr;

	m_vPlayerHead.clear();
	m_vPrepareImage.clear();
	m_vOkImage.clear();
	m_vBetBtn.clear();
	m_vBetValueBtn.clear();
	m_vPlayerPoker.clear();
	m_vPlayerPokerPointImage.clear();
	m_vResultEffect.clear();
	m_vPlayerResultValueBtn.clear();

	m_vPoolInfoImage.clear();
	m_vPoolLabel.clear();
	m_PoolRecord.clear();


	m_Notice = nullptr;
    
    m_isMyNN = false;

	//数据
	m_bPrepareTime = 0;
	m_bCatchBankerTime = 0;
	m_bBetTime = 0;
	m_bSendCardTime = 0;
	memset(m_bCardTypeMul, 0, sizeof(m_bCardTypeMul));

//    m_fClockTime = 0;
	m_fTimeCount = 0;
	m_bTimerChairID = INVALID_BYTE;
	m_iTimerID = 0;
	m_iSendRoundNum = 0;
	m_iSendViewID = 0;

	//m_iPlayerCount = 0;
	m_GameStatus = GS_FREE;
	m_bGameEnd = false;
	m_bAuto = false;
	m_bNextOut = false;
	m_bHasCowRun = false;
	m_bCurUser = INVALID_BYTE;
	m_bNextUser = INVALID_BYTE;
	m_bBankerID = INVALID_BYTE;
	m_vPlayerPrepare.clear();

	memset(m_i64BetArr, 0, sizeof(m_i64BetArr));
	m_vPlayerBetValue.clear();
	m_vPlayerPokerValue.clear();
	m_vPlayerPokerPoint.clear();
	memset(m_i64PlayerResultValue, 0, sizeof(m_i64PlayerResultValue));
	m_vPlayerState.clear();

	m_vPlayerPrepare.resize(m_iPlayerCount);
	m_vPlayerBetValue.resize(m_iPlayerCount);
	m_vBetValueBtn.resize(m_iPlayerCount);
	m_vPlayerPokerValue.resize(m_iPlayerCount);
	for (int i = 0; i < m_iPlayerCount; i++) {
		m_vPlayerPokerValue.at(i).resize(5);
	}
	m_vPlayerResultValueBtn.resize(m_iPlayerCount);
	for (int i = 0; i < m_iPlayerCount; i++)
	{
		m_vPlayerState.push_back(PS_WATCH);
	}
}
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
            auto i64MyMoney = msg->i64MyMoney;
            
            runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                txtReward->setString(Tools::addComma(luckyMoney));
                auto viewSeatNo = _GameTableLogic->logicToViewSeatNo(_GameTableLogic->getMySeatNo());
                m_vPlayerHead.at(viewSeatNo)->setGold(i64MyMoney);
            }), DelayTime::create(retainTime), CallFunc::create([&](){
                this->removeChildByName("rewardResult");
            }), NULL));
        }
    }
}
