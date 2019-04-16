#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNCommon/HNConverCode.h"

#include "NLGameTableUIV88.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "NLSettlementV88.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "base/CCRef.h"
#include "HNLobby/GameRewardEffect.hpp"
#include "HNLobby/PlatformDefine.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "HNLobby/GameData.h"
#define ICON_HEIGHT 133

namespace NL {

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
    
    const static char* NL_BGM = "Games/31000506/music/NL_BGM.mp3";
    const static char* NL_bingo = "Games/31000506/music/NL_bingo.mp3";
    const static char* NL_startLoop = "Games/31000506/music/NL_startLoop.mp3";
    const static char* NL_endLoop = "Games/31000506/music/endLoop.wav";
    const static char* kIconTypeTpl = "Games/31000506/type/LT_%02d.png";
    
    const static char* kJackpotUI = "common/animation/jackPot/Jackpot.csb";
	const static char* effectCsb = "Games/31000506/Node_effect.csb";

    // 结果开奖线闪烁时长与次数
    const static float kResultLineBlinkDuration = 0.5f;
    const static int kResultLineBlinkTimes = 2;
    
    const static vector<Vec2> RATE_POS_LIST = {Vec2(183,26),Vec2(91,26),Vec2(275,26),Vec2(-1,26),Vec2(367,26),Vec2(-93,26),Vec2(459,26),Vec2(-185,26),Vec2(551,26)};
    
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
        , m_bGameFinished(true)
        , m_i64WinMoney(0)
        , m_i64WinJackpot(0)
        , nLotteryTypeCount(0)
    {
        memset(nLineConstitute,0, sizeof(nLineConstitute));
        sMusicBoyorGirl = "";
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

		return true;
	}

	void GameTableUIV88::createRootLayer() {
        //background image
        ImageView* bg = ImageView::create("Games/31000506/ui/18.png");
        if (bg) {
            _root.bg = bg;
            bg->setPosition(WIN_SIZE/2);
            addChild(bg, ZOrder_Bg);
        }
//        //pool
//        ImageView* poolBg = ImageView::create("v88/ninelines/ui/pool.png");
//        if (poolBg) {
//            _root.poolBg = poolBg;
//            poolBg->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
//            poolBg->setPosition(Vec2(WIN_SIZE.width - 10, WIN_SIZE.height-100));
//            addChild(poolBg, ZOrder_Pool);
//            //
//            Text* text = Text::create("213123", FONT_TTF_FZCY, 30);
//            if (text) {
//                _root.poolText = text;
////                text->setTextVerticalAlignment(TextVAlignment::MIDDLE);
////                text->setTextHorizontalAlignment(TextHAlignment::MIDDLE);
//                text->setTextColor(Color4B::WHITE);
//                text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//                text->setPosition(Vec2(poolBg->getContentSize().width/2, poolBg->getContentSize().height*0.2f));
//                _root.poolBg->addChild(text);
//            }
//        }
        //middle Layer
        Node* middle = CSLoader::createNode("Games/31000506/Node_root.csb");
        if (middle) {
            _root.middleNode = middle;
            middle->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2));
            addChild(middle, ZOrder_Middle_Layer);
            
            _root.middlePanel = dynamic_cast<Layout*>(middle->getChildByName("Panel_bg"));
            if (_root.middlePanel) {
                
                char str[128] = {0};
                for (int i = 0; i < LINE_COUNT; i++) {
                    memset(str, 0, sizeof(str));
                    sprintf(str, "Image_line_%d", i);
                    auto line = dynamic_cast<ImageView*>(_root.middlePanel->getChildByName(str));
                    if (line) {
                        line->setVisible(false);
                        m_vAllLines[i] = line;
                    }
                }
                
                
                Text* myName = dynamic_cast<Text*>(_root.middlePanel->getChildByName("Text_name"));
                if (myName) {
                    _root.myName = myName;
                }
                Text* myMoney = dynamic_cast<Text*>(_root.middlePanel->getChildByName("Text_money"));
                if (myMoney) {
                    _root.myMoney = myMoney;
                }
                
                auto jackpotText = dynamic_cast<Text*>(_root.middlePanel->getChildByName("Text_jackpot"));
                if (jackpotText)
                {
                    _root.jackpot = jackpotText;
                }
                
                Button* start = dynamic_cast<Button*>(_root.middlePanel->getChildByName("Button_start"));
                if (start) {
                    _root.startBtn = start;
                    start->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::startBtnCallBack, this));
                }
                
                Button* stop = dynamic_cast<Button*>(_root.middlePanel->getChildByName("Button_stop"));
                if (stop) {
                    _root.stopBtn = stop;
                    _root.stopBtn->setVisible(false);
                    _root.stopBtn->setTouchEnabled(false);
                    stop->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::stopBtnCallBack, this));
                }
                
                Button* autoBtn = dynamic_cast<Button*>(_root.middlePanel->getChildByName("Button_auto"));
                if (autoBtn) {
                    _root.autoBtn = autoBtn;
                    autoBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::autoBtnCallBack, this));
                }
                
                auto cancelBtn = dynamic_cast<Button*>(_root.middlePanel->getChildByName("Button_cancel"));
                if (cancelBtn)
                {
                    _root.cancelBtn = cancelBtn;
                    _root.cancelBtn->setVisible(false);
                    _root.cancelBtn->setTouchEnabled(false);
                    cancelBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::autoBtnCallBack, this));
                }
                
                Button* maxNote = dynamic_cast<Button*>(_root.middlePanel->getChildByName("Button_max_note"));
                if (maxNote) {
                    _root.maxNote = maxNote;
                    maxNote->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::maxNoteBtnCallBack, this));
                }
                Button* maxLine = dynamic_cast<Button*>(_root.middlePanel->getChildByName("Button_max_line"));
                if (maxLine) {
                    _root.maxLine = maxLine;
                    maxLine->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::maxLineBtnCallBack, this));
                }
                Button* addNote = dynamic_cast<Button*>(_root.middlePanel->getChildByName("Button_add_note"));
                if (addNote) {
                    _root.addNote = addNote;
                    addNote->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::addNoteBtnCallBack, this));
                }
                Button* addLine = dynamic_cast<Button*>(_root.middlePanel->getChildByName("Button_add_line"));
                if (addLine) {
                    _root.addLine = addLine;
                    addLine->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::addLineBtnCallBack, this));
                }
                //
                Text* scoreText = dynamic_cast<Text*>(_root.middlePanel->getChildByName("Text_score"));
                if (scoreText) {
                    _root.score = scoreText;
                }
                Text* addLineText = dynamic_cast<Text*>(_root.middlePanel->getChildByName("Text_add_line"));
                if (addLineText) {
                    _root.addLineCount = addLineText;
                }
                Text* addNoteText = dynamic_cast<Text*>(_root.middlePanel->getChildByName("Text_add_note"));
                if (addNoteText) {
                    _root.addNoteCount = addNoteText;
                }
                
                _root.m_FreeModul = dynamic_cast<ImageView*>(_root.middlePanel->getChildByName("Image_free"));

				//背景动画
				for (auto i = 0; i < 4; i++)
				{
					auto pNode = dynamic_cast<Node*>(_root.middlePanel->getChildByName(StringUtils::format("Node_effect_%d", i)));
					if (pNode) {
						auto effectNode = CSLoader::createNode(effectCsb);
						auto effectAction = CSLoader::createTimeline(effectCsb);
						if (effectNode && effectNode)
						{
							pNode->addChild(effectNode);
							effectNode->setPosition(Vec2::ZERO);
							effectNode->runAction(effectAction);
							effectAction->gotoFrameAndPlay(0);
						}
					}
				}
                
                auto jsonPath      = "Games/31000506/animation/deng/jx_deng.json";
                auto atlasPath     = "Games/31000506/animation/deng/jx_deng.atlas";
                auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
                if (skeletonNode) {
                    skeletonNode->setPosition(Vec2(0,0));
                    skeletonNode->setAnimation(0,"jx_deng",true);
                    _root.middlePanel->addChild(skeletonNode,1);
                            
                }
                
                m_RateBg = ImageView::create("Games/31000506/ui/sgdk.png");
                if (m_RateBg) {
                    m_RateBg->setPosition(Vec2(640,110));
                    _root.middlePanel->addChild(m_RateBg);
                    m_RateBg->setVisible(false);
                }
                
            }
            
        }
    

	}

    void GameTableUIV88::loadTurn() {
        char str[128] = {0};
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            
            
            if (_root.middlePanel) {
                //转动的容器
                memset(str, 0, sizeof(str));
                sprintf(str, "Panel_%d", i);
                Layout* layout = dynamic_cast<Layout*>(_root.middlePanel->getChildByName(str));
                if (layout) {
                    layout->setClippingEnabled(true);
                    m_vPanelTurnArea.push_back(layout);
                }
            }
            
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
        NLSettlementV88* settle = NLSettlementV88::create();
        if (settle) {
            settle->setPosition(WIN_SIZE/2);
            _gameSettlement = settle;
            _gameSettlement->hideSettleLayer();
            addChild(_gameSettlement, ZOrder_SettlementLayer);
        }
    }

    void GameTableUIV88::onEnter() {
		HNGameUIBase::onEnter();
        HNAudioEngine::getInstance()->playBackgroundMusic(NL_BGM, true);

	
		//触摸
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
		myListener->setSwallowTouches(false);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

        showLoadingResUI(LOAD_NINELINE_GAME);
        
	}
    void GameTableUIV88::closeLoadingCallBack()
    {
        initData();
        
        int rondom = random(1,2);
        if (rondom == 1) {
            sMusicBoyorGirl = "boy";
        }
        else
        {
            sMusicBoyorGirl = "girl";
        }
        
        createRootLayer();
        createToolBarUI();
        loadTurn();
        
        updateSelectLines();
        setNick(_GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo())->nickName);
        
        _GameTableLogic->sendGameInfo();
        scheduleUpdate();
        
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
                gameSet->setBgPath(NL_BGM);
				gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
			});
            toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
                auto helpLayer = HN::HelpLayer::create();
                helpLayer->setContentOnView("v66/common/help/NL.png");
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
        unschedule(schedule_selector(GameTableUIV88::jackpotUpdate));
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
    }
    
    //满注按钮
    void GameTableUIV88::maxNoteBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        maxNoteLogic();
    }
    
    //自动按钮
    void GameTableUIV88::autoBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type)
    {
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
    void GameTableUIV88::stopBtnCallBack(Ref* pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
       
        if (_root.stopBtn) {
            _root.stopBtn->setVisible(false);
        }
        
    }
	//开始按钮
	void GameTableUIV88::startBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
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
        //cleanResults();
        //createAnimationRotate();
        
        if (_GameTableLogic) {
            m_bHasCashIn = false;
            m_bGameFinished = false;
            struct_c2s_start msg;
            msg.nNoteLines = m_currentMaxLine + 1;
            msg.i64NoteMoney = m_currentNoteValue;
            msg.i64Jackpot   = i64Jackpot;
//            HNLOG_INFO("Request Start Game Lines: %d Money: %d", msg.nNoteLines, msg.i64NoteMoney);
            _GameTableLogic->sendStart(&msg);
        }
        
        if (_gameSettlement) {
            _gameSettlement->hideSettleLayer();
        }
        if (_root.score) {
            _root.score->setString(to_string(0));
        }
        
        _root.EnableButtons(false);
	}

	//////////////////////////////////////////////////////////////////////////

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
        //i64Jackpot = msg->i64Jackpot;
        i64MyMoney = msg->i64MyMoney;
        nLotteryTypeCount = msg->nLotteryTypeCount;
        memcpy(nLineConstitute, msg->nLineConstitute, sizeof(nLineConstitute));
        m_currentNoteValue = i64NoteMin;
        updateJackpot();
        updateMyMoney(msg->i64MyMoney);
        updateSelectNotes();
        loadTurnEx();
        
        i64Jackpot = GameData::getInstance()->getRoomJackpotData();
        schedule(schedule_selector(GameTableUIV88::jackpotUpdate), 0.33f);
	}

	void GameTableUIV88::dealGameBegin(struct_s2c_game_begin* msg) {
		if (msg == nullptr) {
			return;
		}

        updateMyMoney(msg->i64MyMoney);
        
        m_i64WinMoney = msg->i64WinMoney;
        m_i64WinJackpot = msg->i64WinJackpot;
        i64Jackpot -= m_i64WinJackpot;
        memcpy(nOpenLotteryType, msg->nOpenLotteryType, sizeof(nOpenLotteryType));
        memcpy(lineResultNumData, msg->nOpenLineLotteryType, sizeof(lineResultNumData));
        memcpy(nOpenLinePrizeType, msg->nOpenLinePrizeType, sizeof(nOpenLinePrizeType));
        memcpy(nOpenLineCount, msg->nOpenLineCount, sizeof(nOpenLineCount));
        memcpy(nOpenLineRate, msg->nOpenLineRate, sizeof(nOpenLineRate));
        
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
        setFreeModul(msg->nFreeCount);
        //tryShowFreeTurnUI(msg->nFreeCount);
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
    void GameTableUIV88::jackpotUpdate(float dt)
    {
        
        i64Jackpot += random(50,100);
        updateJackpot();
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
            layer->setOkayCallBack([this]() {
                unschedule(schedule_selector(GameTableUIV88::updateTip));
                _GameTableLogic->sendUserUp();
            });
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
        m_RateBg = nullptr;
	}

	void GameTableUIV88::resetUINewGame() {
		
		//resetAllNotesAnimationAndClean();
	}

	void GameTableUIV88::resetData() {


	}
    
    void GameTableUIV88::turnUpdate(float dt) {
        //累计
//        HNLog::logInfo("turnUpdate: %d", m_nHasTurnUnitCount);
        m_nHasTurnUnitCount++;
        
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            for (int j = 0; j < nLotteryTypeCount; j++) {
                if (m_nHasTurnUnitCount <= m_vTurnUnitCount.at(i)) {
                    auto img = m_vTurnIcon.at(i).at(j);
                    if (img) {
//                        HNLog::logInfo("turn update: %d i: %d j: %d ImgTag: %d", m_nHasTurnUnitCount, i, j, img->getTag());
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
                            
                            
                            auto tempTag = 1234567 + ii;
                            if (m_vTurnIcon.at(i).at(ii)->getParent()->getChildByTag(tempTag) == NULL) {
                                //m_vTurnIcon.at(i).at(ii)->loadTexture(str);
                                //stopRotateAnimation(i);
                                auto img = ImageView::create(str);
                                m_vTurnIcon.at(i).at(ii)->getParent()->addChild(img);
                                img->loadTexture(str);
                                img->setTag(tempTag);
                                img->setName(to_string(nOpenLotteryTypeColRow[i][ii]));
                                auto staticPos = m_vTurnIconPos.at(ii);
                                img->setPosition(staticPos);
                                img->runAction(Sequence::create(MoveTo::create(0.10f,Vec2(staticPos.x,staticPos.y - 30)),MoveTo::create(0.10f,staticPos),NULL));
                                
                                _resultImg.push_back(img);
                                if (ii == 0) {
                                    HNAudioEngine::getInstance()->playEffect(NL_endLoop);
                                }
                               
                            }
                            
                        
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
        if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(LOTTERY_COLUMN_COUNT - 1))// && j == (LOTTERY_TYPE_COUNT_NL - 1))
        {
            // stop
            unschedule(schedule_selector(GameTableUIV88::turnUpdate));
            
//            HNLog::logInfo("update line effect End: %d %d", m_nHasTurnUnitCount);
            updateOpenLinesEffect();
            checkHasReward(m_i64WinMoney - m_i64WinJackpot);
        }
    }
    void GameTableUIV88::checkHasReward(long long _winmoney)
    {
        long long _realWinMoney = _winmoney;
        
        int randomPlay = random(1,10);
        bool isCanPlay = randomPlay <= 2;
        std::string pathMusic = "Games/31000506/music/";
        if (_realWinMoney > 0) {
            
            if (isCanPlay) {
                std::string winstr = sMusicBoyorGirl + "soundwin";
                winstr.append(StringUtils::format("%d.mp3",random(1,3)));
                pathMusic.append(winstr);
                HNAudioEngine::getInstance()->playEffect(pathMusic.c_str());
            }
            
            int n_bigWinNum        = Tools::getLianxianGameData(_realWinMoney,m_currentNoteValue,25,70);
            if (n_bigWinNum == 0) {
                
                showMoneyFunc(_realWinMoney);
            }
            else if (n_bigWinNum == 1 || n_bigWinNum == 2)
            {
                
                showBiggerWinLayer(2,n_bigWinNum,_winmoney);
                runAction(Sequence::create(DelayTime::create(3.0f),CallFunc::create([=](){
                    showMoneyFunc(_realWinMoney);
                    
                }),NULL));
            }
            
        }
        else
        {
            if (isCanPlay) {
                std::string losestr = sMusicBoyorGirl + "soundlose";
                losestr.append(StringUtils::format("%d.mp3",random(1,4)));
                pathMusic.append(losestr);
                HNAudioEngine::getInstance()->playEffect(pathMusic.c_str());
            }
            m_bAnimationFinished = true;
           
        }
    }
    void GameTableUIV88::showMoneyFunc(long long winMoney)
    {
        
        long long n_multipleNum = winMoney/m_currentNoteValue;
        int m_level = 0;
        if (n_multipleNum >= 3 && n_multipleNum < 60) {
            m_level = 1;
        }
        else if (n_multipleNum >=60 && n_multipleNum < 100 )
        {
            m_level = 2;
        }
        else if (n_multipleNum >= 100 && n_multipleNum < 120)
        {
            m_level = 3;
        }
        else if (n_multipleNum >= 120 && n_multipleNum < 150)
        {
            m_level = 4;
        }
        else if (n_multipleNum >= 150 && n_multipleNum < 500)
        {
            m_level = 5;
        }
        else if (n_multipleNum > 500)
        {
            m_level = 6;
        }
        showFourLevelMoney(m_level);
        runAction(Sequence::create(DelayTime::create(2.0f),CallFunc::create([=](){
            m_bAnimationFinished = true;
            updateMyMoney(i64MyMoney);
            updateMyRealWinMoney(m_i64WinMoney - m_i64WinJackpot);
            showGetRewardEffect();
            
            if (_root.nFreeNum > 0) { //有免费次数自动转
                if (!m_bAuto) {
                    this->autoBtnCallBack(_root.autoBtn,Widget::TouchEventType::ENDED);
                }
            }
            
        }),NULL));
        
    }
    void GameTableUIV88::showGetRewardEffect()
    {
        vector<int *> lineImgNum;
        lineImgNum.clear();
        int RateIdx = 0;
        
        for (int i = 0; i < LINE_COUNT; i++) {
            if (lineResultNumData[i] != -1) {
                lineImgNum.push_back(nLineConstitute[i]);
                
                m_RateBg->setVisible(true);
                char str[128] = {0};
                memset(str, 0, sizeof(str));
                sprintf(str, kIconTypeTpl,lineResultNumData[i]);
                auto img = ImageView::create(str);
                if (img) {
                    img->setScale(0.3f);
                    RateIdx = RateIdx >= LINE_COUNT ? LINE_COUNT - 1 : RateIdx;
                    auto posRate = RATE_POS_LIST[RateIdx++];
                    img->setPosition(posRate);
                    m_RateBg->addChild(img);
                    
                    auto ratetext = Text::create("","platform/fonts/simhei.ttf",22);
                    if (ratetext) {
                        ratetext->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                        ratetext->setPosition(Vec2(posRate.x + 20,posRate.y - 4));
                        m_RateBg->addChild(ratetext);
                        ratetext->setString(StringUtils::format("X%d",nOpenLineRate[i]));
                    }
                    
                }
                
            }
        }
        
        if (lineImgNum.size() > 0)
        {
            vector<ImageView *>imgVector;
            imgVector.clear();
            
            
            auto blackImg = Layout::create();
            blackImg->setTouchEnabled(false);
            blackImg->setContentSize(_root.middlePanel->getContentSize());
            blackImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            blackImg->setPosition(WIN_SIZE/2);
            
            
            addChild(blackImg,159);
            blackImg->setName("blackImg");
            //blackImg->setOpacity(50);
            
            vector<ImageView*> imgExVec;
            imgExVec.clear();
            if (openLineVec.size() > 0)
            {
                int m = 0;
                for (auto img : openLineVec)
                {
                    m++;
                    auto imgEx = (ImageView*)img->clone();
                    imgEx->setVisible(m_bAuto);
                    blackImg->addChild(imgEx);
                    imgExVec.push_back(imgEx);
                    
                }
                if (!m_bAuto)
                {
                    
                    for (int m = 0; m < imgExVec.size(); m++) {
                        imgExVec.at(m)->runAction(Sequence::create(DelayTime::create(0.5 * m),CallFunc::create([=](){
                            imgExVec.at(m)->setVisible(true);
                            imgExVec.at(m)->runAction(Sequence::create(FadeOut::create(0.01f),DelayTime::create(0.01f),FadeIn::create(0.2f),NULL));
                        }),DelayTime::create(0.5),CallFunc::create([=](){
                            imgExVec.at(m)->setVisible(false);
                            if (m == imgExVec.size() - 1) {
                                for (int x = 0; x < imgExVec.size(); x++) {
                                    imgExVec.at(x)->setVisible(true);
                                }
                            }
                        }),NULL));
                    }
                    
                    
                }
            }
            
            
            for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                Layout* panelTurn = dynamic_cast<Layout*>(m_vPanelTurnArea[i]->clone());
                panelTurn->setTag(333333 + i);
                blackImg->addChild(panelTurn,1);
            }
            
            int idxmap[] = {2,5,8,11,14,1,4,7,10,13,0,3,6,9,12};
            for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                auto panel = (Layout*)(blackImg->getChildByTag(333333 + i));
                if (panel) {
                    for (int j = 0; j < 3; j++) {
                        auto tag = 1234567  + j;
                        auto img = dynamic_cast<ImageView*>(panel->getChildByTag(tag));
                        if (img) {
                            img->setVisible(false);
                            imgVector.push_back(img);
                        }
                    }
                }
                
            }
            
            for (int i = 0; i < LINE_COUNT ; i++) {
                auto lineData = nOpenLineCount[i];
                if (lineData > 0) {
                    for (int m = 0; m < lineData; m++) {
                        auto posIdx = nLineConstitute[i][m];
                        auto mFind = idxmap[posIdx];
                        auto img = imgVector.at(mFind);
                        if (img) {
                            img->setVisible(true);
                            
                            auto m_aniSprite = img->getChildByName("ANIMATON_SPRITE");
                            if (m_aniSprite == nullptr) {
                                char str[32];
                                int startIdx = 0;
                                int endIdx   = 5;
                                std::string animationName = "NL_ICON_EFFECT";
                                Animation * anima = AnimationCache::getInstance()->getAnimation(animationName);
                                if (anima == nullptr) {
                                    anima = Animation::create();
                                    anima->setDelayPerUnit(0.10f);
                                    anima->setLoops(-1);
                                    
                                    for (int i=startIdx; i <= endIdx; i++) {
                                        memset(str,0,sizeof(str));
                                        sprintf(str, "sg_000%02d.png",i);
                                        auto frames = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
                                        if (frames) {
                                            anima->addSpriteFrame(frames);
                                        }
                                        
                                    }
                                    
                                    AnimationCache::getInstance()->addAnimation(anima, animationName);
                                }
                                
                                auto animate = Animate::create(anima);
                                auto animationSprite = Sprite::create();
                                animationSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                                animationSprite->runAction(animate);
                                animationSprite->setScale(0.80f);
                                animationSprite->setName("ANIMATON_SPRITE");
                                animationSprite->setPosition(img->getContentSize()/2);
                                img->addChild(animationSprite);
                            }
                            
                            
                        }
                        
                        
                    }
                    
                }
                
            }
        }
        
    }
    void GameTableUIV88::showGetRewardRate()
    {
        if (m_RateBg) {
            m_RateBg->setVisible(true);
            m_RateBg->removeAllChildren();
        }
        
        
    }
    void GameTableUIV88::setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize) {
        float fStartPosY = parentSize.height / 2 - ICON_HEIGHT;
        
        for (int i = 0; i < (nLotteryTypeCount == 0 ? 3:nLotteryTypeCount); i++) {
            vIconPos.push_back(Vec2(parentSize.width/2, fStartPosY + i*ICON_HEIGHT));
        }
    }
    
    void GameTableUIV88::setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum) {
        for (int i = 0; i < LOTTERY_COUNT_NL; i++) {
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
                if (index >=0 && index < LOTTERY_COUNT_NL) {
                    nOpenLotteryTypeColRow[i][j] = nOpenLotteryType[index];
                }
            }
        }
    }
    
    void GameTableUIV88::randomColumnDatasAndCalcTurnCount() {
        HNAudioEngine::getInstance()->playEffect(NL_startLoop);
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
        //m_vTurnUnitCount = {20,28,36,44,52};
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            int nCount = (1 + i) * nLotteryTypeCount;
            nCount += (nLotteryTypeCount - 3);
            m_vTurnUnitCount.push_back(nCount);
        }
        //启动转盘
        m_nHasTurnUnitCount = 0;
        //schedule(schedule_selector(GameTableUIV88::turnUpdate), 0.05f);
        this->runAction(Sequence::create(CallFunc::create([=]() {
            for (auto layout : m_vPanelTurnArea) {
                if (layout)
                {
                    auto nCount   = layout->getChildrenCount();
                    auto childvec = layout->getChildren();
                    for (int n = 0;n < nCount;n++) {
                        auto moveDown = MoveBy::create(0.2f, Vec2(0, -60));
                        auto icon = childvec.at(n);
                        icon->runAction(Sequence::create(moveDown, DelayTime::create(0.05f),  moveDown->reverse(), nullptr));
                    }
                }
                
            }
        }), DelayTime::create(0.35f), CallFunc::create([=]() {
//            HNLog::logInfo("Schedule turnUpdate");
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
                openLineVec.push_back(img);
            }
        }
        
        //updateMyMoney(i64MyMoney);
        //updateMyRealWinMoney(m_i64WinMoney - m_i64WinJackpot);
        showJackpot();
        
        if (openLinesCount > 0) {
            //m_bHasCashIn = false;
            HNAudioEngine::getInstance()->playEffect(NL_bingo);
            this->runAction(Sequence::create(DelayTime::create(kResultLineBlinkDuration + 0.02f), CallFunc::create([=]() {
                hideAllLines();
                this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=](){
                    m_bHasCashIn = true;
                }), NULL));
//                m_bHasCashIn = true;
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
            _root.jackpot->setString(Tools::addComma(i64Jackpot));
        }
    }
    
    void GameTableUIV88::autoRestart(float dt)
    {
        if (m_bHasCashIn &&
            m_bAuto &&
            _GameTableLogic->getGameStatus() == GS_FREE)
        {
            startBtnCallBack(_root.startBtn, Widget::TouchEventType::ENDED);
        }
    }
    
    void GameTableUIV88::updateMyRealWinMoney(long long money) {
        auto coin = FloatCoin::create();
        if (coin) {
            coin->openParent(this, Vec2(WIN_SIZE.width*0.5f, WIN_SIZE.height-120), ZOrder_Float_Coin);
            coin->updateCoin(money);
        }
        if (_root.score) {
            _root.score->setString(to_string(money));
        }
        //if (money > 0) {
            //GameRewardEffect::createEffect(this);
       // }
    }
    
    // 每帧调用
    void GameTableUIV88::update(float dt)
    {
        // enable 开始按键
        if (m_bGameFinished &&
            m_bAnimationFinished &&
            _GameTableLogic->getGameStatus() == GS_FREE
            && !m_bAuto)
        {
            _root.EnableButtons(true);
        }
        
        // 自动开始
        if (((i64MyMoney > 0 &&
              m_currentNoteValue * (m_currentMaxLine + 1) < i64MyMoney) || _root.nFreeNum > 0 ) &&
            m_bHasCashIn &&
            m_bAuto &&
            _GameTableLogic->getGameStatus() == GS_FREE && m_bAnimationFinished)
        {
            startBtnCallBack(_root.startBtn, Widget::TouchEventType::ENDED);
        }
    }
    
    void GameTableUIV88::showJackpot()
    {
        if (m_i64WinJackpot > 0)
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
                txtJackpot->setString(Tools::addComma(m_i64WinJackpot));
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
                if (nullptr != icon)
                {
                    icon->setVisible(true);
                }
            }
        }
        if (getChildByName("blackImg")) {
            getChildByName("blackImg")->removeFromParent();
        }
        
       
        if (m_RateBg) {
            m_RateBg->setVisible(false);
            m_RateBg->removeAllChildren();
        }
        
        openLineVec.clear();
    }
    void GameTableUIV88::setFreeModul(int num)
    {
        _root.nFreeNum = num;
        if (_root.m_FreeModul == nullptr) {
            return;
        }
        
        _root.m_FreeModul->setVisible(num > 0);
        if (num > 0) {
            auto _textAtlas = dynamic_cast<TextAtlas*>(_root.m_FreeModul->getChildByName("AtlasLabel_1"));
            if (_textAtlas) {
                _textAtlas->setString(StringUtils::format("%d",num));
            }
        }
        
    }
    void GameTableUIV88::createAnimationRotate()
    {
        this->runAction(Sequence::create(CallFunc::create([=](){
            for (int n = 0; n < LOTTERY_COLUMN_COUNT; n++) {
                auto vIcon = m_vTurnIcon.at(n);
                for (int m = 0; m < vIcon.size(); m++) {
                    auto img = vIcon.at(m);
                    if (img) {
                        auto moveDown = MoveBy::create(0.15f, Vec2(0, -50));
                        img->runAction(Sequence::create(moveDown, DelayTime::create(0.05f),  moveDown->reverse(), nullptr));
                    }
                }
            }
        }),DelayTime::create(0.35f),CallFunc::create([=](){
        m_AnimationNodeVec.clear();
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            
            int m_animationType = random(1,2);
            int startIdx = 0;
            int endIdx   = 6;
            
            std::string animationName = StringUtils::format("NL_ICON_ROTATE_%d",m_animationType);
            Animation * anima = AnimationCache::getInstance()->getAnimation(animationName);
            if (anima == nullptr) {
                anima = Animation::create();
                anima->setDelayPerUnit(0.10f);
                anima->setLoops(-1);
                
                for (int j = startIdx; j <= endIdx; j++) {
                    std::string str = StringUtils::format("jx%d_0000%d.png",m_animationType,j);
                    auto frames = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
                    if (frames) {
                        anima->addSpriteFrame(frames);
                    }
                    
                }
                
                AnimationCache::getInstance()->addAnimation(anima, animationName);
            }
            
            auto animate = Animate::create(anima);
            auto animationSprite = Sprite::create();
            animationSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            //animationSprite->runAction(animate);
            
            animationSprite->setPosition(m_vPanelTurnArea.at(i)->getContentSize()/2);
            m_vPanelTurnArea.at(i)->addChild(animationSprite);
            m_AnimationNodeVec.push_back(animationSprite);
            
            animationSprite->runAction(Sequence::create(DelayTime::create(0.1*i),CallFunc::create([=](){
                for (auto ik = 0; ik < nLotteryTypeCount; ++ik)
                {
                    m_vTurnIcon.at(i).at(ik)->setVisible(false);
                }
                
            }),animate,NULL));
            
            
        }
     }),NULL));
    }
    void GameTableUIV88::stopRotateAnimation(int nCol)
    {
        if (nCol >= 0 && nCol < m_AnimationNodeVec.size()) {
            auto animationSprite = m_AnimationNodeVec.at(nCol);
            if (animationSprite) {
                animationSprite->removeFromParent();
                m_AnimationNodeVec.at(nCol) = nullptr;
            }
        }
        
    }
}
