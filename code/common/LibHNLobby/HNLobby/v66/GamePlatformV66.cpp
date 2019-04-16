#include "GamePlatformV66.h"
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
#include "PlatformResV66.h"
#include "GameRankV66.h"

bool GamePlatformV66::init() {
    if (!GamePlatformOne::init()) {
        return false;
    }
	//createGameListLayer();
    return true;
}

void GamePlatformV66::handleReturnButton() {
    if (this->getChildByName(GAME_SECOND_LAYER)) {
        UserDefault::getInstance()->setBoolForKey(TOUCH_HEAD, true);
        m_topMenuDelegate->closeChildLayer();
        return;
    }

    if ((GAMETYPE == m_type|| m_type == GAMELIST) && m_gameListDelegate)
    {
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
		RoomLogic()->closeRoom();
		PlatformLogic()->closePlatform();
		UserDefault::getInstance()->setBoolForKey("isLoginTipBindPhone", false);
		CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
        
        return;
    }
    
	if (m_type == ROOMLIST) {
		if (m_roomListDelegate) {
			m_roomListDelegate->closeRoomList();
		}
        
        if(!m_gameListDelegate){
            createGameListLayer();
			//m_roomListDelegate->openRoomList();
        }
		else
		{
			m_gameListDelegate->openGameList();
			if(_gameListLogic != nullptr) _gameListLogic->start();
		}

        m_type = GAMELIST;
        
		return;
	}

	if (m_type == GAMELIST) {
//        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
//        RoomLogic()->closeRoom();
//        PlatformLogic()->closePlatform();
//        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
//        m_type = ROOMLIST;
        m_type = GAMETYPE;
        m_gameListDelegate->openGameTypeList();
        m_bottomMenuDelegate->setQuickBtn(true);
//		HNAudioEngine::getInstance()->playEffect("music/leave.mp3");
//		auto exitChangeLayer = GameExitChangeLayer::create();
//		exitChangeLayer->showExitChoose(this->getParent(), CHILD_LAYER_EXITCHANGE_TAG, CHILD_LAYER_EXITCHANGE_TAG);
		return;
	}
}

void GamePlatformV66::setLayerType(LayerType layerType) {
    if (m_gameListDelegate == NULL && layerType == GAMELIST) {//游戏中退出直接返回主界面游戏类型选择界面。
        layerType = GAMETYPE;
    }
    m_bottomMenuDelegate->setQuickBtn(false);
	m_type = layerType;
	if (layerType == GAMETYPE) {
        m_bottomMenuDelegate->setQuickBtn(true);
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
             _gameListLogic->start();
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

	_gameListLogic->start();
}

void GamePlatformV66::updateGameList(bool success, const std::string &message) {
	CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
	if (success) {
        createGameListLayer();
	}
	else {
		CommandManager::getInstance()->showGamePrompt(GBKToUtf8(message.c_str()));
	}
}

void GamePlatformV66::createGameListLayer() {
	m_roomListDelegate = nullptr;

	auto gameListComposite = GameListFactory::createWithVersion();
	gameListComposite->setPlatformDelegate(this);
	gameListComposite->setBottomMenuDelegate(m_bottomMenuDelegate);
	gameListComposite->setGameListLogic(_gameListLogic);
	addChild(gameListComposite);
	gameListComposite->setLocalZOrder(100);

	m_gameListDelegate = gameListComposite;
}

void GamePlatformV66::updateRoomList(bool success, const std::string& message) {
	CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
	if (success) {
        createRoomListLayer();
	}
	else {
		GamePromptLayer::create()->showPrompt(GBKToUtf8(message.c_str()));
	}
}

void GamePlatformV66::createRoomListLayer() {
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
	addChild(roomListComposite,BOTTOM_MENU_ORDER_V66 + 1); //房间在上下菜单的上面

	m_roomListDelegate = roomListComposite;
	m_topMenuDelegate->setRoomListDelegate(roomListComposite);
}

void GamePlatformV66::openNewYearActivity()
{
	_gameListLogic->stop();
	_roomListLogic->start();

	UserDefault::getInstance()->setIntegerForKey("Enter_Game_ID", 11118888);
	UserDefault::getInstance()->setBoolForKey("TURNPLATE_REQUEST_LOGIN", true);
    GameCreator()->setCurrentGame(11118888);
    //更新单前游戏的房间信息
    RoomInfoModule()->updateRoomsForTheGame(11118888);

	ComRoomInfo*pRoomInfo = RoomInfoModule()->getRoom(0);
	if (pRoomInfo)
	{
		UserDefault::getInstance()->setIntegerForKey(QUICK_JOIN_ROOM_ID, pRoomInfo->uRoomID);

		//RoomLogic()->setSelectedRoom(pRoomInfo);
		//RoomLogic()->setRoomRule(pRoomInfo->dwRoomRule);
		//UserInfoModule()->clear();
		
		//RoomLogic()->connectRoom(pRoomInfo->szServiceIP, pRoomInfo->uServicePort);
		//RoomLogic()->roomLogin(pRoomInfo->uRoomID);

		//RoomLogic()->loginResult.pUserInfoStruct.bDeskNO = 1;
		//RoomLogic()->loginResult.pUserInfoStruct.bDeskStation = 1;
	}
	//if (m_roomListDelegate)
	//{
		//m_roomListDelegate->closeRoomList();
	//}
	//else
	//{
		//createRoomListLayer();
	//}
	
	//m_roomListDelegate->openRoomList();
}

