#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNCommon/HNConverCode.h"

#include "PubgGameTableUIV88.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "PubgSettlementV88.h"
#include "PubgFinalModeReward.hpp"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "base/CCRef.h"
#include "HNLobby/pattern/CommandManager.h"
#define ICON_HEIGHT 128

namespace Pubg {


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
    
    const static char* Tiger_BGM = "Games/31000511/music/bg.mp3";
    const static char* Pubg_Plane_BGM = "Games/31000511/music/plane.mp3";
    const static char* Pubg_Gun_BGM = "Games/31000511/music/gun.mp3";
    const static char* Tiger_bingo = "v66/music/CSLL/CSLL_bingo.mp3";
    const static char* Tiger_startLoop = "v66/music/CSLL/CSLL_startLoop.mp3";
    const static char* kCsbRoot = "Games/31000511/Node_root.csb";
    const static char* kIconTypeTpl = "Games/31000511/type/%d.png";
    const static char* kHelpFile = "Games/31000511/help/help.png";
    const static char* kJackpotUI = "common/animation/jackPot/Jackpot.csb";
    
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
        , m_bAnimationFinished(true)
        , m_autoRotation(false)
        , m_autoRotationTime(0)
        , m_bGameFinished(true)
        , m_i64WinMoney(0)
        , i64WinJackpot(0)
        ,m_bAirDrop(false)
        ,nGunsignPositionSmall(0)
        ,nGunsignPositionBig(0)
        ,m_bFinalMode(false)
        ,m_bFirstMod(false)
        ,m_bFinishFinalMode(false)
        ,m_bFinalModelNum(0)
    
    {
		
	}

	GameTableUIV88::~GameTableUIV88() {

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

    void GameTableUIV88::createRootLayer() {
//        //background image
//        ImageView* bg = ImageView::create("v88/Tiger/ui/bg.png");
//        if (bg) {
//            _root.bg = bg;
//            bg->setPosition(WIN_SIZE/2);
//            addChild(bg, ZOrder_Bg);
//        }
        //middle Layer
        Node* middle = CSLoader::createNode(kCsbRoot);
        if (middle) {
            _root.middleNode = middle;
            middle->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2));
            addChild(middle, ZOrder_Middle_Layer);
            //
            auto bg = middle->getChildByName("Image_bg");
            if (bg) {
                
                
                char str[128] = {0};
                for (int i = 0; i < LINE_COUNT; i++) {
                    memset(str, 0, sizeof(str));
                    sprintf(str, "Image_line_%d", i+1);
                    auto line = dynamic_cast<ImageView*>(bg->getChildByName(str));
                    if (line) {
                        line->setVisible(false);
                        line->setLocalZOrder(1);
                        m_vAllLines[i] = line;
                    }
                }
                
                ImageView* bigKillNumBg = dynamic_cast<ImageView*>(bg->getChildByName("Image_1"));
                if (bigKillNumBg) {
                    _root.bigKillNumBg = bigKillNumBg;
                    Text* bigKillNum = dynamic_cast<Text*>(_root.bigKillNumBg->getChildByName("Text_1"));
                    if (bigKillNum) {
                        _root.bigKillNum = bigKillNum;
                    }
                    _root.bigKillNumBg->setVisible(false);
                }
                
                Text* myName = dynamic_cast<Text*>(bg->getChildByName("Image_53")->getChildByName("txtName"));
                if (myName) {
                    _root.myName = myName;
                }
                Text* myMoney = dynamic_cast<Text*>(bg->getChildByName("Image_54")->getChildByName("txtMoney"));
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
                    _root.startBtn->setTitleText("");
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

                Button* maxLine = dynamic_cast<Button*>(bg->getChildByName("btnMaxLine"));
                if (maxLine) {
                    _root.maxLine = maxLine;
                    maxLine->setVisible(false);
                    maxLine->setTouchEnabled(false);
                    maxLine->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::maxLineBtnCallBack, this));
                }
                
//                // 九线
                auto nineLine = dynamic_cast<Button*>(bg->getChildByName("btnNineLine"));
                if (nineLine)
                {
                    _root.nineLine = nineLine;
                    nineLine->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::nineLineBtnCallBack, this));
                }
 
               Button* addLine = dynamic_cast<Button*>(bg->getChildByName("btnAddLine"));
               if (addLine) {
                   _root.addLine = addLine;
                   addLine->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::addLineBtnCallBack, this));
                }
                Button* maxNote = dynamic_cast<Button*>(bg->getChildByName("btnMaxNote"));
                if (maxNote) {
                    _root.maxNote = maxNote;
                    maxNote->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::maxNoteBtnCallBack, this));
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
        vector<ImageView*> vIcon;
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            if (_root.middleNode) {
                auto bg = _root.middleNode->getChildByName("Image_bg");
                if (bg) {
                    
                    _root.panelBg = dynamic_cast<Layout*>(bg->getChildByName("Panel_5"));
                    if (_root.panelBg) {
                        _root.panelBg->setClippingEnabled(true);
                        //转动的容器
                        memset(str, 0, sizeof(str));
                        sprintf(str, "Panel_%d", i);
                        Layout* layout = dynamic_cast<Layout*>(_root.panelBg->getChildByName(str));
                        if (layout) {
                            //layout->setClippingEnabled(true);
                            m_vPanelTurnArea.push_back(layout);
                        }
                    }
                    
                }
            }
            //
            //转动的icon
            if (m_vPanelTurnArea.size() > 0) {
                setTurnIcomPosition(m_vTurnIconPos, m_vPanelTurnArea.at(0)->getContentSize(),i);
            }
            
           
            
            vIcon.clear();
            for (int j = 0; j < LOTTERY_TYPE_COUNT_Pubg; j++) {
                sprintf(str, kIconTypeTpl, j);
                ImageView* icon = ImageView::create(str);
                icon->setTag(j);
                m_vPanelTurnArea.at(i)->addChild(icon);
                icon->setPosition(m_vTurnIconPos.at(j));
                
                vIcon.push_back(icon);
            }
            
            m_vTurnIcon.push_back(vIcon);
        }
       // auto pos = m_vPanelTurnArea.at(0)->convertToWorldSpace(m_vTurnIcon.at(0).at(0)->getPosition());
       
        _root.smallMirror = ImageView::create("Games/31000511/ui/smallMirror.png");
        _root.smallMirror->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        auto bg = _root.middleNode->getChildByName("Image_bg");
        if (bg) {
            auto pos = m_vPanelTurnArea.at(0)->convertToWorldSpace(m_vTurnIcon.at(0).at(3)->getPosition());
            _root.smallMirror->setPosition(pos);
            bg->addChild(_root.smallMirror);
        }
        
        _root.bigMirror = ImageView::create("Games/31000511/ui/bigMirror.png");
        _root.bigMirror->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
        if (bg) {
            auto pos = m_vPanelTurnArea.at(2)->convertToWorldSpace(m_vTurnIcon.at(1).at(2)->getPosition());
            _root.bigMirror->setPosition(Vec2(pos.x,pos.y));
            bg->addChild(_root.bigMirror);
            _root.bigMirror->setVisible(false);
        }
        
        _root.bigKillModeBg = ImageView::create("Games/31000511/ui/bigKill.png");
        _root.bigKillModeBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _root.bigKillModeBg->setPosition(WIN_SIZE/2);
        this->addChild(_root.bigKillModeBg,ZOrder_Middle_Layer+1);
        bigKillModeShow(false);
    }
    
    void GameTableUIV88::bigKillModeShow(bool show)
    {
        if (_root.bigKillModeBg) {
            _root.bigKillModeBg->stopAllActions();
            _root.bigKillModeBg->setVisible(show);
            if (show) {
                _root.bigKillModeBg->runAction(RepeatForever::create(Sequence::create(FadeOut::create(0.5f),DelayTime::create(0.5f),FadeIn::create(0.5f), NULL)));
            }
        
        }
        
    }
    
    void GameTableUIV88::smallMirrorMove(int idx)
    {
            for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                for (int j = 0; j < 4; j++) {
                    int index = (4-j-1)*5+i;
                    if (index == idx) {
                        auto pos = m_vPanelTurnArea.at(i)->convertToWorldSpace(m_vTurnIconPos.at(j));
                        _root.smallMirror->runAction(Sequence::create(MoveTo::create(4.5f,pos),CallFunc::create([this](){
                            if (m_bAirDrop) {
                                HNAudioEngine::getInstance()->playEffect(Pubg_Gun_BGM);
                                ImageView* boom = ImageView::create("Games/31000511/ui/tankong.png");
                                if (boom) {
                                    boom->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                    boom->setPosition(_root.smallMirror->getContentSize()/2);
                                    _root.smallMirror->addChild(boom);
                                    boom->runAction(Sequence::create(ScaleTo::create(0.1,0.8),DelayTime::create(0.1f),ScaleTo::create(0.1,1.0),NULL));
                                    boom->setName("boomImg");
                                }
                            }
                            
                            
                        }),NULL));
                    }
                }
            }
    }
   void GameTableUIV88::BigMirrorMove(int idx)
    {
            for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                for (int j = 0; j < 4; j++) {
                    int index = (4-j-1)*5+i;
                    if (index == idx) {
                        auto pos = m_vPanelTurnArea.at(i)->convertToWorldSpace(m_vTurnIconPos.at(j));
                        _root.bigMirror->runAction(Sequence::create(MoveTo::create(4.5f,pos),CallFunc::create([this](){
                        }),NULL));
                    }
                }
            }
        
    }
    
    void GameTableUIV88::createGameSettlementLayer() {
        _gameSettlement = nullptr;
        PubgSettlementV88* settle = PubgSettlementV88::create();
        if (settle) {
            settle->setPosition(WIN_SIZE/2);
            _gameSettlement = settle;
            _gameSettlement->hideSettleLayer();
            addChild(_gameSettlement, ZOrder_SettlementLayer);
        }
    }

    void GameTableUIV88::onEnter() {
		HNGameUIBase::onEnter();
        HNAudioEngine::getInstance()->playBackgroundMusic(Tiger_BGM, true);

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
		//////////////////////////////
		/////////////////////
		_GameTableLogic->sendGameInfo();
	}
    void GameTableUIV88::closeLoadingCallBack()
    {
        
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
				else
                {
					// 游戏不在空闲时期，不允许离开
					if (_GameTableLogic->getGameStatus() != GS_FREE && _GameTableLogic->getGameStatus() != GS_FINAL_MODE) {
						auto layer = GamePromptLayer::create();
						layer->showPrompt(GetText("limit_leave_in_game"));
						return;
					}
					else {
						_GameTableLogic->sendUserUp();
					}
				}
			});
			toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
				IGameSet* gameSet = GameSetFactory::createWithVersion();
                gameSet->setBgPath(Tiger_BGM);
				gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
			});
            toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
                auto helpLayer = HN::HelpLayer::create();
                helpLayer->setContentOnView(kHelpFile);
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
        unschedule(schedule_selector(GameTableUIV88::updateTip));
        unschedule(schedule_selector(GameTableUIV88::turnUpdate));
	}

	void GameTableUIV88::onExitTransitionDidStart() {
		HNGameUIBase::onExitTransitionDidStart();
		HNAudioEngine::getInstance()->stopBackgroundMusic();
	}

//    void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
////        if (bVisible) {
////            if (!m_Notice) {
////                m_Notice = GameNotice::create();
////                m_Notice->setLocalZOrder(NOTICE_ZORDER);
////                m_Notice->setName("GameNotice");
////            }
////            m_Notice->openNotic(this, 10000, message, nCount, nType);
////        }
//    }

	//按钮回调
	////////////////////////////////////////////////////////////////////////////
	//加线按钮
	void GameTableUIV88::addLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}
        addLineLogic();
	}
    
    // 减线按键
    void GameTableUIV88::subLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED)
        {
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
        
        _root.nineLine->setTouchEnabled(true);
        _root.nineLine->setVisible(true);
        _root.maxLine->setTouchEnabled(false);
        _root.maxLine->setVisible(false);
    }
    
    void GameTableUIV88::nineLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED)
        {
            return;
        }
        
        m_currentMaxLine = 8;
        updateSelectLines();
        
        _root.nineLine->setTouchEnabled(false);
        _root.nineLine->setVisible(false);
        _root.maxLine->setTouchEnabled(true);
        _root.maxLine->setVisible(true);
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
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}
        
        if (m_bFinalMode) { //大逃杀模式
            if (_GameTableLogic->getGameStatus() == GS_FINAL_MODE) {
                
                hideAllLines();
                if (_gameSettlement) {
                    _gameSettlement->hideSettleLayer();
                }
                if (_root.score) {
                    _root.score->setString(to_string(0));
                }
                
                if (_GameTableLogic) {
                    m_bHasCashIn = true;
                    m_bGameFinished = false;
                    _root.EnableButtons(false);
                    _GameTableLogic->sendFinalStart();
                    
                    m_bFinalModelNum++;
                    _root.startBtn->setTitleText(StringUtils::format("%d/7",m_bFinalModelNum));
                    
                }
                
            }
            
            return;
        }
        
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
        
        
        
        hideAllLines();
        if (_gameSettlement) {
            _gameSettlement->hideSettleLayer();
        }
        if (_root.score) {
            _root.score->setString(to_string(0));
        }
        
        if (_root.smallMirror) {
            auto imgBoom = _root.smallMirror->getChildByName("boomImg");
            if (imgBoom) {
                imgBoom->removeFromParent();
            }
        }
        //startCallFakeRotate();
        if (_GameTableLogic) {
            m_bHasCashIn = false;
            m_bGameFinished = false;
            _root.EnableButtons(false);
            struct_c2s_start msg;
            msg.nNoteLines = m_currentMaxLine + 1;
            msg.i64NoteMoney = m_currentNoteValue;
            _GameTableLogic->sendStart(&msg);
        }
       
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
        
        m_bFinalMode = false;
        if (msg->bGameState == GS_FINAL_MODE) {
            m_bFinalMode = true;

        }
//
//        if (m_bAuto && _GameTableLogic->getGameStatus() == GS_FREE)
//        {
//            startBtnCallBack(_root.startBtn, Widget::TouchEventType::ENDED);
//        }
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
//        msg->nLineConstitute;
        m_currentNoteValue = i64NoteMin;
        i64MyMoney = msg->i64MyMoney;
        updateJackpot();
        updateMyMoney(msg->i64MyMoney);
        updateSelectNotes();
	}

	
	void GameTableUIV88::dealGameBegin(struct_s2c_game_begin* msg) {
		if (msg == nullptr) {
			return;
		}
        
        updateMyMoney(msg->i64LatestMoney - msg->i64WinMoney);
        
        i64MyMoney = msg->i64LatestMoney;
        m_i64WinMoney = msg->i64WinMoney;
        i64WinJackpot = msg->i64WinJackpot;
        i64Jackpot    = msg->i64LatestJackpot;
        
        nGunsignPositionSmall = msg->nGunsignPosition;
        memcpy(nOpenLotteryType, msg->nOpenLotteryType, sizeof(nOpenLotteryType));
        memcpy(lineResultNumData, msg->nOpenLineLotteryType, sizeof(lineResultNumData));
        memcpy(nOpenLineCount, msg->nOpenLineCount, sizeof(nOpenLineCount));
        memcpy(nOpenLineRate, msg->nOpenLineRate, sizeof(nOpenLineRate));
        
        m_bAirDrop = nOpenLotteryType[nGunsignPositionSmall] == 10; //瞄准镜锁定类型10则触发空投
        //runAction(Sequence::create(DelayTime::create(1.20f),CallFunc::create([this](){
            m_bFirstMod = true;
            //cleanResults();
            transformOpenLotteryType();
            randomColumnDatasAndCalcTurnCount();
        
        smallMirrorMove(nGunsignPositionSmall);
        
        
        
        //}), NULL));//延时处理
	}
    
    void GameTableUIV88::airDropFunc()
    {
        //计算空投的几个位置
        vector<int>dropPos;
        dropPos.clear();
        
        int a[4] = {-1,1,-5,5};
        for (int i = 0; i < 4; i++) {
            if ((nGunsignPositionSmall  == 0 || nGunsignPositionSmall == 5 || nGunsignPositionSmall == 10 || nGunsignPositionSmall == 15) && a[i] == -1) {
                continue;
            }
            else if ((nGunsignPositionSmall  == 4 || nGunsignPositionSmall == 9 || nGunsignPositionSmall == 14) && a[i] == 1)
            {
                continue;
            }
            
            int idx = nGunsignPositionSmall + a[i];
            if ( idx >= 0 && idx <= 19) {
                dropPos.push_back(idx);
            }
        }
        
        ImageView * plane = ImageView::create("Games/31000511/ui/plane.png");
        if (plane) {
            plane->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            plane->setPosition(Vec2(WIN_SIZE.width/2,0));
            this->addChild(plane,ZOrder_Middle_Layer+2);
            HNAudioEngine::getInstance()->playEffect(Pubg_Plane_BGM);
            this->runAction(Sequence::create(CallFunc::create([plane](){
                plane->runAction(Sequence::create(MoveBy::create(4.0,Vec2(0,WIN_SIZE.height*2)),DelayTime::create(4),RemoveSelf::create(),NULL));
            }), DelayTime::create(2.0f), CallFunc::create([this,dropPos](){
                
                vector<Vec2> vecPos;
                vecPos.clear();
                
                vector<ImageView*> imgVec;
                imgVec.clear();
                
                for (int m = 0; m < dropPos.size(); m++) {
                    int idx = dropPos.at(m);
                    
                    for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                        for (int j = 0; j < 4; j++) {
                            int index = (4-j-1)*5+i;
                            if (index == idx) {
                                
                                auto pos = m_vPanelTurnArea.at(i)->convertToWorldSpace(m_vTurnIconPos.at(j));
                                vecPos.push_back(pos);
                                
                                auto result =(ImageView*)(m_vPanelTurnArea.at(i)->getChildByTag(1234567 + j));
                                if (result) {
                                    auto action = Repeat::create(Sequence::create(ScaleTo::create(0.50f,1.2),ScaleTo::create(0.50f,0.8),NULL),4);
                                    action->setTag(333);
                                    result->runAction(action);
                                    
                                    imgVec.push_back(result);
                                }
                               
                                
                            }
                        }
                    }
                }
                
                if (vecPos.size() > 0) {
                    for (int n = 0; n < vecPos.size(); n++) {
                        auto img = ImageView::create("Games/31000511/ui/shan.png");
                        if (img) {
                            img->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                            img->setPosition(Vec2(WIN_SIZE.width/2,WIN_SIZE.height/2+150));
                            this->addChild(img,ZOrder_Middle_Layer + 1);
                            img->runAction(Sequence::create(MoveTo::create(1.0f,Vec2(vecPos.at(n).x,vecPos.at(n).y + 40)),DelayTime::create(1.0f),CallFunc::create([imgVec](){
                                if (imgVec.size() > 0) {
                                    for (int j = 0; j < imgVec.size(); j++) {
                                        auto img = (ImageView*)imgVec.at(j);
                                        if (img) {
                                            if (img->getActionByTag(333)) {
                                                img->stopActionByTag(333);
                                            }
                                            img->setScale(1.0);
                                            img->loadTexture("Games/31000511/type/9.png");
                                        }
                                    }
                                }
                                
                            }),FadeOut::create(0.5f),DelayTime::create(1.5f),RemoveSelf::create(),NULL));
                        }
                    }
                }
                
                
            }),DelayTime::create(2.5f),CallFunc::create([this](){
                m_bFirstMod = false;
                updateOpenLinesEffect();
                m_bAnimationFinished = true;
                m_bGameFinished = true;
                bigKillModeFunc();
                
            }),NULL));
        }
        
        
        
    }
    
    void GameTableUIV88::dealGameFinish(struct_s2c_game_finish* msg) {
        if (msg == nullptr) {
            return;
        }
        
        m_bGameFinished = true;
        i64MyMoney = msg->i64MyMoney;
        tryShowFreeTurnUI(msg->nFreeCount);
//        updateMyMoney(msg->i64MyMoney);
    }
    
    void GameTableUIV88::dealGameFinalMode(struct_s2c_finalmode_result *msg)
    { //处理大逃杀
        if (msg == nullptr) {
            return;
        }
        
        i64MyMoney    = msg->i64MyMoney;
        m_i64WinMoney = msg->i64WinMoney;
        nGunsignPositionBig = msg->nGunsightPosition;
        memcpy(nOpenLotteryType, msg->nOpenLotteryType, sizeof(nOpenLotteryType));
       
        int nKillCount = msg->nKillCount;                            //本局击杀个数
        
        m_bFinishFinalMode = msg->bGameOver != 0;                            //大逃杀模式是否结束 非0结束大逃杀 0为大逃杀
       
        nkillCount = msg->nAllKillCount;                        //当前累计的总击杀数
        
        //cleanResults();
        transformOpenLotteryType();
        randomColumnDatasAndCalcTurnCount();
        BigMirrorMove(nGunsignPositionBig);
    }
    
    void GameTableUIV88::onGameTipExit(tag_s2c_tip_exit* msg) {
        if (msg == nullptr) {
            return;
        }
        tipMsg = msg;
        //
        //不退出
        if (msg->bIsExit == 0) {
            return;
        }
        //要退出
        this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
            tipCallback();
        })));
    }

    void GameTableUIV88::dealGameFreeCount(struct_s2c_free_count * msg)
    {
        tryShowFreeTurnUI(msg->nFreeCount);
        m_currentMaxLine = msg->nLines - 1;
        updateSelectLines();
        m_currentNoteValue = msg->i64Notes;
        updateSelectNotes();
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

	void GameTableUIV88::resetData()
    {
    }
    
    void GameTableUIV88::turnUpdate(float dt) {
        //累计
        m_nHasTurnUnitCount++;
        
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            for (int j = 0; j < LOTTERY_TYPE_COUNT_Pubg; j++) {
                if (m_nHasTurnUnitCount <= m_vTurnUnitCount.at(i)) {
                    auto img = m_vTurnIcon.at(i).at(j);
                    if (img) {
                        
                        img->runAction(Sequence::create(MoveBy::create(0.1f, Vec2(0, -ICON_HEIGHT)), CallFunc::create([=]() {
                            //更新index
                            int nOldTag = m_vTurnIcon.at(i).at(j)->getTag();
                            int nNewTag = (nOldTag - 1) < 0 ? (LOTTERY_TYPE_COUNT_Pubg - 1) : (nOldTag - 1);
                            m_vTurnIcon.at(i).at(j)->setTag(nNewTag);
                            
                            //一轮结束
                            if (j == LOTTERY_TYPE_COUNT_Pubg - 1) {
                                for (int k = 0; k < LOTTERY_TYPE_COUNT_Pubg; k++) {
                                    int curTag = m_vTurnIcon.at(i).at(k)->getTag();
                                    m_vTurnIcon.at(i).at(k)->setPosition(m_vTurnIconPos.at(curTag));
                                }
                            }

                        }), nullptr));
                    }
                    
                    if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(i))
                    {
                        char str[128] = {0};
                        for (auto ii = 0; ii < 4; ++ii)
                        {
                            memset(str, 0, sizeof(str));
                            sprintf(str, kIconTypeTpl, nOpenLotteryTypeColRow[i][ii]);
                            //                            auto img = m_vTurnIcon.at(i).at(ii);
                            
                            auto imgChild = m_vTurnIcon.at(i).at(ii)->getParent()->getChildByTag(1234567 + ii);
                            if (imgChild == NULL) {
                                auto img = ImageView::create(str);
                                m_vTurnIcon.at(i).at(ii)->getParent()->addChild(img);
                                img->loadTexture(str);
                                img->setPosition(m_vTurnIconPos.at(ii));
                                img->setTag(1234567 + ii);
                                _resultImg.push_back(img);
                            }
                            
                            
                        }
                        
                        for (auto ik = 0; ik < LOTTERY_TYPE_COUNT_Pubg; ++ik)
                        {
                            m_vTurnIcon.at(i).at(ik)->setVisible(false);
                        }
                    }
                }
            }
        }
        
        //最后格子停下来后
        if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(LOTTERY_COLUMN_COUNT - 1))// && j == (LOTTERY_TYPE_COUNT_Tiger - 1))
        {
            unschedule(schedule_selector(GameTableUIV88::turnUpdate));
            
            if (m_bFirstMod) {
                _root.smallMirror->setVisible(true);
                _root.bigMirror->setVisible(false);
                bigKillModeShow(false);
                if (m_bAirDrop) {
                    airDropFunc();//普通模式空投
                }
                else
                {   float delayTime = m_bFinalMode ? 1.0 : 0;
                    runAction(Sequence::create(CallFunc::create([this](){
                            if (m_bFinalMode) {
                                for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                                    for (int j = 0; j < 4; j++) {
                                        int index = (4-j-1)*5+i;
                                        if (index == nGunsignPositionSmall) {
                                            auto result =(ImageView*)(m_vPanelTurnArea.at(i)->getChildByTag(1234567 + j));
                                            if (result) {
                                                HNAudioEngine::getInstance()->playEffect(Pubg_Gun_BGM);
                                                ImageView* boom = ImageView::create("Games/31000511/ui/tankong.png");
                                                if (boom) {
                                                    boom->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                                    boom->setPosition(result->getContentSize()/2);
                                                    boom->runAction(Sequence::create(ScaleTo::create(0.1,0.8),DelayTime::create(0.1f),ScaleTo::create(0.1,1.0),NULL));
                                                    result->addChild(boom);
                                                }
                                                
                                            }
                                        }
                                    }
                                }
                            }
                        }),DelayTime::create(delayTime),CallFunc::create([this](){
                            
                            if (m_bAuto && m_bFinalMode) {
                                autoBtnCallBack(_root.autoBtn, Widget::TouchEventType::ENDED);
                            }
                            
                            updateOpenLinesEffect();
                            m_bAnimationFinished = true;
                            m_bGameFinished = true;
                            m_bFirstMod = false;
                            
                            bigKillModeFunc(); //由普通模式进入大逃杀模式
                            
                        }),NULL));
                }
                
            }
            else if(m_bFinalMode && !m_bFirstMod)//大逃杀模式摇奖
            {
                
                //updateOpenLinesEffect();
                m_bAnimationFinished = true;
                m_bGameFinished = true;
                bigKillPeople();
            }
            else if (m_bFinishFinalMode) //结束大逃杀模式转换为普通模式
            {
              
                bigKillPeople();
                
                auto layer = PubgFinalRewardLayer::create();
                if (layer) {
                    int rank = 10 - nkillCount;
                    layer->showData(rank,nkillCount,m_i64WinMoney,this);
                }
                
                runAction(Sequence::create(DelayTime::create(1),CallFunc::create([this]{
                    if (m_bAuto) {
                        autoBtnCallBack(_root.autoBtn, Widget::TouchEventType::ENDED);
                    }
                    
                    m_bAnimationFinished = true;
                    m_bGameFinished = true;
                    _root.bigKillNumBg->setVisible(false);
                    _root.smallMirror->setVisible(true);
                    _root.bigMirror->setVisible(false);
                    bigKillModeShow(false);
                    _root.startBtn->setTitleText("");
                    
                    m_bFinishFinalMode = false;
                    
                }),NULL));
               
                
            }
            

        }
    }
    void GameTableUIV88::bigKillModeFunc()
    {
        if (m_bFinalMode) {
            
            ImageView* tip = ImageView::create("Games/31000511/ui/tip.png");
            if (tip) {
                tip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                tip->setPosition(WIN_SIZE/2);
                this->addChild(tip,ZOrder_Bottom_Layer + 10);
                
                tip->runAction(Sequence::create(DelayTime::create(1.5f),RemoveSelf::create(),NULL));
            }
            
            _root.smallMirror->setVisible(false);
            _root.bigMirror->setVisible(true);
            bigKillModeShow(true);
            
            m_bFinalModelNum = 0;
            _root.startBtn->setTitleText("0/7");
            _root.bigKillNumBg->setVisible(true);
            _root.bigKillNum->setString("0");
        }
        
    }
    
    void GameTableUIV88::bigKillPeople()
    {
        
        vector<int>dropPos;
        dropPos.clear();
        
        int a[9] = {-1,-4,-5,-6,0,1,4,5,6};
        for (int i = 0; i < 8; i++) {
            int idx = nGunsignPositionBig + a[i];
            if ( idx >= 0 && idx <= 19 && nOpenLotteryType[idx] == 11) {
                dropPos.push_back(idx);
            }
        }
        
        
        for (int m = 0; m < dropPos.size(); m++) {
            int idx = dropPos.at(m);
            
            for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                for (int j = 0; j < 4; j++) {
                    int index = (4-j-1)*5+i;
                    if (index == idx) {
                        auto result =(ImageView*)(m_vPanelTurnArea.at(i)->getChildByTag(1234567 + j));
                        if (result) {
                            HNAudioEngine::getInstance()->playEffect(Pubg_Gun_BGM);
                            ImageView* boom = ImageView::create("Games/31000511/ui/tankong.png");
                            if (boom) {
                                boom->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                boom->setPosition(result->getContentSize()/2);
                                 boom->runAction(Sequence::create(ScaleTo::create(0.1,0.8),DelayTime::create(0.1f),ScaleTo::create(0.1,1.0),NULL));
                                result->addChild(boom);
                            }
                            
                        }
                    }
                }
            }
        }
        
        _root.bigKillNum->setString(to_string(nkillCount));
         updateMyMoney(i64MyMoney);
        
    }
    
    void GameTableUIV88::startCallFakeRotate()
    {
        m_autoRotation = true;
        m_autoRotationTime = 0;
        this->runAction(Sequence::create(CallFunc::create([=]() {
            for (int idx = 0; idx < m_vPanelTurnArea.size(); idx++) {
                auto layout = m_vPanelTurnArea.at(idx);
                if (layout) {
                    auto moveDown = MoveBy::create(0.15f, Vec2(0, -30));
                    layout->runAction(Sequence::create(moveDown, DelayTime::create(0.05f),  moveDown->reverse(), nullptr));
                }
            }
        }), DelayTime::create(0.30f), CallFunc::create([=]() {
            schedule(schedule_selector(GameTableUIV88::turnUpdateEx), 0.05f);
        }), nullptr));
    }
    
    void GameTableUIV88::turnUpdateEx(float dt)
    {
        m_autoRotationTime++;
        if (m_autoRotationTime > 50 && m_autoRotation) { //没有收到服务器消息异常处理
            unschedule(schedule_selector(GameTableUIV88::turnUpdateEx));
            auto prompt = GamePromptLayer::create();
            prompt->setColorLayerTouchEnabled(false);
            prompt->showPrompt("网络连接异常");
            prompt->setOkayCallBack([this]() {
                leaveTheGame();
            });
            return;
        }
        for (int m = 0; m < _resultImg.size(); m++) {
            auto imgResult = (ImageView*)(_resultImg.at(m));
            if (imgResult) {
                imgResult->runAction(MoveBy::create(0.1f, Vec2(0, -ICON_HEIGHT)));
            }
        }
        

        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            for (int j = 0; j < LOTTERY_TYPE_COUNT_Pubg; j++) {
                auto img = m_vTurnIcon.at(i).at(j);
                if (img) {
                    if (j > 3) {
                        img->setVisible(true);
                    }
                    
                    if (_resultImg.size() > 0) {
                        if (_resultImg.at(3)->getPosition().y < 0 ) {
                            img->setVisible(true);
                        }
                    }
                    
                    img->runAction(Sequence::create(MoveBy::create(0.1f, Vec2(0, -ICON_HEIGHT)), CallFunc::create([=]() {
                        //更新index
                        int nOldTag = m_vTurnIcon.at(i).at(j)->getTag();
                        int nNewTag = (nOldTag - 1) < 0 ? (LOTTERY_TYPE_COUNT_Pubg - 1) : (nOldTag - 1);
                        m_vTurnIcon.at(i).at(j)->setTag(nNewTag);
                        
                        //一轮结束
                        if (j == LOTTERY_TYPE_COUNT_Pubg - 1) {
                            for (int k = 0; k < LOTTERY_TYPE_COUNT_Pubg; k++) {
                                int curTag = m_vTurnIcon.at(i).at(k)->getTag();
                                m_vTurnIcon.at(i).at(k)->setPosition(m_vTurnIconPos.at(curTag));
                            }
                        }
                    }), nullptr));
                }
            }
        }
        
        
    }
    
    
    void GameTableUIV88::setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize,int index) {
        float fStartPosY = ICON_HEIGHT / 2; //parentSize.height / 2 -
        
        for (int i = 0; i < LOTTERY_TYPE_COUNT_Pubg; i++) {
            vIconPos.push_back(Vec2(parentSize.width/2, fStartPosY + i*ICON_HEIGHT));
        }
    }
    
    void GameTableUIV88::setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum) {
        for (int i = 0; i < LOTTERY_COUNT_Pubg; i++) {
            //整圈数
            int nCount = (nRoundMinNum + i)*LOTTERY_TYPE_COUNT_Pubg;
            //开的那圈
            nCount += LOTTERY_TYPE_COUNT_Pubg - vCurBeginIcon.at(i) + vCurEndIcon.at(i);
            vTurnUnitCount.push_back(nCount);
        }
    }
    
    void GameTableUIV88::checkTurnIsSuccess() {
    }
    
    void GameTableUIV88::transformOpenLotteryType() {
        memset(nOpenLotteryTypeColRow, 0, sizeof(nOpenLotteryTypeColRow));
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            for (int j = 0; j < 4; j++) {
                int index = (4-j-1)*5+i;
                if (index >=0 && index < LOTTERY_COUNT_Pubg) {
                    nOpenLotteryTypeColRow[i][j] = nOpenLotteryType[index];
                }
            }
        }
    }
    
    void GameTableUIV88::randomColumnDatasAndCalcTurnCount() {
        HNAudioEngine::getInstance()->playEffect(Tiger_startLoop);
        m_bAnimationFinished = false;
        
        char str[128] = {0};
        if (m_vTurnIcon.size() > 0) {
            for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                int index = 0;
                for (int j = 0; j < LOTTERY_TYPE_COUNT_Pubg; j++) {
                    memset(str, 0, sizeof(str));
                    auto img = m_vTurnIcon.at(i).at(j);
                    if (img) {
                        
                        
                        int imgTag = img->getTag();
                        if (imgTag == 0 || imgTag == 1 || imgTag == 2 || imgTag == 3) {
                            
                            continue;
                        }
                        if (imgTag >= LOTTERY_TYPE_COUNT_Pubg - 4) {
                            sprintf(str, kIconTypeTpl, nOpenLotteryTypeColRow[i][index++]);
                        }
                        else {
                            sprintf(str, kIconTypeTpl, rand() % LOTTERY_TYPE_COUNT_Pubg);
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
            int nCount = (2 + i) * LOTTERY_TYPE_COUNT_Pubg;
            nCount += (LOTTERY_TYPE_COUNT_Pubg - 4);
            m_vTurnUnitCount.push_back(nCount);
        }
        //启动转盘
        m_nHasTurnUnitCount = 0;
        this->runAction(Sequence::create(CallFunc::create([=]() {
            for (int idx = 0; idx < m_vPanelTurnArea.size(); idx++) {
                auto layout = m_vPanelTurnArea.at(idx);
                if (layout) {
                    auto moveDown = MoveBy::create(0.15f, Vec2(0, -30));
                    layout->runAction(Sequence::create(moveDown, DelayTime::create(0.05f),  moveDown->reverse(), nullptr));
                }
            }
            
        }), DelayTime::create(0.4f), CallFunc::create([=]() {
            //unschedule(schedule_selector(GameTableUIV88::turnUpdateEx));
            schedule(schedule_selector(GameTableUIV88::turnUpdate), 0.05f);
        }), DelayTime::create(0.3f),CallFunc::create([this](){
            cleanResults();
        }),nullptr));
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
        updateMyRealWinMoney(m_i64WinMoney - i64WinJackpot);
        showJackpot();
        
        if (openLinesCount > 0) {
            //m_bHasCashIn = false;
            HNAudioEngine::getInstance()->playEffect(Tiger_bingo);
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
            memcpy(msg.nOpenLineCount, nOpenLineCount, sizeof(msg.nOpenLineCount));
            memcpy(msg.nOpenLineRate, nOpenLineRate, sizeof(msg.nOpenLineRate));
            _gameSettlement->showSettleLayerAndUpdateData(msg);
            
            runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=](){
                m_bHasCashIn = true;
            }), NULL));
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
        /*if (money > 0) {
            int size = sizeof(nOpenLineRate)/sizeof(nOpenLineRate[0]);
            int rate = 0;
            for (int i = 0; i < size ; i++) {
                rate += nOpenLineRate[i];
            }
            if (rate >= 50) {
                CommandManager::getInstance()->showGameBigWin(money, rate, this);
            }
        }*/
    }

    void GameTableUIV88::update(float dt)
    {
        // enable start button
        if (m_bGameFinished &&
            m_bAnimationFinished &&
            !m_bAuto &&
            (_GameTableLogic->getGameStatus() == GS_FREE || _GameTableLogic->getGameStatus()==GS_FINAL_MODE ))
        {
            _root.EnableButtons(true);
        }
        
        if ( _GameTableLogic->getGameStatus() == GS_FINAL_MODE) { //大逃杀模式自动
            if (m_bAuto && m_bAnimationFinished && m_bGameFinished) {
                startBtnCallBack(_root.startBtn, Widget::TouchEventType::ENDED);
            }
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
            {   img->stopAllActions();
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
    }

}
