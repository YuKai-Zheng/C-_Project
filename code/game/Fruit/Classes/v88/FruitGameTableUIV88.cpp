#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
//#include "../../TTNN/Classes/v88/BRNNSetLayerV88.h"
//#include "../../TTNN/Classes/GameTable/BRNNActions.h"
#include "../../../../code/common/LibHNUI/UI/UIGoldRain.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNLobby/v66/ExtraReward.h"
#include "FruitGameTableUIV88.h"
#include "FruitGameTableLogic.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "FruitLoopSpriteCellV88.h"
#include "FruitGameStateV88.h"
//#include "FruitSettlementV88.h"
//#include "FruitHelpLayerV88.h"
#include "FruitBetMoneyCellV88.h"
#include "base/CCRef.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"

namespace Fruit {

#define WINSIZE Director::getInstance()->getWinSize()
#define NOTICE_ZORDER 100000

	////////////////////////////////////////////////////////////////////////////
	////常量

	enum Em_TableBtnTag_V88 {
		em_toolbarBtn = 1000,
		em_toolbarChangeTableBtn,
		em_toolbarSetBtn,
		em_toolbarHelpBtn,
		em_toolbarStandupBtn,
		em_toolbarLeaveBtn,
		em_toolbarNextOutBtn
	};

	enum Em_BetButtonTag_V88 {
		//bet
		em_betBarTag = 1100, //Bar
		em_betSevenTag,    //
		em_betStarTag,
		em_betWatermelonTag,
		em_betBellTag,
		em_betCherryTag,
		em_betOrangeTag,
		em_betAppleTag,
		em_btnSmallTag,
		em_btnBigTag,
		em_btnLeftTag,
		em_btnRightTag,
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
		ZOrder_LightBar = 345,
		ZOrder_Jackpot = 345,
		ZOrder_GameState = 375,
		ZOrder_Tool = 400,
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
	static const char* FRUIT_HISTORY_LABEL_CSB = "Games/31000502/Node_History_Label_V66.csb";
	static const char* FRUIT_ROOT_V88_CSB = "Games/31000502/Node_Root_V66.csb";

	static const char* FRUIT_LIGHTBAR_KAIJIANG = "v66/animation/fruitmachine/lightbar/lightbar_lottery/lightbar_lottery.csb";
	static const char* FRUIT_LIGHTBAR_DAIJI = "v66/animation/fruitmachine/lightbar/lightbar_daiji/lightbar_daiji.csb";
	static const char* FRUIT_BETSIZE = "v66/animation/fruitmachine/betsize/betsize.csb";

	//
	const static char* BETFONT_V88 = "Games/31000502/shuiguoji/23.png";

	//int posKindArr[24] = { -1, 0, 3, 1, 3, 7, 7, 0, 0, 2, 4, 4, -1, 0, 1, 1, 3, 6, 6, 0, 2, 2, 5, 5 };

	////////////////////////////////////////////////////////////////////////////
	//图片资源
	//background

	//tool
	static const char* FRUIT_TOOL_BAR_PATH = "v66/common/tool/%s";
	const static char* COSTTYPE_PATH_THOUSND = "Games/31000502/shuiguoji/35.png";
	const static char* COSTTYPE_PATH_ONE = "Games/31000502/shuiguoji/36.png";
	const static char* COSTTYPE_PATH_TEN = "Games/31000502/shuiguoji/37.png";	
	const static char* COSTTYPE_PATH_FIFTY = "Games/31000502/shuiguoji/38.png";
	const static char* COSTTYPE_PATH_HUNDREN = "Games/31000502/shuiguoji/39.png";
	////cloc

	//music
//    static const char* BB_BG_MUSIC = "v66/music/BB/BB_BGM.mp3";

//    static const char* SHUIGUOJI_JUMP_EFFECT = "Games/31000502/music/jump.wav";
	static const char* SHUIGUOJI_LUCKY_BIG_EFFECT = "Games/31000502/music/lucky.mp3";
	static const char* SHUIGUOJI_LUCKY_SMALL_EFFECT = "Games/31000502/music/lucky.mp3";
	static const char* SHUIGUOJI_LUCKY_FAIL_EFFECT = "Games/31000502/music/luck_fail.mp3";
	static const char* SHUIGUOJI_BUTTON_BET_EFFECT = "Games/31000502/music/Button_%d.mp3";//

	static const char* SHUIGUOJI_RESULT_SHUIGUO = "Games/31000502/music/Fruit_%d.mp3";//
	static const char* SHUIGUOJI_RESULT_EFFECT = "Games/31000502/music/C_%d.mp3";//
	///////////////////////////////
	static const char* SHUIGUOJI_BUTTON_B_S = "Games/31000502/music/ClickRight.mp3";//
	static const char* SHUIGUOJI_BUTTON_R_L = "Games/31000502/music/randombet.mp3";//

	static const char* SHUIGUOJI_BUTTON_WRONG = "Games/31000502/music/ClickWrong.mp3";//


	static const char* SHUIGUOJI_START_LOOP_EFFECT = "Games/31000502/music/sgj_start.mp3";
	static const char* SHUIGUOJI_MAX_SPEED_LOOP_EFFECT = "Games/31000502/music/sgj_loop.mp3";
	static const char* SHUIGUOJI_SLOW_SPEED_LOOP_EFFECT = "Games/31000502/music/sgj_end.mp3";

	static const char* SHUIGUOJI_GUESS_FAIL_EFFECT = "Games/31000502/music/fail.mp3";//猜大小失败
	static const char* SHUIGUOJI_GUESS_WIN_EFFECT = "Games/31000502/music/randombetwin.mp3";//猜大小
	static const char* SHUIGUOJI_GUESS_ING_EFFECT = "Games/31000502/music/randomspin.mp3";//猜大小进行中

	static const char* SHUIGUOJI_SHOOT_EFFECT = "Games/31000502/music/Shoot_%d.mp3";//猜大小进行中


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
    : m_bLuck(false)
		//    : m_iBetMoneyType(-1)
	{
		//    TextureCache::getInstance()->addImage(NN_GOLD_IMAGE);
	}

	GameTableUIV88::~GameTableUIV88() {
		//    this->unschedule(schedule_selector(GameTableUIV88::updateClock));
		this->unschedule(schedule_selector(GameTableUIV88::updateTip));
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
                    // 游戏不在空闲时期，不允许离开
                    if (m_GameStatus != GS_FREE_NOTE) {
      //                  auto layer = GamePromptLayer::create();
						//layer->showPrompt(GetText("limit_leave_in_game"));
						GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
						layer->setGamePromptTip(GetText("limit_leave_in_game"));
                        return;
                    }
                    else {
                        _GameTableLogic->sendUserUp();
                    }
                }
            });
            toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
                IGameSet* gameSet = GameSetFactory::createWithVersion();
                gameSet->setBgPath("");
                gameSet->open(this, Vec2(0, 0), zOrder+5, 0);
            });
            toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
                auto helpLayer = HN::HelpLayer::create();
                helpLayer->setContentOnView("v66/common/help/shuiguoji.png");
                helpLayer->setName("helpLayer");
                helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                helpLayer->setOrignalPosition();
                helpLayer->moveSelfInWithAction();
                addChild(helpLayer, zOrder+5);
            });
            toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
        }
	}




	void GameTableUIV88::createBetLb() {

		for (size_t i = 0; i < BetA_Count; i++)
		{
			auto cell = FruitBetMoneyCellV88::create();
			//cell->setPosition(Vec2(165+122*i,25));
			Vec2 pos = m_betAreaPosition.at(i);
			cell->setPosition(pos.x, pos.y - 25);
			cell->addBetMonye(0);
			addChild(cell, ZOrder_Loop);
			_betArea.betMoneySp.push_back(cell);
		}


	}
	void GameTableUIV88::createRootLayer() {
		auto root = CSLoader::createNode(FRUIT_ROOT_V88_CSB);
		if (root) {
			root->setPosition(WIN_SIZE / 2);
			addChild(root, ZOrder_Loop);
			auto bg = Sprite::create(GAME_BG_COMMON2_V66);
			addChild(bg, ZOrder_Bg);
			bg->setPosition(WIN_SIZE / 2);
			//
//			bg->setScaleX(_fScaleX);
//			bg->setScaleY(_fScaleY);

			auto tableAreBg = Sprite::create("Games/31000502/shuiguoji/1.png");
			tableAreBg->setAnchorPoint(Vec2(0.5, 0.5));
			addChild(tableAreBg, ZOrder_Bg);
			tableAreBg->setPosition(Vec2(-63.84 + WIN_SIZE.width / 2, 78.86 + WIN_SIZE.height / 2));


			//auto bg = dynamic_cast<Sprite*>(root->getChildByName("Sprite_bg"));
			if (bg) {
				_betArea.tableBg = bg;
			}
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
						//image->setVisible(false);
						pos = image->getPosition();
					}
					_loopArea.loopCellPositions.push_back(pos);
					memset(name, 0, sizeof(name));
				}
			}


			auto betNode = dynamic_cast<Node*>(root->getChildByName("Node_Bet"));
			if (betNode) {
				_betArea.betNode = betNode;
				char name[32] = { 0 };
				for (int i = 0; i < BetA_Count; i++) {
					sprintf(name, "btn_bet_%d", i);
					auto btn = dynamic_cast<Button*>(betNode->getChildByName(name));
					if (btn) {
						Vec2 size = Vec2(0, btn->getContentSize().height / 2);
						m_betAreaPosition.push_back(btn->getPosition() + betNode->getPosition() + root->getPosition() - size);
						btn->setTag(i + em_betBarTag);
						btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::betAreaButtonsCallBack, this));
						_betArea.betButtons.push_back(btn);
					}
					memset(name, 0, sizeof(name));
				}
			}


			auto betLableNode = dynamic_cast<Node*>(root->getChildByName("Node_bet_money"));
			if (betLableNode) {
				_betArea.moneyNode = betLableNode;
				char money[32] = { 0 };
				for (int i = 0; i < BetA_Count; i++) {
					sprintf(money, "Bet_Node_%d", i);
					auto sp = dynamic_cast<Node*>(betLableNode->getChildByName(money));
					if (sp) {
						_betArea.betButtonsVec.push_back(sp->getPosition());
					}

					memset(money, 0, sizeof(money));
				}
			}
			smallBtn = dynamic_cast<Button*>(root->getChildByName("btn_small"));
			smallBtn->setTag(em_btnSmallTag);
			smallBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::guessSizeCallBack, this));
			bigBtn = dynamic_cast<Button*>(root->getChildByName("btn_big"));
			bigBtn->setTag(em_btnBigTag);
			bigBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::guessSizeCallBack, this));
			////////////////////////////
			leftBtn = dynamic_cast<Button*>(root->getChildByName("btn_left"));
			leftBtn->setBright(false);
			leftBtn->setTouchEnabled(false);
			leftBtn->setTag(em_btnLeftTag);
			leftBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::chipInButtonCallBack, this));
			rightBtn = dynamic_cast<Button*>(root->getChildByName("btn_right"));
			rightBtn->setTag(em_btnRightTag);
			rightBtn->setBright(false);
			rightBtn->setTouchEnabled(false);
			rightBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::chipInButtonCallBack, this));

			////////////////
			startBtn = dynamic_cast<Button*>(root->getChildByName("btn_start"));
			startBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::starButtonCallBack, this));

			////////////////////
			jackpotText = dynamic_cast<Text*>(root->getChildByName("jackpotText"));
            jackpotText->setString("");
			myLastMoneyText = dynamic_cast<Text*>(root->getChildByName("myLastMoneyText"));
            myLastMoneyText->setString("");
			currentGetText = dynamic_cast<Text*>(root->getChildByName("currentGetText"));
            currentGetText->setString("");
			guessNumText = dynamic_cast<Text*>(root->getChildByName("guessNumText"));
            guessNumText->setString("");
			//////////////////////////////
			auto jsp = dynamic_cast<Sprite*>(root->getChildByName("spriteJackpot"));
			Vec2 size;

			if (jsp)
			{
				size = Vec2(jsp->getContentSize().width *0.3, 0);
				jackpotSpPoint = jsp->getPosition() + root->getPosition();
				jackpotSpPointLb = jsp->getPosition() + root->getPosition() + size;

			}
			auto guessSp = dynamic_cast<Sprite*>(root->getChildByName("spriteResult"));
			if (guessSp)
			{
				size = Vec2(guessSp->getContentSize().width *0.3, 0);
				guessResultSpPonint = guessSp->getPosition() + root->getPosition();
			}

			auto currentSp = dynamic_cast<Sprite*>(root->getChildByName("current_get"));
			if (currentSp)
			{
				size = Vec2(currentSp->getContentSize().width / 2 - 10, -currentSp->getContentSize().height / 4 + 5);
				cureentMoneySpPoint = currentSp->getPosition() + root->getPosition() + size;
			}
			auto lastSp = dynamic_cast<Sprite*>(root->getChildByName("current_last"));
			if (lastSp)
			{
				size = Vec2(lastSp->getContentSize().width / 2 - 10, -lastSp->getContentSize().height / 4 + 5);
				lastMoneySpPoint = lastSp->getPosition() + root->getPosition() + size;
			}

			auto costT = dynamic_cast<ImageView*>(root->getChildByName("costType"));
			if (costT)
			{
				costTypeImg = costT;
			}



		}
	}


	//void GameTableUIV88::createGameSettlementLayer() {
	//	//_gameSettlement = nullptr;
	//	FruitSettlementV88* settle = FruitSettlementV88::create();
	//	if (settle) {
	//		_gameSettlement = settle;
	//		_gameSettlement->setSettlementLayerPosition(Vec2(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.4f));
	//		_gameSettlement->hideSettlementLayer();
	//		addChild(_gameSettlement, ZOrder_SettlementLayer);
	//	}
	//}

	//游戏历史开局情况
	void GameTableUIV88::createState() {
		if (!_openRecord.node) {
			_openRecord.node = CSLoader::createNode(FRUIT_HISTORY_LABEL_CSB);
			if (_openRecord.node) {
				_openRecord.node->setPosition(Vec2(15, WIN_SIZE.height / 2));
				// addChild(_openRecord.node, ZOrder_CarLight + 1);

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
					auto cell = FruitLoopSpriteCellV88::create();
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

//    void GameTableUIV88::createClock() {
//        m_clockObj = BRNN::ClockV88::create();
//        if (m_clockObj) {
//            m_clockPos += WIN_SIZE / 2;
//            m_clockObj->setPosition(m_clockPos);
//            m_clockObj->setScale(0.9f);
//            m_clockObj->setVisible(false);
//            this->addChild(m_clockObj, ZOrder_Timer);
//        }
//    }

	void GameTableUIV88::addLoopSprites() {
		if (_loopArea.loopCellPositions.empty()) {
			return;
		}
		for (int i = 0; i < _loopArea.loopCellPositions.size(); i++) {
			Vec2 pos = _loopArea.loopCellPositions.at(i);

			FruitLoopSpriteCellV88* cell = FruitLoopSpriteCellV88::create();
			if (cell) {
				cell->setCellPosition(pos + _loopArea.loopNode->getPosition() + WIN_SIZE / 2);
				cell->setCellIndex(i);
				cell->setVisible(false);
				//                cell->setIconIndex(i % Loop_Kind_Count);
				addChild(cell, ZOrder_Loop);
				_loopArea.loopSprites.push_back(cell);
			}
		}
	}



	void GameTableUIV88::leaveTheGame() {
		HNRoomLogic::getInstance()->closeRoom();
		PlatformState* state = PlatformState::create();
		state->setType(ROOMLIST);
		StateManager::getInstance()->setState(state);
	}



	//////////////////////////////////////////////////////////////////////////

//	void GameTableUIV88::onBtnClick(Ref* pSender, Control::EventType event) {
//		//    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
//
//		Node* btn = (Node*)pSender;
//		switch (btn->getTag()) {
//		case em_toolbarBtn: {
//								if (0 == _toolbarBg->getName().compare("1")) {
//									_toolbarBg->setName("0");
//									_toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height)));
//									this->getChildByTag(em_toolbarBtn)->setVisible(false);         //whao
//								}
//		}
//			break;
//		case em_toolbarSetBtn: {
//								   if (0 == _toolbarBg->getName().compare("1")) {
//									   _toolbarBg->setName("0");
//
//									   _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 10)),
//										   CallFunc::create([this]() {
//										   this->getChildByTag(em_toolbarBtn)->setVisible(true);
//									   })));
//								   }
//								   if (this->getChildByName("setLayer")) {
//									   (dynamic_cast<BRNNSetLayerV88*>(this->getChildByName("setLayer")))->closeSet();
//									   return;
//								   }
//								   auto setLayer = BRNNSetLayerV88::create();
//								   setLayer->setName("setLayer");
//								   setLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
//								   setLayer->showSet(this, ZOrder_Tool + 10000);
//								
//		}
//			break;
//		case  em_toolbarHelpBtn: {
//									 if (0 == _toolbarBg->getName().compare("1")) {
//										 _toolbarBg->setName("0");
//
//										 _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 10)),
//											 CallFunc::create([this]() {
//											 this->getChildByTag(em_toolbarBtn)->setVisible(true);
//										 })));
//									 }
//									 if (this->getChildByName("helpLayer")) {
//                                         ((HelpLayer*)this->getChildByName("helpLayer"))->moveSelfOutWithAction();
////										 BRNN::Actions::actionOut(this->getChildByName("helpLayer"));
//										 return;
//									 }
////									 auto helpLayer = FruitHelpLayerV88::create();
////									 helpLayer->setName("helpLayer");
////									 addChild(helpLayer, ZOrder_Tool + 10000);
////									 helpLayer->setPosition(WIN_SIZE.width / 2, WIN_SIZE.height / 2);
////									 BRNN::Actions::actionIn(helpLayer);
//            auto helpLayer = HN::HelpLayer::create();
//            helpLayer->setContentOnView("v66/common/help/shuiguoji.png");
//            helpLayer->setName("helpLayer");
//            helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//            helpLayer->setOrignalPosition();
//            helpLayer->moveSelfInWithAction();
//            addChild(helpLayer, ZOrder_Tool + 10000);
//		}
//			break;
//		case em_toolbarLeaveBtn: {
//									 if (0 == _toolbarBg->getName().compare("1")) {
//										 _toolbarBg->setName("0");
//
//										 _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 10)),
//											 CallFunc::create([this]() {
//											 this->getChildByTag(em_toolbarBtn)->setVisible(true);
//										 })));
//									 }
//									 if (!RoomLogic()->connected()) {
//										 leaveTheGame();
//									 }
//									 else {
//										 // 游戏不在空闲时期，不允许离开
//										 if (m_GameStatus != GS_FREE_NOTE) {
//											 auto layer = GamePromptLayer::create();
//											 layer->showPrompt(GBKToUtf8("游戏过程中不允许离开！"));
//											 return;
//										 }
//										 else {
//											 _GameTableLogic->sendUserUp();
//										 }
//									 }
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
//			_toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height + 10)),
//				CallFunc::create([this]() {
//				this->getChildByTag(em_toolbarBtn)->setVisible(true);
//			})));
//		}

		return true;
	}

	void GameTableUIV88::onEnter() {
		HNGameUIBase::onEnter();
		// HNAudioEngine::getInstance()->playBackgroundMusic(BB_BG_MUSIC, true);

		_fScaleX = WIN_SIZE.width / DESIGN_SIZE.width;
		_fScaleY = WIN_SIZE.height / DESIGN_SIZE.height;

		//触摸
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
		myListener->setSwallowTouches(false);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

		//jackpotLba = LabelAtlas::create("", BETFONT_V88, 25, 34, '0');
		//resultLba = LabelAtlas::create("", BETFONT_V88, 25, 34, '0');
		//currentGetMoneyLba = LabelAtlas::create("", BETFONT_V88, 25, 34, '0');
		//myLastMoneyLba = LabelAtlas::create("", BETFONT_V88, 25, 34, '0');
		i64Reward = 0;
		isCanGuessB = false;
		createRootLayer();
		addLoopSprites();
		createBetLb();
		//createPlayerInfon();
		//createMoneyArea();
		// createNoteLabels();
		//        createCarLayer();
		//createState();
		//        createGameStateLayer();
		//createGameSettlementLayer();
//        createClock();
		createToolBarUI();

		//////////////////////////////
		//jackpotLba->setAnchorPoint(Vec2(1, 0.5));
		//jackpotLba->setPosition(jackpotSpPointLb);
		//addChild(jackpotLba, ZOrder_Loop + 10);
		////
		//resultLba->setAnchorPoint(Vec2(0.5, 0.5));
		//resultLba->setPosition(guessResultSpPonint);
		//addChild(resultLba, ZOrder_Loop + 10);
		////
		//currentGetMoneyLba->setPosition(cureentMoneySpPoint);
		//currentGetMoneyLba->setAnchorPoint(Vec2(1, 0.5));
		//addChild(currentGetMoneyLba, ZOrder_Loop + 10);
		////
		//myLastMoneyLba->setAnchorPoint(Vec2(1, 0.5));
		//myLastMoneyLba->setPosition(lastMoneySpPoint);
		//addChild(myLastMoneyLba, ZOrder_Loop + 10);


		//开奖待机
		lighBarDaijiNode = CSLoader::createNode(FRUIT_LIGHTBAR_DAIJI);
		addChild(lighBarDaijiNode, ZOrder_LightBar);
		lighBarDaijiNode->setPosition(Vec2(WIN_SIZE.width / 2+10, WIN_SIZE.height / 2-22));
		lighBarDaijiAct = CSLoader::createTimeline(FRUIT_LIGHTBAR_DAIJI);
		if (lighBarDaijiNode) {
			lighBarDaijiNode->runAction(lighBarDaijiAct);
			lighBarDaijiAct->play("lightbar_daiji", true);
		}

		//奖池待机
		//jackpotDaijiNode = CSLoader::createNode(FRUIT_JACKPOT_DAIJI);
		//addChild(jackpotDaijiNode, ZOrder_Jackpot);
		//jackpotDaijiNode->setPosition(jackpotSpPoint);
		//jackpotDaijiAct = CSLoader::createTimeline(FRUIT_JACKPOT_DAIJI);

		//if (jackpotDaijiNode) {
		//	jackpotDaijiNode->runAction(jackpotDaijiAct);
		//	jackpotDaijiAct->play("jackpot_daiji", true);
		//}

		//guessDaijiNode = CSLoader::createNode(FRUIT_JACKPOT_DAIJI);
		//addChild(guessDaijiNode, ZOrder_Jackpot);
		//guessDaijiNode->setPosition(Vec2(WIN_SIZE.width*0.615, WIN_SIZE.height*0.635));
		//guessDaijiAct = CSLoader::createTimeline(FRUIT_JACKPOT_DAIJI);

		//if (guessDaijiNode) {
		//	guessDaijiNode->runAction(guessDaijiAct);
		//	guessDaijiAct->play("jackpot_daiji", true);
		//}
		//guessDaijiNode->setScaleX(0.38);


		gameStateText = Text::create();
		gameStateText->setString("");
		gameStateText->setPosition(WIN_SIZE / 2);
		gameStateText->setFontSize(30);
		addChild(gameStateText, ZOrder_GameState);

		betSizeNode = CSLoader::createNode(FRUIT_BETSIZE);
		addChild(betSizeNode, 10000);
		betSizeNode->setVisible(false);
		betSizeNode->setPosition(Vec2(bigBtn->getPosition().x + WIN_SIZE.width / 2 - 55, bigBtn->getPosition().y + WIN_SIZE.height / 2 + 5));
		betSizeAct = CSLoader::createTimeline(FRUIT_BETSIZE);
		if (betSizeNode) {
			betSizeNode->runAction(betSizeAct);

		}
		////////////////

		auto scanner = Scanner::create();
		this->addChild(scanner, 100000);
		scanner->starPlayerEffect();
		scanner->setPosition(70, 240);
		/////////////////////
		_GameTableLogic = new GameTableLogic(this, _bDeskIndex, _bAutoCreate, PLAYER_COUNT);
		_GameTableLogic->sendGameInfo();

		smallBtn->setEnabled(false);
		bigBtn->setEnabled(false);
		guessNumText->setVisible(false);
		isGuessIng = false;


		//    schedule(schedule_selector(GameTableUIV88::updateTestContent), 2.5f);
	}

	void GameTableUIV88::onExit() {
		HNGameUIBase::onExit();
		if (m_audioEffectID > 0)
			HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);
	}

	void GameTableUIV88::onExitTransitionDidStart() {
		HNGameUIBase::onExitTransitionDidStart();
		HNAudioEngine::getInstance()->stopBackgroundMusic();

	}

	void GameTableUIV88::updatePlayerInfo() {

	}

	void GameTableUIV88::addUser(BYTE seatNo, bool bMe) {
//        UserInfoStruct * user = _GameTableLogic->getUserBySeatNo(seatNo);

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

//    void GameTableUIV88::setTimer(BYTE bChairID, int iTimerID, float fTime) {
//        m_bTimerChairID = bChairID;
//        m_iTimerID = iTimerID;
//        m_fClockTime = fTime;
//        m_fTimeCount = fTime - 1;
//        m_bDoTimeOutCallback = true;
//
////        if (m_clockObj && fTime >= 1) {
////            m_clockObj->stop();
////            m_clockObj->setVisible(true);
////            m_clockObj->start(fTime);
////        }
//
////        this->schedule(schedule_selector(GameTableUIV88::updateClock), 1.0f);
//    }

//    void GameTableUIV88::killTimer() {
//        if (m_clockObj) {
//            m_clockObj->stop();
//            m_clockObj->setVisible(false);
//        }
//        this->unschedule(schedule_selector(GameTableUIV88::updateClock));

//    }

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

	////////////////////////////////////////////////////////////////////////////
	//按钮回调
	////////////////////////////////////////////////////////////////////////////


	//开始按钮
	void GameTableUIV88::starButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
		if (isGuessIng)
		{
			HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_BUTTON_WRONG);
			return;
		}
        
		auto btn = (Button*)pSender;
		if (btn) {
			_GameTableLogic->sendStart();
		}
		if (!inInLoop)
		{
			HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);
		}
		int guessNum = 0;
		string guessNums = StringUtils::format("%d", guessNum);
		guessNumText->setString(guessNums);
		guessNumText->setVisible(false);

		i64Reward = 0;
		string addMoney = StringUtils::format("%lld", i64Reward);
		currentGetText->setString(addMoney);
		smallBtn->setEnabled(false);
		bigBtn->setEnabled(false);
	}

	//猜大小
	void GameTableUIV88::guessSizeCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}
		isGuessIng = true;
		betSizeNode->setVisible(false);
		betSizeAct->stop();
		auto btn = (Button*)pSender;
		if (btn) {
			bool isSmall = btn->getTag() == em_btnSmallTag;
			_GameTableLogic->sendGuessNumber(isSmall);
		}
		if (!inInLoop)
		{
			HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);
		}

		HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_BUTTON_B_S);
		m_audioEffectID = HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_GUESS_ING_EFFECT);
		this->schedule(schedule_selector(GameTableUIV88::jumpGuessNum), 0.2, 14, 0.1);
		smallBtn->setEnabled(false);
		bigBtn->setEnabled(false);

	}

	void GameTableUIV88::jumpGuessNum(float time)
	{
		int guessNum = rand() % 13 + 1;
		guessNumText->setVisible(true);
		string guessNums = StringUtils::format("%d", guessNum);
		guessNumText->setString(guessNums);
	}

	//下注增加筹码减少筹码
	void GameTableUIV88::chipInButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
  if (type != Widget::TouchEventType::ENDED) {
            return;
        }
		if (isGuessIng)
		{
			HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_BUTTON_WRONG);
			return;
		}

		auto btn = (Button*)pSender;
		if (btn) {
			bool isLeft = btn->getTag() == em_btnLeftTag;
			_GameTableLogic->sendSwapScore(isLeft);
		}
		if (!inInLoop)
		{
			HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);
		}

		HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_BUTTON_R_L);
	}

	//下注
	void GameTableUIV88::betAreaButtonsCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}

		auto btn = (AUIButton*)pSender;
		if (btn) {
			unsigned int index = btn->getTag() - em_betBarTag;
			char res[100];
			memset(res, 0, sizeof(res));
			sprintf(res, SHUIGUOJI_BUTTON_BET_EFFECT, index);
			HNAudioEngine::getInstance()->playEffect(res);

			_GameTableLogic->sendNote(index);
		}
	}




	//////////////////////////////////////////////////////////////////////////
	//动画

	//只展示筹码飞过去的动画，金额更新动画放到下注消息回调中
	void GameTableUIV88::betMoneyToAreaAnimation(int betMoneyIndex, int betAreaIndex, bool isMe/* =false */) {
		//        if (betMoneyIndex < 0 || betMoneyIndex >= Bet_Coin_Count) {
		//            return;
		//        }
		//        if (m_betAreaPosition.empty()) {
		//            return;
		//        }
		//        if (betAreaIndex < 0 || betAreaIndex >= BetA_Count || betAreaIndex >= m_betAreaPosition.size()) {
		//            return;
		//        }
		//        if (_GameTableLogic->getMyMoney() < BetMoneyValues[betMoneyIndex]) {
		//            return;
		//        }
		//
		//        HNAudioEngine::getInstance()->playEffect(BB_NOTE_COIN_EFFECT);
		//
		//        char path[100] = {0};
		//    //    sprintf(path, Bet_Note_Res_V88, betMoneyIndex);
		//        Sprite* sprite = Sprite::create(path);
		//        Vec2 startPos = Vec2::ZERO;
		//        Vec2 endPos = m_betAreaPosition.at(betAreaIndex);
		//        startPos = isMe == true ? m_notesPosition.at(betMoneyIndex) : Vec2(WIN_SIZE.width / 10.0f * (rand() % 8 + 2), -50.0f);
		//        if (sprite) {
		//            sprite->setScale(0.6f);
		//            sprite->setPosition(startPos);
		//            endPos += Vec2((rand() % 8) * 5 * (rand() % 3 - 1), (rand() % 8) * 5 * (rand() % 3 - 1));
		//            Sequence* seq = Sequence::create(MoveTo::create(0.4f, endPos), nullptr);
		//            sprite->runAction(seq);
		//            addChild(sprite, ZOrder_BetButtonLabel - 1);
		//            m_AllNotesMoveAnimations.at(betAreaIndex).push_back(sprite);
		//        }
	}

	void GameTableUIV88::updateBetMoneySelectedState(int betMoneyIndex) {
		//        if (betMoneyIndex < 0 || betMoneyIndex >= Bet_Coin_Count) {
		//            return;
		//        }
		//        if (_betArea.betMoneyLight.empty()) {
		//            return;
		//        }
		//        if (betMoneyIndex >= _betArea.betMoneyLight.size()) {
		//            return;
		//        }
		//        for (Sprite* light : _betArea.betMoneyLight) {
		//            if (light) {
		//                light->setVisible(false);
		//            }
		//        }
		//        if (_betArea.betMoneyLight.at(betMoneyIndex)) {
		//            _betArea.betMoneyLight.at(betMoneyIndex)->setVisible(true);
		//        }
	}

    void GameTableUIV88::showRunLoopAnimation(int endIndex, int loopTime, float delayTime) {
		unschedule(schedule_selector(GameTableUIV88::updateRandomBlinkLightFree));
		if (endIndex < 0 || endIndex >= Loop_Sprite_Count) {
			//转盘上的序号是 0-Loop_Sprite_Count
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

		HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_START_LOOP_EFFECT);

		//        schedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition), 0.16);//前10个，慢速，0.16s一帧，共1.6s
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
			updateMoneyState();
			//

			//播放水果的音效
			auto funa = CallFunc::create([&]() {
				int type = posKindArr[m_loopAnimationCurrentIndex];
				if (type != -1)
				{
					char res[100];
					memset(res, 0, sizeof(res));
					sprintf(res, SHUIGUOJI_RESULT_SHUIGUO, type);
					HNAudioEngine::getInstance()->playEffect(res);
				}

			});

			auto func = CallFunc::create([&]() {
				int type = posKindArr[m_loopAnimationCurrentIndex];
				if (type != -1)
				{
					char res[100];
					memset(res, 0, sizeof(res));
					sprintf(res, SHUIGUOJI_RESULT_EFFECT, type);
					m_audioEffectID = HNAudioEngine::getInstance()->playEffect(res);

					leftBtn->setBright(true);
					rightBtn->setBright(true);

					leftBtn->setTouchEnabled(true);
					rightBtn->setTouchEnabled(true);
				}
				else
				{
					leftBtn->setBright(false);
					rightBtn->setBright(false);
					startBtn->setBright(true);

					leftBtn->setTouchEnabled(false);
					rightBtn->setTouchEnabled(false);
					startBtn->setTouchEnabled(true);
				}
			});
			if (yazhongBool)
			{
				this->runAction(Sequence::create(funa, DelayTime::create(0.5f), func, nullptr));
			}
			else
			{
				this->runAction(Sequence::create(funa, nullptr));
			}




			lighBarKaijiangNode = CSLoader::createNode(FRUIT_LIGHTBAR_KAIJIANG);
			addChild(lighBarKaijiangNode, ZOrder_LightBar);
			lighBarKaijiangNode->setPosition(Vec2(WIN_SIZE.width / 2+10, WIN_SIZE.height / 2-22));
			lighBarKaijiangAct = CSLoader::createTimeline(FRUIT_LIGHTBAR_KAIJIANG);
			if (lighBarKaijiangNode) {
				lighBarKaijiangNode->runAction(lighBarKaijiangAct);
			}
			lighBarKaijiangNode->setVisible(false);


			////奖池开奖
			//jackpotKaijiangNode = CSLoader::createNode(FRUIT_JACKPOT_KAIJIANG);
			//addChild(jackpotKaijiangNode, ZOrder_Jackpot);
			//jackpotKaijiangNode->setPosition(jackpotSpPoint);
			//jackpotKaijiangAct = CSLoader::createTimeline(FRUIT_JACKPOT_KAIJIANG);

			//if (jackpotKaijiangNode) {
			//	jackpotKaijiangNode->runAction(jackpotKaijiangAct);

			//}
			//jackpotKaijiangNode->setVisible(false);

			CallFunc* kaijiaEffect = CallFunc::create([&]() {
				//开奖t特效
				lighBarDaijiNode->setVisible(false);
				lighBarDaijiAct->stop();
				lighBarKaijiangNode->setVisible(true);
				lighBarKaijiangAct->play("lightbar_lottery", false);
				//
		/*		jackpotDaijiNode->setVisible(false);
				jackpotDaijiAct->stop();
				jackpotKaijiangNode->setVisible(true);
				jackpotKaijiangAct->play("jackpot_kaijiang", false);*/

			});
			CallFunc* daijiEffect = CallFunc::create([&]() {

				lighBarKaijiangNode->setVisible(false);
				lighBarDaijiNode->setVisible(true);
				lighBarDaijiAct->play("paomadeng_daiji", true);
				//jackpotKaijiangNode->setVisible(false);
				//jackpotDaijiNode->setVisible(true);
				//jackpotDaijiAct->play("jackpot_daiji", true);

			});
			inInLoop = false;
			lighBarKaijiangNode->runAction(Sequence::create(kaijiaEffect, DelayTime::create(3.0f), daijiEffect, RemoveSelf::create(), nullptr));
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
				updateMoneyState();

				m_loopAnimationRemainTime = 0;
				m_loopAnimationMaxTime = 0;
				m_loopAnimationCurrentIndex = m_loopAnimationEndIndex;


				//播放水果的音效
				auto funa = CallFunc::create([&]() {
					int type = posKindArr[m_loopAnimationCurrentIndex];
					if (type != -1)
					{
						char res[100];
						memset(res, 0, sizeof(res));
						sprintf(res, SHUIGUOJI_RESULT_SHUIGUO, type);
						//HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);
						HNAudioEngine::getInstance()->playEffect(res);
					}

				});

				auto func = CallFunc::create([&]() {
					int type = posKindArr[m_loopAnimationCurrentIndex];
					if (type != -1)
					{
						char res[100];
						memset(res, 0, sizeof(res));
						sprintf(res, SHUIGUOJI_RESULT_EFFECT, type);
						m_audioEffectID = HNAudioEngine::getInstance()->playEffect(res);
					}

				});

				if (yazhongBool)
				{
					this->runAction(Sequence::create(funa, DelayTime::create(0.5f), func, nullptr));
				}
				else
				{
					this->runAction(Sequence::create(funa, nullptr));
				}


				unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
				lighBarKaijiangNode = CSLoader::createNode(FRUIT_LIGHTBAR_KAIJIANG);
				addChild(lighBarKaijiangNode, ZOrder_LightBar);
				lighBarKaijiangNode->setPosition(Vec2(WIN_SIZE.width / 2+10, WIN_SIZE.height / 2-22));
				lighBarKaijiangAct = CSLoader::createTimeline(FRUIT_LIGHTBAR_KAIJIANG);
				if (lighBarKaijiangNode) {
					lighBarKaijiangNode->runAction(lighBarKaijiangAct);
				}
				lighBarKaijiangNode->setVisible(false);


				////奖池开奖
				//jackpotKaijiangNode = CSLoader::createNode(FRUIT_JACKPOT_KAIJIANG);
				//addChild(jackpotKaijiangNode, ZOrder_Jackpot);
				//jackpotKaijiangNode->setPosition(jackpotSpPoint);
				//jackpotKaijiangAct = CSLoader::createTimeline(FRUIT_JACKPOT_KAIJIANG);

				//if (jackpotKaijiangNode) {
				//	jackpotKaijiangNode->runAction(jackpotKaijiangAct);

				//}
				//jackpotKaijiangNode->setVisible(false);

				CallFunc* kaijiaEffect = CallFunc::create([&]() {
					//开奖t特效
					lighBarDaijiNode->setVisible(false);
					lighBarDaijiAct->stop();
					lighBarKaijiangNode->setVisible(true);
					lighBarKaijiangAct->play("lightbar_lottery", false);

					//
					//jackpotDaijiNode->setVisible(false);
					//jackpotDaijiAct->stop();
					//jackpotKaijiangNode->setVisible(true);
					//jackpotKaijiangAct->play("jackpot_kaijiang", false);

				});
				CallFunc* daijiEffect = CallFunc::create([&]() {

					lighBarKaijiangNode->setVisible(false);
					lighBarDaijiNode->setVisible(true);
					//lighBarDaijiAct->play("paomadeng_daiji", true);
					//jackpotKaijiangNode->setVisible(false);
					//jackpotDaijiNode->setVisible(true);
					//jackpotDaijiAct->play("jackpot_daiji", true);
				});
				lighBarKaijiangNode->runAction(Sequence::create(kaijiaEffect, DelayTime::create(3.0f), daijiEffect, RemoveSelf::create(), nullptr));
				inInLoop = false;
				return;
			}
		}
		//
		if (m_loopAnimationRemainTime == m_loopAnimationMaxTime - 9) {
			//            unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
			m_audioEffectID = HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_MAX_SPEED_LOOP_EFFECT);
			unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
			schedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition), (7.5f - 5.0f) * 0.9f / (m_loopAnimationMaxTime - 20));
		}
		else if (m_loopAnimationRemainTime == 15) {
			unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
			schedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition), 0.1f);//共1.2s
		}
		else if (m_loopAnimationRemainTime == 5) {
			//            unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
			HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);
			m_audioEffectID = HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_SLOW_SPEED_LOOP_EFFECT);
			unschedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition));
			schedule(schedule_selector(GameTableUIV88::updateLoopSpritePosition), 0.35f); //共2.7s
		}
	}

	//固定时间间隔随机闪烁，其最后停止的位置即为下局转盘开始的位置
	void GameTableUIV88::updateRandomBlinkLightFree(float dt) {
		//        if (m_GameStatus != GS_XIAZHU) {
		//            unschedule(schedule_selector(GameTableUIV88::updateRandomBlinkLightFree));
		//            return;
		//        }
		//        m_loopAnimationEndIndex = rand() % Loop_Sprite_Count;
		//        if (m_loopAnimationEndIndex < 0 || m_loopAnimationEndIndex >= Loop_Sprite_Count) {
		//            m_loopAnimationEndIndex = 0;
		//        }
		//        m_loopAnimationCurrentIndex = m_loopAnimationEndIndex;
		//        updateBlinkOpenAnimalLight();
	}

	void GameTableUIV88::updateTestContent(float dt) {
		//        if (_gameState) {
		//            int stateID = rand()%6;
		//            _gameState->updateGameStateContent(stateID);
		//        }
	}


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



	void GameTableUIV88::showOpenAnimalAnimation(int index) {
		if (index < 0 || index >= Loop_Sprite_Count) {
			return;
		}
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
		//跳动的音效
		index = index % Loop_Sprite_Count;
		if (index >= 0 && index < _loopArea.loopSprites.size()) {
			if (_loopArea.loopSprites.at(index)) {
				_loopArea.loopSprites.at(index)->setVisible(true);
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
        //play big win animation
        if (index < Loop_Sprite_Count) {
            int logoType = g_Logos[index];  //开奖的类型
            if (logoType <= BetA_Star) {
                int logoRate = g_Logos_Rate[index];
                if (m_i64OwnNotes[logoType] > 0 && logoRate >= 30) {
                    playBigWinAnimation(FRUIT_ID, logoType, logoRate, m_i64OwnNotes[logoType]*logoRate*m_i64BaseCoin);
                }
            }
        }

	}

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
			// createState();
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
					_openRecord.openLogos.at(i)->showLightEffect(true);
					//                animationMoveOpenItemToStateLabel(kinds.at(i));
				}
			}
		}

	}

	//将开奖的icon移动到走势图最后的位置，endIdx为最后一个
	void GameTableUIV88::animationMoveOpenItemToStateLabel(int endIdx) {
		Vec2 startPos = Vec2::ZERO;
		Vec2 endPos = Vec2::ZERO;
		int lastIndexInState = 0;
		auto cell = FruitLoopSpriteCellV88::create();
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
					auto newCell = FruitLoopSpriteCellV88::create();
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




	//玩家坐下(非自己)
	void GameTableUIV88::userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
		addUser(userSit->bDeskStation, userSit->bDeskStation == _GameTableLogic->getMySeatNo());
	}

	//玩家站起
	void GameTableUIV88::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
		if (m_GameStatus == GS_FREE_NOTE) {

		}
	}
	//每次下注返回
	void GameTableUIV88::onGameNoteResponse(GameNoteResponse *msg) {
		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->gameState;
		string addMoney = StringUtils::format("%lld", msg->i64MyMoney);
		myLastMoneyText->setString(addMoney);
		int len = sizeof(msg->iOwnNotesNumber) / sizeof(msg->iOwnNotesNumber[0]);
		for (size_t i = 0; i < len; i++)
		{
			m_i64OwnNotes[i] = msg->iOwnNotesNumber[i];
			auto cell = _betArea.betMoneySp.at(i);
			if (cell)
			{
				cell->addBetMonye(msg->iOwnNotesNumber[i]);
			}
		}


	}

	void GameTableUIV88::onGameNoteFail(GameNoteFailedMsg *msg) {
		if (msg == nullptr) {
			return;
		}
		switch (msg->bFailCode)
		{
		case 0:
			gameStateText->setString(GBKToUtf8("金币不足！"));
			HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_BUTTON_WRONG);
			break;
		case 1:
			gameStateText->setString(GBKToUtf8("下注超过限额！"));
			HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_BUTTON_WRONG);
			break;
		default:
			break;
		}
		this->scheduleOnce(schedule_selector(GameTableUIV88::hideGameStateText), 1.5f);
	}



	void GameTableUIV88::onGameBeginFailLackCoin(GameBeginFailLackCoin *msg) {

		if (msg == nullptr) {
			return;
		}
		switch (msg->bFailCode)
		{
		case 0:
			gameStateText->setString(GBKToUtf8("金币不足！"));
			HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_BUTTON_WRONG);
			break;
		default:
			break;
		}
		this->scheduleOnce(schedule_selector(GameTableUIV88::hideGameStateText), 1.5f);
	}


	void GameTableUIV88::hideGameStateText(float time) {
		gameStateText->setString("");
	}
	//开奖结果
	void GameTableUIV88::onGameOpenLogo(GameLoopResult *msg) {
		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->gameState;
		if (msg->iOpenLogo < 0 || msg->iOpenLogo >= Loop_Sprite_Count) {
			return;
		}
		if (msg->iEndPos < 0 || msg->iEndPos >= Loop_Sprite_Count) {
			return;
		}

		i64MyMoney = msg->i64MyMoney;
		i64Reward = msg->i64Reward;
		yazhongBool = false;
		if (i64Reward>0)
		{
			yazhongBool = true;
		}

		//终点
		//    m_loopAnimationCurrentIndex = msg->iStarPos;
		inInLoop = true;
		m_loopAnimationEndIndex = msg->iEndPos;
		showRunLoopAnimation(m_loopAnimationEndIndex, msg->iTimes);

		startBtn->setBright(false);
		leftBtn->setBright(false);
		rightBtn->setBright(false);

		startBtn->setTouchEnabled(false);
		leftBtn->setTouchEnabled(false);
		rightBtn->setTouchEnabled(false);
	}

	void GameTableUIV88::updateMoneyState()
	{
		string myMoney = StringUtils::format("%lld", i64MyMoney);
		myLastMoneyText->setString(myMoney);
		//
		string addMoney = StringUtils::format("%lld", i64Reward);
		currentGetText->setString(addMoney);
	}
    
    void GameTableUIV88::playBigWinAnimation(int gameID, int openLogo, int logoRate, LLONG money) {
        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
        addChild(colorLayer, ZOrder_GameState+1);
        colorLayer->runAction(Sequence::create(DelayTime::create(2.5f), RemoveSelf::create(), nullptr));
        auto node = CSLoader::createNode("v88/animation/common/jackpotting/jackpotting.csb");
        if (node) {
            node->setPosition(WIN_SIZE/2);
            addChild(node, ZOrder_GameState+1);
            auto action = CSLoader::createTimeline("v88/animation/common/jackpotting/jackpotting.csb");
            if (action) {
                node->runAction(action);
                action->play("jackpotting", false);
            }
            //
            char gameNamePath[64] = {0};
            sprintf(gameNamePath, "v66/common/bigwin/%d.png", gameID);
            std::string logoName = "";
            if (openLogo == BetA_Bar) {
                logoName = "BAR";
            }
            else if (openLogo == BetA_Seven) {
                logoName = "77";
            }
            else if (openLogo == BetA_Star) {
                logoName = "双星";
            }
            char tipString[256] = {0};
            sprintf(tipString, GetText("BIG_WIN_TIP_FRUIT"), logoName.c_str(), logoRate);
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


	//第一次进入游戏基础数据同步d
	void GameTableUIV88::syncBasicDataFirstTime(SyncDataStruct *msg) {
		if (msg == nullptr) {
			return;
		}
		//long long mymoney = msg->gameState;
		i64MyMoney = msg->i64MyMoney;
		m_GameStatus = msg->gameState;
        m_i64BaseCoin = msg->iBasicCoin;
		if (msg->iBasicCoin >= 1000000)
		{
			costTypeImg->loadTexture(COSTTYPE_PATH_HUNDREN);
		}
		else if (msg->iBasicCoin >= 500000)
		{
			costTypeImg->loadTexture(COSTTYPE_PATH_FIFTY);
		}
		else if (msg->iBasicCoin >= 100000)
		{
			costTypeImg->loadTexture(COSTTYPE_PATH_TEN);
		}
		else if (msg->iBasicCoin >= 10000)
		{
			costTypeImg->loadTexture(COSTTYPE_PATH_ONE);
		}
		else
		{
			costTypeImg->loadTexture(COSTTYPE_PATH_THOUSND);
		}
		updateMoneyState();

		leftBtn->setBright(false);
		rightBtn->setBright(false);
		
		leftBtn->setTouchEnabled(false);
		rightBtn->setTouchEnabled(false);
        
        if (!m_bLuck)
        {
            startBtn->setTouchEnabled(true);
            startBtn->setBright(true);
        }
	}


	//是否可以猜大小
	void GameTableUIV88::onGameNotifyGuess(Fruit::GameNotifyGuessNumber *msg) {
		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->gameState;
		isCanGuessB = false;
		if (!msg->canGuess)
		{
			return;
		}
		isCanGuessB = true;
		betSizeNode->setVisible(true);
		betSizeAct->play("betsize", true);

		smallBtn->setEnabled(true);
		bigBtn->setEnabled(true);

		rightBtn->setBright(true);
		leftBtn->setBright(true);
		startBtn->setBright(true);

		rightBtn->setTouchEnabled(true);
		leftBtn->setTouchEnabled(true);
		startBtn->setTouchEnabled(true);
	}

	void GameTableUIV88::onGameGuessOverTime(Fruit::GameGuessOverTime *msg) {
		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->gameState;


	}
	//猜大小结果
	void GameTableUIV88::onGameGuessResult(Fruit::GameGuessResult *msg) {
		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->gameState;
		int resultNum = msg->guessNumber;
		string guessNum = StringUtils::format("%d", resultNum);
		guessNumText->setString(guessNum);
		guessNumText->setVisible(true);
        isGuessIng = false;
		if (msg->canContinueGuess)
		{
			isCanGuessB = true;
			betSizeNode->setVisible(true);
			betSizeAct->play("betsize", true);
			HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_GUESS_WIN_EFFECT);
			smallBtn->setEnabled(true);
			bigBtn->setEnabled(true);
		}
		else
		{
			isCanGuessB = false;
			HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_GUESS_FAIL_EFFECT);
			betSizeNode->setVisible(false);
			betSizeAct->stop();
			smallBtn->setEnabled(false);
			bigBtn->setEnabled(false);
		}
		HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);

		//
		i64MyMoney = msg->i64MyMoney;
		i64Reward = msg->i64Reward;
		updateMoneyState();

	}

	void GameTableUIV88::onGameSwapScoreResponse(Fruit::GameSwapScoreReseponse *msg) {
		if (msg == nullptr) {
			return;
		}

		i64MyMoney = msg->i64MyMoney;
		i64Reward = msg->i64Reward;
		updateMoneyState();

		if (msg->iSwapCode == 0)
		{
			leftBtn->setBright(false);
			rightBtn->setBright(false);
			startBtn->setBright(true);

			leftBtn->setTouchEnabled(false);
			rightBtn->setTouchEnabled(false);
			startBtn->setTouchEnabled(true);
		}

	}

	void GameTableUIV88::onGameNotifyLucky(Fruit::GameNotifyLucky *msg) {
		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->gameState;
	}
	//开奖出来lucky
	void GameTableUIV88::onGameOpenLuckyLogos(Fruit::GameLuckyOpen *msg) {

		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->gameState;
		unschedule(schedule_selector(GameTableUIV88::delayShowOpenLogoEffect));
		int len = sizeof(msg->iOpenLucky) / msg->iOpenLucky[0];
		luckT = 0;
		for (int i = 0; i < msg->openTimes; i++)
		{
			int luckyNum = msg->iOpenLucky[i];
			iOpenLucky[i] = luckyNum;

		}
		i64MyMoney = msg->i64MyMoney;
		//i64Reward = msg->i64Reward;
		luckOpenT = msg->openTimes;
		if (luckOpenT < 1)
		{
			HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_LUCKY_FAIL_EFFECT);
		}
		else
		{
			HNAudioEngine::getInstance()->playEffect(SHUIGUOJI_LUCKY_SMALL_EFFECT);
		}

		isCanGuessB = false;
		schedule(schedule_selector(GameTableUIV88::delayShowOpenLogoEffect), 4.0);

        m_bLuck = true;
        startBtn->setTouchEnabled(false);
        startBtn->setBright(false);
	}

	void GameTableUIV88::delayShowOpenLogoEffect(float time)
	{

		unschedule(schedule_selector(GameTableUIV88::delayShowOpenLogoEffect));
		if (luckT >= luckOpenT)
		{
			long long addMoneyThis = resultAllMoney - i64MyMoney;
			if (addMoneyThis > 0)
			{
				char versionStr[64] = { 0 };
				sprintf(versionStr, "+ %lld", addMoneyThis);
				auto version = Text::create(versionStr, FONT_TTF_FZCY, 26);
				version->setColor(Color3B::YELLOW);
				version->setPosition(Vec2(80, WIN_SIZE.height*0.57));
				addChild(version, 100000);
				Spawn* spawn = Spawn::create(MoveBy::create(1.0f, Point(0, 30.0f)), FadeIn::create(0.5f), nullptr);
				version->runAction(Sequence::create(spawn, RemoveSelf::create(), nullptr));
			}
            
            m_bLuck = false;
            startBtn->setBright(true);
            startBtn->setTouchEnabled(true);
			updateMoneyState();
			return;
		}

		//播放水果的音效
		auto funa = CallFunc::create([&]() {
			int type = 0;
			if (type != -1)
			{
				char res[100];
				memset(res, 0, sizeof(res));
				sprintf(res, SHUIGUOJI_SHOOT_EFFECT, type);
				HNAudioEngine::getInstance()->playEffect(res);
			}

		});

		//播放水果的音效
		auto funb = CallFunc::create([&]() {
			int type = 1;
			char res[100];
			memset(res, 0, sizeof(res));
			sprintf(res, SHUIGUOJI_SHOOT_EFFECT, type);
			HNAudioEngine::getInstance()->playEffect(res);

		});

		auto func = CallFunc::create([&]() {
			int type = 2;
			char res[100];
			memset(res, 0, sizeof(res));
			sprintf(res, SHUIGUOJI_SHOOT_EFFECT, type);
			HNAudioEngine::getInstance()->playEffect(res);

			rightBtn->setBright(true);
			leftBtn->setBright(true);

			rightBtn->setTouchEnabled(true);
			leftBtn->setTouchEnabled(true);
		});

		this->runAction(Sequence::create(DelayTime::create(0.2f), funa, DelayTime::create(0.1f), funb, DelayTime::create(0.1f), func, nullptr));


		m_loopAnimationEndIndex = iOpenLucky[luckT];
		luckT++;
		schedule(schedule_selector(GameTableUIV88::showLuckyEffectLoop), 0.16);//前10个，慢速，0.16s一帧，共1.6s

	}


	void GameTableUIV88::showLuckyEffectLoop(float time)
	{
		if (m_loopAnimationCurrentIndex >= Loop_Sprite_Count)
		{
			m_loopAnimationCurrentIndex = 0;
		}
		if (m_loopAnimationCurrentIndex == m_loopAnimationEndIndex)
		{
			playOpenLogoAnimationAtIndex(m_loopAnimationCurrentIndex);
			unschedule(schedule_selector(GameTableUIV88::showLuckyEffectLoop));
			schedule(schedule_selector(GameTableUIV88::delayShowOpenLogoEffect), 0.5);
			return;
		}
		m_loopAnimationCurrentIndex++;
		stopLuckLoopAnimation();
		startLoopAnimationAtIndex(m_loopAnimationCurrentIndex);


	}

	//lucy 之后的效果
	void GameTableUIV88::stopLuckLoopAnimation() {
		if (_loopArea.loopSprites.empty()) {
			return;
		}
		for (int i = 0; i < _loopArea.loopSprites.size(); i++) {
			if (_loopArea.loopSprites.at(i)) {
				_loopArea.loopSprites.at(i)->showLightEffect(false);
			}
		}

		for (int j = 0; j < (luckT - 1); j++)
		{
			_loopArea.loopSprites.at(iOpenLucky[j])->showLightEffect(true);
		}


		if (luckOpenT >= 4)
		{
			_loopArea.loopSprites.at(0)->showLightEffect(true);
		}
		else
		{
			_loopArea.loopSprites.at(12)->showLightEffect(true);
		}



	}
	//同步奖池的数据
	void GameTableUIV88::onGameRewardsPool(GameRewardsPool* msg) {
		if (msg == nullptr) {
			return;
		}
		string poolNum = StringUtils::format("%lld", msg->i64RewardsPool);
		jackpotText->setString(poolNum);
		jackpotText->setVisible(true);

	}

	//计算结果
	void GameTableUIV88::onGameSettlementMsg(GameSettlementMsg *msg) {

		if (msg == nullptr) {
			return;
		}
		m_GameStatus = msg->gameState;
		i64MyMoney = msg->i64MyMoney;
		resultAllMoney = msg->i64MyMoney;
		betSizeNode->setVisible(false);
		betSizeAct->stop();
		smallBtn->setEnabled(false);
		bigBtn->setEnabled(false);

		if (m_GameStatus != GS_LUCKY)
		{
			updateMoneyState();
		}

        auto coin = FloatCoin::create();
        if (coin) {
            coin->openParent(this, WIN_SIZE/2, ZOrder_SettlementLayer);
            coin->updateCoin(msg->i64Reward);
        }


		//memset(m_i64OwnNotes, 0, sizeof(m_i64OwnNotes));//清空数据
		//for (size_t i = 0; i < _betArea.betMoneySp.size(); i++)
		//{	
		//	auto cell = _betArea.betMoneySp.at(i);
		//	if (cell)
		//	{
		//		cell->addBetMonye(m_i64OwnNotes[i]);
		//	}
		//}

	}


	void GameTableUIV88::onGameTipExit(tag_s2c_tip_exit* msg) {
		if (msg == nullptr) {
			return;
		}

		tipMsg = msg;
		m_GameStatus = msg->gameState;
		updateMoneyState();
		//
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

//		//要退出
//		this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
//			tipCallback();
//		})));
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



	void GameTableUIV88::onGameNotify(tag_s2c_notify* msg) {

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
		m_GameStatus = GS_FREE_NOTE;
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
		/*if (_gameSettlement) {
			_gameSettlement->hideSettlementLayer();
		}*/
		//animationStopCarLight();

		leftBtn->setBright(false);
		rightBtn->setBright(false);
		startBtn->setBright(true);

		leftBtn->setTouchEnabled(false);
		rightBtn->setTouchEnabled(false);
		startBtn->setTouchEnabled(true);
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
		//_gameSettlement = nullptr;

//        m_clockObj = nullptr;
//        m_clockPos = Vec2(-247.8, 214);
		gameStateText = nullptr;
		// games
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
		//    m_GameStatus = GS_FREE_NOTE;
		//    m_bGameEnd = false;
		//    m_bNextOut = false;
		//    m_bCurUser = INVALID_BYTE;
		//    m_bNextUser = INVALID_BYTE;
		//
		//    m_canPrepareForNew = false;
		//    //m_bBankerID = INVALID_BYTE;
		//    //m_vPlayerPrepare.clear();
		//



		m_notesPosition.clear();
		m_betAreaPosition.clear();
		m_betLabelPos.clear();


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
	/*	if (_gameSettlement) {
			_gameSettlement->hideSettlementLayer();
		}*/

        m_i64BaseCoin = 1;
//        i64MyLastMoney = 0;
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
		m_AllNotesMoveAnimations.resize(BetA_Count);
	}

	//Tools
	void GameTableUIV88::setButtonEnabled(ui::Button* pButton, bool enabled) {
		if (pButton != nullptr) {
			GLubyte opacity = enabled ? 255 : 125;
			pButton->setTouchEnabled(enabled);
			pButton->setOpacity(opacity);
		}
	}

	void GameTableUIV88::updateAllNoteButtonsState(bool enable) {
		//        for (Sprite* money : _betArea.betMoneySp) {
		//            if (money) {
		//                //setButtonEnabled(money, enable);
		//            }
		//        }
		//        if (!enable) {
		//            for (int i = 0; i < Bet_Coin_Count; i++) {
		//                _betArea.betMoneyLight[i]->setVisible(false);
		//            }
		//        }
	}

	void GameTableUIV88::updateBetMoneyEnable() {
		//        for (int i = 0; i < Bet_Coin_Count; i++) {
		//            if (_GameTableLogic->getMyMoney() >= BetMoneyValues[i]) {
		//                _betData.betMoneyEnable[i] = true;
		//            } else {
		//                _betData.betMoneyEnable[i] = false;
		//                _betArea.betMoneyLight[i]->setVisible(false);
		//            }
		//            if (_betArea.betButtons.size() > i && _betArea.betButtons.at(i)) {
		//               // setButtonEnabled(_betArea.betMoneyBtn.at(i), _betData.betMoneyEnable[i]);
		//            }
		//        }
	}

	void GameTableUIV88::enableAllBetMoney() {
		updateBetMoneyEnable();
	}

	void GameTableUIV88::disableAllBetMoney() {
		//        for (int i = 0; i < Bet_Coin_Count; i++) {
		//           // setButtonEnabled(_betArea.betMoneyBtn.at(i), false);
		//        }
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
            i64MyMoney = msg->i64MyMoney;
            runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                txtReward->setString(Tools::addComma(luckyMoney));
                updateMoneyState();
            }), DelayTime::create(retainTime), CallFunc::create([&](){
                this->removeChildByName("rewardResult");
            }), NULL));
        }
    }
}
