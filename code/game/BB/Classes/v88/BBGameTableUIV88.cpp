#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
//#include "../../TTNN/Classes/GameTable/BRNNHelpLayer.h"
//#include "../../TTNN/Classes/v88/BRNNSetLayerV88.h"
//#include "../../TTNN/Classes/GameTable/BRNNActions.h"
#include "../../../../code/common/LibHNUI/UI/UIGoldRain.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNLobby/v66/ExtraReward.h"

#include "BBGameTableUIV88.h"
#include "BBGameTableLogic.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "BBLoopSpriteCellV88.h"
#include "BBPlayerHeadV88.h"
#include "BBGameStateV88.h"
#include "BBSettlementV88.h"
#include "BBNoteLabelCellV88.h"
//#include "BBHelpLayerV88.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"

namespace BB {

#define WINSIZE Director::getInstance()->getWinSize()
#define NOTICE_ZORDER 10000

	////////////////////////////////////////////////////////////////////////////
	////常量
	//
	//    
	enum Em_TableBtnTag_V88 {
		em_toolbarBtn = 1000,
		em_toolbarChangeTableBtn,
		em_toolbarSetBtn,
		em_toolbarHelpBtn,
		em_toolbarStandupBtn,
		em_toolbarLeaveBtn,
		em_toolbarNextOutBtn,
		em_toolbarNextOutLight
	};

	enum Em_BetButtonTag_V88 {
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

	enum Em_BetMoneyTag_V88 {
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

	//    enum {
	//        Bet_Area_Num = 11,	//下注界面按钮数量（不包含取消与续投）
	//        Open_Animal_Num = 10,	//服务端开出的动物总数量为12种
	//    };
	//    
	//    
	////押注按钮名字
	//static char* Bet_Button_Names[Bet_Area_Num] = {
	//    "Button_fq",//飞禽
	//    "Button_yz",//燕子
	//    "Button_gz",//鸽子
	//    "Button_kq",//孔雀
	//    "Button_ly",//老鹰
	//    "Button_zs",//走兽
	//    "Button_sz",//狮子
	//    "Button_xm",//熊猫
	//    "Button_hz",//猴子
	//    "Button_tz",//兔子
	//    "Button_sy",//鲨鱼
	//};
	//
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
//	//押注类型按钮Tag数组
//	static int Bet_Button_Tag[LoopK_A_BetA_Count] = {
//		tag_lam_max,
//		tag_bmw_max,
//		tag_ben_max,
//		tag_vm_max,
//		tag_vm_min,
//		tag_ben_min,
//		tag_bmw_min,
//		tag_lam_min,
//	};
//	//押注金额按钮Tag数组
//	static int Bet_Money_Tag[5] = {
//		tag_level_1,
//		tag_level_2,
//		tag_level_3,
//		tag_level_4,
//		tag_level_5,
//	};
//
//	//押注金额
//	static long long g_i64BetValue[Bet_Coin_Count] = {
//		1000,
//		10000,
//		100000,
//		1000000,
//		5000000
//	};

	//转盘变速类型
	enum LoopType {
		Loop_Start,
		Loop_Max_Speed,
		Loop_Slow_Speed,
		Loop_End
	};


	//////////////////////////////////////////////////////////////////////////
	//CSB文件
	//    static const char* BB_BASIC_RESULT_CELL_CSB = "csb/Games/BB_V66/Node_Basic_Result_Cell.csb";
	static const char* BB_BET_OPERATE_BUTTONS_CSB = "csb/Games/BB_V66/Node_Bet_Operate_Buttons_V66.csb";
	static const char* BB_HISTORY_LABEL_CSB = "csb/Games/BB_V66/Node_History_Label_V66.csb";
	static const char* BB_ROOT_V88_CSB = "csb/Games/BB_V66/Node_Root_V66.csb";


	//Resources
//	static const char* Request_Banker_Bg_Res = "v66/button/4/shangzhuang.png";
//	static const char* Request_Banker_Content_Res = "v66/button/4/2.png";
	static const char* Bet_Note_Res_V88 = "v66/button/1/%d.png";
	static const char* Bet_Clear_Rebet_BG_Res_V88 = "v66/button/blue1/BG.png";
	static const char* Bet_Clear_Rebet_BG_SEL_V88 = "v66/button/blue1/BG_1.png";
//	static const char* Bet_Clear_Rebet_BG_DISABLE_V88 = "v66/button/blue1/BG_0.png";
	static const char* Bet_Clear_Content_V88 = "v66/button/blue1/2.png";
	static const char* Bet_Rebet_Content_V88 = "v66/button/blue1/1.png";
//	static const char* BB_CAR_IMAGE_PATH = "v66/BB/car.png";
//	static const char* Xiazhuang_Content_Res = "v66/button/4/yishenqin.png";

	//static const char* FQZS_RESULT_LABEL_CSB		= "FQZS/Result.csb";
	//static const char* FQZS_BET_AREA_CSB			= "FQZS/Bet_Area.csb";
	//static const char* FQZS_CLEAR_RESET_AREA_CSB	= "FQZS/ClearResetArea.csb";
	//static const char* FQZS_BET_BUTTONS_CSB			= "FQZS/Bet_Buttons.csb";
	//static const char* FQZS_RESULT_LOOP_CSB			= "FQZS/Result_Loop.csb";
	//static const char* FQZS_STATE_CSB				= "FQZS/State.csb";
	//static const char* FQZS_STATE_LABEL_CSB			= "FQZS/GameState.csb";
	//static const char* FQZS_POOL_CSB				= "FQZS/Pool.csb";
	//
	////Animation
	//static const char* FQZS_ANIMATION_YZ_CSB = "common/animation/FQZS_animal/swallow/swallow.csb";
	//static const char* FQZS_ANIMATION_GZ_CSB = "common/animation/FQZS_animal/dove/dove.csb";
	//static const char* FQZS_ANIMATION_KQ_CSB = "common/animation/FQZS_animal/peacock/peacock.csb";
	//static const char* FQZS_ANIMATION_LY_CSB = "common/animation/FQZS_animal/eagle/eagle.csb";
	//static const char* FQZS_ANIMATION_SZ_CSB = "common/animation/FQZS_animal/lion/lion.csb";
	//static const char* FQZS_ANIMATION_XM_CSB = "common/animation/FQZS_animal/panda/panda.csb";
	//static const char* FQZS_ANIMATION_HZ_CSB = "common/animation/FQZS_animal/monkey/monkey.csb";
	//static const char* FQZS_ANIMATION_TZ_CSB = "common/animation/FQZS_animal/rabbit/rabbit.csb";
	//static const char* FQZS_ANIMATION_SY_CSB = "common/animation/FQZS_animal/shark/blueshark.csb";
	//static const char* FQZS_ANIMATION_JS_CSB = "common/animation/FQZS_animal/shark/goldenshark.csb";
	//static const char* FQZS_ANIMATION_BET_CSB = "common/animation/FQZS_text/bet/FQZS_bet.csb";
	//static const char* FQZS_ANIMATION_TIP_START_CIRCLE_CSB = "common/animation/FQZS_text/lottery/lottery.csb";


	////////////////////////////////////////////////////////////////////////////
	////字体
	//static const char* FQZS_FONT_TTF = "platform/fonts/font.ttf";
	//

	////////////////////////////////////////////////////////////////////////////
	//图片资源
	//background

	//tool
//	static const char* BB_TOOL_BAR_PATH = "v66/common/tool/%s";
	
	//music
	static const char* BB_BG_MUSIC = "v66/music/BB/BB_BGM.mp3";
	static const char* BB_STOP_NOTE_EFFECT = "v66/music/BB/bcbm_count.wav";
	static const char* BB_START_LOOP_EFFECT = "v66/music/BB/bcbm_start.wav";
	static const char* BB_MAX_SPEED_LOOP_EFFECT = "v66/music/BB/bcbm_loop.wav";
	static const char* BB_SLOW_SPEED_LOOP_EFFECT = "v66/music/BB/bcbm_stop.wav";
	static const char* BB_NOTE_COIN_EFFECT = "v66/music/BB/add_bet.wav";


	//////////////////////////////////////////////////////////////////////////

	HNGameUIBase* GameTableUIV88::create(BYTE bDeskIndex, bool bAutoCreate) {
		GameTableUIV88* tableUI = new GameTableUIV88();
		if (tableUI && tableUI->init(bDeskIndex, bAutoCreate)) {
			tableUI->autorelease();
		}
		else {
			delete tableUI;
			tableUI = nullptr;
		}
		return tableUI;
	}

	GameTableUIV88::GameTableUIV88()
		//    : m_iBetMoneyType(-1)
	{
		//    TextureCache::getInstance()->addImage(NN_GOLD_IMAGE);
	}

	GameTableUIV88::~GameTableUIV88() {
		//    this->unschedule(schedule_selector(GameTableUIV88::updateClock));
		//    this->unschedule(schedule_selector(GameTableUIV88::updateTip));
		this->unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
		//    this->unschedule(schedule_selector(GameTableUIV88::updateResultAnimation));
		//    this->unschedule(schedule_selector(GameTableUIV88::showGameStateLabelAnimation));
		this->unschedule(schedule_selector(GameTableUIV88::updateRandomBlinkLightFree));

		//    this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));

		//this->unschedule(schedule_selector(GameTableUIV88::sendCardTimer));
		//    this->unschedule(schedule_selector(GameTableUIV88::showOpenBtn));
		CC_SAFE_DELETE(_GameTableLogic);

		resetData();
	}

	bool GameTableUIV88::init(BYTE bDeskIndex, bool bAutoCreate) {
		if (!HNGameUIBase::init()) {
			return false;
		}

		resetData();

		_bDeskIndex = bDeskIndex;
		_bAutoCreate = bAutoCreate;

		return true;
	}

	void GameTableUIV88::createToolBarUI() {
        int zOrder = ZOrder_Tool;
        auto toolbar = ToolBarLayer::create();
        if (toolbar) {
            toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
                if (!RoomLogic()->connected()) {
                    leaveTheGame();
                }
                else {
                    if (m_iSelfIsBanker) {
                        //是否是庄
                        if (m_iSelfIsBanker) {
                            auto layer = GamePromptLayer::create();
                            layer->showPrompt(GBKToUtf8("您需连庄五局后才可以下庄离开！"));
                            return;
                        }
                    }
                    else {
                        // 游戏不在空闲时期，不允许离开
                        bool bHasNote = false;
                        for (int i = 0; i < LoopK_A_BetA_Count; i++) {
                            if (m_i64OwnNotes[i] > 0) {
                                bHasNote = true;
                            }
                        }
                        if (bHasNote) {
                            auto layer = GamePromptLayer::create();
							layer->showPrompt(GetText("limit_leave_in_game"));
                            return;
                        }
                        else {
                            try
                            {
                                _GameTableLogic->sendUserUp();
                            }
                            catch (char* str)
                            {
                                log("%s\n", str);
                            }
                        }
                    }
                }
            });
            toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
                IGameSet* gameSet = GameSetFactory::createWithVersion();
                gameSet->setBgPath(BB_BG_MUSIC);
                gameSet->open(this, Vec2(0, 0), zOrder+5, 0);
            });
            toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
                auto helpLayer = HN::HelpLayer::create();
                helpLayer->setContentOnView("v66/common/help/BB.png");
                helpLayer->setName("helpLayer");
                helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                helpLayer->setOrignalPosition();
                helpLayer->moveSelfInWithAction();
                addChild(helpLayer, zOrder+5);
            });
            toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
        }
	}

	void GameTableUIV88::createRootLayer() {
		auto root = CSLoader::createNode(BB_ROOT_V88_CSB);
		if (root) {
			root->setPosition(WIN_SIZE / 2);
			addChild(root, ZOrder_Loop);
			auto bg = Sprite::create(GAME_BG_COMMON2_V66);
			bg->setAnchorPoint(Vec2(0.5, 0.5));
			addChild(bg, ZOrder_Loop - 10);
			bg->setPosition(WIN_SIZE / 2);

            auto tableBg = Sprite::create("v66/BB/1_1.png");
            tableBg->setAnchorPoint(Vec2(0.5, 0.5));
            addChild(tableBg,ZOrder_Loop - 9);
            tableBg->setPosition(Vec2(WIN_SIZE.width/2,WIN_SIZE.height/2+40));
	
			/*
			auto bg = dynamic_cast<Sprite*>(root->getChildByName("Sprite_bg"));
			if (bg) {
                bg->setVisible(false);
				_betArea.tableBg = bg;
			
			}
             */
			//Loop Area
			auto loopNode = dynamic_cast<Node*>(root->getChildByName("Node_Loop"));
			if (loopNode) {
				_loopArea.loopNode = loopNode;
				char name[32] = { 0 };
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
				char name[32] = { 0 };
				for (int i = 0; i < LoopK_A_BetA_Count; i++) {
					sprintf(name, "Button_%d", i);
					auto btn = dynamic_cast<Button*>(betNode->getChildByName(name));
					if (btn) {
						m_betAreaPosition.push_back(btn->getPosition() + betNode->getPosition() + root->getPosition());
						btn->setTag(i + em_betLamMaxTag);
						btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::betAreaButtonsCallBack, this));
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
				reqBankerPos.x = _bankerArea.bankerCoin->getPositionX()-120;
				reqBankerPos.y = (_bankerArea.bankerCoin->getPositionY())+20 ;
			}
			//            AUIButton* request = AUIButton::create(Request_Banker_Bg_Res, Request_Banker_Bg_Res);
			auto request = dynamic_cast<Button*>(root->getChildByName("Button_Req"));
			if (request) {
				request->setTag(em_reqBankerBtnTag);
				request->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
				//ou request->setPosition(reqBankerPos);
				request->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::requestBankerCallBack, this));
				_bankerArea.requestBankerBtn = request;
				setButtonEnabled(request, false,false);

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
	void GameTableUIV88::createMoneyArea() {
		auto moneyRoot = CSLoader::createNode(BB_BET_OPERATE_BUTTONS_CSB);
		if (moneyRoot) {
			_betArea.moneyNode = moneyRoot;
			_betArea.moneyNode->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/64));
			//
			char name[32] = { 0 };
			for (int i = 0; i < Bet_Coin_Count; i++) {
				sprintf(name, "Bet_%d", i);
				auto btn = dynamic_cast<Button*>(moneyRoot->getChildByName(name));
				if (btn) {
					m_notesPosition.push_back(btn->getPosition() + Vec2(0, btn->getContentSize().height / 2) + moneyRoot->getPosition());
					setButtonEnabled(btn, false,true);
					btn->setTag(em_money0Tag + i);
					btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::betMoneyCallBack, this));
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
		AUIButton* renoteBtn = AUIButton::create(Bet_Clear_Rebet_BG_Res_V88, Bet_Clear_Rebet_BG_SEL_V88);
		Size xutouSize = renoteBtn->getContentSize();
		xutouImg = ImageView::create(Bet_Rebet_Content_V88);
		xutouImg->setPosition(Vec2(xutouSize.width / 2, xutouSize.height / 2));
		renoteBtn->addChild(xutouImg);
		if (renoteBtn) {
			_betArea.reBetBtn = renoteBtn;
            _betArea.reBetBtn->setVisible(true);
			renoteBtn->setTag(em_Bet_Rebet_Tag);
			//            renoteBtn->setTitle(Bet_Rebet_Content_V88);
			renoteBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			//            renoteBtn->setTitleOffset(Vec2(-4, 4));
			Size size = renoteBtn->getContentSize();
			Vec2 pos = Vec2(WIN_SIZE.width - size.width / 2 - 60, 10 + size.height);
			renoteBtn->setPosition(pos);
			renoteBtn->setScale(0.8f);
			renoteBtn->setTitle(Bet_Rebet_Content_V88);
			setButtonEnabled(renoteBtn, false,false);
			//xutouImg->setEnabled(false);
			
			renoteBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::betRebetCallBack, this));
			addChild(_betArea.reBetBtn, ZOrder_BetButton);
		}
		AUIButton* clearBtn = AUIButton::create(Bet_Clear_Rebet_BG_Res_V88, Bet_Clear_Rebet_BG_SEL_V88);
		Size clearBtnSize = clearBtn->getContentSize();
	    clearImg = ImageView::create(Bet_Clear_Content_V88);
		clearImg->setPosition(Vec2(clearBtnSize.width / 2, clearBtnSize.height / 2));
		clearBtn->addChild(clearImg);
		if (clearBtn) {
			_betArea.clearBtn = clearBtn;
            _betArea.clearBtn->setVisible(true);
			clearBtn->setTag(em_Bet_Clear_Tag);
			//            clearBtn->setTitle(Bet_Clear_Content_V88);
			clearBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			//            clearBtn->setTitleOffset(Vec2(-4, 4));
			Size size = clearBtn->getContentSize();
			Vec2 pos = Vec2(WIN_SIZE.width - size.width / 2 - 60, 10);
			clearBtn->setPosition(pos);
			clearBtn->setScale(0.8f);
			clearBtn->setTitle(Bet_Clear_Content_V88);
			setButtonEnabled(clearBtn, false,false);		
			clearBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::betClearCallBack, this));
			addChild(_betArea.clearBtn, ZOrder_BetButton);
		}
	}

	void GameTableUIV88::createNoteLabels() {
		if (_betArea.noteLabels.empty()) {
			//需要初始化
			ostringstream ss;
			for (int i = 0; i < LoopK_A_BetA_Count; i++) {
				if (_betArea.betButtons.at(i)) {

					Vec2 pos = m_betLabelPos.at(i);
					pos += WIN_SIZE / 2;
					if (i < 4)
					{
						pos.y = pos.y + 40;

					}
					else
					{
						pos.y = pos.y + 20;
					}
					BBNoteLabelCellV88* cell = BBNoteLabelCellV88::create();
					cell->setNotePosition(pos);
					cell->clearNotes();
					_betArea.noteLabels.push_back(cell);
					addChild(cell, ZOrder_BetButtonLabel);
				}
			}
		}
	}


	void GameTableUIV88::createPlayerInfon() {
		_playerHead = nullptr;
		BBPlayerHeadV88* head = BBPlayerHeadV88::create();
		if (head) {
			_playerHead = head;
			_playerHead->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
			_playerHead->setHeadPosition(Vec2(WIN_SIZE.width-80, WIN_SIZE.height-10.0f));
			addChild(_playerHead, ZOrder_MyInfo);
			//_playerHead->setScale(0.85);
		}
	}

	void GameTableUIV88::createGameStateLayer() {
		_gameState = nullptr;
		BBGameStateV88* state = BBGameStateV88::create();
		if (state) {
			_gameState = state;
			_gameState->setStateGirlPosition(Vec2(WIN_SIZE.width,0));
			_gameState->setStateLabelPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height * 0.75f));
			_gameState->hideStateLayer();
			addChild(_gameState, ZOrder_StateLayer);
		}
	}

	void GameTableUIV88::createGameSettlementLayer() {
		_gameSettlement = nullptr;
		BBSettlementV88* settle = BBSettlementV88::create();
		if (settle) {
			_gameSettlement = settle;
			_gameSettlement->setSettlementLayerPosition(WIN_SIZE / 2);
			_gameSettlement->hideSettlementLayer();
			addChild(_gameSettlement, ZOrder_SettlementLayer);
		}
	}

	//游戏历史开局情况
	void GameTableUIV88::createState() {
		if (!_openRecord.node) {
			_openRecord.node = CSLoader::createNode(BB_HISTORY_LABEL_CSB);
			if (_openRecord.node) {
				_openRecord.node->setPosition(Vec2(76.0f, WIN_SIZE.height / 2));
				addChild(_openRecord.node, ZOrder_CarLight + 1);

				Vec2 posStart = _openRecord.node->getPosition();
				Size nodeSize = Size::ZERO;
				auto bg = dynamic_cast<ImageView*>(_openRecord.node->getChildByName("Image_Bg"));
				if (bg) {
					nodeSize = bg->getContentSize();

				}
				else {
					nodeSize = Size(68, 529);
				}
				posStart.y -= nodeSize.height / 2;


				float height = 76.0f;	//精灵高
				float space = 0.0f;		//精灵间距
				float scaleRate = 0.7f;

				for (int i = 0; i < Open_History_Logo_Count; i++) {
					auto cell = BBLoopSpriteCellV88::create();
					if (cell) {
						cell->setCellScale(scaleRate);
						height = cell->getContentImageSize().height;
						height = height * scaleRate;
						space = (nodeSize.height - height * 9.5) / 10.0f;
						space = space > 0 ? space : 0;
						if (!m_openRecordAnimal.empty()) {
							cell->setCellIndex(m_openRecordAnimal.at(i));
						}
						else {
							cell->showLoopCellNode(false);
							//                            cell->setCellIndex(i);
						}
						Vec2 cellPos = Vec2(-nodeSize.width / 2, -nodeSize.height / 2 -20+ space * i + height * (i + 0.5));
						cell->showLightEffect(false);
						cell->setCellPosition(cellPos);
						_openRecord.openLogos.push_back(cell);
						_openRecord.openLogosPositions.push_back(cellPos);

						_openRecord.node->addChild(_openRecord.openLogos.at(i));

					}

				}
				if (_openRecord.openLogos.back()) {
					_openRecord.openLogos.back()->showLightRound(true);
				}
			}
		}
	}



	void GameTableUIV88::addLoopSprites() {
		if (_loopArea.loopCellPositions.empty()) {
			return;
		}
		for (int i = 0; i < _loopArea.loopCellPositions.size(); i++) {
			Vec2 pos = _loopArea.loopCellPositions.at(i);

			BBLoopSpriteCellV88* cell = BBLoopSpriteCellV88::create();
			if (cell) {
				cell->setCellPosition(pos + _loopArea.loopNode->getPosition() + WIN_SIZE / 2);
				cell->setCellIndex(i);
				//                cell->setIconIndex(i % Loop_Kind_Count);
				addChild(cell, ZOrder_Loop);
				_loopArea.loopSprites.push_back(cell);
			}
		}
	}


	void GameTableUIV88::leaveTheGame() {
		HNRoomLogic::getInstance()->closeRoom();
		PlatformState* state = PlatformState::create();
		state->setType(GAMELIST);
		StateManager::getInstance()->setState(state);
	}

	//第一次进入游戏基础数据同步
	void GameTableUIV88::syncBasicDataFirstTime(BB::SyncDataStruct *msg) {
		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->gameState;
		if (_playerHead) {
			auto user = _GameTableLogic->getUserInfoBySeatNo(msg->bDeskStation);
			if (user) {
				_playerHead->setPlayerIcon(user);
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
				setButtonEnabled(_bankerArea.requestBankerBtn, msg->i64MyMoney >= m_i64BankerNeedMoney, false);
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
	void GameTableUIV88::resumeTheGameFree(GameStation_Base* pcene, BYTE gameStation) {
		if (pcene == nullptr) {
			return;
		}
		m_GameStatus = pcene->bGameStation;
		//banker info
		if (pcene->bCurrentBankerID > 0) {
			if (_bankerArea.requestBankerBtn) {
				//            _bankerArea.requestBankerBtn->setVisible(true);
				setButtonEnabled(_bankerArea.requestBankerBtn, pcene->i64MyMoney >= m_i64BankerNeedMoney, false);
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
	}

	void GameTableUIV88::resumeTheGameBet(GameStation_Bet* pcene, BYTE gameStation) {
		if (pcene == nullptr) {
			return;
		}
		m_GameStatus = pcene->bGameStation;
		//banker info
		if (pcene->bCurrentBankerID > 0) {
			if (_bankerArea.requestBankerBtn) {
				//            _bankerArea.requestBankerBtn->setVisible(true);
				setButtonEnabled(_bankerArea.requestBankerBtn, pcene->i64MyMoney >= m_i64BankerNeedMoney,false);
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

		setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_XIAZHU, pcene->iCurrentStateLeftTimes);

		//转盘上随机显示一个光环，其最后结束的位置就是下局转盘开始时动物的起始位置
		schedule(schedule_selector(GameTableUIV88::updateRandomBlinkLightFree), 1.5f);
	}

	void GameTableUIV88::resumeTheGameKaiJiang(GameStation_Kaijiang* pcene, BYTE gameStation) {
		if (pcene == nullptr) {
			return;
		}
		m_GameStatus = pcene->bGameStation;
		if (pcene->bCurrentBankerID > 0) {
			if (_bankerArea.requestBankerBtn) {
				//            _bankerArea.requestBankerBtn->setVisible(true);
				setButtonEnabled(_bankerArea.requestBankerBtn, pcene->i64MyMoney >= m_i64BankerNeedMoney, false);
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
	
		setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_KAIJIANG, pcene->iCurrentStateLeftTimes);
	}

	void GameTableUIV88::resumeTheGameOver(GameStation_Base* pcene, BYTE gameStation) {
		if (pcene == nullptr) {
			return;
		}
		m_GameStatus = pcene->bGameStation;
		if (pcene->bCurrentBankerID > 0) {
			if (_bankerArea.requestBankerBtn) {
				//            _bankerArea.requestBankerBtn->setVisible(true);
				setButtonEnabled(_bankerArea.requestBankerBtn, pcene->i64MyMoney >= m_i64BankerNeedMoney,false);
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
		setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_SETTLEMENT, pcene->iCurrentStateLeftTimes);
	}

	//////////////////////////////////////////////////////////////////////////

//	void GameTableUIV88::onBtnClick(Ref* pSender, Control::EventType event) {
//		//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//		Node* btn = (Node*)pSender;
//		switch (btn->getTag()) {
//		case em_toolbarBtn: {
//								if (0 == _toolbarBg->getName().compare("0")) {
//									_toolbarBg->setName("1");
//									_toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height)));
//									this->getChildByTag(em_toolbarBtn)->setVisible(false);         //whao
//								}
//		}
//			break;
//		case em_toolbarSetBtn: {
//								   if (this->getChildByName("helpLayer")) {
//                                       ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
////									   this->getChildByName("helpLayer")->removeFromParent();
//								   }
//
//								   if (0 == _toolbarBg->getName().compare("1")) {
//									   _toolbarBg->setName("0");
//
//									   _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//										   CallFunc::create([this]() {
//										   this->getChildByTag(em_toolbarBtn)->setVisible(true);
//									   })));
//								   }
//								   if (this->getChildByName("setLayer")) {
//									   BRNN::Actions::actionOut(this->getChildByName("setLayer"));
//									   return;
//								   }
//								   auto setLayer = BRNNSetLayerV88::create();
//								   setLayer->setName("setLayer");
//								   setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//								   setLayer->showSet(this, ZOrder_Tool+5);
//								 /*  setLayer->setSoundCallback([]() {
//									   HNAudioEngine::getInstance()->playBackgroundMusic(BB_BG_MUSIC);
//								   });*/
//		}
//			break;
//		case  em_toolbarHelpBtn: {
//									 if (this->getChildByName("setLayer")) {
//
//										 this->getChildByName("setLayer")->removeFromParent();
//									 }
//
//									 if (0 == _toolbarBg->getName().compare("1")) {
//										 _toolbarBg->setName("0");
//
//										 _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//											 CallFunc::create([this]() {
//											 this->getChildByTag(em_toolbarBtn)->setVisible(true);
//										 })));
//									 }
//									 if (this->getChildByName("helpLayer")) {
//                                         ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
//										 //(dynamic_cast< BRNN::BRNNHelpLayer*>(this->getChildByName("helpLayer")))->actionOut();
////										 BRNN::Actions::actionOut(this->getChildByName("helpLayer"));
//										 return;
//									 }
//
//
//            auto helpLayer = HN::HelpLayer::create();
//            helpLayer->setContentOnView("v66/common/help/BB.png");
//            helpLayer->setName("helpLayer");
//            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//            helpLayer->setOrignalPosition();
//            helpLayer->moveSelfInWithAction();
//            addChild(helpLayer, ZOrder_Tool+5);
//            
////									 auto helpLayer = BB::BBHelpLayerV88::create();
////									 helpLayer->setName("helpLayer");
////									 addChild(helpLayer, ZOrder_Tool+5);
////									 helpLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
////									 BRNN::Actions::actionIn(helpLayer);
//
//										
//		}
//			break;
//		case em_toolbarLeaveBtn: {
//									 if (0 == _toolbarBg->getName().compare("1")) {
//										 _toolbarBg->setName("0");
//
//										 _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//											 CallFunc::create([this]() {
//											 this->getChildByTag(em_toolbarBtn)->setVisible(true);
//										 })));
//									 }
//									 if (!RoomLogic()->connected()) {
//										 leaveTheGame();
//									 }
//									 else {
//										 if (m_iSelfIsBanker) {
//											 //是否是庄
//											 if (m_iSelfIsBanker) {
//												 auto layer = GamePromptLayer::create();
//												 layer->showPrompt(GBKToUtf8("您需连庄五局后才可以下庄离开！"));
//												 return;
//											 }
//										 }
//										 else {
//											 // 游戏不在空闲时期，不允许离开
//											 bool bHasNote = false;
//											 for (int i = 0; i < LoopK_A_BetA_Count; i++) {
//												 if (m_i64OwnNotes[i] > 0) {
//													 bHasNote = true;
//												 }
//											 }
//											 if (bHasNote) {
//												 auto layer = GamePromptLayer::create();
//												 layer->showPrompt(GBKToUtf8("游戏过程中不允许离开！"));
//												 return;
//											 }
//											 else {
//												 _GameTableLogic->sendUserUp();
//											 }
//										 }
//									 }
//		}
//			break;
//		case em_toolbarNextOutBtn: {
//									   /*  if (0 == _toolbarBg->getName().compare("1")) {
//											 _toolbarBg->setName("0");
//
//											 _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//											 CallFunc::create([this]() {
//											 this->getChildByTag(em_toolbarBtn)->setVisible(true);
//											 })));
//											 }*/
//									   if (btn->getUserData() == (void*)true) {
//										   btn->setUserData((void*)false);
//										   btn->getChildByName("checkSprite")->setVisible(false);
//										   m_bNextOut = false;
//									   }
//									   else if (btn->getUserData() == (void*)false) {
//										   btn->setUserData((void*)true); 
//										   btn->getChildByName("checkSprite")->setVisible(true);
//										   m_bNextOut = true;
//									   }
//		}
//			break;
//		}
//	}

	bool GameTableUIV88::onTouchBegan(Touch* touch, Event* event) {
//		Point point = touch->getLocation();
//		point = _betArea.tableBg->convertToNodeSpace(point);
//
//		if (0 == _toolbarBg->getName().compare("1")) {
//			_toolbarBg->setName("0");
//
//			_toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//				CallFunc::create([this]() {
//				this->getChildByTag(em_toolbarBtn)->setVisible(true);
//			})));
//		}

		return true;
	}

	void GameTableUIV88::onEnter() {
		HNGameUIBase::onEnter();
		HNAudioEngine::getInstance()->playBackgroundMusic(BB_BG_MUSIC, true);


		_fScaleX = WIN_SIZE.width / DESIGN_SIZE.width;
		_fScaleY = WIN_SIZE.height / DESIGN_SIZE.height;

		//触摸
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
		myListener->setSwallowTouches(false);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);


		createRootLayer();
		addLoopSprites();
		createPlayerInfon();
		createMoneyArea();
		createNoteLabels();
		//createCarLayer();
		createState();
		createGameStateLayer();
		createGameSettlementLayer();
		createToolBarUI();
		createClock();

		auto scanner = Scanner::create();
		this->addChild(scanner, 100000);
		scanner->starPlayerEffect();
		scanner->setPosition(250, 60);
		//scanner->setVisible(false);

		_GameTableLogic = new GameTableLogic(this, _bDeskIndex, _bAutoCreate, PLAYER_COUNT);
		_GameTableLogic->sendGameInfo();

	}

	void GameTableUIV88::onExit() {
		HNGameUIBase::onExit();
	}

	void GameTableUIV88::onExitTransitionDidStart() {
		HNGameUIBase::onExitTransitionDidStart();
		HNAudioEngine::getInstance()->stopBackgroundMusic();
	}

	void GameTableUIV88::updatePlayerInfo() {

	}

	void GameTableUIV88::addUser(BYTE seatNo, bool bMe) {
		UserInfoStruct * user = _GameTableLogic->getUserBySeatNo(seatNo);

		if (bMe && user) {
			_playerHead->setPlayerName(user->nickName);
			_playerHead->setPlayerIcon(user);
			_playerHead->setPlayerMoney(user->i64Money);
		}
		//auto viewSeatNo = _GameTableLogic->logicToViewSeatNo(seatNo);

		//m_vPlayerHead.at(viewSeatNo)->setVisible(true);
		//m_vPlayerHead.at(viewSeatNo)->setHead(user->bLogoID);
		//m_vPlayerHead.at(viewSeatNo)->setNick(user->nickName);
		//m_vPlayerHead.at(viewSeatNo)->setGold(user->i64Money);
	}

//    void GameTableUIV88::updateClock(float dt) {
//        if (m_fTimeCount <= 0) {
//            killTimer();
//        }
//        m_fTimeCount -= floor(dt);
//    }

	void GameTableUIV88::setTimer(BYTE bChairID, int iTimerID, float fTime) {
		m_bTimerChairID = bChairID;
		m_iTimerID = iTimerID;
		m_fClockTime = fTime;
//        m_fTimeCount = fTime - 1;
		m_bDoTimeOutCallback = true;

		if (m_clockObj && m_fClockTime >= 1) {
			m_clockObj->start(m_fClockTime);
		}
	}

	void GameTableUIV88::killTimer() {
		if (m_clockObj) {
			m_clockObj->stop();
//            m_clockObj->setVisible(false);
		}
//        this->unschedule(schedule_selector(GameTableUIV88::updateClock));

	}
    
    void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
        if (bVisible) {
            if (!m_Notice) {
                m_Notice = GameNotice::create();
                m_Notice->setLocalZOrder(NOTICE_ZORDER);
                m_Notice->setName("GameNotice");
            }
            m_Notice->openNotic(this, 10000, message, nCount, nType);
        }
    }

	
	//按钮回调
	////////////////////////////////////////////////////////////////////////////

	//
	void GameTableUIV88::requestBankerCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}
		auto btn = (AUIButton*)pSender;
		if (btn) {
			if (btn->getTag() == em_reqBankerBtnTag) {
				setButtonEnabled(btn, false,false);
				_GameTableLogic->sendRequestBanker(true);

			}
		}
	}

	//
	void GameTableUIV88::betAreaButtonsCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
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
	void GameTableUIV88::betMoneyCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
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

	void GameTableUIV88::betClearCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
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

	void GameTableUIV88::betRebetCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
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

	

	//////////////////////////////////////////////////////////////////////////
	//动画

	//只展示筹码飞过去的动画，金额更新动画放到下注消息回调中
	void GameTableUIV88::betMoneyToAreaAnimation(int betMoneyIndex, int betAreaIndex, bool isMe/* =false */) {
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

		char path[100] = { 0 };
		sprintf(path, Bet_Note_Res_V88, betMoneyIndex);
		Sprite* sprite = Sprite::create(path);
		Vec2 startPos = Vec2::ZERO;
		Vec2 endPos = m_betAreaPosition.at(betAreaIndex);
		startPos = isMe == true ? m_notesPosition.at(betMoneyIndex) : Vec2(WIN_SIZE.width / 10.0f * (rand() % 8 + 2), -50.0f);
		if (sprite) {
			sprite->setScale(0.6f);
			sprite->setPosition(startPos);
			endPos += Vec2((rand() % 10) * 5 * (rand() % 3 - 1), (rand() % 10) * 5 * (rand() % 3 - 1));
			Sequence* seq = Sequence::create(MoveTo::create(0.4f, endPos), nullptr);
			sprite->runAction(seq);
			addChild(sprite, ZOrder_BetButtonLabel - 1);
			m_AllNotesMoveAnimations.at(betAreaIndex).push_back(sprite);
		}
	}

	void GameTableUIV88::updateBetMoneySelectedState(int betMoneyIndex) {
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







	void GameTableUIV88::showRunLoopAnimation(int endIndex, int loopTime, float delayTime) {
		unschedule(schedule_selector(GameTableUIV88::updateRandomBlinkLightFree));
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
		schedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition), 0.16);//前10个，慢速，0.16s一帧，共1.6s
	}

	//至少会转动4圈，有116-140个
	void GameTableUIV88::updateLoopSpritePosition(float dt) {
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
			unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
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
				unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
				return;
			}
		}
		//
		if (m_loopAnimationRemainTime == m_loopAnimationMaxTime - 9) {
			m_audioEffectID = HNAudioEngine::getInstance()->playEffect(BB_MAX_SPEED_LOOP_EFFECT);
			unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
			schedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition), (7.5f - 5.0f) * 0.9f / (m_loopAnimationMaxTime - 20));
		}
		else if (m_loopAnimationRemainTime == 15) {
			unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
			schedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition), 0.1f);//共1.2s
		}
		else if (m_loopAnimationRemainTime == 5) {
			m_audioEffectID = HNAudioEngine::getInstance()->playEffect(BB_SLOW_SPEED_LOOP_EFFECT);
			unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
			schedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition), 0.54f); //共2.7s
		}
	}

	//固定时间间隔随机闪烁，其最后停止的位置即为下局转盘开始的位置
	void GameTableUIV88::updateRandomBlinkLightFree(float dt) {
		if (m_GameStatus != GS_XIAZHU) {
			unschedule(schedule_selector(GameTableUIV88::updateRandomBlinkLightFree));
			return;
		}
		m_loopAnimationEndIndex = rand() % Loop_Sprite_Count;
		if (m_loopAnimationEndIndex < 0 || m_loopAnimationEndIndex >= Loop_Sprite_Count) {
			m_loopAnimationEndIndex = 0;
		}
		m_loopAnimationCurrentIndex = m_loopAnimationEndIndex;
		updateBlinkOpenAnimalLight();
	}

	////开奖后展示开奖动物相关结果动画，如下注区域闪烁，开的动物光环闪烁
	//void GameTableUIV88::showResultAnimalAnimation(BYTE animalKind, float showTime) {
	//    m_openResultAnimalKind.push_back(animalKind);
	//    float singleTime = 0.8f;
	//    schedule(schedule_selector(GameTableUIV88::updateResultAnimation), singleTime, (unsigned int)(floor(showTime / singleTime)), 0.1f);
	//
	//}
	//
	//void GameTableUIV88::updateResultAnimation(float dt) {
	//    if (m_openResultAnimalKind.empty()) {
	//        unschedule(schedule_selector(GameTableUIV88::updateResultAnimation));
	//        stopBlinkAnimalLight();
	//        return;
	//    }
	//    updateBlinkOpenAnimalLight();
	//
	//    for (int i = 0; i < m_openResultAnimalKind.size(); i++) {
	//        if (m_openResultAnimalKind.at(i) >= K_YZ && m_openResultAnimalKind.at(i) <= K_LY) {
	//            //飞禽类则飞禽也要展示
	//            m_openResultAnimalIndex.push_back(0);
	//            //
	//            BYTE index = getBetButtonIndexByKindID(m_openResultAnimalKind.at(i));
	//            if (index != INVALID_BYTE) {
	//                //其他类型
	//                m_openResultAnimalIndex.push_back(index);
	//            }
	//        } else if (m_openResultAnimalKind.at(i) >= K_SZ && m_openResultAnimalKind.at(i) <= K_TZ) {
	//            //走兽类则走兽也要展示
	//            m_openResultAnimalIndex.push_back(5);
	//            //
	//            BYTE index = getBetButtonIndexByKindID(m_openResultAnimalKind.at(i));
	//            if (index != INVALID_BYTE) {
	//                //其他类型
	//                m_openResultAnimalIndex.push_back(index);
	//            }
	//        } else if (m_openResultAnimalKind.at(i) == K_TP) {
	//            //通赔
	//            for (int i = 0; i < Bet_Area_Num; i++) {
	//                m_openResultAnimalIndex.push_back(i);
	//            }
	//        } else {
	//            //鲨鱼
	//            BYTE index = getBetButtonIndexByKindID(m_openResultAnimalKind.at(i));
	//            if (index != INVALID_BYTE) {
	//                //其他类型
	//                m_openResultAnimalIndex.push_back(index);
	//            }
	//        }
	//    }
	//    //
	//    if (!m_openResultAnimalIndex.empty()) {
	//        for (int i = 0; i < m_openResultAnimalIndex.size(); i++) {
	//            if (!_betArea.btnLights.empty() && m_openResultAnimalIndex.at(i) < _betArea.btnLights.size()) {
	//                if (_betArea.btnLights.at(m_openResultAnimalIndex.at(i))) {
	//                    Sequence* seq = Sequence::create(CallFunc::create([=] {
	//                        if (!m_openResultAnimalIndex.empty())
	//                            _betArea.btnLights.at(m_openResultAnimalIndex.at(i))->setVisible(false);
	//                    }), DelayTime::create(0.2f), CallFunc::create([=] {
	//                        if (!m_openResultAnimalIndex.empty())
	//                            _betArea.btnLights.at(m_openResultAnimalIndex.at(i))->setVisible(true);
	//                    }), DelayTime::create(0.28f), CallFunc::create([=] {
	//                        if (!m_openResultAnimalIndex.empty())
	//                            _betArea.btnLights.at(m_openResultAnimalIndex.at(i))->setVisible(false);
	//                    }), nullptr);
	//                    //
	//                    if (!m_openResultAnimalIndex.empty())
	//                        _betArea.btnLights.at(m_openResultAnimalIndex.at(i))->runAction(seq);
	//                }
	//            }
	//        }
	//    } else {
	//        unschedule(schedule_selector(GameTableUIV88::updateResultAnimation));
	//        stopBlinkAnimalLight();
	//    }
	//}

	//开启的动画位置的光环交替闪烁
	void GameTableUIV88::updateBlinkOpenAnimalLight() {
		if (m_loopAnimationEndIndex < 0 || m_loopAnimationEndIndex >= Loop_Sprite_Count) {
			m_loopAnimationCurrentIndex = 0;
			m_loopAnimationEndIndex = 0;
			return;
		}
		if (!_loopArea.loopSprites.empty()) {
			if (_loopArea.loopSprites.at(m_loopAnimationEndIndex)) {
				_loopArea.loopSprites.at(m_loopAnimationEndIndex)->stopAllActions();
				Sequence* seq = Sequence::create(CallFunc::create([this](){
					stopAllLoopAnimation();
				}), DelayTime::create(0.2f), CallFunc::create([this](){
					startLoopAnimationAtIndex(m_loopAnimationEndIndex);
				}), DelayTime::create(0.28f), CallFunc::create([this](){
					stopAllLoopAnimation();
				}), nullptr);
				if (seq) {
					_loopArea.loopSprites.at(m_loopAnimationEndIndex)->runAction(seq);
				}
			}
		}
	}


	void GameTableUIV88::showOpenAnimalAnimation(int index) {
		if (index < 0 || index >= Loop_Sprite_Count) {
			return;
		}
		//    playOpenEffectAudio(Area_Kind[index]);
		//    if (Area_Kind[index] == K_TS) {
		//        //通杀
		//        tongpeiTongshaAnimation(false);
		//    } else if (Area_Kind[index] == K_TP) {
		//        //通赔
		//        tongpeiTongshaAnimation(true);
		//    } else if (Area_Kind[index] == K_YZ) {
		//        playAnimalsAnimation(FQZS_ANIMATION_YZ_CSB, "swallow");
		//    } else if (Area_Kind[index] == K_GZ) {
		//        playAnimalsAnimation(FQZS_ANIMATION_GZ_CSB, "dove");
		//    } else if (Area_Kind[index] == K_KQ) {
		//        playAnimalsAnimation(FQZS_ANIMATION_KQ_CSB, "peacock");
		//    } else if (Area_Kind[index] == K_LY) {
		//        playAnimalsAnimation(FQZS_ANIMATION_LY_CSB, "eagle");
		//    } else if (Area_Kind[index] == K_SZ) {
		//        playAnimalsAnimation(FQZS_ANIMATION_SZ_CSB, "lion");
		//    } else if (Area_Kind[index] == K_XM) {
		//        playAnimalsAnimation(FQZS_ANIMATION_XM_CSB, "panda");
		//    } else if (Area_Kind[index] == K_HZ) {
		//        playAnimalsAnimation(FQZS_ANIMATION_HZ_CSB, "monkey");
		//    } else if (Area_Kind[index] == K_TZ) {
		//        playAnimalsAnimation(FQZS_ANIMATION_TZ_CSB, "rabbit");
		//    } else if (Area_Kind[index] == K_SY) {
		//        playAnimalsAnimation(FQZS_ANIMATION_SY_CSB, "blueshark");
		//    } else if (Area_Kind[index] == K_JS) {
		//        playAnimalsAnimation(FQZS_ANIMATION_JS_CSB, "goldenshark");
		//    }
	}

	void GameTableUIV88::stopAllLoopAnimation() {
		if (_loopArea.loopSprites.empty()) {
			return;
		}
		for (int i = 0; i < _loopArea.loopSprites.size(); i++) {
			if (_loopArea.loopSprites.at(i)) {
				_loopArea.loopSprites.at(i)->showLightEffect(false);
			}
		}
	}

	void GameTableUIV88::startLoopAnimationAtIndex(int index) {
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

	void GameTableUIV88::playOpenLogoAnimationAtIndex(int index) {
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

	//
	

	//传入新的开局记录数组，先将数组的值保存，然后再更新UI显示
	void GameTableUIV88::updateStateImage(int iHistory[]) {

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
				}
				else {
					_openRecord.openLogos.at(i)->showLightRound(true);
					//                animationMoveOpenItemToStateLabel(kinds.at(i));
				}
			}
		}

	}
    
    //
    void GameTableUIV88::playBigWinAnimation(int gameID, int openLogo, LLONG money) {
        if (openLogo >= BetA_Lam_Max && openLogo <= BetA_Ben_Max) {
            int mul = g_iBetRate[openLogo];
            auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
            addChild(colorLayer, ZOrder_SettlementLayer+1);
            colorLayer->runAction(Sequence::create(DelayTime::create(2.5f), RemoveSelf::create(), nullptr));
            auto node = CSLoader::createNode("v88/animation/common/jackpotting/jackpotting.csb");
            if (node) {
                node->setPosition(WIN_SIZE/2);
                addChild(node, ZOrder_SettlementLayer+1);
                auto action = CSLoader::createTimeline("v88/animation/common/jackpotting/jackpotting.csb");
                if (action) {
                    node->runAction(action);
                    action->play("jackpotting", false);
                }
                //
                char gameNamePath[64] = {0};
                sprintf(gameNamePath, "v66/common/bigwin/%d.png", gameID);
                std::string logoName = "";
                if (openLogo == BetA_Lam_Max) {
                    logoName = "法拉利";
                }
                else if (openLogo == BetA_BMW_Max) {
                    logoName = "兰博基尼";
                }
                else if (openLogo == BetA_Ben_Max) {
                    logoName = "玛莎拉蒂";
                }
                char tipString[256] = {0};
                sprintf(tipString, GetText("BIG_WIN_TIP_BB"), logoName.c_str(), mul);
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
                            moneyAtals->setProperty(to_string(money*mul), "v66/common/zi/111.png", 55, 78, "0");
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
    }

	//将开奖的icon移动到走势图最后的位置，endIdx为最后一个
	void GameTableUIV88::animationMoveOpenItemToStateLabel(int endIdx) {
		Vec2 startPos = Vec2::ZERO;
		Vec2 endPos = Vec2::ZERO;
		int lastIndexInState = 0;
		auto cell = BBLoopSpriteCellV88::create();
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
						lastIndexInState = (int)_openRecord.openLogos.size();
						endPos = _openRecord.openLogosPositions.at(lastIndexInState);
					}

				}
				else {
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
					auto newCell = BBLoopSpriteCellV88::create();
					if (newCell) {
						newCell->setCellIndex(endIdx);
						newCell->setPosition(_openRecord.openLogosPositions.at(lastIndexInState));
						newCell->setCellScale(0.7f);
						newCell->showLightRound(true);
					}
				}
			}), nullptr);

			cell->runAction(seq);
			//            }
			//        }
		}



	}


	//车灯动画
	void GameTableUIV88::animationOpenCarLight() {
		if (_car.carAction) {
			_car.carAction->play("car", false);
		}
		if (_car.carNode) {
			_car.carNode->setVisible(true);
		}
	}

	void GameTableUIV88::animationStopCarLight() {
		if (_car.carNode) {
			_car.carNode->setVisible(false);
		}
		if (_car.carAction) {
			_car.carAction->stop();
		}
	}


	


	//玩家坐下(非自己)
	void GameTableUIV88::userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
		addUser(userSit->bDeskStation, userSit->bDeskStation == _GameTableLogic->getMySeatNo());
	}

	//玩家站起
	void GameTableUIV88::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
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

	void GameTableUIV88::onGameFree(BB::GameFreeStruct *msg) {
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
					_playerHead->setPlayerIcon(info);
				}
			}
		}

		setTimer(_GameTableLogic->getMySeatNo(), CLIENT_TIMER_FREE, msg->bCurrentStateLeftTimes);
	}

	void GameTableUIV88::onConfirmBanker(BB::GameBeganAndConfirmBanker *msg) {
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
				setButtonEnabled(_bankerArea.requestBankerBtn, true,false);
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
			}
			else {
				sprintf(name, GetText("BB_ZHUANG_LIST_COUNT"), msg->iBankerListWaitNumber);
				_bankerArea.requestBankerCount->setString(name);
			}
		}
		//如果自己是庄家，限制下注等
		if (msg->isBanker) {
			m_iSelfIsBanker = true;
			disableAllBetMoney();
			setButtonEnabled(_betArea.reBetBtn, false,false);
			setButtonEnabled(_betArea.clearBtn, false,false);
			//xutouImg->setEnabled(false);
			//clearImg->setEnabled(false);
			
		}
		else {
			m_iSelfIsBanker = false;
		}






	}

	void GameTableUIV88::onGameBeginNote(BB::GameBeganBet *msg) {
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
				setButtonEnabled(_betArea.reBetBtn, true,false);
			}
			setButtonEnabled(_betArea.clearBtn, true, false);
			//xutouImg->setEnabled(true);
			//clearImg->setEnabled(true);
		}
		else {
			disableAllBetMoney();
			setButtonEnabled(_betArea.reBetBtn, false, false);
			setButtonEnabled(_betArea.clearBtn, false, false);
			//xutouImg->setEnabled(false);
			//clearImg->setEnabled(false);
		}


		//转盘上随机显示一个光环，其最后结束的位置就是下局转盘开始时动物的起始位置
		schedule(schedule_selector(GameTableUIV88::updateRandomBlinkLightFree), 1.5f);

	}

	void GameTableUIV88::onGameNoteResponse(BB::GameXiaZhuResponse *msg) {

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
		}
		else {
			//其他玩家下注

		}




		//下注类型
		//msg->iType;

		//自己下的总注
		//msg->iOwnAllCountNote;

		//最近一笔主值
		//msg->iNoteNum;
	}

	void GameTableUIV88::onGameClearResponse(BB::GameClearResponse *msg) {
		updateBetMoneyEnable();
	}

	void GameTableUIV88::onGameReBetResponse(BB::GameReBetResponse *msg) {
		updateBetMoneyEnable();
	}

	void GameTableUIV88::onGameRequestBankerResponse(BB::GameRequestBankerResponse *msg) {

		if (msg == nullptr) {
			return;
		}
		if (msg->requestSucceed) {
			//按钮变成下庄按钮
			_bankerArea.isShangZhuang = false;//显示已申请
			if (_bankerArea.requestBankerBtn) {
				_bankerArea.requestBankerBtn->setBright(false);
			}
			if (_gameState) {
				_gameState->updateGameStateContent(GSTATE_SHANGZHUANG_SUCCEED);
			}
		}
		else {
			_bankerArea.isShangZhuang = true;
			if (_bankerArea.requestBankerBtn) {
				setButtonEnabled(_bankerArea.requestBankerBtn, true,false);
			}
			if (_gameState) {
				_gameState->updateGameStateContent(GSTATE_SHANGZHUANG_FAIL_LACK_MONEY);
			}
		}

		if (_bankerArea.bankerName) {
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
			}
			else {
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

	void GameTableUIV88::onGameNoteFinish(BB::GameXiaZhuFinish *msg) {
		m_audioEffectID = HNAudioEngine::getInstance()->playEffect(BB_STOP_NOTE_EFFECT);
		_BetSingleOperate.moneyIndex = -1;
		_BetSingleOperate.betAreaIndex = -1;

		m_GameStatus = msg->gameState;

		setButtonEnabled(_betArea.clearBtn, false, false);
		setButtonEnabled(_betArea.reBetBtn, false, false);
		//xutouImg->setEnabled(false);
		//clearImg->setEnabled(false);

		updateBetMoneyEnable();
		updateAllNoteButtonsState(false);
		updateGameState(m_GameStatus);

	}


	void GameTableUIV88::onGameOpenLogo(GameLoopResult* msg) {
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
	void GameTableUIV88::onGameSettlementMsg(GameSettlementMsg* msg) {
		if (msg == nullptr) {
			return;
		}

		animationStopCarLight();

		//刷新纪录
		updateStateImage(msg->iHistory);

		if (_playerHead) {
			_playerHead->setPlayerMoney(msg->i64UserMoney);
		}
        
        auto coin = FloatCoin::create();
        if (coin) {
            coin->openParent(this, Vec2(250, 120), ZOrder_MyInfo);
            coin->updateCoin(msg->i64RealWinMoney);
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
        
        //big win animation
        int logoType = g_Logos[msg->openLogoIndexAtLoop];
        if (m_i64OwnNotes[logoType] > 0 && (logoType >= BetA_Lam_Max && logoType <= BetA_Ben_Max)) {
            playBigWinAnimation(BB_ID, logoType, m_i64OwnNotes[logoType]);
        }
	}

	void GameTableUIV88::onGameTipExit(tag_s2c_tip_exit* msg) {
		if (msg == nullptr) {
			return;
		}

		tipMsg = msg;
		//不退出
		if (msg->bIsExit == 0) {
			return;
		}
		else {
			auto layer = GamePromptLayer::create();
			if (layer) {
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
			}
		}
		
	}

	void GameTableUIV88::onGameNotify(tag_s2c_notify* msg) {
		if (msg->i64WinMoney < 0) return;
		char str[128] = { 0 };
		if (msg->bDeskID == INVALID_BYTE) {
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
				}
				else if (msg->bGameID == 2) {
					sprintf(str, "%s %s", GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName));
					label1->setString(ForceGBKToUtf8(str));

					sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName));
				}
				else if (msg->bGameID == 3) {
					sprintf(str, "%s %s", GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName));
					label1->setString(ForceGBKToUtf8(str));

					sprintf(str, GetText("NN_Reward_Notify"), ForceGBKToUtf8(msg->cNickName), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName));
				}
				else {
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
		}
		else {
			if (msg->bGameID == 1) {
				sprintf(str, GetText("NN_Catch_Bull_Notify"), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName), msg->bDeskID + 1, ForceGBKToUtf8(msg->cNickName), msg->i64WinMoney);
			}
			else if (msg->bGameID == 2) {
				sprintf(str, GetText("NN_Catch_Bull_Notify"), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName), msg->bDeskID + 1, ForceGBKToUtf8(msg->cNickName), msg->i64WinMoney);
			}
			else if (msg->bGameID == 3) {
				sprintf(str, GetText("NN_Catch_Bull_Notify"), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName), msg->bDeskID + 1, ForceGBKToUtf8(msg->cNickName), msg->i64WinMoney);
			}
			else {
				return;
			}

			showNotice(str, 1, 0);
		}
	}

	//
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

	void GameTableUIV88::initData() {
		//m_vGameStatePosition.clear();
	}

	void GameTableUIV88::resetNewGame() {
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
		//    m_iPrevHasNote = 0;	//重置上局下注信息，等待服务端重新通知
		//
		//    memset(m_i64OwnNotes, 0, sizeof(m_i64OwnNotes));
		//    memset(m_i64AreaNotes, 0, sizeof(m_i64AreaNotes));
		//    //////////////
		//    stopAnimalsAnimation();
		m_iPrevHasNote = 0;	//重置上局下注信息，等待服务端重新通知
		//m_iSelfIsBanker = false;

		memset(m_i64OwnNotes, 0, sizeof(m_i64OwnNotes));
		memset(m_i64AreaNotes, 0, sizeof(m_i64AreaNotes));

		this->unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
		//    this->unschedule(schedule_selector(GameTableUIV88::updateResultAnimation));
		this->unschedule(schedule_selector(GameTableUIV88::updateRandomBlinkLightFree));
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
		memset(m_iBankerNickName, 0, sizeof(m_iBankerNickName));
	}

	void GameTableUIV88::resetUINewGame() {
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

	void GameTableUIV88::resetData() {
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
//        m_ClockBg = nullptr;
//        m_ClockTimeLabel = nullptr;
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
//        m_fTimeCount = 0;
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

		m_clockObj = nullptr;
		m_clockPos = Vec2(-247.8, 214);
		//

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
//        m_fTimeCount = 0;
		m_audioEffectID = 0;

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

	}

	void GameTableUIV88::resetAllNotesAnimationAndClean() {
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
	void GameTableUIV88::setButtonEnabled(ui::Button* pButton, bool enabled, bool IsSetOpacity) {
		if (pButton != nullptr) {

			if (IsSetOpacity)
			{
				GLubyte opacity = enabled ? 255 : 125;
				pButton->setTouchEnabled(enabled);
				pButton->setOpacity(opacity);
			}
			else
			{
				pButton->setEnabled(enabled);
			}


		}
	}

	void GameTableUIV88::updateAllNoteButtonsState(bool enable) {
		for (Button* money : _betArea.betMoneyBtn) {
			if (money) {
				setButtonEnabled(money, enable,true);
			}
		}
		if (!enable) {
			for (int i = 0; i < Bet_Coin_Count; i++) {
				_betArea.betMoneyLight[i]->setVisible(false);
			}
		}
	}

	void GameTableUIV88::updateBetMoneyEnable() {
		for (int i = 0; i < Bet_Coin_Count; i++) {
			if (_GameTableLogic->getMyMoney() >= BetMoneyValues[i]) {
				_betData.betMoneyEnable[i] = true;
			}
			else {
				_betData.betMoneyEnable[i] = false;
				_betArea.betMoneyLight[i]->setVisible(false);
			}
			if (_betArea.betButtons.size() > i && _betArea.betButtons.at(i)) {
				setButtonEnabled(_betArea.betMoneyBtn.at(i), _betData.betMoneyEnable[i],true);
			}
		}
	}

	void GameTableUIV88::updateGameState(int gameState) {
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

	void GameTableUIV88::hideGameState(bool hideState) {
		if (_gameState) {
			//            _gameState->setNodeVisible(hideState);
			_gameState->hideStateLayer();
		}
	}

	void GameTableUIV88::onGameRobotNoteMsg(BB::GameRobotXiaZhuResponse *msg) {
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
	void GameTableUIV88::onGameNoteFail(BB::GameXiaZhuFaileMsg *msg) {
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

	void GameTableUIV88::onGameXiaZhuangResponse(BB::GameXiaZhuangResponse *msg) {
		if (msg == nullptr) {
			return;
		}
	}

	void GameTableUIV88::tipCallback() {
		if (tipMsg->bIsExit == 1) {
			//准备关闭掉
			//m_PrepareBtn->setVisible(false);

			auto layer = GamePromptLayer::create();
//			layer->setCloseExitBtn();
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
		}
		else {

		}
	}

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





	void GameTableUIV88::disableAllBetMoney() {
		for (int i = 0; i < Bet_Coin_Count; i++) {
			setButtonEnabled(_betArea.betMoneyBtn.at(i), false,true);
		}
	}


	void GameTableUIV88::updateTestContent(float dt) {
		//        if (_gameState) {
		//            int stateID = rand()%6;
		//            _gameState->updateGameStateContent(stateID);
		//        }
	}

	void GameTableUIV88::createClock() {
		m_clockObj = TimerObject::create();
		if (m_clockObj) {
			m_clockPos += WIN_SIZE / 2;
			m_clockObj->setPosition(m_clockPos);
//            m_clockObj->setScale(0.9f);
//            m_clockObj->setVisible(false);
			this->addChild(m_clockObj, ZOrder_Timer);
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
            runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                txtReward->setString(Tools::addComma(luckyMoney));
            }), DelayTime::create(retainTime), CallFunc::create([&](){
                this->removeChildByName("rewardResult");
            }), NULL));
        }
    }

}
