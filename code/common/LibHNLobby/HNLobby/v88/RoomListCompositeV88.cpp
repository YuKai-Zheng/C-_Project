#include "RoomListCompositeV88.h"
#include "PlatformResV88.h"

#include "HNLobby/GameNotice.h"
#include "../FontConfig.h"
#include "../pattern/CommandManager.h"
#include "../PlatformDefine.h"
#include "../command/ShowGamePrompt.h"
#include "UI/LoadingLayer.h"
#include "UI/Base/AUIButton.h"

const char*	ROOM_ICON_PATH_V88 = "v88/room/%d.png";
const char*	ROOM_REQUEST_GOLD_BG_V88 = "v88/room/5.png";

RoomListCompositeV88::RoomListCompositeV88() {
    _roomLogic = nullptr;
    _deskLogic = nullptr;
}

RoomListCompositeV88::~RoomListCompositeV88() {
    if (_roomLogic) {
        HN_SAFE_DELETE(_roomLogic);
    }
    if (_deskLogic) {
        HN_SAFE_DELETE(_deskLogic);
    }
}

void RoomListCompositeV88::onEnter()
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

bool RoomListCompositeV88::init() {
    if (!Node::init()) {

        return false;
    }

	m_bIsTouch = false;

    _roomLogic = new HNRoomLogicBase(this);

    //房间信息
    int roomCount = RoomInfoModule()->getRoomCount();
    for (int i = 0; i < roomCount; i++) {
        auto info = RoomInfoModule()->getRoom(i);
        if(info->uNameID == 11118888)
        {
            RoomLogic()->setSelectedRoom(info);
            _roomLogic->start();
            _roomLogic->requestLogin(info->uRoomID);
            return true;
        }
        
        m_vRoomInfo.push_back(info);
    }

    //暂时固定4个场次
    float fChangeX = 434;
    float fFreeArea = 280;
    if(Application::getInstance()->getTargetPlatform() == cocos2d::Application::Platform::OS_IPAD)
    {
        fFreeArea = 500;
        fChangeX = 359;
    }
    float fItemLayerHeight = WIN_SIZE.height - fFreeArea;
    int nRoomCount = 4;
    float fItemHeight = 134;
    float fSpace = (fItemLayerHeight - fItemHeight*nRoomCount)/(nRoomCount+1);
    float fStartPosY = WIN_SIZE.height / 2 + fItemLayerHeight / 2 - (fSpace + fItemHeight/2);
    
    for (int i = 0; i < nRoomCount; i++) {
        if (i < m_vRoomInfo.size()) {
            char temp[64] = { 0 };
            sprintf(temp, ROOM_ICON_PATH_V88, i + 1);

            auto btn = AUIButton::create("v88/room/bg.png");
            if (btn) {
                //btn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);//868 359
                btn->setPosition(Vec2(WIN_SIZE.width - fChangeX, fStartPosY - i *(fSpace + fItemHeight)));
                btn->setTitle(temp);
                addChild(btn);
                btn->setTouchEnabled(true);
				btn->setPressedActionEnabled(true);
                btn->setShowEffect(false);
                btn->addTouchEventListener(CC_CALLBACK_2(RoomListCompositeV88::enterRoomCallback, this));
                btn->setUserData((void*)m_vRoomInfo.at(i));

				m_vRoomItem.push_back(btn);
				
				//房间入门金额
                auto minGold = Text::create("", FONT_TTF_FZCY, 20);
                if (minGold) {
                    stringstream stream;
                    stream << converNumberWithWAN(m_vRoomInfo.at(i)->iLessPoint);
                    minGold->setTextColor(Color4B(254, 215, 82, 255));
                    minGold->setString(stream.str());
                    minGold->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                    minGold->setPosition(Vec2(minGold->getContentSize().width*0.55f, minGold->getContentSize().height*1.55f));
                    btn->addChild(minGold);
                }
            }
        }
    }
    
    setPosition(Vec2(0, WIN_SIZE.height));
    
    return true;
}

void RoomListCompositeV88::execute() {

}

void RoomListCompositeV88::closeRoomList() {

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

void RoomListCompositeV88::openRoomList(){
    if(_roomLogic)
    {
        _roomLogic->start();
    }
    
    topMenudelegate->setRoomListDelegate(this);
    
    roomListMoveIn(true);
}

void RoomListCompositeV88::updateRoomUserCount(UINT roomID, UINT userCount) {
    if (gameRoomLayer) {
        gameRoomLayer->updateRoomPeopleCount(roomID, userCount);
    }
}

void RoomListCompositeV88::onRoomLoginCallback(bool success, const std::string& message, UINT roomID) {
    if (success) {

        HNLOG_WARNING("the user enters a room complete message!");

        ComRoomInfo* roomInfo = RoomInfoModule()->getByRoomID(roomID);
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
			for (DeskInfo* deskInfo : _deskinfos) {
				//桌子未坐满
				if (deskInfo->peopleValue != deskInfo->peopleMax) {
					validPeople = deskInfo->peopleValue;
					validDeskNo = deskInfo->deskID;
					break;
				}
			}

			_deskLogic->start();
			_deskLogic->requestSit(validDeskNo);

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
    } else {
		CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
        CommandManager::getInstance()->showGamePrompt(message);
    }
}

//直接从房间进入游戏
void RoomListCompositeV88::onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) {
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

void RoomListCompositeV88::enterRoomCallback(Ref* pSender, Widget::TouchEventType type) {
 
//	auto smallToBig = ScaleTo::create(0.03f, 1.01f);
//	auto bigToSmall = ScaleTo::create(0.03f, 1.0f);

    Button* btn = (Button*)pSender;

	switch (type)
	{
//	case cocos2d::ui::Widget::TouchEventType::BEGAN:
//	{
//	btn->runAction(smallToBig);
//	Sprite* bg = dynamic_cast<Sprite*>(btn->getChildByName("goldBg"));
//	bg->runAction(smallToBig->clone());
//	Label*	label = dynamic_cast<Label*>(bg->getChildByName("goldLabel"));
//	label->runAction(smallToBig->clone());
//	}
//		break;
//	case cocos2d::ui::Widget::TouchEventType::MOVED:
//		break;
	case cocos2d::ui::Widget::TouchEventType::ENDED:
	{
	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);

//	btn->runAction(bigToSmall);
//	Sprite* bg = dynamic_cast<Sprite*>(btn->getChildByName("goldBg"));
//	bg->runAction(bigToSmall->clone());
//	Label*	label = dynamic_cast<Label*>(bg->getChildByName("goldLabel"));
//	label->runAction(bigToSmall->clone());

	if (!m_bIsTouch)
	{
		ComRoomInfo* roomInfo = static_cast<ComRoomInfo*>(btn->getUserData());
		// 参数校验
		if (nullptr == roomInfo) {
			return;
		}

		m_bIsTouch = true;
		this->scheduleOnce(schedule_selector(RoomListCompositeV88::cancelTouchRoom), 3.0f);

		RoomLogic()->setSelectedRoom(roomInfo);

		//保存进入游戏id,给游戏中多游戏逻辑区分用
		UserDefault::getInstance()->setIntegerForKey("Enter_Game_ID", roomInfo->uNameID);
//		UserDefault::getInstance()->setBoolForKey(IS_QUICK_JOIN, false);
//		UserDefault::getInstance()->setIntegerForKey(QUICK_JOIN_ROOM_ID, roomInfo->uRoomID);

		//CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
		_roomLogic->start();
		_roomLogic->requestLogin(roomInfo->uRoomID);
	}

	}
		break;
//	case cocos2d::ui::Widget::TouchEventType::CANCELED:
//	{
//	btn->runAction(bigToSmall);
//	Sprite* bg = dynamic_cast<Sprite*>(btn->getChildByName("goldBg"));
//	bg->runAction(bigToSmall->clone());
//	Label*	label = dynamic_cast<Label*>(bg->getChildByName("goldLabel"));
//	label->runAction(bigToSmall->clone());
//	}
//		break;
	default:
		break;
	}
}

void RoomListCompositeV88::cancelTouchRoom(float delata){
	m_bIsTouch = false;
}

void RoomListCompositeV88::roomListMoveIn(bool bIn)
{
    float fMovePosY = bIn ? (-WIN_SIZE.height) : (WIN_SIZE.height);
    
    if(bIn)
    {
        runAction(Sequence::create(DelayTime::create(0.5f), MoveBy::create(0.3f, Vec2(0, fMovePosY)), nullptr));
    }else {
        runAction(Sequence::create(DelayTime::create(0.3f), MoveBy::create(0.3f, Vec2(0, fMovePosY)), RemoveSelf::create(), nullptr));
    }
}

std::string RoomListCompositeV88::converNumberWithWAN(LLONG number) {
    char buffer[128] = {0};
    if (number < 10000) {
        sprintf(buffer, GetText("MIN_ROOM_SINGLE_MONEY"), number);
    }
    else {
        sprintf(buffer, GetText("MIN_ROOM_WAN_MONEY"), number/10000);
    }
    return std::string(buffer);
}
