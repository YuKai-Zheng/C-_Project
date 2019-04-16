#include "RoomListCompositeV66.h"
#include "PlatformResV66.h"

#include "HNLobby/GameNotice.h"
#include "../FontConfig.h"
#include "../pattern/CommandManager.h"
#include "../PlatformDefine.h"
#include "../command/ShowGamePrompt.h"
#include "UI/LoadingLayer.h"
#include "UI/Base/AUIButton.h"
#include "HNLobby/GameData.h"

const char*	ROOM_ICON_PATH = "csb/newplatform/room/%d.png";
const char* ROOM_ICON_FISH_PATH = "csb/newplatform/room/fish/%d.png";
const char*	ROOM_REQUEST_GOLD_BG = "v66/room/5.png";
const char*	ROOM_REQUEST_Bg = "v66/common/commonBg/2.png";

RoomListCompositeV66::RoomListCompositeV66() {
    _roomLogic   = nullptr;
    _deskLogic   = nullptr;
    m_nRoomCount = 0;
    m_experienceItem = nullptr;
	_canClose = true;
}

RoomListCompositeV66::~RoomListCompositeV66() {
    if (_roomLogic) {
        HN_SAFE_DELETE(_roomLogic);
    }
    if (_deskLogic) {
        HN_SAFE_DELETE(_deskLogic);
    }
}

void RoomListCompositeV66::onEnter()
{
	Node::onEnter();

//	//快速加入
//	bottomMenuDelegate->quickJoinGame = [&](){
//		int iRoomID = 0;
//		iRoomID = UserDefault::getInstance()->getIntegerForKey(QUICK_JOIN_ROOM_ID);
//
//		//是否有该id房间
//		bool hasRoom = false;
//		Button* targetBtn = nullptr;
//		for (auto btn : m_vRoomItem)
//		{
//			ComRoomInfo* roomInfo = static_cast<ComRoomInfo*>(btn->getUserData());
//			if (roomInfo->uRoomID == iRoomID)
//			{
//				hasRoom = true;
//				targetBtn = btn;
//				break;
//			}
//		}
//
//		//处理异常的房间id
//		if (iRoomID <= 0 || !hasRoom)
//		{
//			if (m_vRoomItem.size() > 0)
//			{
//				targetBtn = m_vRoomItem.at(0);
//				ComRoomInfo* roomInfo = static_cast<ComRoomInfo*>(targetBtn->getUserData());
//				iRoomID = roomInfo->uRoomID;
//			}
//			else
//			{
//				CommandManager::getInstance()->showGamePrompt(GBKToUtf8("快速启动失败！"));
//				//setVisible(true);
//				return;
//			}
//		}
//
//		if (targetBtn)
//		{
//			enterRoomCallback(targetBtn, Widget::TouchEventType::ENDED);
//			//放按钮回调后面区分是否是quick join
//			UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, true);
//		}
//	};
//
//	if (UserDefault::getInstance()->getBoolForKey(IS_QUICK_JOIN))
//	{
//		//setVisible(false);
//		bottomMenuDelegate->quickJoinGame();
//	}
}
void RoomListCompositeV66::onExit()
{
    Node::onExit();
    PlatformLogic()->removeEventSelector(MDM_GP_MESSAGE, ASS_GP_GET_LEFTTIME);
    
}
bool RoomListCompositeV66::init() {
    if (!Node::init()) {

        return false;
    }


	//getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, closeBtn);
	_roomLogic = new HNRoomLogicBase(this);
	int roomid = 0;

	//房间信息
	int roomCount = RoomInfoModule()->getRoomCount();
	for (int i = 0; i < roomCount; i++) {
		auto info = RoomInfoModule()->getRoom(i);
		if (info->uNameID == 11116666)
		{
			RoomLogic()->setSelectedRoom(info);
			_roomLogic->start();
			_roomLogic->requestLogin(info->uRoomID);
			roomid = info->uRoomID;
			return true;
		}
		roomid = info->uNameID;
		m_vRoomInfo.push_back(info);
	}

	std::string strCsbPath;
	if (roomid == 33003109) { //大闹天宫单独处理
		strCsbPath = std::string("csb/newplatform/room/Node_roomlist_fish.csb"); 
	}
	else
	{
		strCsbPath = std::string("csb/newplatform/room/node_roomlist.csb");
	}

	m_bIsTouch = false;
    //加载房间列表动画;
	auto rootnode = CSLoader::createNode(strCsbPath);
    this->addChild(rootnode);
	rootnode->setPosition(WIN_SIZE / 2);
	auto blacknode = rootnode->getChildByName("bg")->getChildByName("Panel_2");
	blacknode->setScaleX(WIN_SIZE.width / 1600.0);
	blacknode->setScaleY(WIN_SIZE.height / 720.0);

	auto pSpriteBack = rootnode->getChildByName("Sprite_back");
	if (pSpriteBack != nullptr)
	{
		pSpriteBack->setPositionX(-(WIN_SIZE.width*0.5f));
	}
    
    //auto listener = EventListenerTouchOneByOne::create();
    //listener->setSwallowTouches(true);
    //listener->onTouchBegan = CC_CALLBACK_2(RoomListCompositeV66::onTouchBegan,this);
        
	auto closeBtn = (Button*)rootnode->getChildByName("bg")->getChildByName("Button_exit");
    closeBtn->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type)
	{
        if (type != Widget::TouchEventType::ENDED) 
			return;
        
        if (platformDelegate&&_canClose) 
		{
            platformDelegate->handleReturnButton();
        }

		_canClose = false;
		schedule(schedule_selector(RoomListCompositeV66::canTouchTime), 0.6);
    });

    //暂时固定5个场次
    int nRoomCount = m_vRoomInfo.size();
    m_nRoomCount = nRoomCount;
    m_jackpotModelData.clear();
    m_experienceItem = nullptr;
    for (int i = nRoomCount - 1; i >= 0; i--) {
        if (i < m_vRoomInfo.size()) {
           // char temp[64] = { 0 };
			char btnname[64] = { 0 };
			sprintf(btnname, "btn_%d", i);
            //if (roomid == 33003109) { //大闹天宫单独处理
                //sprintf(temp, ROOM_ICON_FISH_PATH, i);
            //}
           // else
           // {
              //  sprintf(temp, ROOM_ICON_PATH, i);
          //  }
            
			auto btn = (Button*)rootnode->getChildByName("bg")->getChildByName(string(btnname));
			//auto txtSprite = (Sprite*)btn->getChildByName("txt");
            if (btn) 
			{
                if (i == 0)
				{
                    m_experienceItem = btn;                
                }

				//txtSprite->setTexture(string(temp));
				btn->setUserData((void*)m_vRoomInfo.at(i));
				btn->addTouchEventListener(CC_CALLBACK_2(RoomListCompositeV66::enterRoomCallback, this));
				btn->setAlphaTouchEnable(true);
	/*			btn->addTouchEventListener([this](Ref* pSender, Widget::TouchEventType type)
				{
					if (type == Widget::TouchEventType::ENDED)
					{
					}
				});*/

				m_vRoomItem.push_back(btn);


				bool isHasJackpot = GameData::getInstance()->getGameHasJackpot(roomid);
				if (isHasJackpot) {
					auto jackpotModel = JackpotModel::create();
					if (jackpotModel) {
						jackpotModel->setPosition(Vec2(btn->getContentSize().width / 2, btn->getContentSize().height / 2 - 5));
						btn->addChild(jackpotModel);	//房间暂时不加奖池特效
						jackpotModel->setScale(0.50f);
						jackpotModel->setGameData(-1, roomid, i);
						m_jackpotModelData.push_back(jackpotModel);
						jackpotModel->setVisible(i != 0);
						jackpotModel->setVisible(false);
					}
				}


				//房间入门金额
              /* auto minGold = Text::create("", FONT_TTF_FZCY, 20);
                if (minGold) {
                    stringstream stream;
                    stream << converNumberWithWAN(m_vRoomInfo.at(i)->iLessPoint);
					minGold->setTextColor(Color4B(255, 255, 255, 255));
                    minGold->setString(stream.str());
                    minGold->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    minGold->setPosition(Vec2(btn->getContentSize().width*0.50f, minGold->getContentSize().height*3.0f));
                    btn->addChild(minGold);
                }*/
                

            }
        }
    }
    
    setPosition(Vec2(0, WIN_SIZE.height));
    
    
    MSG_GR_RS_LeftTime pTimeData;
    pTimeData.nUserID = PlatformLogic()->getInstance()->loginResult.dwUserID;
    PlatformLogic()->sendData(MDM_GP_MESSAGE, ASS_GP_GET_LEFTTIME, &pTimeData, sizeof(pTimeData), HN_SOCKET_CALLBACK(RoomListCompositeV66::roomExperienceTimeSelector, this));
    return true;
}

void RoomListCompositeV66::canTouchTime(float dt)
{
	_canClose = true;
	unschedule(schedule_selector(RoomListCompositeV66::canTouchTime));
}

bool RoomListCompositeV66::roomExperienceTimeSelector(HNSocketMessage* socketMessage)
{
    CCAssert(sizeof(MSG_GR_REP_USERLEFTTIME) == socketMessage->objectSize, "sizeof(MSG_GR_REP_USERLEFTTIME) != objectSize");
    if (sizeof(MSG_GR_REP_USERLEFTTIME) != socketMessage->objectSize)    return false;

#if 0
    MSG_GR_REP_USERLEFTTIME* msg = (MSG_GR_REP_USERLEFTTIME*)socketMessage->object;
    if (msg->nUserID == PlatformLogic()->getInstance()->loginResult.dwUserID) {
        if (m_experienceItem) {
            if (msg->leftTime > 0) {
                auto btn = dynamic_cast<Button*>(m_experienceItem->getChildByTag(8888));
                if (btn) {
                    btn->setTouchEnabled(true);
                    btn->setBright(true);
                }
                
                auto img = ImageView::create("v66/room/tip.png");
                if (img) {
                    img->setPosition(Vec2(m_experienceItem->getContentSize().width/2 + 20 ,m_experienceItem->getContentSize().height/2 - 10));
                    m_experienceItem->addChild(img);
                }
            }
            else
            {
                
                int timeSecond = msg->nextTime;
                auto promptText = Text::create("", gFontFile, 30);
                if (promptText) {
                    promptText->setPosition(Vec2(m_experienceItem->getContentSize().width/2 + 20 ,m_experienceItem->getContentSize().height/2 - 5));
                    promptText->setColor(Color3B(255, 255, 255));
                    m_experienceItem->addChild(promptText);
                    
                    promptText->setString(StringUtils::format("距离下次体验时间: %d分钟",timeSecond));
                }
                
            }
        }
    }
#endif
    
    return true;
}
bool RoomListCompositeV66::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    
    return true;
}

void RoomListCompositeV66::execute() {

}

void RoomListCompositeV66::closeRoomList() {

    if (_deskLogic) {
        _deskLogic->stop();
    }

    if (_roomLogic) {
        _roomLogic->stop();
    }

	topMenudelegate->setRoomListDelegate(nullptr);
    RoomLogic()->closeRoom();
    
    roomListMoveIn(false);
//    this->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([&]() {
//        if (platformDelegate) {
//            platformDelegate->setLayerType(GAMELIST);
//        }
//        this->removeFromParent();
//    }), nullptr));
}

void RoomListCompositeV66::openRoomList(){
    if(_roomLogic)
    {
        _roomLogic->start();
    }
    
    topMenudelegate->setRoomListDelegate(this);
    
    roomListMoveIn(true);
}

void RoomListCompositeV66::updateRoomUserCount(UINT roomID, UINT userCount) {
    if (gameRoomLayer) {
        gameRoomLayer->updateRoomPeopleCount(roomID, userCount);
    }
}

void RoomListCompositeV66::onRoomLoginCallback(bool success, const std::string& message, UINT roomID) {
    auto isQuickJoin = UserDefault::getInstance()->getBoolForKey(IS_QUICK_JOIN);
    
    if (success && !isQuickJoin) {

        HNLOG_WARNING("the user enters a room complete message!");

        ComRoomInfo* roomInfo = RoomInfoModule()->getByRoomID(roomID);
		if (!roomInfo)
		{
			return;
		}
        RoomLogic()->setSelectedRoom(roomInfo);
        RoomLogic()->setRoomRule(roomInfo->dwRoomRule);

        if (GameCreator()->getCurrentGameType() == HNGameCreator::NORMAL) {// 金币场不扣积分
            _roomLogic->stop();

            _deskLogic = new HNRoomDeskLogic(this);

            std::vector<DeskInfo*>	_deskinfos;
            int deskNO = 0;
            for (int i = 0; i < roomInfo->uDeskCount; i++) {
                // 获取桌子玩家信息
                std::vector<UserInfoStruct*> deskUsers;
                UserInfoModule()->findDeskUsers(deskNO, deskUsers);

                DeskInfo* desk = new DeskInfo();
                //memset(desk, 0x0, sizeof(desk));

                if (nullptr != desk) {
                    desk->deskID = deskNO;
                    desk->multiple = roomInfo->iBasePoint;
                    desk->goldMin = roomInfo->iLessPoint;
                    desk->goldMax = roomInfo->iMaxPoint;
                    desk->peopleValue = deskUsers.size();
                    desk->peopleMax = roomInfo->uDeskPeople;
                    BYTE value = RoomLogic()->deskStation.bVirtualDesk[desk->deskID / 8] & (1 << (desk->deskID % 8));
                    if (0 != value) {
                        desk->peopleValue = roomInfo->uDeskPeople;
                    }
                }
                _deskinfos.push_back(desk);

                deskNO++;
            }
			
			//找出人数最多的桌子，如有相同，选择第一张（前提是桌子未坐满）
			//有效的桌子号
			int validPeople = 0;
			int validDeskNo = 0;
            
            vector<DeskInfo*> DeskInfoVecLocal;
            DeskInfoVecLocal.clear();
            for (DeskInfo* deskInfo : _deskinfos) {
                //桌子未坐满
                if (deskInfo->peopleValue != deskInfo->peopleMax) {
                    DeskInfoVecLocal.push_back(deskInfo);
                }
            }
            
            auto vecSize = DeskInfoVecLocal.size();//从未满的桌子中随机一张
            if (vecSize > 0) {
                auto maxSize =(int)vecSize - 1;
                int rand = random(0,maxSize);
                auto deskInfo = DeskInfoVecLocal.at(rand);
                
                validPeople = deskInfo->peopleValue;
                validDeskNo = deskInfo->deskID;
                
                _deskLogic->start();
                _deskLogic->requestSit(validDeskNo);
            }

        } else if (GameCreator()->getCurrentGameType() == HNGameCreator::BR) { //百人
            CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
            _roomLogic->requestSit(0, 0);
        } else if (GameCreator()->getCurrentGameType() == HNGameCreator::SINGLE) {
            CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
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
    else if(!success && !isQuickJoin)
    {
		CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
        CommandManager::getInstance()->showGamePrompt(message);
		if (platformDelegate) {
			platformDelegate->handleReturnButton();
		}
    }
    if(_roomLogic)
    {
        _roomLogic->start();
    }
}

//直接从房间进入游戏
void RoomListCompositeV66::onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) {
    
    auto isQuickJoin = UserDefault::getInstance()->getBoolForKey(IS_QUICK_JOIN);
    if (isQuickJoin) {
        if (_roomLogic) {
            _roomLogic->stop();
        }
        if (_deskLogic) {
            _deskLogic->stop();
        }
        return;
    }
    
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
	m_bIsTouch = false;
    if (success) {
        if (_roomLogic) {
            _roomLogic->stop();
        }
        if (_deskLogic) {
            _deskLogic->stop();
        }

        if (INVALID_DESKNO != deskNo && INVALID_DESKSTATION != seatNo) {
            // 启动游戏
            bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, deskNo, true);
            if (!ret) {
                CommandManager::getInstance()->showGamePrompt(GBKToUtf8("游戏启动失败。"));
            }
			else
			{
				//启动成功置为false防止游戏退到roomlist有执行快速加入逻辑
				UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, false);
			}
        }
    } else {
        ShowGamePrompt* gamePrompt = (ShowGamePrompt*)CommandManager::getInstance()->get(SHOW_GAME_PROMPT);
        gamePrompt->setText(GBKToUtf8(message.c_str()));
        gamePrompt->setCallBack([=]() {
            if (GameCreator()->getCurrentGameType() == HNGameCreator::SINGLE || GameCreator()->getCurrentGameType() == HNGameCreator::BR) {
                RoomLogic()->closeRoom();
                _roomLogic->stop();
            }
        });
        gamePrompt->execute();
    }
}

void RoomListCompositeV66::enterRoomCallback(Ref* pSender, Widget::TouchEventType type) {
    Button* btn = (Button*)pSender;
	switch (type)
	{
	case cocos2d::ui::Widget::TouchEventType::BEGAN:
		btn->setScale(0.8f);
		break;
	case cocos2d::ui::Widget::TouchEventType::CANCELED:
		btn->setScale(1.0f);
		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
		HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
		btn->setScale(1.0f);
		if (!m_bIsTouch)
		{
			float fMovePosY = WIN_SIZE.height;
			runAction(Sequence::create(/*DelayTime::create(0.5f), */MoveBy::create(0.2f, Vec2(0, fMovePosY)), nullptr));

			ComRoomInfo* roomInfo = static_cast<ComRoomInfo*>(btn->getUserData());
			// 参数校验
			if (nullptr == roomInfo) {
				return;
			}

			m_bIsTouch = true;
			this->scheduleOnce(schedule_selector(RoomListCompositeV66::cancelTouchRoom), 3.0f);

			RoomLogic()->setSelectedRoom(roomInfo);

			//保存进入游戏id,给游戏中多游戏逻辑区分用
			UserDefault::getInstance()->setIntegerForKey("Enter_Game_ID", roomInfo->uNameID);
			UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, false);
			UserDefault::getInstance()->setIntegerForKey(QUICK_JOIN_ROOM_ID, roomInfo->uRoomID);
			UserDefault::getInstance()->setIntegerForKey("Enter_GAME_ROOM_IDX", -1);
			CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
			_roomLogic->start();
			_roomLogic->requestLogin(roomInfo->uRoomID);
			if (m_jackpotModelData.size() > 0) {
				int tagRoom = btn->getTag() - 8888;
				UserDefault::getInstance()->setIntegerForKey("Enter_GAME_ROOM_IDX", tagRoom);
				int _idx = (m_nRoomCount - 1) - tagRoom;
				if (_idx >= 0 && _idx < m_jackpotModelData.size())
				{
					auto jackModel = m_jackpotModelData.at(_idx);
					if (jackModel) {
						GameData::getInstance()->setRoomJackpotData(jackModel->getCurJackpot());
					}
				}
			}
		}

	}
	break;

	default:
		break;
	}
}

void RoomListCompositeV66::cancelTouchRoom(float delata){
	m_bIsTouch = false;
}

void RoomListCompositeV66::roomListMoveIn(bool bIn)
{
    float fMovePosY = bIn ? (-WIN_SIZE.height) : (WIN_SIZE.height);
    
    if(bIn)
    {
        runAction(Sequence::create(DelayTime::create(0.5f), MoveBy::create(0.3f, Vec2(0, fMovePosY)), nullptr));
    }else {
        runAction(Sequence::create(DelayTime::create(0.3f), MoveBy::create(0.3f, Vec2(0, fMovePosY)), RemoveSelf::create(), nullptr));
    }
}

std::string RoomListCompositeV66::converNumberWithWAN(LLONG number) {
    char buffer[128] = {0};
    if (number < 10000) {
        sprintf(buffer, GetText("MIN_ROOM_SINGLE_MONEY"), number);
    }
    else {
        sprintf(buffer, GetText("MIN_ROOM_WAN_MONEY"), number/10000);
    }
    return std::string(buffer);
}
