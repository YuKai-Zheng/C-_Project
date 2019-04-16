#include "BRNNGameTableUIV88.h"
#include "BRNNActionsV88.h"
#include "BRNNBankerLayerV88.h"
#include "BRNNBetV88.h"
#include "BRNNBetObjV88.h"
//#include "BRNNChatDialogV88.h"
//#include "BRNNClockV88.h"
#include "BRNNGameTableLogic.h"
//#include "BRNNGiveGiftV88.h"
#include "BRNNMsgV88.h"
#include "BRNNOtherPlayerV88.h"
#include "BRNNPlayerSeatV88.h"
#include "BRNNPokerCardV88.h"
//#include "BRNNSendFaceV88.h"
//#include "BRNNUionBankerLayerV88.h"
//#include"BRNNHelpLayer.h"

#include "BRNNResV88.h"
//#include "BRNNSetLayerV88.h"
#include "BRNNSettlementLayerV88.h"
//#include "BRNNUserInfoV88.h"
#include "HNLobbyExport.h"
#include "HNNetExport.h"

#include "HNLobby/v66/ExtraReward.h"
#include "../LibHNLogic/VersionInfo/VersionInfo.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/CommonMacro.h"
#include "HNLobby/pattern/CommandManager.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "UI/ToolBarLayer.h"
#include "UI/Scanner.h"
#include "UI/HelpLayer.h"
#include "UI/FloatCoin.h"
#include "HNLobby/pattern/factory/gameSet/GameSetFactory.h"

//static const char* BRNN_TOOL_BAR_PATH = "v66/13zhang/xialaanniu/%s";
//static const char* BRNN_TOOL_PATH = "v66/13zhang/%s";
#define WINSIZE Director::getInstance()->getWinSize()
#define NOTICE_ZORDER 100
#define DISTANCE 200.0
#define PI 3.1415926
//static const char* GAME_SOUND_BTN = "music/btnEffect/button_click_sound.mp3";

namespace BRNN {

HNGameUIBase *GameTableUIV88::create(BYTE bDeskIndex, bool bAutoCreate) {
    GameTableUIV88 *tableUI = new GameTableUIV88();
    if (tableUI && tableUI->init(bDeskIndex, bAutoCreate)) {
        tableUI->autorelease();
    } else {
        delete tableUI;
        tableUI = nullptr;
    }
    return tableUI;
}

GameTableUIV88::GameTableUIV88()
: _tableBg(nullptr), _gameStateSpr(nullptr), _zhuangBout(nullptr),
_myMoneyLabel(nullptr), _bankerSeatUI(nullptr), _clockObj(nullptr),
_noSeatBtn(nullptr), _bRepeatBetSataus(false), /*_chatDialog(nullptr),*/
_bankerInfo(nullptr), _bUpForChangeSeat(false), _zhangqi(false),
_bankeLayer(nullptr), _xiazhu(0), _mySeatIndex(-1), _bankerIndex(-1), _isNote(false),
      _xScale(1.0f), _yScale(1.0f), isPlayeTian(false), isPlayDi(false),
	  isPlayXuan(false), isPlayHuang(false), m_Notice(nullptr),toolBar_flag(true),m_bNextOut(false){
    _vecBetObj.clear();
    _vecPlayerSeatUI.clear();
    _vecWinOrLoseProp.clear();
    _vecWinTipSprite.clear();
    _allBetValInZone.clear();
    _myBetValInZone.clear();
    _vecBankerList.clear();
    //_vecChatList.clear();

    memset(_latest72GameInfoArr, 0, sizeof(_latest72GameInfoArr));
    memset(_recordGameInfoSpriteArr, 0, sizeof(_recordGameInfoSpriteArr));
    memset(_bUionBankerInfoArr, -1, sizeof(_bUionBankerInfoArr));

    _ownInfo = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID);
    //在进入游戏的构造里给平台数据赋值，解决游戏中退出游戏游戏数据和平台数据不同步的问题
    PlatformLogic()->loginResult.i64Money = _ownInfo->i64Money;

    initTheNewGameData();
}

GameTableUIV88::~GameTableUIV88() {
    //this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
    this->unschedule(schedule_selector(GameTableUIV88::LastClean));
    
    CC_SAFE_DELETE(_GameTableLogic);
    CC_SAFE_DELETE(m_pBeginData);
    m_Notice = nullptr;
}

void GameTableUIV88::removeTexture(const std::string &textureKeyName) {
    Director::getInstance()->getTextureCache()->removeTextureForKey(
        textureKeyName);
}

void GameTableUIV88::onEnter() {
    HNGameUIBase::onEnter();
	HNAudioEngine::getInstance()->playBackgroundMusic(BRNN_MUSIC_PLAYGAME_V88, true);
}

void GameTableUIV88::onExitTransitionDidStart() {
    HNGameUIBase::onExitTransitionDidStart();
    HNAudioEngine::getInstance()->stopBackgroundMusic();

   // this->unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
    this->unschedule(schedule_selector(GameTableUIV88::showBankerCardResult));
    this->unschedule(schedule_selector(GameTableUIV88::showSouthCardResult));
    this->unschedule(schedule_selector(GameTableUIV88::showWesternCardResult));
    this->unschedule(schedule_selector(GameTableUIV88::showNorthCardResult));
    this->unschedule(schedule_selector(GameTableUIV88::showEastCardResult));
}

void GameTableUIV88::onExit() {
    HNGameUIBase::onExit();
}

//触摸/按钮响应逻辑

void GameTableUIV88::onBtnClick(Ref *pSender) {
    Node *btn = (Node *)pSender;
    switch (btn->getTag()) {
	case em_banker_list:
    case em_makerBtn: {
        // TODO
        bool isShow = (btn->getTag() == em_banker_list);
        bool bMeIn = false;
        // for (int i = 0; i < _vecBankerList.size(); i++) {
        //    if (_vecBankerList[i].dwUserId ==
        //    PlatformLogic()->loginResult.dwUserID) {
        //        bMeIn = true;
        //        break;
        //    }
        //}
        bMeIn = (isMySeatInBankerList() == 1);
        if (_bankerInfo) {
            if (_bankerInfo->dwUserID == _ownInfo->dwUserID)
                bMeIn = true;
        }

		if (this->getChildByName("bankerLayer")) {
			(dynamic_cast<BankerLayerV88*>(this->getChildByName("bankerLayer")))->closeBankerLayer();
			return;
		}

		bankerLayer = BankerLayerV88::create(
						  _vecBankerList, m_pBeginData->minMoneyOfApplyBanker, bMeIn);
		BRNN::ActionsV88::actionIn(bankerLayer);
		int i = WINSIZE.width / 2;
		int h = WINSIZE.height / 2;
		bankerLayer->setAnchorPoint(Vec2(0.5, 1));
		Vec2 pos = btn->getPosition();
	/*	bankerLayer->setPosition(Vec2(WINSIZE.width / 2 - bankerLayer->getContentSize().width / 2,
			WINSIZE.height / 2 - bankerLayer->getContentSize().height / 2));*/
		bankerLayer->setPosition(Vec2(pos.x, pos.y - bankerLayer->getContentSize().height-40));
		bankerLayer->setVisible(isShow);
		if (isShow == true)
		{
			bankerLayer->setName("bankerLayer");
		}
		this->addChild(bankerLayer, 100);
    }
    break;
    case em_openBank: {
        if (getChildByTag(em_openBank)) {
            _bankeLayer = GameBankLayer::createGameBank();
            _bankeLayer->openBank(this, Vec2(WIN_SIZE.width, 0), 20000, 20000);
        }
    }
    break;
    case em_uionmakerBtn: {
        // sendData(MDM_GM_GAME_NOTIFY, ASS_SHANG_ZHUANG_UION_LIST);

//         vecTmpShangZhuangUionList vecInfo;
//         auto deskUserList = _GameTableLogic->getDeskUserList();
//         for (int i = 0; i < MAX_UION_COUNT; i++) {
//             tmpShangZhuangUionList item;
//             int userCount = 0;
//             for (int j = 0; j < MAX_UION_COUNT; j++) {
//                 if (-1 == _bUionBankerInfoArr[i][j])
//                     break;
//                 userCount++;
//             }
//             item.count = userCount;
//             LLONG tmpMoney = 0;
//             for (int j = 0; j < userCount; j++) {
//                 UserInfoStruct *pUser =
//                     deskUserList->getUserByDeskStation(_bUionBankerInfoArr[i][j]);
//                 item.users[j] = pUser;
//                 if (pUser)
//                     tmpMoney += pUser->i64Money;
//             }
//             if (tmpMoney >= m_pBeginData->minMoneyOfApplyBanker)
//                 item.bIsFull = true;
//             vecInfo.push_back(item);
//         }
//         UionBankerListV88::create(vecInfo, m_pBeginData->minMoneyOfApplyBanker / 10);
    }
    break;
    case em_toolbarBtn: {
        /*if (!dropdownList->isVisible()) {
            dropdownList->runAction(Sequence::create(
                                        CallFunc::create([&] { dropdownList->setVisible(true); }),
                                        MoveBy::create(0.3f,
                                                Vec2(0.0f, -dropdownList->getContentSize().height)),
                                        nullptr));
            this->getChildByTag(em_toolbarBtn)->setVisible(false);
        }*/
    }
    break;

    case em_shopBtn:
        //					GameCharge::create();
        break;
    case em_noSeatBtn: {
        auto deskUserList = _GameTableLogic->getDeskUserList();
        std::vector<UserInfoStruct *> uList;
		deskUserList->getActivatedUser(uList);
        //std::vector<UserInfoStruct *> vecOthers;
       // vecOthers.clear();
		std::vector<char*> vecOthersName;
		vecOthersName.clear();
        for (unsigned int i = 0; i < uList.size(); i++) {
			bool bEqual = false;
            for (int j = 0; j < _vecPlayerSeatUI.size(); j++) {
                if (_vecPlayerSeatUI.at(j)->getUser()) {
                    if (_vecPlayerSeatUI.at(j)->getUser()->dwUserID ==
                            uList[i]->dwUserID) {
						bEqual = true;
                        break;
                    }
                }
            }
			if (bEqual)
                continue;

            if (_bankerInfo) {
				if (uList[i]->dwUserID != _bankerInfo->dwUserID){
					vecOthersName.push_back(uList[i]->nickName);
				}
            } else {
				vecOthersName.push_back(uList[i]->nickName);
            }
        }
		//m_pOtherPlayerV88 = OtherPlayerV88::create(vecOthersName);
		if (vecOthersName.empty())
		{
			CommandManager::getInstance()->showGamePrompt(GBKToUtf8("此时无无座玩家！"));
		}
		else
		{
			OtherPlayerV88::create(vecOthersName);
		}
    }
    break;
    case em_repeatBetBtn: {
        if (hasLastBet()) {
            _bRepeatBetSataus = true;
			_isNote = true;
            this->getChildByTag(em_repeatBetBtn)->setVisible(false);
            this->getChildByTag(em_cancelRepeatBetBtn)->setVisible(true);

            if (GD_NEED_OPEN) {
                this->getChildByTag(em_repeatBetBtn)->setVisible(false);
                this->getChildByTag(em_cancelRepeatBetBtn)->setVisible(false);
            }
        }
    }
    break;
    case em_cancelRepeatBetBtn:
		_isNote = false;
        cancelRepeatBet();
        break;
    case em_chatOpenBtn: {
        /*_chatDialog = HNChatDialogV88::create(_vecChatList);
        _chatDialog->onSpeakCallBack = [this](const std::string &text) {
            _GameTableLogic->sendChatMsg(text);
        };
        _chatDialog->onMoveSelfAndNull = [this](void) {
            _chatDialog = nullptr;
        };
        _chatDialog->setPosition(this->getContentSize().width / 2,
                                 this->getContentSize().height / 2);
        this->addChild(_chatDialog);*/
    }
    break;

	case em_zoushituBtn: {
							 showWinLosShowcase();
	}
	break;
	case em_zoushituHideBtn: {
						hideWinLosShowcase();
	}
	break;

	
    case em_emotionBtn: {
        /*bool b = false;
        //判断我是否在12个座位上或者在庄家座位上
        for (int i = 0; i < _vecPlayerSeatUI.size(); i++) {
            if (_vecPlayerSeatUI.at(i)->getUser()) {
                if (_vecPlayerSeatUI.at(i)->getUser()->dwUserID ==
                        PlatformLogic()->loginResult.dwUserID) {
                    b = true;
                    break;
                }
            }
        }
        if (_bankerInfo) {
            if (_bankerInfo->dwUserID == PlatformLogic()->loginResult.dwUserID)
                b = true;
        }
		auto face = SendFaceV88::create(b);
		face->onSpeakCallBack = [this](const std::string &text) {
		_GameTableLogic->sendChatMsg(text);
		};*/
    }
    break;
    }
}

void GameTableUIV88::handleDropdownList(Ref *pSender, Control::EventType event) {
//    Node *btn = (Node *)pSender;
//    switch (btn->getTag()) {
//    case DropdownListTag::DL_SetBtn: {
//										 auto setLayer = BRNNSetLayerV88::create();
//										 setLayer->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
//										 setLayer->showSet(btn->getParent()->getParent(), 1);
//										
//    }
//    break;
//    case DropdownListTag::DL_HelpBtn: {
//        auto helpLayer = BRNNHelpLayerV88::create();
//        helpLayer->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
//        btn->getParent()->getParent()->addChild(helpLayer, 100);
//    }
//    break;
//    case DropdownListTag::DL_LeaveBtn: {
//        GameTableUIV88 *gameTableUI =
//			(dynamic_cast<GameTableUIV88 *>(btn->getParent()->getParent()));
//		int status = gameTableUI->_gameStatus;
//        if (!RoomLogic()->connected()) {
//            // this->_gameStatus
//			gameTableUI->leaveTheGame();
//        } else {
//            //玩家无下注，任何阶段都可以退出
//			if (gameTableUI->_myBetMoney == 0) {
//				gameTableUI->_GameTableLogic->sendUserUp();
//                // GameTableUIV88->leaveTheGame();
//                return;
//            }
//
//            if ((ASS_FREE_TIME !=
//                    status) /* && (_myBetMoney != 0)*/) { // 游戏不在空闲时期，并且投注金额非0，执行托管
//                leaveBombBox();
//                return;
//            }
//
//			gameTableUI->_GameTableLogic->sendUserUp();
//            // GameTableUIV88->leaveTheGame();
//        }
//        // handleLeaveBtn();
//    }
//    break;
//    case DropdownListTag::DL_NextRoundExitBtn: {
//        (dynamic_cast<DropdownList *>(btn->getParent()))->changeCheckBox();
//    }
//    break;
//    }
}

void GameTableUIV88::handleLeaveBtn() {
    //(dynamic_cast<DropdownList*>(btn->getParent()))
    //if (!RoomLogic()->connected()) {
    //    // this->_gameStatus
    //    leaveTheGame();
    //} else {
    //    if ((ASS_FREE_TIME !=
    //            _gameStatus) /* && (_myBetMoney != 0)*/) { // 游戏不在空闲时期，并且投注金额非0，执行托管
    //        leaveBombBox();
			
    //        return;
    //    }
    //    // leaveTheGame();
    //}
}

void GameTableUIV88::cancelRepeatBet() {
    _bRepeatBetSataus = false;
    this->getChildByTag(em_repeatBetBtn)->setVisible(true);
    this->getChildByTag(em_cancelRepeatBetBtn)->setVisible(false);

    if (GD_NEED_OPEN) {
        this->getChildByTag(em_repeatBetBtn)->setVisible(false);
        this->getChildByTag(em_cancelRepeatBetBtn)->setVisible(false);
    }
}

void GameTableUIV88::leaveTheGame() {
    if (_bUpForChangeSeat) {
        _bUpForChangeSeat = false;
        return;
    }

    //    _GameTableLogic->sendUserUp();
    HNRoomLogic::getInstance()->closeRoom();
    //更新玩家钱包和银行金额
    PlatformLogic()->loginResult.i64Money = _ownInfo->i64Money;
    PlatformLogic()->loginResult.i64Bank = _ownInfo->i64Bank;
    reset();
    //GamePlatform::returnPlatform(ROOMLIST);
    PlatformState* state = PlatformState::create();
    state->setType(ROOMLIST);
    StateManager::getInstance()->setState(state);
}

void GameTableUIV88::reset() {
    CC_SAFE_DELETE(_GameTableLogic);
    CC_SAFE_DELETE(m_pBeginData);
    //  GameBankLayer *_bankeLayer;
    //  HNChatDialog *_chatDialog;
    //  Sprite *_tableBg;
    //  PlayerSeat *_bankerSeatUI;
    //    Label *_zhuangBout;
    //    Label *_myMoneyLabel;
    //    Clock *_clockObj;
    //    Label *_gameStateTextTip;
    //    AUIButton *_noSeatBtn;
    //  BankerLayer *bankerLayer;
    //    Sprite *_chatTips;
    //    Button *_chatOpenBtn;
    //    Label *_chatUserName;
    //    Label *_chatLabel;
    //    Sprite *m_AnimateImage;
    //    DropdownList *dropdownList;

    _vecBankerList.clear();
    //_vecChatList.clear();
    _vecPlayerSeatUI.clear();
    _vecBetObj.clear();
    _betsInTianZone.clear();
    _betsInDiZone.clear();
    _betsInXuanZone.clear();
    _betsInHuangZone.clear();

    _myBetTypeInTianZone.clear();
    _myBetTypeInDiZone.clear();
    _myBetTypeInXuanZone.clear();
    _myBetTypeInHuangZone.clear();

    logos.clear();
    _allBetValInZone.clear();
    _myBetValInZone.clear();
    _myBetValInZone.clear();

    _vecWinTipSprite.clear();
    _vecWinOrLoseProp.clear();

    //  delete[] _cowValue;
    memset(_cowValue, 0, sizeof(_cowValue));
    //  delete _cowRes[5];
    memset(_cowRes, 0, sizeof(_cowRes));
	memset(_cowBgRes, 0, sizeof(_cowBgRes));
    memset(_rateResInZone, 0, sizeof(_rateResInZone));
    memset(_rateDataInZone, 0, sizeof(_rateDataInZone));
    memset(_cardsValue, 0, sizeof(_cardsValue));
    memset(_cardsRes, 0, sizeof(_cardsRes));
    memset(_cardsPos, 0, sizeof(_cardsPos));
    memset(_bUionBankerInfoArr, 0, sizeof(_bUionBankerInfoArr));
    memset(_latest72GameInfoArr, 0, sizeof(_latest72GameInfoArr));
    memset(_recordGameInfoSpriteArr, 0, sizeof(_recordGameInfoSpriteArr));
}

void GameTableUIV88::leaveBombBox() {
    //auto layer = GamePromptLayer::create();
    //layer->showPrompt(GetText("limit_leave_in_game"));
	GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
	layer->setGamePromptTip(GetText("limit_leave_in_game"));
}

bool GameTableUIV88::onTouchBegan(Touch *touch, Event *event) {
    Point point = touch->getLocation();
    point = this->convertToNodeSpace(point);

    //缩回下拉菜单
    /*if (dropdownList->isVisible()) {
        dropdownList->runAction(Sequence::create(
                                    MoveBy::create(0.3f, Vec2(0.0f, dropdownList->getContentSize().height)),
                                    CallFunc::create([&] { dropdownList->setVisible(false); }), nullptr));
        this->getChildByTag(em_toolbarBtn)->setVisible(true);
        return true;
    }*/

	//收回下拉框
	//if (0 == _toolbarBg->getName().compare("1")) {
	//	_toolbarBg->setName("0");

	//	_toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
	//		CallFunc::create([this]() {
	//		this->getChildByTag(em_toolbarBtn)->setVisible(true);
	//	})));
	//	return true;
	//}

//     //给庄家送礼
//     if (_bankerSeatUI->getBoundingBox().containsPoint(point)) {
//         if (_bankerInfo) {
//             // GiveGift::create(_bankerInfo);
//             BRNNUserInfoV88::create(_bankerInfo);
//         }
//         return true;
//     }

    //检查是否交换座位以及是否弹出信息框
//     for (int i = 0; i < _vecPlayerSeatUI.size(); i++) {
//         if (_vecPlayerSeatUI.at(i)->getBoundingBox().containsPoint(point)) {
//             if (_vecPlayerSeatUI.at(i)->getUser()) {
//                 // GiveGift::create(_vecPlayerSeatUI.at(i)->getUser());
//                 //BRNNUserInfoV88::create(_vecPlayerSeatUI.at(i)->getUser());
//             } else {
//                 _GameTableLogic->sendChangeSeat(i);
//                 // userChangeSeat(i);
//                 // //这个版本不支持换座位，系统分到哪个座位就在哪个座位
//             }
//             return true;
//         }
//     }

    if (ASS_XIA_ZHU == _gameStatus) { //处于下注时间
        //判断是否点击了四个下注区域
        for (int i = 0; i < BET_ZONE_NUM_V88; i++) {
            if (_vecWinTipSprite.at(i)->getBoundingBox().containsPoint(point)) {
                //得到投注数和区域，发送消息请求投注
                sendBet(i, getBetMoney(getSelectBet()), getSelectBet());
                cancelRepeatBet();
				_isNote = true;
                return true;
            }
        }
        //选中投注币
        for (int i = 0; i < MAX_JETTON_TYPE_V88; i++) {
            if (_vecBetObj.at(i)->getBoundingBox().containsPoint(point)) {
                if (!_vecBetObj.at(i)->isClick())
                    return true;
                if (i == getSelectBet())
                    return true;
                for (int j = 0; j < MAX_JETTON_TYPE_V88; j++) {
                    _vecBetObj.at(j)->setSelect(false);
                }
                _vecBetObj.at(i)->setSelect(true);
                return true;
            }
        }
    }

    return true;
}

// end=======================================

void GameTableUIV88::initTheNewGameData() {
    _i_cardIdx = -1;
    _j_cardIdx = -1;
    _myBetMoney = 0;
    _FuctionTimes = 0;

    memset(_cardsValue, 0, sizeof(_cardsValue));
    memset(_cardsRes, 0, sizeof(_cardsRes));
    memset(_cowValue, 0, sizeof(_cowValue));
    memset(_cowRes, 0, sizeof(_cowRes));
	memset(_cowBgRes, 0, sizeof(_cowBgRes));
    memset(_rateResInZone, 0, sizeof(_rateResInZone));
    memset(_rateDataInZone, 0, sizeof(_rateDataInZone));

    _betsInTianZone.clear();
    _betsInDiZone.clear();
    _betsInXuanZone.clear();
    _betsInHuangZone.clear();

    _myBetTypeInTianZone.clear();
    _myBetTypeInDiZone.clear();
    _myBetTypeInXuanZone.clear();
    _myBetTypeInHuangZone.clear();

    _ownInfo = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID);
}

void GameTableUIV88::updateUI() {
    updateBankerUI();
    // for(int i=0; i<MAX_SEAT_NUM; i++)
    //{
    //	_vecPlayerSeatUI.at(i)->userExit();
    //}
    // auto deskUserList = _GameTableLogic->getDeskUserList();
    // if(0 <= _bankerSeatNum && _bankerSeatNum < PLAY_COUNT)//庄家存在
    //{
    //	if(MAX_SEAT_NUM <= _bankerSeatNum)
    //	{
    //		for(int i=0; i<MAX_SEAT_NUM; i++)
    //		{
    //			UserInfoStruct* pUser =
    // deskUserList->getUserByDeskStation(i);
    //			if(pUser)
    //			{
    //				_vecPlayerSeatUI.at(i)->setUser(pUser);
    //			}
    //		}
    //	}
    //	else
    //	{
    //		for(int i=0; i<MAX_SEAT_NUM + 1; i++)
    //		{
    //			UserInfoStruct* pUser =
    // deskUserList->getUserByDeskStation(i);
    //			if(pUser)
    //			{
    //				if(i < _bankerSeatNum)
    //				{
    //					_vecPlayerSeatUI.at(i)->setUser(pUser);
    //				}
    //				else if(i > _bankerSeatNum)
    //				{
    //					_vecPlayerSeatUI.at(i-1)->setUser(pUser);
    //				}
    //			}
    //		}
    //	}
    //}
    // else
    //{
    //	for(int i=0; i<MAX_SEAT_NUM; i++)
    //	{
    //		UserInfoStruct* pUser = deskUserList->getUserByDeskStation(i);
    //		if(pUser)
    //		{
    //			_vecPlayerSeatUI.at(i)->setUser(pUser);
    //		}
    //	}
    //}
}

void GameTableUIV88::updateBankerUI() {
    getBankerInfo();
    _bankerSeatUI->userExit();
    if (_bankerInfo) {
        _bankerSeatUI->setUser(_bankerInfo);
        for (int i = 0; i < _vecPlayerSeatUI.size(); i++) {
            if (_vecPlayerSeatUI.at(i)->getUser()) {
                if (_vecPlayerSeatUI.at(i)->getUser()->dwUserID ==
                        _bankerInfo->dwUserID) {
                    _vecPlayerSeatUI.at(i)->userExit();
                    break;
                }
            }
        }
    }
    if (m_pBeginData->iBankerSeatNo > 240 ||
            m_pBeginData->iBankerSeatNo == SYS_BANKER_STATION_V88) {
        _bankerSeatUI->setGirl();
    } else {
        if (!_bankerInfo)
            return;
        if (_bankerIndex != _bankerInfo->dwUserID) {
            _bankerIndex = _bankerInfo->dwUserID;
            // showBankerSign(_bankerSeatUI->getPosition());
        }
    }
}

void GameTableUIV88::showBankerSign(Vec2 pos) {
	auto sign = ImageView::create(BRNN_BANKER_SIGN_V88);
    sign->setPosition(
        pos - Vec2(-_bankerSeatUI->getContentSize().width / 2 -24 * _xScale,
                   _bankerSeatUI->getContentSize().height / 2 + 10 * _yScale));
    this->addChild(sign, 100);
    sign->runAction(Sequence::create(
                        ScaleTo::create(0.2f, 0.7f), ScaleTo::create(0.5f, 1.1f),
                        ScaleTo::create(0.5f, 0.7f), ScaleTo::create(0.5f, 1.1f),
                        ScaleTo::create(0.5f, 0.7f), RemoveSelf::create(), nullptr));
}

void GameTableUIV88::updateSomeData() {
    _bankerSeatUI->updateUser();
    for (int i = 0; i < _vecPlayerSeatUI.size(); i++) {
        _vecPlayerSeatUI.at(i)->updateUser();
    }
    _ownInfo = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID);
    _myMoneyLabel->setString(Tools::addComma(_ownInfo->i64Money));
}

void GameTableUIV88::updateLatest6GameInfo() {
    //寻找当前多少局，这里有点吭
    int zhiqianJuShu = GAME_SAVE_COUNT;
    for (int i = 0; i < GAME_SAVE_COUNT; i++) {
        if (0 == _latest72GameInfoArr[i][0]) {
            zhiqianJuShu = i;
            break;
        }
    }

    auto director = Director::getInstance();
    for (int i = 0; i < BET_ZONE_NUM_V88; i++) {
        int setPosNum = RECORD_LATEST_SHOW_NUM_V88;
        if (zhiqianJuShu < RECORD_LATEST_SHOW_NUM_V88)
            setPosNum = zhiqianJuShu;
        for (int j = 0; j < setPosNum; j++) {
            if (_recordGameInfoSpriteArr[i][j]) {
				if (_latest72GameInfoArr[zhiqianJuShu - 1 - j][i] > 0)
				{
					_recordGameInfoSpriteArr[i][j]->setTexture(Director::getInstance()->getTextureCache()->addImage(BRNN_TABLERES_RECORDWIN_V88));
					_recordGameInfoSpriteArr[i][j]->setVisible(true);

				}
				else
				{
					_recordGameInfoSpriteArr[i][j]->setTexture(Director::getInstance()->getTextureCache()->addImage(BRNN_TABLERES_RECORDLOSE_V88));
					_recordGameInfoSpriteArr[i][j]->setVisible(true);
				}
			}
        }
        float winCnt = 0.0;
        for (int k = 0; k < zhiqianJuShu; k++) {
            if (_latest72GameInfoArr[i][k] > 0)
                winCnt = winCnt + 1;
        }
        if (zhiqianJuShu > 0)
            _vecWinOrLoseProp.at(i)->setString(
                StringUtils::format("%2.f", winCnt / zhiqianJuShu * 100) + "%");
    }
}

void GameTableUIV88::getBankerInfo() {
    auto deskUserList = _GameTableLogic->getDeskUserList();
    _bankerInfo = deskUserList->getUserByDeskStation(m_pBeginData->iBankerSeatNo);
    //有时候座位号-1还能拿到玩家信息
    if (-1 == m_pBeginData->iBankerSeatNo || m_pBeginData->iBankerSeatNo > 240 ||
            SYS_BANKER_STATION_V88 == m_pBeginData->iBankerSeatNo) {
        _bankerInfo = nullptr;
    }
}

// byStation:玩家本身的位置号
// byLastSeatPosition:原来的位置
// bySeatPosition:玩家要坐的位置
void GameTableUIV88::betChangeStartPos(BYTE byStation, BYTE byLastStation,
                                    BYTE seatNo) {
    Point prePos;
    if (byLastStation != 255) {
        prePos = _vecPlayerSeatUI.at(byLastStation)->getPosition();
    } else {
        if (_ownInfo->bDeskStation == byStation)
            prePos = _myMoneyLabel->getPosition();
        else
            prePos = _noSeatBtn->getPosition();
    }

    Point futurePos;
    if (seatNo != 255) {
        futurePos = _vecPlayerSeatUI.at(seatNo)->getPosition();
    } else {
        if (_ownInfo->bDeskStation == byStation)
            futurePos = _myMoneyLabel->getPosition();
        else
            futurePos = _noSeatBtn->getPosition();
    }

    for (int i = 0; i < _betsInTianZone.size(); i++) {
        if (_betsInTianZone.at(i)->_startPos == prePos) {
            _betsInTianZone.at(i)->_startPos = futurePos;
        }
    }

    for (int i = 0; i < _betsInDiZone.size(); i++) {
        if (_betsInDiZone.at(i)->_startPos == prePos) {
            _betsInDiZone.at(i)->_startPos = futurePos;
        }
    }

    for (int i = 0; i < _betsInXuanZone.size(); i++) {
        if (_betsInXuanZone.at(i)->_startPos == prePos) {
            _betsInXuanZone.at(i)->_startPos = futurePos;
        }
    }

    for (int i = 0; i < _betsInHuangZone.size(); i++) {
        if (_betsInHuangZone.at(i)->_startPos == prePos) {
            _betsInHuangZone.at(i)->_startPos = futurePos;
        }
    }
}

//游戏逻辑=============================
void GameTableUIV88::resumeTheGameFree(TagGameState_WaiteAgee *pcene,
                                    BYTE gameState) {
    // auto deskUserList = _GameTableLogic->getDeskUserList();
    ////玩家位置
    // for (int i = 0; i < 12; i++) {
    //    if (pcene->bySeatPostion[i] == 255)		continue;
    //    UserInfoStruct* pUser =
    //    deskUserList->getUserByDeskStation(pcene->bySeatPostion[i]);
    //    if (!pUser)
    //        continue;
    //    if (pUser->dwUserID == PlatformLogic()->loginResult.dwUserID) {
    //        _mySeatIndex = i;
    //    }
    //    _vecPlayerSeatUI.at(i)->setUser(pUser);
    //}
    updateNonBankerSeatUI(pcene->bySeatPosition);

	int i = pcene->iZhuangBaShu + 1;
	getRemainDealer()->setString(StringUtils::format("第%d局", i));

    memcpy(_latest72GameInfoArr, pcene->iResultInfo,
           sizeof(_latest72GameInfoArr));

    m_pBeginData->iXiaZhuTime = pcene->iXiaZhuTime;
    m_pBeginData->iKaiPaiTime = pcene->iKaiPaiTime;
    m_pBeginData->iFreeTime = pcene->iFreeTime;
    m_pBeginData->maxRate = pcene->bMaxBase;
    m_pBeginData->minMoneyOfApplyBanker = pcene->i64ShangZhuangLimit;

    m_pBeginData->iBankerSeatNo = pcene->curBankerStation;
    getBankerInfo();

    updateUI();
    updateAppliedBankerList(pcene->bankerList);
    updateLatest6GameInfo();
    _gameStatus = ASS_FREE_TIME;
    _clockObj->start(pcene->iRemaindTime);
	_gameStateSpr->setTexture("v66/zi/13.png");
    setBetsClick(false);
}

void GameTableUIV88::updateAppliedBankerList(short int list[PLAY_COUNT + 1][2]) {
    _vecBankerList.clear();
    auto seatUserList = _GameTableLogic->getDeskUserList();
    for (int i = 0; i < PLAY_COUNT; i++) {
        if (-1 != list[i][0]) {
            UserInfoStruct *pUser = seatUserList->getUserByDeskStation(list[i][0]);
            if (pUser) {
                TMP_BANKER_INFOV88 info;
                info.name = pUser->nickName;
                info.money = pUser->i64Money;
                info.dwUserId = pUser->dwUserID;
                _vecBankerList.push_back(info);
            } else {
                if (list[i][0] > 240) {
                    TMP_BANKER_INFOV88 info;
                    info.name = StringUtils::format(GetText("union_banker"),
                                                    abs(list[i][0] - 255) + 1)
                                .c_str();
                    info.money = m_pBeginData->minMoneyOfApplyBanker;
                    info.dwUserId = list[i][0];
                    _vecBankerList.push_back(info);
                }
            }
        } else {
            break;
        }
    }
    bool bMeIn = (isMySeatInBankerList() == 1);
    Button *listButton =
        dynamic_cast<Button *>(this->getChildByTag(em_banker_list));
    Button *makerButton =
        dynamic_cast<Button *>(this->getChildByTag(em_makerBtn));

    listButton->setVisible(bMeIn);
    //listButton->setEnabled(bMeIn);

    makerButton->setVisible(!bMeIn);
   // makerButton->setEnabled(!bMeIn);
}

void GameTableUIV88::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
    if (bVisible) {
        if (!m_Notice) {
            m_Notice = GameNotice::create();
            m_Notice->setLocalZOrder(200);
            m_Notice->setName("GameNotice");
        }
        m_Notice->openNotic(this, 10000, message, nCount, nType);
    }
}

void GameTableUIV88::resumeTheGameBet(TagGameState_Bet *pcene, BYTE gameState) {
    // auto deskUserList = _GameTableLogic->getDeskUserList();
    ////玩家位置
    // for (int i = 0; i < 12; i++) {
    //    if (pcene->bySeatPostion[i] == 255)		continue;
    //    UserInfoStruct* pUser =
    //    deskUserList->getUserByDeskStation(pcene->bySeatPostion[i]);
    //    if (!pUser)
    //        continue;
    //    if (pUser->dwUserID == PlatformLogic()->loginResult.dwUserID) {
    //        _mySeatIndex = i;
    //    }
    //    _vecPlayerSeatUI.at(i)->setUser(pUser);
    //}

	

    updateNonBankerSeatUI(pcene->bySeatPosition);

	//当庄第几局
	int i = pcene->iZhuangBaShu + 1;
	getRemainDealer()->setString(StringUtils::format("第%d局", i));

    for (int i = 0; i < 72; i++) {
        for (int j = 0; j < 4; j++) {
            //这里服务器下发的是char类型的
            _latest72GameInfoArr[i][j] = pcene->iResultInfo[i][j];
        }
    }

    m_pBeginData->iXiaZhuTime = pcene->iXiaZhuTime;
    m_pBeginData->iKaiPaiTime = pcene->iKaiPaiTime;
    m_pBeginData->iFreeTime = pcene->iFreeTime;
    m_pBeginData->maxRate = pcene->bMaxBase;
    m_pBeginData->minMoneyOfApplyBanker = pcene->i64ShangZhuangLimit;
    m_pBeginData->iBankerSeatNo = pcene->curBankerStation;
    m_pBeginData->maxBetOfZone = pcene->i64MaxNote;

    getBankerInfo();
    updateUI();
    updateAppliedBankerList(pcene->bankerList);
    updateLatest6GameInfo();
    _gameStatus = ASS_XIA_ZHU;
    setBetsClick(true);
    _clockObj->start(pcene->iRemaindTime);
	_gameStateSpr->setTexture("v66/zi/12.png");

    setBetInZoneVisible(true, pcene->zoneBet, pcene->myBet);
    setBetsClick(true);
}

void GameTableUIV88::updateNonBankerSeatUI(BYTE bySeatPostion[12]) {
    auto deskUserList = _GameTableLogic->getDeskUserList();
    //有座位的玩家位置
    for (int i = 0; i < 12; i++) {
        if (bySeatPostion[i] == 255)
            continue;
        UserInfoStruct *pUser =
            deskUserList->getUserByDeskStation(bySeatPostion[i]);
        if (!pUser)
            continue;
        if (pUser->dwUserID == PlatformLogic()->loginResult.dwUserID) {
            _mySeatIndex = i;
        }
        _vecPlayerSeatUI.at(i)->setUser(pUser);
    }
}

void GameTableUIV88::resumeTheGameOpenCard(TagGameState_OpenCard *pcene,
                                        BYTE gameStation) {
    auto deskUserList = _GameTableLogic->getDeskUserList();

	int i = pcene->iZhuangBaShu + 1;
	getRemainDealer()->setString(StringUtils::format("第%d局", i));

    //玩家位置
    for (int i = 0; i < MAX_NON_BANKER_SEAT_NUM_V88; i++) {
        if (pcene->bySeatPosition[i] == 255)
            continue;
        UserInfoStruct *pUser =
            deskUserList->getUserByDeskStation(pcene->bySeatPosition[i]);
        if (!pUser)
            continue;
        if (pUser->dwUserID == PlatformLogic()->loginResult.dwUserID) {
            _mySeatIndex = i;
        }
        _vecPlayerSeatUI.at(i)->setUser(pUser);
    }

    for (int i = 0; i < PLAY_COUNT + 1; i++) {
        for (int j = 0; j < 2; j++) {
            log("pcene->idzhuangState[%d][%d] = %d", i, j, pcene->bankerList[i][j]);
        }
    }

    // 72局4区域
    for (int i = 0; i < 72; i++) {
        for (int j = 0; j < 4; j++) {
            //这里服务器下发的是char类型的
            _latest72GameInfoArr[i][j] = pcene->iResultInfo[i][j];
        }
    }

    m_pBeginData->iXiaZhuTime = pcene->iXiaZhuTime;
    m_pBeginData->iKaiPaiTime = pcene->iKaiPaiTime;
    m_pBeginData->iFreeTime = pcene->iFreeTime;

    m_pBeginData->maxRate = pcene->bMaxBase;
    m_pBeginData->minMoneyOfApplyBanker = pcene->i64ShangZhuangLimit;

    m_pBeginData->iBankerSeatNo = pcene->curBankerStation;
    getBankerInfo();

    this->savaCardValue(pcene->byCard);
    //输赢区域赋值，里面即为倍数
    for (BYTE i = 0; i < BET_ZONE_NUM_V88; i++) {
        _rateDataInZone[i] = pcene->iComputePoint[i];
        _cowValue[i] = pcene->cowValue[i];
    }

    this->recoveryOpenCardScene();

    //显示庄闲家的投注额
    setBetInZoneVisible(true, pcene->zoneBet, pcene->myBet);

    updateUI();
    updateAppliedBankerList(pcene->bankerList);
    updateLatest6GameInfo();
    _gameStatus = ASS_KAI_PAI;
    setBetsClick(false);
    _clockObj->start(pcene->iRemaindTime);
	_gameStateSpr->setTexture("v66/zi/14.png");
}

void GameTableUIV88::resumeTheGameResult(TagGameState_ShowResult *pcene,
                                      BYTE gameStation) {
    // auto deskUserList = _GameTableLogic->getDeskUserList();
    ////玩家位置
    // for (int i = 0; i < 12; i++) {
    //    if (pcene->bySeatPostion[i] == 255)		continue;
    //    UserInfoStruct* pUser =
    //    deskUserList->getUserByDeskStation(pcene->bySeatPostion[i]);
    //    if (!pUser)			continue;
    //    if (pUser->dwUserID == PlatformLogic()->loginResult.dwUserID) {
    //        _mySeatIndex = i;
    //    }
    //    _vecPlayerSeatUI.at(i)->setUser(pUser);
    //}
    updateNonBankerSeatUI(pcene->bySeatPosition);

	//(pcene->iZhuangBaShu)

	//log("坐庄次数：%d", pcene->iZuangBaShu);
	////数据从服务端传过来，不从0开始计数，从1开始，所以加1
	//sprintf(message, "%d", pcene->iZuangBaShu + 1);
	//_gameUICallback->getRemainDealer()->setString(GBKToUtf8(message));

	int i = pcene->iZhuangBaShu + 1;
	getRemainDealer()->setString(StringUtils::format("第%d局",i));

    for (int i = 0; i < 72; i++) {
        for (int j = 0; j < 4; j++) {
            //这里服务器下发的是char类型的
            _latest72GameInfoArr[i][j] = pcene->iResultInfo[i][j];
        }
    }

    m_pBeginData->iXiaZhuTime = pcene->iXiaZhuTime;
    m_pBeginData->iKaiPaiTime = pcene->iKaiPaiTime;
    m_pBeginData->iFreeTime = pcene->iFreeTime;
    m_pBeginData->maxRate = pcene->bMaxBase;
    m_pBeginData->minMoneyOfApplyBanker = pcene->i64ShangZhuangLimit;
    m_pBeginData->iBankerSeatNo = pcene->curBankerStation;

    getBankerInfo();
    updateUI();
    updateAppliedBankerList(pcene->bankerList);
    updateLatest6GameInfo();
    _clockObj->start(pcene->iRemaindTime);
}

//////////////////////////////////////////////////////////////////////////发送请求

void GameTableUIV88::sendBet(INT betArea, LLONG betVal, INT betType) {
    TagXiaZhu xiazhu;
    xiazhu.iMoney = betVal;
    xiazhu.iChouMaType = betType;
    xiazhu.iBetArea = betArea;
    //向服务器发送指令
    _GameTableLogic->sendBet((char *)&xiazhu, sizeof(TagXiaZhu));
}

//////////////////////////////////////////////////////////////////////////

void GameTableUIV88::startNewTurn(BEGIN_DATA *pcene) {
    if (m_bNextOut) {
        // leaveTheGame();
        _GameTableLogic->sendUserUp();
        return;
    }
	gameStart->setVisible(true);
	gameStartAnimation->play("gamestart", false);

    isPlayeTian = false;
    isPlayDi = false;
    isPlayXuan = false;
    isPlayHuang = false;

    //isFirstTouch = true;

    checkAllBeforeXiaZhu();

    memcpy(m_pBeginData, pcene, sizeof(BEGIN_DATA));
    memcpy(_bUionBankerInfoArr, pcene->bUionBankerInfo,
           sizeof(_bUionBankerInfoArr));

    updateAppliedBankerList(pcene->iNTlist);

    //设置为下注状态
    _gameStatus = ASS_XIA_ZHU;
    _clockObj->start(m_pBeginData->iXiaZhuTime);
	_gameStateSpr->setTexture("v66/zi/12.png");
    LLONG allBetVal[BET_ZONE_NUM_V88] = {0, 0, 0, 0};
    LLONG myBetVal[BET_ZONE_NUM_V88] = {0, 0, 0, 0};
    setBetInZoneVisible(false, allBetVal, myBetVal);


    //是否自动下注
    if (_bRepeatBetSataus) {
		_isNote = true;
        if (!hasLastBet()) {
            cancelRepeatBet();
        } else {
            //看钱够否
            LLONG allBetMoney = 0;
            for (unsigned int i = 0; i < _myBetTypeInTianZone.size(); i++) {
                allBetMoney = allBetMoney + getBetMoney(_myBetTypeInTianZone[i]);
            }
            for (unsigned int i = 0; i < _myBetTypeInDiZone.size(); i++) {
                allBetMoney = allBetMoney + getBetMoney(_myBetTypeInDiZone[i]);
            }
            for (unsigned int i = 0; i < _myBetTypeInXuanZone.size(); i++) {
                allBetMoney = allBetMoney + getBetMoney(_myBetTypeInXuanZone[i]);
            }
            for (unsigned int i = 0; i < _myBetTypeInHuangZone.size(); i++) {
                allBetMoney = allBetMoney + getBetMoney(_myBetTypeInHuangZone[i]);
            }
            if (allBetMoney > _ownInfo->i64Money) {
                cancelRepeatBet();
                MsgTipV88::create(GetText("hand_bet"));
            } else {
                for (unsigned int i = 0; i < _myBetTypeInTianZone.size(); i++) {
                    sendBet(TIAN_AREA_V88, getBetMoney(_myBetTypeInTianZone[i]),
                            _myBetTypeInTianZone[i]);
                }
                for (unsigned int i = 0; i < _myBetTypeInDiZone.size(); i++) {
                    sendBet(DI_AREA_V88, getBetMoney(_myBetTypeInDiZone[i]),
                            _myBetTypeInDiZone[i]);
                }
                for (unsigned int i = 0; i < _myBetTypeInXuanZone.size(); i++) {
                    sendBet(XUAN_AREA_V88, getBetMoney(_myBetTypeInXuanZone[i]),
                            _myBetTypeInXuanZone[i]);
                }
                for (unsigned int i = 0; i < _myBetTypeInHuangZone.size(); i++) {
                    sendBet(HUANG_AREA_V88, getBetMoney(_myBetTypeInHuangZone[i]),
                            _myBetTypeInHuangZone[i]);
                }

            }
        }
    }
    _myBetTypeInTianZone.clear();
    _myBetTypeInDiZone.clear();
    _myBetTypeInXuanZone.clear();
    _myBetTypeInHuangZone.clear();
    //
    updateUI();
    updateNonBankerSeatUI(pcene->bySeatPosition);
    updateLatest6GameInfo();
    clearTheNewGameData();
    setBetsClick(true);
	HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_START_JETTON_V88);

    for (int i = 255; i >= 246; i--) {
        if (m_pBeginData->iBankerSeatNo == i) {
            for (int j = 0; j < MAX_UION_COUNT; j++) {
                if (_ownInfo->bDeskStation == _bUionBankerInfoArr[i][j]) {
                    MsgTipV88::create(GetText("start_union"));
                }
            }
        }
    }

    //开启假投注
    if (false) {
        this->schedule(schedule_selector(GameTableUIV88::betFalse), 0.01f, 500, 0.0f);
    }

}

void GameTableUIV88::showBet(TagXiaZhu *pcene) {
    // cleanCardAndCowAndMultipleSprite();
    auto deskUserList = _GameTableLogic->getDeskUserList();
    //根据实际位置获取玩家信息，看这个投注者是否是在12各座位之一
    auto betUser = deskUserList->getUserByDeskStation(pcene->byStation);
    if (!betUser)
        return;

    int posIdx = -1; //-1:无座玩家投注
    for (int i = 0; i < _vecPlayerSeatUI.size(); i++) {
        auto seatUser = _vecPlayerSeatUI.at(i)->getUser();
        if (seatUser && betUser && seatUser->dwUserID == betUser->dwUserID) {
            posIdx = i;
            break;
        }
    }

    int betType = 0;
    for (int i = 0; i < 5; i++) {
        if (pcene->iMoney == ChoumaValueV88[i]) {
            betType = i;
            break;
        }
    }

	HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_DOWN_COIN_V88);

    if (betType > 1) {
        int num = getBetMoney(betType) / getBetMoney(betType - 1);
        for (int i = 0; i < num; i++) {
            BetV88 *bet = nullptr;
            if (-1 == posIdx) { //无座玩家投注
                if (_ownInfo->dwUserID ==
                        betUser->dwUserID) { //自己是无座玩家，赢钱后，投注币飞到 金钱数值处
                    bet = BetV88::create(betType - 1, pcene->iBetArea,
                                      _myMoneyLabel->getPosition());
                } else {
                    bet = BetV88::create(betType - 1, pcene->iBetArea,
                                      _noSeatBtn->getPosition() +
                                      Vec2(_noSeatBtn->getContentSize().width / 2,
                                           -_noSeatBtn->getContentSize().height / 2));
                }
            } else {
                bet = BetV88::create(
                          betType - 1, pcene->iBetArea,
                          _vecPlayerSeatUI.at(posIdx)->getPosition() +
                          Vec2(_vecPlayerSeatUI.at(posIdx)->getContentSize().width / 2,
                               -_vecPlayerSeatUI.at(posIdx)->getContentSize().height /
                               2));
            }
            this->addChild(bet);

            if (TIAN_AREA_V88 == pcene->iBetArea) {
                bet->moveToBetZone2(logos.at(TIAN_AREA_V88)->getBoundingBox());
                _betsInTianZone.pushBack(bet);
                //if (_ownInfo->dwUserID == betUser->dwUserID)
                //    _myBetTypeInTianZone.push_back(betType);
            } else if (DI_AREA_V88 == pcene->iBetArea) {
                bet->moveToBetZone2(logos.at(DI_AREA_V88)->getBoundingBox());
                _betsInDiZone.pushBack(bet);
                //if (_ownInfo->dwUserID == betUser->dwUserID)
                //    _myBetTypeInDiZone.push_back(betType);
            } else if (XUAN_AREA_V88 == pcene->iBetArea) {
                bet->moveToBetZone2(logos.at(XUAN_AREA_V88)->getBoundingBox());
                _betsInXuanZone.pushBack(bet);
                //if (_ownInfo->dwUserID == betUser->dwUserID)
                //    _myBetTypeInXuanZone.push_back(betType);
            } else if (HUANG_AREA_V88 == pcene->iBetArea) {
                bet->moveToBetZone2(logos.at(HUANG_AREA_V88)->getBoundingBox());
                _betsInHuangZone.pushBack(bet);
                //if (_ownInfo->dwUserID == betUser->dwUserID)
                //    _myBetTypeInHuangZone.push_back(betType);
            }
        }
    } else {
        BetV88 *bet = nullptr;
        if (-1 == posIdx) { //无座玩家投注
            if (_ownInfo->dwUserID ==
                    betUser->dwUserID) { //自己是无座玩家，赢钱后，投注币飞到 金钱数值处
                bet =
                    BetV88::create(betType, pcene->iBetArea, _myMoneyLabel->getPosition());
            } else {
                bet = BetV88::create(betType, pcene->iBetArea,
                                  _noSeatBtn->getPosition() +
                                  Vec2(_noSeatBtn->getContentSize().width / 2,
                                       -_noSeatBtn->getContentSize().height / 2));
            }
        } else {
            bet = BetV88::create(
                      betType, pcene->iBetArea,
                      _vecPlayerSeatUI.at(posIdx)->getPosition() +
                      Vec2(_vecPlayerSeatUI.at(posIdx)->getContentSize().width / 2,
                           -_vecPlayerSeatUI.at(posIdx)->getContentSize().height / 2));
        }
        this->addChild(bet);

        if (TIAN_AREA_V88 == pcene->iBetArea) {
            bet->moveToBetZone2(logos.at(TIAN_AREA_V88)->getBoundingBox());

            _betsInTianZone.pushBack(bet);
            //if (_ownInfo->dwUserID == betUser->dwUserID)
            //	_myBetTypeInTianZone.push_back(betType);
        } else if (DI_AREA_V88 == pcene->iBetArea) {
            bet->moveToBetZone2(logos.at(DI_AREA_V88)->getBoundingBox());
            _betsInDiZone.pushBack(bet);
            //if (_ownInfo->dwUserID == betUser->dwUserID)
            //    _myBetTypeInDiZone.push_back(betType);
        } else if (XUAN_AREA_V88 == pcene->iBetArea) {
            bet->moveToBetZone2(logos.at(XUAN_AREA_V88)->getBoundingBox());
            _betsInXuanZone.pushBack(bet);
            //if (_ownInfo->dwUserID == betUser->dwUserID)
            //    _myBetTypeInXuanZone.push_back(betType);
        } else if (HUANG_AREA_V88 == pcene->iBetArea) {
            bet->moveToBetZone2(logos.at(HUANG_AREA_V88)->getBoundingBox());
            _betsInHuangZone.pushBack(bet);
            //if (_ownInfo->dwUserID == betUser->dwUserID)
            //    _myBetTypeInHuangZone.push_back(betType);
        }
    }

    if (TIAN_AREA_V88 == pcene->iBetArea) {
        //bet->moveToBetZone2(logos.at(TIAN_AREA_V88)->getBoundingBox());
        //_betsInTianZone.pushBack(bet);
        if (_ownInfo->dwUserID == betUser->dwUserID)
            _myBetTypeInTianZone.push_back(betType);
    } else if (DI_AREA_V88 == pcene->iBetArea) {
        //bet->moveToBetZone2(logos.at(DI_AREA_V88)->getBoundingBox());
        //_betsInDiZone.pushBack(bet);
        if (_ownInfo->dwUserID == betUser->dwUserID)
            _myBetTypeInDiZone.push_back(betType);
    } else if (XUAN_AREA_V88 == pcene->iBetArea) {
        //bet->moveToBetZone2(logos.at(XUAN_AREA_V88)->getBoundingBox());
        //_betsInXuanZone.pushBack(bet);
        if (_ownInfo->dwUserID == betUser->dwUserID)
            _myBetTypeInXuanZone.push_back(betType);
    } else if (HUANG_AREA_V88 == pcene->iBetArea) {
        //bet->moveToBetZone2(logos.at(HUANG_AREA_V88)->getBoundingBox());
        //_betsInHuangZone.pushBack(bet);
        if (_ownInfo->dwUserID == betUser->dwUserID)
            _myBetTypeInHuangZone.push_back(betType);
    }
    ////

    //更新投注区域的金钱值
    std::string allBetStr = _allBetValInZone.at(pcene->iBetArea)->getString();
    allBetStr = StringUtils::format("%ld", atol(allBetStr.c_str()) +
                                    ChoumaValueV88[betType]);
    _allBetValInZone.at(pcene->iBetArea)->setString(allBetStr);
    _allBetValInZone.at(pcene->iBetArea)->setVisible(true);

    if (_ownInfo->dwUserID == betUser->dwUserID) {
        int changeTimes =
            UserDefault::getInstance()->getIntegerForKey("RecordBetTimes", 0);
        if (changeTimes >= 60)
            changeTimes = 60;
        UserDefault::getInstance()->setIntegerForKey("RecordBetTimes", changeTimes);

        //更新当前玩家在投注区域的金钱值
        std::string myBetStr = _myBetValInZone.at(pcene->iBetArea)->getString();
        myBetStr = StringUtils::format("%ld", atol(myBetStr.c_str()) +
                                       ChoumaValueV88[betType]);
        _myBetValInZone.at(pcene->iBetArea)->setString(myBetStr);
        _myBetValInZone.at(pcene->iBetArea)->setVisible(true);

        //计算我当前的总投注值
        _myBetMoney = _myBetMoney + ChoumaValueV88[betType];
    }

    //if (isFirstTouch) {
    //    isFirstTouch = false;
    //    _myBetTypeInTianZone.clear();
    //    _myBetTypeInDiZone.clear();
    //    _myBetTypeInXuanZone.clear();
    //    _myBetTypeInHuangZone.clear();
    //}

    //根据当前还可下的最大数目刷新界面的显示
    m_pBeginData->maxBetOfZone = pcene->iMaxZhu;
    setBetsClick(true);
}

//开牌状态
void GameTableUIV88::showCard(KAI_PAI *pcene) {
    checkBetPosBeforeSendCard();

	HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_START_SENDCARD_V88);
    this->savaCardValue(pcene->pai);
    //输赢区域赋值，里面即为倍数
    for (BYTE i = 0; i < BET_ZONE_NUM_V88; i++) {
        _rateDataInZone[i] = pcene->iComputePoint[i];
    }
    for (BYTE i = 0; i < BET_ZONE_NUM_V88 + 1; i++) {
        _cowValue[i] = pcene->cowValue[i];
    }

    _clockObj->start(m_pBeginData->iKaiPaiTime);
    _gameStatus = ASS_KAI_PAI;
    setBetsClick(false);
	_gameStateSpr->setTexture("v66/zi/14.png");
    this->faPai();
}

//发牌=========================
void GameTableUIV88::faPai() {
    createPokerCard(false);
    createCowNum();

    _i_cardIdx = -1;
    _j_cardIdx = -1;
    _i_cardIdx += 1;
    this->schedule(schedule_selector(GameTableUIV88::faPaiTimer), 0.24f);
}

void GameTableUIV88::createPokerCard(bool isSync) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            PokerCardV88 *poker;
            if (isSync) {
                poker = PokerCardV88::create(convertPoint(600.0, 353.0), _cardsPos[j][i],
                                          _cardsValue[j][i]);
            } else {
                poker = PokerCardV88::create(convertPoint(600.0, 353.0));
            }
            poker->setCardValue(_cardsValue[j][i]);
            poker->setVisible(false);
            this->addChild(poker);
            _cardsRes[i][j] = poker;
        }
    }
}

void GameTableUIV88::createCowNum() {
    Vec2 cowPos[5];
    cowPos[0] = convertGL(656, 260);
    for (int i = 1; i < 5; i++) {
        cowPos[i] = convertGL(270 + (i - 1) * 240, 468);
    }
    for (int i = 0; i < 5; i++) {
        auto cowNum = Sprite::create(
			StringUtils::format(BRNN_TABLERES_COWVALUE_V88, _cowValue[i]));
        cowNum->setPosition(cowPos[i]);
        cowNum->setVisible(false);
		this->addChild(cowNum, 10);
        _cowRes[i] = cowNum;

		auto cowNumBg = Sprite::create(BRNN_TABLERES_COWVALUE_BG_V88);
		cowNumBg->setPosition(cowPos[i]);
		cowNumBg->setVisible(false);
		this->addChild(cowNumBg, 9);
		_cowBgRes[i] = cowNumBg;

    }
}

void GameTableUIV88::faPaiTimer(float dt) {
    _j_cardIdx += 1;

    //_cardsRes[_i_cardIdx][_j_cardIdx]->setScale(0.2);
    if (!_cardsRes[0][0])
        return;
    _cardsRes[_i_cardIdx][_j_cardIdx]->setVisible(true);

    if (4 != _i_cardIdx) {
        _cardsRes[_i_cardIdx][_j_cardIdx]->FaMingPai(
            _cardsPos[_j_cardIdx][_i_cardIdx]);
    } else {
        _cardsRes[_i_cardIdx][_j_cardIdx]->FaAnPai(
            _cardsPos[_j_cardIdx][_i_cardIdx]);
    }

    //最后还原索引
    if (4 == _j_cardIdx) {
        _i_cardIdx += 1;
        _j_cardIdx = -1;
    }
    //发牌结束了，开牌
    if (5 == _i_cardIdx) {
        this->unschedule(schedule_selector(GameTableUIV88::faPaiTimer));
        this->scheduleOnce(schedule_selector(GameTableUIV88::kaiPai), 0.5);
    }
}

//结算
void GameTableUIV88::showSettlement(tagResult *pcene) {
    checkCardAndCowBeforeSettlement();
	_isNote = false;
    _gameStatus = ASS_RESULT;

//    _clockObj->setVisible(false);
//    _clockObj->runAction(Sequence::create(
//                             DelayTime::create(1.0),
//    CallFunc::create([&]() {
//        _clockObj->setVisible(true);
//    }), nullptr));

    memcpy(_latest72GameInfoArr, pcene->iResultInfo,
           sizeof(_latest72GameInfoArr));

    if (pcene->iZhuangFen != 0) {
        m_bankerFen = pcene->iZhuangFen;
        std::string bankerMoney;
        LabelAtlas* bankerMoneyLabel = nullptr;

        if (pcene->iZhuangFen > 0) { //庄赢钱了
            bankerMoney = StringUtils::format("%lld", m_bankerFen);
            bankerMoneyLabel = LabelAtlas::create(";" + bankerMoney, FNT_YELLOW_THREE_V66, 22, 28, '0');
            bankerMoneyLabel->setPosition(_bankerSeatUI->getPositionX(),
                                          _bankerSeatUI->getPositionY() -
                                          50 * _yScale);
        } else if (pcene->iZhuangFen < 0) {
            bankerMoney = StringUtils::format("%lld" , -m_bankerFen);
			bankerMoneyLabel = LabelAtlas::create(":" + bankerMoney, FNT_BLUE_ONE_V66, 22, 28, '0');
            bankerMoneyLabel->setPosition(_bankerSeatUI->getPosition());
        }

        bankerMoneyLabel->setAnchorPoint(Vec2(0.5, 0.5));
        bankerMoneyLabel->setName("bankerMoneyLabel");
        bankerMoneyLabel->setVisible(false);
        bankerMoneyLabel->runAction(Sequence::create(
        DelayTime::create(0.5), CallFunc::create([&]() {
            auto label = (LabelAtlas *)this->getChildByName("bankerMoneyLabel");
            label->setVisible(true);
        }),
        MoveBy::create(2, Point(0, 50)), RemoveSelf::create(), nullptr));
        this->addChild(bankerMoneyLabel);
    }

    if (pcene->iUserFen != 0) {
        m_myselfFen = pcene->iUserFen;
        std::string myMoney;
        LabelAtlas* myMoneyLabel = nullptr;
        if (pcene->iUserFen > 0) { //我赢钱了
            myMoney = StringUtils::format("%lld", m_myselfFen);
            myMoneyLabel =
                LabelAtlas::create(";" + myMoney, FNT_BLUE_ONE_V66, 22, 28, '0');
            myMoneyLabel->setPosition(_myMoneyLabel->getPositionX(),
                                      _myMoneyLabel->getPositionY() - 50 * _yScale);
        } else if (pcene->iUserFen < 0) {
            myMoney = StringUtils::format("%lld", -m_myselfFen);
            myMoneyLabel =
                LabelAtlas::create(":" + myMoney, FNT_BLUE_ONE_V66, 22, 28, '0');
            myMoneyLabel->setPosition(_myMoneyLabel->getPositionX(),
                                      _myMoneyLabel->getPositionY());
        }

        myMoneyLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
        myMoneyLabel->setName("myMoneyLabel");
        myMoneyLabel->setVisible(false);
//        myMoneyLabel->runAction(Sequence::create(
//        DelayTime::create(0.5f), CallFunc::create([&]() {
//            auto label = (LabelAtlas *)this->getChildByName("myMoneyLabel");
//            label->setVisible(true);
//        }),
//        MoveBy::create(2.0f, Point(0.0f, 50.0f)), RemoveSelf::create(),
//        nullptr));
        this->addChild(myMoneyLabel);
        //
        auto coin = FloatCoin::create();
        if (coin) {
            coin->openParent(this, _myMoneyLabel->getPosition() + Vec2(0, 50.0f), myMoneyLabel->getLocalZOrder());
            coin->updateCoin(m_myselfFen);
        }

    }

    updateSomeData();
    //当前客户端随机对庄家或者闲家一些礼物，为了促进玩家礼物消费和仿真，这些表现都假装是由无座玩家送出（1/3概率发）
    // if(0 == rand()%3) {
    //    int sendTimes = rand()%11 + 10;
    //    this->schedule(schedule_selector(GameTableUIV88::clientGiftRand), 0.3,
    //    sendTimes, 0);
    //}
}

void GameTableUIV88::waitForNoDealer(DENG_DAI *pcene) {
    m_pBeginData->iBankerSeatNo = -1; //庄家座位号
    updateUI();
    _clockObj->start(m_pBeginData->iFreeTime);
    _gameStatus = ASS_FREE_TIME;
    setBetsClick(false);
	_gameStateSpr->setTexture("v66/zi/18.png");
}

void GameTableUIV88::waitForIdle(DENG_DAI *pcene) {
    /*全部翻牌完后*/
    showAllResult();

    memcpy(_bUionBankerInfoArr, pcene->bUionBankerInfo,
           sizeof(_bUionBankerInfoArr));
    clearTable();
    setBetsClick(false);

    _gameStatus = ASS_FREE_TIME;
    _clockObj->start(m_pBeginData->iFreeTime);
	_gameStateSpr->setTexture("v66/zi/13.png");
}

void GameTableUIV88::showUnableBet() {
    setBetsClick(false);
    MsgTipV88::create(GetText("to_bet_limit"));
}

void GameTableUIV88::getOrQuitBanker(SHANG_ZHUANG *shangZhuanug) {
    updateAppliedBankerList(shangZhuanug->zhuangState);

    if (shangZhuanug->station == _GameTableLogic->getMySeatNo()) {
        if (!shangZhuanug->success) {
            if (HNPlatformLogic::getInstance()->loginResult.i64Money <
                    m_pBeginData->minMoneyOfApplyBanker &&
                    m_pBeginData->minMoneyOfApplyBanker > 0) {
                //        char str[128] = {0};
                //        sprintf(str, "上庄失败, 金币大于%lld才能上庄",
                //                m_pBeginData->minMoneyOfApplyBanker);
                MsgTipV88::create(StringUtils::format(GetText("apply_banker_fail"),
                                                   m_pBeginData->minMoneyOfApplyBanker)
                               .c_str());
            } else {
                MsgTipV88::create(GetText("been_in_banker_list"));
            }
            return;
        }
        if (shangZhuanug->shang) {
            MsgTipV88::create(GetText("apply_banker_success"));
            //bankerLayer->setBankerList(_vecBankerList);
        } else {
            if (_bankerInfo) {
                if (_bankerInfo->dwUserID == _ownInfo->dwUserID) {
                    MsgTipV88::create(GetText("next_auto_bet"));
                    return;
                }
            }
        }
    }
}

//系统消息
void GameTableUIV88::showSystemWords(char *talk) {
    // std::string msg = talk;

    // HNChatStructInfo info;
    // info.name = GBKToUtf8("【系统消息】");
    // info.words = GBKToUtf8(msg.c_str());
    // if(_vecChatList.size() > 50)
    //    _vecChatList.clear();
    //_vecChatList.push_back(info);

    //_chatTips->stopAllActions();
    //_chatTips->runAction(Actions::LoomingAction(0.5, 12));

    //_chatUserName->setString(info.name);
    //_chatLabel->setString(info.words);

    // if(_chatDialog) {
    //    _chatDialog->_isNomal = true;
    //    _chatDialog->updateUI(_vecChatList);
    //}
}

//玩家聊天
void GameTableUIV88::showUserWords(MSG_GR_RS_NormalTalk *normalTalk) {
    // std::string msg = normalTalk->szMessage;
    // int pos = msg.find("/:");
    // if (pos >= 0 && pos <= msg.size())		return;

    // for(int i=0; i<27; i++) { //目前有27种表情，0-26
    //    if(StringUtils::format("--%d--", i) == msg) {
    //        if(_bankerInfo) {
    //            if(_bankerInfo->dwUserID == normalTalk->dwSendID) {
    //                _bankerSeatUI->showEmotion(i);
    //                return;
    //            }
    //        }
    //        for (int j = 0; j<MAX_NON_BANKER_SEAT_NUM_V88; j++) {
    //            auto user = _vecPlayerSeatUI.at(j)->getUser();
    //            if(user) {
    //                if(user->dwUserID == normalTalk->dwSendID) {
    //                    _vecPlayerSeatUI.at(j)->showEmotion(i);
    //                    return;
    //                }
    //            }
    //        }
    //        break;
    //    }
    //}

    ////
    // UserInfoStruct* chatUser =
    // UserInfoModule()->findUser(normalTalk->dwSendID);
    // if(chatUser) {
    //    HNChatStructInfo info;
    //    info.name  = GBKToUtf8(chatUser->nickName);
    //    info.words = GBKToUtf8(normalTalk->szMessage);
    //    if(_vecChatList.size() > 50)
    //        _vecChatList.clear();
    //    _vecChatList.push_back(info);
    //    //
    //    //_chatTips->stopAllActions();
    //    //_chatTips->runAction(Actions::LoomingAction(0.5, 15));//0.05  5

    //    std::string talk = GBKToUtf8("【%s说：】");
    //    talk = StringUtils::format(talk.c_str(), GBKToUtf8(chatUser->nickName));
    //    _chatUserName->setString(talk);

    //    _chatLabel->setString(info.words);
    //    if(_chatDialog) {
    //        _chatDialog->updateUI(_vecChatList);
    //    }
    //    if(_bankerInfo) {
    //        if(_bankerInfo->dwUserID == normalTalk->dwSendID) {
    //            _bankerSeatUI->showWords(info.words);
    //            return;
    //        }
    //    }
    //    for(int j=0; j<MAX_NON_BANKER_SEAT_NUM_V88; j++) {
    //        auto user = _vecPlayerSeatUI.at(j)->getUser();
    //        if(user) {
    //            if(user->dwUserID == normalTalk->dwSendID) {
    //                _vecPlayerSeatUI.at(j)->showWords(info.words);
    //            }
    //        }
    //    }
    //}
}

void GameTableUIV88::giveGift(MSG_PROP_GIVE *pcene, int resultCode) {
    if (0 != resultCode) {
        if (2 == resultCode)
            MsgTipV88::create(GetText("give_fail"));
        return;
    }
    if (pcene->nPropID < 0 || pcene->nPropID > 5)
        return;

    // int priceArr[6] = {1000,1000,10000,100000,500000,5000000};
    int priceArr[5] = {1000, 10000, 100000, 1000000, 5000000};

    updateOtherGiftMovey(pcene->dwUserID, pcene->dwTargetID,
                         priceArr[pcene->nPropID]);

    //赠礼人信息和收礼人信息
    Point startPos = _noSeatBtn->getPosition();
    bool bSenderInTable = false;
    if (_bankerInfo) {
        if (_bankerInfo->dwUserID == pcene->dwUserID) {
            startPos = _bankerSeatUI->getPosition();
            bSenderInTable = true;
        }
    }
    if (!bSenderInTable) {
        for (int i = 0; i < MAX_NON_BANKER_SEAT_NUM_V88; i++) {
            auto user = _vecPlayerSeatUI.at(i)->getUser();
            if (user) {
                if (user->dwUserID == pcene->dwUserID) {
                    startPos = _vecPlayerSeatUI.at(i)->getPosition();
                    bSenderInTable = true;
                    break;
                }
            }
        }
    }
    if (!bSenderInTable) {
        if (pcene->dwUserID == PlatformLogic()->loginResult.dwUserID) {
            startPos = _myMoneyLabel->getPosition();
        }
    }
    if (pcene->dwUserID == PlatformLogic()->loginResult.dwUserID) {
        _ownInfo->i64Money -= priceArr[pcene->nPropID];
        MsgTipV88::create(
            StringUtils::format(GetText("give_sucess"), priceArr[pcene->nPropID])
            .c_str());
        updateMyData();
    }

    Point targetPos = _noSeatBtn->getPosition();
    bool bRecverInTable = false;
    if (_bankerInfo) {
        if (_bankerInfo->dwUserID == pcene->dwTargetID) {
            targetPos = _bankerSeatUI->getPosition();
            bRecverInTable = true;
        }
    }
    if (!bRecverInTable) {
        for (int i = 0; i < MAX_NON_BANKER_SEAT_NUM_V88; i++) {
            auto user = _vecPlayerSeatUI.at(i)->getUser();
            if (user) {
                if (user->dwUserID == pcene->dwTargetID) {
                    targetPos = _vecPlayerSeatUI.at(i)->getPosition();
                    bRecverInTable = true;
                    break;
                }
            }
        }
    }
    if (!bRecverInTable) {
        if (pcene->dwTargetID == PlatformLogic()->loginResult.dwUserID) {
            targetPos = _myMoneyLabel->getPosition();
        }
    }

    //this->giftMove(startPos, targetPos, pcene->nPropID);

    if (pcene->dwTargetID ==
            PlatformLogic()->loginResult.dwUserID) { //如果是我收到礼物 加钱
        if (pcene->nPropID >= 0) {
            auto myOldMoney = PlatformLogic()->loginResult.i64Money;
            auto getMoney = priceArr[pcene->nPropID];
            auto myNewMoney = myOldMoney + getMoney;
            PlatformLogic()->loginResult.i64Money = myNewMoney;
            RoomLogic()->loginResult.pUserInfoStruct.i64Money = myNewMoney;
            _ownInfo->i64Money = myNewMoney;
            MsgTipV88::create(
                StringUtils::format(GetText("receive_friend_gift"), getMoney)
                .c_str());
            updateMyData();
        }
    }
}

void GameTableUIV88::getUionBankerList(SHANG_ZHUANG_UION_LIST *pcene) {
    memcpy(_bUionBankerInfoArr, pcene->bUionBankerInfo,
           sizeof(_bUionBankerInfoArr));
}

//联合上庄上下庄
void GameTableUIV88::uionBankerUpOrDown(SHANG_ZHUANG_UION *pcene) {
    for (int i = 0; i < MAX_UION_COUNT; i++) {
        if (i == pcene->uionarea) {
            for (int j = 0; j < MAX_UION_COUNT; j++) {
                _bUionBankerInfoArr[i][j] = pcene->station[j];
            }
            // memcpy(_bUionBankerInfoArr[i], pcene->station,
            // sizeof(_bUionBankerInfoArr)/10);
            break;
        }
    }
    if (pcene->currentStation == _GameTableLogic->getMySeatNo()) {
        if (pcene->shang) {
            if (pcene->success) {
                MsgTipV88::create(GetText("apply_union_sucess"));
            } else {
                MsgTipV88::create(GetText("apply_union_fail"));
            }
        } else {
            if (255 - m_pBeginData->iBankerSeatNo == pcene->uionarea) {
                MsgTipV88::create(GetText("next_auto_bet"));
            } else {
                MsgTipV88::create(GetText("exit_union_sucess"));
            }
        }
    }
}

//玩家站起
// void GameTableUIV88::userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
//    /*for(int i=0; i<_vecPlayerSeatUI.size(); i++)
//    {
//      if(_vecPlayerSeatUI.at(i)->getUser())
//      {
//              if(_vecPlayerSeatUI.at(i)->getUser()->dwUserID ==
//              user->dwUserID)
//              {
//                      _vecPlayerSeatUI.at(i)->userExit();
//                      break;
//              }
//      }
//    }*/
//    /*updateUI();
//    if(_bUpForChangeSeat && user->dwUserID ==
//    PlatformLogic()->loginResult.dwUserID)
//    {
//      _GameTableLogic->sendUserSit(_changedSeatNum);
//    }*/
//}
void GameTableUIV88::setLIMIT(LLONG value) {
    _xiazhu = value;
}

//玩家坐下
void GameTableUIV88::userSitDown(MSG_GR_R_UserSit *userSit, UserInfoStruct *user) {
    updateUI();
}

void GameTableUIV88::showStartMes() {
    if (!_zhangqi) {
        char *message = "亲爱的用户，欢迎来到“聚乐牛牛”！";
        showSystemWords(message);
        char message2[200] = {0};
        if (_xiazhu == 0) {
            sprintf(message2, "本房间上庄需%lld，用户下注无上限",
                    m_pBeginData->minMoneyOfApplyBanker);
        } else {
            sprintf(message2, "本房间上庄需%lld，用户下注限额%lld",
                    m_pBeginData->minMoneyOfApplyBanker, _xiazhu);
        }
        showSystemWords(message2);
    }
}

void GameTableUIV88::changeSeat(BYTE byStation, BYTE byLastStation, BYTE seatNo) {
    betChangeStartPos(byStation, byLastStation, seatNo);

    auto deskUserList = _GameTableLogic->getDeskUserList();
    auto pUser = deskUserList->getUserByDeskStation(byStation);
    if (pUser) {
        if (pUser->dwUserID == PlatformLogic()->loginResult.dwUserID) {
            if (_mySeatIndex != -1) {
                _vecPlayerSeatUI.at(_mySeatIndex)->userExit();
            }
            _mySeatIndex = seatNo;
            _vecPlayerSeatUI.at(seatNo)->setUser(pUser);

            showBankerSign(_vecPlayerSeatUI.at(seatNo)->getPosition());
            //_bankerSeatUI->setUser(pUser);
        } else {
            if (byLastStation != 255) {
                _vecPlayerSeatUI.at(byLastStation)->userExit();
            }
            _vecPlayerSeatUI.at(seatNo)->setUser(pUser);
            //_bankerSeatUI->setUser(pUser);
        }

        // showBankerSign(_vecPlayerSeatUI.at(seatNo)->getPosition());
    }
}

void GameTableUIV88::playerStandUp(BYTE byStation, BYTE seatNo) {
    betChangeStartPos(byStation, seatNo);
    auto deskUserList = _GameTableLogic->getDeskUserList();
    auto pUser = deskUserList->getUserByDeskStation(byStation);
    if (pUser && pUser->dwUserID == PlatformLogic()->loginResult.dwUserID) {
        _mySeatIndex = -1;
    }
    if (_vecPlayerSeatUI.at(seatNo)->getUser()) {
        _vecPlayerSeatUI.at(seatNo)->userExit();
    }
}

void GameTableUIV88::reachMaxBetLimit() {
    char *message = "下注满，提前发牌 。";
    showSystemWords(message);
}

void GameTableUIV88::judgeZhuangJiaMessage() {
    //  log("judgeZhuangJiaMessage");
    //  this->runAction(Sequence::create(
    //      DelayTime::create(1.5), CallFunc::create([&]() {
    //        char message[200] = {0};
    //        if (m_pBeginData->iBankerSeatNo == SYS_BANKER_STATION_V88) {
    //          sprintf(message, "系统坐庄");
    //        } else {
    //          sprintf(message, "连庄次数:0");
    //        }
    //
    //        getRemainDealer()->setString(GBKToUtf8(message));
    //      }),
    //      nullptr));
}

void GameTableUIV88::updateTip(float dt) {
	int iTag = m_TextTip->getTag();
	if (iTag == 0) {
		unschedule(schedule_selector(GameTableUIV88::updateTip));
		_GameTableLogic->sendUserUp();
		return;
	}

	char str[128] = { 0 };
	iTag -= 1;
	sprintf(str, "金币数额低于%lld，%ds后离开游戏！", m_i64TipMoney, iTag);
	m_TextTip->setString(GBKToUtf8(str));
	m_TextTip->setTag(iTag);
}

void GameTableUIV88::onGameTipExit(S_C_TIP_EXIT *msg) {
	if (msg->bIsExit == 1) {
		auto layer = GamePromptLayer::create();
//		layer->setCloseExitBtn();
		char str[128] = { 0 };
		int sec = 3;
		sprintf(str, "金币数额低于%lld，%ds后离开游戏！", msg->i64Money, sec);
		layer->showPrompt(GBKToUtf8(str));

		layer->setOkayCallBack([&]() {
			unschedule(schedule_selector(GameTableUIV88::updateTip));
			_GameTableLogic->sendUserUp();
		});
		layer->setColorLayerTouchEnabled(false);

		m_i64TipMoney = msg->i64Money;
		m_TextTip = layer->getPromptText();
		m_TextTip->setTag(sec);
		schedule(schedule_selector(GameTableUIV88::updateTip), 1.0f);
	}
	else {

	}	
}

// end================

void GameTableUIV88::savaCardValue(BYTE card[5][5]) {
    //高四位是花色，低四位是大小
    memset(_cardsValue, 0, sizeof(_cardsValue));
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; ++j) {
            BYTE value = BYTE(0x0F & card[i][j]);
            BYTE color = BYTE(0xF0 & card[i][j]);
            if (0x0D == value) { // A
                value = 0x01;
            } else {
                value += 0x01;
            }
            value = color | value;
            _cardsValue[i][j] = value;
        }
    }
    HNLog::logDebug("——————");

    // BYTE poke[5][5] = {0x00};
    // memcpy(poke, _cardsValue, sizeof(poke));
    // for(int i=0; i<5; i++) {
    //    for(int j=0; j<5; j++) {
    //        poke[i][j] = BYTE(0x0F & poke[i][j]);
    //    }
    //}
    // int poker[5][5] = {0};
    // for(int i=0; i<5; i++) {
    //    for(int j=0; j<5; j++) {
    //        poker[i][j] = poke[i][j];
    //    }
    //    //_cowValue[i] = findCowValue(poker[i]);
    //}
}

int GameTableUIV88::findCowValue(int (&poke)[5]) {
    ////四炸
    // for (int i = 0; i < 5; i++) {
    //    for (int j = i + 1; j < 5; j++) {
    //        for (int k = j + 1; k < 5; k++) {
    //            for(int z = k + 1; z < 5; z++) {
    //                if(poke[i] == poke[j] == poke[k] == poke[z])
    //                    return 13;
    //            }
    //        }
    //    }
    //}

    if (poke[0] + poke[1] + poke[2] + poke[3] + poke[4] <= 10) //五小牛
        return 12;
    if (poke[0] >= 10 && poke[1] >= 10 && poke[2] >= 10 && poke[3] >= 10 &&
            poke[4] >= 10) //五花牛
        return 11;

    //整理牌
    for (int i = 0; i < 5; i++) {
        if (poke[i] > 10)
            poke[i] = 10;
    }

    //定义找牛所需的变量
    struct Cow {
        int cowNum;
    };
    int cowCount = 0; //结构体数组索引，也是当前牛的组合个数
    Cow cow[10];      //假定有十个，最多也就只有10个

    //开始找牛
    for (int i = 0; i < 5 - 2; i++) {
        for (int j = i + 1; j < 5 - 1; j++) {
            for (int k = j + 1; k < 5; k++) {
                if (0 == (poke[i] + poke[j] + poke[k]) % 10) { //找到了一个牛
                    //这里最好封装成一个函数/////////////////判断i,j,k之外的是什么牌
                    int except_i_j_k_data[5] = {0, 1, 2, 3, 4}; //判断i,j,k之外的数据所需
                    int remainderData[2];   //保存i,j,k之外的数据
                    int remainderCount = 0; //保存i,j,k之外的数据的数组索引
                    for (int t = 0; t < 5; t++) {
                        if (i == except_i_j_k_data[t] || j == except_i_j_k_data[t] ||
                                k == except_i_j_k_data[t]) {
                            continue;
                        }
                        remainderData[remainderCount] = t;
                        remainderCount++;
                    }

                    int cowNum = (poke[remainderData[0]] + poke[remainderData[1]]) % 10;
                    if (0 == cowNum) { //牛牛 必然是最大的 直接返回
                        return 10;
                    }

                    cow[cowCount].cowNum = cowNum; //存牛
                    cowCount++;
                }
            }
        }
    }

    //牛找完了，开始找最大的牛，找最大牛前判断当前有没有找到牛
    if (0 == cowCount) {
        return 0;
    } else if (1 == cowCount) {
        return cow[0].cowNum;
    }

    for (int c1 = 0; c1 < cowCount - 1; c1++) {
        for (int c2 = c1 + 1; c2 < cowCount; c2++) {
            if (cow[c1].cowNum > cow[c2].cowNum) {
                Cow tmp;
                tmp.cowNum = cow[c2].cowNum;
                cow[c2].cowNum = cow[c1].cowNum;
                cow[c1].cowNum = tmp.cowNum;
            }
        }
    }
    return cow[cowCount - 1].cowNum;
}

void GameTableUIV88::recoveryOpenCardScene() {
    createPokerCard(true);
    createCowNum();
    this->showAllResult();
}

// end===================

//开牌=========================
void GameTableUIV88::kaiPai(float dt) {
    // eastFlipLastPoke(2);
    this->scheduleOnce(schedule_selector(GameTableUIV88::bankerFlipLastPoke), 0.2f);
    this->scheduleOnce(schedule_selector(GameTableUIV88::eastFlipLastPoke), 1.4f);
    this->scheduleOnce(schedule_selector(GameTableUIV88::southFlipLastPoke), 2.6f);
    this->scheduleOnce(schedule_selector(GameTableUIV88::westernFlipLastPoke), 3.8f);
    this->scheduleOnce(schedule_selector(GameTableUIV88::northFlipLastPoke), 5.0f);
}

void GameTableUIV88::eastFlipLastPoke(float dt) {
    if (_cardsRes[4][1])
        _cardsRes[4][1]->FlipPoke();
    this->scheduleOnce(schedule_selector(GameTableUIV88::showEastCardResult), 0.5);
}

void GameTableUIV88::southFlipLastPoke(float dt) {
    if (_cardsRes[4][2])
        _cardsRes[4][2]->FlipPoke();
    this->scheduleOnce(schedule_selector(GameTableUIV88::showSouthCardResult), 0.5);
}

void GameTableUIV88::westernFlipLastPoke(float dt) {
    if (_cardsRes[4][3])
        _cardsRes[4][3]->FlipPoke();
    this->scheduleOnce(schedule_selector(GameTableUIV88::showWesternCardResult),
                       0.5);
}

void GameTableUIV88::northFlipLastPoke(float dt) {
    if (_cardsRes[4][4])
        _cardsRes[4][4]->FlipPoke();
    this->scheduleOnce(schedule_selector(GameTableUIV88::showNorthCardResult), 0.5);
}

void GameTableUIV88::bankerFlipLastPoke(float dt) {
    if (_cardsRes[4][0])
        _cardsRes[4][0]->FlipPoke();
    this->scheduleOnce(schedule_selector(GameTableUIV88::showBankerCardResult), 0.5);


}

void GameTableUIV88::showEastCardResult(float dt) {
    showCardResult(1);
}

void GameTableUIV88::showSouthCardResult(float dt) {
    showCardResult(2);
}

void GameTableUIV88::showWesternCardResult(float dt) {
    showCardResult(3);
}

void GameTableUIV88::showNorthCardResult(float dt) {
    showCardResult(4);
}

void GameTableUIV88::showBankerCardResult(float dt) {
    showCardResult(0);
}

void GameTableUIV88::showCardResult(int zoneNo) {
    HNAudioEngine::getInstance()->playEffect(
		StringUtils::format(BRNN_MUSIC_COWNUM_V88, _cowValue[zoneNo]).c_str());

	if (_cowValue[zoneNo] >= 1 && _cowValue[zoneNo] <= 10)
	{
		//播放牛动画
		auto cowAnimation = CSLoader::createNode(BRNN_TABLERES_COW_ANIMATION_V88);
		cowAnimation->setPosition(_cowRes[zoneNo]->getPosition());
		cowAnimation->setTag(zoneNo * 10);
		this->addChild(cowAnimation, 9);

		auto cowCartoon = CSLoader::createTimeline(BRNN_TABLERES_COW_ANIMATION_V88);
		cowAnimation->runAction(cowCartoon);
		cowCartoon->play("shuziniu", false);
	}
	else if (_cowValue[zoneNo] == 0)
	{
		_cowBgRes[zoneNo]->setVisible(true);
	}
	

	_cowRes[zoneNo]->setVisible(true);
	//_cowBgRes[zoneNo]->setVisible(true);
	_cowRes[zoneNo]->setScale(3);
	_cowRes[zoneNo]->runAction(ActionsV88::SmallToBig());

    if (_cowValue[zoneNo] == 11) {
        showCowRun();
    }
}
// end=========================

void GameTableUIV88::showAllResult() {
    if (_FuctionTimes)
        return;
    _FuctionTimes++;

    //    char name[30] = {0};
    for (int i = 0; i < BET_ZONE_NUM_V88; i++) {
        auto multipleSprite = Sprite::create(
			StringUtils::format(BRNN_TABLERES_MULTIPLE_V88, _rateDataInZone[i]));
        if (multipleSprite) { //服务器数据直接索引资源，以防数据错误引起空指针
            multipleSprite->setPosition(_vecWinTipSprite.at(i)->getPosition());
            multipleSprite->setScale(0);
            multipleSprite->runAction(ActionsV88::SmallToBig());
            this->addChild(multipleSprite);
            /*sprintf(name, "multipleSprite%d", i);
               multipleSprite->setName(name);*/
            _rateResInZone[i] = multipleSprite;
        }
        if (_rateDataInZone[i] > 0) {
			_vecWinTipSprite.at(i)->runAction(ActionsV88::LoomingAction(0.7f, 5.0f));
        }
    }
}

//"清除"操作=========================

void GameTableUIV88::clearTheNewGameData() {
    _i_cardIdx = -1;
    _j_cardIdx = -1;
    _myBetMoney = 0;
    _FuctionTimes = 0;

    size_t i = 0;
    for (i = 0; i < 5; i++) {
		if (_cowRes[i])
		{
			_cowRes[i]->runAction(ActionsV88::SmallAndRemove());
			_cowBgRes[i]->runAction(ActionsV88::SmallAndRemove());
			auto p = getChildByTag(i * 10);
			if (p)
			{
				p->runAction(ActionsV88::SmallAndRemove());
			}
		}

    }
    for (i = 0; i < 4; i++) {
        if (_rateResInZone[i])
			_rateResInZone[i]->runAction(ActionsV88::SmallAndRemove());
    }
    for (i = _betsInTianZone.size(); i > 0; i--) {
        backEastZoneBet(0);
    }
    for (i = _betsInDiZone.size(); i > 0; i--) {
        backSouthZoneBet(0);
    }
    for (i = _betsInXuanZone.size(); i > 0; i--) {
        backWesternZoneBet(0);
    }
    for (i = _betsInHuangZone.size(); i > 0; i--) {
        backNorthZoneBet(0);
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (_cardsRes[i][j]) {
                _cardsRes[i][j]->setCardValue(0);
                _cardsRes[i][j]->reverseFlipPokeAndBack(0);
            }
        }
    }

    memset(_cardsValue, 0, sizeof(_cardsValue));
    memset(_cardsRes, 0, sizeof(_cardsRes));
    memset(_cowValue, 0, sizeof(_cowValue));
    memset(_cowRes, 0, sizeof(_cowRes));
	memset(_cowBgRes, 0, sizeof(_cowBgRes));
    memset(_rateResInZone, 0, sizeof(_rateResInZone));
    memset(_rateDataInZone, 0, sizeof(_rateDataInZone));

    _betsInTianZone.clear();
    _betsInDiZone.clear();
    _betsInXuanZone.clear();
    _betsInHuangZone.clear();
    //_myBetTypeInTianZone.clear();
    //_myBetTypeInDiZone.clear();
    //_myBetTypeInXuanZone.clear();
    //_myBetTypeInHuangZone.clear();

    _ownInfo = UserInfoModule()->findUser(PlatformLogic()->loginResult.dwUserID);
}

void GameTableUIV88::clearTable() {
    //精灵移除，数据更新，界面还原
    this->backZoneBet();

    if (_cardsRes[0][0]) {
        _cardsRes[0][0]->_removeCallback = [&]() {
            memset(_cardsRes, 0, sizeof(_cardsRes));
        };
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (_cardsRes[i][j]) {
                _cardsRes[i][j]->setCardValue(0);
                _cardsRes[i][j]->reverseFlipPokeAndBack();
            }
        }
    }

    this->scheduleOnce(schedule_selector(GameTableUIV88::LastClean), 8.0);

    //做输赢特效和界面跟新，数据还原在新一轮的时候再去还原
}

void GameTableUIV88::backZoneBet() {
    size_t times1 = _betsInTianZone.size();
    size_t times2 = _betsInDiZone.size();
    size_t times3 = _betsInXuanZone.size();
    size_t times4 = _betsInHuangZone.size();
    float delayTime1 = 0.01f;
    float delayTime2 = 0.01f;
    float delayTime3 = 0.01f;
    float delayTime4 = 0.01f;
    if (times1 > 300) {
        delayTime1 = 3.0f / times1;
    }
    if (times2 > 300) {
        delayTime2 = 3.0f / times2;
    }
    if (times3 > 300) {
        delayTime3 = 3.0f / times3;
    }
    if (times4 > 300) {
        delayTime4 = 3.0f / times4;
    }
    float yanshi[4];
    for (int i = 0; i < 4; i++) {
        if (_rateDataInZone[i] > 0) {
            yanshi[i] = 2.0;
        } else {
            yanshi[i] = 3.0;
        }
    }

    this->schedule(schedule_selector(GameTableUIV88::backEastZoneBet), delayTime1,
                   (unsigned int)times1, yanshi[0]);
    this->schedule(schedule_selector(GameTableUIV88::backSouthZoneBet), delayTime2,
                   (unsigned int)times2, yanshi[1]);
    this->schedule(schedule_selector(GameTableUIV88::backWesternZoneBet), delayTime3,
                   (unsigned int)times3, yanshi[2]);
    this->schedule(schedule_selector(GameTableUIV88::backNorthZoneBet), delayTime4,
                   (unsigned int)times4, yanshi[3]);

    bool isWin = false;
    bool isLose = false;
    for (int i = 0; i < 3; i++) {
        if (yanshi[i] == 2.0) {
            isWin = true;
        }
        if (yanshi[i] - yanshi[i + 1] != 0) {
            isLose = true;
        }
    }
    if (isWin && isLose) {
        this->runAction(Sequence::create(
        DelayTime::create(2.0), CallFunc::create([&]() {
            playCleanBet();
        }),
        /*DelayTime::create(3.0), CallFunc::create([&]() {
            playCleanBet();
        }),*/
        nullptr));
    } else if (isWin && !isLose) {
        this->runAction(
            Sequence::create(DelayTime::create(2.0),
        CallFunc::create([&]() {
            playCleanBet();
        }), nullptr));
    } else if (!isWin && !isLose) {
        this->runAction(
            Sequence::create(DelayTime::create(5.0),
        CallFunc::create([&]() {
            playCleanBet();
        }), nullptr));
    }
}

void GameTableUIV88::backEastZoneBet(float dt) {
    if (_betsInTianZone.size() < 1)
        return;

    if (!isPlayeTian) {
        isPlayeTian = true;
		HNAudioEngine::getInstance()->playEffect(BRNN_MUSIC_DROPGOLD_V88);
    }
    //筹码飞回去时要在牌的上面，所以修改筹码的层级
    _betsInTianZone.at(_betsInTianZone.size() - 1)->setLocalZOrder(1);
    if (_rateDataInZone[TIAN_AREA_V88] > 0) {
        _betsInTianZone.at(_betsInTianZone.size() - 1)->moveBack();
    } else {
		Vec2 i = _bankerSeatUI->getPosition();
        _betsInTianZone.at(_betsInTianZone.size() - 1)
        ->moveToBanker(_bankerSeatUI->getPosition() );
    }
    _betsInTianZone.popBack();
}

void GameTableUIV88::backSouthZoneBet(float dt) {
    if (_betsInDiZone.size() < 1)
        return;

    if (!isPlayDi) {
        isPlayDi = true;
		HNAudioEngine::getInstance()->playEffect(BRNN_MUSIC_DROPGOLD_V88);
    }
    //筹码飞回去时要在牌的上面，所以修改筹码的层级
    _betsInDiZone.at(_betsInDiZone.size() - 1)->setLocalZOrder(1);
    if (_rateDataInZone[DI_AREA_V88] > 0)
        _betsInDiZone.at(_betsInDiZone.size() - 1)->moveBack();
    else
		Vec2 i = _bankerSeatUI->getPosition();
        _betsInDiZone.at(_betsInDiZone.size() - 1)
        ->moveToBanker(_bankerSeatUI->getPosition() );
    _betsInDiZone.popBack();
}

void GameTableUIV88::backWesternZoneBet(float dt) {
    if (_betsInXuanZone.size() < 1)
        return;

    if (!isPlayXuan) {
        isPlayXuan = true;
		HNAudioEngine::getInstance()->playEffect(BRNN_MUSIC_DROPGOLD_V88);
    }
    //筹码飞回去时要在牌的上面，所以修改筹码的层级
    _betsInXuanZone.at(_betsInXuanZone.size() - 1)->setLocalZOrder(1);
    if (_rateDataInZone[XUAN_AREA_V88] > 0)
        _betsInXuanZone.at(_betsInXuanZone.size() - 1)->moveBack();
    else
		Vec2 i = _bankerSeatUI->getPosition();
        _betsInXuanZone.at(_betsInXuanZone.size() - 1)
        ->moveToBanker(_bankerSeatUI->getPosition() );
    _betsInXuanZone.popBack();
}

void GameTableUIV88::backNorthZoneBet(float dt) {
    if (_betsInHuangZone.size() < 1)
        return;

    if (!isPlayHuang) {
        isPlayHuang = true;
		HNAudioEngine::getInstance()->playEffect(BRNN_MUSIC_DROPGOLD_V88);
    }
    //筹码飞回去时要在牌的上面，所以修改筹码的层级
    _betsInHuangZone.at(_betsInHuangZone.size() - 1)->setLocalZOrder(1);
    if (_rateDataInZone[HUANG_AREA_V88] > 0)
        _betsInHuangZone.at(_betsInHuangZone.size() - 1)->moveBack();
    else
		Vec2 i = _bankerSeatUI->getPosition();
        _betsInHuangZone.at(_betsInHuangZone.size() - 1)
 			->moveToBanker(_bankerSeatUI->getPosition());
    _betsInHuangZone.popBack();
}

void GameTableUIV88::LastClean(float time) {
	HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_CLEAR_CARD_V88);
    //    char name[30] = {0};
    for (int i = 0; i < 5; i++) {
		if (_cowRes[i])
		{
			_cowRes[i]->runAction(ActionsV88::SmallAndRemove());
			_cowBgRes[i]->runAction(ActionsV88::SmallAndRemove());
			auto p = getChildByTag(i * 10);
			if (p)
			{
				p->runAction(ActionsV88::SmallAndRemove());
			}
		}
           
    }
    for (int i = 0; i < 4; i++) {
        if (_rateResInZone[i])
			_rateResInZone[i]->runAction(ActionsV88::SmallAndRemove());
        /*else
           {
                sprintf(name, "multipleSprite%d", i);
                auto sprite = (Sprite*)this->getChildByName(name);
                if (sprite)
                {
                        sprite->runAction(Actions::SmallAndRemove());
                }
           }*/
    }

    LLONG allBetVal[BET_ZONE_NUM_V88] = {0, 0, 0, 0};
    LLONG myBetVal[BET_ZONE_NUM_V88] = {0, 0, 0, 0};
    setBetInZoneVisible(false, allBetVal, myBetVal);

    memset(_cowRes, 0, sizeof(_cowRes));
	memset(_cowBgRes, 0, sizeof(_cowBgRes));
    memset(_rateResInZone, 0, sizeof(_rateResInZone));
}

void GameTableUIV88::setBetInZoneVisible(bool isVisible,
                                      LLONG allBetVal[MAX_NOTE_COUNT],
                                      LLONG myBetVal[MAX_NOTE_COUNT]) {
    for (int i = 0; i < BET_ZONE_NUM_V88; i++) {
        if (_allBetValInZone.at(i)) {
            _allBetValInZone.at(i)->setString(
                StringUtils::format("%lld", allBetVal[i]));
            _allBetValInZone.at(i)->setVisible((allBetVal[i] > 0));
        }
        if (_myBetValInZone.at(i)) {
            _myBetValInZone.at(i)->setString(
                StringUtils::format("%lld", myBetVal[i]));
            _myBetValInZone.at(i)->setVisible((myBetVal[i] > 0));
        }
    }
}

void GameTableUIV88::checkAllBeforeXiaZhu() {
    //removeNianShouButton();
    stopAllActions();
    unscheduleAllCallbacks();

    cleanCardAndCowAndMultipleSprite();
    cleanMoneyLabel();
    cleanBet();
    cleanData();
}

void GameTableUIV88::cleanBet() {
    for (auto m : _betsInTianZone) {
        m->removeFromParent();
    }
    for (auto m : _betsInDiZone) {
        m->removeFromParent();
    }
    for (auto m : _betsInXuanZone) {
        m->removeFromParent();
    }
    for (auto m : _betsInHuangZone) {
        m->removeFromParent();
    }

    _betsInTianZone.clear();
    _betsInDiZone.clear();
    _betsInXuanZone.clear();
    _betsInHuangZone.clear();
    //_myBetTypeInTianZone.clear();
    //_myBetTypeInDiZone.clear();
    //_myBetTypeInXuanZone.clear();
    //_myBetTypeInHuangZone.clear();
}

void GameTableUIV88::cleanMoneyLabel() {
    auto bankerMoneyLabel =
        (LabelAtlas *)this->getChildByName("bankerMoneyLabel");
    if (bankerMoneyLabel) {
        bankerMoneyLabel->removeFromParent();
    }

    auto myMoneyLabel = (LabelAtlas *)this->getChildByName("myMoneyLabel");
    if (myMoneyLabel) {
        myMoneyLabel->removeFromParent();
    }
}

void GameTableUIV88::cleanCardAndCowAndMultipleSprite() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (_cardsRes[i][j]) {
                _cardsRes[i][j]->stopAllActions();
                _cardsRes[i][j]->removeFromParent();
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        if (_cowRes[i]) {
            _cowRes[i]->removeFromParent();
			_cowBgRes[i]->removeFromParent();
			auto p = getChildByTag(i * 10);
			if (p)
			{
				p->runAction(ActionsV88::SmallAndRemove());
			}
        }
    }

    for (int i = 0; i < 4; i++) {
        if (_rateResInZone[i]) {
            _rateResInZone[i]->removeFromParent();
        }
    }

    memset(_cardsValue, 0, sizeof(_cardsValue));
    memset(_cardsRes, 0, sizeof(_cardsRes));
    memset(_cowValue, 0, sizeof(_cowValue));
    memset(_cowRes, 0, sizeof(_cowRes));
	memset(_cowBgRes, 0, sizeof(_cowBgRes));
    memset(_rateResInZone, 0, sizeof(_rateResInZone));
    memset(_rateDataInZone, 0, sizeof(_rateDataInZone));
}

void GameTableUIV88::cleanData() {
    LLONG allBetVal[BET_ZONE_NUM_V88] = {0, 0, 0, 0};
    LLONG myBetVal[BET_ZONE_NUM_V88] = {0, 0, 0, 0};
    setBetInZoneVisible(false, allBetVal, myBetVal);

    //_betsInTianZone.clear();
    //_betsInDiZone.clear();
    //_betsInXuanZone.clear();
    //_betsInHuangZone.clear();
    //_myBetTypeInTianZone.clear();
    //_myBetTypeInDiZone.clear();
    //_myBetTypeInXuanZone.clear();
    //_myBetTypeInHuangZone.clear();
}
// end

bool GameTableUIV88::isEnableRepeatBet() {
    return true;
}

int GameTableUIV88::isMySeatInBankerList() {
    for (UINT i = 0; i < _vecBankerList.size(); i++) {
        if (_ownInfo->dwUserID == _vecBankerList[i].dwUserId)
            return 1;
    }

    for (int i = 0; i < MAX_UION_COUNT; i++) {
        for (int j = 0; j < MAX_UION_COUNT; j++) {
            if (-1 == _bUionBankerInfoArr[i][j])
                break;
            if (_ownInfo->bDeskStation == _bUionBankerInfoArr[i][j])
                return 2;
        }
    }
    return 0;
}

int GameTableUIV88::getSelectBet() {
    for (int j = 0; j < MAX_JETTON_TYPE_V88; j++) {
        if (_vecBetObj.at(j)->isSelect()) {
            return j;
        }
    }
    return -1;
}

int GameTableUIV88::getBetMoney(int type) {
    // int betArr[5] = { 1000, 10000, 100000, 1000000, 5000000 };
    if (0 <= type && type <= 4) {
        return ChoumaValueV88[type];
    } else {
        return 0;
    }
}

void GameTableUIV88::playCleanBet() {
	HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_CLEANBET_V88);

    if (_myBetMoney != 0) {
        auto settlementLayer = BRNNSettlementLayerV88::create();
        settlementLayer->setBankerMoney(StringUtils::format("%lld", m_bankerFen).c_str());
        settlementLayer->setMyselfMoney(StringUtils::format("%lld", m_myselfFen).c_str());
        settlementLayer->setPosition(Vec2((WIN_SIZE.width / 2), (WIN_SIZE.height / 2)));
        addChild(settlementLayer, settlement_layer);
    }
}

void GameTableUIV88::setBetsClick(bool bClick) {
    for (int i = 0; i < _vecBetObj.size(); i++) {
        if (_bankerInfo) {
            if (_bankerInfo->dwUserID ==
                    _ownInfo->dwUserID) { //如果自己为庄，不可下注
                _vecBetObj.at(i)->setClick(false);
                continue;
            }
        }
        if (bClick) {
            //这里得根据实际情况处理一下，比如庄家金币情况和玩家金币情况
            if ((m_pBeginData->maxBetOfZone != 0 &&
                    m_pBeginData->maxBetOfZone < getBetMoney(i)) ||
                    _ownInfo->i64Money < LLONG(getBetMoney(i)) * m_pBeginData->maxRate) {
                if (i == getSelectBet()) {
                    _vecBetObj.at(i)->setSelect(true);
                }
                _vecBetObj.at(i)->setClick(false);
            } else {
                _vecBetObj.at(i)->setClick(true);
                if (_ownInfo->i64Money <
                        (_myBetMoney + getBetMoney(i)) * m_pBeginData->maxRate) {
                    _vecBetObj.at(i)->setClick(false);
                }

                bool bHaveClickBet = false;
                for (int j = 0; j < 5; j++) {
                    if (_vecBetObj.at(j)->isClick()) {
                        bHaveClickBet = true;
                        break;
                    }
                }
                if (-1 == getSelectBet() && bHaveClickBet) {
                    //_vecBetObj.at(0)->setSelect(true);
                }
            }
        } else {
            _vecBetObj.at(i)->setClick(false);
            _vecBetObj.at(i)->setSelect(false);
        }
    }
}

void GameTableUIV88::updateMyData() {
    if (_myMoneyLabel) {
        _myMoneyLabel->setString(Tools::addComma(_ownInfo->i64Money));
    }
    for (int i = 0; i < _vecPlayerSeatUI.size(); i++) {
        UserInfoStruct *user = _vecPlayerSeatUI.at(i)->getUser();
        if (user) {
            if (user->dwUserID == _ownInfo->dwUserID)
                _vecPlayerSeatUI.at(i)->updateUser();
        }
    }
}

//赠送礼物的逻辑==================

void GameTableUIV88::updateOtherGiftMovey(int sendId, int targetId, int money) {
    auto deskUserList = _GameTableLogic->getDeskUserList();
    if (sendId != _ownInfo->dwUserID) {
        UserInfoStruct *user = deskUserList->getUserByUserID(sendId);
        if (user)
            user->i64Money -= money;
    }
    if (targetId != _ownInfo->dwUserID) {
        UserInfoStruct *user = deskUserList->getUserByUserID(targetId);
        if (user)
            user->i64Money += money;
    }
    for (int i = 0; i < _vecPlayerSeatUI.size(); i++) {
        if (_vecPlayerSeatUI.at(i)->getUser()) {
            if (_vecPlayerSeatUI.at(i)->getUser()->dwUserID == sendId ||
                    _vecPlayerSeatUI.at(i)->getUser()->dwUserID == targetId) {
                _vecPlayerSeatUI.at(i)->updateUser();
            }
        }
    }
    if (_bankerInfo) {
        if (_bankerInfo->dwUserID == sendId || _bankerInfo->dwUserID == targetId) {
            _bankerSeatUI->updateUser();
        }
    }
}
//void GameTableUIV88::giftMove(Point startPos, Point targetPos, int giftId) {
//    if (giftId < 0 || giftId > 5)
//        return;
//
//    auto gift = Sprite::create(StringUtils::format(BRNN_GIFT, giftId));
//    if (gift) {
//        gift->setPosition(startPos);
//        this->addChild(gift, 10000);
//        gift->setScale(0);
//        gift->runAction(Sequence::create(
//                            ScaleTo::create(0.3f, 1.0f), DelayTime::create(0.5f),
//                            MoveTo::create(0.5f, targetPos), DelayTime::create(0.5f),
//                            Spawn::create(ScaleTo::create(0.3f, 0.5f), FadeOut::create(0.3f),
//                                          nullptr),
//                            RemoveSelf::create(), nullptr));
//    }
//}
// end

void GameTableUIV88::checkBetPosBeforeSendCard() {
    stopAllActions();
    setBetEndPos(_betsInTianZone);
    setBetEndPos(_betsInDiZone);
    setBetEndPos(_betsInXuanZone);
    setBetEndPos(_betsInHuangZone);
}

void GameTableUIV88::setBetEndPos(Vector<BetV88 *> &betList) {
    for (auto &m : betList) {
        m->stopAllActions();
        m->setPosition(
            Vec2(m->_xiazhuAreaPos.x, m->_xiazhuAreaPos.y));
    }
}

void GameTableUIV88::checkCardAndCowBeforeSettlement() {
    //removeNianShouButton();
    stopAllActions();
    unscheduleAllCallbacks();

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (_cardsRes[i][j]) {
                _cardsRes[i][j]->stopAllActions();
                _cardsRes[i][j]->runAction(
                    OrbitCamera::create(0.01f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f));
                _cardsRes[i][j]->ChangePoke();
                _cardsRes[i][j]->setPosition(_cardsPos[j][i]);
                _cardsRes[i][j]->setScale(1.0);
                _cardsRes[i][j]->setVisible(true);
            }
        }
    }

    for (int i = 0; i < 5; i++) {
        if (_cowRes[i]) {
            _cowRes[i]->setVisible(true);
			//_cowBgRes[i]->setVisible(true);
        }
    }
}

bool GameTableUIV88::hasLastBet() {
    if (_myBetTypeInTianZone.empty() && _myBetTypeInDiZone.empty() &&
            _myBetTypeInXuanZone.empty() && _myBetTypeInHuangZone.empty()) {
        MsgTipV88::create(GetText("last_no_bet"));
        return false;
    }
    return true;
}

//初始化资源==================================
bool GameTableUIV88::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }

    // HNAudioEngine::getInstance()->playBackgroundMusic(BRNN_MUSIC_PLAYGAME_V88,
    // true);
    HNAudioEngine::getInstance()->pauseBackgroundMusic();

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan = CC_CALLBACK_2(GameTableUIV88::onTouchBegan, this);
	myListener->setSwallowTouches(false);
    dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

    _tableSize = WIN_SIZE;

    //    auto visibleSize = Director::getInstance()->getVisibleSize();

    //_xScale = Director::getInstance()->getOpenGLView()->getFrameSize().width/
    // 1280.0f;
    //_xScale = 2.0f;
    //_yScale = visibleSize.height / 720.0f;
    //_yScale = Director::getInstance()->getOpenGLView()->getFrameSize().height /
    // 720.0f;
    // if (_xScale>_yScale) {
    //    _xScale = _yScale;
    //} else {
    //    _yScale = _xScale;
    //}

    GLView *glview = Director::getInstance()->getOpenGLView();
    // Size frameSize = glview->getFrameSize();
    // Size WIN_SIZE = Size(1280.0f, 720.0f);
    // float widthRate = frameSize.width / WIN_SIZE.width;
    // float heightRate = frameSize.height / WIN_SIZE.height;

    // if (widthRate > heightRate) {

    if (glview->getResolutionPolicy() == ResolutionPolicy::FIXED_WIDTH) {
        _xScale = 1.0f;
        _yScale = WIN_SIZE.height /
                  PlatformConfig::getInstance()->getGameDesignSize().height;
    }

    //_yScale = heightRate / widthRate;
    // glview->setDesignResolutionSize(WIN_SIZE.width, WIN_SIZE.height*heightRate /
    // widthRate, kResolutionNoBorder);
    //} else {
    // glview->setDesignResolutionSize(WIN_SIZE.width*widthRate / heightRate,
    // WIN_SIZE.height, kResolutionNoBorder);
    //}

    /*  GLView* glview = Director::getInstance()->getOpenGLView();

       Size frameSize = glview->getFrameSize();
       Size lsSize = Size(1280.0f, 720.0f);

       float scaleX = frameSize.width / lsSize.width;
       float scaleY = frameSize.height / lsSize.height;

       float scale = 0.0f;
       if (scaleX > scaleY) {
       scale = scaleX / (frameSize.height / lsSize.height);
       } else {
       scale = scaleY / (frameSize.width / lsSize.width);
       }
       _xScale = _yScale = scale;*/

	_GameTableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate);

    loadRes();
    initTable();
    initCardPos();
    initSeats();
    initPlayerInfo();
    initButtons();
	createToolBarUI();
    // initChat();

	auto betBg = Sprite::create(BRNN_GAME_TABLERES_BET_BG_V88);
	betBg->setPosition(Vec2(WINSIZE.width*0.5f, betBg->getContentSize().height / 2));
	this->addChild(betBg);

    //_GameTableLogic->initTableLogicData();
    _GameTableLogic->sendGameInfo();

	gameStart = CSLoader::createNode("v88/animation/gamestart/gamestart.csb");
	gameStart->setPosition(Vec2(WIN_SIZE.width*0.5f, WIN_SIZE.height * 0.5f));
	gameStart->setVisible(false);
	this->addChild(gameStart, 10);

	gameStartAnimation = CSLoader::createTimeline("v88/animation/gamestart/gamestart.csb");
	gameStart->runAction(gameStartAnimation);

    //雷达监测
    playScanner();
    
    return true;
}

void GameTableUIV88::loadRes() {
    m_pBeginData = new BEGIN_DATA();
    //输赢记录标记
    Director::getInstance()->getTextureCache()->addImage(BRNN_TABLERES_RECORDWIN_V88);
    Director::getInstance()->getTextureCache()->addImage(
		BRNN_TABLERES_RECORDLOSE_V88);
    //区域赢的提示
    //Director::getInstance()->getTextureCache()->addImage(
        //BRNN_TABLERES_WINZONETIP_V88);

	for (int i = 9; i < 13; i++)
	{
		Director::getInstance()->getTextureCache()->addImage(
			StringUtils::format(BRNN_TABLERES_WINZONETIP_V88, i));
	}

    //投注币
    for (int i = 0; i < 5; i++) {
        Director::getInstance()->getTextureCache()->addImage(
            StringUtils::format(BRNN_TABLERES_BET_V88, i));
    }
}

void GameTableUIV88::initCardPos() {
    // 50 68
    int x[] = {580, 195, 430, 665, 910};
    int y[] = {190, 405, 405, 405, 405};
    int offx = 30;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            _cardsPos[i][j] =
                Point(convertGL(x[i] + j * offx + 50 / 2, y[i] + 68 / 2));
        }
    }
}

void GameTableUIV88::initTable() {
    auto director = Director::getInstance();

	_tableBg = Sprite::create(GAME_BG_BAIRENNIUNIU_V66);
    _tableBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _tableBg->setPosition(WIN_SIZE/2);
//    _tableBg->setScale(_xScale, _yScale);
    this->addChild(_tableBg, 0);

	const char *path[4] = { BRNN_TABLERES_TIAN_V88, BRNN_TABLERES_DI_V88,
                           BRNN_TABLERES_XUAN_V88, BRNN_TABLERES_HUANG_V88
                          };
    //天地玄黄四个区域赢牌提示sprite
    float winTipPosXArr[BET_ZONE_NUM_V88];
	float winFramePosXArr[BET_ZONE_NUM_V88];
    for (int i = 0; i < 4; i++) {
		winTipPosXArr[i] = 274 + i * 234;
    }
	for (int i = 0; i < 4; i++)
	{
		if (i == 1)
		{
			winFramePosXArr[i] = 274 + i * 234 - 5;
		}
		else if (i == 2)
		{
			winFramePosXArr[i] = 274 + i * 234 + 5;
		}
		else
		{
			winFramePosXArr[i] = 274 + i * 234;
		}
	}

    for (BYTE i = 0; i < BET_ZONE_NUM_V88; i++) {
        //赢钱提示
		/*auto winTip =
			Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(
			BRNN_TABLERES_WINZONETIP_V88));*/

		auto winTip = Sprite::createWithTexture(director->getTextureCache()->getTextureForKey
			(StringUtils::format(BRNN_TABLERES_WINZONETIP_V88, i + 9)));
		winTip->setPosition(convertGL(winFramePosXArr[i], 387));
        winTip->setOpacity(0);
        this->addChild(winTip);
        _vecWinTipSprite.pushBack(winTip);
        //区域背景
        auto logo = Sprite::create(path[i]);
        logo->setPosition(convertGL(winTipPosXArr[i], 390));
        this->addChild(logo);
        logos.pushBack(logo);
        //区域总投注额
        auto zoneAllBet = LabelAtlas::create("0", FNT_YELLOW_ZERO_V88, 20, 29, '0');
        zoneAllBet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        zoneAllBet->setPosition(
            logo->getPositionX(),
            logo->getPositionY() + zoneAllBet->getContentSize().height +
            40 * _yScale); //- logo->getContentSize().height/2
        zoneAllBet->setVisible(false);
        this->addChild(zoneAllBet, 1);
        _allBetValInZone.pushBack(zoneAllBet);
        //区域我的投注额
		auto zoneMyBet = LabelAtlas::create("0", FNT_YELLOW_ZERO_V66, 20, 29, '0');
        zoneMyBet->setPosition(zoneAllBet->getPositionX(),
                               zoneAllBet->getPositionY() -
                               zoneAllBet->getContentSize().height - 15);
        zoneMyBet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        zoneMyBet->setVisible(false);
        this->addChild(zoneMyBet, 1);
        _myBetValInZone.pushBack(zoneMyBet);
    }
    //
	initWinLosShowcase();
    //筹码按钮列表
    for (int i = 0; i < 5; i++) {
        BetObjV88 *betObj = BetObjV88::create(i, false, false);
		betObj->setPosition(Vec2(WINSIZE.width *0.5f - 226 + i * 110, betObj->getBetSize().height));
        this->addChild(betObj,1);
        _vecBetObj.pushBack(betObj);
    }

    //显示连庄背景和次数
	auto _zhuangBoutBg = Sprite::create(BRNN_ZHUANG_BOUT_BG_V88);
	_zhuangBoutBg->setPosition(Vec2(WINSIZE.width - 433, WINSIZE.height - 189));
    this->addChild(_zhuangBoutBg);

	//auto _zhangBoutTip = Label::createWithTTF(GetText("continuation_game"), DEFAULT_FONT, FONT_SIZE_25);
	//_zhangBoutTip->setColor(Color3B(255, 255, 255));
	//_zhangBoutTip->setPosition(Vec2(WINSIZE.width - 434, WINSIZE.height - 190));
	//_zhangBoutTip->setSystemFontSize(FONT_SIZE_25);
	//this->addChild(_zhangBoutTip);

	_zhuangBout = Label::createWithTTF("", DEFAULT_FONT, FONT_SIZE_25);
    _zhuangBout->setColor(Color3B(255, 255, 255));
    _zhuangBout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _zhuangBout->setPosition(_zhuangBoutBg->getContentSize()/2);
    _zhuangBoutBg->addChild(_zhuangBout);
    //
    _clockObj = TimerObject::create();
	_clockObj->setPosition(Vec2(WINSIZE.width / 2 - 336, WINSIZE.height - 230));
    this->addChild(_clockObj);
    //发牌器
	auto fapaiqi = Sprite::create(BRNN_FAPAIQI_V88);
	fapaiqi->setPosition(Vec2(WINSIZE.width - 324, WINSIZE.height - 235));
    this->addChild(fapaiqi);

    //当局状态提示
	_gameStateSpr = Sprite::create("v66/zi/13.png");
	_gameStateSpr->setPosition(Vec2(WINSIZE.width*0.5f, WINSIZE.height - 140));
	this->addChild(_gameStateSpr);
}

void GameTableUIV88::initWinLosShowcase() {
    Director *director = Director::getInstance();
    
    //天地玄黄最近6局胜负标记sprite 402 384 366 348
    _recordBg = Sprite::create(BRNN_TABLERES_RECORD_BG_V88);
	_recordBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_recordBg->setPosition(Vec2(WIN_SIZE/2));
    this->addChild(_recordBg,1000000);
	_recordBg->setVisible(false);
	//
	auto clocseBtn = AUIButton::create(BRNN_TABLERES_RECORD_CLOCSE_V88, BRNN_TABLERES_RECORD_CLOCSE_V88);
	clocseBtn->addClickEventListener(CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
	_recordBg->addChild(clocseBtn);
	clocseBtn->setAnchorPoint(Vec2(1, 1));
	Size recordS = _recordBg->getContentSize();
	clocseBtn->setPosition(recordS);
	clocseBtn->setTag(em_zoushituHideBtn);
	clocseBtn->setPressedActionEnabled(true);

    //    Texture2D* texture =
    //    director->getTextureCache()->getTextureForKey(BRNN_TABLERES_RECORDWIN);
    float width = 20.5;
    float height = 103;
    float posWinOrLoseX[RECORD_LATEST_SHOW_NUM_V88];
    float posWinOrLoseY[BET_ZONE_NUM_V88];
    for (int i = 0; i < RECORD_LATEST_SHOW_NUM_V88; i++) {
		//posWinOrLoseX[i] = WINSIZE.width / 2 - 585 + i * (width + 12);
		posWinOrLoseX[i] = _recordBg->getContentSize().width / 7.5 + i*(width + 33) +25;
    }
    for (int i = 0; i < BET_ZONE_NUM_V88; i++) {
		//posWinOrLoseY[i] = WINSIZE.height / 2 - 345 + i * height;
		posWinOrLoseY[i] = _recordBg->getContentSize().height / 8 * 7 - i * height-70;
    }

    for (BYTE i = 0; i < BET_ZONE_NUM_V88; i++) {
        //胜负比例值
        auto propLabel = Label::create();
        propLabel->setPosition(Vec2(posWinOrLoseX[9] + (posWinOrLoseX[9] - posWinOrLoseX[8]) + 40, posWinOrLoseY[i]));
        _recordBg->addChild(propLabel, 2);
		propLabel->setSystemFontSize(30);
        propLabel->setString("50%");
        _vecWinOrLoseProp.pushBack(propLabel);
    }
    
    for (BYTE i = 0; i < RECORD_LATEST_SHOW_NUM_V88; i++) {
        int j = 0;
        auto tianStatus = Sprite::createWithTexture(
			director->getTextureCache()->getTextureForKey(BRNN_TABLERES_RECORDWIN_V88));
        tianStatus->setPosition(Vec2(posWinOrLoseX[i], posWinOrLoseY[j++]));
		_recordBg->addChild(tianStatus, 1);
        _recordGameInfoSpriteArr[0][i] = tianStatus;
		_recordGameInfoSpriteArr[0][i]->setVisible(false);

        auto diStatus =
            Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(
			BRNN_TABLERES_RECORDWIN_V88));
		diStatus->setPosition(Vec2(posWinOrLoseX[i], posWinOrLoseY[j++]));
		_recordBg->addChild(diStatus, 1);
        _recordGameInfoSpriteArr[1][i] = diStatus;
		_recordGameInfoSpriteArr[0][i]->setVisible(false);

        auto xuanStatus = Sprite::createWithTexture(
			director->getTextureCache()->getTextureForKey(BRNN_TABLERES_RECORDWIN_V88));
		xuanStatus->setPosition(Vec2(posWinOrLoseX[i], posWinOrLoseY[j++]));
		_recordBg->addChild(xuanStatus, 1);
        _recordGameInfoSpriteArr[2][i] = xuanStatus;
		_recordGameInfoSpriteArr[0][i]->setVisible(false);

        auto huangStatus =
            Sprite::createWithTexture(director->getTextureCache()->getTextureForKey(
			BRNN_TABLERES_RECORDWIN_V88));
		huangStatus->setPosition(Vec2(posWinOrLoseX[i], posWinOrLoseY[j++]));
		_recordBg->addChild(huangStatus, 1);
        _recordGameInfoSpriteArr[3][i] = huangStatus;
		_recordGameInfoSpriteArr[0][i]->setVisible(false);
    }
}

//显示或者隐藏趋势图
void GameTableUIV88::showWinLosShowcase() {
	_recordBg->setVisible(true);

}

void GameTableUIV88::hideWinLosShowcase() {
	_recordBg->setVisible(false);
}

void GameTableUIV88::initPlayerInfo() {

	//float fScale = 0.76f;
	//m_mySelf = UserHead::create(HEAD_FRAME_IN_GAME_V88);
	//m_mySelf->setClipUserHeadOffset(Vec2(0, 8));
	//Size size = m_mySelf->getContentSize()*fScale;
	//setContentSize(size);

	//m_mySelf->setScale(fScale);
	//m_mySelf->setClipUserHeadOffset(Vec2(0, 20));
	//m_mySelf->setPosition(120, 80);
	//m_mySelf->setHeadTouchEnabled(true);
	//auto userinfo = _GameTableLogic->getUserByUserID(PlatformLogic()->getInstance()->loginResult.dwUserID);
	//if (userinfo)
	//{
	//	m_mySelf->setHeadWithInfo(userinfo);
	//}
	//this->addChild(m_mySelf);

    //显示玩家的用户名和金钱
	auto _userNameBg = Sprite::create(BRNN_USERNAME_BG_V88);
    _userNameBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _userNameBg->setPosition(convertGL(115, 625));
    this->addChild(_userNameBg);

	auto _userMoneyBg = Sprite::create(BRNN_USERMONEYBG_V88);
	_userMoneyBg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_userMoneyBg->setPosition(convertGL(115, 675));
	this->addChild(_userMoneyBg);

	auto _myNameLabel = Label::createWithTTF("", FONT_TTF_MSYH, FONT_SIZE_NUM_26);
	_myNameLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_myNameLabel->setPosition(Vec2(_userNameBg->getPositionX() + 35, _userNameBg->getPositionY() - 0));
    _myNameLabel->setString(ForceGBKToUtf8(_ownInfo->nickName));
    this->addChild(_myNameLabel);

	_myMoneyLabel = Label::createWithTTF("", FONT_TTF_FZCY, FONT_SIZE_NUM_26);
	_myMoneyLabel->setColor(FONT_CLOLOR_YELLOW_V66);
	_myMoneyLabel->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_myMoneyLabel->setPosition(Vec2(_userMoneyBg->getPositionX() + 35, _userMoneyBg->getPositionY() - 8));
	_myMoneyLabel->setString(Tools::addComma(_ownInfo->i64Money));
    this->addChild(_myMoneyLabel);
}

void GameTableUIV88::initChat() {
    ////chat
//    auto chatSize = Size(233, 123);
//    _chatOpenBtn = Button::create(BRNN_CHAT_BG_V88, BRNN_CHAT_BG_V88);
//    _chatOpenBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
//    _chatOpenBtn->setPosition(convertGL(1018, 567));
//    _chatOpenBtn->setTag(em_chatOpenBtn);
//    ////_chatOpenBtn->addClickEventListener(
//    ////    CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
//    ////this->addChild(_chatOpenBtn);
//
//    _chatUserName = Label::createWithTTF(
//                        "", DEFAULT_FONT, 20, Size(_chatOpenBtn->getContentSize().width,
//                                _chatOpenBtn->getContentSize().height / 2));
//    _chatUserName->setAnchorPoint(Vec2(0, 0.5));
//    _chatUserName->setColor(Color3B(222, 138, 57));
//    _chatUserName->setPosition(0,
//                               _chatOpenBtn->getContentSize().height * 2.0 / 3);
//    _chatOpenBtn->addChild(_chatUserName);
//
//    _chatLabel = Label::createWithTTF(
//                     "", DEFAULT_FONT, 18, Size(_chatOpenBtn->getContentSize().width - 18,
//                             _chatOpenBtn->getContentSize().height / 2));
//    _chatLabel->setAnchorPoint(Vec2(0, 1));
//    _chatLabel->setPosition(_chatOpenBtn->getContentSize().width * 0.045,
//                            _chatOpenBtn->getContentSize().height * 1.05 / 2);
//    _chatOpenBtn->addChild(_chatLabel);

    /*_chatTips = Sprite::create(BRNN_SYSTEM_INFO_TIPS);
    _chatTips->setPosition(_chatOpenBtn->getPositionX() + chatSize.width / 2,
                           _chatOpenBtn->getPositionY() + chatSize.height / 2);
    this->addChild(_chatTips);*/
}

void GameTableUIV88::initSeats() {
    //显示庄家和玩家的座位
    _vecPlayerSeatUI.reserve(180);

	_bankerSeatUI = PlayerSeatV88::create(MAX_NON_BANKER_SEAT_NUM_V88 + 1);
	//_bankerSeatUI->setBankerBg(BRNN_BANKER_BG_V88);
	//_bankerSeatUI->setScale(0.76);
    //_bankerSeatUI->setBankerQuan(BRNN_BANKER_QUAN_V88);
    //_bankerSeatUI->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_bankerSeatUI->setPosition(Vec2(WINSIZE.width / 2 -170, WINSIZE.height - 48));
	int oi = WINSIZE.width / 2 + 8;
	int li = WINSIZE.height - 98;
    this->addChild(_bankerSeatUI);
	_bankerSeatUI->setGameTableLogic(_GameTableLogic);

    //左3个 //右3个 //顶部左3个 //顶部右3个
    Vec2 seatPos[MAX_NON_BANKER_SEAT_NUM_V88];
    Vec2 directionPos[4] = {Vec2(3, 120), Vec2(WIN_SIZE.width - 150, 120), Vec2(118, 5),
                            Vec2(722, 5)
                           };
    Vec2 offPos[4] = {Vec2(0, 146), Vec2(0, 146), Vec2(148, 0), Vec2(148, 0)};
    for (int i = 0; i < 4; i++) {
        offPos[i].scale(Vec2(_xScale, _yScale));
    }
    for (int i = 0; i < MAX_NON_BANKER_SEAT_NUM_V88; i++) {
        seatPos[i] = directionPos[i / 3] + (i % 3) * (offPos[i / 3]);
    }

    for (int i = 0; i < MAX_NON_BANKER_SEAT_NUM_V88; i++) {
		if (i>=6 && i <12)
		{
		}
		else{
			auto seat = PlayerSeatV88::create(i);
			seat->setPosition(convertGL(
				seatPos[i].x, seatPos[i].y)); // convertGL(seatPos[i].x,seatPos[i].y)
			seat->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			this->addChild(seat);
			seat->setGameTableLogic(_GameTableLogic);
			_vecPlayerSeatUI.pushBack(seat);

		}
    }
}

void GameTableUIV88::initButtons() {
    //上庄按钮
    auto makerBtn =
		AUIButton::create(BUTTON_GREEN_BG0_V88, BUTTON_GREEN_BG1_V88);
	makerBtn->setTitle(APPLY_BANKER_TITLE_V88);
    makerBtn->setTitleOffset(Vec2(-5, 5));
    makerBtn->addClickEventListener(CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
	makerBtn->setAnchorPoint(Vec2::ZERO);
	makerBtn->setPosition(Vec2(WINSIZE.width / 2+40, WINSIZE.height-80));
    makerBtn->setTag(em_makerBtn);
    makerBtn->setPressedActionEnabled(true);
    makerBtn->setVisible(false);
    this->addChild(makerBtn);

    //庄家列表按钮
    auto bankerListBtn =
		AUIButton::create(BUTTON_GREEN_BG0_V88, BUTTON_GREEN_BG1_V88);
    bankerListBtn->setTitle(OPEN_BANKER_LIST_TITLE_V88);
    bankerListBtn->setTitleOffset(Vec2(-5, 5));
    bankerListBtn->addClickEventListener(
        CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
	bankerListBtn->setAnchorPoint(Vec2::ZERO);
	bankerListBtn->setPosition(Vec2(WINSIZE.width / 2 + 40, WINSIZE.height - 80));
    bankerListBtn->setTag(em_banker_list);
    bankerListBtn->setPressedActionEnabled(true);
    bankerListBtn->setVisible(false);
    this->addChild(bankerListBtn);

    //重复下注按钮
    auto repeatBetBtn =
		AUIButton::create(BUTTON_BULE0_BG_V88, BUTTON_BULE1_BG_V88);
	repeatBetBtn->setTitle(REPEATE_BET_TITLE_V88);
    repeatBetBtn->setTitleOffset(Vec2(-5, 5));
    repeatBetBtn->addClickEventListener(
        CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
    repeatBetBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    repeatBetBtn->setPosition(convertGL(WIN_SIZE.width - 323, 650 - 20));
    repeatBetBtn->setTag(em_repeatBetBtn);
    repeatBetBtn->setPressedActionEnabled(true);
    this->addChild(repeatBetBtn);

    //取消重复下注按钮
    auto cancelRepeatBetBtn =
		AUIButton::create(BUTTON_BULE0_BG_V88, BUTTON_BULE1_BG_V88);
    cancelRepeatBetBtn->setTitle(CANCEL_REPEATE_BET_TITLE_V88);
    cancelRepeatBetBtn->setTitleOffset(Vec2(-5, 5));
    cancelRepeatBetBtn->addClickEventListener(
        CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
    cancelRepeatBetBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	cancelRepeatBetBtn->setPosition(convertGL(857 + 100, 650 - 20));
    cancelRepeatBetBtn->setTag(em_cancelRepeatBetBtn);
    cancelRepeatBetBtn->setVisible(false);
    cancelRepeatBetBtn->setPressedActionEnabled(true);
    this->addChild(cancelRepeatBetBtn);
    //无座玩家按钮
	_noSeatBtn = AUIButton::create(BUTTON_BULE0_BG_V88, BUTTON_BULE1_BG_V88);
	_noSeatBtn->setTitle(NO_SEAT_PLAYER_TITILE_V88);
    _noSeatBtn->setTitleOffset(Vec2(-5, 5));
    _noSeatBtn->addClickEventListener(
        CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
    _noSeatBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _noSeatBtn->setPosition(
        convertGL(WIN_SIZE.width -
                  _noSeatBtn->getContentSize().width - 20 * _xScale + 18,
                  26 * _yScale));
    _noSeatBtn->setTag(em_noSeatBtn);
    _noSeatBtn->setPressedActionEnabled(true);
    this->addChild(_noSeatBtn);
	_noSeatBtn->setVisible(false);
    //下拉按钮
    /*auto toobarBtn = Button::create(BTN_TOOLBAR, BTN_TOOLBAR);
    toobarBtn->addClickEventListener(
        CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
    toobarBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    toobarBtn->setPosition(convertGL(29, 15));
    toobarBtn->setTag(em_toolbarBtn);
	toobarBtn->setVisible(false);
    toobarBtn->setPressedActionEnabled(true);
    this->addChild(toobarBtn, 1000);*/

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#else
    //银行按钮
    auto _openBankBtn =
		AUIButton::create(BUTTON_BULE0_BG_V88, BUTTON_BULE1_BG_V88);
    _openBankBtn->setTitle(OPEN_BANK_TITLE_V88);
    _openBankBtn->setTitleOffset(Vec2(-5, 5));
    _openBankBtn->addClickEventListener(
        CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
    _openBankBtn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _openBankBtn->setPosition(
        Vec2(_noSeatBtn->getPositionX(), makerBtn->getPositionY()));
    _openBankBtn->setTag(em_openBank);
    _openBankBtn->setPressedActionEnabled(true);
    //this->addChild(_openBankBtn);
#endif

    initDropdownList();

    if (GD_NEED_OPEN) {
        repeatBetBtn->setVisible(false);
        cancelRepeatBetBtn->setVisible(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#else
        _openBankBtn->setVisible(false);
#endif
    }

    ////合作上庄
    // auto uionmakerPic = Sprite::create(BRNN_TABLERES_BTN_UION_MAKER1);
    // auto uionmakerPic1 =
    // ui::Scale9Sprite::create(BRNN_TABLERES_BTN_UION_MAKER1);
    // auto uionmakerPic2 =
    // ui::Scale9Sprite::create(BRNN_TABLERES_BTN_UION_MAKER2);
    // auto  uionmakerBtn = ControlButton::create(uionmakerPic1);
    // uionmakerBtn->setBackgroundSpriteForState(uionmakerPic2,
    // Control::State::HIGH_LIGHTED);
    // uionmakerBtn->setPreferredSize(uionmakerPic->getContentSize());
    // uionmakerBtn->setZoomOnTouchDown(false);
    // uionmakerBtn->setPosition(convertPoint(0, 390.0) +
    // Vec2(makerBtn->getPositionX(), 0));
    // uionmakerBtn->setTag(em_uionmakerBtn);
    // uionmakerBtn->addTargetWithActionForControlEvents(this,
    // cccontrol_selector(GameTableUIV88::onBtnClick),
    // Control::EventType::TOUCH_UP_INSIDE);
    // uionmakerBtn->setVisible(false);
    // this->addChild(uionmakerBtn);
    // auto cancelRepeatBetPic = Sprite::create(BRNN_TABLERES_BTN_NOREPEAT1);
    // auto cancelRepeatBetPic1 =
    // ui::Scale9Sprite::create(BRNN_TABLERES_BTN_NOREPEAT1);
    // auto cancelRepeatBetPic2 =
    // ui::Scale9Sprite::create(BRNN_TABLERES_BTN_NOREPEAT2);
    // auto cancelRepeatBetBtn = ControlButton::create(cancelRepeatBetPic1);
    // cancelRepeatBetBtn->setBackgroundSpriteForState(cancelRepeatBetPic2,
    // Control::State::HIGH_LIGHTED);
    // cancelRepeatBetBtn->setPreferredSize(cancelRepeatBetPic->getContentSize());
    // cancelRepeatBetBtn->setZoomOnTouchDown(false);
    // cancelRepeatBetBtn->setPosition(repeatBetBtn->getPosition());
    // cancelRepeatBetBtn->setTag(em_cancelRepeatBetBtn);
    // cancelRepeatBetBtn->addTargetWithActionForControlEvents(this,
    // cccontrol_selector(GameTableUIV88::onBtnClick),
    // Control::EventType::TOUCH_UP_INSIDE);
    // this->addChild(cancelRepeatBetBtn);
    // cancelRepeatBetBtn->setVisible(false);

    //商店按钮  whao
    // auto shopPic = Sprite::create(BRNN_TABLERES_BTN_SHOP1);
    // auto shopPic1 = ui::Scale9Sprite::create(BRNN_TABLERES_BTN_SHOP1);
    // auto shopPic2 = ui::Scale9Sprite::create(BRNN_TABLERES_BTN_SHOP2);
    // auto shopBtn = ControlButton::create(shopPic1);
    // shopBtn->setBackgroundSpriteForState(shopPic2,
    // Control::State::HIGH_LIGHTED);
    // shopBtn->setPreferredSize(shopPic->getContentSize());
    // shopBtn->setZoomOnTouchDown(false);
    // shopBtn->setPosition(getContentSize().width-shopPic->getContentSize().width/2,
    // getContentSize().height-shopPic->getContentSize().height/2);
    // shopBtn->setTag(em_shopBtn);
    // shopBtn->addTargetWithActionForControlEvents(this,
    // cccontrol_selector(GameTableUIV88::onBtnClick),
    // Control::EventType::TOUCH_UP_INSIDE);
    // this->addChild(shopBtn);

    //表情按钮
    // auto emotionPic = Sprite::create(BRNN_TABLERES_BTN_FACE1);
    // auto emotionPic1 = ui::Scale9Sprite::create(BRNN_TABLERES_BTN_FACE1);
    // auto emotionPic2 = ui::Scale9Sprite::create(BRNN_TABLERES_BTN_FACE2);
    // auto emotionBtn = ControlButton::create(emotionPic1);
    // emotionBtn->setBackgroundSpriteForState(emotionPic2,
    // Control::State::HIGH_LIGHTED);
    // emotionBtn->setPreferredSize(emotionPic->getContentSize());
    // emotionBtn->setZoomOnTouchDown(false);
    // emotionBtn->setPosition(emotionPic->getContentSize().width/2,
    // emotionPic->getContentSize().height/2);
    // emotionBtn->setTag(em_emotionBtn);
    // emotionBtn->addTargetWithActionForControlEvents(this,
    // cccontrol_selector(GameTableUIV88::onBtnClick),
    // Control::EventType::TOUCH_UP_INSIDE);
    // this->addChild(emotionBtn);



	//走势图按钮
	auto zoushituBtn =
		AUIButton::create(ZOUSHITU_BTN_BG0_V88, ZOUSHITU_BTN_BG1_V88);
	zoushituBtn->addClickEventListener(
		CC_CALLBACK_1(GameTableUIV88::onBtnClick, this));
	zoushituBtn->setAnchorPoint(Vec2(1.0f,1.0f));
	zoushituBtn->setPosition(Vec2(WIN_SIZE.width - 10, WIN_SIZE.height - 20));
	zoushituBtn->setTag(em_zoushituBtn);
	zoushituBtn->setPressedActionEnabled(true);
	this->addChild(zoushituBtn);
}


void GameTableUIV88::createToolBarUI() {
	int zOrder = 60;
	auto toolbar = ToolBarLayer::create();
	if (toolbar) {
		toolbar->addImageAndCallBack(TOOL_BAR_EXIT, [=]() {
			if (!RoomLogic()->connected()) {
				leaveTheGame();
			}
			else {
				if ((GS_WAIT_SETGAME == _GameTableLogic->_gameStatus  || GS_FREE == _GameTableLogic->_gameStatus
					|| _isNote == false)) {
						_GameTableLogic->sendUserUp();
					return;
				}
				//auto layer = GamePromptLayer::create();
				//layer->showPrompt(GetText("limit_leave_in_game"));
				GamePromptTip* layer = GamePromptTip::create(this, Color4B(0, 0, 0, 160));
				layer->setGamePromptTip(GetText("limit_leave_in_game"));
			}
		});
        toolbar->addImageAndCallBack(TOOL_BAR_SETTING, [=]() {
            IGameSet* gameSet = GameSetFactory::createWithVersion();
            gameSet->setBgPath("");
            gameSet->open(this, Vec2(0, 0), zOrder + 5, 0);
        });
		toolbar->addImageAndCallBack(TOOL_BAR_HELP, [=]() {
			auto helpLayer = HN::HelpLayer::create();
			helpLayer->setContentOnView("v66/common/help/niuniu.png");
			helpLayer->setName("helpLayer");
			helpLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			helpLayer->setOrignalPosition();
			helpLayer->moveSelfInWithAction();
			addChild(helpLayer, zOrder + 5);
		});
		toolbar->create(this, Vec2(10, WIN_SIZE.height), zOrder, 100);
	}
}

//void GameTableUIV88::createToolBarUI() {
//	char resStr[128] = { 0 };
//
//	sprintf(resStr, BRNN_TOOL_PATH, "2.png");
//	auto toobarBtn1 = Button::create(resStr, resStr);
//	toobarBtn1->setShowEffect(false);
//	toobarBtn1->addClickEventListener([=](Ref*) {
//		_toolbarBg->setName("1");
//		_toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height)));
//		this->getChildByTag(em_toolbarBtn)->setVisible(false);
//	});
//	toobarBtn1->setAnchorPoint(Vec2(0.5, 1));
//	toobarBtn1->setPosition(Vec2(40, WIN_SIZE.height - 10));
//	toobarBtn1->setTag(em_toolbarBtn);
//	this->addChild(toobarBtn1, 70);
//	toobarBtn1->setVisible(true);
//	toobarBtn1->setLocalZOrder(1000);
//
//	sprintf(resStr, BRNN_TOOL_BAR_PATH, "23.png");
//	_toolbarBg = Sprite::create(resStr);
//	_toolbarBg->setAnchorPoint(Vec2(0.5, 1));
//	_toolbarBg->setPosition(150, WIN_SIZE.height + _toolbarBg->getContentSize().height);
//	_toolbarBg->setName("0");//0为在上面 1在下面
//	this->addChild(_toolbarBg, 71);     //whao
//
//	Size toolbarSize = _toolbarBg->getContentSize();
//
//	sprintf(resStr, BRNN_TOOL_BAR_PATH, "24.png");
//	auto setPic = Sprite::create(resStr);
//	setPic->setPosition(toolbarSize.width / 2 + 2, toolbarSize.height * 3 / 4 + 66.0 / 1280.0 *WINSIZE.height);
//	_toolbarBg->addChild(setPic);
//	auto setBtn = ControlButton::create();
//	setBtn->setShowEffect(false);
//	setBtn->setPreferredSize(setPic->getContentSize());
//	setBtn->setPosition(setPic->getPosition());
//	setBtn->setTag(em_toolbarSetBtn);
//	setBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV88::toolBarClickCallback), Control::EventType::TOUCH_UP_INSIDE);
//	_toolbarBg->addChild(setBtn);
//
//	sprintf(resStr, BRNN_TOOL_BAR_PATH, "25.png");
//	auto helpPic = Sprite::create(resStr);
//	helpPic->setPosition(toolbarSize.width / 2 + 2, toolbarSize.height * 3 / 5 + 5.0);
//	_toolbarBg->addChild(helpPic);
//	auto helpBtn = ControlButton::create();
//	helpBtn->setShowEffect(false);
//	helpBtn->setPreferredSize(helpPic->getContentSize());
//	helpBtn->setPosition(helpPic->getPosition());
//	helpBtn->setTag(em_toolbarHelpBtn);
//	helpBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV88::toolBarClickCallback), Control::EventType::TOUCH_UP_INSIDE);
//	_toolbarBg->addChild(helpBtn);
//
//	sprintf(resStr, BRNN_TOOL_BAR_PATH, "26.png");
//	auto leavePic = Sprite::create(resStr);
//	leavePic->setPosition(toolbarSize.width / 2 + 2, toolbarSize.height * 2 / 5 - 10.0);
//	_toolbarBg->addChild(leavePic);
//	auto leaveBtn = ControlButton::create();
//	leaveBtn->setShowEffect(false);
//	leaveBtn->setPreferredSize(leavePic->getContentSize());
//	leaveBtn->setPosition(leavePic->getPosition());
//	leaveBtn->setTag(em_toolbarLeaveBtn);
//	leaveBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV88::toolBarClickCallback), Control::EventType::TOUCH_UP_INSIDE);
//	_toolbarBg->addChild(leaveBtn);
//
//	sprintf(resStr, BRNN_TOOL_BAR_PATH, "27.png");
//	auto nextRoundOutPic = Sprite::create(resStr);
//	nextRoundOutPic->setTag(em_toolbarNextOutLight);
//	nextRoundOutPic->setPosition(toolbarSize.width / 2 + 2, toolbarSize.height * 1 / 5 - 17.0);
//	_toolbarBg->addChild(nextRoundOutPic);
//	auto nextRoundOutBtn = ControlButton::create();
//	nextRoundOutBtn->setShowEffect(false);
//	nextRoundOutBtn->setPreferredSize(nextRoundOutPic->getContentSize());
//	nextRoundOutBtn->setPosition(nextRoundOutPic->getPosition());
//	nextRoundOutBtn->setTag(em_toolbarNextOutBtn);
//	nextRoundOutBtn->setUserData((void*)false);
//	nextRoundOutBtn->addTargetWithActionForControlEvents(this, cccontrol_selector(GameTableUIV88::toolBarClickCallback), Control::EventType::TOUCH_UP_INSIDE);
//	_toolbarBg->addChild(nextRoundOutBtn);
//}

//void GameTableUIV88::toolBarClickCallback(Ref* pSender, Control::EventType event)
//{
//	Node* btn = (Node*)pSender;
//	switch (btn->getTag()) {
//	case em_toolbarBtn: {
//							if (0 == _toolbarBg->getName().compare("0")) {
//								_toolbarBg->setName("1");
//								_toolbarBg->runAction(MoveBy::create(0.3, Point(0, -_toolbarBg->getContentSize().height)));
//								this->getChildByTag(em_toolbarBtn)->setVisible(false);         //whao
//							}
//	}
//		break;
//	case em_toolbarSetBtn: {
//							   auto setLayer = BRNNSetLayerV88::create();
//							   setLayer->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
//							   setLayer->showSet(this, 60);
//
//							   _toolbarBg->setName("0");
//
//							   _toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//								   CallFunc::create([this]() {
//								   this->getChildByTag(em_toolbarBtn)->setVisible(true);
//							   })));
//	}
//		break;
//	case  em_toolbarHelpBtn: {
//								 //hintClickCallback(btn, Widget::TouchEventType::ENDED);
//								 auto helpLayer = BRNNHelpLayerV88::create();
//        helpLayer->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2));
//        btn->getParent()->getParent()->addChild(helpLayer, 100);
//
//		_toolbarBg->setName("0");
//
//		_toolbarBg->runAction(CCSequence::createWithTwoActions(MoveBy::create(0.3, Point(0, _toolbarBg->getContentSize().height)),
//			CallFunc::create([this]() {
//			this->getChildByTag(em_toolbarBtn)->setVisible(true);
//		})));
//	}
//		break;
//	case em_toolbarLeaveBtn: {
//								 /*if (_tableLogic->m_gameState != GAME_FREE) {
//									 GamePromptLayer::create()->showPrompt(GetText("GAME_PLAYING_NOT_EXIT"));
//									 }
//									 else {
//									 _tableLogic->sendUserUp();
//									 }*/
//								 GameTableUIV88 *gameTableUI = (dynamic_cast<GameTableUIV88 *>(btn->getParent()->getParent()));
//								 int status = gameTableUI->_gameStatus;
//
//								 if (!RoomLogic()->connected()) {
//									 gameTableUI->leaveTheGame();
//								 }
//								 else {
//									 //下注或者坐庄不能退出
//									 if (_myBetMoney != 0 || _bankerInfo->bDeskStation == _GameTableLogic->getMySeatNo()) {
//										 leaveBombBox();
//										 return;
//									 }
//
//									 gameTableUI->_GameTableLogic->sendUserUp();
//								 }
//	}
//		break;
//	case em_toolbarNextOutBtn: {
//								   if (btn->getUserData() == (void*)true) {
//									   btn->setUserData((void*)false);
//									   auto spr = static_cast<Sprite*>(_toolbarBg->getChildByTag(em_toolbarNextOutLight));
//									   if (spr)
//										   spr->setTexture("v66/13zhang/xialaanniu/27.png");
//									   m_bNextOut = false;
//								   }
//								   else if (btn->getUserData() == (void*)false) {
//									   btn->setUserData((void*)true);
//									   auto spr = static_cast<Sprite*>(_toolbarBg->getChildByTag(em_toolbarNextOutLight));
//									   if (spr)
//										   spr->setTexture("v66/13zhang/xialaanniu/28.png");
//									   m_bNextOut = true;
//								   }
//	}
//		break;
//	default:
//		break;
//	}
//}

void GameTableUIV88::toolBarFlag(float dt)
{
	toolBar_flag = !toolBar_flag;
}

void GameTableUIV88::initDropdownList() {
//    dropdownList = DropdownList::create();
//    dropdownList->setPosition(dropdownList->getContentSize().width / 2,
//                              VISIBLE_SIZE.height +
//                              dropdownList->getContentSize().height / 2);
//    dropdownList->setSetBtnCallback(
//        cccontrol_selector(GameTableUIV88::handleDropdownList));
//    dropdownList->setLeaveBtnCallback(
//        cccontrol_selector(GameTableUIV88::handleDropdownList));
//    dropdownList->setNextRoundBtnCallback(
//        cccontrol_selector(GameTableUIV88::handleDropdownList));
//    dropdownList->setHelpBtnCallback(
//        cccontrol_selector(GameTableUIV88::handleDropdownList));
//    dropdownList->setVisible(false);
//    addChild(dropdownList);
}
// end

Vec2 GameTableUIV88::convertPoint(float x, float y) {
    Vec2 tmp = Vec2(_tableSize.width * x / 816, _tableSize.height * y / 546);
    tmp.scale(getScale());
    return tmp;

    // return Vec2(_tableSize.width*x / 816, _tableSize.height*y / 546);
}

Vec2 GameTableUIV88::convertGL(float x, float y) {
    // Vec2 tmp = Director::getInstance()->convertToGL(Vec2(x, y));
    Vec2 tmp = Vec2(x * _xScale, WIN_SIZE.height - y * _yScale);
    // tmp.scale(getScale());
    return tmp;

    // return Director::getInstance()->convertToGL(Vec2(x, y));
}

//////////////////////////////////////////////////////////////////////////
void GameTableUIV88::betFalse(float dt) {
    int betType = rand() % 3; //假象数据没变 注额小一些看起来真实点
    int betZone = rand() % 4;
    BetV88 *bet = BetV88::create(betType, betZone, _noSeatBtn->getPosition());
    this->addChild(bet);
    if (TIAN_AREA_V88 == betZone) {
        bet->moveToBetZone(Vec2(382 * _xScale, 334 * _yScale));
        _betsInTianZone.pushBack(bet);
    } else if (DI_AREA_V88 == betZone) {
        bet->moveToBetZone(Vec2(535 * _xScale, 334 * _yScale));
        _betsInDiZone.pushBack(bet);
    } else if (XUAN_AREA_V88 == betZone) {
        bet->moveToBetZone(Vec2(688 * _xScale, 334 * _yScale));
        _betsInXuanZone.pushBack(bet);
    } else if (HUANG_AREA_V88 == betZone) {
        bet->moveToBetZone(Vec2(841 * _xScale, 334 * _yScale));
        _betsInHuangZone.pushBack(bet);
    }
}

Vec2 GameTableUIV88::getScale() {
    return Vec2(1 / _xScale, 1 / _yScale);
}

void GameTableUIV88::showCowRun() {
    Animation *animation = Animation::create();

    SpriteFrame *frame;

    char str[128] = {0};

    for (int i = 0; i < 6; i++) {
        sprintf(str, "common/animation/cowRun/cow%d.png", i);
        frame = SpriteFrame::create(str, Rect(0, 0, 700, 400));
        animation->addSpriteFrame(frame);
    }
    animation->setLoops(3);
    animation->setRestoreOriginalFrame(true);
    animation->setDelayPerUnit(0.6f / 6.0f);

    Animate *animate = Animate::create(animation);

    m_AnimateImage = Sprite::create("common/animation/cowRun/cow0.png");
    addChild(m_AnimateImage, 100000);
    m_AnimateImage->setPosition(Vec2(-400 * _xScale, WIN_SIZE.height / 2));
    m_AnimateImage->setVisible(false);

    Spawn *spawn = Spawn::create(
                       animate, MoveBy::create(1.5f, Vec2(WIN_SIZE.width + 550, 0)), nullptr);

    CallFunc *func1 =
    CallFunc::create([&]() {
        m_AnimateImage->setVisible(true);
    });

    CallFunc *func2 =
    CallFunc::create([&]() {
        m_AnimateImage->setVisible(false);
    });

    Sequence *seq = Sequence::create(func1, spawn, func2, nullptr);

    m_AnimateImage->runAction(seq);
}

    void GameTableUIV88::onHandleNewMessage(int iType, const std::string &message, int iCount)
    {
        //全服公告
        if (iType == 0)
        {
            showNotice(message, 100, iType);
        }
        else
        {
            showNotice(message, iCount, iType);
        }
    }

void GameTableUIV88::playScanner()
{
    runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([&] () {
        auto scanner = Scanner::create();
        this->addChild(scanner, 100000000);
        scanner->starPlayerEffect();
        scanner->setPosition(50, 40);
    }), nullptr));

}

void GameTableUIV88::onGameNotify(NN::tag_s2c_notify* msg) {
    if (msg->i64WinMoney < 0) return;
    char str[128] = { 0 };

    if (msg->bDeskID == INVALID_BYTE) {
        //广播
        if (msg->bGameID == 1) {
            sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_One"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
        } else if (msg->bGameID == 2) {
            sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Two"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
        } else if (msg->bGameID == 3) {
            sprintf(str, GetText("NN_Catch_Bull_Plus_Notify"), GetText("NN_Game_Three"), ForceGBKToUtf8(msg->cRoomName), ForceGBKToUtf8(msg->cNickName), Tools::addComma(msg->i64WinMoney).c_str());
        } else {
            return;
        }
        showNotice(str, 1, 1);
    }
}
// int GameTableUIV88::viewToLogic(int lSeat)
//{
//	if(-1 == _bankerSeatNum)
//		return lSeat;

//	if(lSeat >= _bankerSeatNum)
//	{
//		return lSeat + 1;
//	}
//	else
//	{
//		return lSeat;
//	}
//	return 0;
//}

// int GameTableUIV88::logicToView(int lSeat)
//{
//	if(-1 == _bankerSeatNum)
//		return lSeat;

//	if(lSeat > _bankerSeatNum)
//	{
//		return lSeat - 1;
//	}
//	else if(lSeat < _bankerSeatNum)
//	{
//		return lSeat;
//	}
//	return 0;
//}

// bool GameTableUIV88::giftToMeAddMoney(HNSocketMessage* socketMessage)
//{
//	if (1 == socketMessage->messageHead.bHandleCode)
//	{
//		CCAssert(sizeof(MSG_GP_CS_NOTIFY_IOSPayResult) ==
// socketMessage->objectSize, "MSG_GP_CS_NOTIFY_IOSPayResult is error.");
//		MSG_GP_CS_NOTIFY_IOSPayResult* pIOSPayResult =
//(MSG_GP_CS_NOTIFY_IOSPayResult*)socketMessage->object;
//		PlatformLogic()->loginResult.i64Money = pIOSPayResult->llMoney;
//		RoomLogic()->loginResult.pUserInfoStruct.i64Money =
// pIOSPayResult->llMoney;
//		_ownInfo->i64Money = pIOSPayResult->llMoney;
//		MsgTip::create(GBKToUtf8(StringUtils::format("收到朋友的礼物，系统给你兑换了%d金币",
//_getGiftMoneyNum).c_str()));
//		updateMyData();
//	}
//	return true;
//}

// int GameTableUIV88::getEmptySeat()
//{
//	auto deskUserList = _GameTableLogic->getDeskUserList();
//	for (int i = PLAY_COUNT - 1; i >= 0; i--)
//	{
//		UserInfoStruct* user = deskUserList->getUserByDeskStation(i);
//		if (!user)
//		{
//			return i;
//		}
//	}
//	return 0;
//}

//void GameTableUIV88::userChangeSeat(int targetSeat, bool bViewToLogic/* = true*/) //该版本暂时不支持换座位，所以该方法不能调用
//{
//	if(_bankerInfo)
//	{
//		if(_bankerInfo->dwUserID != _ownInfo->dwUserID)
//		{
//			_bUpForChangeSeat = true;
//			if(bViewToLogic)
//				_changedSeatNum = viewToLogic(targetSeat);
//			else
//				_changedSeatNum = targetSeat;
//			_GameTableLogic->sendUserUp();
//		}
//		else
//		{
//			MsgTip::create(GBKToUtf8("您当前正在做庄"));
//			return;
//		}
//	}
//	else
//	{
//		_bUpForChangeSeat = true;
//		if(bViewToLogic)
//			_changedSeatNum = viewToLogic(targetSeat);
//		else
//			_changedSeatNum = targetSeat;
//		_GameTableLogic->sendUserUp();
//	}
//}

////换庄 //这个消息暂时没有处理 不知道之前是为什么 现在的话需要处理 我需要显示新的上庄列表
//void GameTableUIV88::changeDealer(void* object) {
//    CHANGE_ZHUANG* pdata = (CHANGE_ZHUANG*)object;
//    _vecBankerList.clear();
//    auto deskUserList = _GameTableLogic->getDeskUserList();
//    for(int i=0; i<PLAY_COUNT; i++) {
//        if(-1 != pdata->dzhuangState[i][0]) {
//            UserInfoStruct* pUser = deskUserList->getUserByDeskStation(pdata->dzhuangState[i][0]);
//            if(pUser) {
//                TMP_BANKER_INFO info;
//                info.name = pUser->nickName;
//                info.money = pUser->i64Money;
//                info.dwUserId = pUser->dwUserID;
//                _vecBankerList.push_back(info);
//            } else {
//                if(pdata->dzhuangState[i][0] > 240) {
//                    TMP_BANKER_INFO info;
//                    info.name = GBKToUtf8(StringUtils::format("联合上庄组合%d", abs(pdata->dzhuangState[i][0]-255)+1).c_str());
//                    info.money = _bankerMoneyLimit;
//                    info.dwUserId = pdata->dzhuangState[i][0];
//                    _vecBankerList.push_back(info);
//                }
//            }
//        } else
//            break;
//    }
//}

//void GameTableUIV88::clientGiftRand(float dt)
//{
//	Point startPos = _noSeatBtn->getPosition();
//	Point targetPos = _bankerSeatUI->getPosition();//庄家获得3/4概率
//	bool bBankerGet = true;
//	if(0 == rand()%4)
//	{
//		int randSeat = rand()%12;
//		if(_vecPlayerSeatUI.at(randSeat)->getUser())
//		{
//			targetPos = _vecPlayerSeatUI.at(randSeat)->getPosition();
//			bBankerGet = false;
//		}
//	}
//	int giftId = rand()%6;//6个礼物
//	if(bBankerGet)
//	{
//		int prob = rand()%6;//6是概率分配 1/6随机  2/6花 3/6乌龟
//		if(0 < prob && prob < 3)// 2/6
//			giftId = 0;
//		else if(prob > 2) //3/6
//			giftId = 1;
//	}
//	this->giftMove(startPos, targetPos, giftId);
//}


////年兽触发
//void GameTableUIV88::onGameShowNianShou() {
//    //播动画
//    showNianShouAnimation();
//}
//
////赶年兽结果
//void GameTableUIV88::onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) {
//    //过滤只有自己看的到
//    if (nUserID != _GameTableLogic->getUserId(_GameTableLogic->getMySeatNo())) {
//        return;
//    }
//
//    removeNianShouButton();
//
//    Vec2 pos = NewYearActivityInGame::NianShouButtonPosition;
//
//    //有鞭炮赶年兽奖励
//    if (i64RewardMoney > 0) {
//        NewYearActivity::NSAnimationTool::playTNTAudio(i64RewardMoney);
//        auto daijiNode = NewYearActivity::NSAnimationTool::createNSGanNodeAni("hit", false);
//        if (daijiNode) {
//            daijiNode->setPosition(Vec2(pos));
//            this->addChild(daijiNode, NewYearActivityInGame::NianShouZOrder);
//        }
//
//        NewYearActivity::NSAnimationTool::createRewardLabel(i64RewardMoney, this, Vec2(pos.x, pos.y + 50));
//    } else { //无鞭炮
//        NewYearActivity::NSAnimationTool::createTipOfNoProp(this, pos);
//    }
//}
//
////对局获得鞭炮
//void GameTableUIV88::onGameMatchGainFirecraker(int nUserID) {
//    //该id获得一个中型鞭炮
//    //只有自己显示
//    if (_GameTableLogic->getUserId(_GameTableLogic->getMySeatNo()) == nUserID) {
//        ImageView* pTipImage = ImageView::create("common/gainFirecrakerTip.png");
//        pTipImage->setScale(0.1);
//        pTipImage->setPosition(Vec2(250, 140));
//        addChild(pTipImage, NewYearActivityInGame::NianShouZOrder);
//
//        Spawn* spawn = Spawn::create(ScaleTo::create(0.5f, 1.0f), FadeIn::create(0.5f), nullptr);
//        Sequence* seq = Sequence::create(spawn, DelayTime::create(2.0f), RemoveSelf::create(), nullptr);
//        pTipImage->runAction(seq);
//    }
//}
//
//void GameTableUIV88::showNianShouAnimation() {
//    if (this->getChildByName("DriveNianShouBtn")) {
//        return;
//    }
//    auto btn = NewYearActivity::NSAnimationTool::createNSButton("DriveNianShouBtn");
//    if (btn) {
//        btn->setPosition(NewYearActivityInGame::NianShouButtonPosition);
//        btn->addTouchEventListener(CC_CALLBACK_2(GameTableUIV88::driveNianShouClickCallback, this));
//        addChild(btn, NewYearActivityInGame::NianShouZOrder);
//
//        auto daijiNode = NewYearActivity::NSAnimationTool::createNSDaijiNodeAni("daiji", true);
//        if (daijiNode) {
//            daijiNode->setPosition(Vec2(btn->getContentSize().width*0.5f, btn->getContentSize().height*0.5f));
//            btn->addChild(daijiNode);
//        }
//        scheduleOnce(schedule_selector(GameTableUIV88::autoRemoveNianShouButton), 5.0f);
//    }
//}

//void GameTableUIV88::driveNianShouClickCallback(Ref* pSender, Widget::TouchEventType type) {
//    if (type != Widget::TouchEventType::ENDED)
//        return;
//
//    //        unschedule(schedule_selector(GameTableUIV88::autoRemoveNianShouButton));
//
//    Button* btn = (Button*)pSender;
//    btn->setTouchEnabled(false);
//
//    //发包
//    int nUserID = _GameTableLogic->getUserId(_GameTableLogic->getMySeatNo());
//    _GameTableLogic->sendDriveNianShou(nUserID);
//}
//
//void GameTableUIV88::removeNianShouButton() {
//    auto btn = this->getChildByName("DriveNianShouBtn");
//    if (btn) {
//        btn->stopAllActions();
//        btn->removeFromParentAndCleanup(true);
//    }
//}
//
//void GameTableUIV88::autoRemoveNianShouButton(float dt) {
//    removeNianShouButton();
//}
    void GameTableUIV88::onLuckyMoneyNotify(LuckyMoneyNotify *msg)
    {
        if (nullptr == msg)
        {
            return;
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        auto notify = CSLoader::createNode("Reward/ui.csb");
        notify->setName("RewardNotice");
        auto btnOpenReward = dynamic_cast<Button*>(notify->getChildByName("btnOpen"));
        btnOpenReward->addTouchEventListener(std::bind([&](Ref* psender, Widget::TouchEventType type){
            if (Widget::TouchEventType::ENDED != type)
            {
                return;
            }
            
            RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, ASS_OPEN_LUCKYMONEY, 0, 0);
            this->removeChildByName("RewardNotice");
        }, std::placeholders::_1, std::placeholders::_2));
        
        addChild(notify, 200);
        notify->setPosition(Vec2(visibleSize)/2);
        
        runAction(Sequence::create(DelayTime::create(msg->nStayTime), CallFunc::create([&](){
            this->removeChildByName("RewardNotice");
        }), NULL));
    }
    
    void GameTableUIV88::onLuckyMoneyResult(LuckyMoneyResult *msg)
    {
        if (nullptr == msg)
        {
            return;
        }
        
        if (msg->nType != 1)
        {
            GamePromptLayer::create()->showPrompt("手慢无！下次快点哦。");
            return;
        }
        
        auto result = CSLoader::createNode("Reward/reward.csb");
        auto action = CSLoader::createTimeline("Reward/reward.csb");
        if (nullptr != result && nullptr != action)
        {
            addChild(result, 200);
            auto visibleSize = Director::getInstance()->getVisibleSize();
            result->setPosition(Vec2(visibleSize)/2);
            result->setName("rewardResult");
            result->getChildByName("show")->runAction(action);
            action->gotoFrameAndPlay(0, false);
            
            auto txtReward = dynamic_cast<Text*>(result->getChildByName("txtReward"));
            txtReward->setString("");
            
            auto luckyMoney = msg->i64LuckyMoney;
            auto retainTime = msg->nShowTime;
            runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=](){
                txtReward->setString(Tools::addComma(luckyMoney));
            }), DelayTime::create(retainTime), CallFunc::create([&](){
                this->removeChildByName("rewardResult");
            }), NULL));
        }
    }

}
