#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNCommon/HNConverCode.h"

#include "CSLLGameTableUIV88.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "CSLLSettlementV88.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "base/CCRef.h"
#include "HNLobby/v66/ExtraReward.h"
#include "HNLobby/GameRewardEffect.hpp"
#include "UI/BigWinner.hpp"
#include "HNLobby/PlatformDefine.h"
#include "UI/UserHead.h"
#include "Tool/XCaseStringUtil.hpp"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>

#define ICON_HEIGHT 161

namespace CSLL {
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
    
    const static char* CSLL_BGM = "Games/31000507/music/CSLL_BGM.mp3";
    const static char* CSLL_bingo = "Games/31000507/music/CSLL_bingo.mp3";
    const static char* CSLL_startLoop = "Games/31000507/music/CSLL_startLoop.mp3";
    const static char* CSLL_btn = "Games/31000507/music/btn.mp3";
    const static char* CSLL_endloop = "Games/31000507/music/loopEnd.wav";
    const static char* CSLL_ICON_TYPE = "Games/31000507/type/LT_%02d.png";
    
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
        , m_bGameFinished(true)
        , m_bAnimationFinished(true)
        , m_i64WinMoney(0)
        , i64WinJackpot(0)
        , nLotteryTypeCount(0)
    {
        memset(nLineConstitute,0, sizeof(nLineConstitute));
        _robotVec.clear();
        _robotParentNodeVec.clear();
        
        _maryLayer = nullptr;
        nMaryCount = 0;
        nFreeNum   = 0;
        m_AnimationNodeVec.clear();
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
        //scheduleUpdate();
		return true;
	}

	void GameTableUIV88::createRootLayer()
	{
        ImageView* bg = ImageView::create("Games/31000507/ui_v2/bg_zjm.png");
        if (bg)
		{
            _root.bg = bg;
            bg->setPosition(WIN_SIZE/2);
            addChild(bg, ZOrder_Bg);
            //createYunDuo();

			std::string jsonPath = "Games/31000507/spine/csll_beijing_effect.json";
			std::string atlasPath = "Games/31000507/spine/csll_beijing_effect.atlas";

			auto skeletonNode = spine::SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath);
			if (skeletonNode) {
				Vec2 vPos(800, 80);
				skeletonNode->setPosition(vPos);
				skeletonNode->setAnimation(0, "csll_beijing_effect", true);
				skeletonNode->setName("ANIMATON_BG");
				bg->addChild(skeletonNode);
			}
        }
        
        Node* middle = CSLoader::createNode("Games/31000507/Node_root.csb");
        if (middle) {
            _root.middleNode = middle;
            middle->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height/2));
            addChild(middle, ZOrder_Middle_Layer);
            //
            auto bg = dynamic_cast<Layout*>(middle->getChildByName("Panel_bg"));
            if (bg)
			{
                _root.gamePanel = bg;

				auto lines = dynamic_cast<Layout*>(bg->getChildByName("Panel_lines"));
				if (lines)
				{
					_root.linesPanel = lines;
					char str[128] = { 0 };
					for (int i = 0; i < LINE_COUNT; i++)
					{
						memset(str, 0, sizeof(str));
						sprintf(str, "Image_line_%d", i + 1);
						auto line = dynamic_cast<ImageView*>(lines->getChildByName(str));
						if (line)
						{
							line->setVisible(false);
							m_vAllLines[i] = line;
						}
					}
				}

				int iChildrenCount = bg->getChildrenCount();
                
                
                //Text* myMoney = dynamic_cast<Text*>(bg->getChildByName("Text_money"));
				Text* myMoney = dynamic_cast<Text*>(Tools::getNodeByName(bg,"Text_money"));
                if (myMoney) {
                    _root.myMoney = myMoney;
                }
                
                
                //Button* start = dynamic_cast<Button*>(bg->getChildByName("Button_start"));
				Button* start = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_start"));
                if (start) {
                    _root.startBtn = start;
                    start->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::startBtnCallBack, this));
                    
                    /*auto _startChild = dynamic_cast<ImageView*>(start->getChildByName("Image_1"));
                    if (_startChild) {
                        _startChild->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.0f),FadeOut::create(1.0f), NULL)));
                        _startChild->runAction(RepeatForever::create(RotateBy::create(1.0f,90)));
                    }*/
                    
					//auto istartNode = dynamic_cast<Node*>(start->getChildByName("Node_start"));
                    auto istartNode = dynamic_cast<Node*>(Tools::getNodeByName(bg, "Node_start"));
                    if (istartNode) {
                        _root.m_startBtnEffect = istartNode;
                        
                        /*auto rotatefire = CSLoader::createNode("Games/31000507/Node_start.csb");
                        auto rotateaction = CSLoader::createTimeline("Games/31000507/Node_start.csb");
                        if (rotatefire && rotateaction)
                        {
                            _root.m_startBtnEffect->addChild(rotatefire);
                            rotatefire->setPosition(Vec2::ZERO);
                            rotatefire->runAction(rotateaction);
                            rotateaction->gotoFrameAndPlay(0);
                        }*/
                        
                    }
                    
                }
                
                Button* stop = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_stop"));
                if (stop) {
                    _root.stopBtn = stop;
                    _root.stopBtn->setVisible(false);
                    _root.stopBtn->setTouchEnabled(false);
                    stop->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::stopBtnCallBack, this));
                }
                
                
                Button* autoBtn = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_auto"));
                if (autoBtn) 
				{
                    _root.autoBtn = autoBtn;
                    autoBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::autoBtnCallBack, this));
                }
                
                auto cancelBtn = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_cancel"));
                if (cancelBtn)
                {
                    _root.cancelBtn = cancelBtn;
                    _root.cancelBtn->setVisible(false);
                    _root.cancelBtn->setTouchEnabled(false);
                    _root.cancelBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::autoBtnCallBack, this));
                }
                
                Button* maxNote = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_max_note"));
                if (maxNote) {
                    _root.maxNote = maxNote;
                    maxNote->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::maxNoteBtnCallBack, this));
                }
                
                Button* addNote = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_add_note"));
                if (addNote) {
                    _root.addNote = addNote;
                    addNote->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::addNoteBtnCallBack, this));
                }
                
                Button* subNote = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_sub_note"));
                if (subNote) {
                    _root.subNote = subNote;
                    subNote->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::addNoteBtnCallBack, this));
                }
                
                Button* addLine = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_add_line"));
                if (addLine) {
                    _root.addLine = addLine;
                    addLine->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::addLineBtnCallBack, this));
                }
                
                Button* subLine = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_sub_line"));
                if (subLine) {
                    _root.subLine = subLine;
                    subLine->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::addLineBtnCallBack, this));
                }
                
                Text* scoreText = dynamic_cast<Text*>(Tools::getNodeByName(bg, "Text_score"));
                if (scoreText) {
                    _root.score = scoreText;
                }
                
                auto jackpotText = dynamic_cast<Text*>(Tools::getNodeByName(bg, "Text_jackpot"));
                if (jackpotText)
                {
                    _root.jackpot = jackpotText;
                }
                
                Text* addLineText = dynamic_cast<Text*>(Tools::getNodeByName(bg, "Text_add_line"));
                if (addLineText) {
                    _root.addLineCount = addLineText;
                }
                Text* addNoteText = dynamic_cast<Text*>(Tools::getNodeByName(bg, "Text_add_note"));
                if (addNoteText) {
                    _root.addNoteCount = addNoteText;
                }
                Text* totalNoteText = dynamic_cast<Text*>(Tools::getNodeByName(bg, "Text_noteTotal"));
                if (totalNoteText) {
                    _root.NoteTotal = totalNoteText;
                }
                Node* flower = dynamic_cast<Node*>(Tools::getNodeByName(bg, "Node_flower"));
                if (flower) {
                    _root.m_flower = flower;
                    flower->setVisible(false);
                }
                
                _root.m_freeGetTip = dynamic_cast<ImageView*>(Tools::getNodeByName(bg, "Image_free"));
                MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
                Text* myName = dynamic_cast<Text*>(Tools::getNodeByName(bg, "Text_name"));
                if (myName) {
                    _root.myName = myName;
                    std::string nickName(LogonResult.nickName);
                    if (!nickName.empty()) {
                        if (nickName.length() > 6) {
                            auto strUtil = XCaseStringUtil::create(ForceGBKToUtf8(nickName.c_str()));
                            std::string tempName = strUtil->sub(1,5,true);
                            
                            _root.myName->setString(tempName);
                        }
                        else
                        {
                            _root.myName->setString(ForceGBKToUtf8(nickName.c_str()));
                        }
                        
                    }
                    else {
                        _root.myName->setString(GBKToUtf8("未知"));
                    }
                }
                
                
                auto userHead = UserHead::create("hallimg/main/touxiangk.png");
                userHead->setHeadTouchEnabled(false);
                userHead->setPosition(Vec2(46,60));
                bg->addChild(userHead);
                if (Tools::needLoadWechatImage()) {
                    Texture2D* texture = new Texture2D;
                    Tools::loadWechatImageFromLocal(texture);
                    if (texture) {
                        userHead->setClipUserHeadWithTexTure(texture, "hallimg/niuniu/5_1.png", 0.6f);
                    }
                }
                else {
                    std::string head_path = StringUtils::format(PLAYER_HEAD_V66, LogonResult.bLogoID);
                    if (!FileUtils::getInstance()->isFileExist(head_path)) {
                        head_path = DEFAULT_PLAYER_HEAD_V66;
                    }
                    userHead->setClipUserHeadWithFile(head_path, "hallimg/niuniu/5_1.png",0.6f);
                }
                
                for (int n = 1; n <= 4; n++) {
                     auto robotNode = dynamic_cast<Node*>(bg->getChildByName(StringUtils::format("Node_robot%d",n)));
                     if (robotNode) {
                        _robotParentNodeVec.push_back(robotNode);
                     }
                }
            }
            
            
        }
	}

    void GameTableUIV88::loadTurn() {
        char str[128] = {0};
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            if (_root.middleNode)
			{
                auto bg = _root.middleNode->getChildByName("Panel_bg");
                if (bg) {
                    //转动的容器
                    memset(str, 0, sizeof(str));
                    sprintf(str, "Panel_%d", i);
                    Layout* layout = dynamic_cast<Layout*>(Tools::getNodeByName(_root.middleNode,str));
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
                sprintf(str, CSLL_ICON_TYPE, j);
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
                sprintf(str, CSLL_ICON_TYPE, j);
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
        CSLLSettlementV88* settle = CSLLSettlementV88::create();
        if (settle) {
            settle->setPosition(WIN_SIZE/2);
            _gameSettlement = settle;
            _gameSettlement->hideSettleLayer();
            addChild(_gameSettlement, ZOrder_SettlementLayer);
        }
    }

    void GameTableUIV88::onEnter() {
		HNGameUIBase::onEnter();
        HNAudioEngine::getInstance()->playBackgroundMusic(CSLL_BGM, true);

        //触摸
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
		myListener->setSwallowTouches(false);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

        showLoadingResUI(LOAD_CSLL_GAME);
	}
    void GameTableUIV88::closeLoadingCallBack()
    {
        
        initData();
        
        createRootLayer();
        createToolBarUI();
        loadTurn();
        
        updateSelectLines();
        //setNick(_GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo())->nickName);
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
                gameSet->setBgPath(CSLL_BGM);
				gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
			});
            toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
                auto helpLayer = HN::HelpLayer::create();
                helpLayer->setContentOnView("v66/common/help/CSLL.png");
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
        
        HNAudioEngine::getInstance()->playEffect(CSLL_btn);
        auto name = ((Button*)pSender)->getName();
        if (name == "Button_sub_line") {
            if (m_currentMaxLine != 0) {
                m_currentMaxLine -= 1;
            }
        }
        else if (name == "Button_add_line")
        {
            m_currentMaxLine += 1;
            
        }
        
        if (m_currentMaxLine <= 0) {
            m_currentMaxLine = 0;
            
        }
        if (m_currentMaxLine >= LINE_COUNT) {
            m_currentMaxLine = LINE_COUNT - 1;
        }
        
        updateSelectLines();
        updateSelectNotes();
	}

    //加注按钮
    void GameTableUIV88::addNoteBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        HNAudioEngine::getInstance()->playEffect(CSLL_btn);
        auto name = ((Button*)pSender)->getName();
        if (name == "Button_sub_note") {
            m_currentNoteValue -= i64NoteUnit;
        }
        else if (name == "Button_add_note")
        {
            m_currentNoteValue += i64NoteUnit;
            
        }
        
        if (m_currentNoteValue < i64NoteMin) {
            m_currentNoteValue = i64NoteMax;
            
            //return;
        }
        
        if (m_currentNoteValue > i64NoteMax) {
            m_currentNoteValue = i64NoteMin;
            //return;
        }
        updateSelectNotes();
    }
    
    //满线按钮
    void GameTableUIV88::maxLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        HNAudioEngine::getInstance()->playEffect(CSLL_btn);
        maxLineLogic();
      
    }
    
    // 九线
    void GameTableUIV88::nineLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED)
        {
            return;
        }
        HNAudioEngine::getInstance()->playEffect(CSLL_btn);
        m_currentMaxLine = 8;
        updateSelectLines();
        
    }
    
    //满注按钮
    void GameTableUIV88::maxNoteBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type)
	{
        if (type != Widget::TouchEventType::ENDED) 
		{
            return;
        }
        HNAudioEngine::getInstance()->playEffect(CSLL_btn);
        maxNoteLogic();
    }
    
    //自动按钮
    void GameTableUIV88::autoBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) 
	{
        if (type != Widget::TouchEventType::ENDED) 
		{
            return;
        }
        HNAudioEngine::getInstance()->playEffect(CSLL_btn);
        m_bAuto = !m_bAuto;
        if (m_bAuto && _GameTableLogic->getGameStatus() == GS_FREE)
        {
            startBtnCallBack(_root.startBtn, Widget::TouchEventType::ENDED);
        }
        
//        if (m_bAuto)
//        {
//            schedule(schedule_selector(GameTableUIV88::autoRestart), 0.5f);
//        }
//        else
//        {
//            unschedule(schedule_selector(GameTableUIV88::autoRestart));
//        }
        
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
        HNAudioEngine::getInstance()->playEffect(CSLL_btn);
        
        if (_root.stopBtn) {
            _root.stopBtn->setVisible(false);
        }
        
    }
	//开始按钮
	void GameTableUIV88::startBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
       
        if (type != Widget::TouchEventType::ENDED) {
    
            return;
        }
        HNAudioEngine::getInstance()->playEffect(CSLL_btn);
        if (m_currentNoteValue < i64NoteMin || m_currentNoteValue > i64NoteMax) {
            //押注金额异常
            auto prompt = GamePromptLayer::create();
            prompt->showPrompt("下注数值异常");
            return;
        }
        if ( (m_currentNoteValue * (m_currentMaxLine + 1) > i64MyMoney || i64MyMoney <= 0) && nFreeNum == 0) {
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
        
        if (_GameTableLogic)
        {
            m_bHasCashIn = false;
            m_bGameFinished = false;
            struct_c2s_start msg;
            msg.nNoteLines = m_currentMaxLine + 1;
            msg.i64NoteMoney = m_currentNoteValue;
            _GameTableLogic->sendStart(&msg);
        }
        
        if (_gameSettlement) {
            _gameSettlement->hideSettleLayer();
        }
        if (_root.score) {
            _root.score->setString(to_string(0));
        }
        
        _root.EnableButtons(false);
        
        showFlower(false);
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
        i64MyMoney = msg->i64MyMoney;
        nLotteryTypeCount = msg->nLotteryTypeCount;
        memcpy(nLineConstitute, msg->nLineConstitute, sizeof(nLineConstitute));
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
        
//        msg->i64WinMoney;
//        msg->nFreeCount;
//
//        msg->nOpenRoundTime;
//        msg->nOpenLineTime;
//        msg->nShowResultTime;
        m_i64WinMoney = msg->i64WinMoney;
        i64WinJackpot = msg->i64WinJackpot;
        memcpy(nOpenLotteryType, msg->nOpenLotteryType, sizeof(nOpenLotteryType));
        memcpy(lineResultNumData, msg->nOpenLineLotteryType, sizeof(lineResultNumData));
        memcpy(nOpenLinePrizeType, msg->nOpenLinePrizeType, sizeof(nOpenLinePrizeType));
        memcpy(nOpenLineCount, msg->nOpenLineCount, sizeof(nOpenLineCount));
        memcpy(nOpenLineRate, msg->nOpenLineRate, sizeof(nOpenLineRate));
        nMaryCount = msg->nMaryCount;
        
        cleanResults();
        transformOpenLotteryType();
        randomColumnDatasAndCalcTurnCount();
	}
    
    void GameTableUIV88::dealGameFinish(struct_s2c_game_finish* msg) {
        if (msg == nullptr) {
            return;
        }
//
//        msg->i64Jackpot;
//        msg->nFreeCount;
        m_bGameFinished = true;
        i64MyMoney = msg->i64MyMoney;
        tryShowFreeTurnUI(msg->nFreeCount);
        _root.rootnFreeNum = msg->nFreeCount;
        
        if (nFreeNum == 0 && msg->nFreeCount > 0) { //免费次数提示
            
            showFreeGetTip(msg->nFreeCount);
            
        }
        nFreeNum = msg->nFreeCount;
//        updateMyMoney(msg->i64MyMoney);
//        updateMyRealWinMoney(m_i64WinMoney - i64WinJackpot);
//        showJackpot();
        //_root.EnableButtons(true);
    }
    void GameTableUIV88::dealGameFreeCount(struct_s2c_free_count * msg)
    {
        nFreeNum = msg->nFreeCount;
        _root.rootnFreeNum = msg->nFreeCount;
        tryShowFreeTurnUI(msg->nFreeCount);
        m_currentMaxLine = msg->nLines - 1;
        updateSelectLines();
        m_currentNoteValue = msg->i64Notes;
        updateSelectNotes();
    }
    void GameTableUIV88::dealGameRobotInfo(struct_s2c_robot_info * msg)
    {
        if (_robotVec.size() == 0 && _robotParentNodeVec.size() == 4) {
            for (int i = 0;i < 4; i++) {
                
                 auto _robotNode = RobotInfo::create();
                 if (_robotNode) {
                     _robotNode->createRobotData(msg->userID[i],msg->fakeMoney[i],msg->nickName[i],msg->LogoID[i],msg->szWXTouXiang[i]);
                     _robotParentNodeVec.at(i)->addChild(_robotNode);
                     _robotVec.push_back(_robotNode);
                 }
            }
            
        }
        else
        {
            for (int i = 0;i < _robotVec.size(); i++) {
                
                RobotInfo* robotNode = _robotVec.at(i);
                if (robotNode) {
                    robotNode->updateRobotData(msg->userID[i],msg->fakeMoney[i],msg->nickName[i],msg->LogoID[i],msg->szWXTouXiang[i]);
                }
            }
            
        }
        
    }
    void GameTableUIV88::dealGameMaryInfo(struct_s2c_open_result * msg)
    {
        if (_maryLayer == nullptr) {
            return;
        }
        
        _maryLayer->dealBoxOpenResult(msg);
        
        
    }
    void GameTableUIV88::dealGameMaryEndInfo(struct_s2c_mary_result * msg)
    {
        if (_maryLayer == nullptr) {
            return;
        }
        
        _maryLayer->dealMaryEnd(msg);
        
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
	}

	void GameTableUIV88::resetUINewGame() {
		
		//resetAllNotesAnimationAndClean();
	}

	void GameTableUIV88::resetData() {


	}
    
    void GameTableUIV88::setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize) {
        float fStartPosY = parentSize.height / 2 - ICON_HEIGHT;
        float posX = parentSize.width/2;
        for (int i = 0; i < (nLotteryTypeCount == 0 ? 3:nLotteryTypeCount); i++) {
            vIconPos.push_back(Vec2(posX, fStartPosY + i*ICON_HEIGHT));
        }
    }
    
    void GameTableUIV88::setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum) {
        for (int i = 0; i < LOTTERY_COUNT_CSLL; i++) {
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
                if (index >=0 && index < LOTTERY_COUNT_CSLL) {
                    nOpenLotteryTypeColRow[i][j] = nOpenLotteryType[index];
                }
            }
        }
    }
    
    void GameTableUIV88::randomColumnDatasAndCalcTurnCount() {
        HNAudioEngine::getInstance()->playEffect(CSLL_startLoop);
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
                            sprintf(str, CSLL_ICON_TYPE, nOpenLotteryTypeColRow[i][index++]);
                        }
                        else {
                            sprintf(str, CSLL_ICON_TYPE, rand() % nLotteryTypeCount);
                        }
                        img->loadTexture(str);
                    }
                }
            }
        }

        //转动数据
        m_nHasTurnUnitCount = 0;
        m_vTurnUnitCount.clear();
        //m_vTurnUnitCount = {15,22,29,36,43};
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
                if (layout) {
                    auto moveDown = MoveBy::create(0.15f, Vec2(0, -30));
                    layout->runAction(Sequence::create(moveDown, DelayTime::create(0.05f),  moveDown->reverse(), nullptr));
                }
            }
        }), DelayTime::create(0.35f), CallFunc::create([=]() {
            schedule(schedule_selector(GameTableUIV88::turnUpdate), 0.05f);
			
        }), nullptr));
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
                        for (auto ii = 0; ii < 3; ii++)
                        {
                            memset(str, 0, sizeof(str));
                            sprintf(str, CSLL_ICON_TYPE, nOpenLotteryTypeColRow[i][ii]);
//                            auto img = m_vTurnIcon.at(i).at(ii);
                            auto tempTag = 1234567 + ii;
                            if (m_vTurnIcon.at(i).at(ii)->getParent()->getChildByTag(tempTag) == nullptr) {
                                //m_vTurnIcon.at(i).at(ii)->loadTexture(str);//底下的换成正确的结果
                                //stopRotateAnimation(i);
                                auto img = ImageView::create(str);
                                m_vTurnIcon.at(i).at(ii)->getParent()->addChild(img);
                                img->loadTexture(str);
                                img->setTag(tempTag);
                                auto staticPos = m_vTurnIconPos.at(ii);
                                img->setPosition(staticPos);
                                img->runAction(Sequence::create(MoveTo::create(0.10f,Vec2(staticPos.x,staticPos.y - 30)),MoveTo::create(0.10f,staticPos),NULL));
                                _resultImg.push_back(img);
                                if (ii == 0) {
                                    HNAudioEngine::getInstance()->playEffect(CSLL_endloop);
                                }
                               /* if (_root.stopBtn) {
                                    _root.stopBtn->setVisible(false);
                                }*/
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
        if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(LOTTERY_COLUMN_COUNT - 1))// && j == (LOTTERY_TYPE_COUNT_CSLL - 1))
        {
            unschedule(schedule_selector(GameTableUIV88::turnUpdate));
            if (nMaryCount > 0) {
                _maryLayer = CSLLMary::create();
                if (_maryLayer) {
                    _maryLayer->open(this,Vec2::ZERO,1000);
                    _maryLayer->setStartMaryCount(nMaryCount);
                    _maryLayer->setMaryMsgToServerCallBack([=](int boxTag){
                        struct_c2s_openBox msg;
                        msg.nBoxIdx = boxTag;
                        _GameTableLogic->sendMary(&msg);
                    });
                    _maryLayer->setMaryEndCallBack([=](long long _winMoney){
                        updateOpenLinesEffect();
                        checkHasReward(_winMoney); //小游戏开宝箱赢的钱
                        _maryLayer = nullptr;
                        
                        HNAudioEngine::getInstance()->stopBackgroundMusic();
                        HNAudioEngine::getInstance()->playBackgroundMusic(CSLL_BGM);
                    });
                }
                
            }
            else
            {
                updateOpenLinesEffect();
                checkHasReward(m_i64WinMoney - i64WinJackpot);
                
            }
            
           
            
        }
    }
	void GameTableUIV88::turnUpdateEx(float dt)
	{
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
						char str[128] = { 0 };
						for (auto ii = 0; ii < 3; ii++)
						{
							memset(str, 0, sizeof(str));
							sprintf(str, CSLL_ICON_TYPE, nOpenLotteryTypeColRow[i][ii]);
							//                            auto img = m_vTurnIcon.at(i).at(ii);
							auto tempTag = 1234567 + ii;
							if (m_vTurnIcon.at(i).at(ii)->getParent()->getChildByTag(tempTag) == nullptr) {
								//m_vTurnIcon.at(i).at(ii)->loadTexture(str);//底下的换成正确的结果
								//stopRotateAnimation(i);
								auto img = ImageView::create(str);
								m_vTurnIcon.at(i).at(ii)->getParent()->addChild(img);
								img->loadTexture(str);
								img->setTag(tempTag);
								auto staticPos = m_vTurnIconPos.at(ii);
								img->setPosition(staticPos);
								img->runAction(Sequence::create(MoveTo::create(0.10f, Vec2(staticPos.x, staticPos.y - 30)), MoveTo::create(0.10f, staticPos), NULL));
								_resultImg.push_back(img);
								if (ii == 0) {
									HNAudioEngine::getInstance()->playEffect(CSLL_endloop);
								}
								/* if (_root.stopBtn) {
									 _root.stopBtn->setVisible(false);
								 }*/
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
		if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(LOTTERY_COLUMN_COUNT - 1))// && j == (LOTTERY_TYPE_COUNT_CSLL - 1))
		{
			unschedule(schedule_selector(GameTableUIV88::turnUpdateEx));
			if (nMaryCount > 0) {
				_maryLayer = CSLLMary::create();
				if (_maryLayer) {
					_maryLayer->open(this, Vec2::ZERO, 1000);
					_maryLayer->setStartMaryCount(nMaryCount);
					_maryLayer->setMaryMsgToServerCallBack([=](int boxTag) {
						struct_c2s_openBox msg;
						msg.nBoxIdx = boxTag;
						_GameTableLogic->sendMary(&msg);
					});
					_maryLayer->setMaryEndCallBack([=](long long _winMoney) {
						updateOpenLinesEffect();
						checkHasReward(_winMoney); //小游戏开宝箱赢的钱
						_maryLayer = nullptr;

						HNAudioEngine::getInstance()->stopBackgroundMusic();
						HNAudioEngine::getInstance()->playBackgroundMusic(CSLL_BGM);
					});
				}

			}
			else
			{
				updateOpenLinesEffect();
				checkHasReward(m_i64WinMoney - i64WinJackpot);

			}



		}


	
	}
    void GameTableUIV88::checkHasReward(long long winMoney)
    {
        long long _realWinMoney = winMoney;
        
        if (_realWinMoney > 0) {
            int n_bigWinNum        = Tools::getLianxianGameData(_realWinMoney,m_currentNoteValue,80,200);
            if (n_bigWinNum == 0) {
                
                showMoneyFunc(_realWinMoney);
            }
            else if(n_bigWinNum == 1 || n_bigWinNum == 2)
            {
                
                showBiggerWinLayer(1,n_bigWinNum,winMoney);
                runAction(Sequence::create(DelayTime::create(3.0f),CallFunc::create([=](){
                    showMoneyFunc(_realWinMoney);
                    
                }),NULL));
            }
        }
        else
        {
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
        showFlower(true);
        showFourLevelMoney(m_level);
        runAction(Sequence::create(DelayTime::create(2.0f),CallFunc::create([=](){
            m_bAnimationFinished = true;
            updateMyMoney(i64MyMoney);
            
            updateMyRealWinMoney(winMoney);
            showGetRewardEffect();
            
            auto m_imgLight = ImageView::create("Games/31000507/ui/shan.png");
            if (m_imgLight) {
                m_imgLight->setPosition(Vec2(135,42));
                _root.gamePanel->addChild(m_imgLight);
                m_imgLight->runAction(Sequence::create(DelayTime::create(0.1f),CallFunc::create([=](){
                    m_imgLight->setScaleX(2.0f);
                }),RemoveSelf::create(),NULL));
            }
            
            if (nFreeNum > 0) { //有免费次数自动转
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
        for (int i = 0; i < LINE_COUNT; i++) {
            if (lineResultNumData[i] != -1) {
                lineImgNum.push_back(nLineConstitute[i]);
            }
        }
        
        if (lineImgNum.size() > 0)
        {
            vector<ImageView *>imgVector;
            imgVector.clear();
            
            
            auto blackImg = Layout::create();
            blackImg->setTouchEnabled(false);
            blackImg->setContentSize(_root.gamePanel->getContentSize());
            blackImg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            blackImg->setPosition(WIN_SIZE/2);
           // blackImg->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
            
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
                                std::string animationName = "CSLL_ICON_EFFECT";
                                Animation * anima = AnimationCache::getInstance()->getAnimation(animationName);
                                if (anima == nullptr) {
                                    anima = Animation::create();
                                    anima->setDelayPerUnit(0.10f);
                                    anima->setLoops(-1);
                                    
                                    for (int i=startIdx; i <= endIdx; i++) {
                                        memset(str,0,sizeof(str));
                                        sprintf(str, "cs_000%02d.png",i);
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
                                img->addChild(animationSprite,1);
                            }
                            
                            auto mSpineNode = img->getChildByName("ANIMATON_SPINE");
                            if (mSpineNode == nullptr) {
                                int nIconType = nOpenLotteryType[posIdx];
                                if (nIconType >= 0 && nIconType <= 11) {
                                    vector<std::string>m_spineData = getGameIconSpineName(nIconType);
                                    if (m_spineData.size() == 2) {
                                        auto jsonPath      = m_spineData.at(0) + ".json";
                                        auto atlasPath     = m_spineData.at(0) + ".atlas";
                                        
                                        auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
                                        if (skeletonNode) {
                                            skeletonNode->setPosition(getGameIconSpinePos(nIconType));
                                            skeletonNode->setAnimation(0,m_spineData.at(1),true);
                                            skeletonNode->setName("ANIMATON_SPINE");
                                            img->addChild(skeletonNode);
                                        }
                                    }
                                }
                                
                            }
                            
                            
                        }
                        
                        
                    }
                    
                }
                
            }
        }
        
    }
    vector<std::string> GameTableUIV88::getGameIconSpineName(int _typeId)
    {
        vector<std::string> spineData;
        switch (_typeId) {
            case 11:
                spineData.push_back("Games/31000507/spine/cs_caishen");
                spineData.push_back("cs_caishen_1");
                break;
            case 10:
                spineData.push_back("Games/31000507/spine/cs_tongzi");
                spineData.push_back("cs_tongzi");
                break;
            case 9:
                spineData.push_back("Games/31000507/spine/cs_shizi");
                spineData.push_back("cs_shizi");
                break;
            case 8:
                spineData.push_back("Games/31000507/spine/cs_mile");
                spineData.push_back("cs_mile");
                break;
            case 7:
                spineData.push_back("Games/31000507/spine/cs_yuruyi");
                spineData.push_back("cs_yuruyi");
                break;
            case 6:
                spineData.push_back("Games/31000507/spine/cs_jinyu");
                spineData.push_back("cs_jinyu");
                break;
            case 5:
                spineData.push_back("Games/31000507/spine/cs_yuanbao");
                spineData.push_back("cs_yuanbao");
                break;
            default:
                break;
        }
        
        return spineData;
    }
    Vec2 GameTableUIV88::getGameIconSpinePos(int _typeId)
    {
        Vec2 iconSpinePos(0,0);
        switch (_typeId) {
            case 11:
                iconSpinePos = Vec2(4,4);
                break;
            case 10:
                iconSpinePos = Vec2(6,7);
                break;
            case 9:
                iconSpinePos = Vec2(4,7);
                break;
            case 8:
                iconSpinePos = Vec2(6,9);
                break;
            case 7:
                iconSpinePos = Vec2(4,11);
                break;
            case 6:
                iconSpinePos = Vec2(6,7);
                break;
            case 5:
                iconSpinePos = Vec2(4,11);
                break;
            default:
                break;
        }
        
        return iconSpinePos;
        
        
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
        //updateMyRealWinMoney(m_i64WinMoney - i64WinJackpot);
        showJackpot();
        
        if (openLinesCount > 0) {
            //m_bHasCashIn = false;
            //HNAudioEngine::getInstance()->playEffect(CSLL_bingo);
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
            _root.myMoney->setString(Tools::goldDisplayByType(money).c_str());
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
        if (_root.NoteTotal) {
            _root.NoteTotal->setString(to_string(m_currentNoteValue * (m_currentMaxLine+1)));
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
            coin->openParent(this, Vec2(WIN_SIZE.width*0.5f, WIN_SIZE.height-120), ZOrder_Float_Coin);
            coin->updateCoin(money);
        }
        if (_root.score) {
            _root.score->setString(to_string(money));
        }
        //if (money > 0) {
           // GameRewardEffect::createEffect(this);
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
            _root.EnableButtons(true);
        }
        
        // 自动
        if (((i64MyMoney > 0 &&
              m_currentNoteValue * (m_currentMaxLine + 1) < i64MyMoney) || nFreeNum > 0 ) &&
            m_bHasCashIn &&
            m_bAuto &&
             _GameTableLogic->getGameStatus() == GS_FREE && m_bAnimationFinished)
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
    
    // ---------------------- 测试代码 ---------------------------
    void GameTableUIV88::TestLuckyUI()
    {
//        auto btn = Button::create("Reward/ui/btnOpen.png");
//        btn->setTitleText("测试Lucky");
//        btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::TestLuckyUICallBack, this));
//        this->addChild(btn, 200);
//        btn->setPosition(Vec2(500, 600));
    }
    
    void GameTableUIV88::TestLuckyUICallBack(Ref* pSender, Widget::TouchEventType type)
    {
//        if (Widget::TouchEventType::ENDED != type)
//        {
//            return;
//        }
////
//        LuckyMoneyNotify notify;
//            notify.nStayTime = 2.0f;
//
//            this->onLuckyMoneyNotify(&notify);
////        }, DelayTime::create(0.5f), [&](){
////            LuckyMoneyResult result;
////            result.nType = -1;
////            result.i64MyMoney = 10000;
////            result.i64LuckyMoney = 1000;
////
////            this->onLuckyMoneyResult(&result);
////        }, nullptr));
    }
    // ---------------------- 测试代码 End ---------------------------
    
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
        if (getChildByName("blackImg")) {
            getChildByName("blackImg")->removeFromParent();
        }
        openLineVec.clear();
    }
    void GameTableUIV88::showFlower(bool show)
    {
        if (_root.m_flower) {
            if (show) {
                _root.m_flower->setVisible(true);
                _root.m_flower->setScale(0.1f);
                _root.m_flower->runAction(ScaleTo::create(0.7f,1.5f));
                
                auto jsonPath      = "Games/31000507/animation/tongqian/tongqian.json";
                auto atlasPath     = "Games/31000507/animation/tongqian/tongqian.atlas";
                
                auto skeletonNode  = spine::SkeletonAnimation::createWithJsonFile(jsonPath,atlasPath);
                if (skeletonNode) {
                    skeletonNode->setPosition(Vec2(WIN_SIZE.width/2 - 640,0));
                    skeletonNode->setAnimation(0, "tongqian",false);
                    skeletonNode->setEndListener([=](int trackIndex){
                        
                        skeletonNode->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
                    });
                    addChild(skeletonNode,1000);
                }
            }
            else
            {
                _root.m_flower->setVisible(false);
            }
        
        }
        
       
    
    }
    void GameTableUIV88::createYunDuo()
    {
        vector<int>yPosList = {167,322,481,650};
        for (int i = 0; i <= 19; i++) {
            auto imgYunDuo = ImageView::create("Games/31000507/ui/yunduo.png");
            if (imgYunDuo) {
                int maxWidth = WIN_SIZE.width + 150;
                int x = random(-150,maxWidth);
                int y = yPosList.at(random(0,3));
                imgYunDuo->setPosition(Vec2(x,y));
                startYunDuoMove(imgYunDuo);
                _root.bg->addChild(imgYunDuo);
            }
           
        }
        
    }
    void GameTableUIV88::startYunDuoMove(ImageView* yunduo)
    {
        if (yunduo) {
            auto posX   = yunduo->getPositionX();
            posX = posX >= WIN_SIZE.width + 150 ? WIN_SIZE.width + 150 : posX;
            posX = posX <= -150 ? -150 : posX;
            int endPosx = 0;
            if (posX < WIN_SIZE.width/2) {
                yunduo->setScaleX(-1.0f);
                endPosx = random(100,800);
            }
            else
            {
                yunduo->setScaleX(1.0);
                endPosx = 0 - random(100,800);
            }
            
            yunduo->runAction(Sequence::create(MoveBy::create(abs(endPosx)/20,Vec2(endPosx,0)),CallFunc::create([=](){
                this->startYunDuoMove(yunduo);
            }),NULL));
            
            
        }
        
    }
    void GameTableUIV88::showFreeGetTip(int num)
    {
        if (_root.m_freeGetTip == nullptr) {
            return;
        }
        
        auto _textAtlas = dynamic_cast<TextAtlas*>(_root.m_freeGetTip->getChildByName("AtlasLabel_1"));
        if (_textAtlas) {
            _textAtlas->setString(StringUtils::format("%d",num));
        }
        _root.m_freeGetTip->setVisible(true);
        _root.m_freeGetTip->setScale(0.1f);
        _root.m_freeGetTip->runAction(Sequence::create(ScaleTo::create(0.3f,1.0f),DelayTime::create(2.0f),CallFunc::create([=](){
            _root.m_freeGetTip->setVisible(false);
        }),NULL));
        
        
        
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
        }),DelayTime::create(0.45f),CallFunc::create([=](){
            m_AnimationNodeVec.clear();
            for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                
                int m_animationType = random(1,1);
                int startIdx = 0;
                int endIdx   = 6;
                
                std::string animationName = StringUtils::format("CSLL_ICON_ROTATE_%d",m_animationType);
                Animation * anima = AnimationCache::getInstance()->getAnimation(animationName);
                if (anima == nullptr) {
                    anima = Animation::create();
                    anima->setDelayPerUnit(0.10f);
                    anima->setLoops(-1);
                    
                    for (int j = startIdx; j <= endIdx; j++) {
                        std::string str = StringUtils::format("cs%d_0000%d.png",m_animationType,j);
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
