#include "GamePlatformOne.h"
#include "VersionInfo/VersionInfo.h"
#include "../../PlatformDefine.h"
#include "../../pattern/CommandManager.h"
#include "../../PersionalCenter/GameUserDataLayer.h"
#include "../../ChildLayer/GameSetLayer.h"
#include "../../ChildLayer/ServiceLayer.h"
#include "../../PersionalCenter/BindPhone.h"
#include "../../PersionalCenter/ModifyPassword.h"
#include "../../ChildLayer/GameExitChangeLayer.h"
#include "../../ChildLayer/GameSignLayer.h"
#include "../../ChildLayer/GameRankingList.h"
#include "../../ChildLayer/GameBankLayer.h"
#include "../../ChildLayer/GameStoreLayer.h"
#include "../../ChildLayer/GameOnlineReward.h"
#include "../../ChildLayer/GameRechargeRebate.h"
#include "../../GameExitLayer.h"
#include "../../command/ShowGamePrompt.h"
#include "../../pattern/factory/deskList/DeskListFactory.h"
#include "../../pattern/factory/deskList/IDeskList.h"
#include "../../GameNotice.h"
//#include "../../v3/PlatformResV3.h"
#include "../../pattern/factory/menu/TopMenuFactory.h"
#include "../../pattern/factory/menu/BottomMenuFactory.h"
#include "../../pattern/factory/gameList/GameListFactory.h"
#include "../../pattern/factory/roomList/RoomListFactory.h"
#include "../../pattern/factory/gameType/GameTypeFactory.h"
#include "../../pattern/factory/background/BackgroundFactory.h"
#include "../../Popularise/SpreadEditLayer.h"
#include "../../GamePrompt.h"
//#include "../../v99/PlatformResV99.h"
#include "../../PlatformMessage.h"
//#include "../../v66/PlatformResV66.h"
#include "../../v66/PlatformResV66.h"
#include "../../v66/BindPhone.hpp"

GamePlatformOne::GamePlatformOne() {
    _httpTime = 0;
    _roomListLogic = new HNPlatformRoomList(this);
    _gameListLogic = new HNPlatformGameList(this);

	m_gameTypeDelegate = nullptr;
    m_gameListDelegate = nullptr;
    m_roomListDelegate = nullptr;
}

GamePlatformOne::~GamePlatformOne() {
    HN_SAFE_DELETE(_roomListLogic);
    HN_SAFE_DELETE(_gameListLogic);
}

bool GamePlatformOne::init() {
    if (!HNLayer::init()) {
        return false;
    }
    enableKeyboard();
    initPlatform();

    auto userDefault = UserDefault::getInstance();
    //userDefault->setIntegerForKey(IS_FIRST_REGIST, 1);

    //获取账号密码
    auto account = userDefault->getStringForKey(USERNAME_TEXT);
    auto pass = userDefault->getStringForKey(PASSWORD_TEXT);
    //所有账号
    auto totleAccStr = UserDefault::getInstance()->getStringForKey("RECORD_SIGN_ACCOUNT");

    int count = Tools::findCharCountOnString('_', totleAccStr);
    if (totleAccStr.find(account) == string::npos) {
        if (count < 5) {
            if (totleAccStr == "") {
                totleAccStr = account + '_';
            } else {
                totleAccStr = account + '_' + totleAccStr;
            }
            UserDefault::getInstance()->setStringForKey("RECORD_SIGN_ACCOUNT", totleAccStr);
        } else {
            totleAccStr = account + '_' + totleAccStr;
            std::string newStr;
            int count = 0;
            for (auto i = totleAccStr.begin(); i != totleAccStr.end(); i++) {
                if (count >= 5)

                    break;
                newStr = newStr + *i;
                if (*i == '_')
                    count++;
            }
            UserDefault::getInstance()->setStringForKey("RECORD_SIGN_ACCOUNT", newStr);
        }
    } else {
        if (count >= 5) {
            if (totleAccStr.find(account) != string::npos)
                return true;
            //auto weizi = totleAccStr.find(dd); 查找子串第一次出现的位置
            totleAccStr = account + '_' + totleAccStr;
            std::string newStr;
            int count = 0;
            for (auto i = totleAccStr.begin(); i != totleAccStr.end(); i++) {
                if (count >= 5)

                    break;
                newStr = newStr + *i;
                if (*i == '_')
                    count++;
            }
            UserDefault::getInstance()->setStringForKey("RECORD_SIGN_ACCOUNT", newStr);
        }
    }
    
    if (!UserDefault::getInstance()->getBoolForKey("WELCOME_TO_GAME_8LE")) {
        PlatformLogic()->sendData(MDM_GP_MESSAGE, ASS_GP_LOGONMSG_REQUEST,nullptr,0,HN_SOCKET_CALLBACK(GamePlatformOne::dealVirualSockteMsg,this));
        
        UserDefault::getInstance()->setBoolForKey("WELCOME_TO_GAME_8LE",true);
    }
    
    return true;
}
bool GamePlatformOne::dealVirualSockteMsg(HNSocketMessage* socketMessage)//这个回调没什么用
{
    return true;
}
void GamePlatformOne::onEnter() {
    HNLayer::onEnter();
    bool canPlayBgMusic = UserDefault::getInstance()->getBoolForKey(MUSIC_SWITCH);
    HNAudioEngine::getInstance()->setSwitchOfMusic(canPlayBgMusic);

    bool canPlayEffect = UserDefault::getInstance()->getBoolForKey(EFFECT_SWITCH);
    HNAudioEngine::getInstance()->setSwitchOfEffect(canPlayEffect);

//    if (VERSION_THREE) {
//        HNAudioEngine::getInstance()->playBackgroundMusic(NEW_YEAR_BG, true);
//    } else
    /*
    if (VERSION_EIGHTY_EIGHT) {
		if (HNAudioEngine::getInstance()->hasBackgroundMusic()) {
			HNAudioEngine::getInstance()->resumeBackgroundMusic();
		}
        HNAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC_V88, true);
    }
     */
//    else if (VERSION_NINETY_NINE) {
//        HNAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC_V99, true);
//    }
    {
        HNAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSIC, true);
    }
}

void GamePlatformOne::onEnterTransitionDidFinish() {
    HNLayer::onEnterTransitionDidFinish();
}

void GamePlatformOne::onExitTransitionDidStart() {
    HNLayer::onExitTransitionDidStart();
    _roomListLogic->stop();
    _gameListLogic->stop();
    HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GamePlatformOne::onExit() {
    Layer::onExit();
}

void GamePlatformOne::initPlatform() {
    m_roomListDelegate = nullptr;
    m_bottomMenuDelegate = nullptr;
    m_topMenuDelegate = nullptr;
    m_gameListDelegate = nullptr;

    // 背景图
    if (VERSION_ONE || VERSION_TWO) {
        setBackGroundImage(PLATFORM_BG);
    }


	if (VERSION_NINETY_NINE || VERSION_EIGHTY_EIGHT || VERSION_SIXTY_SIX) {
        
    }
    else
    {
        auto csb = CSLoader::createNode(BG_ANIMATION_CSB);
        csb->setPosition(Vec2(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.5f));
        addChild(csb);
        auto antAction = CSLoader::createTimeline(BG_ANIMATION_CSB);
        csb->runAction(antAction);
        antAction->play("guangban", true);
    }
    
    auto topMenuComposite = TopMenuFactory::createWithVersion();
    topMenuComposite->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height));
    topMenuComposite->setPlatformDelegate(this);
    topMenuComposite->setLocalZOrder(TOP_MENU_ORDER_V66);
//    if (VERSION_NINETY_NINE) {
//        topMenuComposite->setLocalZOrder(BOTTOM_MENU_ORDER_V99);
//    } else
//    if (VERSION_SIXTY_SIX) {
//        topMenuComposite->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height));
//        topMenuComposite->setLocalZOrder(TOP_MENU_ORDER_V66);
//    }
    addChild(topMenuComposite);

    auto bottomMenuComposite = BottomMenuFactory::createWithVersion();
    bottomMenuComposite->setPosition(Vec2(WIN_SIZE.width / 2,0));
    bottomMenuComposite->setTopMenuDelegate(topMenuComposite);
	bottomMenuComposite->setPlatformDelegate(this);
    bottomMenuComposite->setLocalZOrder(BOTTOM_MENU_ORDER_V66);
//    if (VERSION_NINETY_NINE) {
//        bottomMenuComposite->setLocalZOrder(TOP_MENU_ORDER_V99);
//    } else
//    if (VERSION_SIXTY_SIX) {
//        //v88需要bottom在子功能下面
//        bottomMenuComposite->setPosition(Vec2(WIN_SIZE.width/2, 0));
//        bottomMenuComposite->setLocalZOrder(BOTTOM_MENU_ORDER_V66);
//    }
    
    
    addChild(bottomMenuComposite);

    topMenuComposite->setBottomMenuDelegate(bottomMenuComposite);
    m_topMenuDelegate = topMenuComposite;
    m_bottomMenuDelegate = bottomMenuComposite;

    auto curMoreAss = UserDefault::getInstance()->getStringForKey("CUR_MORE_ASS");
    auto curMorePass= UserDefault::getInstance()->getStringForKey("CUR_MORE_PASS");
    
    /*auto loginResult = PlatformLogic()->loginResult;
    if (loginResult.iBindMobile == 0 && !UserDefault::getInstance()->getBoolForKey("isLoginTipBindPhone")) {
        
        auto promptLayer = GamePromptLayer::create();
        promptLayer->setPrompt(" \n  为了保证您的账号安全，请绑定手机!!! \n\n备注：绑定成功随机赠送10～66万金币(重复绑定无效)");
        promptLayer->setOkayCallBack([this](){
            
        auto bindPhoneGui = BindPhoneGui::create();
        string title("绑定手机");
        bindPhoneGui->SetOpType(BindPhoneOpType::SendCode, true);
        bindPhoneGui->Open(this, title,WIN_SIZE/2, 101, 0);//cocos2d::Vec2::ANCHOR_MIDDLE
            
        });
        
        addChild(promptLayer,101);
        
        UserDefault::getInstance()->setBoolForKey("isLoginTipBindPhone",true);
        
    }*/
    
    
}

void GamePlatformOne::createGameListLayer() {
    m_roomListDelegate = nullptr;

    auto gameListComposite = GameListFactory::createWithVersion();
    gameListComposite->setPlatformDelegate(this);
    gameListComposite->setGameListLogic(_gameListLogic);
    addChild(gameListComposite);
    gameListComposite->setLocalZOrder(-1);

    m_gameListDelegate = gameListComposite;
}

void GamePlatformOne::updateGameList(bool success, const std::string &message) {
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

void GamePlatformOne::updateGameUserCount(UINT Id, UINT count) {
    if (m_gameListDelegate) {
        m_gameListDelegate->updateGameUserCount(Id, count);
    }
}

void GamePlatformOne::updateRoomList(bool success, const std::string& message) {
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    if (success) {
        createRoomListLayer();
    } else {
        GamePromptLayer::create()->showPrompt(GBKToUtf8(message.c_str()));
    }
}

void GamePlatformOne::updateRoomUserCount(UINT roomID, UINT userCount) {
    if (m_roomListDelegate) {
        m_roomListDelegate->updateRoomUserCount(roomID, userCount);
    }
}

void GamePlatformOne::createDeskListLayer(ComRoomInfo *roomInfo) {
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

void GamePlatformOne::platformDisConnect(const std::string& message) {
    ShowGamePrompt* prompt = (ShowGamePrompt*)CommandManager::getInstance()->get(SHOW_GAME_PROMPT);
    prompt->setText(message);
    prompt->setCallBack([this]() {
        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
        RoomLogic()->closeRoom();
        PlatformLogic()->closePlatform();
    });
    prompt->execute();
}

void GamePlatformOne::platformNews(int iType, const std::string &message, int iCount) {
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
    } else {
        GameNotice::getNotic(this)->openNotic(this, 1000, message, iCount, iType);
    }
}

void GamePlatformOne::platformNoticeList(const std::string message) {
	if (m_gameTypeDelegate)
	{
		m_gameTypeDelegate->typeNoticeList(message);
	}
}

std::string GamePlatformOne::showHint() {
    /*srand(time(NULL));
    int randNum = rand()%7+1;*/
    int randNum = 1;
    char str[400];
    switch (randNum) {
    case 1:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        sprintf(str, "%s",GBKToUtf8("尊敬的玩家，聚乐熊商城充值通道暂停服务!"
                                    "现已开通人工在线充值通道:/nQQ号:3306697775(聚乐熊-乐乐)""/n微信号:Sun093(添加时请注明：充值)""/n请认准官方号码，谨防诈骗！"));
#else
        sprintf(str, "%s",
                GBKToUtf8("尊敬的玩家，聚乐熊现已开通在线客服:/nQQ号:3306697775(""聚乐熊-乐乐)/n微信号:Sun093(添加时请注明：聚乐熊)""/n请认准官方号码，谨防诈骗！"));
#endif
        return str;
    }
    return "";
}

void GamePlatformOne::createRoomListLayer() {
    m_gameListDelegate = nullptr;
    m_deskListDelegate = nullptr;

    auto roomListComposite = RoomListFactory::createWithVersion();
    addChild(roomListComposite);
	roomListComposite->setLocalZOrder(-2);
    roomListComposite->setName("gameRoomLayer");
    roomListComposite->setTopMenuDelegate(m_topMenuDelegate);
    roomListComposite->setBottomMenuDelegate(m_bottomMenuDelegate);
    roomListComposite->setPlatformDelegate(this);
    roomListComposite->setRoomListLogic(_roomListLogic);
    roomListComposite->setGameListLogic(_gameListLogic);
    roomListComposite->execute();
    m_roomListDelegate = roomListComposite;

    m_topMenuDelegate->setRoomListDelegate(roomListComposite);
}

void GamePlatformOne::createGameTypeLayer() {
    m_gameListDelegate = nullptr;
    m_roomListDelegate = nullptr;
    m_deskListDelegate = nullptr;
    m_gameTypeDelegate = nullptr;
    
    auto delegate = GameTypeFactory::createWithVersion();
    delegate->setPlatformDelegate(this);
    delegate->setBottomMenuDelegate(m_bottomMenuDelegate);
//    if (VERSION_NINETY_NINE) {
//        delegate->setLocalZOrder(GAMETYPE_ORDER_V99);
//    }
    addChild(delegate);
    
    m_gameTypeDelegate = delegate;
}

//void GamePlatformOne::ShowTIme(bool open) {
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

void GamePlatformOne::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *pEvent) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (EventKeyboard::KeyCode::KEY_F1 == keyCode)
#endif
    {
//         do {
//             ServiceLayer *serviceLayer = dynamic_cast<ServiceLayer *>(
//                                              this->getChildByTag(CHILD_LAYER_SERVICE_TAG));
//             if (nullptr != serviceLayer) {
//                 serviceLayer->closeService();
//                 break;
//             }
// 
//             GameSetLayer *setLayer = dynamic_cast<GameSetLayer *>(
//                                          this->getChildByTag(CHILD_LAYER_SET_TAG));
//             if (nullptr != setLayer) {
//                 setLayer->closeSet();
//                 break;
//             }
// 
//             GameUserDataLayer *userDataLayer = dynamic_cast<GameUserDataLayer *>(
//                                                    this->getChildByTag(CHILD_LAYER_USERHEAD_TAG));
//             if (nullptr != userDataLayer) {
//                 userDataLayer->closeUserLayer();
//                 break;
//             }
// 
//             BindPhoneLayer *bindPhone = dynamic_cast<BindPhoneLayer *>(
//                                             this->getChildByTag(CHILD_LAYER_PHONE_TAG));
//             if (nullptr != bindPhone) {
//                 bindPhone->close();
//                 break;
//             }
// 
//             ModifyPasswordLayer *modifyPass = dynamic_cast<ModifyPasswordLayer *>(
//                                                   this->getChildByTag(CHILD_LAYER_PASS_TAG));
//             if (nullptr != modifyPass) {
//                 modifyPass->close();
//                 break;
//             }
// 
//             GameExitChangeLayer *exitChange = dynamic_cast<GameExitChangeLayer *>(
//                                                   this->getChildByTag(CHILD_LAYER_EXITCHANGE_TAG));
//             if (nullptr != exitChange) {
//                 exitChange->closeExitChoose();
//                 break;
//             }
// 
//             GameSignLayer *gameSign = dynamic_cast<GameSignLayer *>(
//                                           this->getChildByTag(CHILD_LAYER_SIGN_TAG));
//             if (nullptr != gameSign) {
//                 gameSign->closeSign();
//                 break;
//             }
// 
//             GameRankingList *gameRank = dynamic_cast<GameRankingList *>(
//                                             this->getChildByTag(CHILD_LAYER_RANK_TAG));
//             if (nullptr != gameRank) {
//                 gameRank->closeRanking();
//                 break;
//             }
// 
//             GameBankLayer *bankLayer = dynamic_cast<GameBankLayer *>(
//                                            this->getChildByTag(CHILD_LAYER_BANK_TAG));
//             if (nullptr != bankLayer) {
//                 bankLayer->closeBank();
//                 break;
//             }
// 
//             GameStoreLayer *storeLayer = dynamic_cast<GameStoreLayer *>(
//                                              this->getChildByTag(CHILD_LAYER_STORE_TAG));
//             if (nullptr != storeLayer) {
//                 storeLayer->closeStore();
//                 break;
//             }
// 
//             GameOnlineReward *OnlineReward = dynamic_cast<GameOnlineReward *>(
//                                                  this->getChildByTag(CHILD_LAYER_ONLINE_TAG));
//             if (nullptr != OnlineReward) {
//                 OnlineReward->closeOnlineReward();
//                 break;
//             }
// 
//             GameRechargeRebate *rebateLayer = dynamic_cast<GameRechargeRebate *>(
//                                                   this->getChildByTag(CHILD_LAYER_REBATE_TAG));
//             if (nullptr != rebateLayer) {
//                 rebateLayer->closeRebate();
//                 break;
//             }
// 
//             // 调用离开游戏层
//             GameExitLayer *exitLayer = dynamic_cast<GameExitLayer *>(
//                                            this->getChildByTag(CHILD_LAYER_EXIT_TAG));
//             if (nullptr == exitLayer) {
//                 auto exitLayer = GameExitLayer::create();
//                 exitLayer->setPosition(Size::ZERO);
//                 addChild(exitLayer, 100000000, CHILD_LAYER_EXIT_TAG);
//             } else {
//                 exitLayer->callBackNo();
//             }
//         } while (0);
    }
}

void GamePlatformOne::setLayerType(LayerType layerType) {
    m_type = layerType;
    if (layerType == GAMETYPE) {
        CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
        m_topMenuDelegate->setType(game_type_type);
        m_topMenuDelegate->autoMove();
        m_bottomMenuDelegate->autoMove();
        if (UserDefault::getInstance()->getBoolForKey("FIRST_LOGIN")) {
            _gameListLogic->start();
            _gameListLogic->requestGameList();
        }

        createGameTypeLayer();

    } else if (layerType == GAMELIST || layerType == GAMELIST_ROOM) {
        m_topMenuDelegate->setType(game_list_type);
        m_topMenuDelegate->autoMove();
        if (layerType == GAMELIST) {
            m_bottomMenuDelegate->autoMove();
        } else if (layerType == GAMELIST_ROOM) {
            m_bottomMenuDelegate->moveOut(true);
        }

        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
        _gameListLogic->start();
        _gameListLogic->requestGameList();

    } else if (layerType == ROOMLIST) {
        CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
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

void GamePlatformOne::handleReturnButton() {
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

void GamePlatformOne::openNewYearActivity() {
    _gameListLogic->stop();
    _roomListLogic->start();
    PlatformLogic()->requestRoomListInfo(1, 11118888);
    GameCreator()->setCurrentGame(11118888);
    //UserDefault::getInstance()->setBoolForKey("NEW_YEAR", true);

    CommandManager::getInstance()->addLoaderCommand(GBKToUtf8("加载中..."));
}

void GamePlatformOne::onSpreadInfo(bool result, bool bInfo, const std::string &msg) {
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
