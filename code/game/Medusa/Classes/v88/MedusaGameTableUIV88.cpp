#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNCommon/HNConverCode.h"

#include "MedusaGameTableUIV88.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "MedusaSettlementV88.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "base/CCRef.h"
#include "HNLobby/GameRewardEffect.hpp"
#define ICON_HEIGHT 161

namespace Medusa {
    enum {
        ZOrder_Bg = 100,
        ZOrder_Middle_Layer = 150,
        ZOrder_Top_Layer = 151,
        ZOrder_Bottom_Layer = 152,
        ZOrder_Pool = 155,
        ZOrder_Float_Coin = 160,
        ZOrder_SettlementLayer = 340,
        ZOrder_Tool = 400,
    };
    
    const static char* bgMusic = "Games/31000510/music/medusaBg.mp3";
    const static char* Medusa_bingo = "v66/music/CSLL/CSLL_bingo.mp3";
    const static char* _startLoop = "v66/music/CSLL/CSLL_startLoop.mp3";
    const static char* kIconTypeTpl = "Games/31000510/type/%02d.png";
    const static char* shIconType = "Games/31000510/type/0%dsh.png";
    const static char* kHelpImg = "Games/31000510/help/Medusa.png";
    const static char* kCsbRootFile = "Games/31000510/ui.csb";
    const static char* kJackpotUI = "common/animation/jackPot/Jackpot.csb";
    const static char* kMedusaEffect = "Games/31000510/MedusaNode.csb";
    const static char* ksmallshEffect = "Games/31000510/smallshNode.csb";
    const static char* klargeshEffect = "Games/31000510/largeshNode.csb";
    // 结果开奖线闪烁时长与次数
    const static float kResultLineBlinkDuration = 0.5f;
    const static int kResultLineBlinkTimes = 2;
    
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
        : m_bAuto(false)
        , m_bHasCashIn(true)
        , m_bGameFinished(true)
        , m_bAnimationFinished(true)
        , m_i64WinMoney(0)
        , i64WinJackpot(0)
        , nLotteryTypeCount(0)
		, m_iFreeCount(0)
    {
        bHasMedusa = 0;
        bIsPetrified = 0;
	}

	GameTableUIV88::~GameTableUIV88()
    {
		this->unschedule(schedule_selector(GameTableUIV88::updateTip));
	
		CC_SAFE_DELETE(_GameTableLogic);

		resetData();
	}

	bool GameTableUIV88::init(BYTE bDeskIndex, bool bAutoCreate) {
		if (!HNGameUIBase::init()) {
			return false;
		}

		resetData();
		_GameTableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate, PLAYER_COUNT);
        scheduleUpdate();
		return true;
	}

    void GameTableUIV88::closeLoadingCallBack()
    {
        
    }
	void GameTableUIV88::createRootLayer() {
        //background image
//        ImageView* bg = ImageView::create("v66//ui/bg.png");
//        if (bg) {
//            _root.bg = bg;
//            bg->setPosition(WIN_SIZE/2);
//            addChild(bg, ZOrder_Bg);
//        }
        
        //middle Layer
        Node* csbRoot = CSLoader::createNode(kCsbRootFile);
        if (csbRoot) {
            _root.middleNode = csbRoot;
            csbRoot->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2));
            addChild(csbRoot, ZOrder_Middle_Layer);
            //
            auto bg = csbRoot->getChildByName("Image_bg");
            if (bg) {
                char str[128] = {0};
                for (int i = 0; i < LINE_COUNT; i++) {
                    memset(str, 0, sizeof(str));
                    sprintf(str, "Image_line_%d", i+1);
                    auto line = dynamic_cast<ImageView*>(bg->getChildByName(str));
                    if (line) {
                        line->setVisible(false);
                        m_vAllLines[i] = line;
                    }
                }
                
                Text* myName = dynamic_cast<Text*>(bg->getChildByName("txtName"));
                if (myName) {
                    _root.myName = myName;
                }
                Text* myMoney = dynamic_cast<Text*>(bg->getChildByName("txtMoney"));
                if (myMoney) {
                    _root.myMoney = myMoney;
                }

                Text* scoreText = dynamic_cast<Text*>(bg->getChildByName("txtScore"));
                if (scoreText) {
                    _root.score = scoreText;
                }
                auto jackpotText = dynamic_cast<Text*>(bg->getChildByName("txtJackpot"));
                if (jackpotText)
                {
                    _root.jackpot = jackpotText;
                }
                Text* addLineText = dynamic_cast<Text*>(bg->getChildByName("txtLine"));
                if (addLineText) {
                    _root.addLineCount = addLineText;
                }
                Text* addNoteText = dynamic_cast<Text*>(bg->getChildByName("txtNote"));
                if (addNoteText) {
                    _root.addNoteCount = addNoteText;
                }
                
                Button* start = dynamic_cast<Button*>(bg->getChildByName("btnStart"));
                if (start) {
                    _root.startBtn = start;
                    start->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::startBtnCallBack, this));
                }
                Button* autoBtn = dynamic_cast<Button*>(bg->getChildByName("btnAutoStart"));
                if (autoBtn) {
                    _root.autoBtn = autoBtn;
                    autoBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::autoBtnCallBack, this));
                }
                
                auto cancelBtn = dynamic_cast<Button*>(bg->getChildByName("btnCancelAuto"));
                if (cancelBtn)
                {
                    _root.cancelBtn = cancelBtn;
                    _root.cancelBtn->setVisible(false);
                    _root.cancelBtn->setTouchEnabled(false);
                    _root.cancelBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::autoBtnCallBack, this));
                }
                
                Button* maxNote = dynamic_cast<Button*>(bg->getChildByName("btnMaxNote"));
                if (maxNote) {
                    _root.maxNote = maxNote;
                    maxNote->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::maxNoteBtnCallBack, this));
                }
                
                Button* maxLine = dynamic_cast<Button*>(bg->getChildByName("btnMaxLine"));
                if (maxLine) {
                    _root.maxLine = maxLine;
                    maxLine->setBright(false);
                    maxLine->setTouchEnabled(false);
                    maxLine->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::maxLineBtnCallBack, this));
                }
                
                auto nineLine = dynamic_cast<Button*>(bg->getChildByName("btnHalfLine"));
                if (nineLine)
                {
                    _root.nineLine = nineLine;
                    nineLine->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::nineLineBtnCallBack, this));
                }
                
                Button* addNote = dynamic_cast<Button*>(bg->getChildByName("btnAddNote"));
                if (addNote) {
                    _root.addNote = addNote;
                    addNote->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::addNoteBtnCallBack, this));
                }
            }
        }
	}

    void GameTableUIV88::loadTurn() {
        char str[128] = {0};
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            if (_root.middleNode) {
                auto bg = _root.middleNode->getChildByName("Image_bg");
                if (bg) {
                    //转动的容器
                    memset(str, 0, sizeof(str));
                    sprintf(str, "Panel_%d", i);
                    Layout* layout = dynamic_cast<Layout*>(bg->getChildByName(str));
                    if (layout) {
                        layout->setClippingEnabled(true);
                        m_vPanelTurnArea.push_back(layout);
                    }
                }
            }
            //
            //转动的icon
            if (m_vPanelTurnArea.size() > 0) {
                setTurnIcomPosition(m_vTurnIconPos, m_vPanelTurnArea.at(0)->getContentSize());
            }
            for (int j = 0; j < 3; j++) { //先创建5列3行假的,拿到服务端下发的真实icon个数再创建
                sprintf(str, kIconTypeTpl, j);
                ImageView* icon = ImageView::create(str);
                icon->setTag(j);
                m_vPanelTurnArea.at(i)->addChild(icon);
                icon->setPosition(m_vTurnIconPos.at(j));
                
            }
        }
    }
    void GameTableUIV88::loadTurnEx()
    {
        //转动的icon
        if (m_vPanelTurnArea.size() > 0) {
            m_vTurnIconPos.clear();
            setTurnIcomPosition(m_vTurnIconPos, m_vPanelTurnArea.at(0)->getContentSize());
        }
        
        char str[128] = {0};
        vector<ImageView*> vIcon;
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            vIcon.clear();
            for (int j = 0; j < nLotteryTypeCount; j++) {
                ImageView * oldIcon =(ImageView*)m_vPanelTurnArea.at(i)->getChildByTag(j);
                if (oldIcon) {
                    oldIcon->removeFromParent();
                }
                sprintf(str, kIconTypeTpl, j);
                ImageView* icon = ImageView::create(str);
                icon->setTag(j);
                m_vPanelTurnArea.at(i)->addChild(icon);
                icon->setPosition(m_vTurnIconPos.at(j));
                
                vIcon.push_back(icon);
            }
            m_vTurnIcon.push_back(vIcon);
        }
        
        
        
    }
    void GameTableUIV88::createGameSettlementLayer() {
        _gameSettlement = nullptr;
        SettlementV88* settle = SettlementV88::create();
        if (settle) {
            settle->setPosition(WIN_SIZE/2);
            _gameSettlement = settle;
            _gameSettlement->hideSettleLayer();
            addChild(_gameSettlement, ZOrder_SettlementLayer);
        }
    }

    void GameTableUIV88::onEnter() {
		HNGameUIBase::onEnter();
        HNAudioEngine::getInstance()->playBackgroundMusic(bgMusic, true);
        
        //触摸
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
		myListener->setSwallowTouches(false);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

        initData();

        createRootLayer();
		createToolBarUI();
        loadTurn();
        createGameSettlementLayer();
        
        updateSelectLines();
        setNick(_GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo())->nickName);
		_GameTableLogic->sendGameInfo();
	}

    void GameTableUIV88::setNick(char* nick) {
        if (_root.myName) {
            _root.myName->setString(ForceGBKToUtf8(nick));
        }
	}

    bool GameTableUIV88::onTouchBegan(Touch* touch, Event* event) {
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
					if (_GameTableLogic->getGameStatus() != GS_FREE) {
						//auto layer = GamePromptLayer::create();
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
                gameSet->setBgPath(bgMusic);
				gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
			});
            toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
                auto helpLayer = HN::HelpLayer::create();
                helpLayer->setContentOnView(kHelpImg);
                helpLayer->setName("helpLayer");
                helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                helpLayer->setOrignalPosition();
                helpLayer->moveSelfInWithAction();
                addChild(helpLayer, zOrder + 5);
            });
//            toolbar->addImageAndCallBack(TOOL_BAR_HELP, nullptr);
            toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
		}
	}

    void GameTableUIV88::leaveTheGame() {
		HNRoomLogic::getInstance()->closeRoom();
		PlatformState* state = PlatformState::create();
		state->setType(ROOMLIST);
		StateManager::getInstance()->setState(state);
	}

	void GameTableUIV88::onExit() {
		HNGameUIBase::onExit();
        if (m_audioEffectID > 0) {
			HNAudioEngine::getInstance()->stopEffectWithID(m_audioEffectID);
        }
	}

	void GameTableUIV88::onExitTransitionDidStart() {
		HNGameUIBase::onExitTransitionDidStart();
		HNAudioEngine::getInstance()->stopBackgroundMusic();

	}

	//按钮回调
	////////////////////////////////////////////////////////////////////////////
	//加线按钮
	void GameTableUIV88::addLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}
        addLineLogic();
	}
    
    // 减线
    void GameTableUIV88::subLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        
        --m_currentMaxLine;
        if (m_currentMaxLine < 0)
        {
            m_currentMaxLine = 0;
        }
        
        updateSelectLines();
    }

    //加注按钮
    void GameTableUIV88::addNoteBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        addNoteLogic();
    }
    
    //满线按钮
    void GameTableUIV88::maxLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        
        maxLineLogic();
        
        _root.maxLine->setBright(false);
        _root.maxLine->setTouchEnabled(false);
        _root.nineLine->setTouchEnabled(true);
        _root.nineLine->setBright(true);
    }
    
    // 九线
    void GameTableUIV88::nineLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED)
        {
            return;
        }
        
        m_currentMaxLine = LINE_COUNT / 2 - 1;
        updateSelectLines();
        
        _root.maxLine->setBright(true);
        _root.maxLine->setTouchEnabled(true);
        _root.nineLine->setTouchEnabled(false);
        _root.nineLine->setBright(false);
    }
    
    //满注按钮
    void GameTableUIV88::maxNoteBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        maxNoteLogic();
    }
    
    //自动按钮
    void GameTableUIV88::autoBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        
        m_bAuto = !m_bAuto;
        if (m_bAuto && _GameTableLogic->getGameStatus() == GS_FREE)
        {
            startBtnCallBack(_root.startBtn, Widget::TouchEventType::ENDED);
        }
        
        _root.autoBtn->setVisible(!m_bAuto);
        _root.autoBtn->setTouchEnabled(!m_bAuto);
        
        _root.cancelBtn->setVisible(m_bAuto);
        _root.cancelBtn->setTouchEnabled(m_bAuto);
    }
    
	//开始按钮
	void GameTableUIV88::startBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        auto startBtn = (Button*)pSender;
        if (type != Widget::TouchEventType::ENDED) {
            if (type == Widget::TouchEventType::BEGAN) {
                
                startBtn->setScale(1.3f);
            }
            else if (type == Widget::TouchEventType::CANCELED)
            {
                
                startBtn->setScale(1.0f);
            }
            return;
        }
        startBtn->setScale(1.0f);
        if (m_currentNoteValue < i64NoteMin || m_currentNoteValue > i64NoteMax) {
            //押注金额异常
            auto prompt = GamePromptLayer::create();
            prompt->showPrompt("下注数值异常");
            return;
        }
        if (m_currentNoteValue * (m_currentMaxLine + 1) > i64MyMoney || i64MyMoney <= 0) {
            //金币不足
            auto prompt = GamePromptLayer::create();
            prompt->showPrompt("金币不足");
            return;
        }
        
        if (nullptr == _GameTableLogic ||
            _GameTableLogic->getGameStatus() != GS_FREE ||  // 游戏状态不正确
            !m_bHasCashIn ||                                // 连线动画没有播完
            !m_bAnimationFinished)                          // 转动动画没有播完
        {
            // 当前状态不可开始
            return;
        }
        
        if (_GameTableLogic)
        {
            m_bHasCashIn = false;
            m_bGameFinished = false;
            struct_c2s_start msg;
            msg.nNoteLines = m_currentMaxLine + 1;
            msg.i64NoteMoney = m_currentNoteValue;
            _GameTableLogic->sendStart(&msg);
        }
        
        hideAllLines();
        if (_gameSettlement) {
            _gameSettlement->hideSettleLayer();
        }
        if (_root.score) {
            _root.score->setString(to_string(0));
        }
        
        _root.EnableButtons(false);
	}

    //玩家坐下(非自己)
	void GameTableUIV88::userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
		addUser(userSit->bDeskStation, userSit->bDeskStation == _GameTableLogic->getMySeatNo());
	}

	//玩家站起
	void GameTableUIV88::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
		if (_GameTableLogic->getGameStatus() == GS_FREE) {

		}
	}

    void GameTableUIV88::addUser(BYTE seatNo, bool bMe) {
//        UserInfoStruct * user = _GameTableLogic->getUserBySeatNo(seatNo);
    }

	
    void GameTableUIV88::dealGameState(struct_s2c_game_state* msg) {
        if (msg == nullptr) {
            return;
        }
    }

	
	void GameTableUIV88::dealGameBaseInfo(struct_s2c_base_info* msg) {
		if (msg == nullptr) {
			return;
		}
		//
        i64NoteUnit = msg->i64NoteUnit;
        i64NoteMin = msg->i64NoteMin;
        i64NoteMax = msg->i64NoteMax;
        i64Jackpot = msg->i64Jackpot;
        i64MyMoney = msg->i64MyMoney;
        nLotteryTypeCount = msg->nLotteryTypeCount;
//        msg->nLineConstitute;
        m_currentNoteValue = i64NoteMin;
        updateJackpot();
        updateMyMoney(msg->i64MyMoney);
        updateSelectNotes();
        loadTurnEx();
	}

	
	void GameTableUIV88::dealGameBegin(struct_s2c_game_begin* msg) {
		if (msg == nullptr) {
			return;
		}
        
        updateMyMoney(msg->i64MyMoney);
        i64MyMoney = msg->i64MyMoney;
        m_i64WinMoney = msg->i64WinMoney;
        memcpy(nOpenLotteryType, msg->nOpenLotteryType, sizeof(nOpenLotteryType));
        memcpy(lineResultNumData, msg->nOpenLineLotteryType, sizeof(lineResultNumData));
        memcpy(nOpenLinePrizeType, msg->nOpenLinePrizeType, sizeof(nOpenLinePrizeType));
        memcpy(nOpenLineCount, msg->nOpenLineCount, sizeof(nOpenLineCount));
        memcpy(nOpenLineRate, msg->nOpenLineRate, sizeof(nOpenLineRate));
        
        bHasMedusa   = (int)(msg->bHasMedusa);
        bIsPetrified = (int)(msg->bIsPetrified);
        
        cleanResults();
        transformOpenLotteryType();
        randomColumnDatasAndCalcTurnCount();
	}
    
    void GameTableUIV88::dealGameFinish(struct_s2c_game_finish* msg) {
        if (msg == nullptr) {
            return;
        }

        m_bGameFinished = true;
        i64MyMoney = msg->i64MyMoney;
        tryShowFreeTurnUI(msg->nFreeCount);
		m_iFreeCount = msg->nFreeCount;
//        updateMyMoney(msg->i64MyMoney);
//        updateMyRealWinMoney(m_i64WinMoney);
    }
    
    void GameTableUIV88::onGameTipExit(tag_s2c_tip_exit* msg) {
        if (msg == nullptr) {
            return;
        }
        tipMsg = msg;

        if (msg->bIsExit == 0) {
            return;
        }

        this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
            tipCallback();
        })));
    }

 

//    //一闪一闪的效果
//    void GameTableUIV88::showLineEffect(int lineNum)
//    {
////        showEffectLine = allLineSpriteVec.at(lineNum);
////        Sequence* seq = Sequence::create(DelayTime::create(0.23f*showEffectTime), CallFunc::create([this] {
////            showEffectLine->setVisible(true);
////            HNAudioEngine::getInstance()->playEffect(SHZ_LINE_MUSIC);//线的声音
////        }), DelayTime::create(0.23f*showEffectTime), CallFunc::create([this] {
//////            showEffectLine->setVisible(false);
//////        }), DelayTime::create(0.23f*showEffectTime), CallFunc::create([this] {
//////            showEffectLine->setVisible(true);
//////        }), DelayTime::create(0.23f*showEffectTime), CallFunc::create([this] {
////            showEffectLine->setVisible(false);
////        }), nullptr);
////        this->runAction(seq);
//    }


    
//    void GameTableUIV88::showAllOpenLineVisble(bool isShow)
//    {
//        for (int i = 0; i < LINE_COUNT; i++)
//        {
////            int linenum = tempLineResultData[i];
////            if (linenum != -1)
////            {
////                showEffectLine = allLineSpriteVec.at(linenum);
////                showEffectLine->setVisible(isShow);
////            }
//        }
//    }
//
//    void GameTableUIV88::setAllLineVisblefalse()
//    {
//        for (int i = 0; i < LINE_COUNT; i++)
//        {
////            showEffectLine = allLineSpriteVec.at(i);
////            showEffectLine->setVisible(false);
//        }
//    }

    void GameTableUIV88::tipCallback() {
		if (tipMsg->bIsExit == 1) {
			//准备关闭掉
			//m_PrepareBtn->setVisible(false);

			auto layer = GamePromptLayer::create();
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

    void GameTableUIV88::initData() {
		//m_vGameStatePosition.clear();
        m_i64TipMoney = 0;
//        m_GameStatus = GS_FREE;
        _gameSettlement = nullptr;
        m_audioEffectID = 0;
        m_TextTip = nullptr;
        i64Reward = 0;
        i64MyMoney = 0;
        memset(nOpenLotteryType, 0, sizeof(nOpenLotteryType));
        memset(nOpenLotteryTypeColRow, 0, sizeof(nOpenLotteryTypeColRow));
        memset(lineResultNumData, 0, sizeof(lineResultNumData));
        memset(nOpenLinePrizeType, 0, sizeof(nOpenLinePrizeType));
        memset(nOpenLineCount, 0, sizeof(nOpenLineCount));
        memset(nOpenLineRate, 0, sizeof(nOpenLineRate));
        m_vPanelTurnArea.clear();
        m_vTurnIcon.clear();
        m_vTurnIconPos.clear();
        m_vTurnUnitCount.clear();
        m_nHasTurnUnitCount = 0;
        m_fTurnUnitTime = 0.1f;
        m_currentMaxLine = LINE_COUNT - 1;
        
//        i64MyMoney = 0;
        i64NoteUnit = 0;
        i64NoteMin = 0;
        i64NoteMax = 0;
        i64Jackpot = 0;
        m_currentNoteValue = 0;
        
        tipMsg = nullptr;
	}

	void GameTableUIV88::resetUINewGame() {
		
		//resetAllNotesAnimationAndClean();
	}

	void GameTableUIV88::resetData() {


	}
    
    void GameTableUIV88::turnUpdate(float dt) {
        //累计
        m_nHasTurnUnitCount++;
        
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            for (int j = 0; j < nLotteryTypeCount; j++) {
                if (m_nHasTurnUnitCount <= m_vTurnUnitCount.at(i)) {
                    auto img = m_vTurnIcon.at(i).at(j);
                    if (img) {
                        
                        img->runAction(Sequence::create(MoveBy::create(0.1f, Vec2(0, -ICON_HEIGHT)), CallFunc::create([=]() {
                            //更新index
                            int nOldTag = m_vTurnIcon.at(i).at(j)->getTag();
                            int nNewTag = (nOldTag - 1) < 0 ? (nLotteryTypeCount - 1) : (nOldTag - 1);
                            m_vTurnIcon.at(i).at(j)->setTag(nNewTag);
                            
                            //一轮结束
                            if (j == nLotteryTypeCount - 1) {
                                for (int k = 0; k < nLotteryTypeCount; k++) {
                                    int curTag = m_vTurnIcon.at(i).at(k)->getTag();
                                    m_vTurnIcon.at(i).at(k)->setPosition(m_vTurnIconPos.at(curTag));
                                }
                            }
                        }), nullptr));
                    }
                    
                    if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(i))
                    {
                        char str[128] = {0};
                        for (auto ii = 0; ii < 3; ++ii)
                        {
                            memset(str, 0, sizeof(str));
                            sprintf(str, kIconTypeTpl, nOpenLotteryTypeColRow[i][ii]);
                            //                            auto img = m_vTurnIcon.at(i).at(ii);
                            auto img = ImageView::create(str);
                            m_vTurnIcon.at(i).at(ii)->getParent()->addChild(img);
                            img->loadTexture(str);
                            img->setTag(nOpenLotteryTypeColRow[i][ii] + 8888);
                            auto staticPos = m_vTurnIconPos.at(ii);                           
                            img->setPosition(staticPos);
                            img->runAction(Sequence::create(MoveTo::create(0.10f,Vec2(staticPos.x,staticPos.y - 30)),DelayTime::create(0.1f),MoveTo::create(0.10f,staticPos),NULL));
                            _resultImg.push_back(img);
                        }
                        
                        for (auto ik = 0; ik < nLotteryTypeCount; ++ik)
                        {
                            m_vTurnIcon.at(i).at(ik)->setVisible(false);
                        }
                    }
                }
            }
        }
        
        //最后格子停下来后
        if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(LOTTERY_COLUMN_COUNT - 1))// && j == (LOTTERY_TYPE_COUNT_ - 1))
        {
            unschedule(schedule_selector(GameTableUIV88::turnUpdate));
            updateMedusaSh();
            updateOpenLinesEffect();
            m_bAnimationFinished = true;
        }
    }
    
    void GameTableUIV88::setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize) {
        float fStartPosY = parentSize.height / 2 - ICON_HEIGHT;
        
        for (int i = 0; i < (nLotteryTypeCount == 0 ? 3:nLotteryTypeCount); i++) {
            vIconPos.push_back(Vec2(parentSize.width/2, fStartPosY + i*ICON_HEIGHT));
        }
    }
    
    void GameTableUIV88::setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum) {
        for (int i = 0; i < LOTTERY_COUNT; i++) {
            //整圈数
            int nCount = (nRoundMinNum + i)*nLotteryTypeCount;
            //开的那圈
            nCount += nLotteryTypeCount - vCurBeginIcon.at(i) + vCurEndIcon.at(i);
            vTurnUnitCount.push_back(nCount);
        }
    }
    
    void GameTableUIV88::checkTurnIsSuccess() {
    }
    
    void GameTableUIV88::transformOpenLotteryType() {
        memset(nOpenLotteryTypeColRow, 0, sizeof(nOpenLotteryTypeColRow));
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            for (int j = 0; j < 3; j++) {
                int index = (3-j-1)*5+i;
                if (index >=0 && index < LOTTERY_COUNT) {
                    nOpenLotteryTypeColRow[i][j] = nOpenLotteryType[index];
                }
            }
        }
    }
    
    void GameTableUIV88::randomColumnDatasAndCalcTurnCount() {
        HNAudioEngine::getInstance()->playEffect(_startLoop);
        m_bAnimationFinished = false;
        char str[128] = {0};
        if (m_vTurnIcon.size() > 0) {
            for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                int index = 0;
                for (int j = 0; j < nLotteryTypeCount; j++) {
                    memset(str, 0, sizeof(str));
                    auto img = m_vTurnIcon.at(i).at(j);
                    if (img) {
                        int imgTag = img->getTag();
                        if (imgTag == 0 || imgTag == 1 || imgTag == 2 || imgTag == 3) {
                            continue;
                        }
                        if (imgTag >= nLotteryTypeCount - 3) {
                            sprintf(str, kIconTypeTpl, nOpenLotteryTypeColRow[i][index++]);
                        }
                        else {
                            sprintf(str, kIconTypeTpl, rand() % nLotteryTypeCount);
                        }
                        img->loadTexture(str);
                    }
                }
            }
        }

        //转动数据
        m_nHasTurnUnitCount = 0;
        m_vTurnUnitCount.clear();
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            int nCount = (2 + i) * nLotteryTypeCount;
            nCount += (nLotteryTypeCount - 3);
            m_vTurnUnitCount.push_back(nCount);
        }
        //启动转盘
        m_nHasTurnUnitCount = 0;
        this->runAction(Sequence::create(CallFunc::create([=]() {
            for (auto layout : m_vPanelTurnArea) {
                if (layout) {
                    auto nCount   = layout->getChildrenCount();
                    auto childvec = layout->getChildren();
                    for (int n = 0;n < nCount;n++) {
                        auto moveDown = MoveBy::create(0.15f, Vec2(0, -30));
                        auto icon = childvec.at(n);
                        icon->runAction(Sequence::create(moveDown, DelayTime::create(0.05f),  moveDown->reverse(), nullptr));
                    }
                    
               }
            }
        }), DelayTime::create(0.5f), CallFunc::create([=]() {
            schedule(schedule_selector(GameTableUIV88::turnUpdate), 0.05f);
        }), nullptr));
    }
    
    void GameTableUIV88::updateOpenLinesEffect() {
        int openLinesCount = 0;
        for (int i = 0; i < LINE_COUNT; i++) {
            auto img = m_vAllLines[i];
            if (img && lineResultNumData[i] != -1) {
                openLinesCount++;
                img->setVisible(true);
                img->runAction(Sequence::create(Blink::create(kResultLineBlinkDuration, kResultLineBlinkTimes), nullptr));
            }
        }
        
        updateMyMoney(i64MyMoney);
        //updateMyRealWinMoney(m_i64WinMoney - i64WinJackpot);
        showJackpot();
        
        if (openLinesCount > 0) {
            //m_bHasCashIn = false;
            HNAudioEngine::getInstance()->playEffect(Medusa_bingo);
            this->runAction(Sequence::create(DelayTime::create(kResultLineBlinkDuration + 0.02f), CallFunc::create([=]() {
                
                hideAllLines();
                this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=](){
                    m_bHasCashIn = true;
                }), NULL));
                // 不显示结算界面
                //updateSettlementLayer();
            }), nullptr));
        }
        else
        {
            m_bHasCashIn = true;
        }
    }
    
    void GameTableUIV88::updateSettlementLayer() {
        if (_gameSettlement) {
            struct_s2c_game_begin msg;
            memcpy(msg.nOpenLineLotteryType, lineResultNumData, sizeof(msg.nOpenLineLotteryType));
            memcpy(msg.nOpenLinePrizeType, nOpenLinePrizeType, sizeof(msg.nOpenLinePrizeType));
            memcpy(msg.nOpenLineCount, nOpenLineCount, sizeof(msg.nOpenLineCount));
            memcpy(msg.nOpenLineRate, nOpenLineRate, sizeof(msg.nOpenLineRate));
            _gameSettlement->showSettleLayerAndUpdateData(msg);
            
            runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=](){
                m_bHasCashIn = true;
            }), nullptr));
        }
    }
    
    void GameTableUIV88::addLineLogic() {
        m_currentMaxLine++;
        if (m_currentMaxLine >= LINE_COUNT) {
            m_currentMaxLine = 0;
        }
        updateSelectLines();
    }
    
    void GameTableUIV88::maxLineLogic() {
        m_currentMaxLine = LINE_COUNT - 1;
        updateSelectLines();
    }
    
    void GameTableUIV88::updateSelectLines() {
        for (int i = 0; i < LINE_COUNT; i++) {
            auto lineImg = m_vAllLines[i];
            if (lineImg) {
                lineImg->setVisible(i <= m_currentMaxLine);
            }
        }
        if (_root.addLineCount) {
            _root.addLineCount->setString(to_string(m_currentMaxLine+1));
        }
    }
    
    void GameTableUIV88::hideAllLines() {
        for (int i = 0; i < LINE_COUNT; i++) {
            auto lineImg = m_vAllLines[i];
            if (lineImg) {
                lineImg->setVisible(false);
            }
        }
    }
    
    void GameTableUIV88::updateMyMoney(long long money) {
//        i64MyMoney = money;
        if (_root.myMoney) {
            _root.myMoney->setString(Tools::addComma(money));
        }
    }

    void GameTableUIV88::addNoteLogic() {
        if (m_currentNoteValue < i64NoteMin) {
            m_currentNoteValue = i64NoteMin;
        }
        m_currentNoteValue += i64NoteUnit;
        if (m_currentNoteValue > i64NoteMax) {
            m_currentNoteValue = i64NoteMin;
        }
        updateSelectNotes();
    }
    
    void GameTableUIV88::maxNoteLogic() {
        m_currentNoteValue = i64NoteMax;
        updateSelectNotes();
    }
    
    void GameTableUIV88::updateSelectNotes() {
        if (_root.addNoteCount) {
            _root.addNoteCount->setString(to_string(m_currentNoteValue));
        }
    }
    
    void GameTableUIV88::updateJackpot()
    {
        if (_root.jackpot)
        {
            _root.jackpot->setString(to_string(i64Jackpot));
        }
    }

    void GameTableUIV88::updateMyRealWinMoney(long long money) {
        auto coin = FloatCoin::create();
        if (coin) {
            coin->openParent(this, Vec2(WIN_SIZE.width*0.7f, WIN_SIZE.height-120), ZOrder_Float_Coin);
            coin->updateCoin(money);
        }
        if (_root.score) {
            _root.score->setString(to_string(money));
        }
        //if (money > 0) {
            //GameRewardEffect::createEffect(this);
        //}
    }
    
    void GameTableUIV88::update(float dt)
    {
        // enable start button
        if (m_bGameFinished &&
            m_bAnimationFinished &&
            !m_bAuto &&
            _GameTableLogic->getGameStatus() == GS_FREE)
        {
			if (m_iFreeCount > 0) _root.EnableButtons(false);
			else _root.EnableButtons(true);
        }
        
        // 自动
        if (i64MyMoney > 0 &&
            m_currentNoteValue * (m_currentMaxLine + 1) < i64MyMoney &&
            m_bHasCashIn &&
            m_bAuto &&
            _GameTableLogic->getGameStatus() == GS_FREE)
        {
            startBtnCallBack(_root.startBtn, Widget::TouchEventType::ENDED);
        }
    }
    
    void GameTableUIV88::showJackpot()
    {
        if (i64WinJackpot > 0)
        {
            auto jackpotNode = CSLoader::createNode(kJackpotUI);
            auto action = CSLoader::createTimeline(kJackpotUI);
            if (jackpotNode == nullptr || action == nullptr)
            {
                return;
            }
            
            auto viewSize = Director::getInstance()->getVisibleSize();
            jackpotNode->setName("JackpotUI");
            jackpotNode->setPosition(viewSize/2);
            this->addChild(jackpotNode, ZOrder_Float_Coin - 1);
            jackpotNode->runAction(action);
            action->gotoFrameAndPlay(0);
            
            auto txtJackpot = dynamic_cast<Text*>(jackpotNode->getChildByName("txtJackpot"));
            if (txtJackpot != nullptr)
            {
                txtJackpot->setString(Tools::addComma(i64WinJackpot));
            }
            
            // hide
            this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=](){
                this->removeChildByName("JackpotUI");
            }),NULL));
            
            
        }
    }
    
    void GameTableUIV88::cleanResults()
    {
        for (auto img : _resultImg)
        {
            if (nullptr != img)
            {
                img->removeFromParent();
            }
        }
        
        _resultImg.clear();
        
        for (auto iconVec : m_vTurnIcon)
        {
            for (auto icon : iconVec)
            {
                if(nullptr != icon)
                    icon->setVisible(true);
            }
        }
        
        for (int index = LOTTERY_COLUMN_COUNT - 2; index <= LOTTERY_COLUMN_COUNT - 1 ; index ++) {
            auto panel = m_vPanelTurnArea.at(index);
            if (panel) {
                auto imgMedusa = panel->getChildByTag(9999 + index);
                if (imgMedusa) {
                    imgMedusa->removeFromParent();
                }
            }
        }
    }
    void GameTableUIV88::showEffectAnimation(int index,Node * node)// index == 2大石化 1小石化
    {
        if (index == 0) {
            return;
        }
        
        std::string animaName;
        auto removeTime = 0.6;
        auto pos = node->getContentSize()/2;
        auto anchr = Vec2(0.5,0.5);
        if (index == 1) {
            animaName=ksmallshEffect;
        
        }
        else if (index == 2)
        {
            animaName=klargeshEffect;
        }
        else{
            animaName=kMedusaEffect;
            removeTime = 2;
            pos = Vec2(WIN_SIZE.width/1.35,WIN_SIZE.height/2 - 30);
            anchr = Vec2(1,0);
        }
        
        auto anim = CSLoader::createNode(animaName);
        auto action = CSLoader::createTimeline(animaName);
       
        anim->setAnchorPoint(anchr);
        anim->setPosition(pos);
        node->addChild(anim,1000);
        anim->runAction(action);
        anim->runAction(Sequence::create(DelayTime::create(removeTime),RemoveSelf::create(),NULL));
        action->gotoFrameAndPlay(0,false);
        
        
    }
    void GameTableUIV88::updateMedusaSh()
    {
        if (bHasMedusa == 1 || bIsPetrified == 1) {
            runAction(Sequence::create(CallFunc::create([&](){
                
                showEffectAnimation(3,this);
            }),DelayTime::create(1.3),CallFunc::create([&](){
                
                if (bHasMedusa == 1) {
                    
                    for (int m = LOTTERY_COLUMN_COUNT - 2; m <= LOTTERY_COLUMN_COUNT - 1; m++) {
                        auto panel = m_vPanelTurnArea.at(m);
                        if (panel) {
                            auto img = ImageView::create("Games/31000510/type/000.png");
                            panel->addChild(img,999,9999+m);
                            showEffectAnimation(2,img);
                            img->setAnchorPoint(Vec2(0,0));
                            img->setPosition(Vec2(-20,-5));
                        }
                    }
                    
                    
                }
                if (bIsPetrified == 1) {
                    for (auto img : _resultImg)
                    {
                        if (nullptr != img)
                        {
                            auto imgTag = img->getTag();
                            if (imgTag == 6 + 8888 || imgTag == 7 + 8888) {
                                showEffectAnimation(1,img);
                                img->loadTexture(StringUtils::format(shIconType,imgTag - 8888));
                            }
                        }
                    }
                    
                }
                
                
            }),DelayTime::create(1), CallFunc::create([&](){
                updateMyRealWinMoney(m_i64WinMoney - i64WinJackpot);
                updateMyMoney(i64MyMoney);
            }), NULL));
        }
        else{
            
            updateMyRealWinMoney(m_i64WinMoney - i64WinJackpot);
            updateMyMoney(i64MyMoney);
        }
    }
    
}
