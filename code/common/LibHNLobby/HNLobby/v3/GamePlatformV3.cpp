#include "GamePlatformV3.h"
#include "../PlatformConfig.h"
#include "../pattern/CommandManager.h"
#include "../GamePlatform.h"
#include "../GamePrompt.h"


#include "../ChildLayer/GameSetLayer.h"
#include "../ChildLayer/ServiceLayer.h"
#include "../PersionalCenter/GameUserDataLayer.h"
#include "../PersionalCenter/BindPhone.h"
#include "../PersionalCenter/ModifyPassword.h"
#include "../ChildLayer/GameExitChangeLayer.h"
#include "../ChildLayer/GameSignLayer.h"
#include "../ChildLayer/GameRankingList.h"
#include "../ChildLayer/GameBankLayer.h"
#include "../ChildLayer/GameStoreLayer.h"
#include "../ChildLayer/GameOnlineReward.h"
#include "../ChildLayer/GameRechargeRebate.h"
#include "../GameExitLayer.h"
#include "../Popularise/SpreadEditLayer.h"
#include "../CommonMacro.h"
#include "../pattern/factory/gameList/GameListFactory.h"
#include "../pattern/factory/roomList/RoomListFactory.h"
#include "../pattern/factory/deskList/DeskListFactory.h"
#include "../pattern/factory/menu/TopMenuFactory.h"
#include "../pattern/factory/menu/BottomMenuFactory.h"
#include "../pattern/factory/gameType/GameTypeFactory.h"
#include "../GameNotice.h"

bool GamePlatformV3::init() {
    if (!GamePlatform::init()) {
        return false;
    }
    return true;
}

void GamePlatformV3::initPlatform() {
    m_roomListDelegate = nullptr;
    m_bottomMenuDelegate = nullptr;
    m_topMenuDelegate = nullptr;
    m_gameListDelegate = nullptr;

    // 背景图
    setBackGroundImage(PLATFORM_BG);

    auto csb = CSLoader::createNode(BG_ANIMATION_CSB);
    csb->setPosition(Vec2(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.5f));
    addChild(csb);
    auto antAction = CSLoader::createTimeline(BG_ANIMATION_CSB);
    csb->runAction(antAction);
    antAction->play("guangban", true);


    float _xScale = WIN_SIZE.width /PlatformConfig::getInstance()->getPlatformDesignSize().width;
    float _yScale = WIN_SIZE.height / PlatformConfig::getInstance()->getPlatformDesignSize().height;


    auto topMenuComposite = TopMenuFactory::createWithVersion();
    topMenuComposite->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height));
    addChild(topMenuComposite);

    auto bottomMenuComposite = BottomMenuFactory::createWithVersion();
    bottomMenuComposite->setPosition(Vec2(WIN_SIZE.width / 2, 0));
    addChild(bottomMenuComposite,9999999);

    m_topMenuDelegate = topMenuComposite;
    m_bottomMenuDelegate = bottomMenuComposite;
}

void GamePlatformV3::updateGameList(bool success, const std::string &message) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    if (success) {
        //跳过
        if (UserDefault::getInstance()->getBoolForKey("FIRST_LOGIN")) {
            UserDefault::getInstance()->setBoolForKey("FIRST_LOGIN", false);
        } else {
            createGameListLayer();
        }
    } else {
        GamePromptLayer::create()->showPrompt(GBKToUtf8(message.c_str()));
    }
}

void GamePlatformV3::updateGameUserCount(UINT Id, UINT count) {
    if (m_gameListDelegate) {
        m_gameListDelegate->updateGameUserCount(Id, count);
    }
}

void GamePlatformV3::updateRoomList(bool success, const std::string& message) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    if (success) {
        createRoomListLayer();
    } else {
        GamePromptLayer::create()->showPrompt(GBKToUtf8(message.c_str()));
    }
}

void GamePlatformV3::updateRoomUserCount(UINT roomID, UINT userCount) {
    if (m_roomListDelegate) {
        m_roomListDelegate->updateRoomUserCount(roomID, userCount);
    }
}

void GamePlatformV3::onSpreadInfo(bool result, bool bInfo, const std::string& msg) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    if (result) {
        if (bInfo) { //有推广信息，不一定就是推广员
            auto layer = SpreadEditLayer::create(); //显示资料界面
            layer->show(this, PLATFORM_UI_ZORDER_POPULARISE,
                        PLATFORM_UI_ZORDER_POPULARISE);
        } else { //肯定不是推广员，直接申请
            auto layer = SpreadEditLayer::create(); //申请推广员界面
            layer->show(this, PLATFORM_UI_ZORDER_POPULARISE,
                        PLATFORM_UI_ZORDER_POPULARISE);
        }
    } else {
        GamePromptLayer::create()->showPrompt(msg);
    }
}

void GamePlatformV3::createGameListAndRoom() {
    auto gameListComposite = GameListFactory::createWithVersion();
    addChild(gameListComposite);
    gameListComposite->setGameListLogic(_gameListLogic);
    gameListComposite->setRoomListLogic(_roomListLogic);
    gameListComposite->setTopMenuDelegate(m_topMenuDelegate);
    gameListComposite->setBottomMenuDelegate(m_bottomMenuDelegate);

    m_gameListDelegate = gameListComposite;
}

void GamePlatformV3::createRoomListLayer() {
    m_gameListDelegate = nullptr;

    auto roomListComposite = RoomListFactory::createWithVersion();
    addChild(roomListComposite);
    roomListComposite->setName("gameRoomLayer");
    roomListComposite->setTopMenuDelegate(m_topMenuDelegate);
    roomListComposite->setBottomMenuDelegate(m_bottomMenuDelegate);
    roomListComposite->setPlatformDelegate(this);
    roomListComposite->setRoomListLogic(_roomListLogic);
    roomListComposite->setGameListLogic(_gameListLogic);
    roomListComposite->execute();
    m_roomListDelegate = roomListComposite;

    m_topMenuDelegate->setRoomListDelegate(roomListComposite);

    showTopAction(false, nullptr);
    showBottomAction(false, nullptr);
}

void GamePlatformV3::createGameListLayer() {
    showBottomAction(false, nullptr);
    showTopAction(false, nullptr);


    auto gameTypeComposite = GameTypeFactory::createWithVersion();
    gameTypeComposite->setPlatformDelegate(this);
    addChild(gameTypeComposite);



}

void GamePlatformV3::createDeskListLayer(ComRoomInfo *roomInfo) {
    m_roomListDelegate = nullptr;
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);

    IDeskList* deskListComposite = DeskListFactory::createWithVersion(roomInfo);
    addChild(deskListComposite);
    deskListComposite->setName("gameDeskLayer");
    deskListComposite->setTopMenuDelegate(m_topMenuDelegate);
    deskListComposite->setBottomMenuDelegate(m_bottomMenuDelegate);
    deskListComposite->setRoomListLogic(_roomListLogic);
    deskListComposite->setGameListLogic(_gameListLogic);
    //deskListComposite->setBacksTageOpen(_backsTageOpen);
    deskListComposite->execute();

    m_topMenuDelegate->setDeskListDelegate(deskListComposite);
}

void GamePlatformV3::showTopAction(bool isOut, CallFunc* func) {
    m_topMenuDelegate->moveOut(isOut);
}

void GamePlatformV3::showBottomAction(bool isOut, CallFunc* func) {
    m_bottomMenuDelegate->moveOut(isOut);
}

void GamePlatformV3::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *pEvent) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (EventKeyboard::KeyCode::KEY_F1 == keyCode)
#endif
    {
        do {
            ServiceLayer *serviceLayer = dynamic_cast<ServiceLayer *>(
                                             this->getChildByTag(CHILD_LAYER_SERVICE_TAG));
            if (nullptr != serviceLayer) {
                serviceLayer->closeService();
                break;
            }

            GameSetLayer *setLayer = dynamic_cast<GameSetLayer *>(
                                         this->getChildByTag(CHILD_LAYER_SET_TAG));
            if (nullptr != setLayer) {
                setLayer->closeSet();
                break;
            }

            GameUserDataLayer *userDataLayer = dynamic_cast<GameUserDataLayer *>(
                                                   this->getChildByTag(CHILD_LAYER_USERHEAD_TAG));
            if (nullptr != userDataLayer) {
                userDataLayer->closeUserLayer();
                break;
            }

            BindPhoneLayer *bindPhone = dynamic_cast<BindPhoneLayer *>(
                                            this->getChildByTag(CHILD_LAYER_PHONE_TAG));
            if (nullptr != bindPhone) {
                bindPhone->close();
                break;
            }

            ModifyPasswordLayer *modifyPass = dynamic_cast<ModifyPasswordLayer *>(
                                                  this->getChildByTag(CHILD_LAYER_PASS_TAG));
            if (nullptr != modifyPass) {
                modifyPass->close();
                break;
            }

            GameExitChangeLayer *exitChange = dynamic_cast<GameExitChangeLayer *>(
                                                  this->getChildByTag(CHILD_LAYER_EXITCHANGE_TAG));
            if (nullptr != exitChange) {
                exitChange->closeExitChoose();
                break;
            }

            GameSignLayer *gameSign = dynamic_cast<GameSignLayer *>(
                                          this->getChildByTag(CHILD_LAYER_SIGN_TAG));
            if (nullptr != gameSign) {
                gameSign->closeSign();
                break;
            }

            GameRankingList *gameRank = dynamic_cast<GameRankingList *>(
                                            this->getChildByTag(CHILD_LAYER_RANK_TAG));
            if (nullptr != gameRank) {
                gameRank->closeRanking();
                break;
            }

            GameBankLayer *bankLayer = dynamic_cast<GameBankLayer *>(
                                           this->getChildByTag(CHILD_LAYER_BANK_TAG));
            if (nullptr != bankLayer) {
                bankLayer->closeBank();
                break;
            }

            GameStoreLayer *storeLayer = dynamic_cast<GameStoreLayer *>(
                                             this->getChildByTag(CHILD_LAYER_STORE_TAG));
            if (nullptr != storeLayer) {
                storeLayer->closeStore();
                break;
            }

            GameOnlineReward *OnlineReward = dynamic_cast<GameOnlineReward *>(
                                                 this->getChildByTag(CHILD_LAYER_ONLINE_TAG));
            if (nullptr != OnlineReward) {
                OnlineReward->closeOnlineReward();
                break;
            }

            GameRechargeRebate *rebateLayer = dynamic_cast<GameRechargeRebate *>(
                                                  this->getChildByTag(CHILD_LAYER_REBATE_TAG));
            if (nullptr != rebateLayer) {
                rebateLayer->closeRebate();
                break;
            }

            // 调用离开游戏层
            GameExitLayer *exitLayer = dynamic_cast<GameExitLayer *>(
                                           this->getChildByTag(CHILD_LAYER_EXIT_TAG));
            if (nullptr == exitLayer) {
                auto exitLayer = GameExitLayer::create();
                exitLayer->setPosition(Size::ZERO);
                addChild(exitLayer, 100000000, CHILD_LAYER_EXIT_TAG);
            } else {
                exitLayer->callBackNo();
            }
        } while (0);
    }
}

void GamePlatformV3::createGameTypeLayer() {
    auto gameTypeComposite = GameTypeFactory::createWithVersion();
    gameTypeComposite->setPlatformDelegate(this);
    addChild(gameTypeComposite);
}

void GamePlatformV3::openNewYearActivity() {
    //m_bottomMenuDelegate->moveOut(true);
    //m_topMenuDelegate->moveOut(true);
    
    //m_topMenuDelegate->autoMove();
    //m_bottomMenuDelegate->autoMove();
    
    m_topMenuDelegate->setType(room_type);
    
    _gameListLogic->stop();
    
    _roomListLogic->start();
    PlatformLogic()->requestRoomListInfo(1, 11118888);
    
    CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("加载中..."));
}

void GamePlatformV3::setLayerType(LayerType layerType) {
    GamePlatform::setLayerType(layerType);
//    if (m_type == GAMETYPE) {
//        if (UserDefault::getInstance()->getBoolForKey("FIRST_LOGIN")) {
//            _gameListLogic->requestGameList();
//        }
//    }
}
