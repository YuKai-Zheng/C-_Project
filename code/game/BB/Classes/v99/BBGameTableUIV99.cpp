#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
//#include "../../TTNN/Classes/v99/BRNNSetLayerV99.h"
//#include "../../TTNN/Classes/GameTable/BRNNActions.h"
#include "../../../../code/common/LibHNUI/UI/UIGoldRain.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"

#include "BBGameTableUIV99.h"
#include "BBGameTableLogic.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "BBLoopSpriteCellV99.h"
#include "BBPlayerHeadV99.h"
#include "BBGameStateV99.h"
#include "BBSettlementV99.h"
#include "BBNoteLabelCellV99.h"
#include "BBHelpLayerV99.h"

#include "Tool/Tools.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/ToolBarLayer.h"
#include "HNLobby/v66/ExtraReward.h"
#include "HNLobby/v99/PlatformResV99.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"

namespace BB {

#define WINSIZE Director::getInstance()->getWinSize()
#define NOTICE_ZORDER 1000

////////////////////////////////////////////////////////////////////////////
////常量

enum Em_TableBtnTag_V99 {
    em_toolbarBtn = 1000,
    em_toolbarChangeTableBtn,
    em_toolbarSetBtn,
    em_toolbarHelpBtn,
    em_toolbarStandupBtn,
    em_toolbarLeaveBtn,
    em_toolbarNextOutBtn
};

enum Em_BetButtonTag_V99 {
    //bet
    em_betLamMaxTag = 1100, //大倍率lamberghini
    em_betBMWMaxTag,    //
    em_betBenMaxTag,
    em_betVMMaxTag,
    em_betVMMinTag,
    em_betBenMinTag,
    em_betBMWMinTag,
    em_betLamMinTag,
    em_reqBankerBtnTag = 1150,
    em_Bet_Clear_Tag,
    em_Bet_Rebet_Tag,
};

enum Em_BetMoneyTag_V99 {
    em_money0Tag = 1200,    //下注筹码表识
    em_money1Tag,
    em_money2Tag,
    em_money3Tag,
    em_money4Tag
};


enum {
    ZOrder_Bg = 100,
    ZOrder_Loop = 150,
    ZOrder_BetButtonNotes = 210,
    ZOrder_BetButtonLabel = 300,
    ZOrder_CarSprite = 310,
    ZOrder_CarLight = 311,
    ZOrder_BetButton = 315,
    ZOrder_MyInfo = 325,
    ZOrder_StateLayer = 330,
    ZOrder_Timer = 335,
    ZOrder_SettlementLayer = 340,
    ZOrder_Tool = 345,
};


//押注按钮Tag 8个值
enum {
    //押注类型按钮
    tag_lam_max = 0,
    tag_bmw_max,
    tag_ben_max,
    tag_vm_max,
    tag_vm_min,
    tag_ben_min,
    tag_bmw_min,
    tag_lam_min,
    //押注金额按钮
    tag_level_1 = 15,
    tag_level_2,
    tag_level_3,
    tag_level_4,
    tag_level_5,
    //取消、续注按钮
    tag_clear = 20,
    tag_rebet,
    //    //走势图
    //    tag_left = 25,
    //    tag_right,
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
//    static const char* BB_BASIC_RESULT_CELL_CSB = "Games/31000501/Node_Basic_Result_Cell.csb";
static const char* BB_BET_OPERATE_BUTTONS_CSB   = "Games/31000501/Node_Bet_Operate_Buttons.csb";
static const char* BB_HISTORY_LABEL_CSB         = "Games/31000501/Node_History_Label.csb";
static const char* BB_ROOT_V99_CSB              = "Games/31000501/Node_Root_V99.csb";
static const char* BB_CAR_LIGHT_CSB             = "v99/animation/BenzBMW/car.csb";

//Resources
//    static const char* Request_Banker_Bg_Res        = "v99/button/yellow/yellow.png";
//    static const char* Request_Banker_Content_Res   = "v99/button/yellow/shangzhuang.png";
//    static const char* Xiazhuang_Content_Res        = "v99/button/yellow/yishenqin.png";
static const char* Bet_Note_Res_V99             = "v99/button/1/%d.png";
//    static const char* Bet_Clear_Rebet_BG_Res_V99   = "v99/button/blue/blue.png";
//    static const char* Bet_Clear_Content_V99        = "v99/button/blue/qingkong.png";
static const char* Bet_Clear_Content_V99        = "v99/button/blue/qk.png";
//    static const char* Bet_Rebet_Content_V99        = "v99/button/blue/xutou.png";
static const char* Bet_Rebet_Content_V99        = "v99/button/blue/xt.png";
static const char* BB_CAR_IMAGE_PATH            = "Games/31000501/BB/car.png";




////////////////////////////////////////////////////////////////////////////
//图片资源
//background

//tool
static const char* BB_TOOL_BAR_PATH			= "v99/cow/exit/%s";
////clock

//music
static const char* BB_BG_MUSIC                  = "Games/31000501/music/BB_BGM.mp3";
static const char* BB_STOP_NOTE_EFFECT          = "Games/31000501/music/bcbm_count.wav";
static const char* BB_START_LOOP_EFFECT         = "Games/31000501/music/bcbm_start.wav";
static const char* BB_MAX_SPEED_LOOP_EFFECT     = "Games/31000501/music/bcbm_loop.wav";
static const char* BB_SLOW_SPEED_LOOP_EFFECT    = "v99/music/BB/bcbm_stop.wav";
static const char* BB_NOTE_COIN_EFFECT          = "v99/music/BB/add_bet.wav";
static const char* AUDIO_BG_FREE_V99            = "v66/FQZS/music/FQZS/back1.mp3";

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
//    this->unschedule(schedule_selector(GameTableUIV99::updateClock));
    this->unschedule(schedule_selector(GameTableUIV99::updateTip));
    this->unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
//    this->unschedule(schedule_selector(GameTableUIV99::updateResultAnimation));
//    this->unschedule(schedule_selector(GameTableUIV99::showGameStateLabelAnimation));
    this->unschedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree));

//    this->unschedule(schedule_selector(GameTableUIV99::autoRemoveNianShouButton));

    //this->unschedule(schedule_selector(GameTableUIV99::sendCardTimer));
//    this->unschedule(schedule_selector(GameTableUIV99::showOpenBtn));
    CC_SAFE_DELETE(_GameTableLogic);

    resetData();
}

bool GameTableUIV99::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }

    resetData();
    _GameTableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate, PLAYER_COUNT);

    return true;
}

void GameTableUIV99::createToolBarUI() {
    //char resStr[128] = {0};

    //sprintf(resStr, BB_TOOL_BAR_PATH, "tool_show_hide.png");
    //auto toobarBtn1 = Button::create(resStr, resStr);
    //toobarBtn1->addClickEventListener([=](Ref*) {
    //    _toolbarBg->setName("1");

    //    if (this->getChildByName("setLayer")) {
    //        (dynamic_cast<BRNNSetLayerV99*>(this->getChildByName("setLayer")))->closeSet();
    //    }
    //    if (this->getChildByName("helpLayer")) {
    //        BRNN::Actions::actionOut(this->getChildByName("helpLayer"));
    //    }

    //    _toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height - 10)));
    //    this->getChildByTag(em_toolbarBtn)->setVisible(false);
    //});
    //toobarBtn1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //toobarBtn1->setPosition(Vec2(10, WIN_SIZE.height - 10));
    //toobarBtn1->setTag(em_toolbarBtn);
    //this->addChild(toobarBtn1, ZOrder_Tool);
    //toobarBtn1->setVisible(true);
    //toobarBtn1->setLocalZOrder(1000);

    //sprintf(resStr, BB_TOOL_BAR_PATH, "tool_bg_three.png");
    //_toolbarBg = Sprite::create(resStr);
    //_toolbarBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    //_toolbarBg->setPosition(10, WIN_SIZE.height + _toolbarBg->getContentSize().height);
    //_toolbarBg->setName("0");//0为在上面 1在下面
    //this->addChild(_toolbarBg, ZOrder_Tool+1);     //whao

    //Size toolbarSize = _toolbarBg->getContentSize();

    //sprintf(resStr, BB_TOOL_BAR_PATH, "tool_setting.png");
    //auto setPic = Sprite::create(resStr);
    //setPic->setPosition(toolbarSize.width / 2, toolbarSize.height * 0.82f);
    //_toolbarBg->addChild(setPic);
    //auto setBtn = ControlButton::create();
    //setBtn->setPreferredSize(setPic->getContentSize());
    //setBtn->setPosition(setPic->getPosition());
    //setBtn->setTag(em_toolbarSetBtn);
    //setBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
    //_toolbarBg->addChild(setBtn);

    //sprintf(resStr, BB_TOOL_BAR_PATH, "tool_help.png");
    //auto helpPic = Sprite::create(resStr);
    //helpPic->setPosition(toolbarSize.width / 2, toolbarSize.height * 0.5f);
    //_toolbarBg->addChild(helpPic);
    //auto helpBtn = ControlButton::create();
    //helpBtn->setPreferredSize(helpPic->getContentSize());
    //helpBtn->setPosition(helpPic->getPosition());
    //helpBtn->setTag(em_toolbarHelpBtn);
    //helpBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
    //_toolbarBg->addChild(helpBtn);

    //sprintf(resStr, BB_TOOL_BAR_PATH, "tool_exit.png");
    //auto leavePic = Sprite::create(resStr);
    //leavePic->setPosition(toolbarSize.width / 2, toolbarSize.height * 0.18f);
    //_toolbarBg->addChild(leavePic);
    //auto leaveBtn = ControlButton::create();
    //leaveBtn->setPreferredSize(leavePic->getContentSize());
    //leaveBtn->setPosition(leavePic->getPosition());
    //leaveBtn->setTag(em_toolbarLeaveBtn);
    //leaveBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
    //_toolbarBg->addChild(leaveBtn);

//    sprintf(resStr, BB_TOOL_BAR_PATH, "tool_next_out.png");
//    auto nextRoundOutPic = Sprite::create(resStr);
//    nextRoundOutPic->setPosition(toolbarSize.width / 2, toolbarSize.height * (40.0f / 313.0f));
//    nextRoundOutPic->setScaleY(1 / 0.8f);
//    _toolbarBg->addChild(nextRoundOutPic);
//    auto nextRoundOutBtn = ControlButton::create();
//    nextRoundOutBtn->setPreferredSize(nextRoundOutPic->getContentSize());
//    nextRoundOutBtn->setPosition(nextRoundOutPic->getPosition());
//    nextRoundOutBtn->setTag(em_toolbarNextOutBtn);
//    nextRoundOutBtn->setUserData((void*)false);
//    nextRoundOutBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV99::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
//    _toolbarBg->addChild(nextRoundOutBtn);

//    sprintf(resStr, BB_TOOL_BAR_PATH, "tool_check.png");
//    auto checkSprite = Sprite::create(resStr);
//    checkSprite->setPosition(Vec2(25, nextRoundOutPic->getContentSize().height / 2 + 6));
//    checkSprite->setName("checkSprite");
//    checkSprite->setVisible(false);
//    nextRoundOutBtn->addChild(checkSprite);
	int zOrder = ZOrder_Tool;
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
					for (int i = 0; i < LoopK_A_BetA_Count; i++) {
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
			auto helpLayer = HN::HelpLayer::create();
			helpLayer->setContentOnView("v66/common/help/BB.png");
			helpLayer->setName("helpLayer");
			helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			helpLayer->setOrignalPosition();
			helpLayer->moveSelfInWithAction();
			addChild(helpLayer, zOrder + 5);
		});
		toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
	}
}

void GameTableUIV99::createRootLayer() {
    auto root = CSLoader::createNode(BB_ROOT_V99_CSB);
    if (root) {
        root->setPosition(WIN_SIZE / 2);
        addChild(root, ZOrder_Loop);
        auto bg = dynamic_cast<Sprite*>(root->getChildByName("Sprite_bg"));
        if (bg) {
            _betArea.tableBg = bg;
        }
        //Loop Area
        auto loopNode = dynamic_cast<Node*>(root->getChildByName("Node_Loop"));
        if (loopNode) {
            _loopArea.loopNode = loopNode;
            char name[32] = {0};
            for (int i = 0; i < Loop_Sprite_Count; i++) {
                sprintf(name, "Image_%d", i);
                auto image = dynamic_cast<ImageView*>(loopNode->getChildByName(name));
                Vec2 pos = Vec2::ZERO;
                if (image) {
                    image->setVisible(false);
                    pos = image->getPosition();
                }
                _loopArea.loopCellPositions.push_back(pos);
                memset(name, 0, sizeof(name));
            }
        }
        //BetArea
        auto betNode = dynamic_cast<Node*>(root->getChildByName("Node_BetArea"));
        if (betNode) {
            _betArea.betNode = betNode;
            char name[32] = {0};
            for (int i = 0; i < LoopK_A_BetA_Count; i++) {
                sprintf(name, "Button_%d", i);
                auto btn = dynamic_cast<Button*>(betNode->getChildByName(name));
                if (btn) {
                    m_betAreaPosition.push_back(btn->getPosition() + betNode->getPosition() + root->getPosition());
                    btn->setTag(i + em_betLamMaxTag);
                    btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::betAreaButtonsCallBack, this));
                    _betArea.betButtons.push_back(btn);
                }
                memset(name, 0, sizeof(name));
            }
        }
        //Banker Area
        Text* bankerName = dynamic_cast<Text*>(root->getChildByName("Text_Zhuang_Name"));
        if (bankerName) {
            bankerName->setString("");
            bankerName->setVisible(false);
            _bankerArea.bankerName = bankerName;
        }
        Text* bankerCoin = dynamic_cast<Text*>(root->getChildByName("Text_Zhuang_Money"));
        if (bankerCoin) {
            bankerCoin->setString("");
            _bankerArea.bankerCoin = bankerCoin;
        }
        Text* reqBankerCount = dynamic_cast<Text*>(root->getChildByName("Text_SZ_Count"));
        if (reqBankerCount) {
            reqBankerCount->setVisible(false);
            _bankerArea.requestBankerCount = reqBankerCount;
        }
        Text* reqBankerNeeds = dynamic_cast<Text*>(root->getChildByName("Text_SZ_Need"));
        if (reqBankerNeeds) {
            reqBankerNeeds->setVisible(false);
            _bankerArea.requestBankerNeeds = reqBankerNeeds;
        }
        Text* needLabel = dynamic_cast<Text*>(root->getChildByName("Text_SZ_Label"));
        if (needLabel) {
            needLabel->setVisible(false);
            _bankerArea.requestBankerNeedsLabel = needLabel;
        }
        Vec2 reqBankerPos = Vec2(-400, 70);
        if (_bankerArea.bankerCoin && _bankerArea.requestBankerCount) {
            reqBankerPos.x = _bankerArea.bankerCoin->getPositionX();
            reqBankerPos.y = (_bankerArea.bankerCoin->getPositionY() - _bankerArea.requestBankerCount->getPositionY()) / 2;
        }
        auto request = dynamic_cast<Button*>(root->getChildByName("Button_Req"));
        if (request) {
            request->setTag(em_reqBankerBtnTag);
            request->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            request->setPosition(reqBankerPos);
            request->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::requestBankerCallBack, this));
            _bankerArea.requestBankerBtn = request;
            setButtonEnabled(request, false);

        }
        auto timerSprite = dynamic_cast<Sprite*>(root->getChildByName("Sprite_Timer"));
        if (timerSprite) {
            timerSprite->setVisible(false);
            m_clockPos = timerSprite->getPosition();
        }
        char name[50];
        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
            memset(name, 0, sizeof(name));
            sprintf(name, "FileNode_%d", i);
            auto node = dynamic_cast<Node*>(root->getChildByName(name));
            if (node) {
                m_betLabelPos.push_back(node->getPosition());
            }
        }
    }
}

//
void GameTableUIV99::createMoneyArea() {
    auto moneyRoot = CSLoader::createNode(BB_BET_OPERATE_BUTTONS_CSB);
    if (moneyRoot) {
        _betArea.moneyNode = moneyRoot;
        _betArea.moneyNode->setPosition(Vec2(WIN_SIZE.width * 0.5f, 10));
        //
        char name[32] = {0};
        for (int i = 0; i < Bet_Coin_Count; i++) {
            sprintf(name, "Bet_%d", i);
            auto btn = dynamic_cast<Button*>(moneyRoot->getChildByName(name));
            if (btn) {
                m_notesPosition.push_back(btn->getPosition() + Vec2(0, btn->getContentSize().height / 2) + moneyRoot->getPosition());
                setButtonEnabled(btn, false);
                btn->setTag(em_money0Tag + i);
                btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::betMoneyCallBack, this));
                _betArea.betMoneyBtn.push_back(btn);
                //
                Sprite* light = dynamic_cast<Sprite*>(btn->getChildByName("Sprite"));
                if (light) {
                    light->setVisible(false);
                    _betArea.betMoneyLight.push_back(light);
                }
            }
            memset(name, 0, sizeof(name));
        }
        addChild(_betArea.moneyNode, ZOrder_BetButton);
    }
    //清空、续投
    AUIButton* renoteBtn = AUIButton::create(Bet_Rebet_Content_V99);
    if (renoteBtn) {
        _betArea.reBetBtn = renoteBtn;
        renoteBtn->setTag(em_Bet_Rebet_Tag);
//            renoteBtn->setTitle(Bet_Rebet_Content_V99);
        renoteBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
//            renoteBtn->setTitleOffset(Vec2(-4, 4));
        Size size = renoteBtn->getContentSize();
        Vec2 pos = Vec2(WIN_SIZE.width * 0.5f + 250 + size.width / 2 + 10, 10 + 10 + size.height * 0.8f);
        renoteBtn->setPosition(pos);
        renoteBtn->setScale(0.8f);
        setButtonEnabled(renoteBtn, false);
        renoteBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::betRebetCallBack, this));
        addChild(_betArea.reBetBtn, ZOrder_BetButton);
    }
    AUIButton* clearBtn = AUIButton::create(Bet_Clear_Content_V99);
    if (clearBtn) {
        _betArea.clearBtn = clearBtn;
        clearBtn->setTag(em_Bet_Clear_Tag);
//            clearBtn->setTitle(Bet_Clear_Content_V99);
        clearBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
//            clearBtn->setTitleOffset(Vec2(-4, 4));
        Size size = clearBtn->getContentSize();
        Vec2 pos = Vec2(WIN_SIZE.width * 0.5f + 250 + size.width / 2 + 10, 10);
        clearBtn->setPosition(pos);
        clearBtn->setScale(0.8f);
        setButtonEnabled(clearBtn, false);
        clearBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::betClearCallBack, this));
        addChild(_betArea.clearBtn, ZOrder_BetButton);
    }
}

void GameTableUIV99::createNoteLabels() {
    if (_betArea.noteLabels.empty()) {
        //需要初始化
        ostringstream ss;
        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
            if (_betArea.betButtons.at(i)) {
//                    auto pos = m_betAreaPosition.at(i);
//                    pos.y = (i < 4 ? WIN_SIZE.height * 0.6 : WIN_SIZE.height * 0.4);
//                    if (i == 0 || i == 7) {
//                        pos.x -= 20;
//                    }
//                    if (i == 3 || i == 4) {
//                        pos.x += 10;
//                    }
                Vec2 pos = m_betLabelPos.at(i);
                pos += WIN_SIZE / 2;
                BBNoteLabelCellV99* cell = BBNoteLabelCellV99::create();
                cell->setNotePosition(pos);
                cell->clearNotes();
                _betArea.noteLabels.push_back(cell);
                addChild(cell, ZOrder_BetButtonLabel);
            }
        }
    }
}

void GameTableUIV99::createCarLayer() {
    ImageView* car = ImageView::create(BB_CAR_IMAGE_PATH);
    if (car) {
        _car.cardImage = car;
        car->setPosition(WIN_SIZE / 2);
        addChild(car, ZOrder_CarSprite);
    }
    auto node = CSLoader::createNode(BB_CAR_LIGHT_CSB);
    if (node) {
        _car.carNode = node;
        node->setPosition(WIN_SIZE / 2);
        node->setVisible(false);
        addChild(node, ZOrder_CarLight);
    }
    auto action = CSLoader::createTimeline(BB_CAR_LIGHT_CSB);
    if (action) {
        _car.carAction = action;
//            action->play("car", true);
        if (_car.carNode) {
            _car.carNode->runAction(_car.carAction);
        }
    }
}

void GameTableUIV99::createPlayerInfon() {
    _playerHead = nullptr;
    BBPlayerHeadV99* head = BBPlayerHeadV99::create();
    if (head) {
        _playerHead = head;
        _playerHead->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _playerHead->setHeadPosition(Vec2(10.0f, 10.0f));
        addChild(_playerHead, ZOrder_MyInfo);
    }
}

void GameTableUIV99::createGameStateLayer() {
    _gameState = nullptr;
    BBGameStateV99* state = BBGameStateV99::create();
    if (state) {
        _gameState = state;
        Vec2 pos = Vec2(WIN_SIZE.width - 88, 0);
        if (_openRecord.node) {
            Size nodeSize = Size::ZERO;
            auto bg = dynamic_cast<ImageView*>(_openRecord.node->getChildByName("Image_Bg"));
            if (bg) {
                nodeSize = bg->getContentSize();
            } else {
                nodeSize = Size(68, 529);
            }
            pos.x = _openRecord.node->getPositionX() - 10 - nodeSize.width;
        }
        _gameState->setStateGirlPosition(pos);
        _gameState->setStateLabelPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.75f));
        _gameState->hideStateLayer();
        addChild(_gameState, ZOrder_StateLayer);
    }
}

void GameTableUIV99::createGameSettlementLayer() {
    _gameSettlement = nullptr;
    BBSettlementV99* settle = BBSettlementV99::create();
    if (settle) {
        _gameSettlement = settle;
        _gameSettlement->setSettlementLayerPosition(Vec2(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.4f));
        _gameSettlement->hideSettlementLayer();
        addChild(_gameSettlement, ZOrder_SettlementLayer);
    }
}

//游戏历史开局情况
void GameTableUIV99::createState() {
    if (!_openRecord.node) {
        _openRecord.node = CSLoader::createNode(BB_HISTORY_LABEL_CSB);
        if (_openRecord.node) {
            _openRecord.node->setPosition(Vec2(WIN_SIZE.width - 10.0f, WIN_SIZE.height / 2));
            addChild(_openRecord.node, ZOrder_CarLight + 1);

            Vec2 posStart = _openRecord.node->getPosition();
            Size nodeSize = Size::ZERO;
            auto bg = dynamic_cast<ImageView*>(_openRecord.node->getChildByName("Image_Bg"));
            if (bg) {
                nodeSize = bg->getContentSize();

            } else {
                nodeSize = Size(68, 529);
            }
            posStart.y -= nodeSize.height / 2;


            float height = 76.0f;	//精灵高
            float space = 0.0f;		//精灵间距
            float scaleRate = 0.7f;

            for (int i = 0; i < Open_History_Logo_Count; i++) {
                auto cell = BBLoopSpriteCellV99::create();
                if (cell) {
                    cell->setCellScale(scaleRate);
                    height = cell->getContentImageSize().height;
                    height = height * scaleRate;
                    space = (nodeSize.height - height * 9.5) / 10.0f;
                    space = space > 0 ? space : 0;
                    if (!m_openRecordAnimal.empty()) {
                        cell->setCellIndex(m_openRecordAnimal.at(i));
                    } else {
                        cell->showLoopCellNode(false);
//                            cell->setCellIndex(i);
                    }
                    Vec2 cellPos = Vec2(-nodeSize.width / 2, -nodeSize.height / 2 + space * i + height * (i + 0.5));
                    cell->showLightEffect(false);
                    cell->setCellPosition(cellPos);
                    _openRecord.openLogos.push_back(cell);
                    _openRecord.openLogosPositions.push_back(cellPos);

                    _openRecord.node->addChild(_openRecord.openLogos.at(i));

                }

            }
            if (_openRecord.openLogos.back()) {
                _openRecord.openLogos.back()->showLightEffect(true);
            }
        }
    }
}

void GameTableUIV99::createClock() {
    m_clockObj = TimerObject::create();
    if (m_clockObj) {
        m_clockPos += WIN_SIZE / 2;
        m_clockObj->setPosition(m_clockPos);
        m_clockObj->setScale(0.9f);
        m_clockObj->setVisible(false);
        this->addChild(m_clockObj, ZOrder_Timer);
    }
}

void GameTableUIV99::addLoopSprites() {
    if (_loopArea.loopCellPositions.empty()) {
        return;
    }
    for (int i = 0; i < _loopArea.loopCellPositions.size(); i++) {
        Vec2 pos = _loopArea.loopCellPositions.at(i);

        BBLoopSpriteCellV99* cell = BBLoopSpriteCellV99::create();
        if (cell) {
            cell->setCellPosition(pos + _loopArea.loopNode->getPosition() + WIN_SIZE / 2);
            cell->setCellIndex(i);
//                cell->setIconIndex(i % Loop_Kind_Count);
            addChild(cell, ZOrder_Loop);
            _loopArea.loopSprites.push_back(cell);
        }
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

//void GameTableUIV99::createClock() {
//    m_ClockBg = ImageView::create(FQZS_CLOCK_BG);
//    addChild(m_ClockBg);
//    if (_resultLoop.loopNode) {
//        m_ClockBg->setPosition(Vec2(_resultLoop.loopNode->getPositionX(), _resultLoop.loopNode->getPositionY() + 120.0f));
//    } else {
//        m_ClockBg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 + 110.0f));
//    }
//
//    m_ClockTimeLabel = LabelAtlas::create("00", FQZS_CLOCK_NUM, 25, 34, '0');
//    m_ClockBg->addChild(m_ClockTimeLabel);
//    m_ClockTimeLabel->setVisible(false);
//    m_ClockTimeLabel->setAnchorPoint(Vec2(0.5, 0.5));
//    m_ClockTimeLabel->setPosition(m_ClockBg->getContentSize() / 2);
//}
//
//void GameTableUIV99::createReadyUI() {
//    ////准备按钮
//    //m_PrepareBtn = Button::create(NN_PREPARE_BTN);
//    //addChild(m_PrepareBtn);
//    //m_PrepareBtn->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - 110));
//    //m_PrepareBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::prepareClickCallback, this));
//
//    //Size readyBtnSize = m_PrepareBtn->getContentSize();
//    //auto readyBtnImage = ImageView::create(NN_PREPARE_WORD);
//    //readyBtnImage->setPosition(Vec2(readyBtnSize.width / 2, readyBtnSize.height / 2 + 4));
//    //m_PrepareBtn->addChild(readyBtnImage);
//
//    //for (int i = 0; i < PLAYER_COUNT; i++) {
////       auto readyImage = ImageView::create(NN_PREPARE_IMAGE);
////       addChild(readyImage);
//    //	readyImage->setAnchorPoint(getStateTipAnchor(i));
////       readyImage->setPosition(getStateTipPos(i));
////       readyImage->setVisible(false);
//
////       m_vPrepareImage.push_back(readyImage);
////   }
//
//    //m_vOkImage.resize(PLAYER_COUNT);
//    //for (int i = 0; i < PLAYER_COUNT; i++) {
////       auto okImage = ImageView::create(NN_OK_IMAGE);
////       addChild(okImage, 55);
////       int iViewID = _GameTableLogic->logicToViewSeatNo(i);
//    //	okImage->setPosition(getCardPosVector(PLAYER_COUNT).at(iViewID).at(2));
////       okImage->setVisible(false);
//
////       m_vOkImage.at(iViewID) = okImage;
////   }
//
//    ////叫庄按钮
//    //m_CatchBankerBtn = Button::create(NN_CATCH_BTN_BG);
//    //addChild(m_CatchBankerBtn);
//    //m_CatchBankerBtn->setPosition(Vec2(WIN_SIZE.width / 2 - 80, WIN_SIZE.height / 2 - 100));
//    //m_CatchBankerBtn->setTag(1);//1:叫
//    //m_CatchBankerBtn->setVisible(false);
//    //m_CatchBankerBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::catchBankerClickCallback, this));
//
//    //Size catchBtnSize = m_CatchBankerBtn->getContentSize();
//    //auto catchImage = ImageView::create(NN_CATCH_WORD);
//    //catchImage->setPosition(Vec2(catchBtnSize.width / 2, catchBtnSize.height / 2));
//    //m_CatchBankerBtn->addChild(catchImage);
//
//    ////不叫按钮
//    //m_NotCatchBtn = Button::create(NN_CATCH_BTN_BG);
//    //addChild(m_NotCatchBtn);
//    //m_NotCatchBtn->setPosition(Vec2(WIN_SIZE.width / 2 + 80, WIN_SIZE.height / 2 - 100));
//    //m_NotCatchBtn->setTag(0);//0:不叫
//    //m_NotCatchBtn->setVisible(false);
//    //m_NotCatchBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::catchBankerClickCallback, this));
//
//    //Size giveupBtnSize = m_NotCatchBtn->getContentSize();
//    //auto giveupImage = ImageView::create(NN_GIVE_UP_WORD);
//    //giveupImage->setPosition(Vec2(giveupBtnSize.width / 2, giveupBtnSize.height / 2));
//    //m_NotCatchBtn->addChild(giveupImage);
//
//
////   //阶段标志
////   m_StageImage = ImageView::create(NN_STATE_IMAGE_CATCH);
////   addChild(m_StageImage);
//    //m_StageImage->setAnchorPoint(getStateTipAnchor(0));
//    //m_StageImage->setPosition(getStateTipPos(0));
////   m_StageImage->setVisible(false);
//
//    //广播通知
//// 		_NoticeBG = Sprite::create(Notice_Bg);
//// 		_NoticeBG->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.75f));
//// 		_NoticeBG->setCascadeOpacityEnabled(true);
//// 		this->addChild(_NoticeBG, NOTICE_ZORDER);
//// 		_NoticeBG->setVisible(false);
////
//// 		_notic = GameNotice::create();
//// 		_notic->setPosition(Vec2(_NoticeBG->getContentSize() / 2));
//// 		_notic->setCallBack([this](){
//// 			_NoticeBG->setVisible(false);
//// 		});
//// 		_NoticeBG->addChild(_notic);
//}

////下注操作区域
//void GameTableUIV99::createBetArea() {
//    ostringstream ss;
//    if (!_betArea.areaNode) {
//        _betArea.areaNode = CSLoader::createNode(FQZS_BET_AREA_CSB);
//        _betArea.areaNode->setPosition(Vec2(WIN_SIZE.width / 2, 0));
//        addChild(_betArea.areaNode);
//    }
//    //取消、续投区域
//    if (!_betArea.clearResetNode) {
//        _betArea.clearResetNode = CSLoader::createNode(FQZS_CLEAR_RESET_AREA_CSB);
//        if (_betArea.clearResetNode) {
//            Vec2 pos = Vec2(WIN_SIZE.width - 20, WIN_SIZE.height - 201);
//
//            if (_goldPool.poolNode) {
//                Sprite* bg = dynamic_cast<Sprite*>(_goldPool.poolNode->getChildByName("GoldPool"));
//                if (bg) {
//                    pos = Vec2(WIN_SIZE.width - 20, WIN_SIZE.height - bg->getContentSize().height);
//                }
//            }
//
//            _betArea.clearResetNode->setPosition(pos);
//            addChild(_betArea.clearResetNode);
//            //
//            if (!_betArea.clearBtn) {
//                _betArea.clearBtn = dynamic_cast<Button*>(_betArea.clearResetNode->getChildByName("Button_clear"));
//                if (_betArea.clearBtn) {
//                    _betArea.clearBtn->setTag(tag_clear);
//                    _betArea.clearBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::clearRebetCallBack, this));
//                }
//            }
//            if (!_betArea.resetBtn) {
//                _betArea.resetBtn = dynamic_cast<Button*>(_betArea.clearResetNode->getChildByName("Button_rebet"));
//                if (_betArea.resetBtn) {
//                    _betArea.resetBtn->setTag(tag_rebet);
//                    _betArea.resetBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::clearRebetCallBack, this));
//                }
//            }
//        }
//
//    }
//    if (!_betArea.myGoldSprite) {
//        _betArea.myGoldSprite = dynamic_cast<Sprite*>(_betArea.areaNode->getChildByName("MyCoin"));
//        if (_betArea.myGoldSprite) {
//            if (!_betArea.myGold) {
//                _betArea.myGold = dynamic_cast<Text*>(_betArea.myGoldSprite->getChildByName("my"));
//                ss << Tools::addComma(0);
//                _betArea.myGold->setString(ss.str().c_str());
//                ss.str("");
//            }
//        }
//    }
//    if (!_betArea.myWinSprite) {
//        _betArea.myWinSprite = dynamic_cast<Sprite*>(_betArea.areaNode->getChildByName("MyWin"));
//        if (_betArea.myWinSprite) {
//            if (!_betArea.myWin) {
//                _betArea.myWin = dynamic_cast<Text*>(_betArea.myWinSprite->getChildByName("my"));
//                ss << Tools::addComma(0);
//                _betArea.myWin->setString(ss.str().c_str());
//                ss.str("");
//            }
//        }
//    }
//    //下注的按钮
//    if (_betArea.betButtons.empty()) {
//        //需要初始化
//        for (int i = 0; i < Bet_Area_Num; i++) {
//            Button* btn = dynamic_cast<Button*>(_betArea.areaNode->getChildByName(Bet_Button_Names[i]));
//            if (btn) {
//                btn->setTag(Bet_Button_Tag[i]);
//                btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::betAreaCallBack, this));
//                _betArea.betButtons.push_back(btn);
//            }
//        }
//    }
//    if (_betArea.btnLights.empty()) {
//        //需要初始化
//        for (int i = 0; i < Bet_Area_Num; i++) {
//            if (_betArea.betButtons.at(i)) {
//                Sprite* light = dynamic_cast<Sprite*>(_betArea.betButtons.at(i)->getChildByName("light"));
//                if (light) {
//                    light->setVisible(false);
//                    light->setTag(Bet_Button_Tag[i]);	//按钮的光效与按钮本身使用同一个tag值
//                    _betArea.btnLights.push_back(light);
//                }
//            }
//        }
//    }
//    if (_betArea.myCoinText.empty()) {
//        //需要初始化
//        for (int i = 0; i < Bet_Area_Num; i++) {
//            if (_betArea.betButtons.at(i)) {
//                Text* my = dynamic_cast<Text*>(_betArea.betButtons.at(i)->getChildByName("my"));
//                if (my) {
//                    ss << 0;
//                    my->setString(ss.str().c_str());
//                    ss.str("");
//                    _betArea.myCoinText.push_back(my);
//                }
//            }
//        }
//    }
//    if (_betArea.allCoinText.empty()) {
//        //需要初始化
//        for (int i = 0; i < Bet_Area_Num; i++) {
//            if (_betArea.betButtons.at(i)) {
//                Text* all = dynamic_cast<Text*>(_betArea.betButtons.at(i)->getChildByName("all"));
//                if (all) {
//                    ss << 0;
//                    all->setString(ss.str().c_str());
//                    ss.str("");
//                    _betArea.allCoinText.push_back(all);
//                }
//            }
//        }
//    }
//    //
//    initBetButtonsPosition();
//    //进入游戏后，非下注阶段下注按钮禁止响应
//    controlBetButtonEnable(false);
//}
//
////动态设置押注按钮位置
//void GameTableUIV99::initBetButtonsPosition() {
//    const float edgeOff = 10.0f;	//左右边距
//    const float upOff = 5.0f;		//距离底框距离
//    float space = 0.0f;				//水平方向间距
//    const float upSpace = 10;		//垂直方向控件间距
//    Size aSize = Size::ZERO;//动物
//    Size kSize = Size::ZERO;//类型
//    Size sSize = Size::ZERO;//鲨鱼
//    if (_betArea.myWinSprite) {
//        _betArea.myWinSprite->setPosition(Vec2(-(WIN_SIZE.width / 2 - edgeOff), upOff));
//        //if (_betArea.clearBtn)
//        //{
//        //	_betArea.clearBtn->setPosition(Vec2(-_betArea.clearBtn->getContentSize().width,
//        //		_betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height / 2));
//        //}
//    }
//    if (_betArea.myGoldSprite) {
//        _betArea.myGoldSprite->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff, upOff));
//        //if (_betArea.resetBtn)
//        //{
//        //	_betArea.resetBtn->setPosition(Vec2(_betArea.resetBtn->getContentSize().width,
//        //		_betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height / 2));
//        //}
//    }
//    if (!_betArea.betButtons.empty()) {
//        for (int i = 0; i < Bet_Area_Num; i++) {
//            if (_betArea.betButtons.at(i)) {
//                if (_betArea.betButtons.at(i)->getTag() == tag_sy) {
//                    sSize = _betArea.betButtons.at(i)->getContentSize();
//                    m_betAreaKindSize[1] = sSize;
//                    continue;
//                }
//                if (_betArea.betButtons.at(i)->getTag() == tag_fq || _betArea.betButtons.at(i)->getTag() == tag_zs) {
//                    kSize = _betArea.betButtons.at(i)->getContentSize();
//                    m_betAreaKindSize[0] = kSize;
//                    continue;
//                }
//                aSize = _betArea.betButtons.at(i)->getContentSize();
//            }
//        }
//        //计算水平方向间距
//        space = (WIN_SIZE.width - edgeOff * 2 - aSize.width * 8 - sSize.width) / 8;
//        //
//        for (int i = 0; i < Bet_Area_Num; i++) {
//            if (_betArea.betButtons.at(i)) {
//                switch (_betArea.betButtons.at(i)->getTag()) {
//                case tag_fq://飞禽
//                    if (_betArea.myWinSprite) {
//                        m_betAreaPosition[0] = Vec2(-(WIN_SIZE.width / 2 - edgeOff),
//                                                    _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + aSize.height + upSpace);
//                        _betArea.betButtons.at(i)->setPosition(m_betAreaPosition.at(0));
//                    }
//                    break;
//                case tag_yz://燕子
//                    if (_betArea.myWinSprite) {
//                        _betArea.betButtons.at(i)->setPosition(Vec2(-(WIN_SIZE.width / 2 - edgeOff),
//                                                               _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace));
//                    }
//                    break;
//                case tag_gz://鸽子
//                    if (_betArea.myWinSprite) {
//                        _betArea.betButtons.at(i)->setPosition(Vec2(-(WIN_SIZE.width / 2 - aSize.width * 1.5f - space - edgeOff),
//                                                               _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace));
//                    }
//                    break;
//                case tag_kq://孔雀
//                    if (_betArea.myWinSprite) {
//                        _betArea.betButtons.at(i)->setPosition(Vec2(-(WIN_SIZE.width / 2 - aSize.width * 2.5f - 2 * space - edgeOff),
//                                                               _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace));
//                    }
//                    break;
//                case tag_ly://老鹰
//                    if (_betArea.myWinSprite) {
//                        _betArea.betButtons.at(i)->setPosition(Vec2(-(WIN_SIZE.width / 2 - aSize.width * 3.5f - 3 * space - edgeOff),
//                                                               _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace));
//                    }
//                    break;
//                case tag_zs://走兽
//                    if (_betArea.myGoldSprite) {
//                        m_betAreaPosition[1] = Vec2(WIN_SIZE.width / 2 - edgeOff,
//                                                    _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + aSize.height + upSpace);
//                        _betArea.betButtons.at(i)->setPosition(m_betAreaPosition.at(1));
//                    }
//                    break;
//                case tag_sz://狮子
//                    if (_betArea.myGoldSprite) {
//                        _betArea.betButtons.at(i)->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff - aSize.width * 3.5f - 3 * space,
//                                                               _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + upSpace));
//                    }
//                    break;
//                case tag_xm://熊猫
//                    if (_betArea.myGoldSprite) {
//                        _betArea.betButtons.at(i)->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff - aSize.width * 2.5f - 2 * space,
//                                                               _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + upSpace));
//                    }
//                    break;
//                case tag_hz://猴子
//                    if (_betArea.myGoldSprite) {
//                        _betArea.betButtons.at(i)->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff - aSize.width * 1.5f - space,
//                                                               _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + upSpace));
//                    }
//                    break;
//                case tag_tz://兔子
//                    if (_betArea.myGoldSprite) {
//                        _betArea.betButtons.at(i)->setPosition(Vec2(WIN_SIZE.width / 2 - edgeOff,
//                                                               _betArea.myGoldSprite->getPositionY() + _betArea.myGoldSprite->getContentSize().height + upSpace));
//                    }
//                    break;
//                case tag_sy://鲨鱼
//                    if (_betArea.myWinSprite) {
//                        m_betAreaPosition[2] = Vec2(0,
//                                                    _betArea.myWinSprite->getPositionY() + _betArea.myWinSprite->getContentSize().height + upSpace);
//                        _betArea.betButtons.at(i)->setPosition(m_betAreaPosition.at(2));
//                    }
//                    break;
//                default:
//                    break;
//                }
//            }
//        }
//    }
//}
//
////选择下注金额操作界面
//void GameTableUIV99::createBetButtons() {
//    if (!_betButton.node) {
//        ostringstream ss;
//        _betButton.node = CSLoader::createNode(FQZS_BET_BUTTONS_CSB);
//        if (_betButton.node) {
//            Vec2 pos = Vec2(WIN_SIZE.width / 2, 30 + 5);
//            Sprite* bg = dynamic_cast<Sprite*>(_betButton.node->getChildByName("bg"));
//            if (bg) {
//                pos = Vec2(WIN_SIZE.width / 2, bg->getContentSize().height / 2 + 5);
//            }
//            _betButton.node->setPosition(pos);
//            addChild(_betButton.node);
//            if (_betButton.buttons.empty()) {
//                for (int i = 0; i < Bet_Count; i++) {
//                    ss << "B_" << (i + 1);
//                    Button* btn = dynamic_cast<Button*>(_betButton.node->getChildByName(ss.str().c_str()));
//                    if (btn) {
//                        btn->setTag(Bet_Money_Tag[i]);
//                        btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV99::betButtonsCallBack, this));
//                        _betButton.buttons.push_back(btn);
//                    }
//                    ss.str("");
//                }
//            }
//            if (_betButton.btnLights.empty()) {
//                for (int i = 0; i < Bet_Count; i++) {
//                    if (!_betButton.buttons.empty() && i < _betButton.buttons.size()) {
//                        if (_betButton.buttons.at(i)) {
//                            Sprite* light = dynamic_cast<Sprite*>(_betButton.buttons.at(i)->getChildByName("light"));
//                            if (light) {
//                                light->setTag(Bet_Money_Tag[i]);
//                                light->setVisible(false);
//                                _betButton.btnLights.push_back(light);
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//}

////彩金池
//void GameTableUIV99::createPool() {
//    if (!_goldPool.poolNode) {
//        _goldPool.poolNode = CSLoader::createNode(FQZS_POOL_CSB);
//        if (_goldPool.poolNode) {
//            _goldPool.poolNode->setPosition(Vec2(WIN_SIZE.width, WIN_SIZE.height));
//            addChild(_goldPool.poolNode);
//            if (!_goldPool.poolMoney) {
//                _goldPool.poolMoney = dynamic_cast<Text*>(_goldPool.poolNode->getChildByName("gold"));
//                ostringstream ss;
//                ss << Tools::addComma(_goldPool.i64Money);
//                _goldPool.poolMoney->setString(ss.str().c_str());
//                ss.str("");
//            }
//        }
//    }
//}

////结果展示
//void GameTableUIV99::createResult() {
//    if (!_resultLabel.labelNode) {
//        _resultLabel.labelNode = CSLoader::createNode(FQZS_RESULT_LABEL_CSB);
//        if (_resultLabel.labelNode) {
//            Vec2 pos = WIN_SIZE / 2;
//            if (_resultLoop.loopNode) {
//                pos.x = _resultLoop.loopNode->getPositionX();
//                pos.y = _resultLoop.loopNode->getPositionY() - _betArea.myGoldSprite->getContentSize().height / 2 - 85.0f;
//            }
//            _resultLabel.labelNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
//            _resultLabel.labelNode->setPosition(pos);
//            addChild(_resultLabel.labelNode);
//            //
//            if (!_resultLabel.font) {
//                _resultLabel.font = dynamic_cast<TextBMFont*>(_resultLabel.labelNode->getChildByName("Result"));
//                if (_resultLabel.font) {
//                    ostringstream ss;
//                    ss << "+" << Tools::addComma(0);
//                    _resultLabel.font->setString(ss.str().c_str());
//                    ss.str("");
//                }
//            }
//            _resultLabel.labelNode->setVisible(false);
//        }
//    }
//}
//
////动物椭圆区域
//void GameTableUIV99::createResultLoop() {
//    char ss[20];
//    if (!_resultLoop.loopNode) {
//        _resultLoop.loopNode = CSLoader::createNode(FQZS_RESULT_LOOP_CSB);
//        if (_resultLoop.loopNode) {
//            _resultLoop.loopNode->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height - 10 - 200 - 40));
//            addChild(_resultLoop.loopNode);
//        }
//    }
//    if (_resultLoop.animals.empty()) {
//        for (int i = 0; i < Open_Area; i++) {
//            memset(ss, 0, sizeof(ss));
//            sprintf(ss, "A_%02d", (i + 1));
//            Sprite* ani = dynamic_cast<Sprite*>(_resultLoop.loopNode->getChildByName(ss));
//            if (ani) {
//                Vec2 pos = ani->getPosition();
//                if (_resultLoop.loopNode) {
//                    pos.x += _resultLoop.loopNode->getPositionX();
//                    pos.y += _resultLoop.loopNode->getPositionY();
//                }
//                _resultLoop.animals.push_back(ani);
//                m_loopAnimalPosition.push_back(pos);
//            }
//        }
//        //
//        if (!m_loopAnimalPosition.empty()) {
//            for (int i = 2; i >= 0; i--) {
//                Sprite* sprite = Sprite::create(FQZS_LOOP_LIGHT_RES);
//                sprite->setPosition(m_loopAnimalPosition.at(i));
//                sprite->setTag(i);
//                sprite->setOpacity(80 * i + 75);
//                sprite->setVisible(false);
//                m_loopAnimationSprite.at(i) = sprite;
//                addChild(m_loopAnimationSprite.at(i));
//            }
//        }
//    }
//
//}
//
////游戏状态label
//void GameTableUIV99::createGameState() {
//    if (!m_gameStateNode) {
//        unschedule(schedule_selector(GameTableUIV99::showGameStateLabelAnimation));
//        m_gameStateNode = CSLoader::createNode(FQZS_STATE_LABEL_CSB);
//        if (m_gameStateNode) {
//            Vec2 pos = WIN_SIZE / 2;
//            if (_resultLoop.loopNode) {
//                pos = _resultLoop.loopNode->getPosition();
//                pos.y += 50;
//            }
//            m_gameStateNode->setPosition(pos);
//            this->addChild(m_gameStateNode);
//            if (!m_gameStateLabel) {
//                unschedule(schedule_selector(GameTableUIV99::showGameStateLabelAnimation));
//                m_gameStateLabel = dynamic_cast<Text*>(m_gameStateNode->getChildByName("Text"));
//                if (m_gameStateLabel) {
//                    ostringstream ss;
//                    ss << GetText("FQZS_FREE_VALUE");//空闲
//                    m_gameStateLabel->setString(ss.str());
//                    ss.str("");
//                    //
//                    schedule(schedule_selector(GameTableUIV99::showGameStateLabelAnimation), 1.0f);
//                }
//            }
//        }
//    }
//}

void GameTableUIV99::leaveTheGame() {
    HNRoomLogic::getInstance()->closeRoom();
    PlatformState* state = PlatformState::create();
    state->setType(ROOMLIST);
    StateManager::getInstance()->setState(state);
}

//第一次进入游戏基础数据同步
void GameTableUIV99::syncBasicDataFirstTime(BB::SyncDataStruct *msg) {
    if (msg == nullptr) {
        return;
    }
    m_GameStatus = msg->gameState;
    if (_playerHead) {
        auto user = _GameTableLogic->getUserInfoBySeatNo(msg->bDeskStation);
        if (user) {
            _playerHead->setPlayerIcon(user->bLogoID);
            _playerHead->setPlayerName(user->nickName);
            _playerHead->setPlayerMoney(msg->i64MyMoney);
        }
    }
    m_i64SingleNoteLimit = msg->i64SingleNoteLimit;
    m_i64BankerNeedMoney = msg->requestBankerNeedMoney;
    memcpy(m_iMultiplying, msg->imultiplying, sizeof(m_iMultiplying));
    updateStateImage(msg->iHistory);
    //banker info
    if (msg->bCurrentBankerID > 0) {
        if (_bankerArea.requestBankerBtn) {
            _bankerArea.requestBankerBtn->setVisible(true);
            setButtonEnabled(_bankerArea.requestBankerBtn, msg->i64MyMoney >= m_i64BankerNeedMoney);
        }

        memset(m_iBankerNickName, 0, sizeof(m_iBankerNickName));
        memcpy(m_iBankerNickName, msg->bCurrentBankerNickName, sizeof(m_iBankerNickName));
        if (_bankerArea.bankerName) {
            _bankerArea.bankerName->setString(ForceGBKToUtf8(msg->bCurrentBankerNickName));
        }
        if (_bankerArea.bankerCoin) {
            _bankerArea.bankerCoin->setString(Tools::addComma(msg->iCurrentBankerMoney));
        }
    }

    char name[100];
    if (_bankerArea.requestBankerCount) {
        _bankerArea.requestBankerCount->setVisible(true);
        memset(name, 0, sizeof(name));
        sprintf(name, GetText("BB_ZHUANG_LIST_COUNT"), msg->requestBankerListCount);
        _bankerArea.requestBankerCount->setString(name);
    }
    if (_bankerArea.requestBankerNeedsLabel) {
        memset(name, 0, sizeof(name));
        _bankerArea.requestBankerNeedsLabel->setVisible(true);
        sprintf(name, "%s", GetText("BB_ZHUANG_NEED"));
        _bankerArea.requestBankerNeedsLabel->setString(name);
        memset(name, 0, sizeof(name));
    }
    if (_bankerArea.requestBankerNeeds) {
        _bankerArea.requestBankerNeeds->setVisible(true);
        _bankerArea.requestBankerNeeds->setString(Tools::addComma(msg->requestBankerNeedMoney));
    }
}

////进入或者短线重连恢复游戏桌子场景
void GameTableUIV99::resumeTheGameFree(GameStation_Base* pcene, BYTE gameStation) {
    if (pcene == nullptr) {
        return;
    }
    m_GameStatus = pcene->bGameStation;
    //banker info
    if (pcene->bCurrentBankerID > 0) {
        if (_bankerArea.requestBankerBtn) {
//            _bankerArea.requestBankerBtn->setVisible(true);
            setButtonEnabled(_bankerArea.requestBankerBtn, pcene->i64MyMoney >= m_i64BankerNeedMoney);
        }

        memset(m_iBankerNickName, 0, sizeof(m_iBankerNickName));
        memcpy(m_iBankerNickName, pcene->iBankerNickName, sizeof(m_iBankerNickName));
        if (_bankerArea.bankerName) {
            _bankerArea.bankerName->setVisible(true);
            _bankerArea.bankerName->setString(ForceGBKToUtf8(pcene->iBankerNickName));
        }
        if (_bankerArea.bankerCoin) {
            _bankerArea.bankerCoin->setString(Tools::addComma(pcene->iBankerMoney));
        }
    }
//    updateGameStateLabelContent();
//    controlBetButtonEnable(m_GameStatus == GS_XIAZHU);
//    m_i64SingleNoteLimit = pcene->i64SingleNoteLimit;
//    memcpy(m_iMultiplying, pcene->imultiplying, sizeof(pcene->imultiplying));
//    auto leftTime = pcene->itime;
//    memcpy(m_iHistory, pcene->iHistory, sizeof(pcene->iHistory));
//
//    //刷新身上金额
//    updateGoldLabel(_GameTableLogic->getMyMoney());
//
//    //刷新纪录
//    updateStateImage(m_iHistory);
//
//    m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_FREE, pcene->iCurrentStateLeftTimes);
}

void GameTableUIV99::resumeTheGameBet(GameStation_Bet* pcene, BYTE gameStation) {
    if (pcene == nullptr) {
        return;
    }
    m_GameStatus = pcene->bGameStation;
    //banker info
    if (pcene->bCurrentBankerID > 0) {
        if (_bankerArea.requestBankerBtn) {
//            _bankerArea.requestBankerBtn->setVisible(true);
            setButtonEnabled(_bankerArea.requestBankerBtn, pcene->i64MyMoney >= m_i64BankerNeedMoney);
        }

        memset(m_iBankerNickName, 0, sizeof(m_iBankerNickName));
        memcpy(m_iBankerNickName, pcene->iBankerNickName, sizeof(m_iBankerNickName));
        if (_bankerArea.bankerName) {
            _bankerArea.bankerName->setVisible(true);
            _bankerArea.bankerName->setString(ForceGBKToUtf8(pcene->iBankerNickName));
        }
        if (_bankerArea.bankerCoin) {
            _bankerArea.bankerCoin->setString(Tools::addComma(pcene->iBankerMoney));
        }
    }
//    updateGameStateLabelContent();
//    controlBetButtonEnable(m_GameStatus == GS_XIAZHU);
////    playBetMusic();
//    m_i64SingleNoteLimit = pcene->i64SingleNoteLimit;
//    memcpy(m_iMultiplying, pcene->imultiplying, sizeof(pcene->imultiplying));
//    auto leftTime = pcene->itime;
//    memcpy(m_iHistory, pcene->iHistory, sizeof(pcene->iHistory));
//    memcpy(m_i64OwnNotes, pcene->i64OwnNotes, sizeof(pcene->i64OwnNotes));
//    memcpy(m_i64AreaNotes, pcene->i64AreaNotes, sizeof(pcene->i64AreaNotes));
//
//    //刷新身上金额
//    updateGoldLabel(_GameTableLogic->getMyMoney());
//
//    //刷新纪录
//    updateStateImage(m_iHistory);
//
//    //自己下的每个区域
//    if (_betArea.myCoinText.size() > 0) {
//        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
//            string moneyString = Tools::addComma(pcene->i64OwnNotes[i]);
//            _betArea.myCoinText.at(i)->setString(moneyString);
//        }
//    }
//
//    //所有区域下注值
//    if (_betArea.allCoinText.size() > 0) {
//        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
//            string moneyString = Tools::addComma(pcene->i64AreaNotes[i]);
//            _betArea.allCoinText.at(i)->setString(moneyString);
//        }
//    }
//
//    m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_XIAZHU, pcene->iCurrentStateLeftTimes);

    //转盘上随机显示一个光环，其最后结束的位置就是下局转盘开始时动物的起始位置
    schedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree), 1.5f);
}

void GameTableUIV99::resumeTheGameKaiJiang(GameStation_Kaijiang* pcene, BYTE gameStation) {
    if (pcene == nullptr) {
        return;
    }
    m_GameStatus = pcene->bGameStation;
    if (pcene->bCurrentBankerID > 0) {
        if (_bankerArea.requestBankerBtn) {
//            _bankerArea.requestBankerBtn->setVisible(true);
            setButtonEnabled(_bankerArea.requestBankerBtn, pcene->i64MyMoney >= m_i64BankerNeedMoney);
        }

        memset(m_iBankerNickName, 0, sizeof(m_iBankerNickName));
        memcpy(m_iBankerNickName, pcene->iBankerNickName, sizeof(m_iBankerNickName));
        if (_bankerArea.bankerName) {
            _bankerArea.bankerName->setVisible(true);
            _bankerArea.bankerName->setString(ForceGBKToUtf8(pcene->iBankerNickName));
        }
        if (_bankerArea.bankerCoin) {
            _bankerArea.bankerCoin->setString(Tools::addComma(pcene->iBankerMoney));
        }
    }
//    updateGameStateLabelContent();
//    controlBetButtonEnable(m_GameStatus == GS_XIAZHU);
//    m_i64SingleNoteLimit = pcene->i64SingleNoteLimit;
//    memcpy(m_iMultiplying, pcene->imultiplying, sizeof(pcene->imultiplying));
//    auto leftTime = pcene->itime;
//    memcpy(m_iHistory, pcene->iHistory, sizeof(pcene->iHistory));
//
//    memcpy(m_i64OwnNotes, pcene->i64OwnNotes, sizeof(pcene->i64OwnNotes));
//    memcpy(m_i64AreaNotes, pcene->i64AreaNotes, sizeof(pcene->i64AreaNotes));
//
//    //开的动物
//
//    //彩金
//    if (_goldPool.poolMoney) {
//        _goldPool.i64Money = pcene->i64Winning;
//        string poolMoneyStr = Tools::addComma(pcene->i64Winning);
//        _goldPool.poolMoney->setString(poolMoneyStr);
//    }
//
//    //刷新身上金额
//    updateGoldLabel(_GameTableLogic->getMyMoney());
//
//    //刷新纪录
//    updateStateImage(m_iHistory);
//
//    //自己下的每个区域
//    if (_betArea.myCoinText.size() > 0) {
//        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
//            string moneyString = Tools::addComma(pcene->i64OwnNotes[i]);
//            _betArea.myCoinText.at(i)->setString(moneyString);
//        }
//    }
//
//    //所有区域下注值
//    if (_betArea.allCoinText.size() > 0) {
//        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
//            string moneyString = Tools::addComma(pcene->i64AreaNotes[i]);
//            _betArea.allCoinText.at(i)->setString(moneyString);
//        }
//    }
//
//    m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_KAIJIANG, pcene->iCurrentStateLeftTimes);
}

void GameTableUIV99::resumeTheGameOver(GameStation_Base* pcene, BYTE gameStation) {
    if (pcene == nullptr) {
        return;
    }
    m_GameStatus = pcene->bGameStation;
    if (pcene->bCurrentBankerID > 0) {
        if (_bankerArea.requestBankerBtn) {
//            _bankerArea.requestBankerBtn->setVisible(true);
            setButtonEnabled(_bankerArea.requestBankerBtn, pcene->i64MyMoney >= m_i64BankerNeedMoney);
        }

        memset(m_iBankerNickName, 0, sizeof(m_iBankerNickName));
        memcpy(m_iBankerNickName, pcene->iBankerNickName, sizeof(m_iBankerNickName));
        if (_bankerArea.bankerName) {
            _bankerArea.bankerName->setVisible(true);
            _bankerArea.bankerName->setString(ForceGBKToUtf8(pcene->iBankerNickName));
        }
        if (_bankerArea.bankerCoin) {
            _bankerArea.bankerCoin->setString(Tools::addComma(pcene->iBankerMoney));
        }
    }
//    m_i64SingleNoteLimit = pcene->i64SingleNoteLimit;
//    memcpy(m_iMultiplying, pcene->imultiplying, sizeof(pcene->imultiplying));
//    auto leftTime = pcene->itime;
//    memcpy(m_iHistory, pcene->iHistory, sizeof(pcene->iHistory));
//
//    //刷新身上金额
//    updateGoldLabel(_GameTableLogic->getMyMoney());
//
//    //刷新纪录
//    updateStateImage(m_iHistory);
//
//// 	m_bDoTimeOutCallback = false;
    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_SETTLEMENT, pcene->iCurrentStateLeftTimes);
}

//////////////////////////////////////////////////////////////////////////
//
//void GameTableUIV99::onBtnClick(Ref* pSender,Control::EventType event) {
////    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//    Node* btn = (Node*)pSender;
//    switch (btn->getTag()) {
//    case em_toolbarBtn: {
//        if(0 == _toolbarBg->getName().compare("1")) {
//            _toolbarBg->setName("0");
//            _toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height)));
//            this->getChildByTag(em_toolbarBtn)->setVisible(false);         //whao
//        }
//    }
//    break;
//    case em_toolbarSetBtn: {
//        if(0 == _toolbarBg->getName().compare("1")) {
//            _toolbarBg->setName("0");
//
//            _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height+10)),
//            CallFunc::create([this]() {
//                this->getChildByTag(em_toolbarBtn)->setVisible(true);
//            })));
//        }
//        if (this->getChildByName("setLayer")) {
//            (dynamic_cast<BRNNSetLayerV99*>(this->getChildByName("setLayer")))->closeSet();
//            return;
//        }
//        auto setLayer = BRNNSetLayerV99::create();
//        setLayer->setName("setLayer");
//        setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//        setLayer->showSet(this, ZOrder_Tool+5);
//        setLayer->setSoundCallback([]() {
//            HNAudioEngine::getInstance()->playBackgroundMusic(BB_BG_MUSIC);
//        });
//    }
//    break;
//    case  em_toolbarHelpBtn: {
//        if(0 == _toolbarBg->getName().compare("1")) {
//            _toolbarBg->setName("0");
//
//            _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height+10)),
//            CallFunc::create([this]() {
//                this->getChildByTag(em_toolbarBtn)->setVisible(true);
//            })));
//        }
//        if (this->getChildByName("helpLayer")) {
//            BRNN::Actions::actionOut(this->getChildByName("helpLayer"));
//            return;
//        }
//        auto helpLayer = BBHelpLayerV99::create();
//        helpLayer->setName("helpLayer");
//        addChild(helpLayer, ZOrder_Tool+5);
//        helpLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//        BRNN::Actions::actionIn(helpLayer);
//    }
//    break;
//    case em_toolbarLeaveBtn: {
//        if(0 == _toolbarBg->getName().compare("1")) {
//            _toolbarBg->setName("0");
//
//            _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height+10)),
//            CallFunc::create([this]() {
//                this->getChildByTag(em_toolbarBtn)->setVisible(true);
//            })));
//        }
//        if (!RoomLogic()->connected()) {
//            leaveTheGame();
//        } else {
//            if (m_iSelfIsBanker) {
//                //是否是庄
//                if (m_iSelfIsBanker) {
//                    auto layer = GamePromptLayer::create();
//                    layer->showPrompt(GBKToUtf8("您需连庄五局后才可以下庄离开！"));
//                    return;
//                }
//            } else {
//                // 游戏不在空闲时期，不允许离开
//                bool bHasNote = false;
//                for (int i = 0; i < LoopK_A_BetA_Count; i++) {
//                    if (m_i64OwnNotes[i] > 0) {
//                        bHasNote = true;
//                    }
//                }
//                if (bHasNote) {
//                    auto layer = GamePromptLayer::create();
//                    layer->showPrompt(GBKToUtf8("游戏过程中不允许离开！"));
//                    return;
//                } else {
//                    _GameTableLogic->sendUserUp();
//                }
//            }
//        }
//    }
//    break;
//    }
//}

bool GameTableUIV99::onTouchBegan(Touch* touch, Event* event) {
    //Point point = touch->getLocation();
    //point = _betArea.tableBg->convertToNodeSpace(point);

    //if(0 == _toolbarBg->getName().compare("1")) {
    //    _toolbarBg->setName("0");

    //    _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 10)),
    //    CallFunc::create([this]() {
    //        this->getChildByTag(em_toolbarBtn)->setVisible(true);
    //    })));
    //}

    return true;
}

void GameTableUIV99::onEnter() {
    HNGameUIBase::onEnter();
    HNAudioEngine::getInstance()->playBackgroundMusic(BB_BG_MUSIC, true);

    _fScaleX = WIN_SIZE.width / DESIGN_SIZE.width;
    _fScaleY = WIN_SIZE.height / DESIGN_SIZE.height;

    //触摸
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan=CC_CALLBACK_2(GameTableUIV99::onTouchBegan,this);
    myListener->setSwallowTouches(false);
    dispatcher->addEventListenerWithSceneGraphPriority(myListener,this);


    createRootLayer();
    addLoopSprites();
    createPlayerInfon();
    createMoneyArea();
    createNoteLabels();
    createCarLayer();
    createState();
    createGameStateLayer();
    createGameSettlementLayer();
    createClock();
    createToolBarUI();


    _GameTableLogic->sendGameInfo();



//    schedule(schedule_selector(GameTableUIV99::updateTestContent), 2.5f);
}

void GameTableUIV99::onExit() {
    if (m_audioEffectID > 0)
        HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);

	HttpClient::getInstance()->destroyInstance();

    HNGameUIBase::onExit();
}

void GameTableUIV99::onExitTransitionDidStart() {
    HNGameUIBase::onExitTransitionDidStart();
    HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameTableUIV99::updatePlayerInfo() {

}

void GameTableUIV99::addUser(BYTE seatNo, bool bMe) {
    UserInfoStruct * user = _GameTableLogic->getUserBySeatNo(seatNo);

    if (bMe && user) {
        _playerHead->setPlayerName(user->nickName);
        _playerHead->setPlayerIcon(user->bLogoID);
        _playerHead->setPlayerMoney(user->i64Money);
    }
    //auto viewSeatNo = _GameTableLogic->logicToViewSeatNo(seatNo);

    //m_vPlayerHead.at(viewSeatNo)->setVisible(true);
    //m_vPlayerHead.at(viewSeatNo)->setHead(user->bLogoID);
    //m_vPlayerHead.at(viewSeatNo)->setNick(user->nickName);
    //m_vPlayerHead.at(viewSeatNo)->setGold(user->i64Money);
}

void GameTableUIV99::updateClock(float dt) {
    if (m_fTimeCount <= 0) {
        killTimer();
    }
    m_fTimeCount -= floor(dt);
}

void GameTableUIV99::setTimer(BYTE bChairID, int iTimerID, float fTime) {
    m_bTimerChairID = bChairID;
    m_iTimerID = iTimerID;
    m_fClockTime = fTime;
    m_fTimeCount = fTime - 1;
    m_bDoTimeOutCallback = true;

    if (m_clockObj && fTime >= 1) {
        m_clockObj->stop();
        m_clockObj->setVisible(true);
        m_clockObj->start(fTime);
    }

    this->schedule(schedule_selector(GameTableUIV99::updateClock), 1.0f);
}

void GameTableUIV99::killTimer() {
    if (m_clockObj) {
        m_clockObj->stop();
        m_clockObj->setVisible(false);
    }
    this->unschedule(schedule_selector(GameTableUIV99::updateClock));

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

//void GameTableUIV99::timeOutCallback(BYTE bChairID, int iTimerID) {
//    if (_GameTableLogic->getMySeatNo() != bChairID) return;
//
//    if (m_bDoTimeOutCallback == false) return;
//
//    //准备超时
//    if (iTimerID == CLIENT_TIMER_FREE) {
//// 		killTimer();
//// 		//killTimer(CLIENT_TIMER_PREPARE);
////         if (!RoomLogic()->connected()) {
////             leaveTheGame();
////         } else {
////             _GameTableLogic->sendUserUp();
////         }
//    }
//    //else if (iTimerID == CLIENT_TIMER_CATCH_BANKER) {
//    //	killTimer();
////       catchBankerClickCallback(m_NotCatchBtn, Widget::TouchEventType::ENDED);
////   }
//    //else if (iTimerID == CLIENT_TIMER_SET_MONEY) {
//    //	killTimer();
//    //	//如果超时，则默认使用最小金币值
//    //	autoUpdateSetMoneyUI(m_i64BetArr[0]);
////   }
//    //else if (iTimerID == CLIENT_TIMER_SET_BOMB) {
//    //	killTimer();
//    //	autoUpdateSetBombUI(NULL, rand() % 10);
////   }
//    //else if (iTimerID == CLIENT_TIMER_OPEN_LE)
//    //{
//    //	killTimer();
//    //	sendOpenLEButtonCallBack(_waitEnvelopeNode.Open_Button, Widget::TouchEventType::ENDED);
//    //}
//}
//
//void GameTableUIV99::updateUIForResume() {
//    //if (m_CatchBankerBtn)
//    //{
//    //	m_CatchBankerBtn->setVisible(false);
//    //}
//    //if (m_NotCatchBtn)
//    //{
//    //	m_NotCatchBtn->setVisible(false);
//    //}
//    //if (m_StageImage)
//    //{
//    //	m_StageImage->setVisible(false);
//    //}
//    //if (m_bPrepareTime)
//    //{
//    //	m_PrepareBtn->setVisible(false);
//    //}
//}
//
//void GameTableUIV99::updateTip(float dt) {
//    int iTag = m_TextTip->getTag();
//    if (iTag == 0) {
//        unschedule(schedule_selector(GameTableUIV99::updateTip));
//        _GameTableLogic->sendUserUp();
//        return;
//    }
//
////     char str[128] = { 0 };
////     iTag -= 1;
////     sprintf(str, "金币数额低于%lld，%ds后离开游戏！", m_i64TipMoney, iTag);
////     m_TextTip->setString(GBKToUtf8(str));
////     m_TextTip->setTag(iTag);
//	iTag -= 1;
//
//	char s[128] {0};
//	std::string str;
//	str.append(GBKToUtf8("金币数额低于"));
//	sprintf(s, "%lld", m_i64TipMoney);
//	str.append(s);
//	sprintf(s, ",%d", iTag);
//	str.append(s);
//	str.append(GBKToUtf8("s后离开游戏！"));
//	m_TextTip->setText(str);
//	m_TextTip->setTag(iTag);
//}

//void GameTableUIV99::setBtnTouch(Button* btn, bool bTouch) {
//
//    btn->setTouchEnabled(bTouch);
//    auto image = dynamic_cast<ImageView*>(btn->getChildByName("openImage"));
//    if (bTouch) {
//        btn->setColor(Color3B(255, 255, 255));
//        image->setColor(Color3B(255, 255, 255));
//    } else {
//        btn->setColor(Color3B(180, 180, 180));
//        image->setColor(Color3B(180, 180, 180));
//    }
//}
//
//void GameTableUIV99::tipCallback() {
//    if (tipMsg.bIsExit == 1) {
//        //准备关闭掉
//        //m_PrepareBtn->setVisible(false);
//
//        auto layer = GamePromptLayer::create();
//        layer->setCloseExitBtn();
////         char str[128] = { 0 };
////         int sec = 3;
////         sprintf(str, "金币数额低于%lld，%ds后离开游戏！", tipMsg.i64Money, sec);
////         layer->showPrompt(GBKToUtf8(str));
//		int sec = 3;
//		char s[128] {0};
//		std::string str;
//		str.append(GBKToUtf8("金币数额低于"));
//		sprintf(s, "%lld", tipMsg.i64Money);
//		str.append(s);
//		sprintf(s, ",%d", sec);
//		str.append(s);
//		str.append(GBKToUtf8("s后离开游戏！"));
//		layer->showPrompt(str);
//
//        layer->setCallBack([&]() {
//            unschedule(schedule_selector(GameTableUIV99::updateTip));
//            _GameTableLogic->sendUserUp();
//        });
//        layer->setColorLayerTouchEnabled(false);
//
//        m_i64TipMoney = tipMsg.i64Money;
//        m_TextTip = layer->getPromptText();
//        m_TextTip->setTag(sec);
//        schedule(schedule_selector(GameTableUIV99::updateTip), 1.0f);
//    } else {
//
//    }
//}
//
//void GameTableUIV99::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
//    if (bVisible) {
//        if (!m_Notice) {
//            m_Notice = GameNotice::create();
//            m_Notice->setLocalZOrder(NOTICE_ZORDER);
//            m_Notice->setName("GameNotice");
//        }
//        m_Notice->openNotic(this, 10000, message, nCount, nType);
//    }
//}
//
////////////////////////////////////////////////////////////////////////////
//按钮回调
////////////////////////////////////////////////////////////////////////////

//
void GameTableUIV99::requestBankerCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    auto btn = (AUIButton*)pSender;
    if (btn) {
        if (btn->getTag() == em_reqBankerBtnTag) {
//                setButtonEnabled(btn, false);
            _GameTableLogic->sendRequestBanker(true);
        }
    }
}

//
void GameTableUIV99::betAreaButtonsCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    if (_BetSingleOperate.moneyIndex < 0 || _BetSingleOperate.moneyIndex >= Bet_Coin_Count) {
        //未正确选择下注筹码
        return;
    }
    auto btn = (Button*)pSender;
    int betIndex = -1;
    if (btn) {
        betIndex = btn->getTag() - em_betLamMaxTag;
    }
    if (betIndex >= 0 && betIndex < LoopK_A_BetA_Count) {
        _BetSingleOperate.betAreaIndex = betIndex;
    }
    //判断是否满足下注条件，满足则将下注信息发给服务端
    _betData.betMoney += BetMoneyValues[_BetSingleOperate.moneyIndex];
    _betData.betMoneyEachArea[_BetSingleOperate.moneyIndex] += BetMoneyValues[_BetSingleOperate.moneyIndex];
    //
    _GameTableLogic->sendBet(_BetSingleOperate.betAreaIndex, BetMoneyValues[_BetSingleOperate.moneyIndex]);

    //animation
    betMoneyToAreaAnimation(_BetSingleOperate.moneyIndex, _BetSingleOperate.betAreaIndex, true);

    _BetSingleOperate.betAreaIndex = -1;
}

//
void GameTableUIV99::betMoneyCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    auto btn = (Button*)pSender;
    int moneyIndex = -1;
    if (btn) {
        if (_betData.betMoneyEnable[btn->getTag() - em_money0Tag]) {
            moneyIndex = btn->getTag() - em_money0Tag;
        }
    }
    if (moneyIndex >= 0 && moneyIndex < Bet_Coin_Count) {
        _BetSingleOperate.moneyIndex = moneyIndex;
        updateBetMoneySelectedState(_BetSingleOperate.moneyIndex);
    }
}

void GameTableUIV99::betClearCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    if (m_GameStatus != GS_XIAZHU) {
        return;
    }
    auto btn = (AUIButton*)pSender;
    if (btn) {
        _GameTableLogic->sendClearRequest();
    }
}

void GameTableUIV99::betRebetCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED) {
        return;
    }
    if (m_GameStatus != GS_XIAZHU) {
        return;
    }
    auto btn = (AUIButton*)pSender;
    if (btn) {
        _GameTableLogic->sendRebetRequest();
    }
}

////下注操作区域按钮回调
//void GameTableUIV99::betAreaCallBack(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED) {
//        return;
//    }
//    //HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//    playBetMoneyEffect();
//    if (m_iBetMoneyType < 0 || m_iBetMoneyType >= Bet_Count) {
//        return;//下注前没有选择金额
//    }
//    auto btn = (Button*)pSender;
//    hideBetAreaSelectState();
//    if (btn) {
//        m_iBetType = btn->getTag();
//        if (m_iBetType >= K_FQ && m_iBetType <= K_SY) {
//            if (m_iBetMoneyType >= 0 && m_iBetMoneyType < Bet_Count) {
//                _GameTableLogic->sendBet(m_iBetType, g_i64BetValue[m_iBetMoneyType]);
//            }
//        }
//    }
//}
//
////下注金额选择按钮回调
//void GameTableUIV99::betButtonsCallBack(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED) {
//        return;
//    }
//
//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//    if (m_GameStatus != GS_XIAZHU) {
//        controlBetButtonEnable(false);
//        return;
//    }
//
//    auto btn = (Button*)pSender;
//
//    if (btn) {
//        hideBetButtonSelectState();
//
//        int iTag = btn->getTag();
//        int iMoneyType = iTag - 15;
//        m_iBetMoneyType = iMoneyType;
//        if (m_iBetMoneyType < Bet_Count) {
//
//            if (!_betButton.btnLights.empty() && m_iBetMoneyType < _betButton.btnLights.size()) {
//                if (_betButton.btnLights.at(m_iBetMoneyType)) {
//                    if (_betButton.btnLights.at(m_iBetMoneyType)->getTag() == iTag) {
//                        _betButton.btnLights.at(m_iBetMoneyType)->setVisible(true);
//                    }
//                }
//            }
//        }
//    }
//}
//
////取消、续注按钮回调
//void GameTableUIV99::clearRebetCallBack(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED) {
//        return;
//    }
//
//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//    //showResultButtonAnimation(K_SZ);
//
//    if (m_GameStatus != GS_XIAZHU) {
//        controlBetButtonEnable(false);
//        return;
//    }
//
//    auto btn = (Button*)pSender;
//
//    if (btn) {
//
//        int iTag = btn->getTag();
//
//        if (iTag == tag_clear) {
//            _GameTableLogic->sendBet(k_QK, 0);
//            //其他区域恢复
//            //m_betButtonTag = INVALID_BYTE;
//            //controlBetMoneyArea(INVALID_BYTE, false);
//            hideBetAreaSelectState();
//        } else if (iTag == tag_rebet) {
//            _GameTableLogic->sendBet(k_XT, 0);
//        } else {
//
//        }
//    }
//}
//
////左右按钮回调
//void GameTableUIV99::leftRightArrowCallBack(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED) {
//        return;
//    }
//    auto btn = (Button*)pSender;
//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//    if (btn) {
//        if (btn->getTag() == tag_left) {
//            //向左按钮回调
//
//        } else if (btn->getTag() == tag_right) {
//            //向右按钮回调
//
//        }
//    }
//}


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

////控制下注金额按钮的响应
//void GameTableUIV99::controlBetValueButtonsEnable(bool canTouch /* = true */) {
//    if (!_betButton.buttons.empty()) {
//        for (int i = 0; i < _betButton.buttons.size(); i++) {
//            if (_betButton.buttons.at(i)) {
//                _betButton.buttons.at(i)->setTouchEnabled(canTouch);
//            }
//        }
//    }
//}
//
////控制下注按钮是否允许响应
//void GameTableUIV99::controlBetButtonEnable(bool canTouch) {
//    //十一种动物
//    if (!_betArea.betButtons.empty()) {
//        for (int i = 0; i < _betArea.betButtons.size(); i++) {
//            if (_betArea.betButtons.at(i)) {
//                //if (!canTouch)
//                //{
//                //	_betArea.betButtons.at(i)->setScale(1.0f);
//                //}
//                _betArea.betButtons.at(i)->setTouchEnabled(canTouch);
//            }
//        }
//    }
//    //五个下注金额
//    if (!_betButton.buttons.empty()) {
//        for (int i = 0; i < _betButton.buttons.size(); i++) {
//            if (_betButton.buttons.at(i)) {
//                //if (!canTouch)
//                //{
//                //_betButton.buttons.at(i)->setScale(1.0f);
//                //}
//                _betButton.buttons.at(i)->setTouchEnabled(canTouch);
//            }
//        }
//    }
//    if (!canTouch) {
//        hideBetButtonSelectState();
//    }
//    controlClearButtonEnable(canTouch);
//    controlRebetButtonEnable(canTouch && m_iPrevHasNote);
//}
//
////隐藏金额按钮全部选中效果
//void GameTableUIV99::hideBetButtonSelectState() {
//    if (!_betButton.btnLights.empty()) {
//        for (int i = 0; i < _betButton.btnLights.size(); i++) {
//            if (_betButton.btnLights.at(i)) {
//                _betButton.btnLights.at(i)->setVisible(false);
//            }
//        }
//    }
//}
//
////控制清空按钮
//void GameTableUIV99::controlClearButtonEnable(bool canTouch /* = true */) {
//    if (_betArea.clearBtn) {
//        _betArea.clearBtn->setBright(canTouch);
//        _betArea.clearBtn->setTouchEnabled(canTouch);
//    }
//}
//
////控制续投按钮
//void GameTableUIV99::controlRebetButtonEnable(bool canTouch /* = true */) {
//    if (_betArea.resetBtn) {
//        _betArea.resetBtn->setBright(canTouch);
//        _betArea.resetBtn->setTouchEnabled(canTouch);
//    }
//}
//
//void GameTableUIV99::hideBetAreaSelectState() {
//    //选择之后隐藏
//    if (!_betArea.btnLights.empty()) {
//        for (int i = 0; i < Bet_Area_Num; i++) {
//            if (_betArea.btnLights.at(i)) {
//                _betArea.btnLights.at(i)->stopAllActions();
//                _betArea.btnLights.at(i)->setVisible(false);
//            }
//        }
//    }
//}
//
//void GameTableUIV99::resetTextValue() {
//    //自己下的每个区域
//    if (_betArea.myCoinText.size() > 0) {
//        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
//            _betArea.myCoinText.at(i)->setString("0");
//        }
//    }
//
//    //所有区域下注值
//    if (_betArea.allCoinText.size() > 0) {
//        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
//            _betArea.allCoinText.at(i)->setString("0");
//        }
//    }
//}

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

//只展示筹码飞过去的动画，金额更新动画放到下注消息回调中
void GameTableUIV99::betMoneyToAreaAnimation(int betMoneyIndex, int betAreaIndex, bool isMe/* =false */) {
    if (betMoneyIndex < 0 || betMoneyIndex >= Bet_Coin_Count) {
        return;
    }
    if (m_betAreaPosition.empty()) {
        return;
    }
    if (betAreaIndex < 0 || betAreaIndex >= LoopK_A_BetA_Count || betAreaIndex >= m_betAreaPosition.size()) {
        return;
    }
    if (_GameTableLogic->getMyMoney() < BetMoneyValues[betMoneyIndex]) {
        return;
    }

    m_audioEffectID = HNAudioEngine::getInstance()->playEffect(BB_NOTE_COIN_EFFECT);

    char path[100] = {0};
    sprintf(path, Bet_Note_Res_V99, betMoneyIndex);
    Sprite* sprite = Sprite::create(path);
    Vec2 startPos = Vec2::ZERO;
    Vec2 endPos = m_betAreaPosition.at(betAreaIndex);
    startPos = isMe == true ? m_notesPosition.at(betMoneyIndex) : Vec2(WIN_SIZE.width / 10.0f * (rand() % 8 + 2), -50.0f);
    if (sprite) {
        sprite->setScale(0.6f);
        sprite->setPosition(startPos);
        endPos += Vec2((rand() % 8) * 5 * (rand() % 3 - 1), (rand() % 8) * 5 * (rand() % 3 - 1));
        Sequence* seq = Sequence::create(MoveTo::create(0.4f, endPos), nullptr);
        sprite->runAction(seq);
        addChild(sprite, ZOrder_BetButtonLabel - 1);
        m_AllNotesMoveAnimations.at(betAreaIndex).push_back(sprite);
    }
}

void GameTableUIV99::updateBetMoneySelectedState(int betMoneyIndex) {
    if (betMoneyIndex < 0 || betMoneyIndex >= Bet_Coin_Count) {
        return;
    }
    if (_betArea.betMoneyLight.empty()) {
        return;
    }
    if (betMoneyIndex >= _betArea.betMoneyLight.size()) {
        return;
    }
    for (Sprite* light : _betArea.betMoneyLight) {
        if (light) {
            light->setVisible(false);
        }
    }
    if (_betArea.betMoneyLight.at(betMoneyIndex)) {
        _betArea.betMoneyLight.at(betMoneyIndex)->setVisible(true);
    }
}







void GameTableUIV99::showRunLoopAnimation(int endIndex, int loopTime, float delayTime) {
    unschedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree));
    if (endIndex < 0 || endIndex >= Loop_Sprite_Count) {
        //转盘上的序号是 0-31
        return;
    }

    //上局的终点即为这局的起点
    m_loopAnimationCurrentIndex = (m_loopAnimationCurrentIndex < 0 || m_loopAnimationCurrentIndex >= Loop_Sprite_Count) ? 0 : m_loopAnimationCurrentIndex;
    m_loopAnimationEndIndex = endIndex; //结束时亮光的位置


    m_loopAnimationRemainTime = loopTime * Loop_Sprite_Count +
                                ((endIndex - m_loopAnimationCurrentIndex) <= 0 ? Loop_Sprite_Count : 0) + (endIndex - m_loopAnimationCurrentIndex);//精灵要移动的数量，超过一圈要做修正
    m_loopAnimationMaxTime = m_loopAnimationRemainTime;


    //先将起点的光环点亮，之后再开始闪烁的动作
    stopAllLoopAnimation();
    startLoopAnimationAtIndex(m_loopAnimationCurrentIndex);

    m_audioEffectID = HNAudioEngine::getInstance()->playEffect(BB_START_LOOP_EFFECT);
    schedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition), 0.16);//前10个，慢速，0.16s一帧，共1.6s
}

//至少会转动4圈，有116-140个
void GameTableUIV99::updateLoopSpritePosition(float dt) {
    m_loopAnimationCurrentIndex++;
    m_loopAnimationRemainTime--;

    if (m_loopAnimationRemainTime < 0) {
        stopAllLoopAnimation();
        //最后一个，结束，不再旋转
        startLoopAnimationAtIndex(m_loopAnimationCurrentIndex);
        //最后一个，展示动画
        playOpenLogoAnimationAtIndex(m_loopAnimationCurrentIndex);
        //////////////////////////////////////////////////////////////////////////
        m_loopAnimationRemainTime = 0;
        m_loopAnimationMaxTime = 0;
        m_loopAnimationCurrentIndex = m_loopAnimationEndIndex;
        unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
        return;
    }
    if (!_loopArea.loopSprites.empty()) {
        stopAllLoopAnimation();
        startLoopAnimationAtIndex(m_loopAnimationCurrentIndex);

        //
        if (m_loopAnimationRemainTime <= 0) {
            stopAllLoopAnimation();
            //最后一个，结束，不再旋转
            startLoopAnimationAtIndex(m_loopAnimationCurrentIndex);
            //最后一个，展示动画
            playOpenLogoAnimationAtIndex(m_loopAnimationCurrentIndex);
            //////////////////////////////////////////////////////////////////////////
            m_loopAnimationRemainTime = 0;
            m_loopAnimationMaxTime = 0;
            m_loopAnimationCurrentIndex = m_loopAnimationEndIndex;
            unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
            return;
        }
    }
    //
    if (m_loopAnimationRemainTime == m_loopAnimationMaxTime - 9) {
        m_audioEffectID = HNAudioEngine::getInstance()->playEffect(BB_MAX_SPEED_LOOP_EFFECT);
        unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
        schedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition), (7.5f - 5.0f) * 0.9f / (m_loopAnimationMaxTime - 20));
    } else if (m_loopAnimationRemainTime == 15) {
        unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
        schedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition), 0.1f);//共1.2s
    } else if (m_loopAnimationRemainTime == 5) {
        m_audioEffectID = HNAudioEngine::getInstance()->playEffect(BB_SLOW_SPEED_LOOP_EFFECT);
        unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
        schedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition), 0.54f); //共2.7s
    }
}

//固定时间间隔随机闪烁，其最后停止的位置即为下局转盘开始的位置
void GameTableUIV99::updateRandomBlinkLightFree(float dt) {
    if (m_GameStatus != GS_XIAZHU) {
        unschedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree));
        return;
    }
    m_loopAnimationEndIndex = rand() % Loop_Sprite_Count;
    if (m_loopAnimationEndIndex < 0 || m_loopAnimationEndIndex >= Loop_Sprite_Count) {
        m_loopAnimationEndIndex = 0;
    }
    m_loopAnimationCurrentIndex = m_loopAnimationEndIndex;
    updateBlinkOpenAnimalLight();
}

void GameTableUIV99::updateTestContent(float dt) {
//        if (_gameState) {
//            int stateID = rand()%6;
//            _gameState->updateGameStateContent(stateID);
//        }
}


//开启的动画位置的光环交替闪烁
void GameTableUIV99::updateBlinkOpenAnimalLight() {
    if (m_loopAnimationEndIndex < 0 || m_loopAnimationEndIndex >= Loop_Sprite_Count) {
        m_loopAnimationCurrentIndex = 0;
        m_loopAnimationEndIndex = 0;
        return;
    }
    if (!_loopArea.loopSprites.empty()) {
        if (_loopArea.loopSprites.at(m_loopAnimationEndIndex)) {
            _loopArea.loopSprites.at(m_loopAnimationEndIndex)->stopAllActions();
            Sequence* seq = Sequence::create(CallFunc::create([this]() {
                stopAllLoopAnimation();
            }), DelayTime::create(0.2f), CallFunc::create([this]() {
                startLoopAnimationAtIndex(m_loopAnimationEndIndex);
            }), DelayTime::create(0.28f), CallFunc::create([this]() {
                stopAllLoopAnimation();
            }), nullptr);
            if (seq) {
                _loopArea.loopSprites.at(m_loopAnimationEndIndex)->runAction(seq);
            }
        }
    }
}



void GameTableUIV99::showOpenAnimalAnimation(int index) {
    if (index < 0 || index >= Loop_Sprite_Count) {
        return;
    }
}

void GameTableUIV99::stopAllLoopAnimation() {
    if (_loopArea.loopSprites.empty()) {
        return;
    }
    for (int i = 0; i < _loopArea.loopSprites.size(); i++) {
        if (_loopArea.loopSprites.at(i)) {
            _loopArea.loopSprites.at(i)->showLightEffect(false);
        }
    }
}

void GameTableUIV99::startLoopAnimationAtIndex(int index) {
    if (_loopArea.loopSprites.empty()) {
        return;
    }
    index = index % Loop_Sprite_Count;
    if (index >= 0 && index < _loopArea.loopSprites.size()) {
        if (_loopArea.loopSprites.at(index)) {
            _loopArea.loopSprites.at(index)->showLightEffect(true);
        }
    }
}

void GameTableUIV99::playOpenLogoAnimationAtIndex(int index) {
    if (_loopArea.loopSprites.empty()) {
        return;
    }
    index = index % Loop_Sprite_Count;
    if (index >= 0 && index < _loopArea.loopSprites.size()) {
        if (_loopArea.loopSprites.at(index)) {
            _loopArea.loopSprites.at(index)->playOpenLogoAnimation();
        }
    }
}


//传入新的开局记录数组，先将数组的值保存，然后再更新UI显示
void GameTableUIV99::updateStateImage(int iHistory[]) {

    vector<int> kinds;
    for (int i = 0; i < Open_History_Logo_Count; i++) {
        if (iHistory[i] >= 0 && iHistory[i] < Loop_Sprite_Count) {
            kinds.push_back(iHistory[i]);
        }
    }
    if (kinds.empty()) {
        return;
    }
    m_openRecordAnimal.clear();
    for (int i = 0; i < Open_History_Logo_Count; i++) {
        if (i < kinds.size()) {
            m_openRecordAnimal.push_back(kinds.at(i));
        }
    }
    //
    if (_openRecord.openLogos.empty()) {
        createState();
    }
    for (int i = 0; i < kinds.size(); i++) {
        if (_openRecord.openLogos.at(i)) {
            _openRecord.openLogos.at(i)->setCellIndex(kinds.at(i));
            _openRecord.openLogos.at(i)->showLoopCellNode(true);
            //i不是最后一个
            if (i != kinds.size() - 1) {
                _openRecord.openLogos.at(i)->setCellIndex(kinds.at(i));
            } else {
                _openRecord.openLogos.at(i)->showLightEffect(true);
//                animationMoveOpenItemToStateLabel(kinds.at(i));
            }
        }
    }

}

//将开奖的icon移动到走势图最后的位置，endIdx为最后一个
void GameTableUIV99::animationMoveOpenItemToStateLabel(int endIdx) {
    Vec2 startPos = Vec2::ZERO;
    Vec2 endPos = Vec2::ZERO;
    int lastIndexInState = 0;
    auto cell = BBLoopSpriteCellV99::create();
    if (cell) {
        cell->setCellIndex(endIdx);
        if (!_loopArea.loopCellPositions.empty() && endIdx < _loopArea.loopCellPositions.size()) {
            startPos = _loopArea.loopCellPositions.at(endIdx);
        }
        if (!_openRecord.openLogosPositions.empty()) {
            if (_openRecord.openLogos.size() < Open_History_Logo_Count) {
                //未满，则与size相同
//                if (_openRecord.openLogos.size() < _openRecord.openLogosPositions.size())
                {
                    lastIndexInState =(int)_openRecord.openLogos.size();
                    endPos = _openRecord.openLogosPositions.at(lastIndexInState);
                }

            } else {
                lastIndexInState = (int)_openRecord.openLogosPositions.size() - 1;
                endPos = _openRecord.openLogosPositions.back();
            }
        }
        //
        startPos = convertToWorldSpace(startPos);
        endPos = convertToWorldSpace(endPos);
        cell->setCellPosition(startPos);
        addChild(cell);
        //Animations
        Sequence* seq = Sequence::create(Spawn::create(ScaleTo::create(0.5f, 2.0f),
                                         MoveTo::create(0.5f, WIN_SIZE / 2),
                                         nullptr),
                                         Spawn::create(ScaleTo::create(0.3f, 1.0f),
                                                 MoveTo::create(0.3f, endPos),
                                                 nullptr),
        CallFunc::create([=] {

            if (cell) {
                cell->removeFromParent();
            }
            if (!_openRecord.openLogosPositions.empty()) {
                auto newCell = BBLoopSpriteCellV99::create();
                if (newCell) {
                    newCell->setCellIndex(endIdx);
                    newCell->setPosition(_openRecord.openLogosPositions.at(lastIndexInState));
                    newCell->setCellScale(0.7f);
                    newCell->showLightEffect(true);
                }
            }
        }), nullptr);

        cell->runAction(seq);
//            }
//        }
    }



}


//车灯动画
void GameTableUIV99::animationOpenCarLight() {
    if (_car.carAction) {
        _car.carAction->play("car", true);
    }
    if (_car.carNode) {
        _car.carNode->setVisible(true);
    }
}

void GameTableUIV99::animationStopCarLight() {
    if (_car.carNode) {
        _car.carNode->setVisible(false);
    }
    if (_car.carAction) {
        _car.carAction->stop();
    }
}


////请下注 动画
//void GameTableUIV99::animationTipBetBegin() {
//    Vec2 pos = WIN_SIZE / 2;
//    if (_openRecord.node) {
//        pos = _openRecord.node->getPosition();
//    }
//    if (getChildByTag(FQZS_ANIMATION_BET_NODE_TAG)) {
//        getChildByTag(FQZS_ANIMATION_BET_NODE_TAG)->stopAllActions();
//        getChildByTag(FQZS_ANIMATION_BET_NODE_TAG)->removeFromParent();
//    }
//
//    auto node = CSLoader::createNode(FQZS_ANIMATION_BET_CSB);
//    if (node) {
//        node->setPosition(pos);
//        node->setTag(FQZS_ANIMATION_BET_NODE_TAG);
//        addChild(node);
//        //
//        auto action = CSLoader::createTimeline(FQZS_ANIMATION_BET_CSB);
//        node->runAction(action);
//        action->play("bet", false);
//    }
//}
//
////请开奖动画，播放完动画后转圈开奖
//void GameTableUIV99::animationTipStartCircle() {
//    auto node = CSLoader::createNode(FQZS_ANIMATION_TIP_START_CIRCLE_CSB);
//    if (node) {
//        Vec2 pos = WIN_SIZE / 2;
//        if (_openRecord.node) {
//            pos = _openRecord.node->getPosition();
//        }
//        node->setPosition(pos);
//        addChild(node);
//    }
//    //
//    auto action = CSLoader::createTimeline(FQZS_ANIMATION_TIP_START_CIRCLE_CSB);
//    if (action) {
//        node->runAction(action);
//        action->play("lottery", false);
//    }
//}
//
//void GameTableUIV99::playFreeMusic() {
////    HNAudioEngine::getInstance()->stopBackgroundMusic();
////    HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_FREE, true);
//}
//
//void GameTableUIV99::playBetMusic() {
////    HNAudioEngine::getInstance()->stopBackgroundMusic();
////    HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_BET, true);
//    HNAudioEngine::getInstance()->playEffect(AUDIO_TIP_BET);
//}
//
//void GameTableUIV99::playStopBetMusic() {
//    HNAudioEngine::getInstance()->playEffect(AUDIO_TIP_STOP_BET);
//}
//
//void GameTableUIV99::playBetMoneyEffect() {
//    HNAudioEngine::getInstance()->playEffect(AUDIO_BET_MONEY);
//}
//
//void GameTableUIV99::playOpenMusic() {
////    HNAudioEngine::getInstance()->stopBackgroundMusic();
////    HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_OPEN, true);
//}
//
//void GameTableUIV99::playOpenEffectAudio(BYTE kind) {
//    if (kind == INVALID_BYTE) {
//        return;
//    }
//    char res[100];
//    memset(res, 0, sizeof(res));
//    sprintf(res, AUDIO_KIND_RES, kind);
//    HNAudioEngine::getInstance()->playEffect(res);
//}
//
//void GameTableUIV99::playLoopEffectAudio(BYTE type) {
//    switch (type) {
//    case Loop_Start:
//        HNAudioEngine::getInstance()->playEffect(AUDIO_LOOP_START);
//        break;
//    case Loop_Max_Speed:
//        HNAudioEngine::getInstance()->playEffect(AUDIO_LOOP_MAX_SPEED);
//        break;
//    case Loop_Slow_Speed:
//        HNAudioEngine::getInstance()->playEffect(AUDIO_LOOP_SLOW_SPEED);
//        break;
//    case Loop_End:
//        HNAudioEngine::getInstance()->playEffect(AUDIO_LOOP_END);
//        break;
//    default:
//        break;
//    }
//}
//
////////////////////////////////////////////////////////////////////////////
////传入服务端开的动物类型，返回押注区域对应按钮的索引编号
//BYTE GameTableUIV99::getBetButtonIndexByKindID(BYTE kindID) {
//    BYTE index = INVALID_BYTE;
//    for (int i = 0; i < Bet_Area_Num; i++) {
//        if (Bet_Button_ID[i] == kindID) {
//            index = i;
//            break;
//        }
//    }
//    return index;
//}
//
////传入服务端转盘结束的位置，将该位置转换成对应的动物类型
//BYTE GameTableUIV99::getAnimalKindByLoopIndex(BYTE endIndex) {
//    BYTE kindID = INVALID_BYTE;
//    if (endIndex < 0 || endIndex >= Open_Area) {
//        return kindID;
//    }
//    kindID = Area_Kind[endIndex];
//    return kindID;
//}
//
//void GameTableUIV99::updateMyWinLabel(long long money) {
//    if (_betArea.myWin) {
//        ostringstream ss;
//        m_allMyWinGold += money;
//        ss << Tools::addComma(m_allMyWinGold);
//        _betArea.myWin->setString(ss.str().c_str());
//        ss.str("");
//    }
//}
//
//void GameTableUIV99::updateGoldLabel(long long money) {
//    if (_betArea.myGold && money >= 0) {
//        string moneyStr = Tools::addComma(money);
//        _betArea.myGold->setString(moneyStr);
//    }
//}

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
//	resetUINewGame();
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



//玩家坐下(非自己)
void GameTableUIV99::userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    addUser(userSit->bDeskStation, userSit->bDeskStation == _GameTableLogic->getMySeatNo());
}

//玩家站起
void GameTableUIV99::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    if (m_GameStatus == GS_FREE) {
//        BYTE bViewChairID = _GameTableLogic->logicToViewSeatNo(userSit->bDeskStation);
        //m_vPlayerHead.at(bViewChairID)->setVisible(false);
        //m_vPrepareImage.at(bViewChairID)->setVisible(false);

//        if (m_bGameEnd) {
//
//            //m_vPlayerPokerPointImage.at(bViewChairID)->setVisible(false);
//            //if (m_vSettlementValueLabel.at(bViewChairID))
//            //{
//            //	m_vSettlementValueLabel.at(bViewChairID)->setVisible(false);
//            //}
//        }
    }
}

void GameTableUIV99::onGameFree(BB::GameFreeStruct *msg) {
    //重置上轮游戏ui
    resetNewGame();

    if (msg == nullptr) {
        return;
    }

    memcpy(m_iMultiplying, msg->imultiplying, sizeof(msg->imultiplying));

    m_iFreeTime = msg->bCurrentStateLeftTimes;
    updateStateImage(msg->iHistory);


    //update My Info
    if (_playerHead) {
        if (msg->bDeskStation >= 0) {
            auto info = _GameTableLogic->getUserInfoBySeatNo(msg->bDeskStation);
            if (info) {
                _playerHead->setPlayerMoney(info->i64Money);
                _playerHead->setPlayerName(info->nickName);
                _playerHead->setPlayerIcon(info->bLogoID);
            }
        }
    }

    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_FREE, msg->bCurrentStateLeftTimes);

}

void GameTableUIV99::onConfirmBanker(BB::GameBeganAndConfirmBanker *msg) {
    if (msg == nullptr) {
        return;
    }
//        if (_bankerArea.requestBankerBtn) {
//            _bankerArea.requestBankerBtn->setVisible(true);
//            setButtonEnabled(_bankerArea.requestBankerBtn, true);
//        }
    m_GameStatus = msg->gameState;
    m_i64SingleNoteLimit = msg->i64SingleNoteLimit;

//        msg->bCurrentStateLeftTimes;

    memset(m_iBankerNickName, 0, sizeof(m_iBankerNickName));
    memcpy(m_iBankerNickName, msg->bCurrentBankerNickName, sizeof(m_iBankerNickName));
    if (_bankerArea.bankerName) {
        _bankerArea.bankerName->setVisible(true);
        _bankerArea.bankerName->setString(ForceGBKToUtf8(msg->bCurrentBankerNickName));
    }
    if (_bankerArea.bankerCoin) {
        _bankerArea.bankerCoin->setString(Tools::addComma(msg->iCurrentBankerMoney));
    }

    if (m_iSelfIsBanker && !msg->isBanker) {
        //自己上局是庄家这局不是庄家
        switch (msg->bankerCode) {
        case BANKER_XIAZHUANG_LACK_COIN:
            if (_gameState) {
                _gameState->updateGameStateContent(GSTATE_XIAZHUANG_LACK_MONEY);
            }
            break;

        case BANKER_XIAZHUANG_OVER_FIVE:
            if (_gameState) {
                _gameState->updateGameStateContent(GSTATE_XIAZHUANG_OVER_FIVE);
            }
            break;
        }
    }
    if (msg->isWaitingBanker && !msg->isBanker) {
        //等待上庄
    }
    if (!msg->isWaitingBanker && !msg->isBanker) {
        //此时允许重新申请上庄
        _bankerArea.isShangZhuang = true;
        if (_bankerArea.requestBankerBtn) {
            _bankerArea.requestBankerBtn->setBright(true);
            setButtonEnabled(_bankerArea.requestBankerBtn, true);
        }
    }
    if (msg->isBanker) {
        //目前是庄家
        if (_gameState) {
            switch (msg->bankerCode) {
            case BANKER_BE_BANKER_SUCCEED:
                _gameState->updateGameStateContent(GSTATE_BE_BANKER_SUCCEED);
                break;
            case BANKER_TWO:
                _gameState->updateGameStateContent(GSTATE_BE_BANKER_TWO);
                break;
            case BANKER_THREE:
                _gameState->updateGameStateContent(GSTATE_BE_BANKER_THREE);
                break;
            case BANKER_FOUR:
                _gameState->updateGameStateContent(GSTATE_BE_BANKER_FOUR);
                break;
            case BANKER_FIVE:
                _gameState->updateGameStateContent(GSTATE_BE_BANKER_FIVE);
                break;
            }
        }
    }
    if (_bankerArea.requestBankerCount) {
        char name[100];
        memset(name, 0, sizeof(name));
        if (msg->isWaitingBanker) {
            sprintf(name, GetText("BB_ZHUANG_WAITING_LIST_COUNT"), msg->iBankerListWaitNumber);
            _bankerArea.requestBankerCount->setString(name);
        } else {
            sprintf(name, GetText("BB_ZHUANG_LIST_COUNT"), msg->iBankerListWaitNumber);
            _bankerArea.requestBankerCount->setString(name);
        }
    }
    //如果自己是庄家，限制下注等
    if (msg->isBanker) {
        m_iSelfIsBanker = true;
        disableAllBetMoney();
        setButtonEnabled(_betArea.reBetBtn, false);
        setButtonEnabled(_betArea.clearBtn, false);
    } else {
        m_iSelfIsBanker = false;
    }





}

void GameTableUIV99::onGameBeginNote(BB::GameBeganBet *msg) {
    if (msg == nullptr) {
        return;
    }
    m_GameStatus = msg->gameState;
    m_iBetTime = msg->bCurrentStateLeftTimes;
    m_iPrevHasNote = msg->iPrevHasNote;

    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_XIAZHU, msg->bCurrentStateLeftTimes);


    updateGameState(m_GameStatus);

    m_iSelfIsBanker = msg->isBanker;
    if (!m_iSelfIsBanker) {
        updateBetMoneyEnable();
        if (msg->iPrevHasNote) {
            setButtonEnabled(_betArea.reBetBtn, true);
        }
        setButtonEnabled(_betArea.clearBtn, true);
    } else {
        disableAllBetMoney();
        setButtonEnabled(_betArea.reBetBtn, false);
        setButtonEnabled(_betArea.clearBtn, false);
    }





    //转盘上随机显示一个光环，其最后结束的位置就是下局转盘开始时动物的起始位置
    schedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree), 1.5f);

}

void GameTableUIV99::onGameNoteResponse(BB::GameXiaZhuResponse *msg) {

    if (msg->bDeskStation == _GameTableLogic->getMySeatNo()) {
        if (_playerHead) {
            _playerHead->setPlayerMoney(msg->iUserMoney);
        }
        updateBetMoneyEnable();
        //每个区域下注值，这个方法目前只局限于自己下注，其他玩家下注的需要特殊处理
        if (_betArea.noteLabels.size() > 0) {
            for (int i = 0; i < LoopK_A_BetA_Count; i++) {
                m_i64OwnNotes[i] = msg->iOwnNotes[i];
                m_i64AreaNotes[i] = msg->iNotePerRegion[i];
                auto cell = _betArea.noteLabels.at(i);
                if (cell) {
                    cell->updateNotes(m_i64OwnNotes[i], m_i64AreaNotes[i]);
                }
            }
        }
    } else {
        //其他玩家下注

    }




    //下注类型
    //msg->iType;

    //自己下的总注
    //msg->iOwnAllCountNote;

    //最近一笔主值
    //msg->iNoteNum;
}

void GameTableUIV99::onGameRobotNoteMsg(BB::GameRobotXiaZhuResponse *msg) {
    if (msg == nullptr) {
        return;
    }
    betMoneyToAreaAnimation(msg->iNoteMoneyIndex, msg->iNoteArea, true);
    //每个区域下注值
    if (_betArea.noteLabels.size() > 0) {
        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
            m_i64AreaNotes[i] = msg->iNotePerRegion[i];
            auto cell = _betArea.noteLabels.at(i);
            if (cell) {
                cell->updateNotes(m_i64OwnNotes[i], m_i64AreaNotes[i]);
            }
        }
    }

}

void GameTableUIV99::onGameNoteFail(BB::GameXiaZhuFaileMsg *msg) {
    if (msg == nullptr) {
        return;
    }
    if (_gameState) {
        switch (msg->bFailCode) {
        case 0://金币不足
            _gameState->updateGameStateContent(GSTATE_XIAZHU_FAILE_LACK_MONEY);
            break;
        case 1://庄家不够赔，下注权限
            _gameState->updateGameStateContent(GSTATE_XIAZHU_FAIL_BANKER_LACK_MONEY);
            break;
        case 2:
            _gameState->updateGameStateContent(GSTATE_XIAZHU_FAILE_OVER);
            break;
        default:
            _gameState->updateGameStateContent(GSTATE_XIAZHU_FAIL_BANKER_LACK_MONEY);
            break;
        }
    }
}

void GameTableUIV99::onGameClearResponse(BB::GameClearResponse *msg) {
    updateBetMoneyEnable();
}

void GameTableUIV99::onGameReBetResponse(BB::GameReBetResponse *msg) {
    updateBetMoneyEnable();
}

void GameTableUIV99::onGameRequestBankerResponse(BB::GameRequestBankerResponse *msg) {
    if (msg == nullptr) {
        return;
    }
    if (msg->requestSucceed) {
        //按钮变成下庄按钮
        _bankerArea.isShangZhuang = false;//显示已申请
        if (_bankerArea.requestBankerBtn) {
            _bankerArea.requestBankerBtn->setBright(false);
            setButtonEnabled(_bankerArea.requestBankerBtn, false);
        }
        if (_gameState) {
            _gameState->updateGameStateContent(GSTATE_SHANGZHUANG_SUCCEED);
        }
    } else {
        _bankerArea.isShangZhuang = true;
        if (_bankerArea.requestBankerBtn) {
            setButtonEnabled(_bankerArea.requestBankerBtn, true);
        }
        if (_gameState) {
            _gameState->updateGameStateContent(GSTATE_SHANGZHUANG_FAIL_LACK_MONEY);
        }
    }

    if (_bankerArea.bankerName) {
        _bankerArea.bankerName->setVisible(true);
        _bankerArea.bankerName->setString(ForceGBKToUtf8(msg->iBankerNickName));
    }
    if (_bankerArea.bankerCoin) {
        _bankerArea.bankerCoin->setString(Tools::addComma(msg->iCurrentBankerMoney));
    }
    char info[100];
    memset(info, 0, sizeof(info));
    if (_bankerArea.requestBankerCount) {
        _bankerArea.requestBankerCount->setVisible(true);
        if (msg->requestSucceed) {
            sprintf(info, GetText("BB_ZHUANG_WAITING_LIST_COUNT"), msg->iBankerListNumber);
            _bankerArea.requestBankerCount->setString(info);
        } else {
            sprintf(info, GetText("BB_ZHUANG_LIST_COUNT"), msg->iBankerListNumber);
            _bankerArea.requestBankerCount->setString(info);
        }
        memset(info, 0, sizeof(info));
    }
    if (_bankerArea.requestBankerNeedsLabel) {
        _bankerArea.requestBankerNeedsLabel->setVisible(true);
        sprintf(info, "%s", GetText("BB_ZHUANG_NEED"));
        _bankerArea.requestBankerNeedsLabel->setString(info);
        memset(info, 0, sizeof(info));
    }
    if (_bankerArea.requestBankerNeeds) {
        _bankerArea.requestBankerNeeds->setVisible(true);
        _bankerArea.requestBankerNeeds->setString(Tools::addComma(msg->iBankerNeedMoney));
    }


}

void GameTableUIV99::onGameXiaZhuangResponse(BB::GameXiaZhuangResponse *msg) {
    if (msg == nullptr) {
        return;
    }

}

void GameTableUIV99::onGameNoteFinish(BB::GameXiaZhuFinish *msg) {
    m_audioEffectID = HNAudioEngine::getInstance()->playEffect(BB_STOP_NOTE_EFFECT);

    _BetSingleOperate.moneyIndex = -1;
    _BetSingleOperate.betAreaIndex = -1;

    m_GameStatus = msg->gameState;

    setButtonEnabled(_betArea.clearBtn, false);
    setButtonEnabled(_betArea.reBetBtn, false);

    updateBetMoneyEnable();
    updateAllNoteButtonsState(false);
    updateGameState(m_GameStatus);

}


//void GameTableUIV99::onGameStartCircle(GamePlay* msg) {
////    //停止下注声音提示
//////    playStopBetMusic();
////    //下注阶段的ui关闭
////    hideBetAreaSelectState();
////    //controlBetMoneyArea(INVALID_BYTE, false);
////    controlBetButtonEnable(false);
////
////    //////////要刷新的东西
////    m_iCircleTime = msg->iTime;
////    //起点
////    //msg->iStarPos;
////    int endPos = msg->iEndPos;
////
////    //先播放开奖动画提示，然后转圈效果展示
////    Sequence* seq = Sequence::create(CallFunc::create([=] {
////        animationTipStartCircle();
////    }),
////    DelayTime::create(2.1f),
////    CallFunc::create([=] {
////        //终点位置  圈数  总时长
////        showRunLoopAnimation(endPos, 3 + rand()%2, 7.5f);
////    }), nullptr);
////
////    this->runAction(seq);
////
////    //开的动物
////    //msg->iEndAnimal;
////
////    //是否射灯
////    //msg->iIsShedeng;
////
////    //更新彩金池金额
////    if (_goldPool.poolMoney) {
////        _goldPool.i64Money = msg->i64Winnings;
////        string poolMoneyStr = Tools::addComma(msg->i64Winnings);
////        _goldPool.poolMoney->setString(poolMoneyStr);
////    }
////
////    m_bDoTimeOutCallback = false;
////    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_KAIJIANG, m_iCircleTime);
////
////    m_GameStatus = GS_KAIJIANG;
////    updateGameStateLabelContent();
////    //playOpenMusic();
//}

void GameTableUIV99::onGameOpenLogo(GameLoopResult* msg) {
    m_GameStatus = msg->gameState;
    m_iCircleTime = msg->bCurrentStateLeftTimes;
    if (msg->iOpenLogo < 0 || msg->iOpenLogo >= LoopK_A_BetA_Count) {
        return;
    }
    if (msg->iEndPos < 0 || msg->iEndPos >= Loop_Sprite_Count) {
        return;
    }
    //终点
//    m_loopAnimationCurrentIndex = msg->iStarPos;
    animationOpenCarLight();
    m_loopAnimationEndIndex = msg->iEndPos;
    showRunLoopAnimation(m_loopAnimationEndIndex, msg->iTimes);

    setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_KAIJIANG, msg->bCurrentStateLeftTimes);
}

//计算结果
void GameTableUIV99::onGameSettlementMsg(GameSettlementMsg* msg) {

    if (msg == nullptr) {
        return;
    }

    animationStopCarLight();

    //刷新纪录
    updateStateImage(msg->iHistory);

    if (_playerHead) {
        _playerHead->setPlayerMoney(msg->i64UserMoney);
    }

    if (_gameSettlement) {
        auto user = _GameTableLogic->getUserInfoBySeatNo(msg->playerIndex);
        if (user) {
            _gameSettlement->updateMyName(user->nickName);
        }

        _gameSettlement->updateSettlementLayer(msg->i64RealWinMoney);
        _gameSettlement->updateBankerName(m_iBankerNickName);
        _gameSettlement->updateOpenLogo(msg->openLogoIndexAtLoop);
        _gameSettlement->updateBankerInfo(msg->bankerRealWinMoney);
    }

}

void GameTableUIV99::onGameTipExit(tag_s2c_tip_exit* msg) {
    if (msg == nullptr) {
        return;
    }
//    tipMsg.bIsExit = msg->bIsExit;
//    tipMsg.i64Money = msg->i64Money;
//

	tipMsg = msg;
    //不退出
    if (msg->bIsExit == 0) {
      return;
    } 

	

    //要退出
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
        tipCallback();
    })));
}


void GameTableUIV99::tipCallback() {
	if (tipMsg->bIsExit == 1) {
		//准备关闭掉
		//m_PrepareBtn->setVisible(false);

		auto layer = GamePromptLayer::create();
		//layer->setCloseExitBtn();
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
			unschedule(schedule_selector(GameTableUIV99::updateTip));
			_GameTableLogic->sendUserUp();
		});
		layer->setColorLayerTouchEnabled(false);

		m_i64TipMoney = tipMsg->i64Money;
		m_TextTip = layer->getPromptText();
		m_TextTip->setTag(sec);
		schedule(schedule_selector(GameTableUIV99::updateTip), 1.0f);
	}
	else {

	}
}

void GameTableUIV99::updateTip(float dt) {
	int iTag = m_TextTip->getTag();
	if (iTag == 0) {
		unschedule(schedule_selector(GameTableUIV99::updateTip));
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
	m_TextTip->setText(str);
	m_TextTip->setTag(iTag);
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

//
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
//    updateGameStateLabelContent();
//    playFreeMusic();
//
//    m_bGameEnd = true;
//    m_canPrepareForNew = false;
//    //m_betButtonTag = INVALID_BYTE;
//
//    m_iBetType = -1;
//    m_iBetMoneyType = -1;
//
    m_iPrevHasNote = 0;	//重置上局下注信息，等待服务端重新通知
//    m_iSelfIsBanker = false;

    memset(m_i64OwnNotes, 0, sizeof(m_i64OwnNotes));
    memset(m_i64AreaNotes, 0, sizeof(m_i64AreaNotes));
//    //////////////
//    stopAnimalsAnimation();
    this->unschedule(schedule_selector(GameTableUIV99::updateLoopSpritePosition));
//    this->unschedule(schedule_selector(GameTableUIV99::updateResultAnimation));
    this->unschedule(schedule_selector(GameTableUIV99::updateRandomBlinkLightFree));
//    m_openResultAnimalIndex.clear();
//    m_openResultAnimalKind.clear();
//
//    //controlBetMoneyArea(INVALID_BYTE, false);
//    hideBetAreaSelectState();
//    resetTextValue();
    resetAllNotesAnimationAndClean();
    if (_gameSettlement) {
        _gameSettlement->hideSettlementLayer();
    }
    if (_betArea.noteLabels.size() > 0) {
        for (auto cell : _betArea.noteLabels) {
            if (cell) {
                cell->clearNotes();
            }
        }
    }

    animationStopCarLight();
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
    resetAllNotesAnimationAndClean();
}

void GameTableUIV99::resetData() {
    //ui
    _fScaleX = 0;
    _fScaleY = 0;

    //
    _playerHead = nullptr;
    _gameState = nullptr;
    _gameSettlement = nullptr;
//    _tableSize = Size(0, 0);
//
//    _tableBg = nullptr;
//    _tableBgRight = nullptr;
//
//    _toolbarBg = nullptr;
//    m_ClockBg = nullptr;
//    m_ClockTimeLabel = nullptr;
    m_clockObj = nullptr;
    m_clockPos = Vec2(-247.8, 214);
//
    m_Notice = nullptr;
//
//    //数据
//    m_bPrepareTime = 0;
//    m_bCatchBankerTime = 0;
//    m_bSetMoneyTime = 0;
//    m_bSetBombTime = 0;
//    m_bOpenLETime = 0;
//    m_bShowResultTime = 0;
//
//    m_fClockTime = 0;
    m_fTimeCount = 0;
    m_audioEffectID = 0;
//    m_bTimerChairID = INVALID_BYTE;
//    m_iTimerID = 0;
//    m_iSendRoundNum = 0;
//    m_iSendViewID = 0;
//
//    //上局是否有下注
//    m_iPrevHasNote = 0;
//    m_iBetType = -1;
//    m_iBetMoneyType = -1;
//
//    m_GameStatus = GS_FREE;
//    m_bGameEnd = false;
//    m_bNextOut = false;
//    m_bCurUser = INVALID_BYTE;
//    m_bNextUser = INVALID_BYTE;
//
//    m_canPrepareForNew = false;
//    //m_bBankerID = INVALID_BYTE;
//    //m_vPlayerPrepare.clear();
//
//    //BetArea
//    _betArea.clearResetNode = nullptr;
//    _betArea.myGold = nullptr;
//    _betArea.myWin = nullptr;
//    _betArea.myGoldSprite = nullptr;
//    _betArea.myWinSprite = nullptr;
//    _betArea.clearBtn = nullptr;
//    _betArea.resetBtn = nullptr;
//    _betArea.betButtons.clear();
//    _betArea.allCoinText.clear();
//    _betArea.myCoinText.clear();


    m_notesPosition.clear();
    m_betAreaPosition.clear();
    m_betLabelPos.clear();

//    _betArea.btnLights.clear();
//    _betArea.areaNode = nullptr;
//
//    //ResultLoop
//    _resultLoop.loopNode = nullptr;
//    _resultLoop.animals.clear();
//
//    _resultLabel.labelNode = nullptr;
//    _resultLabel.font = nullptr;
//
//    //MoneyPool
//    _goldPool.poolNode = nullptr;
//    _goldPool.poolMoney = nullptr;
//    _goldPool.i64Money = 0;
//
//    //BetButtons
//    _betButton.node = nullptr;
//    _betButton.buttons.clear();
//    _betButton.btnLights.clear();
//
//    //OpenRecord
//    _openRecord.node = nullptr;
//    _openRecord.arrowLeft = nullptr;
//    _openRecord.arrowRight = nullptr;
//    _openRecord.animals.clear();
//
//    //game state label
//    m_gameStateNode = nullptr;
//    m_gameStateLabel = nullptr;
//
//    //游戏总的输赢
//    m_allMyWinGold = 0;
//    //m_betButtonTag = INVALID_BYTE;
//
//    //animation
//    m_animalAnimationNode = nullptr;
//
//    //
//    m_betAreaKindSize.clear();
//    m_betAreaPosition.clear();
//    m_betAreaKindSize.resize(2);
//    m_betAreaPosition.resize(3);
//
//    //
//    m_loopAnimalPosition.clear();
//    m_loopAnimationSprite.clear();
//    m_loopAnimationSprite.resize(3);
    m_loopAnimationRemainTime = 0;
    m_loopAnimationMaxTime = 0;
    m_loopAnimationEndIndex = -1;
    m_loopAnimationCurrentIndex = 0;


//    m_openRecordAnimal.clear();
//    m_openRecordAnimal.resize(Open_Animal_Num);
//
//    //
//    m_openResultAnimalKind.clear();
//    m_openResultAnimalIndex.clear();

    memset(m_i64OwnNotes, 0, sizeof(m_i64OwnNotes));
    memset(m_i64AreaNotes, 0, sizeof(m_i64AreaNotes));

    resetAllNotesAnimationAndClean();
    if (_gameSettlement) {
        _gameSettlement->hideSettlementLayer();
    }

    m_iSelfIsBanker = false;
    memset(m_iBankerNickName, 0, sizeof(m_iBankerNickName));

}

void GameTableUIV99::resetAllNotesAnimationAndClean() {
    if (!m_AllNotesMoveAnimations.empty()) {
        for (int i = 0; i < m_AllNotesMoveAnimations.size(); i++) {
            std::vector<Sprite*> sprites = m_AllNotesMoveAnimations.at(i);
            for (int j = 0; j < sprites.size(); j++) {
                auto spr = sprites.at(j);
                if (spr) {
                    spr->stopAllActions();
                    spr->removeFromParentAndCleanup(true);
                }
            }
            sprites.clear();
        }
    }
    m_AllNotesMoveAnimations.clear();
    m_AllNotesMoveAnimations.resize(LoopK_A_BetA_Count);
}

//Tools
void GameTableUIV99::setButtonEnabled(ui::Button* pButton, bool enabled) {
    if (pButton != nullptr) {
        GLubyte opacity = enabled ? 255 : 125;
        pButton->setTouchEnabled(enabled);
        pButton->setOpacity(opacity);
    }
}

void GameTableUIV99::updateAllNoteButtonsState(bool enable) {
    for (Button* money : _betArea.betMoneyBtn) {
        if (money) {
            setButtonEnabled(money, enable);
        }
    }
    if (!enable) {
        for (int i = 0; i < Bet_Coin_Count; i++) {
            _betArea.betMoneyLight[i]->setVisible(false);
        }
    }
}

void GameTableUIV99::updateBetMoneyEnable() {
    for (int i = 0; i < Bet_Coin_Count; i++) {
        if (_GameTableLogic->getMyMoney() >= BetMoneyValues[i]) {
            _betData.betMoneyEnable[i] = true;
        } else {
            _betData.betMoneyEnable[i] = false;
            _betArea.betMoneyLight[i]->setVisible(false);
        }
        if (_betArea.betButtons.size() > i && _betArea.betButtons.at(i)) {
            setButtonEnabled(_betArea.betMoneyBtn.at(i), _betData.betMoneyEnable[i]);
        }
    }
}

void GameTableUIV99::enableAllBetMoney() {
    updateBetMoneyEnable();
}

void GameTableUIV99::disableAllBetMoney() {
    for (int i = 0; i < Bet_Coin_Count; i++) {
        setButtonEnabled(_betArea.betMoneyBtn.at(i), false);
    }
}

void GameTableUIV99::updateGameState(int gameState) {
    if (_gameState) {
        Sequence* seq = Sequence::create(CallFunc::create([=]() {
            if (_gameState) {
                _gameState->updateGirlState(gameState);
            }
        }), DelayTime::create(1.5f), CallFunc::create([=]() {
            if (_gameState) {
                _gameState->hideStateLayer();
            }
        }), nullptr);
        if (seq) {
            _gameState->runAction(seq);
        }
    }
}

void GameTableUIV99::hideGameState(bool hideState) {
    if (_gameState) {
//            _gameState->setNodeVisible(hideState);
        _gameState->hideStateLayer();
    }
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
			_playerHead->setPlayerMoney(msg->i64MyMoney);
		}), DelayTime::create(retainTime), CallFunc::create([&]() {
			this->removeChildByName("rewardResult");
		}), NULL));
	}
}

}
