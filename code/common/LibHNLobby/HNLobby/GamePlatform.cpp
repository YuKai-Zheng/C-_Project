#include "GamePlatform.h"

#include "pattern/CommandManager.h"
#include "command/ShowGamePrompt.h"
#include "PlatformDefine.h"
#include "VersionInfo/VersionInfo.h"
#include "GameNotice.h"
#include "ChildLayer/ServiceLayer.h"
#include "ChildLayer/GameSetLayer.h"
#include "PersionalCenter/GameUserDataLayer.h"
#include "PersionalCenter/BindPhone.h"
#include "PersionalCenter/ModifyPassword.h"
#include "ChildLayer/GameExitChangeLayer.h"
#include "ChildLayer/GameSignLayer.h"
#include "ChildLayer/GameRankingList.h"
#include "ChildLayer/GameRechargeRebate.h"
#include "GameExitLayer.h"
#include "ChildLayer/GameBankLayer.h"
#include "ChildLayer/GameStoreLayer.h"
#include "ChildLayer/GameOnlineReward.h"

void GamePlatform::onSpreadInfo(bool result, bool bInfo, const std::string &msg) {
}

GamePlatform::GamePlatform() {
    _httpTime = 0;
    _roomListLogic = new HNPlatformRoomList(this);
    _gameListLogic = new HNPlatformGameList(this);
}

GamePlatform::~GamePlatform() {
    HN_SAFE_DELETE(_roomListLogic);
    HN_SAFE_DELETE(_gameListLogic);
}

bool GamePlatform::init() {
    if (!HNLayer::init()) {
        return false;
    }
    enableKeyboard();
    initPlatform();
    //UserDefault::getInstance()->setIntegerForKey(IS_FIRST_REGIST, 1);
    return true;
}

void GamePlatform::onEnter() {
    HNLayer::onEnter();
    bool canPlayBgMusic = UserDefault::getInstance()->getBoolForKey(MUSIC_SWITCH);
    HNAudioEngine::getInstance()->setSwitchOfMusic(canPlayBgMusic);

    bool canPlayEffect = UserDefault::getInstance()->getBoolForKey(EFFECT_SWITCH);
    HNAudioEngine::getInstance()->setSwitchOfEffect(canPlayEffect);
}

void GamePlatform::onEnterTransitionDidFinish() {
    HNLayer::onEnterTransitionDidFinish();
}

void GamePlatform::onExitTransitionDidStart() {
    HNLayer::onExitTransitionDidStart();
    _roomListLogic->stop();
    _gameListLogic->stop();
    HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GamePlatform::onExit() {
    Layer::onExit();
}

void GamePlatform::initPlatform() {
}

void GamePlatform::createGameListLayer() {
}

void GamePlatform::updateGameList(bool success, const std::string &message) {
}

void GamePlatform::updateGameUserCount(UINT Id, UINT count) {
}

void GamePlatform::updateRoomList(bool success, const std::string& message) {
}

void GamePlatform::updateRoomUserCount(UINT roomID, UINT userCount) {
}

void GamePlatform::createDeskListLayer(ComRoomInfo *roomInfo) {
}

void GamePlatform::platformDisConnect(const std::string& message) {
    ShowGamePrompt* prompt = (ShowGamePrompt*)CommandManager::getInstance()->get(SHOW_GAME_PROMPT);
    prompt->setText(message);
    prompt->setCallBack([this]() {
        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
        RoomLogic()->closeRoom();
        PlatformLogic()->closePlatform();
    });
    prompt->execute();
}

void GamePlatform::platformNews(int iType, const std::string &message, int iCount) {
	//全服公告
	if (iType == 0)
	{
		iCount = 100;
	}
	else if (iType == -11)//银商
	{
        
	}

	if (GameNotice::getNotic(this) == nullptr) {
		auto m_Notice = GameNotice::create();
		m_Notice->setLocalZOrder(10000 - 1);
		m_Notice->setName("GameNotice");
		m_Notice->openNotic(this, 1000, message, iCount, iType);
	}
	else {
		GameNotice::getNotic(this)->openNotic(this, 1000, message, iCount, iType);
	}
}

std::string GamePlatform::showHint() {
    /*srand(time(NULL));
    int randNum = rand()%7+1;*/
    int randNum = 1;
    char str[400];
    switch (randNum) {
    case 1:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||                              \
     CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        sprintf(str, "%s",
                GBKToUtf8("尊敬的玩家，聚乐熊商城充值通道暂停服务!"
                          "现已开通人工在线充值通道:\nQQ号:3306697775(聚乐熊-乐乐)""\n微信号:Sun093(添加时请注明：充值)""\n请认准官方号码，谨防诈骗！"));
#else
        sprintf(str, "%s",
                GBKToUtf8("尊敬的玩家，聚乐熊现已开通在线客服:\nQQ号:3306697775(""聚乐熊-乐乐)\n微信号:Sun093(添加时请注明：聚乐熊)""\n请认准官方号码，谨防诈骗！"));
#endif
        return str;
    }
    return "";
}

//void GamePlatform::ShowTIme(bool open) {
//    struct tm *tm;
//    time_t timep;
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
//    time(&timep);
//#else
//    //	struct  cc_timeval now;
//    //	CCTime::gettimeofdayCocos2d(&now, NULL);
//    //	timep = now.tv_sec;
//#endif
//
//    tm = localtime(&timep);
//    int year = tm->tm_year + 1900;
//    int month = tm->tm_mon + 1;
//    int day = tm->tm_mday;
//    int hour = tm->tm_hour;
//    int min = tm->tm_min;
//    int second = tm->tm_sec;
//    if (open) {
//        CCLOG("Go !!!  %d - %d - %d - %d - %d - %d", year, month, day, hour, min,second);
//    } else {
//        CCLOG("Shou !!!  %d - %d - %d - %d - %d - %d", year, month, day, hour, min,second);
//    }
//}

void GamePlatform::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *pEvent) {
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

void GamePlatform::setLayerType(LayerType layerType) {
    m_type = layerType;
    if (layerType == GAMETYPE) {
        CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
        m_topMenuDelegate->setType(game_type_type);
        //m_topMenuDelegate->moveOut(true);
        //m_bottomMenuDelegate->moveOut(true);
        m_topMenuDelegate->autoMove();
        m_bottomMenuDelegate->autoMove();

        if (UserDefault::getInstance()->getBoolForKey("FIRST_LOGIN")) {
            _gameListLogic->start();
            _gameListLogic->requestGameList();

        }

        createGameTypeLayer();

    } else if (layerType == GAMELIST) {
        m_topMenuDelegate->setType(game_list_type);
        //m_topMenuDelegate->moveOut(true);
        //m_bottomMenuDelegate->moveOut(true);
        m_topMenuDelegate->autoMove();
        m_bottomMenuDelegate->autoMove();

        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        _gameListLogic->start();
        _gameListLogic->requestGameList();

    } else if (layerType == ROOMLIST) {
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        //m_bottomMenuDelegate->moveOut(true);
        //m_topMenuDelegate->moveOut(true);
        m_topMenuDelegate->autoMove();
        m_bottomMenuDelegate->autoMove();
        m_topMenuDelegate->setType(room_type);

        _roomListLogic->start();
        _roomListLogic->requestRoomList();

    } else if (layerType == DESKLIST) {
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        //当前如果有广播存在就隐藏掉
        if (GameNotice::getNotic(this) != nullptr)
            GameNotice::getNotic(this)->setVisible(false);

        m_bottomMenuDelegate->moveOut(true);
        m_topMenuDelegate->setType(desk_type);

        createDeskListLayer(RoomLogic()->getSelectedRoom());
    }
}
