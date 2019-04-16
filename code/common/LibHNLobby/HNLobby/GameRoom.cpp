#include "HNLobby/GameRoom.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GameDesk.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GameMatch.h"
#include "HNUIExport.h"
#include <string>
#include "pattern/StateManager.h"
#include "state/MenuState.h"
#include "pattern/CommandManager.h"
#include "CommonMacro.h"
#include "HNLobby/globel.h"

//////////////////////////////////////////////////////////////////////////
//static const float enter_game_desk_outtime_timer = 30.0f;
//static const float update_game_room_people_timer = 2.0f;
//////////////////////////////////////////////////////////////////////////
//static const char* connect_room_text		= "连接房间......";
//static const char* login_room_text			= "登陆房间......";
//static const char* request_room_info_text	= "获取房间数据......";
//static const char* allocation_table_please_wait_text = "正在配桌，请稍后......";
//static const char* enterGame_please_wait_text = "正在进入游戏，请稍后......";

//////////////////////////////////////////////////////////////////////////
static const char* ROOM_ITEM_UI = "csb/platform/room/roomItem.csb";
//static const char* ROOM_CAIJIN_ANIAMITION = "common/animation/handsel/handsel.csb";
//static const char* ROOM_NAME_FNT = "platform/fonts/gameName.fnt";

static const char* GAME_PAGESPROMPT_PATH = "platform/gameList/gameItemRes/page_Prompt.png";
//////////////////////////////////////////////////////////////////////////

//csb
static const char* CSB_ZHONGJICHANG_AM = "common/animation/room_icon/zhongjichang/zhongji.csb";
static const char* CSB_ZHIZUNCHANG_AM = "common/animation/room_icon/zhizunchang/zhizun.csb";
static const char* CSB_XINSHOUCHANG_AM = "common/animation/room_icon/xinshouchang/xinshou.csb";
static const char* CSB_GAOJICHANG_AM = "common/animation/room_icon/gaojichang/gaoji.csb";
static const char* CSB_CHUJICHANG_AM = "common/animation/room_icon/chujichang/chuji.csb";
static const char* CSB_CHAOJICHANG_AM = "common/animation/room_icon/chaojichang/chaoji.csb";
//static const char* CSB_CAIJI_AM = "common/animation/room_icon/handsel/handsel.csb";

static const Size pageViewRoomsSize(1280, 500);

//////////////////////////////////////////////////////////////////////////
GameRoom::GameRoom()
    : _currentSelectedRoom (nullptr)
    , _pageViewRooms (nullptr)
    , onCloseCallBack (nullptr)
    , onEnterDeskCallBack (nullptr)
    , _currentPageIdx (0)
    , _isTouch (true)
    , _overdueTime(false)
    , _fastEnterMatch(false)
    , _roomID(-1)
    , _gameName(nullptr) {
    memset(_gameRoomChangci, 0, sizeof(_gameRoomChangci));
    _roomLogic = new HNRoomLogicBase(this);
}

GameRoom::~GameRoom() {
    _roomLogic->stop();
    PlatformLogic()->removeEventSelector(MDM_GP_GET_CONTEST_ROOMID, 0);
    PlatformLogic()->removeEventSelector(MDM_GP_GET_CONTEST_ROOMID, 1);
    PlatformLogic()->removeEventSelector(MDM_GP_GET_TIMINGMATCH_TIME, ASS_GP_GET_TIMINGMATCH_TIME);
    HN_SAFE_DELETE(_roomLogic);
}

void GameRoom::closeRoomList() {
    _roomLogic->stop();
    this->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([&]() {
        if (nullptr != onCloseCallBack) {
            onCloseCallBack();
        }
        this->getParent()->removeFromParent();
    }), nullptr));
}

bool GameRoom::init() {
    if (!HNLayer::init()) {
        return false;
    }

    this->setCascadeOpacityEnabled(true);

    int roomAllCount = RoomInfoModule()->getRoomCount();
    for (int i = 0; i < roomAllCount; i++) {
        _vecComRoomInfo.push_back(RoomInfoModule()->getRoom(i));
    }
    int currentIndex = 0;

    //特殊处理活动
    if (roomAllCount > 0 && _vecComRoomInfo.at(0)->uNameID == 11118888) {
        _roomLogic->start();
        _roomLogic->requestLogin(_vecComRoomInfo.at(0)->uRoomID);
        _roomID = _vecComRoomInfo.at(0)->uRoomID;
        return true;
    }

    if (roomAllCount == 1) {
        Point pos = Point(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.5f);
        auto room = createRoomItem(_vecComRoomInfo.at(0));
        room->setPosition(pos);
        addChild(room);
    } else if (roomAllCount == 2) {
        Point pos[2] = { Point(WIN_SIZE.width*0.25f, WIN_SIZE.height*0.5f), Point(WIN_SIZE.width*0.75f, WIN_SIZE.height*0.5f) };
        loadRoom(roomAllCount, pos);
    } else if (roomAllCount == 3) {
        Point pos[3] = { Point(WIN_SIZE.width*0.3f, WIN_SIZE.height*0.58f), Point(WIN_SIZE.width*0.7f, WIN_SIZE.height*0.58f),
                         Point(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.27f)
                       };
        loadRoom(roomAllCount, pos);
    } else if (roomAllCount == 4) {
        Point pos[4] = { Point(WIN_SIZE.width*0.3f, WIN_SIZE.height*0.58f), Point(WIN_SIZE.width*0.7f, WIN_SIZE.height*0.58f),
                         Point(WIN_SIZE.width*0.3f, WIN_SIZE.height*0.27f), Point(WIN_SIZE.width*0.7f, WIN_SIZE.height*0.27f)
                       };
        loadRoom(roomAllCount, pos);
    } else if (roomAllCount == 5) {
        Point pos[5] = { Point(WIN_SIZE.width*0.18f, WIN_SIZE.height*0.58f), Point(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.58f),
                         Point(WIN_SIZE.width*0.82f, WIN_SIZE.height*0.58f), Point(WIN_SIZE.width*0.32f, WIN_SIZE.height*0.27f),
                         Point(WIN_SIZE.width*0.68f, WIN_SIZE.height*0.27f)
                       };
        loadRoom(roomAllCount, pos);
    } else if (roomAllCount == 6) {
        Point pos[6] = { Point(WIN_SIZE.width*0.18f, WIN_SIZE.height*0.58f), Point(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.58f),
                         Point(WIN_SIZE.width*0.82f, WIN_SIZE.height*0.58f), Point(WIN_SIZE.width*0.18f, WIN_SIZE.height*0.27f),
                         Point(WIN_SIZE.width*0.5f, WIN_SIZE.height*0.27f), Point(WIN_SIZE.width*0.82, WIN_SIZE.height*0.27f)
                       };
        loadRoom(roomAllCount, pos);
    }

    //createRoomList();

    PlatformLogic()->addEventSelector(MDM_GP_GET_TIMINGMATCH_TIME, ASS_GP_GET_TIMINGMATCH_TIME,HN_SOCKET_CALLBACK(GameRoom::contestBeginTime, this));

    PlatformLogic()->addEventSelector(MDM_GP_GET_CONTEST_ROOMID, 1, HN_SOCKET_CALLBACK(GameRoom::contestInitError, this));

    return true;
}

void GameRoom::onExit() {
    HNLayer::onExit();
}

void GameRoom::loadRoom(INT roomCount, Point pos[]) {
    for (int i = 0; i < roomCount; i++) {
        auto room = createRoomItem(_vecComRoomInfo.at(i));

        room->setPosition(pos[i]);
        addChild(room);
    }
}


void GameRoom::createRoomList() {
    _pageViewRooms = PageView::create();
    _pageViewRooms->setContentSize(Size(WIN_SIZE.width, pageViewRoomsSize.height));
    //_pageViewRooms->setCl(Size(WIN_SIZE.width, pageViewRoomsSize.height));
    _pageViewRooms->setAnchorPoint(Vec2(0.5f, 0.5f));
    _pageViewRooms->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.5f));
    _pageViewRooms->setTouchEnabled(true);
    //_pageViewRooms->setCustomScrollThreshold(15);
    addChild(_pageViewRooms, 3);

    _pageViewRooms->setCascadeOpacityEnabled(true);

    int roomAllCount = RoomInfoModule()->getRoomCount();
    int currentIndex = 0;

    std::vector<ComRoomInfo*> pages;

    int pageCount = (roomAllCount / 4);
    int remainder = (roomAllCount % 4);

    int pageLen = (remainder == 0) ? pageCount : pageCount + 1;
    int central = -pageLen / 2;
    if (abs(central) > 0) {
        for (int i = 0; i < pageLen; i++) {
            auto page = Sprite::create(GAME_PAGESPROMPT_PATH);
            addChild(page);
            _pagesPrompt.push_back(page);

            page->setPosition(Vec2(WIN_SIZE.width / 2 + central * 25, WIN_SIZE.height * 0.16f));
            if (i > 0) {
                page->setScale(0.8f);
                page->setColor(Color3B(150, 150, 150));
            }
            central++;
        }
    }

    if (pageCount > 0) {
        for (int currentPage  = 0; currentPage < pageCount; currentPage++) {
            pages.clear();
            for (int room = 0; room < 4; room ++, currentIndex ++) {
                ComRoomInfo* roomInfo = RoomInfoModule()->getRoom(currentIndex);
                pages.push_back(roomInfo);
            }
            createRoomPage(pages);
        }
    }

    if (remainder > 0) {
        pages.clear();
        for (int room = 0; room < remainder; room ++, currentIndex ++) {
            ComRoomInfo* roomInfo = RoomInfoModule()->getRoom(currentIndex);
            pages.push_back(roomInfo);
        }
        createRoomPage(pages);
    }
}
//添加房间
void GameRoom::createRoomPage(std::vector<ComRoomInfo*> pages) {
    //创建房间列表子页面
    auto roomItemLayout = Layout::create();
    roomItemLayout->setName("page");
    roomItemLayout->setContentSize(pageViewRoomsSize);

    roomItemLayout->setCascadeOpacityEnabled(true);

    int idx = 0;
    for (auto room : pages) {
        idx++;
        float posX = idx % 2 ? 0.25f : 0.75f;
        float posY = 0.0f;
        if (idx <= 2) posY = 0.72f;
        else posY = 0.28f;

        auto roomItem = createRoomItem(room);
        auto button = (Button*)roomItem->getChildByName("Button_roomItem");
        button->removeFromParentAndCleanup(false);
        if (nullptr != roomItem) {
            button->setPosition(Vec2(roomItemLayout->getContentSize().width * posX, roomItemLayout->getContentSize().height * posY));
            roomItemLayout->addChild(button, 3);
        }
    }

    // 添加子页面进入列表中
    _pageViewRooms->addPage(roomItemLayout);

    // 拖动监听
    _pageViewRooms->addEventListener(PageView::ccPageViewCallback(CC_CALLBACK_2(GameRoom::pageViewMoveCallBack, this)));
}

Node* GameRoom::createRoomItem(ComRoomInfo* roomInfo) {
    auto roomItemNode = CSLoader::createNode(ROOM_ITEM_UI);
    if (nullptr == roomItemNode) return nullptr;
    roomItemNode->setCascadeOpacityEnabled(true);
    auto roomItem = (Button*)roomItemNode->getChildByName("Button_roomItem");
    auto size = roomItem->getContentSize();
    if (nullptr == roomItem) return nullptr;

    char str[100] = {};
    char bStr[100] = { 0 };
    getRoomSpriteByRoomName(str, bStr, roomInfo->szGameRoomName);


    if (roomInfo->uNameID == 33003107 || roomInfo->uNameID == 33003108 || roomInfo->uNameID == 33003109) {
        auto biLi = static_cast<Sprite*>(roomItem->getChildByName("caijin"));
        biLi->setVisible(true);

        auto biLiG = CSLoader::createNode("common/animation/room_icon/handsel/handsel.csb");
        biLiG->setPosition(Vec2(biLi->getContentSize().width*0.5f, biLi->getContentSize().height*0.5f));
        biLi->addChild(biLiG);

        auto antActionc = CSLoader::createTimeline("common/animation/room_icon/handsel/handsel.csb");
        biLi->runAction(antActionc);

        antActionc->play("handsel", true);
    }

    roomItem->setPressedActionEnabled(false);
    roomItem->ignoreContentAdaptWithSize(false);
    roomItem->setTouchEnabled(true);

    roomItem->setUserData(roomInfo);
    roomItem->loadTextureNormal(str);
    roomItem->loadTexturePressed(str);
    roomItem->addTouchEventListener(CC_CALLBACK_2(GameRoom::enterRoomEventCallBack, this));

    auto globLimit = static_cast<TextAtlas*>(roomItem->getChildByName("text"));
    globLimit->setString(to_string(roomInfo->iLessPoint / 10000));

    string s0 = GBKToUtf8("新手场");
    string s1 = GBKToUtf8("初级场");
    string s2 = GBKToUtf8("中级场");
    string s3 = GBKToUtf8("高级场");
    string s4 = GBKToUtf8("超级场");
    string s5 = GBKToUtf8("至尊场");
    const char * path = nullptr;
    char *name="";
    if (std::strcmp(ForceGBKToUtf8(roomInfo->szGameRoomName), s0.c_str()) == 0) {
        path = CSB_XINSHOUCHANG_AM;
        name = "xinshou";
    }
    if (std::strcmp(ForceGBKToUtf8(roomInfo->szGameRoomName), s1.c_str()) == 0) {
        path = CSB_CHUJICHANG_AM;
        name = "chuji";
    }
    if (std::strcmp(ForceGBKToUtf8(roomInfo->szGameRoomName), s2.c_str()) == 0) {
        path = CSB_ZHONGJICHANG_AM;
        name = "zhongji";
    }
    if (std::strcmp(ForceGBKToUtf8(roomInfo->szGameRoomName), s3.c_str()) == 0) {
        path = CSB_GAOJICHANG_AM;
        name = "gaoji";
    }
    if (std::strcmp(ForceGBKToUtf8(roomInfo->szGameRoomName), s4.c_str()) == 0) {
        path =CSB_CHAOJICHANG_AM;
        name = "chaoji";
    }
    if (std::strcmp(ForceGBKToUtf8(roomInfo->szGameRoomName), s5.c_str()) == 0) {
        path = CSB_ZHIZUNCHANG_AM;
        name = "zhizun";
    }

    auto csb = CSLoader::createNode(path);
    csb->setPosition(Vec2(size.width*0.5f,size.height*0.5f ));
    roomItem->addChild(csb);

    auto antAction = CSLoader::createTimeline(path);
    roomItem->runAction(antAction);

    antAction->play(name, true);


    return roomItemNode;
}

void GameRoom::getRoomSpriteByRoomName(char name[], char bili[],char* gameName) {
    char str[100] = { 0 };
    char bStr[100] = { 0 };
    string tmpName = ForceGBKToUtf8(gameName);

    string s0 = GBKToUtf8("新手场");
    string s1 = GBKToUtf8("初级场");
    string s2 = GBKToUtf8("中级场");
    string s3 = GBKToUtf8("高级场");
    string s4 = GBKToUtf8("超级场");
    string s5 = GBKToUtf8("至尊场");

    if (std::strcmp(tmpName.c_str(), s0.c_str()) == 0) {
        sprintf(str, "%s", "platform/room/tongyong/xinshouchang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishing/gaojichang.png");
    } else if (std::strcmp(tmpName.c_str(), s1.c_str())==0) {
        sprintf(str, "%s", "platform/room/tongyong/chujichang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishing/chujichang.png");
    } else if (std::strcmp(tmpName.c_str(), s2.c_str()) == 0) {
        sprintf(str, "%s", "platform/room/tongyong/zhongjichang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishing/zhongjichang.png");
    } else if (std::strcmp(tmpName.c_str(), s3.c_str()) == 0) {
        sprintf(str, "%s", "platform/room/tongyong/gaojichang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishing/gaojichang.png");
    } else if (std::strcmp(tmpName.c_str(), s4.c_str()) == 0) {
        sprintf(str, "%s", "platform/room/tongyong/chaojichang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishing/chaojichang.png");
    } else if (std::strcmp(tmpName.c_str(), s5.c_str()) == 0) {
        sprintf(str, "%s", "platform/room/tongyong/zhizunchang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishing/zhizunchang.png");
    } else {
        sprintf(str, "%s", "platform/room/tongyong/xinshouchang.png");
        sprintf(bStr, "%s", "platform/lobbyUi/PlatformUi_TOP/fishing/gaojichang.png");
    }
    strcpy(name, str);
    strcpy(bili, bStr);
}

void GameRoom::pageViewMoveCallBack(Ref* pSender, PageView::EventType type) {
    if (_currentPageIdx == _pageViewRooms->getCurrentPageIndex()) return;
    _currentPageIdx = _pageViewRooms->getCurrentPageIndex();

    auto iter = _pagesPrompt.begin();
    for (int i = 0; iter != _pagesPrompt.end(); iter++, i++) {
        auto page = (Sprite*)*iter;
        page->setColor(Color3B(150, 150, 150));
        page->setScale(0.8f);
        if (i == _currentPageIdx) {
            page->setScale(1.0f);
            page->setColor(Color3B(255, 255, 255));
        }
    }
}

//子页面按钮回调
void GameRoom::enterRoomEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (!_isTouch)
        return;

    _currentSelectedRoom = dynamic_cast<Button*>(pSender);
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        _currentSelectedRoom->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED:
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {

        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

        _isTouch = false;
        this->runAction(Sequence::create(DelayTime::create(15.0f), CCCallFunc::create([=]() {
            _isTouch = true;
        }), nullptr));

        _currentSelectedRoom->setColor(Color3B(255, 255, 255));

        ComRoomInfo* roomInfo = static_cast<ComRoomInfo*>(_currentSelectedRoom->getUserData());

        // 参数校验
        CCAssert(nullptr != roomInfo, "room is nullptr!");
        if (nullptr == roomInfo) {
            _isTouch = true;
            return;
        }

        _overdueTime = false;
        if ((roomInfo->dwRoomRule & GRR_CONTEST) || (roomInfo->dwRoomRule & GRR_TIMINGCONTEST)) {
            //获取比赛信息
            MSG_GP_GetContestRoomID pData;
            pData.iGameID = roomInfo->uNameID;
            pData.iUserID = PlatformLogic()->loginResult.dwUserID;
            pData.iContestID = roomInfo->iContestID;
            pData.iUpPeople = roomInfo->iUpPeople;

            if (roomInfo->dwRoomRule & GRR_TIMINGCONTEST) {
                //获取定时赛开始时间
                PlatformLogic()->sendData(MDM_GP_GET_TIMINGMATCH_TIME, ASS_GP_GET_TIMINGMATCH_TIME, (void*)&pData, sizeof(pData),
                                          HN_SOCKET_CALLBACK(GameRoom::contestBeginTime, this));
            }

            //初始化报名界面消息
            PlatformLogic()->sendData(MDM_GP_GET_CONTEST_ROOMID, 0, (void*)&pData, sizeof(pData), HN_SOCKET_CALLBACK(GameRoom::contestInit, this));
            return;
        }
        _roomLogic->start();
        _roomLogic->requestLogin(roomInfo->uRoomID);
        _roomID = roomInfo->uRoomID;

    }
    break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        _currentSelectedRoom->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
void GameRoom::onRoomLoginCallback(bool success, const std::string& message, UINT roomID) {
    if (success) {
        HNLOG_WARNING("the user enters a room complete message!");

        ComRoomInfo* roomInfo = nullptr;
        if (_fastEnterMatch) {
            _fastEnterMatch = false;
            roomInfo = RoomInfoModule()->getByRoomID(getTimeMatchRoomID());
        } else {
            roomInfo = RoomInfoModule()->getByRoomID(_roomID);
        }

        RoomLogic()->setSelectedRoom(roomInfo);
        RoomLogic()->setRoomRule(roomInfo->dwRoomRule);

        if ((RoomLogic()->getRoomRule() & GRR_CONTEST) || (RoomLogic()->getRoomRule() & GRR_TIMINGCONTEST)) {	// 定时淘汰比赛场
            _roomLogic->stop();
            if (_overdueTime) {
                _isTouch = true;
                _overdueTime = false;
                GamePromptLayer::create()->showPrompt(GBKToUtf8("定时赛已过期"));
                RoomLogic()->closeRoom();
                return;
            }
            GameMatch::createMatch();
        } else if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME) {	// 排队机
            LoadingLayer::createLoading(this, gFontConfig_22, GetText("LODING_REQUEST_ROOMLIST"), "platform/loading.png");
            // 进入排队游戏
            _roomLogic->requestJoinQueue();
        } else {											// 金币场不扣积分
            if (GameCreator()->getCurrentGameType() == HNGameCreator::NORMAL) {
                _roomLogic->stop();
                if (nullptr != onRunActionCallBack) {
                    onRunActionCallBack();
                }
                this->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([&]() {
                    if (nullptr != onEnterDeskCallBack) {
                        _isTouch = true;
                        LoadingLayer::createLoading(this, gFontConfig_22, GetText("LODING_REQUEST_ROOMLIST"), "platform/loading.png");
                        onEnterDeskCallBack(RoomLogic()->getSelectedRoom());
                    }
                    this->getParent()->removeFromParent();
                }), nullptr));
            } else if (GameCreator()->getCurrentGameType() == HNGameCreator::BR) {
                LoadingLayer::createLoading(this, gFontConfig_22, GetText("LODING_REQUEST_ROOMLIST"), "platform/loading.png");
                _roomLogic->requestSit(0, 0);
            } else if (GameCreator()->getCurrentGameType() == HNGameCreator::SINGLE) {
                LoadingLayer::createLoading(this, gFontConfig_22, GetText("LODING_REQUEST_ROOMLIST"), "platform/loading.png");
                for (int i = 0; i < roomInfo->uDeskCount; i++) {
                    std::vector<UserInfoStruct*> deskUsers;
                    HNUserInfoModule::getInstance()->findGameUsers(i, deskUsers);
                    if (deskUsers.empty()) {
                        _roomLogic->requestSit(i, 0);
                        break;
                    }
                }
            } else {

            }
        }
    } else {
        _isTouch = true;
        RoomLogic()->closeRoom();
        GamePromptLayer::create()->showPrompt(message);
    }
}

void GameRoom::onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) {
    LoadingLayer::removeLoading(this);
    _isTouch = true;
    if (success) {
        _roomLogic->stop();
        if (INVALID_DESKNO != deskNo && INVALID_DESKSTATION != seatNo) {
            // 启动游戏
            bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, deskNo, true);
            if (!ret) {
                GamePromptLayer::create()->showPrompt(GBKToUtf8("游戏启动失败。"));
            }
        }
    } else {
        auto prompt = GamePromptLayer::create();
        prompt->showPrompt(GBKToUtf8(message.c_str()));
        prompt->setOkayCallBack([=]() {
            if (GameCreator()->getCurrentGameType() == HNGameCreator::SINGLE ||
                    GameCreator()->getCurrentGameType() == HNGameCreator::BR) {
                RoomLogic()->closeRoom();
                _roomLogic->stop();
            }
        });
    }
}

void GameRoom::onRoomDisConnect(const std::string &message) {
    auto prompt = GamePromptLayer::create();
    prompt->showPrompt(GBKToUtf8(message.c_str()));
    prompt->setOkayCallBack([this]() {
        RoomLogic()->closeRoom();
        PlatformLogic()->closePlatform();

        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
        //StateManager::getInstance()->setState(MenuState::create());
        //GameMenu::createMenu();
    });
}

//更新房间人数
void GameRoom::updateRoomPeopleCount(UINT roomID, UINT userCount) {
    //Vector<Layout*> pages = _pageViewRooms->getPages();
    //char buffer[64];
    //for (auto iter = pages.begin(); iter != pages.end(); ++iter)
    //{
    //	Vector<Node*> childs = (*iter)->getChildren();
    //	for (auto iter1 = childs.begin(); iter1 != childs.end(); ++iter1)
    //	{
    //		auto roomButton = dynamic_cast<Button*>(*iter1);
    //		auto Label_PeopleCount = dynamic_cast<Text*>(Helper::seekWidgetByName(roomButton, "Text_PeopleCount"));

    //		if (nullptr != roomButton)
    //		{
    //			ComRoomInfo* pRoom = static_cast<ComRoomInfo*>(roomButton->getUserData());
    //			if (pRoom->uRoomID == roomID)
    //			{
    //				pRoom->uPeopleCount = userCount;
    //				if (nullptr != Label_PeopleCount)
    //				{
    //					//sprintf(buffer, "%u人", pRoom->uPeopleCount);
    //					//Label_PeopleCount->setString(GBKToUtf8(buffer));
    //				}
    //				break;
    //			}
    //		}
    //	}
    //}
}

void GameRoom::fastEnterTimeMatch(UINT roomID) {
    _roomLogic->start();
    _roomLogic->requestLogin(roomID);
    _roomID = roomID;
}

UINT GameRoom::getTimeMatchRoomID() {
    char str[64] = {0};
    for (int i = 0; i < 10; i++) {
        sprintf(str, "MARK_%d", i);
        if (UserDefault::getInstance()->getIntegerForKey(str) == 3) {
            sprintf(str, "RoomID_%d", i);
            UINT roomID = UserDefault::getInstance()->getIntegerForKey(str);
            _fastEnterMatch = true;
            return roomID;
        }
    }
    return 255;
}

bool GameRoom::JudgeMatchRoom(UINT upPeople, UINT roomRule) {
    if (!((roomRule & GRR_CONTEST) || (roomRule & GRR_TIMINGCONTEST)))			return false;

    if (roomRule & GRR_TIMINGCONTEST)	return false;

    for (auto it = _vecContestRoom.begin(); it != _vecContestRoom.end(); it++) {
        if (*it == upPeople && *(++it) == roomRule)	return true;
    }

    //_vecContestRoom.push_back(upPeople);
    //_vecContestRoom.push_back(roomRule);

    return false;
}

bool GameRoom::contestInitError(HNSocketMessage* socketMessage) {
    GamePromptLayer::create()->showPrompt(GBKToUtf8("比赛已经开始，请进入其它房间！"));
    _isTouch = true;
    RoomLogic()->closeRoom();
    return true;
}

bool GameRoom::contestInit(HNSocketMessage* socketMessage) {
    CHECK_SOCKET_DATA_RETURN(DL_GetContestRoomID, socketMessage->objectSize, true, "DL_GetContestRoomID size is error");
    auto pData = (DL_GetContestRoomID*)socketMessage->object;

    _roomLogic->start();
    _roomLogic->requestLogin(pData->iRoomID);
    _roomID = pData->iRoomID;

    UserDefault::getInstance()->setIntegerForKey("iConstestNum", pData->iConstestNum);
    UserDefault::getInstance()->setIntegerForKey("iChampionCount", pData->iChampionCount);
    UserDefault::getInstance()->setIntegerForKey("iBestRank", pData->iBestRank);
    UserDefault::getInstance()->setIntegerForKey("iJoinCount", pData->iJoinCount);
    UserDefault::getInstance()->setIntegerForKey("iEntryFee", pData->iEntryFee);
    UserDefault::getInstance()->setIntegerForKey("iRankAward1", pData->iRankAward[0]);
    UserDefault::getInstance()->setIntegerForKey("iRankAward2", pData->iRankAward[1]);
    UserDefault::getInstance()->setIntegerForKey("iRankAward3", pData->iRankAward[2]);
    UserDefault::getInstance()->setIntegerForKey("iAwardType1", pData->iAwardType[0]);
    UserDefault::getInstance()->setIntegerForKey("iAwardType2", pData->iAwardType[1]);
    UserDefault::getInstance()->setIntegerForKey("iAwardType3", pData->iAwardType[2]);
    UserDefault::getInstance()->flush();

    return true;
}

void GameRoom::cleanTimeMatchData() {
    char str[64] = {0};
    for (int i = 1; i < 10; i++) {
        sprintf(str, "MARK_%d", i);
        if (UserDefault::getInstance()->getIntegerForKey(str) == 1 || UserDefault::getInstance()->getIntegerForKey(str) == 4) {	//没有参赛的数据
            UserDefault::getInstance()->setIntegerForKey(str, 0);
            sprintf(str, "beginTime_%d", i);
            UserDefault::getInstance()->setStringForKey(str, "");
            continue;
        }

        sprintf(str, "beginTime_%d", i);
        std::string beginTime = UserDefault::getInstance()->getStringForKey(str);
        time_t bTime = 0;
        for (auto iter = beginTime.begin(); iter != beginTime.end(); iter++) {
            bTime = bTime*10 + *iter - '0';
        }
        time_t nTime = time(0);
        if (nTime + 7 * 3600 > bTime) {		//开赛时间过期超过1小时（留时间断线重连）
            UserDefault::getInstance()->setStringForKey(str, "");
            sprintf(str, "MARK_%d", i);
            UserDefault::getInstance()->setIntegerForKey(str, 0);
        }
    }
    UserDefault::getInstance()->setStringForKey("DXCL", "");
    UserDefault::getInstance()->flush();
}

bool GameRoom::contestBeginTime(HNSocketMessage* socketMessage) {
    CHECK_SOCKET_DATA_RETURN(MSG_GP_GETMatchBeginTime, socketMessage->objectSize, true, "MSG_GP_GETMatchBeginTime size is error");
    auto pData = (MSG_GP_GETMatchBeginTime*)socketMessage->object;

    if (RoomLogic()->getRoomRule() & GRR_CONTEST)			return false;

    cleanTimeMatchData();

    LLONG t = pData->I64BeginTime + 8 * 3600;
    std::string BeginTime;
    while (t) {
        char c = t % 10 + '0';
        BeginTime = c + BeginTime;
        t = t / 10;
    }

    time_t bTime = pData->I64BeginTime;
    if (bTime < time(0)) {
        char str[64] = { 0 };
        bool isJoined = false;
        for (int i = 1; i < 10; i++) {
            sprintf(str, "MARK_%d", i);
            if (UserDefault::getInstance()->getIntegerForKey(str) == 5) {
                sprintf(str, "beginTime_%d", i);
                if (UserDefault::getInstance()->getStringForKey(str) == BeginTime) {
                    isJoined = true;
                    break;
                }
            }
        }

        if (isJoined) {
            UserDefault::getInstance()->setStringForKey("DXCL", BeginTime);
            UserDefault::getInstance()->flush();
            return true;
        } else {
            _overdueTime = true;
            return false;
        }
    }

    char mark[64] = { 0 };
    std::string name = "";
    for (int i = 1; i < 10; i++) {
        sprintf(mark, "MARK_%d", i);
        if (UserDefault::getInstance()->getIntegerForKey(mark) == 0) {
            UserDefault::getInstance()->setIntegerForKey(mark, 1);
            name = "beginTime_";
            name += (i + '0');
            break;
        }
    }
    UserDefault::getInstance()->setStringForKey(name.c_str(), BeginTime);
    UserDefault::getInstance()->flush();
    return true;
}
