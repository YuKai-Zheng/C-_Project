#include "GamePlatformV88.h"
#include "../PlatformConfig.h"
#include "../pattern/CommandManager.h"
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
#include "../pattern/factory/gameRank/GameRankFactory.h"
#include "../GameNotice.h"
#include "PlatformResV88.h"
#include "GameRankV88.h"

bool GamePlatformV88::init() {
    if (!GamePlatformOne::init()) {
        return false;
    }

    return true;
}

void GamePlatformV88::handleReturnButton() {
    if (this->getChildByName(GAME_SECOND_LAYER)) {
        UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, true);
        m_topMenuDelegate->closeChildLayer();
        return;
    }

	if (m_type == ROOMLIST) {
		if (m_roomListDelegate) {
			m_roomListDelegate->closeRoomList();
		}
        
        if(!m_gameListDelegate){
            createGameListLayer();
        }
        m_type = GAMELIST;
        m_gameListDelegate->openGameList();
		return;
	}

	if (m_type == GAMELIST) {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        RoomLogic()->closeRoom();
        PlatformLogic()->closePlatform();
        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
        
//		HNAudioEngine::getInstance()->playEffect("music/leave.mp3");
//		auto exitChangeLayer = GameExitChangeLayer::create();
//		exitChangeLayer->showExitChoose(this->getParent(), CHILD_LAYER_EXITCHANGE_TAG, CHILD_LAYER_EXITCHANGE_TAG);
		return;
	}
}

void GamePlatformV88::setLayerType(LayerType layerType) {
	m_type = layerType;
	if (layerType == GAMELIST) {
        if (UserDefault::getInstance()->getBoolForKey("FIRST_LOGIN")) {
            CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
            _gameListLogic->start();
            _gameListLogic->requestGameList();
            //
            PlatformLogic()->requestAllRoomListInfo();
            UserDefault::getInstance()->setBoolForKey("FIRST_LOGIN", false);
			//登录清空银行密码
			UserDefault::getInstance()->setStringForKey("bankTransactionPassword", "");
        } else {
             createGameListLayer();
        }
	}
	else if (layerType == ROOMLIST) {
		if (m_gameListDelegate)
		{
			m_gameListDelegate->closeGameList();
		}

        createRoomListLayer();
        m_roomListDelegate->openRoomList();
	}
	else
	{
	}
}

void GamePlatformV88::updateGameList(bool success, const std::string &message) {
	CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
	if (success) {
        createGameListLayer();
	}
	else {
		CommandManager::getInstance()->showGamePrompt(GBKToUtf8(message.c_str()));
	}
}

void GamePlatformV88::createGameListLayer() {
	m_roomListDelegate = nullptr;

	auto gameListComposite = GameListFactory::createWithVersion();
	gameListComposite->setPlatformDelegate(this);
	gameListComposite->setBottomMenuDelegate(m_bottomMenuDelegate);
	gameListComposite->setGameListLogic(_gameListLogic);
	addChild(gameListComposite);
	gameListComposite->setLocalZOrder(-1);

	m_gameListDelegate = gameListComposite;
}

void GamePlatformV88::updateRoomList(bool success, const std::string& message) {
	CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
	if (success) {
        createRoomListLayer();
	}
	else {
		GamePromptLayer::create()->showPrompt(GBKToUtf8(message.c_str()));
	}
}

void GamePlatformV88::createRoomListLayer() {
	m_deskListDelegate = nullptr;
    
    //清理
    if(getChildByName("gameRoomLayer"))
    {
        getChildByName("gameRoomLayer")->removeFromParentAndCleanup(true);
    }
    
	auto roomListComposite = RoomListFactory::createWithVersion();
	roomListComposite->setName("gameRoomLayer");
	roomListComposite->setTopMenuDelegate(m_topMenuDelegate);
	roomListComposite->setBottomMenuDelegate(m_bottomMenuDelegate);
	roomListComposite->setPlatformDelegate(this);
	roomListComposite->setRoomListLogic(_roomListLogic);
	roomListComposite->setGameListLogic(_gameListLogic);
	roomListComposite->execute();
	addChild(roomListComposite);

	m_roomListDelegate = roomListComposite;
	m_topMenuDelegate->setRoomListDelegate(roomListComposite);
}

void GamePlatformV88::openNewYearActivity()
{
    GameCreator()->setCurrentGame(11118888);
    //更新单前游戏的房间信息
    RoomInfoModule()->updateRoomsForTheGame(11118888);
    
    createRoomListLayer();
}
