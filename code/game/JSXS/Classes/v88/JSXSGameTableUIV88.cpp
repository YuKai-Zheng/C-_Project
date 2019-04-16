#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
#include "HNCommon/HNConverCode.h"

#include "JSXSGameTableUIV88.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "JSXSSettlementV88.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"
#include "UI/HelpLayer.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/FloatCoin.h"
#include "base/CCRef.h"
#include "HNLobby/GameRewardEffect.hpp"
#include "HNLobby/PlatformDefine.h"
#include "UI/UserHead.h"
#include "Tool/XCaseStringUtil.hpp"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#define ICON_HEIGHT 142

namespace JSXS {


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
    
    const static char* JSXS_BGM = "Games/31000508/music/JSXS_BGM.mp3";
    const static char* JSXS_EFFECT = "Games/31000508/music/jinyouxi.mp3";
    const static char* JSXS_bingo = "Games/31000508/music/JSXS_bingo.mp3";
    const static char* JSXS_startLoop = "Games/31000508/music/JSXS_startLoop.mp3";
    const static char* JSXS_4Loop = "Games/31000508/music/top4Stop.wav";
    const static char* JSXS_endLoop = "Games/31000508/music/endStop.wav";
     const static char* JSXS_btnClick = "Games/31000508/music/click.mp3";
    const static char* kIconTypeTpl = "Games/31000508/type/LT_%02d.png";
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
        , m_bGameFinished(true)
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

	GameTableUIV88::~GameTableUIV88() {

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
        //setBackGroundImgFunc("v88/JSXS/ui/bigbg.png",this);
        ImageView* bg = ImageView::create("Games/31000508/ui_v2/jsxs_bg1.png");
        if (bg) {
            _root.bg = bg;
            bg->setPosition(WIN_SIZE/2);
            addChild(bg, ZOrder_Bg);

			/*std::string jsonPath = "Games/31000508/spine/jsxs_beijing_effect.json";
			std::string atlasPath = "Games/31000508/spine/jsxs_beijing_effect.atlas";

			auto skeletonNode = spine::SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath);
			if (skeletonNode) {
				Vec2 vPos(800, -720);
				skeletonNode->setPosition(vPos);
				skeletonNode->setAnimation(0, "jsxs_beijing_effect", true);
				skeletonNode->setName("ANIMATON_BG");
				_root.bg->addChild(skeletonNode);
			}*/
        }

        //middle Layer
        Node* middle = CSLoader::createNode("Games/31000508/Node_root.csb");
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
					char str[128] = { 0 };
					for (int i = 0; i < LINE_COUNT; i++)
					{
						memset(str, 0, sizeof(str));
						sprintf(str, "Image_line_%d", i + 1);
						auto line = dynamic_cast<ImageView*>(lines->getChildByName(str));
						if (line) {
							line->setVisible(false);
							line->setScaleX(0.85f);
							m_vAllLines[i] = line;
						}
					}
				}
                
                /*auto fireNode = dynamic_cast<Node*>(Tools::getNodeByName(bg, "Node_fire"));
                if (fireNode) {
                    auto topfire = CSLoader::createNode("Games/31000508/Node_fire.csb");
                    auto topfireaction = CSLoader::createTimeline("Games/31000508/Node_fire.csb");
                    if (topfire && topfireaction)
                    {
                        fireNode->addChild(topfire);
                        topfire->setPosition(Vec2::ZERO);
                        topfire->runAction(topfireaction);
                        topfireaction->gotoFrameAndPlay(0);
                    }
                    
                }*/
                
                auto lzNodeLeft = dynamic_cast<Node*>(Tools::getNodeByName(bg, "Node_lz_left"));
                if (lzNodeLeft) {
                    auto bottomfire = CSLoader::createNode("Games/31000508/Node_lz.csb");
                    auto bottomfireaction = CSLoader::createTimeline("Games/31000508/Node_lz.csb");
                    if (bottomfire && bottomfireaction)
                    {
						lzNodeLeft->addChild(bottomfire);
                        bottomfire->setPosition(Vec2::ZERO);
                        bottomfire->runAction(bottomfireaction);
                        bottomfireaction->gotoFrameAndPlay(0);
                    }
                }

				auto lzNodeRight = dynamic_cast<Node*>(Tools::getNodeByName(bg, "Node_lz_right"));
				if (lzNodeRight) {
					auto bottomfire = CSLoader::createNode("Games/31000508/Node_lz.csb");
					auto bottomfireaction = CSLoader::createTimeline("Games/31000508/Node_lz.csb");
					if (bottomfire && bottomfireaction)
					{
						lzNodeRight->addChild(bottomfire);
						bottomfire->setPosition(Vec2::ZERO);
						bottomfire->runAction(bottomfireaction);
						bottomfireaction->gotoFrameAndPlay(0);
					}
				}
                
				/* auto fz1Node = dynamic_cast<Node*>(Tools::getNodeByName(bg, "Node_fz1"));
                if (fz1Node) {
                    auto nodeFz = CSLoader::createNode("Games/31000508/Node_fz1.csb");
                    auto nodeFzAction = CSLoader::createTimeline("Games/31000508/Node_fz1.csb");
                    if (nodeFz && nodeFzAction)
                    {
                        fz1Node->addChild(nodeFz);
                        nodeFz->setPosition(Vec2::ZERO);
                        nodeFz->runAction(nodeFzAction);
                        nodeFzAction->gotoFrameAndPlay(0);
                    }
                }*/
                
				/*auto fz2Node = dynamic_cast<Node*>(Tools::getNodeByName(bg, "Node_fz2"));
                if (fz2Node) {
                    auto nodeFz = CSLoader::createNode("Games/31000508/Node_fz2.csb");
                    auto nodeFzAction = CSLoader::createTimeline("Games/31000508/Node_fz2.csb");
                    if (nodeFz && nodeFzAction)
                    {
                        fz2Node->addChild(nodeFz);
                        nodeFz->setPosition(Vec2::ZERO);
                        nodeFz->runAction(nodeFzAction);
                        nodeFzAction->gotoFrameAndPlay(0);
                    }
                }*/
                
                
               /* auto imageTitleFire = dynamic_cast<ImageView*>(bg->getChildByName("Image_titleFire"));
                if (imageTitleFire) {
                    imageTitleFire->runAction(RepeatForever::create(Sequence::create(FadeIn::create(1.0f),FadeOut::create(1.0f),NULL)));
                }*/
                
                
                /*auto imageCharm = dynamic_cast<ImageView*>(bg->getChildByName("Image_14"));
                if (imageCharm) {
                    _root.charmImg = imageCharm;
                    _root.charmImg->setVisible(false);
                    _root.charmImg->setPositionY(_root.charmImg->getPositionY() + 100);
                }*/
                
                
                Text* myMoney = dynamic_cast<Text*>(Tools::getNodeByName(bg, "Text_money"));
                if (myMoney) {
                    _root.myMoney = myMoney;
                }
                
                
                Button* start = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_start"));
                if (start) {
                    _root.startBtn = start;
                    start->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::startBtnCallBack, this));
                }
                
                /*Button* stop = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_stop"));
                if (stop) {
                    _root.stopBtn = stop;
                    _root.stopBtn->setVisible(false);
                    _root.stopBtn->setTouchEnabled(false);
                    stop->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::stopBtnCallBack, this));
                }*/
                
                
                Button* autoBtn = dynamic_cast<Button*>(Tools::getNodeByName(bg, "Button_auto"));
                if (autoBtn) {
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
	//}

    void GameTableUIV88::loadTurn() {
        char str[128] = {0};
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            if (_root.middleNode) {
                auto bg = dynamic_cast<Layout*>(_root.middleNode->getChildByName("Panel_bg"));
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
        JSXSSettlementV88* settle = JSXSSettlementV88::create();
        if (settle) {
            settle->setPosition(WIN_SIZE/2);
            _gameSettlement = settle;
            _gameSettlement->hideSettleLayer();
            addChild(_gameSettlement, ZOrder_SettlementLayer);
        }
    }

    void GameTableUIV88::onEnter() {
		HNGameUIBase::onEnter();
        HNAudioEngine::getInstance()->playBackgroundMusic(JSXS_BGM, true);
        HNAudioEngine::getInstance()->playEffect(JSXS_EFFECT);
        //触摸
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
		myListener->setSwallowTouches(false);
		dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

        initData();
        
        showLoadingResUI(LOAD_JSXS_GAME);
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
        schedule(schedule_selector(GameTableUIV88::fireUpdate),5.0f);
        
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
                gameSet->setBgPath(JSXS_BGM);
				gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
			});
            toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
                auto helpLayer = HN::HelpLayer::create();
                helpLayer->setContentOnView("v66/common/help/JSXS.png");
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
        unschedule(schedule_selector(GameTableUIV88::fireUpdate));
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
         HNAudioEngine::getInstance()->playEffect(JSXS_btnClick);
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
        //addLineLogic();
	}

    //加注按钮
    void GameTableUIV88::addNoteBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
       // addNoteLogic();
         HNAudioEngine::getInstance()->playEffect(JSXS_btnClick);
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
        HNAudioEngine::getInstance()->playEffect(JSXS_btnClick);
        maxLineLogic();
    }
    
    void GameTableUIV88::nineLineBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type)
    {
        if (type != Widget::TouchEventType::ENDED)
        {
            return;
        }
        HNAudioEngine::getInstance()->playEffect(JSXS_btnClick);
        m_currentMaxLine = 8;
        updateSelectLines();

    }
    
    //满注按钮
    void GameTableUIV88::maxNoteBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        HNAudioEngine::getInstance()->playEffect(JSXS_btnClick);
        maxNoteLogic();
       
    }
    
    //自动按钮
    void GameTableUIV88::autoBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        HNAudioEngine::getInstance()->playEffect(JSXS_btnClick);
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
        HNAudioEngine::getInstance()->playEffect(JSXS_btnClick);
        
        if (_root.stopBtn) {
            _root.stopBtn->setVisible(false);
        }
        
    }
    
	//开始按钮
	void GameTableUIV88::startBtnCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        //auto startBtn = (Button*)pSender;
        if (type != Widget::TouchEventType::ENDED) {
            /*if (type == Widget::TouchEventType::BEGAN) {
                
                startBtn->setScale(1.1f);
            }
            else if (type == Widget::TouchEventType::CANCELED)
            {
                
                startBtn->setScale(1.0f);
            }*/
            return;
        }
       // startBtn->setScale(1.0f);
        HNAudioEngine::getInstance()->playEffect(JSXS_btnClick);
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
        if (_gameSettlement) {
            _gameSettlement->hideSettleLayer();
        }
        if (_root.score) {
            _root.score->setString(to_string(0));
        }
        if (_GameTableLogic) {
            m_bHasCashIn = false;
            m_bGameFinished = false;
            struct_c2s_start msg;
            msg.nNoteLines = m_currentMaxLine + 1;
            msg.i64NoteMoney = m_currentNoteValue;
            _GameTableLogic->sendStart(&msg);
        }
        
        _root.EnableButtons(false);
        //showBeginThreeCharm();
        //showImgLightEffect(2);
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
//        msg->nLineConstitute;
        m_currentNoteValue = i64NoteMin;
        memcpy(nLineConstitute, msg->nLineConstitute, sizeof(nLineConstitute));
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
        nMaryCount    = msg->nMaryCount;
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
        tryShowFreeTurnUI(msg->nFreeCount);
        _root.rootnFreeNum = msg->nFreeCount;
        if (nFreeNum == 0 && msg->nFreeCount > 0) { //免费次数提示
            
            showFreeGetTip(msg->nFreeCount);
            
        }
        nFreeNum = msg->nFreeCount;
//        msg->i64Jackpot;
//        msg->nFreeCount;
//        updateMyMoney(msg->i64MyMoney);
//        updateMyRealWinMoney(m_i64WinMoney - i64WinJackpot);
//        showJackpot();
//        _root.EnableButtons(true);
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
                _robotNode->createRobotData(msg->userID[i],msg->fakeMoney[i],msg->nickName[i],msg->LogoID[i],msg->szWXTouXiang[i]);
                //addChild(_robotNode);
                _robotParentNodeVec.at(i)->addChild(_robotNode);
                _robotVec.push_back(_robotNode);
                
            }
            
        }
        else
        {
            for (int i = 0;i < _robotVec.size(); i++) {
                
                RobotInfo* robotNode = _robotVec.at(i);
                robotNode->updateRobotData(msg->userID[i],msg->fakeMoney[i],msg->nickName[i],msg->LogoID[i],msg->szWXTouXiang[i]);
                
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

	void GameTableUIV88::resetData()
    {
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
                            
                            
                            auto tempTag = 1234567 + (i*LOTTERY_COLUMN_COUNT + ii);
                            if (m_vTurnIcon.at(i).at(ii)->getParent()->getChildByTag(tempTag) == NULL) {
                                //m_vTurnIcon.at(i).at(ii)->loadTexture(str);//底下的换成正确的结果
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
                                if (ii == 0 && i < LOTTERY_COLUMN_COUNT - 1) {
                                    HNAudioEngine::getInstance()->playEffect(JSXS_4Loop);
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
        if (m_nHasTurnUnitCount == m_vTurnUnitCount.at(LOTTERY_COLUMN_COUNT - 1))// && j == (LOTTERY_TYPE_COUNT_JSXS - 1))
        {
            unschedule(schedule_selector(GameTableUIV88::turnUpdate));
            HNAudioEngine::getInstance()->playEffect(JSXS_endLoop);
            if (nMaryCount > 0) {
                _maryLayer = JSXSMary::create();
                if (_maryLayer) {
                    _maryLayer->open(this,Vec2::ZERO,1000);
                    _maryLayer->setMaryMsgToServerCallBack([=](int fzTag){
                        struct_c2s_openBox msg;
                        msg.nBoxIdx = fzTag;
                        _GameTableLogic->sendMary(&msg);
                    });
                    _maryLayer->setMaryEndCallBack([=](long long _winMoney){
                        updateOpenLinesEffect();
                        checkHasReward(_winMoney); //小游戏开宝箱赢的钱
                        _maryLayer = nullptr;
                        
                        HNAudioEngine::getInstance()->stopBackgroundMusic();
                        HNAudioEngine::getInstance()->playBackgroundMusic(JSXS_BGM, true);
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
    void GameTableUIV88::checkHasReward(long long _winmoney)
    {
        long long _realWinMoney = _winmoney;
        
        if (_realWinMoney > 0) {
            int n_bigWinNum        = Tools::getLianxianGameData(_realWinMoney,m_currentNoteValue,80,200);
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
        showCharm();
        showFourLevelMoney(m_level);
        runAction(Sequence::create(DelayTime::create(2.0f),CallFunc::create([=](){
            m_bAnimationFinished = true;
            updateMyMoney(i64MyMoney);
            //showImgLightEffect(1);
            updateMyRealWinMoney(winMoney);//m_i64WinMoney - i64WinJackpot
            showGetRewardEffect();
            
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
            
            addChild(blackImg,159);
            blackImg->setName("blackImg");
            blackImg->setOpacity(200);
            
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
                auto panelTurn = m_vPanelTurnArea[i]->clone();
                panelTurn->setTag(333 + i);
                blackImg->addChild(panelTurn,1);
            }
            
            int idxmap[] = {2,5,8,11,14,1,4,7,10,13,0,3,6,9,12};
            for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
                auto panel = blackImg->getChildByTag(333 + i);
                for (int j = 0; j < 3; j++) {
                    auto img = (ImageView*)(panel->getChildByTag(1234567 + i * LOTTERY_COLUMN_COUNT + j));
                    if (img) {
                        img->setVisible(false);
                        imgVector.push_back(img);
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
                                std::string animationName = "JSXS_ICON_EFFECT";
                                Animation * anima = AnimationCache::getInstance()->getAnimation(animationName);
                                if (anima == nullptr) {
                                    anima = Animation::create();
                                    anima->setDelayPerUnit(0.10f);
                                    anima->setLoops(-1);
                                    
                                    for (int i=startIdx; i <= endIdx; i++) {
                                        memset(str,0,sizeof(str));
                                        sprintf(str, "mds_000%02d.png",i);
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
                spineData.push_back("Games/31000508/spine/js_daoshi");
                spineData.push_back("js_daoshi_1");
                break;
            case 10:
                spineData.push_back("Games/31000508/spine/js_shenfu");
                spineData.push_back("js_shenfu");
                break;
            case 9:
                spineData.push_back("Games/31000508/spine/js_heshang");
                spineData.push_back("js_heshang");
                break;
            case 8:
                spineData.push_back("Games/31000508/spine/js_dajiangshi");
                spineData.push_back("js_dajiangshi");
                break;
            case 7:
                spineData.push_back("Games/31000508/spine/js_nvgui");
                spineData.push_back("js_nvgui");
                break;
            case 6:
                spineData.push_back("Games/31000508/spine/js_xiaogui");
                spineData.push_back("js_xiaogui");
                break;
            case 5:
                spineData.push_back("Games/31000508/spine/js_xiaojiangshi");
                spineData.push_back("js_xiaojiangshi");
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
                iconSpinePos = Vec2(0,8);
                break;
            case 10:
                iconSpinePos = Vec2(0,5);
                break;
            case 9:
                iconSpinePos = Vec2(0,5);
                break;
            case 8:
                iconSpinePos = Vec2(0,5);
                break;
            case 7:
                iconSpinePos = Vec2(-2,0);
                break;
            case 6:
                iconSpinePos = Vec2(-2,2);
                break;
            case 5:
                iconSpinePos = Vec2(-2,0);
                break;
            default:
                break;
        }
        
        return iconSpinePos;
        
        
    }
    void GameTableUIV88::setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize) {
        float fStartPosY = parentSize.height / 2 - ICON_HEIGHT;
        
        for (int i = 0; i < (nLotteryTypeCount == 0 ? 3:nLotteryTypeCount); i++) {
            vIconPos.push_back(Vec2(parentSize.width/2, fStartPosY + i*ICON_HEIGHT));
        }
    }
    
    void GameTableUIV88::setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum) {
        for (int i = 0; i < LOTTERY_COUNT_JSXS; i++) {
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
                if (index >=0 && index < LOTTERY_COUNT_JSXS) {
                    nOpenLotteryTypeColRow[i][j] = nOpenLotteryType[index];
                }
            }
        }
    }
    
    void GameTableUIV88::randomColumnDatasAndCalcTurnCount() {
        HNAudioEngine::getInstance()->playEffect(JSXS_startLoop);
        
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
                if (layout) {
                    auto moveDown = MoveBy::create(0.15f, Vec2(0, -30));
                    layout->runAction(Sequence::create(moveDown, DelayTime::create(0.05f),  moveDown->reverse(), nullptr));
                }
            }
        }), DelayTime::create(0.35f), CallFunc::create([=]() {
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
        
       
        //showJackpot();
        
        if (openLinesCount > 0) {
            //m_bHasCashIn = false;
            //HNAudioEngine::getInstance()->playEffect(JSXS_bingo);
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
        if (_root.NoteTotal) {
            _root.NoteTotal->setString(to_string(m_currentNoteValue * (m_currentMaxLine+1)));
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
            _root.jackpot->setString(Tools::addComma(i64Jackpot));
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
        openLineVec.clear();
    }
    void GameTableUIV88::showCharm()
    {
        
        if (_root.charmImg) {
            _root.charmImg->setVisible(true);
            auto _charmChild = _root.charmImg->getChildByName("Image_15");
            if (_charmChild) {
                _charmChild->runAction(FadeIn::create(0.0f));
                _charmChild->runAction(Sequence::create(ScaleTo::create(0.5f,1.5f),FadeOut::create(0.5f),CallFunc::create([=](){
                    _root.charmImg->setVisible(false);
                    
                }),NULL));
            }
        }
        
    }
    void GameTableUIV88::showBeginThreeCharm()
    {
        vector<Vec2>  posVec   = {Vec2(1110,143),Vec2(1190,132),Vec2(1038,130)};
        vector<float> scaleVec = {0.8f,0.7f,0.6f};
        
        for (int m = 1; m <= 3; m++) {
             auto startCharm = ImageView::create("Games/31000508/ui/fz4.png");
             if (startCharm) {
                 startCharm->setPosition(posVec.at(m-1));
                 startCharm->setScale(scaleVec.at(m-1));
                 _root.gamePanel->addChild(startCharm);
                 startCharm->setVisible(false);
                 
                 startCharm->runAction(Sequence::create(DelayTime::create(m*0.1f),CallFunc::create([=](){
                     startCharm->setVisible(true);
                 }),Spawn::create(MoveBy::create(0.5f,Vec2(0,30)),FadeOut::create(0.5f), NULL),RemoveSelf::create(),NULL));
             }
        }
    }
    void GameTableUIV88::showImgLightEffect(int showType) //showType 1金币收分。2开始按钮
    {
        Vec2 posLight = showType == 1 ? Vec2(135,42) : Vec2(1110,64);
        auto m_imgLight = ImageView::create("Games/31000508/ui/shan.png");
        if (m_imgLight) {
            m_imgLight->setPosition(posLight);
            _root.gamePanel->addChild(m_imgLight);
            m_imgLight->runAction(Sequence::create(DelayTime::create(0.1f),CallFunc::create([=](){
                m_imgLight->setScaleX(2.0f);
            }),RemoveSelf::create(),NULL));
        }
        
    }
    void GameTableUIV88::fireUpdate(float dt)
    {
        
		if (WIN_SIZE.width <= 1280)
		{
			return;
		}

        int leftFireNum  = random(1,3);
        int rightFireNum = random(1,3);
        
        
        
        for (int i = 0; i < leftFireNum; i++) {
            auto node = Node::create();
            int offX = WIN_SIZE.width/2 - 640;
            int staticX = 10;
            
            node->setPositionX(random(staticX,offX));
            node->setPositionY(random(140,280));
            _root.gamePanel->addChild(node);
            
            
            auto topfire = CSLoader::createNode("Games/31000508/Node_fire.csb");
            auto topfireaction = CSLoader::createTimeline("Games/31000508/Node_fire.csb");
            if (topfire && topfireaction)
            {
                node->addChild(topfire);
                topfire->setPosition(Vec2::ZERO);
                topfire->runAction(topfireaction);
                topfireaction->gotoFrameAndPlay(0);
                topfire->runAction(FadeOut::create(5.0f));
            }
            
            node->runAction(Sequence::create(Spawn::create(MoveBy::create(5.0f,Vec2(0,random(300,400))),FadeOut::create(5.0f),NULL),RemoveSelf::create(),NULL));
            
            
        }
        
        for (int i = 0; i < rightFireNum; i++) {
            auto node = Node::create();
            int offX = WIN_SIZE.width - 20;
            int staticX = 1280;
            
            node->setPositionX(random(staticX,offX));
            node->setPositionY(random(140,280));
            _root.gamePanel->addChild(node);
            
            
            auto topfire = CSLoader::createNode("Games/31000508/Node_fire.csb");
            auto topfireaction = CSLoader::createTimeline("Games/31000508/Node_fire.csb");
            if (topfire && topfireaction)
            {
                node->addChild(topfire);
                topfire->setPosition(Vec2::ZERO);
                topfire->runAction(topfireaction);
                topfireaction->gotoFrameAndPlay(0);
                topfire->runAction(FadeOut::create(5.0f));
            }
            
            node->runAction(Sequence::create(Spawn::create(MoveBy::create(5.0f,Vec2(0,random(300,400))),FadeOut::create(5.0f),NULL),RemoveSelf::create(),NULL));
        }
    
    }
    void GameTableUIV88::showFreeGetTip(int num)
    {
        if (_root.m_freeGetTip == nullptr) {
            return;
        }
        
        auto _text = dynamic_cast<Text*>(_root.m_freeGetTip->getChildByName("Text_free"));
        if (_text) {
            _text->setString(StringUtils::format("%d",num));
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
        }),DelayTime::create(0.35f),CallFunc::create([=](){
        m_AnimationNodeVec.clear();
        for (int i = 0; i < LOTTERY_COLUMN_COUNT; i++) {
            
            int m_animationType = random(1,2);
            int startIdx = 0;
            int endIdx   = 5;
            
            std::string animationName = StringUtils::format("JSXS_ICON_ROTATE_%d",m_animationType);
            Animation * anima = AnimationCache::getInstance()->getAnimation(animationName);
            if (anima == nullptr) {
                anima = Animation::create();
                anima->setDelayPerUnit(0.10f);
                anima->setLoops(-1);
                
                for (int j = startIdx; j <= endIdx; j++) {
                    std::string str = StringUtils::format("js%d_0000%d.png",m_animationType,j);
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
