#include "HNLobby/GameDesk.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNUIExport.h"
#include "GameDeskUserInfoLayer.h"
#include "CommonMacro.h"
#include "HNLobby/globel.h"

//////////////////////////////////////////////////////////////////////////
//static const char* DESK_ITEM_UI = "platform/DeskList/deskListUi.csb";

//static const char* GAME_PAGESPROMPT_PATH = "platform/gameList/gameItemRes/page_Prompt.png";
static const char* DESK_LISTVIEW_CSB = "csb/platform/DeskList/deskListUi.csb";
static const char* DESK_SIXREN_DESKNODE_PATH = "csb/platform/DeskList/duoren.csb";
static const char* DESK_TWOREN_DESKNODE_CSB = "csb/platform/DeskList/two.csb";
static const char* DESK_FOURREN_DESK_NODE_CSB = "csb/platform/DeskList/four.csb";

static const Size pageViewDesksSize(1280, 500);

//////////////////////////////////////////////////////////////////////////

GameDesk* GameDesk::createDesk(ComRoomInfo* roomInfo) {
    GameDesk* deskList = new GameDesk();
    if (deskList->initData(roomInfo)) {
        deskList->autorelease();
        return deskList;
    } else {
        CC_SAFE_DELETE(deskList);
        return nullptr;
    }
}

GameDesk::GameDesk()
    : _currentSelectedDesk(nullptr)
    , _deskNO(0)
    , _pageLen(0)
    , _canCreate(true)
    , _pageEven(false)
    , _currentPageIdx(0)
    , _idx(0)
    , _isTouch(true)
    , _defoutLoadCount(0)
    , _layoutSize(Size(0,0))
    , _curGameDeskCsbPath(0)
    , _scheduleIdx(0) {
    _vecdeskNode.clear();
    _vecUserSit.clear();

    _deskLogic = new HNRoomDeskLogic(this);
    _deskLogic->start();
}

GameDesk::~GameDesk() {
    _deskLogic->stop();
    CC_SAFE_DELETE(_deskLogic);
}

void GameDesk::closeDeskList() {
    _deskLogic->stop();
    RoomLogic()->closeRoom();
    this->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([&]() {
        if (nullptr != onCloseCallBack) {
            onCloseCallBack();
        }
        this->getParent()->removeFromParent();
    }), nullptr));
}

bool GameDesk::initData(ComRoomInfo* roomInfo) {
    if (!HNLayer::init()) return false;
    _deskCount = roomInfo->uDeskCount;
    _playerCount = roomInfo->uDeskPeople;
    this->setCascadeOpacityEnabled(true);

    createDeskList(roomInfo);

    return true;
}

void GameDesk::onExit() {
    _deskLogic->stop();
    HNLayer::onExit();
}

// 创建牌桌列表
void GameDesk::createDeskList(ComRoomInfo* roomInfo) {
    _roomInfo = roomInfo;

    _root = CSLoader::createNode(DESK_LISTVIEW_CSB);
    _root->setAnchorPoint(Vec2(0, 0));
    _root->setPosition(0, 0);
    this->addChild(_root, 3);


    loadListView();
    loadSlider();
    loadUserRoomIfon(_roomInfo);
    setDefoutValueByMaxPlayer(_roomInfo->uDeskPeople);

    int max = _defoutLoadCount;
    int iLoop = 0;

    if (_defoutLoadCount >= _roomInfo->uDeskCount) {
        max = _roomInfo->uDeskCount;
        if (max % CURGAME_DESK_COUNT_ROW > 0) {
            iLoop = max / CURGAME_DESK_COUNT_ROW + 1;
        } else {
            iLoop = max % CURGAME_DESK_COUNT_ROW;
        }
    } else {
        iLoop = _defoutLoadCount / CURGAME_DESK_COUNT_ROW;
    }

    int row = CURGAME_DESK_COUNT_ROW;

    for (int i = 0; i < iLoop; i++) {
        auto layout = Layout::create();
        layout->setContentSize(_layoutSize);
        if (i == iLoop - 1) {
            row = _defoutLoadCount - i* CURGAME_DESK_COUNT_ROW;
        }
        for (int j = 0; j < row; j++) {
            if (_idx == _roomInfo->uDeskCount) break;
            auto deskNode = CSLoader::createNode(_curGameDeskCsbPath);
            auto deskIdText = static_cast<Text*>((deskNode->getChildByName("deskId")));
            Size deskSize  = static_cast<ImageView*>((deskNode->getChildByName("bg")))->getContentSize();
            deskNode->setUserData(_deskinfos.at(_idx));
            deskIdText->setString(to_string(_idx + 1));
            deskNode->setPositionX(j * (deskSize.width / 1280.0f * WIN_SIZE.width + 75.0f / 1280.0f * WIN_SIZE.width) + 100.0f / 1280.0f *WIN_SIZE.width);
            auto btnNode = deskNode->getChildByName("btnNode");
            {
                int tag = 0;
                for (auto key : btnNode->getChildren()) {
                    auto button = static_cast<Button*>(key->getChildByName("btn"));

                    _deskinfos.at(_idx)->seatID = tag;
                    button->setTag(tag);
                    button->setUserData(_deskinfos.at(_idx));
                    button->setSwallowTouches(false);
                    button->addTouchEventListener(CC_CALLBACK_2(GameDesk::enterTableEventCallBack, this));
                    //查找当前加载出的座子有没有玩家 有就显示头像
                    UserInfoStruct* pUser = UserInfoModule()->findUser(_idx, tag);
                    int tmpIdx = tag++;
                    if (pUser) {
                        if (pUser->bUserState == USER_PLAY_GAME) {
                            static_cast<Sprite*>(deskNode->getChildByName("play"))->setVisible(true);
                            static_cast<Sprite*>(deskNode->getChildByName("noplay"))->setVisible(false);
                        }
                        auto parent = static_cast<Sprite*>(key->getChildByName("begin"));
                        static_cast<Sprite*>(key->getChildByName("wait"))->setVisible(false);
                        parent->setVisible(true);
                        updateDeskHead(_idx, tmpIdx, parent);
                    }
                }
                layout->addChild(deskNode);
            }
            _idx++;
            _vecdeskNode.push_back(deskNode);
        }
        _listView->addChild(layout);
        _listOrignY = _listView->getInnerContainer()->getPositionY();
    }

    schedule(schedule_selector(GameDesk::scheCallBack), 0.3);
}

void GameDesk::loadListView() {
    auto size = Size(WIN_SIZE.width, WIN_SIZE.height / 5.0f * 4.0 + 10.0f / 720.0f * WIN_SIZE.height);
    _listView = static_cast<ListView*>(_root->getChildByName("listView"));
    _listView->setInnerContainerSize(size);
    _listView->setContentSize(size);
    _listView->setOpacity(100);
    _listView->addTouchEventListener(CC_CALLBACK_2(GameDesk::listViewItemCallBack, this));
}

void GameDesk::loadSlider() {
    _slider = static_cast<Slider*>(_root->getChildByName("Slider_6"));
    auto bg = static_cast<Sprite*>(_root->getChildByName("sliderBg"));
    _slider->setPositionX(WIN_SIZE.width - 30.0f / 1280 * WIN_SIZE.width);
    _slider->setPositionY(WIN_SIZE.height *0.45);
    _slider->setPercent(0);
    _slider->addEventListener(CC_CALLBACK_2(GameDesk::sliderEventCallBack, this)); //设置事件监听函数
    _slider->setRotation(90);
    bg->setPosition(_slider->getPosition());
    bg->setRotation(_slider->getRotation());
}

void GameDesk::scheCallBack(float dt) {
    if (_idx == _roomInfo->uDeskCount) {
        this->unschedule(schedule_selector(GameDesk::scheCallBack));

        return;
    }
    loadListViewItem(_idx);
}

void GameDesk::loadListViewItem(UINT &idx) {
    UINT curloadItem = _roomInfo->uDeskCount - idx;
    auto noLoadCount = curloadItem >= CURGAME_DESK_COUNT_ROW ? CURGAME_DESK_COUNT_ROW : curloadItem;
    auto layout = Layout::create();
    layout->setContentSize(_layoutSize);
    for (int j = 0; j < noLoadCount; j++) {
        auto deskNode = CSLoader::createNode(_curGameDeskCsbPath);
        deskNode->setUserData(_deskinfos.at(idx));
        auto deskIdText = static_cast<Text*>((deskNode->getChildByName("deskId")));
        Size deskSize = static_cast<ImageView*>((deskNode->getChildByName("bg")))->getContentSize();
        deskIdText->setString(to_string(idx + 1));
        deskNode->setPositionX(j * (deskSize.width / 1280.0f * WIN_SIZE.width + 75.0f / 1280.0f * WIN_SIZE.width) + 100.0f / 1280.0f *WIN_SIZE.width);
        auto btnNode = deskNode->getChildByName("btnNode");
        {
            int tag = 0;
            for (auto key : btnNode->getChildren()) {
                auto button = static_cast<Button*>(key->getChildByName("btn"));
                _deskinfos.at(idx)->seatID = tag;
                button->setTag(tag);
                button->setSwallowTouches(false);
                button->setUserData(_deskinfos.at(idx));
                button->addTouchEventListener(CC_CALLBACK_2(GameDesk::enterTableEventCallBack, this));
                UserInfoStruct* pUser = UserInfoModule()->findUser(idx, tag);
                int tmpIdx = tag++;
                if (pUser) {
                    if (pUser->bUserState == USER_PLAY_GAME) {
                        static_cast<Sprite*>(deskNode->getChildByName("play"))->setVisible(true);
                        static_cast<Sprite*>(deskNode->getChildByName("noplay"))->setVisible(false);
                    }
                    auto parent = static_cast<Sprite*>(key->getChildByName("begin"));
                    static_cast<Sprite*>(key->getChildByName("wait"))->setVisible(false);
                    parent->setVisible(true);
                    updateDeskHead(idx, tmpIdx, parent);
                }
            }
            layout->addChild(deskNode);
            idx++;
            _vecdeskNode.push_back(deskNode);
        }
    }
    _listView->addChild(layout);
    _listView->forceDoLayout();
    _listOrignY = _listView->getInnerContainer()->getPositionY();
}

void GameDesk::setDefoutValueByMaxPlayer(UINT maxPlayer) {
    switch (maxPlayer) {
    case 2: {
        _curGameDeskCsbPath = DESK_TWOREN_DESKNODE_CSB;
        _layoutSize = Size(WIN_SIZE.width, WIN_SIZE.height / 3.8);
        _defoutLoadCount = _roomInfo->uDeskCount >= DEFOUT_LOAD_ITEM_COUNT_TWO ? DEFOUT_LOAD_ITEM_COUNT_TWO : _roomInfo->uDeskCount;
        break;
    }
    case 4: {
        _curGameDeskCsbPath = DESK_FOURREN_DESK_NODE_CSB;
        _layoutSize = Size(WIN_SIZE.width, WIN_SIZE.height / 1.65);
        _defoutLoadCount = _roomInfo->uDeskCount >= DEFOUT_LOAD_ITEM_COUNT_OTHER ? DEFOUT_LOAD_ITEM_COUNT_OTHER : _roomInfo->uDeskCount;
        break;
    }
    default:
        _curGameDeskCsbPath = DESK_SIXREN_DESKNODE_PATH;
        _defoutLoadCount = _roomInfo->uDeskCount >= DEFOUT_LOAD_ITEM_COUNT_OTHER ? DEFOUT_LOAD_ITEM_COUNT_OTHER : _roomInfo->uDeskCount;
        _layoutSize = Size(WIN_SIZE.width, WIN_SIZE.height / 1.65);
        break;
    }
}

void GameDesk::loadUserRoomIfon(ComRoomInfo* roomInfo) {
    for (int i = 0; i < roomInfo->uDeskCount; i++) {
        createDeskPageInfo(_roomInfo);
        vector<UserInfoStruct*>_vecUser;
        _vecUser.clear();
        UserInfoModule()->findDeskUsers(_deskNO, _vecUser);

        for (auto k : _vecUser) {
            MSG_GR_R_UserSit  tmp;
            tmp.bLock = false;
            tmp.bDeskIndex = k->bDeskNO;
            tmp.bDeskStation = k->bDeskStation;
            tmp.bUserState = k->bUserState;
            tmp.dwUserID = k->dwUserID;
            tmp.bIsDeskOwner = 255;
            _vecUserSit.push_back(tmp);
        }
        _deskNO++;
    }
}

void GameDesk::sliderEventCallBack(Ref * pSender, Slider::EventType type) {
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
        _listView->jumpToPercentVertical(_slider->getPercent());
        if (_slider->getPercent() == 100) {

        }
    }
}

void GameDesk::listViewItemCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
    switch (type) {
    case Widget::TouchEventType::MOVED: {
        int percent = _listView->getInnerContainer()->getPositionY() / _listOrignY * 100;
        _slider->setPercent(100 - percent);
        break;
    }
    default:
        break;
    }
}

// 创建牌桌页面数据
void GameDesk::createDeskPageInfo(ComRoomInfo* roomInfo) {
    // 获取桌子玩家信息
    std::vector<UserInfoStruct*> deskUsers;
    UserInfoModule()->findDeskUsers(_deskNO, deskUsers);

    DeskInfo* desk = new DeskInfo;
    memset(desk, 0x0, sizeof(desk));

    if (nullptr != desk) {
        desk->deskID				= _deskNO;
        desk->multiple				= roomInfo->iBasePoint;
        desk->goldMin				= roomInfo->iLessPoint;
        desk->goldMax				= roomInfo->iMaxPoint;
        desk->peopleValue		= deskUsers.size();
        desk->peopleMax			= roomInfo->uDeskPeople;
        BYTE value = RoomLogic()->deskStation.bVirtualDesk[desk->deskID/8]&(1<<(desk->deskID%8));
        if (0 != value) {
            desk->peopleValue	= roomInfo->uDeskPeople;
        }
    }
    _deskinfos.push_back(desk);
}

// 进入游戏桌点击回调
void GameDesk::enterTableEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (_isTouch == false) return;

    _currentSelectedDesk = dynamic_cast<Button*>(pSender);
    _selectorBtnIdx = _currentSelectedDesk->getTag();

    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        _currentSelectedDesk->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED:
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        _isTouch = false;

        this->runAction(Sequence::create(DelayTime::create(3.0f), CCCallFunc::create([=]() {
            _isTouch = true;
        }), nullptr));

        _currentSelectedDesk->setColor(Color3B(255, 255, 255));

        DeskInfo* deskInfo = static_cast<DeskInfo*>(_currentSelectedDesk->getUserData());

        // 参数校验
        CCAssert(nullptr != deskInfo, "desk is nullptr!");
        if (nullptr == deskInfo) {
            _isTouch = true;
            return;
        }

        if (deskInfo->peopleValue == deskInfo->peopleMax) {
            //GamePromptLayer::create()->showPrompt(GBKToUtf8("桌子人数已满。"));
            _isTouch = true;
            return;
        }

        _deskLogic->requestSit(deskInfo->deskID, _selectorBtnIdx);
        this->unschedule(schedule_selector(GameDesk::scheCallBack));
    }
    break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        _currentSelectedDesk->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }
}

void GameDesk::onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) {
    _isTouch = true;
    if(success) {
        ComRoomInfo* pRoomInfo = RoomLogic()->getSelectedRoom();
        if(nullptr == pRoomInfo) {
            GamePromptLayer::create()->showPrompt(GBKToUtf8("查找房间失败。"));
            return;
        }
        bool bRet = GameCreator()->startGameClient(pRoomInfo->uNameID, deskNo, true);
        if (!bRet) {
            GamePromptLayer::create()->showPrompt(GBKToUtf8("游戏启动失败。"));
        }
    } else {
        GamePromptLayer::create()->showPrompt(message);
    }
}

void GameDesk::onRoomDeskLock(BYTE deskNo) {
    //updateDeskPeopleCount(deskNo, true);
    //updateDeskView(deskNo, false);
}

void GameDesk::onDeskState(MSG_GR_DeskStation * pData) {

}

void GameDesk::onDeskState(BYTE bDeskIndex)
{
	if (_idx > bDeskIndex) {
		auto deskNode = _vecdeskNode.at(bDeskIndex);
		if (nullptr != deskNode) {
			DeskInfo* info = static_cast<DeskInfo*>(deskNode->getUserData());

			if (info->deskID == bDeskIndex) {
				info->peopleValue = info->peopleMax;
			}
			else {
				std::vector<UserInfoStruct*> deskUsers;
				UserInfoModule()->findDeskUsers(info->deskID, deskUsers);
				info->peopleValue = deskUsers.size();
			}
		}

		auto node = deskNode->getChildByName("btnNode");
		auto statePlay = static_cast<Sprite*>(deskNode->getChildByName("play"));
		auto stateNoPlay = static_cast<Sprite*>(deskNode->getChildByName("noplay"));

		vector<UserInfoStruct*> vecUserInfoStruct; //这里通过下发的桌子号 去查找判断当前桌玩家数是否 > 1 进去肯定大于一但是也得判断 呵呵   获取当前桌其他玩家状态 显示 桌子UI的状态切换
		vecUserInfoStruct.clear();
		UserInfoModule()->findDeskUsers(bDeskIndex, vecUserInfoStruct);


		if (vecUserInfoStruct.size() >= 1) {
			for (auto user : vecUserInfoStruct) { //  如果房间没有其他玩家 进入游戏返回的游戏状态为 2 坐下桌子状态 暂时没办法处理游戏开始状态
				if (user->bUserState == USER_PLAY_GAME) {
					statePlay->setVisible(true);
					stateNoPlay->setVisible(false);
					break;
				}
				else if (user->bUserState == USER_CUT_GAME) {
					statePlay->setVisible(true);
					stateNoPlay->setVisible(false);
				}
				else {
					statePlay->setVisible(false);
					stateNoPlay->setVisible(true);
				}
			}
		}
	}
}

void GameDesk::onRoomDeskUnLock(BYTE deskNo) {
//	updateDeskView(deskNo, false);
    //updateDeskPeopleCount(deskNo, false);
}

void GameDesk::onRoomDeskUserCountChanged(MSG_GR_R_UserSit * pData) {
    updateDeskView(pData, false);
}

void  GameDesk::updateDeskView(MSG_GR_R_UserSit* pData, bool isLock) { // 值错误
    if (_idx > pData->bDeskIndex) {
        auto deskNode = _vecdeskNode.at(pData->bDeskIndex);
        if (nullptr != deskNode) {
            DeskInfo* info = static_cast<DeskInfo*>(deskNode->getUserData());

            if (info->deskID == pData->bDeskIndex && isLock) {
                info->peopleValue = info->peopleMax;
            } else {
                std::vector<UserInfoStruct*> deskUsers;
                UserInfoModule()->findDeskUsers(info->deskID, deskUsers);
                info->peopleValue = deskUsers.size();
            }
        }
        upDateDeskFilterFunciton(pData, deskNode);
    }
}

//处理玩家以不同形式进入椅子层（包括断线）
void GameDesk::upDateDeskFilterFunciton(MSG_GR_R_UserSit* pData, Node*desk) {
    auto node = desk->getChildByName("btnNode");
    auto seat = static_cast<Button*>(node->getChildByName(to_string((int)(pData->bDeskStation))));
    auto wait = static_cast<Sprite*>(seat->getChildByName("wait"));
    auto begin = static_cast<Sprite*>(seat->getChildByName("begin"));
    auto btn = static_cast<Sprite*>(seat->getChildByName("begin"));

    auto statePlay = static_cast<Sprite*>(desk->getChildByName("play"));
    auto stateNoPlay = static_cast<Sprite*>(desk->getChildByName("noplay"));

    vector<UserInfoStruct*> vecUserInfoStruct; //这里通过下发的桌子号 去查找判断当前桌玩家数是否 > 1 进去肯定大于一但是也得判断 呵呵   获取当前桌其他玩家状态 显示 桌子UI的状态切换
    vecUserInfoStruct.clear();
    UserInfoModule()->findDeskUsers(pData->bDeskIndex, vecUserInfoStruct);


    if (vecUserInfoStruct.size() >= 1) {
        for (auto user : vecUserInfoStruct) { //  如果房间没有其他玩家 进入游戏返回的游戏状态为 2 坐下桌子状态 暂时没办法处理游戏开始状态
            if (user->bUserState == USER_PLAY_GAME) {
                statePlay->setVisible(true);
                stateNoPlay->setVisible(false);
                break;
            } else if (user->bUserState == USER_CUT_GAME) {
                statePlay->setVisible(true);
                stateNoPlay->setVisible(false);
            } else {
                statePlay->setVisible(false);
                stateNoPlay->setVisible(true);
            }
        }
    }

    switch (pData->bUserState) {
    case USER_LOOK_STATE: {
        wait->setVisible(true);
        begin->setVisible(false);
        btn->removeChildByName("head");
    }
    break;
    case USER_SITTING: {
        wait->setVisible(false);
        begin->setVisible(true);

        updateDeskHead(pData->bDeskIndex, pData->bDeskStation, btn);
    }
    break;
    case USER_CUT_GAME: {
        wait->setVisible(true);
        begin->setVisible(false);
        btn->removeChildByName("head");
    }
    break;
    default:
        break;
    }
}

void  GameDesk::updateDeskHead(INT bDeskNo, INT bDeskStation, Sprite* parent) {
    auto head = ImageView::create();
    char str[100] = { 0 };
    UserInfoStruct* pUser = UserInfoModule()->getInstance()->findUser(bDeskNo, bDeskStation);
    sprintf(str, "common/head/%d.png", pUser->bLogoID);
    head->loadTexture(str);
    head->setScale(0.6);
    head->setName("head");
    head->setTouchEnabled(true);
    head->setSwallowTouches(true);
    head->setPosition(Vec2(parent->getContentSize().width* 0.5, parent->getContentSize().height *0.5));
    head->setRotation(-parent->getParent()->getRotation());
    parent->addChild(head, 1);
    //数据被释放
    head->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        switch (type) {
        case cocos2d::ui::Widget::TouchEventType::BEGAN: {
            auto headT = (ImageView*)sender;
            auto* seat = static_cast<Button*>(headT->getParent()->getParent()->getChildByName("btn"));
            DeskInfo*  data = (DeskInfo*)seat->getUserData();
            _deskUserInfoLayer = DeskUserInfoLayer::create();
            _deskUserInfoLayer->openLayer(this, Vec2(WIN_SIZE.width*0.5f - _deskUserInfoLayer->getWidth()*0.5f, WIN_SIZE.height*0.5f - _deskUserInfoLayer->getHight()*0.5f), 10000000);
            _deskUserInfoLayer->updateUserData(data->deskID, seat->getTag());
        }
        case cocos2d::ui::Widget::TouchEventType::MOVED:
            break;
        case cocos2d::ui::Widget::TouchEventType::ENDED:
            _deskUserInfoLayer->closelayer();
            break;
        case cocos2d::ui::Widget::TouchEventType::CANCELED:
            _deskUserInfoLayer->closelayer();
            break;
        default:
            break;
        }
    });
}

void GameDesk::quickEnterGameCallBack(INT deskNo, INT deskSetation) {
    HNAudioEngine::getInstance()->playEffect("music/button_click_sound.mp3");
    DeskInfo* deskInfo = new DeskInfo;
    memset(deskInfo, 0, sizeof(DeskInfo));
    deskInfo->deskID = deskNo;
    deskInfo->seatID = deskSetation;
    std::vector<UserInfoStruct*> users;
    UserInfoModule()->getInstance()->findDeskUsers(deskNo, users);
    deskInfo->peopleValue = users.size();
    deskInfo->peopleMax = _playerCount;
    auto button = Button::create();
    button->setTag(deskSetation);
    button->setUserData(deskInfo);

    enterTableEventCallBack(button, Widget::TouchEventType::ENDED);
}
