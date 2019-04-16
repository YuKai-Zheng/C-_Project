#include "GamePlatformV2.h"

#include "../PlatformConfig.h"
#include "../pattern/CommandManager.h"
#include "../GamePrompt.h"

#include "../ChildLayer/GameSetLayer.h"

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
#include "VersionInfo/VersionInfo.h"
//#include "../v3/PlatformResV3.h"

bool GamePlatformV2::init() {
    if (!GamePlatform::init()) {
        return false;
    }
    return true;
}

void GamePlatformV2::initPlatform() {
    m_roomListDelegate = nullptr;
    m_bottomMenuDelegate = nullptr;
    m_topMenuDelegate = nullptr;
    m_gameListDelegate = nullptr;

// 背景图
    if (VERSION_TWO) {
        setBackGroundImage(PLATFORM_BG);
    }
//    else if (VERSION_THREE) {
//        setBackGroundImage(PLATFORM_BG_V3);
//    }


    auto csb = CSLoader::createNode(BG_ANIMATION_CSB);
    csb->setPosition(Vec2(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.5f));
    addChild(csb);
    auto antAction = CSLoader::createTimeline(BG_ANIMATION_CSB);
    csb->runAction(antAction);
    antAction->play("guangban", true);

    //float _xScale = WIN_SIZE.width /PlatformConfig::getInstance()->getPlatformDesignSize().width;
    //float _yScale = WIN_SIZE.height / PlatformConfig::getInstance()->getPlatformDesignSize().height;

    auto topMenuComposite = TopMenuFactory::createWithVersion();
    topMenuComposite->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height));
    topMenuComposite->setPlatformDelegate(this);
    addChild(topMenuComposite);

    auto bottomMenuComposite = BottomMenuFactory::createWithVersion();
    bottomMenuComposite->setPosition(Vec2(WIN_SIZE.width / 2, 0));
    addChild(bottomMenuComposite);

    m_topMenuDelegate = topMenuComposite;
    m_bottomMenuDelegate = bottomMenuComposite;
}

void GamePlatformV2::updateGameList(bool success, const std::string &message) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    if (success) {
        //跳过
        if (UserDefault::getInstance()->getBoolForKey("FIRST_LOGIN")) {
            //_gameListLogic->stop();
            UserDefault::getInstance()->setBoolForKey("FIRST_LOGIN", false);
        } else {
            createGameListLayer();
        }
    } else {
        GamePromptLayer::create()->showPrompt(GBKToUtf8(message.c_str()));
    }
}

void GamePlatformV2::updateGameUserCount(UINT Id, UINT count) {
    if (m_gameListDelegate) {
        m_gameListDelegate->updateGameUserCount(Id, count);
    }
}

void GamePlatformV2::updateRoomList(bool success, const std::string& message) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    if (success) {
        //if (UserDefault::getInstance()->getBoolForKey("NEW_YEAR")) {
        //    _gameListLogic->stop();
        //    UserDefault::getInstance()->setBoolForKey("NEW_YEAR",false);
        //} else {
        createRoomListLayer();
        //}
    } else {
        GamePromptLayer::create()->showPrompt(GBKToUtf8(message.c_str()));
    }
}

void GamePlatformV2::updateRoomUserCount(UINT roomID, UINT userCount) {
    if (m_roomListDelegate) {
        m_roomListDelegate->updateRoomUserCount(roomID, userCount);
    }
}

void GamePlatformV2::onSpreadInfo(bool result, bool bInfo, const std::string& msg) {
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

void GamePlatformV2::createGameListAndRoom() {
    auto gameListComposite = GameListFactory::createWithVersion();
    addChild(gameListComposite);
    gameListComposite->setGameListLogic(_gameListLogic);
    gameListComposite->setRoomListLogic(_roomListLogic);
    gameListComposite->setTopMenuDelegate(m_topMenuDelegate);
    gameListComposite->setBottomMenuDelegate(m_bottomMenuDelegate);

    m_gameListDelegate = gameListComposite;
}

void GamePlatformV2::handleReturnButton() {
    CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));

    if (m_type == DESKLIST) {
        if (m_deskListDelegate) {
            m_deskListDelegate->closeDeskList();
        }
        if (GameNotice::getNotic(this) != nullptr) {
            GameNotice::getNotic(this)->actionIn(0.6f);
        }
    }

    if (m_type == ROOMLIST) {
        if (m_roomListDelegate) {
            m_roomListDelegate->closeRoomList();
        }
    }

    if (m_type == GAMELIST) {
        if (m_gameListDelegate) {
            //_gameListLogic->stop();
            m_gameListDelegate->closeGameList();
        }
    }
}

void GamePlatformV2::openNewYearActivity() {
    _gameListLogic->stop();
    _roomListLogic->start();
    PlatformLogic()->requestRoomListInfo(1, 11118888);
    GameCreator()->setCurrentGame(11118888);
    //UserDefault::getInstance()->setBoolForKey("NEW_YEAR", true);

    CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("加载中..."));
}

void GamePlatformV2::setLayerType(LayerType layerType) {
    GamePlatform::setLayerType(layerType);
}

void GamePlatformV2::createRoomListLayer() {
    m_gameListDelegate = nullptr;
    m_deskListDelegate = nullptr;

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




    //showTopAction(false);
    //showBottomAction(false);
}

void GamePlatformV2::createGameListLayer() {
    //showBottomAction(false);
    //showTopAction(false);

    m_roomListDelegate = nullptr;

    auto gameListComposite = GameListFactory::createWithVersion();
    gameListComposite->setPlatformDelegate(this);
    gameListComposite->setGameListLogic(_gameListLogic);
    addChild(gameListComposite);

    m_gameListDelegate = gameListComposite;
}

void GamePlatformV2::createGameTypeLayer() {
    //showBottomAction(false);
    //showTopAction(false);
    //m_topMenuDelegate->autoMove();
    m_gameListDelegate = nullptr;
    m_roomListDelegate = nullptr;
    m_deskListDelegate = nullptr;

    auto gameTypeComposite = GameTypeFactory::createWithVersion();
    gameTypeComposite->setPlatformDelegate(this);
    addChild(gameTypeComposite);
}

void GamePlatformV2::createDeskListLayer(ComRoomInfo *roomInfo) {
    m_roomListDelegate = nullptr;

    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);

    IDeskList* deskListComposite = DeskListFactory::createWithVersion(roomInfo);
    addChild(deskListComposite);
    deskListComposite->setName("gameDeskLayer");
    deskListComposite->setTopMenuDelegate(m_topMenuDelegate);
    deskListComposite->setBottomMenuDelegate(m_bottomMenuDelegate);
    deskListComposite->setRoomListLogic(_roomListLogic);
    deskListComposite->setGameListLogic(_gameListLogic);
    deskListComposite->setPlatformDelegate(this);
    deskListComposite->execute();

    m_deskListDelegate = deskListComposite;

    m_topMenuDelegate->setDeskListDelegate(deskListComposite);
}

void GamePlatformV2::showTopAction(bool isOut) {
    //m_topMenuDelegate->moveOut(isOut);
}

void GamePlatformV2::showBottomAction(bool isOut) {
    m_bottomMenuDelegate->moveOut(isOut);
}

