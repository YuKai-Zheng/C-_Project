#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"


#include "RedPacketGameTableUI.h"
#include "RedPacketGameTableLogic.h"
#include "RedPacketGamePlayerCellLayer.h"
#include "RedPacketAllPlayersListLayer.h"
#include "RedPacketSendPacketLayer.h"
#include "RedPacketOpenDetailsLayer.h"
#include "RedPacketMyRecordLayer.h"
#include "RedPacketMyRecordCellLayer.h"

#include "HNLobby/GamePrompt.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/state/PlatformState.h"
#include "base/ChangeRootViewController.h"
#include "base/WeChatAndroid.h"
#include "HNLobby/v88/PlatformResV88.h"
#include "UI/Scanner.h"

namespace RedPacket {

#define WINSIZE Director::getInstance()->getWinSize()
#define NOTICE_ZORDER 100

#define UI_ROTATE_VALUE -90

//////////////////////////////////////////////////////////////////////////

HNGameUIBase* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate) {
    GameTableUI* tableUI = new GameTableUI();
    if (tableUI && tableUI->init(bDeskIndex, bAutoCreate)) {
        tableUI->autorelease();
    } else {
        delete tableUI;
        tableUI = nullptr;
    }
    return tableUI;
}

GameTableUI::GameTableUI()
//    : m_iBetMoneyType(-1)
{
    //    TextureCache::getInstance()->addImage(NN_GOLD_IMAGE);
}

GameTableUI::~GameTableUI() {
//    this->unschedule(schedule_selector(GameTableUI::updateClock));
//    this->unschedule(schedule_selector(GameTableUI::updateTip));
//    this->unschedule(schedule_selector(GameTableUI::updateLoopSpritePosition));
//    this->unschedule(schedule_selector(GameTableUI::updateResultAnimation));
//    this->unschedule(schedule_selector(GameTableUI::showGameStateLabelAnimation));
//    this->unschedule(schedule_selector(GameTableUI::updateRandomBlinkLightFree));

//    this->unschedule(schedule_selector(GameTableUI::autoRemoveNianShouButton));

    //this->unschedule(schedule_selector(GameTableUI::sendCardTimer));
//    this->unschedule(schedule_selector(GameTableUI::showOpenBtn));
    CC_SAFE_DELETE(_GameTableLogic);

    resetData();
}

bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }
    
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	ChangeRootViewController::changeRootViewControllerV();
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	WeChatAndroid::getInstance()->changeScreenDirection(1, WINSIZE.width, WINSIZE.height);
//#endif
    

    //初始化人数和变量
    resetData();
    _GameTableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate, PLAY_COUNT);

    _fScaleX = WIN_SIZE.width / DESIGN_SIZE.width;
    _fScaleY = WIN_SIZE.height / DESIGN_SIZE.height;

    //触摸
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan=CC_CALLBACK_2(GameTableUI::onTouchBegan,this);
    myListener->setSwallowTouches(false);
    dispatcher->addEventListenerWithSceneGraphPriority(myListener,this);

//    //bg
//    _tableBg = Sprite::create("vip/common/zhuozi.png");
//    addChild(_tableBg);
//    _tableBg->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
//    _tableBg->setScaleX(_fScaleX);
//    _tableBg->setScaleY(_fScaleY);


    createPacketsUI();
    createToolBarUI();

    _GameTableLogic->sendGameInfo();
    
    runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([&] () {
        auto scanner = Scanner::create();
        this->addChild(scanner, 100000);
        scanner->starPlayerEffect();
        scanner->setPosition(60, 200);
    }), nullptr));

    this->setRotation(-90);
	auto pOffset = (WIN_SIZE.height - WIN_SIZE.width) / 2;
	this->setPosition(Vec2(pOffset, pOffset));
    
    return true;
}

    void GameTableUI::createToolBarUI() {
    //    char resStr[128] = {0};
    //
    //    sprintf(resStr, FQZS_TOOL_BAR_PATH, "tool_show_hide.png");
    //    auto toobarBtn1 = Button::create(resStr, resStr);
    //    toobarBtn1->addClickEventListener([=](Ref*) {
    //        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BTN);
    //        _toolbarBg->setName("1");
    //        _toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height)));
    //        this->getChildByTag(em_toolbarBtn)->setVisible(false);
    //    });
    //    toobarBtn1->setAnchorPoint(Vec2(0.5, 1));
    //    toobarBtn1->setPosition(Vec2(101, WIN_SIZE.height - 10));
    //    toobarBtn1->setTag(em_toolbarBtn);
    //    this->addChild(toobarBtn1, 70);
    //    toobarBtn1->setVisible(true);
    //    toobarBtn1->setLocalZOrder(1000);
    //
    //    sprintf(resStr, FQZS_TOOL_BAR_PATH, "tool_bg.png");
    //    _toolbarBg = Sprite::create(resStr);
    //    _toolbarBg->setAnchorPoint(Vec2(0.5, 1));
    //    _toolbarBg->setPosition(_toolbarBg->getContentSize().width /2, WIN_SIZE.height + _toolbarBg->getContentSize().height);
    //    _toolbarBg->setName("0");//0为在上面 1在下面
    //    _toolbarBg->setScaleY(0.8f);
    //    this->addChild(_toolbarBg, 71);     //whao
    //
    //    Size toolbarSize = _toolbarBg->getContentSize();
    //
    //    sprintf(resStr, FQZS_TOOL_BAR_PATH, "tool_setting.png");
    //    auto setPic = Sprite::create(resStr);
    //    setPic->setPosition(toolbarSize.width / 2, toolbarSize.height * 3 / 4);
    //    setPic->setScaleY(1 / 0.8f);
    //    _toolbarBg->addChild(setPic);
    //    auto setBtn = ControlButton::create();
    //    setBtn->setPreferredSize(setPic->getContentSize());
    //    setBtn->setPosition(setPic->getPosition());
    //    setBtn->setTag(em_toolbarSetBtn);
    //    setBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUI::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
    //    _toolbarBg->addChild(setBtn);

        //sprintf(resStr, NN_TOOL_BAR_PATH, "tool_help.png");
        //auto helpPic = Sprite::create(resStr);
        //helpPic->setPosition(toolbarSize.width / 2, toolbarSize.height*3 / 5);
        //_toolbarBg->addChild(helpPic);
        //auto helpBtn = ControlButton::create();
        //helpBtn->setPreferredSize(helpPic->getContentSize());
        //helpBtn->setPosition(helpPic->getPosition());
        //helpBtn->setTag(em_toolbarHelpBtn);
        //helpBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUI::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
        //_toolbarBg->addChild(helpBtn);

    //    sprintf(resStr, FQZS_TOOL_BAR_PATH, "tool_exit.png");
    //    auto leavePic = Sprite::create(resStr);
    //    leavePic->setPosition(toolbarSize.width / 2, toolbarSize.height * 2 / 4);
    //    leavePic->setScaleY(1 / 0.8f);
    //    _toolbarBg->addChild(leavePic);
    //    auto leaveBtn = ControlButton::create();
    //    leaveBtn->setPreferredSize(leavePic->getContentSize());
    //    leaveBtn->setPosition(leavePic->getPosition());
    //    leaveBtn->setTag(em_toolbarLeaveBtn);
    //    leaveBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUI::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
    //    _toolbarBg->addChild(leaveBtn);
    //
    //    sprintf(resStr, FQZS_TOOL_BAR_PATH, "tool_next_out.png");
    //    auto nextRoundOutPic = Sprite::create(resStr);
    //    nextRoundOutPic->setPosition(toolbarSize.width / 2, toolbarSize.height * 1 / 4);
    //    nextRoundOutPic->setScaleY(1 / 0.8f);
    //    _toolbarBg->addChild(nextRoundOutPic);
    //    auto nextRoundOutBtn = ControlButton::create();
    //    nextRoundOutBtn->setPreferredSize(nextRoundOutPic->getContentSize());
    //    nextRoundOutBtn->setPosition(nextRoundOutPic->getPosition());
    //    nextRoundOutBtn->setTag(em_toolbarNextOutBtn);
    //    nextRoundOutBtn->setUserData((void*)false);
    //    nextRoundOutBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUI::onBtnClick), Control::EventType::TOUCH_UP_INSIDE);
    //    _toolbarBg->addChild(nextRoundOutBtn);
    //
    //    sprintf(resStr, FQZS_TOOL_BAR_PATH, "tool_check.png");
    //    auto checkSprite = Sprite::create(resStr);
    //    checkSprite->setPosition(Vec2(25, nextRoundOutPic->getContentSize().height / 2 + 6));
    //    checkSprite->setName("checkSprite");
    //    checkSprite->setVisible(false);
    //    nextRoundOutBtn->addChild(checkSprite);
    }

    void GameTableUI::createPacketsUI() {
        auto bg = ImageView::create("v88/fahongbao/hongbaoqun/bg.png");
        if (bg) {
			bg->setContentSize(WIN_SIZE);
			bg->setScale9Enabled(true);
            bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            bg->setPosition(WIN_SIZE/2);
            addChild(bg);
        }
        m_topBar = ImageView::create("v88/fahongbao/2.png");
        if (m_topBar) {
            m_topBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
            m_topBar->setPosition(Vec2(WIN_SIZE.width/2, WIN_SIZE.height));
            addChild(m_topBar);
        }
        m_toolBtn = Button::create("v88/fahongbao/3.png", "v88/fahongbao/4.png");
        if (m_toolBtn) {
            m_toolBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            m_toolBtn->setPosition(Vec2(0, WIN_SIZE.height));
            m_toolBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUI::toolBarButtonCallBack, this));
            addChild(m_toolBtn);
        }
        m_allPlayersBtn = Button::create("v88/fahongbao/5.png", "v88/fahongbao/6.png");
        if (m_allPlayersBtn) {
            m_allPlayersBtn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            m_allPlayersBtn->setPosition(Vec2(WIN_SIZE.width, WIN_SIZE.height));
            m_allPlayersBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUI::allPlayersButtonCallBack, this));
            addChild(m_allPlayersBtn);
        }
        m_sendPacketBtn = Button::create("v88/fahongbao/hongbaoqun/2.png", "v88/fahongbao/hongbaoqun/3.png");
        if (m_sendPacketBtn) {
            m_sendPacketBtn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            m_sendPacketBtn->setPosition(Vec2(WIN_SIZE.width/2, 0));
            m_sendPacketBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUI::sendPacketButtonCallBack, this));
            addChild(m_sendPacketBtn);
        }
//        m_myPacketHistoryBtn = Button::create("v88/fahongbao/5.png", "v88/fahongbao/6.png");
//        if (m_myPacketHistoryBtn) {
//            m_myPacketHistoryBtn->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
//            m_myPacketHistoryBtn->setPosition(Vec2(WIN_SIZE.width - m_myPacketHistoryBtn->getContentSize().width, WIN_SIZE.height));
//            m_myPacketHistoryBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUI::myPacketHistoryButtonCallBack, this));
//            addChild(m_myPacketHistoryBtn);
//        }
        m_packetCells = ListView::create();
        if (m_packetCells) {
            m_packetCells->setGravity(cocos2d::ui::ListView::Gravity::TOP);
            m_packetCells->setDirection(ListView::Direction::VERTICAL);
            m_packetCells->setContentSize(Size(WIN_SIZE.width, WIN_SIZE.height - 210));
            m_packetCells->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_packetCells->setPosition(WIN_SIZE/2);
            m_packetCells->setScrollBarAutoHideEnabled(false);
            m_packetCells->setBounceEnabled(true);
            addChild(m_packetCells);
            
//            for (int i = 0; i < 55; i++) {
//                auto player = RedPacketGamePlayerCellLayer::create();
//                if (player) {
//                    player->setPacketUIByMy(i%2==0);
//                    player->setParentUI(this);
//                    m_packetCells->pushBackCustomItem(player);
//                }
//            }
        }
        m_title = Text::create("", FONT_TTF_MSYH, 35);
        if (m_title) {
            m_title->setTextAreaSize(Size(WIN_SIZE.width - 200, 100));
            m_title->setTextVerticalAlignment(TextVAlignment::CENTER);
            m_title->setTextHorizontalAlignment(TextHAlignment::LEFT);
            m_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
            if (m_toolBtn) {
                m_title->setPosition(Vec2(m_toolBtn->getPositionX()+m_toolBtn->getContentSize().width, m_toolBtn->getPositionY()-m_toolBtn->getContentSize().height/2));
            }
//            ComRoomInfo* roomInfo = RoomLogic()->getSelectedRoom();
//            char name[128];
//            memset(name, 0, sizeof(name));
//            sprintf(name, GetText("PACKET_TITLE"), ForceGBKToUtf8(roomInfo->szGameRoomName));
//            m_title->setString(name);
            addChild(m_title);
        }
//        m_packetsListLayer = RedPacketGamePacketsListLayer::create();
//        if (m_packetsListLayer) {
//            m_packetsListLayer->setPosition(WIN_SIZE/2);
//            m_packetsListLayer->setBackButtonCallBack([&]() {
//                leaveTheGame();
//            });
//            addChild(m_packetsListLayer);
//        }
        if (_GameTableLogic) {
            _GameTableLogic->sendRoomBaseData();
            _GameTableLogic->sendRequestPacketList();
        }
    }

    void GameTableUI::leaveTheGame() {
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//		ChangeRootViewController::changeRootViewControllerH();
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//		WeChatAndroid::getInstance()->changeScreenDirection(0/*, WINSIZE.width, WINSIZE.height*/);
//#endif
        
        HNRoomLogic::getInstance()->closeRoom();
        PlatformState* state = PlatformState::create();
        state->setType(ROOMLIST);
        StateManager::getInstance()->setState(state);
    }
    
    //新红包通知
    void GameTableUI::onNotifyNewPacketResponse(RedPacket::tag_s2c_RedPacketListReq *packetRes) {
        if (packetRes == nullptr) {
            return;
        }

        auto player = RedPacketGamePlayerCellLayer::create();
        if (player) {
            player->setPacketUIByMy(packetRes->nUserID == _GameTableLogic->getUserId(_GameTableLogic->getMySeatNo()));
            player->setParentUI(this);
            UserInfoStruct user;
            user.dwUserID = packetRes->nUserID;
            memcpy(user.nickName, packetRes->szUserName, sizeof(packetRes->szUserName));
            user.isVirtual = packetRes->nIsRobot;
            user.bLogoID = packetRes->nLogoID;
            memcpy(user.szWXTouXiang, packetRes->szWXTouXiang, sizeof(packetRes->szWXTouXiang));
            player->setPacketData(packetRes, &user);
            player->setOpenPacketCallBack([=](unsigned int packetID) {
                _GameTableLogic->sendOpenRedPacketMessage(packetID);
            });
            player->setpacketDetailCallBack([=](unsigned int packetID) {
                _GameTableLogic->sendOpenRedPacketMessage(packetID);
            });
            player->setRequestMyInfoCallBack([=](unsigned int packetID) {
                m_lastRequestMoneyByPacket = packetID;
                _GameTableLogic->sendRequestMyInfo();
            });
            player->setName(to_string(packetRes->nRedPacketID));
            m_packetCells->pushBackCustomItem(player);
            m_packetCells->jumpToBottom();
        }
    }
    
    void GameTableUI::onSendPacketResponse(RedPacket::tag_c2s_EmitRedPacketReq *packetRes) {
        if (packetRes) {
            //发红包结果:	0-成功 1-金币不足 2-红包的份数不满足规则 3-雷的数字设定不满足规则 4-桌面红包数量过多
            int result = packetRes->nResult;
            if (result == 0) {
                //红包发送成功
                
            }
            else {
                char str[32];
                memset(str, 0, sizeof(str));
                sprintf(str, "SEND_PACKET_ERROR_%d", result);
                
                auto prompt = GamePromptLayer::create();
                if (prompt) {
                    prompt->showPrompt(GetText(str));
                    prompt->setColorLayerTouchEnabled(false);
                }
            }
        }
    }

    void GameTableUI::onOpenPacketResponse(RedPacket::tag_c2s_RobRedPacketReq *packetRes) {
        if (packetRes) {
            //自己拆红包的数据存到对应的红包类里面
            if (m_packetCells) {
                RedPacketGamePlayerCellLayer* cell = (RedPacketGamePlayerCellLayer*)m_packetCells->getChildByName(to_string(packetRes->nRedPacketID));
                if (cell) {
                    cell->setMyPacketRobInfo(packetRes);
                }
            }
            //
            auto detailLayer = RedPacketOpenDetailsLayer::create();
            if (detailLayer) {
                detailLayer->setName("Open_Detail");
                detailLayer->openAddToParent(this, WIN_SIZE/2);
                detailLayer->setPacketInfo(packetRes);
                detailLayer->setMyHistoryBtnCallBack([=]() {
                    if (_GameTableLogic) {
                        _GameTableLogic->sendMyPacketHistoryMessage();
                    }
                    auto myHistory = RedPacketMyRecordLayer::create();
                    if (myHistory) {
                        myHistory->setName("My_History");
                        myHistory->setSenderHead(_GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo()));
                        myHistory->openAddToParent(this, WIN_SIZE/2);
                    }
                });
                _GameTableLogic->sendPacketOpenDetailsMessage(packetRes->nRedPacketID);
            }
        }
    }
    
//    void GameTableUI::onNotifyPlayerOpenPacketResponse(RedPacket::RobRedPacketUserInfo *packetRes) {
//        if (packetRes) {
////            packetRes->i64RobMoney;
//        }
//    }
    
    //红包详情基础信息通知更新到UI
    void GameTableUI::onNotifyPacketOpenDetailsResponse(RedPacket::tag_c2s_RedPacketRobLogReq *packetRes) {
        if (packetRes) {
            RedPacketOpenDetailsLayer* detailLayer = (RedPacketOpenDetailsLayer* )this->getChildByName("Open_Detail");
//            if (detailLayer == nullptr) {
//                auto detailLayer = RedPacketOpenDetailsLayer::create();
//                if (detailLayer) {
//                    detailLayer->setName("Open_Detail");
//                    detailLayer->openAddToParent(this, WIN_SIZE/2);
//                    //                    detailLayer->setPacketInfo(packetRes);
//                    detailLayer->setMyHistoryBtnCallBack([=]() {
//                        if (_GameTableLogic) {
//                            _GameTableLogic->sendMyPacketHistoryMessage();
//                        }
//                        auto myHistory = RedPacketMyRecordLayer::create();
//                        if (myHistory) {
//                            myHistory->setName("My_History");
//                            myHistory->setSenderHead(_GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo()));
//                            myHistory->openAddToParent(this, WIN_SIZE/2);
//                        }
//                    });
////                    _GameTableLogic->sendPacketOpenDetailsMessage(packetRes->nRedPacketID);
//                }
//            }
            if (detailLayer) {
                detailLayer->setOpenPacketDetailInfo(packetRes);
            }
        }
    }
    
    //红包详情界面领取红包的所有玩家消息更新到UI
    void GameTableUI::onNotifyPacketOpenPlayerDetailResponse(RobRedPacketUserInfo* packetRes) {
        if (packetRes) {
            RedPacketOpenDetailsLayer* detailLayer = (RedPacketOpenDetailsLayer* )this->getChildByName("Open_Detail");
            if (detailLayer) {
                detailLayer->setOpenPacketPlayerDetail(packetRes);
            }
//            else {
//                auto detailLayer = RedPacketOpenDetailsLayer::create();
//                if (detailLayer) {
//                    detailLayer->setName("Open_Detail");
//                    detailLayer->openAddToParent(this, WIN_SIZE/2);
//                    //                    detailLayer->setPacketInfo(packetRes);
//                    detailLayer->setMyHistoryBtnCallBack([=]() {
//                        if (_GameTableLogic) {
//                            _GameTableLogic->sendMyPacketHistoryMessage();
//                        }
//                        auto myHistory = RedPacketMyRecordLayer::create();
//                        if (myHistory) {
//                            myHistory->setName("My_History");
//                            myHistory->setSenderHead(_GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo()));
//                            myHistory->openAddToParent(this, WIN_SIZE/2);
//                        }
//                    });
//                    _GameTableLogic->sendPacketOpenDetailsMessage(packetRes->nRedPacketID);
//                }
//            }
        }
    }

    void GameTableUI::onGetMyPacketHistiryResponse(RedPacket::tag_s2c_MyHistoryRobLogReq *packetRes) {
        if (packetRes) {
            RedPacketMyRecordLayer* historyLayer = (RedPacketMyRecordLayer*)this->getChildByName("My_History");
            if (historyLayer) {
                auto cell = RedPacketMyRecordCellLayer::create();
                if (cell) {
                    cell->setHistoryCellData(packetRes);
                    historyLayer->addRobMoney(packetRes->i64GainMoney);
                    historyLayer->addHistoryCell(cell);
                }
                
            }
        }
    }
    
    //更新所有玩家列表
    void GameTableUI::onUpdateAllPlayerListResponse(std::vector<tag_s2c_PlayerListMsgReq> allPlayer) {
        RedPacketAllPlayersListLayer* allLayer = (RedPacketAllPlayersListLayer*)this->getChildByName("All_Player_List");
        for (int i = 0; i < allPlayer.size(); i++) {
            tag_s2c_PlayerListMsgReq* packet = &allPlayer.at(i);
            if (packet) {
                if (allLayer) {
                    auto cell = RedPacketAllPlayerCellLayer::create();
                    if (cell) {
                        UserInfoStruct user;
                        user.dwUserID = packet->nUserID;
                        memcpy(user.nickName, packet->szUserName, sizeof(packet->szUserName));
                        user.isVirtual = packet->nIsRobot;
                        user.bLogoID = packet->nLogoID;
                        memcpy(user.szWXTouXiang, packet->szWXTouXiang, sizeof(packet->szWXTouXiang));
                        cell->setHead(&user);
                        allLayer->addCellIntoList(cell);
                    }
                }
            }
        }
        if (allLayer) {
            allLayer->updateAllPlayers();
        }
    }
    
    //红包领取完或者红包过期
    void GameTableUI::onNotityPacketExpiredMessage(RedPacket::tag_s2c_RobRedPacketOverReq *packetRes) {
        if (packetRes) {
            if (packetRes->nOverType == 1) {
                //红包领取完
            }
            else if (packetRes->nOverType == 2) {
                //红包过期
                if (m_packetCells) {
                    RedPacketGamePlayerCellLayer* cell = (RedPacketGamePlayerCellLayer*)m_packetCells->getChildByName(to_string(packetRes->nRedPacketID));
                    if (cell) {
                        m_packetCells->removeChildByName(to_string(packetRes->nRedPacketID));
                    }
                    m_packetCells->requestDoLayout();
                    m_packetCells->jumpToBottom();
                }
            }
        }
    }
    
    //更新身上的金币
    void GameTableUI::onUpdateMyPacketMoney(LLONG money) {
        if (m_packetCells && m_lastRequestMoneyByPacket > 0) {
            RedPacketGamePlayerCellLayer* cell = (RedPacketGamePlayerCellLayer*)m_packetCells->getChildByName(to_string(m_lastRequestMoneyByPacket));
            if (cell) {
                auto myUser = _GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo());
                cell->updateMyWalletMoney(myUser->nickName, money);
            }
        }
    }
    
    //更新房间基础信息展示
    void GameTableUI::onUpdateRoomBaseInfo() {
        if (m_title && _GameTableLogic) {
            tag_s2c_BaseConfigMsgReq info = _GameTableLogic->getRoomBaseData();
            char name[128];
            memset(name, 0, sizeof(name));
            sprintf(name, GetText("PACKET_TITLE"), info.i64MinMoney/10000, info.i64MaxMoney/10000, info.nCanSplitCount, info.nLeiRate);
            m_title->setString(name);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    void GameTableUI::onBtnClick(Ref* pSender,Control::EventType event) {
        
    }

    bool GameTableUI::onTouchBegan(Touch* touch, Event* event) {
    //    Point point = touch->getLocation();
    //    point = _tableBg->convertToNodeSpace(point);
    //
    //    if(0 == _toolbarBg->getName().compare("1")) {
    //        _toolbarBg->setName("0");
    //
    //        _toolbarBg->runAction(Sequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
    //        CallFunc::create([this]() {
    //            this->getChildByTag(em_toolbarBtn)->setVisible(true);
    //        })));
    //    }

        return true;
    }

    void GameTableUI::onEnter() {
        HNGameUIBase::onEnter();
    }

    void GameTableUI::onExit() {
        HNGameUIBase::onExit();
    }

    void GameTableUI::onExitTransitionDidStart() {
        HNGameUIBase::onExitTransitionDidStart();
        HNAudioEngine::getInstance()->stopBackgroundMusic();
    }

    void GameTableUI::addUser(BYTE seatNo, bool bMe) {
       
    }


    void GameTableUI::setBtnTouch(Button* btn, bool bTouch) {

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

    void GameTableUI::tipCallback() {
//        if (tipMsg.bIsExit == 1) {
//            //准备关闭掉
//            //m_PrepareBtn->setVisible(false);
//
//            auto layer = GamePromptLayer::create();
//    //        layer->setCloseExitBtn();
//    //         char str[128] = { 0 };
//    //         int sec = 3;
//    //         sprintf(str, "金币数额低于%lld，%ds后离开游戏！", tipMsg.i64Money, sec);
//    //         layer->showPrompt(GBKToUtf8(str));
//            int sec = 3;
//            char s[128] {0};
//            std::string str;
//            str.append(GBKToUtf8("金币数额低于"));
//            sprintf(s, "%lld", tipMsg.i64Money);
//            str.append(s);
//            sprintf(s, ",%d", sec);
//            str.append(s);
//            str.append(GBKToUtf8("s后离开游戏！"));
//            layer->showPrompt(str);
//
//            layer->setOkayCallBack([&]() {
//                unschedule(schedule_selector(GameTableUI::updateTip));
//                _GameTableLogic->sendUserUp();
//            });
//            layer->setColorLayerTouchEnabled(false);
//
//            m_i64TipMoney = tipMsg.i64Money;
//            m_TextTip = layer->getPromptText();
//            m_TextTip->setTag(sec);
//            schedule(schedule_selector(GameTableUI::updateTip), 1.0f);
//        } else {
//
//        }
    }

    void GameTableUI::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
        if (bVisible) {
            if (!m_Notice) {
                m_Notice = GameNotice::create();
                m_Notice->setLocalZOrder(NOTICE_ZORDER);
                m_Notice->setName("GameNotice");
            }
            m_Notice->openNotic(this, 10000, message, nCount, nType);
        }
    }

    //玩家坐下(非自己)
    void GameTableUI::userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
        addUser(userSit->bDeskStation, userSit->bDeskStation == _GameTableLogic->getMySeatNo());
    }

    //玩家站起
    void GameTableUI::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
        
    }
        

    void GameTableUI::onGameTipExit(tag_s2c_tip_exit* msg) {
//        tipMsg.bIsExit = msg->bIsExit;
//        tipMsg.i64Money = msg->i64Money;

//        //不退出
//        if (tipMsg.bIsExit == 0) {
//            //下一局离开
////            if (m_bNextOut) {
////                m_canPrepareForNew = false;
////                this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
////                    _GameTableLogic->sendUserUp();
////                })));
////            } else {
//////                m_canPrepareForNew = true;
////                //m_PrepareBtn->setVisible(false && m_canPrepareForNew);
////                //m_PrepareBtn->setTouchEnabled(false && m_canPrepareForNew);
////            }
//            return;
//        } else {
//    //        tipMsg->bIsExit;
//        }
//
//        //要退出
//        this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f), CallFunc::create([&]() {
//            tipCallback();
//        })));
    }



    void GameTableUI::onHandleNewMessage(int iType, const std::string &message, int iCount)
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


    void GameTableUI::resetNewGame() {
       
    }

    void GameTableUI::resetUINewGame() {

    }

    void GameTableUI::resetData() {
        //ui
        _fScaleX = 0;
        _fScaleY = 0;
        _tableSize = Size(0, 0);
        _toolbarBg = nullptr;  
        m_Notice = nullptr;
        m_packetsListLayer = nullptr;
        m_topBar = nullptr;
        m_toolBtn = nullptr;
        m_allPlayersBtn = nullptr;
        m_sendPacketBtn = nullptr;
        m_title = nullptr;
        m_packetCells = nullptr;        
        m_lastRequestMoneyByPacket = 0;
        
    }
    
    void GameTableUI::toolBarButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        if (!RoomLogic()->connected()) {
            leaveTheGame();
        }
        else {
            _GameTableLogic->sendUserUp();
        }
    }
    
    void GameTableUI::allPlayersButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        if (_GameTableLogic) {
            _GameTableLogic->sendRequestAllPlayerList();
        }
        auto allPlayers = RedPacketAllPlayersListLayer::create();
        if (allPlayers) {
            allPlayers->setName("All_Player_List");
            allPlayers->openAddToParent(this, WIN_SIZE/2);
//            allPlayers->updateAllPlayers();
        }
    }
    
    void GameTableUI::sendPacketButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        auto sendLayer = RedPacketSendPacketLayer::create();
        if (sendLayer) {
            sendLayer->openAddToParent(this, WIN_SIZE/2);
            sendLayer->setSendPacketCallBack([&](LLONG packetMoney, int packetPart, int thunderNumber) {
                if (_GameTableLogic) {
                    _GameTableLogic->sendRedPacketMessage(packetMoney, packetPart, thunderNumber);
                }
            });
            if (_GameTableLogic) {
                sendLayer->updateRoomBaseData(_GameTableLogic->getRoomBaseData());
            }
        }
    }
    
//    void GameTableUI::myPacketHistoryButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
//        if (type != Widget::TouchEventType::ENDED) {
//            return;
//        }
//        if (_GameTableLogic) {
//            _GameTableLogic->sendMyPacketHistoryMessage();
//        }
//        auto myHistory = RedPacketMyRecordLayer::create();
//        if (myHistory) {
//            myHistory->setName("My_History");
//            myHistory->setSenderHead(_GameTableLogic->getUserBySeatNo(_GameTableLogic->getMySeatNo()));
//            myHistory->openAddToParent(this, WIN_SIZE/2);
//        }
////        auto allPlayers = RedPacketAllPlayersListLayer::create();
////        if (allPlayers) {
////            allPlayers->openAddToParent(this, WIN_SIZE/2);
////            allPlayers->updateAllPlayers();
////        }
//    }


}
