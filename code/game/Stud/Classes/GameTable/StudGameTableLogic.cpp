#include "HNNetExport.h"
#include "HNLobbyExport.h"
#include "HNLogicExport.h"

#include "StudMessageHead.h"
#include "StudGameTableLogic.h"
#include "../v88/GameTable/StudGameTableUIV88.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/StateManager.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/v66/ExtraReward.h"
//#include "StudGameTableUICallback.h"

using namespace HN;

namespace STUD {
GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskId, bool autoCreate) :
    _gameUICallback(uiCallback),
    HNGameLogicBase(deskId, PLAY_COUNT, autoCreate, uiCallback),
    _nowBigNote(0),
    _baseNote(0),
    _limitNote(0),
    _followTimesLimit(0),
    _showHaTimesLimit(0),
    _noteTimesLimit(0),
    _noteTimesCount(0),
    _nowOutPeople(0),
    _firstOutPeople(0),
    _pk(0),
    _thinkTimer(0),
    _beginTimer(0),
    _hisToryMoney(0),
    _lastMoney(0),
    _startSeat(INVALID_DESKSTATION) {
    //_tableinfo.byDeskNo = deskId;
    //_tableinfo.bAutoCreate = autoCreate;

    _autoCreate = autoCreate;

    memset(_thisTurnNote, 0, sizeof(_thisTurnNote));
    memset(_thisGameNote, 0, sizeof(_thisGameNote));
    memset(_userState, 0, sizeof(_userState));
    memset(_cardList, 0, sizeof(_cardList));

    //initParams();  2016 7 27 日
}

GameTableLogic::~GameTableLogic() {

}

/************************line*********************************/
void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) {
    if (MDM_GM_GAME_NOTIFY != messageHead->bMainID) {
        HNLOG("onGameMessage MainID is error.");
        return;
    }

    switch (messageHead->bAssistantID) {
    case ASS_CUT_END: // 92
        HNLOG("ASS_CUT_END");
        dealUserCutEnd(object, objectSize);
        break;
    case ASS_GM_GAME_STATION:
        /////////////////////////////////////
        // The ASS_GM_GAME_STATION was captured by the HN PLATFORM
        HNLOG("ASS_GM_GAME_STATION");
        dealGameStationResp(object, objectSize);
        break;
    case ASS_GAME_BEGIN: // 52
        HNLOG("ASS_GAME_BEGIN");
        dealUserGameBegin(object, objectSize);
        break;
    case ASS_SEND_CARD: // 53
        HNLOG("ASS_SEND_CARD");
        dealUserSendCard(object, objectSize);
        break;
    case ASS_SEND_FINISH: // 54
        HNLOG("ASS_SEND_FINISH");
        dealUserSendFinish(object, objectSize);
        break;
    case ASS_GAME_PLAY:// 55
        HNLOG("ASS_GAME_PLAY");
        dealUserGamePlay(object, objectSize);
        break;
    case ASS_NOTE: // 60
        HNLOG("ASS_NOTE");
        dealUserNote(object, objectSize);
        break;
    case ASS_NOTE_RESULT: // 63
        HNLOG("ASS_NOTE_RESULT");
        dealUserNoteResult(object, objectSize);
        break;
    case ASS_COLLECT_JETON: // 70
        HNLOG("ASS_COLLECT_JETON");
        dealUserCollectJeton(object, objectSize);
        break;
    case ASS_AI_STATION: // 71
        HNLOG("ASS_AI_STATION");
        dealUserAiStation(object, objectSize);
        break;
    case ASS_SEND_A_CARD: // 81
        HNLOG("ASS_SEND_A_CARD");
        dealUserSendAcard(object, objectSize);
        break;

    case ASS_NO_CONTINUE_END: // 91
        HNLOG("ASS_NO_CONTINUE_END");
    //dealUserNoContinueEnd(object, objectSize);
    //break;
    case ASS_CONTINUE_END: // 90
        HNLOG("ASS_CONTINUE_END");
        dealUserContinueEnd(object, objectSize);
        break;

    case ASS_SALE_END: // 93
        HNLOG("ASS_SALE_END");
        dealUserSaleEnd(object, objectSize);
        break;
    case ASS_NO_WINER: // 94
        HNLOG("ASS_NO_WINER");
    //dealUserNoWinner(object, objectSize);
    //break;
    case ASS_AHEAD_END: // 95
        HNLOG("ASS_AHEAD_END");
        dealUserAheadEnd(object, objectSize);
        break;


    /////////////////////////////////////
    // not handled by the pc client

    case ASS_UG_USER_SET: // 50
        HNLOG("ASS_UG_USER_SET");
        //dealUserSetResp(object,objectSize);
        break;
    case ASS_UG_USER_SET_CHANGE: // 51
        HNLOG("ASS_UG_USER_SET_CHANGE");
        dealUserSetChangeResp(object, objectSize);
        break;

    case ASS_NOTE_LIMIE: // 56
        HNLOG("ASS_NOTE_LIMIE");
        dealUserNoteLimit(object, objectSize);
        break;
    case ASS_VREB_CHECK: // 61
        HNLOG("ASS_VREB_CHECK");
        dealUserVrebCheck(object, objectSize);
        break;
    case ASS_REPLACE_VREB_CHECK: // 62
        HNLOG("ASS_REPLACE_VREB_CHECK");
        dealUserReplaceVrebCheck(object, objectSize);
        break;
    case ASS_NEW_TURN: // 80
        HNLOG("ASS_NEW_TURN");
        dealUserNewTurn(object, objectSize);
        break;
    case ASS_HAVE_THING: // 100
        HNLOG("ASS_AI_WIN");
        dealUserAiWin(object, objectSize);
        break;
    case ASS_LEFT_RESULT: // 101
        HNLOG("ASS_LEFT_RESULT");
        dealUserLeftResult(object, objectSize);
        break;
    case ASS_MESSAGE: // 102
        HNLOG("ASS_MESSAGE");
        dealUserMessage(object, objectSize);
        break;
    case ASS_STOP_THING: // 103
        HNLOG("ASS_STOP_THING");
        dealUserStopThing(object, objectSize);
        break;
    case ASS_AGREE_STOP: // 104
        HNLOG("ASS_AGREE_STOP");
        dealUserAgreeStop(object, objectSize);
        break;
    case ASS_PLAY_FLASH: // 105
        HNLOG("ASS_PLAY_FLASH");
        dealUserPlayFlash(object, objectSize);
        break;
    case ASS_BEGIN_FLASH: // 106
        HNLOG("ASS_BEGIN_FLASH");
        dealUserBeginFlash(object, objectSize);
        break;
    case ASS_MONEY_NOT_ENOUTH: // 107 金币不足
        HNLOG("ASS_MONEY_NOT_ENOUTH");
        dealMoenyNotEnouth(object, objectSize);
        break;
    case NN::S2C_NOTIFY_MSG:
        {
        CCAssert(sizeof(NN::tag_s2c_notify) == objectSize, "sizeof(tag_s2c_notify) != objectSize");
        if (sizeof(NN::tag_s2c_notify) != objectSize)	return;

        NN::tag_s2c_notify* msg = (NN::tag_s2c_notify*)object;
        _gameUICallback->onGameNotify(msg);
        }
        break;
    case ASS_SHOW_LUCKYMONEY:
        {
            // 出现红包
            if (objectSize != sizeof(LuckyMoneyNotify))
            {
                CCASSERT(false, "sizeof(LuckyMoneyNotify) != objectSize");
                return;
            }
            
            auto msg = (LuckyMoneyNotify*)object;
            _gameUICallback->onLuckyMoneyNotify(msg);
        }
        break;
    case ASS_LUCKYMONEY_RESULT:
        {
            // 打开红包结果
            if (objectSize != sizeof(LuckyMoneyResult))
            {
                CCASSERT(false, "sizeof(LuckyMoneyResult) != objectSize");
                return;
            }
            
            auto msg = (LuckyMoneyResult*)object;
            _gameUICallback->onLuckyMoneyResult(msg);
        }
        break;

//    //年兽
//    case NewYearActivityInGame::S2C_SHOW_NIANSHOU_MSG : {
//        //年兽出现 150
//        _gameUICallback->onGameShowNianShou();
//    }
//    break;
//    case NewYearActivityInGame::S2C_DRIVE_NIANSHOU_RESULT: {
//        //赶年兽 151
//        CCAssert(sizeof(NewYearActivityInGame::GanNianShouRlt) == objectSize, "sizeof(GanNianShouRlt) != objectSize");
//        if (sizeof(NewYearActivityInGame::GanNianShouRlt) != objectSize)    return;
//
//        NewYearActivityInGame::GanNianShouRlt* msg = (NewYearActivityInGame::GanNianShouRlt*)object;
//        _gameUICallback->onGameDriveNianShouResult(msg->nUserID, msg->nRewardMoney);
//    }
//    break;
//    case NewYearActivityInGame::S2C_MATCH_GAIN_BANGER_MSG: {
//        //对局获得鞭炮 152
//        CCAssert(sizeof(NewYearActivityInGame::MatchFireCrackerRlt) == objectSize, "sizeof(MatchFireCrackerRlt) != objectSize");
//        if (sizeof(NewYearActivityInGame::MatchFireCrackerRlt) != objectSize)   return;
//
//        NewYearActivityInGame::MatchFireCrackerRlt* msg = (NewYearActivityInGame::MatchFireCrackerRlt*)object;
//        _gameUICallback->onGameMatchGainFirecraker(msg->nUserID);
//    }
//    break;

    default:
        HNLOG("UNKNOW MESSAGE: %d", messageHead->bAssistantID);
        break;

    }
}
void GameTableLogic::dealMoenyNotEnouth(void* object, INT objectSize) {
    auto pData = (tag_s2c_tip_exit*)object;
    _gameUICallback->userMoneyNotEnouth(pData);
}
//void GameTableLogic::dealUserSetResp(void* object, INT objectSize){}			// 50
void GameTableLogic::dealUserSetChangeResp(void* object, INT objectSize) {}		// 51

// ASS_GAME_BEGIN
void GameTableLogic::dealUserGameBegin(void* object, INT objectSize) {		// 52
    auto buff	= (GameBeginStruct*)object;

    _baseNote = buff->iBaseNote;
    _limitNote = buff->iLimitNote;
    /////////////////////////////////////
    // user state
    memcpy(_userState, buff->bUserState, sizeof(buff->bUserState));

    // init game data & ui
    initGameData();

    _gameUICallback->initGameUi(buff);
    m_gameState = GAME_PLAYING;


    /////////////////////////////////////
    // hide start button
    _gameUICallback->setStartBtnVisible(false);
    _gameUICallback->setMesaVisible(true);
    _gameUICallback->setBetAndOtherBtnOpen(true);
    _gameUICallback->setAllAreadySpriteVisible(false);
    /////////////////////////////////////
    // stop all timer
    stopAllTimer();
    //_gameUICallback->killTimer();

    /////////////////////////////////////
	//清空
	_gameUICallback->resetNote();
    // base note
    for(int i=0; i<PLAY_COUNT; i++) {
        _gameUICallback->updateNote(i, buff->iBaseNote);
    }
	_gameUICallback->playScanner();
    /////////////////////////////////////
    // hide handcards
    _handcardCount = 0;
    memset(_cardList, 0, sizeof(_cardList));
    _gameUICallback->updateHandcard(_handcardCount, _cardList);
}


// ASS_SEND_CARD
void GameTableLogic::dealUserSendCard(void* object, INT objectSize) {			// 53
    auto buff = (SendOneTurnCardStruct*)object;

    _startSeat		= buff->iStartPos;
    _handcardCount	= buff->iFirstSendCardCount;
    for(int i=0; i<PLAY_COUNT; i++) {
        memcpy(&_cardList[i],&buff->iCardList[i],sizeof(BYTE)*HANDCARD_COUNT);
    }
    /////////////////////////////////////
    // update handcard
    _gameUICallback->shuffleCards(0);
    _gameUICallback->updateHandcard(buff);
}

// ASS_SEND_A_CARD
void GameTableLogic::dealUserSendAcard(void* object, INT objectSize) {		// 81
    auto buff = (SendOneTurnCard*)object;

    /////////////////////////////////////
    // hide max card mark
    _gameUICallback->showMaxCardMark(false, _nowOutPeople);

    /////////////////////////////////////
    // init this turn data
    memset(_thisTurnNote, 0, sizeof(_thisTurnNote));
    _startSeat = buff->bStartPos;

    /////////////////////////////////////
    // update handcard value
    for(int i=0; i<PLAY_COUNT; i++) {
        _cardList[i][_handcardCount] = buff->iTurnCard[i];
    }
    _handcardCount++;

    /////////////////////////////////////
    // update handcard
    _gameUICallback->updateHandcard(buff);
}



// ASS_SEND_FINISH
void GameTableLogic::dealUserSendFinish(void* object, INT objectSize) {		// 54
    /////////////////////////////////////
    // send card finished

}

// ASS_GAME_PLAY
void GameTableLogic::dealUserGamePlay(void* object, INT objectSize) {			// 55
    auto buff =(BeginPlayStruct *)object;

    _nowOutPeople		= buff->bDeskStaton;
    _firstOutPeople	= buff->bDeskStaton;

    /////////////////////////////////////
    // show max card mark
    _gameUICallback->showMaxCardMark(true, buff->bDeskStaton);

    /////////////////////////////////////
    // note timer
    if(_nowOutPeople!=INVALID_DESKSTATION)
        _gameUICallback->startTimer(buff->bDeskStaton, _thinkTimer);
    //		_gameUICallback->setTimer(buff->bDeskStaton,1, _thinkTimer);

    /////////////////////////////////////
    // audio
    if(_nowOutPeople == _mySeatNo && _nowOutPeople!=INVALID_DESKSTATION)
        HNAudioEngine::getInstance()->playEffect(STUDAUDIO_SELF_ACTION);

    /////////////////////////////////////
    // init dashboard
    if(_userState[_mySeatNo]) {
        _gameUICallback->setBetBtnText(_baseNote);
    }

    _noteTimesCount = 1;
    if(_mySeatNo==buff->bDeskStaton) {
        auto showhaBtnEnabled = (_noteTimesCount>=_showHaTimesLimit);

        if(_pk) {
            _gameUICallback->setBetBtnEnabled(false, showhaBtnEnabled, false, true);
        } else {
            _gameUICallback->setBetBtnEnabled(true, showhaBtnEnabled, true, true);
        }
    } else {
        _gameUICallback->setBetBtnEnabled(false);
    }

}

// ASS_NOTE_LIMIE
void GameTableLogic::dealUserNoteLimit(void* object, INT objectSize) {		// 56
    /////////////////////////////////////
    // not handled by the pc client

    //auto buff = (LimitStruct*)object;
    //_noteLimit = buff;
}

// ASS_NOTE
void GameTableLogic::dealUserNote(void* object, INT objectSize) {				// 60
    auto buff = (tagUserResult *) object;
    _nowBigNote = buff->iNowBigNote;

    /////////////////////////////////////
    // show max card mark
    if (buff->bNewTurn) {
        _firstOutPeople = buff->iOutPeople;
        _gameUICallback->showMaxCardMark(true, buff->iOutPeople);
    }

    /////////////////////////////////////
    // timer
    _nowOutPeople= buff->iOutPeople;
    if (_nowOutPeople != INVALID_DESKSTATION)
        _gameUICallback->startTimer(buff->iOutPeople, _thinkTimer);
    //	_gameUICallback->setTimer(0, 0, _thinkTimer);

    /////////////////////////////////////
    // audio
    if(_nowOutPeople == _mySeatNo && _nowOutPeople!=INVALID_DESKSTATION)
        HNAudioEngine::getInstance()->playEffect(STUDAUDIO_SELF_ACTION);

    /////////////////////////////////////
    // set dashboard
    _noteTimesCount = buff->bNoteTimes;
    setDashboard(buff->bNoteTimes, buff->iNowBigNoteStyle, buff->iNowBigNote, buff->m_byUserAddNum, buff->bNewTurn);

}


// depends on: (please make sure all the depends parameters are initialed)
//		_nowOutPeople,
//		_pk,
//		_limitNote,
//		_baseNote
//		_followTimesLimit,
//		_showHaTimesLimit,
//		_gameUICallback
// @noteTimes: noted times, counted with each send card
// @lastNoteStyle: TYPE_GIVE_UP, TYPE_NOTE, TYPE_ADD, TYPE_FOLLOW, TYPE_SHUTTLE
// @notedMoney: < thisTurnNoteLimit
// @playerNoteTimes: times of each player noted
// @newTurn: is new turn or not
void GameTableLogic::setDashboard(BYTE noteTimes, BYTE lastNoteStyle, LLONG notedMoney, BYTE playerNoteTimes[], bool newTurn) {
    /////////////////////////////////////
    // set button text
    LLONG thisTurnNoteLimit = 0;
    if (1 == noteTimes) { // the 1st turn
        thisTurnNoteLimit = _limitNote/4;
    } else if (2 == noteTimes) { // the 2nd turn
        thisTurnNoteLimit = _limitNote/2;
    } else { // 3rd or more turn
        thisTurnNoteLimit = (_limitNote - _limitNote/2 - _limitNote/4 - _baseNote);
    }

    /////////////////////////////////////
    // set button enabled
    if(_mySeatNo == _nowOutPeople) {
        // pk mode
        if(_pk) {
            _gameUICallback->setBetBtnEnabled(false, true, false, true);
        } else {
            /// give up button
            bool giveup = true;
            if (_pk) {
                giveup= ((noteTimes > _followTimesLimit) && (lastNoteStyle != TYPE_SHUTTLE));
            } else {
                giveup= ((noteTimes > _followTimesLimit) || (lastNoteStyle == TYPE_SHUTTLE));
            }

            /// all-in button
            bool showha = noteTimes >= _showHaTimesLimit;

            /// note button
            bool add = (lastNoteStyle != TYPE_SHUTTLE && notedMoney >= 0);

            if (notedMoney >= thisTurnNoteLimit) {
                add = false;
            }

            if(_noteTimesLimit>0 && _noteTimesLimit!=255) {
                if (playerNoteTimes[_mySeatNo] >= _noteTimesLimit) {
                    add = false;
                }

                if (_firstOutPeople == _mySeatNo && !newTurn) {
                    add = false;
                    if (playerNoteTimes[_mySeatNo]  == 0)
                        showha = false;
                }
            }

            /// follow
            bool follow = (lastNoteStyle != TYPE_SHUTTLE);

            if (TYPE_SHUTTLE == lastNoteStyle) {
                showha = false;
                follow = true;
            }

            _gameUICallback->setBetBtnEnabled(follow, showha, add, giveup);
        }
    } else {
        _gameUICallback->setBetBtnEnabled(false);
    }
}

void GameTableLogic::dealUserVrebCheck(void* object, INT objectSize) {}			// 61
void GameTableLogic::dealUserReplaceVrebCheck(void* object, INT objectSize) {}	// 62

// ASS_NOTE_RESULT
void GameTableLogic::dealUserNoteResult(void* object, INT objectSize) {		// 63
    CHECK_SOCKET_DATA(NoteResult, objectSize, "NoteResult size is error");
    HNLOG("dealUserNoteResult");
    auto buff = (NoteResult*)object;

    /////////////////////////////////////
    // note audio
    _gameUICallback->playNoteAudio(buff->iOutPeople, buff->bNoteStyle);//播放音效

    /////////////////////////////////////
    // stop the timer
    _gameUICallback->stopTimer(buff->iOutPeople);

    /////////////////////////////////////
    // update note result
    if(buff->bNoteStyle==TYPE_GIVE_UP) {
        if(_mySeatNo==buff->iOutPeople) {
            _gameUICallback->setBetBtnEnabled(false);
        }
        return;
    }
    HNLOG("updateNote");
    _thisTurnNote[buff->iOutPeople] += buff->iCurNote;
    _thisGameNote[buff->iOutPeople] += buff->iCurNote;

    _gameUICallback->updateNote(buff->iOutPeople, buff->iCurNote);
}

// ASS_COLLECT_JETON
void GameTableLogic::dealUserCollectJeton(void* object, INT objectSize) {	// 70
    /////////////////////////////////////
    // reset this turn counters
    memset(_thisTurnNote, 0, sizeof(_thisTurnNote));
}

void GameTableLogic::dealUserAiStation(void* object, INT objectSize) {}			// 71

void GameTableLogic::dealUserNewTurn(void* object, INT objectSize) {}			// 80


// ASS_CONTINUE_END
void GameTableLogic::dealUserContinueEnd(void* object, INT objectSize) {		// 90
    auto buff = (GameEndStruct *)object;
    memset(_userState, 0, sizeof(_userState));


    _lastMoney = buff->iTurePoint[_mySeatNo];
    _hisToryMoney += _lastMoney;
    _gameUICallback->upDataHistoyAndLastMoney(_hisToryMoney, _lastMoney);


    /////////////////////////////////////
    // show start button
    _gameUICallback->setStartBtnVisible(true);
    _gameUICallback->setBetAndOtherBtnOpen(false);
    m_gameState = GAME_FREE;
    /////////////////////////////////////
    // start timer
    for(int seat=0; seat<PLAY_COUNT; seat++) {
        _gameUICallback->startTimer(seat, _beginTimer);
    }

    /////////////////////////////////////
    // show handcard
    bool showhand=true;
    for(int i=0; i<PLAY_COUNT && showhand; i++) {
        if(buff->iUserState[i]==TYPE_GIVE_UP) {
            //	memset(_cardList[i], 0, sizeof(_cardList[i]));
            //showhand=false;
            if (i != _mySeatNo) {
                _cardList[i][0] = 0x00;
            } else {
                for (int j = 0; j < PLAY_COUNT; j++) {
                    if (j != _mySeatNo) {
                        _cardList[j][0] = 0x00;
                    }
                }
            }
        }
    }

    for (int i = 0; i < PLAY_COUNT; i++) {
        if (i != _mySeatNo) {
            _gameUICallback->userPlayerCardTypeEffect(buff->iCardShape[i],0);
        } else {
            _gameUICallback->userPlayerCardTypeEffect(buff->iCardShape[i],0);
        }
    }

    _gameUICallback->updateHandcard(_handcardCount, _cardList);
    _gameUICallback->nextTurnLeavl();
}

void GameTableLogic::dealGamePointResp(void* object, INT objectSize) {
    HNLOG("dealGamePointResp");
    CCAssert(sizeof (MSG_GR_R_UserPoint) == objectSize, "MSG_GR_R_UserPoint is error.");

    MSG_GR_R_UserPoint * buff = (MSG_GR_R_UserPoint*)object;

    /////////////////////////////////////
    // win/fail audio effect
    if(buff->dwUserID == PlatformLogic()->loginResult.dwUserID) {
        auto endAudio = buff->dwMoney>0 ? STUDAUDIO_WIN : STUDAUDIO_LOSE;
        //HNAudioEngine::getInstance()->playEffect(endAudio);
    }

    /////////////////////////////////////
    // recycle chips to the winner
    if(buff->dwMoney> 0) {
        _gameUICallback->recycleChipsById(buff->dwUserID);
    }
}

void GameTableLogic::dealGameEndResp(BYTE bDeskNO) {
    HNLOG("dealGameEndResp");

    /////////////////////////////////////
    // shuffle card
    _gameUICallback->shuffleCards(-1);

    /////////////////////////////////////
    // reload user to update their money
    loadDeskUsersUI();
}

//void GameTableLogic::dealUserNoContinueEnd(void* object, INT objectSize){}		// 91
void GameTableLogic::dealUserCutEnd(void* object, INT objectSize) {}				// 92
void GameTableLogic::dealUserSaleEnd(void* object, INT objectSize) {}			// 93
void GameTableLogic::dealUserNoWinner(void* object, INT objectSize) {}			// 94
void GameTableLogic::dealUserAheadEnd(void* object, INT objectSize) {}			// 95

void GameTableLogic::dealUserAiWin(void* object, INT objectSize) {}				// 100
void GameTableLogic::dealUserLeftResult(void* object, INT objectSize) {}			// 101
void GameTableLogic::dealUserMessage(void* object, INT objectSize) {}			// 102
void GameTableLogic::dealUserStopThing(void* object, INT objectSize) {}			// 103
void GameTableLogic::dealUserAgreeStop(void* object, INT objectSize) {}		// 104
void GameTableLogic::dealUserPlayFlash(void* object, INT objectSize) {}			// 105
void GameTableLogic::dealUserBeginFlash(void* object, INT objectSize) {}			// 106


void GameTableLogic::initGameData() {
    memset(_thisTurnNote, 0, sizeof(_thisTurnNote));
    memset(_thisGameNote, 0, sizeof(_thisGameNote));

    _nowBigNote	= 0;
    _noteTimesCount= 0;

    _nowOutPeople	= INVALID_DESKSTATION;
    _firstOutPeople= INVALID_DESKSTATION;
}



/////////////////////////////////////
// new

// send message MSG_GR_S_UserSit, MDM_GR_USER_ACTION, ASS_GR_USER_SIT
//void GameTableLogic::sendUserSit(BYTE seatId)
//{
//	MSG_GR_S_UserSit UserSit;
//	UserSit.bDeskIndex = _tableinfo.byDeskNo;
//	UserSit.bDeskStation = seatId;
//	HNRoomLogic::getInstance()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, (CHAR*)&UserSit, sizeof(UserSit));
//}

void GameTableLogic::sendUserUp() {
    do {
        if (!RoomLogic()->connected()) {
            leaveDesk();
            break;
        }

        if (INVALID_DESKSTATION == _mySeatNo) {
            leaveDesk();
            break;
        }

        UserInfoStruct* myInfo = _deskUserList->getUserByDeskStation(_mySeatNo);
        if(myInfo != nullptr && _userState[myInfo->bDeskStation]) {
            //GamePromptLayer::create()->showPrompt(GetText("GAME_PLAYING_NOT_EXIT"));
            break;
        }
        RoomLogic()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP);
    } while (0);
}

void GameTableLogic::enterDesk() {
    loadDeskUsersUI();

    // if the table is auto-created, send ready info to server
    if (_autoCreate) {
        sendGameInfo();
    }
}

void GameTableLogic::leaveDesk() {
    HNAudioEngine::getInstance()->resumeBackgroundMusic();
    if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME) {
        RoomLogic()->closeRoom();
        //GamePlatform::returnPlatform(LayerType::ROOMLIST);
        PlatformState* state = PlatformState::create();
        state->setType(GAMELIST);
//        if (VERSION_EIGHTY_EIGHT) {
//            state->setType(GAMELIST);
//        } else if (VERSION_NINETY_NINE) {
//            state->setType(GAMELIST_ROOM);
//        } else {
//            state->setType(DESKLIST);
//        }
        StateManager::getInstance()->setState(state);
    } else {
        RoomLogic()->closeRoom();
        //GamePlatform::returnPlatform(LayerType::ROOMLIST); // 临时处理2016 7 28 返回到房间
        PlatformState* state = PlatformState::create();
        state->setType(GAMELIST);
//        if (VERSION_EIGHTY_EIGHT) {
//            state->setType(GAMELIST);
//        } else if (VERSION_NINETY_NINE) {
//            state->setType(GAMELIST_ROOM);
//        } else {
//            state->setType(DESKLIST);
//        }
        StateManager::getInstance()->setState(state);
    }
}

void GameTableLogic::loadDeskUsersUI() {
    BYTE seatNo = INVALID_DESKNO;
    for(int i = 0; i < PLAY_COUNT; i++) {
        UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
        if(nullptr != pUser) {
            _gameUICallback->loadUser(i, pUser->dwUserID);
        }
    }
}

void GameTableLogic::stopAllTimer() {
    for(int i=0; i<PLAY_COUNT; i++) {
        BYTE seatId = HNGameLogicBase::logicToViewSeatNo(i);
        _gameUICallback->stopTimer(seatId);
    }
}


void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    HNLOG("dealUserSitResp");

    /////////////////////////////////////
    // load desk users(sitting)
    loadDeskUsersUI();

    /////////////////////////////////////
    // start timer
	if (_beginTimer>0 && USER_SITTING == userSit->bUserState &&userSit->bDeskStation == _mySeatNo) {
        _gameUICallback->startTimer(userSit->bDeskStation, _beginTimer);
        _userState[user->bDeskStation] = false;
    }

    /////////////////////////////////////
    // send gameinfo
    if(INVALID_DESKSTATION != _mySeatNo && user->bDeskStation==_mySeatNo) {
        sendGameInfo();
        if(USER_SITTING == userSit->bUserState) {
            _gameUICallback->setStartBtnVisible(true);
        }
    }
}

void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) {
    HNLOG("dealUserUpResp");

    if(user->dwUserID == PlatformLogic()->loginResult.dwUserID) {
        stop();
        leaveDesk();
    } else {
        _gameUICallback->showUserUp(userSit->bDeskStation);
    }
}


void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree) {
    HNLOG("dealUserAgreeResp");

    /////////////////////////////////////
    // start button & timer, stop the timer if the user is ready
    if(agree->bAgreeGame == 1) {
        _gameUICallback->stopTimer(agree->bDeskStation);
        //	_gameUICallback->killTimer();

        if(agree->bDeskStation==_mySeatNo)
            _gameUICallback->setStartBtnVisible(false);
        _gameUICallback->setAreadySpriteVisible(agree->bAgreeGame, agree->bDeskStation);
    }
}

//void GameTableLogic::dealGameStartResp(BYTE deskId)
//{
//	HNGameLogicBase::dealGameStartResp(deskId);
//
//	//_gameUICallback->startGame();
//}

void GameTableLogic::sendNote(LLONG money) {
    HNLOG("sendNote");
    tagUserProcess process;
    process.iNote = money + getMaxNote(_thisTurnNote);
    process.iVrebType = (_nowBigNote==0 ? TYPE_NOTE: TYPE_ADD);
    HN::HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, (CHAR*)&process, sizeof(tagUserProcess));
}

void GameTableLogic::sendFollow() {
    HNLOG("sendFollow");
    tagUserProcess process;
    process.iVrebType=TYPE_FOLLOW;
    //process.iNote=0;
    HN::HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, (CHAR*)&process, sizeof(tagUserProcess));
}
void GameTableLogic::sendGiveUp() {
    HNLOG("sendGiveUp");
    tagUserProcess process;
    process.iVrebType=TYPE_GIVE_UP;
    process.iNote=0;
    HN::HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, (CHAR*)&process, sizeof(tagUserProcess));
}
void GameTableLogic::sendShuttle() {
    HNLOG("sendShuttle");
    tagUserProcess process;
    process.iVrebType=TYPE_SHUTTLE;
    process.iNote=0;
    HN::HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, ASS_VREB_CHECK, (CHAR*)&process, sizeof(tagUserProcess));
}


LLONG GameTableLogic::getMaxNote(LLONG notes[]) {
    LLONG money=-1;

    for(int i=0; i<PLAY_COUNT; i++) {
        if(notes[i]>money) {
            money=notes[i];
        }
    }
    return money;
}

BYTE GameTableLogic::getGameStatus() {
    return _gameStatus;
}


// ASS_GM_GAME_STATION
// login from disconnection
void GameTableLogic::dealGameStationResp(void* object, INT objectSize) {
    HNLOG("dealGameStationResp");
    auto stationBase = (GameStation_Base*)object;

    /////////////////////////////////////
    // init dashboard data
    _followTimesLimit	= stationBase->byUserFollow;
    _showHaTimesLimit	= stationBase->bShowHa;
    _noteTimesLimit	= stationBase->m_byAddNum;
    _pk				= stationBase->bPK;

    _noteTimesCount	= 0;
    _handcardCount		= stationBase->iAllCardCount;

    _baseNote			= stationBase->iBaseNote;
    _limitNote			= stationBase->iLimitNote;

    /////////////////////////////////////
    // init game ui
    auto data=new GameBeginStruct();
    data->iBeenPlayGame	= 0;
    data->iBaseNote		= _baseNote;
    data->iLimitNote	= _limitNote;
    memcpy(&data->bUserState, &stationBase->bUserState, sizeof(BYTE)*PLAY_COUNT);
    _gameUICallback->initGameUi(data);
    _gameUICallback->upDataHistoyAndLastMoney(_hisToryMoney, _lastMoney);
    /////////////////////////////////////
    // load desk user
    loadDeskUsersUI();

    /////////////////////////////////////
    // init timer
    _thinkTimer=stationBase->iThinkTime;
    _beginTimer=stationBase->iBeginTime;
    //_gameStatus = stationBase->bGameStation;
    /////////////////////////////////////
    // init sound engine


    /////////////////////////////////////
    // game station 12345
    switch(stationBase->bGameStation) {
    case GS_WAIT_SETGAME:
    //HNLOG("GS_WAIT_SETGAME");

    case GS_WAIT_ARGEE:
        //HNLOG("GS_WAIT_ARGEE");
        setGameStation_2(object, objectSize);
        break;

    case GS_SEND_CARD:
        //HNLOG("GS_SEND_CARD");
        setGameStation_3(object, objectSize);
        break;

    case GS_PLAY_GAME:
        //HNLOG("GS_PLAY_GAME");
        setGameStation_4(object, objectSize);
        break;

    case GS_WAIT_NEXT:
        //HNLOG("GS_WAIT_NEXT");
        setGameStation_5(object, objectSize);
        break;
    }

}

// GS_WAIT_ARGEE
void GameTableLogic::setGameStation_2(void* object, INT objectSize) {
    HNLOG("setGameStation_2: GS_WAIT_ARGEE");
    m_gameState = GAME_FREE;
    auto station2=(GameStation_2*) object;
    for(int i=0; i<PLAY_COUNT; i++) {
        if(_deskUserList->getUserByDeskStation(i)==nullptr)
            continue;
        if (station2->bAgree[i])
            _gameUICallback->setAreadySpriteVisible(true, i);

        if(!station2->bAgree[i] && (USER_SITTING==_deskUserList->getUserByDeskStation(i)->bUserState)) {
            _gameUICallback->startTimer(i, _beginTimer);

            if(i == _mySeatNo)
                _gameUICallback->setStartBtnVisible(!station2->bAgree[i]);
        } else {
            _gameUICallback->stopTimer(i);
        }
    }
}

// GS_SEND_CARD
void GameTableLogic::setGameStation_3(void* object, INT objectSize) {
    HNLOG("setGameStation_3: GS_SEND_CARD");
    auto station3=(GameStation_3*) object;
    m_gameState = GAME_PLAYING;

    /////////////////////////////////////
    // update dashboard
    _gameUICallback->setBetBtnEnabled(false);
    _gameUICallback->setMesaVisible(true);
    /////////////////////////////////////
    // show max card mark
    _startSeat=station3->iUpGradePeople;
    _gameUICallback->showMaxCardMark(true, _startSeat);

    /////////////////////////////////////
    // handcard & total note
    for(int i=0; i<PLAY_COUNT; i++) {
        memcpy(&_cardList[i],&station3->iUserCard[i],sizeof(BYTE)*HANDCARD_COUNT);
        _gameUICallback->updateNote(i, station3->iTotalNote[i]);
    }
    _gameUICallback->recordDeskInfo(station3->iTotalNote);
    _gameUICallback->setStartBtnVisible(false);
    _gameUICallback->setBetAndOtherBtnOpen(true);

    /////////////////////////////////////
    // update handcard
    _handcardCount=station3->iUserCardCount[0];
    //	station3->iUserCard[0][0]=0x00;
    //station3->iUserCard[1][0]=0x00;
    _gameUICallback->shuffleCards(_handcardCount);
    _gameUICallback->updateHandcard(_handcardCount, station3->iUserCard);
    _gameUICallback->HidTopPlayerOneCard();

}

// GS_PLAY_GAME
void GameTableLogic::setGameStation_4(void* object, INT objectSize) {
    HNLOG("setGameStation_4: GS_PLAY_GAME");

    auto station4=(GameStation_4*) object;
    m_gameState = GAME_PLAYING;
    /////////////////////////////////////
    // dashboard
    _nowBigNote = station4->tag.iNowBigNote;
    _nowOutPeople=station4->iOutCardPeople;
    setDashboard(station4->tag.bNoteTimes, station4->tag.iNowBigNoteStyle, station4->tag.iNowBigNote, station4->tag.m_byUserAddNum, station4->tag.bNewTurn);

    /////////////////////////////////////
    // show max card mark
    _startSeat=station4->iUpGradePeople;

    _gameUICallback->showMaxCardMark(true, _startSeat);
    _gameUICallback->setStartBtnVisible(false);
    _gameUICallback->setBetAndOtherBtnOpen(true);

    _gameUICallback->recordDeskInfo(station4->iTotalNote);

    for (int i = 0; i < PLAY_COUNT; i++) {
        _thisGameNote[i] = station4->iTotalNote[i];
        _thisTurnNote[i] = station4->iThisTurnNote[i];
    }

    //恢复玩家信息
    for (int i = 0; i < PLAY_COUNT; i++) {
        if (i == logicToViewSeatNo(station4->iUpGradePeople)) {
            _gameUICallback->recoveryUserDataCallBack(i,true);
        } else {
            _gameUICallback->recoveryUserDataCallBack(i, false);
        }
    }

    /////////////////////////////////////
    // handcard & total note
    for(int i=0; i<PLAY_COUNT; i++) {
        memcpy(&_cardList[i],&station4->iUserCard[i],sizeof(BYTE)*HANDCARD_COUNT);
        _gameUICallback->updateNote(i, station4->iTotalNote[i]);
    }

    /////////////////////////////////////
    // update handcard
    _handcardCount=station4->iUserCardCount[0];
    //station4->iUserCard[0][0]=0x00;
    //station4->iUserCard[1][0]=0x00;
    _gameUICallback->setMesaVisible(true);
    _gameUICallback->shuffleCards(_handcardCount);
    _gameUICallback->updateHandcard(_handcardCount, station4->iUserCard);
    _gameUICallback->HidTopPlayerOneCard();

    /////////////////////////////////////
    // note timer
    if(_nowOutPeople!=INVALID_DESKSTATION)
        _gameUICallback->startTimer(_nowOutPeople, _thinkTimer);
}

// GS_WAIT_NEXT
void GameTableLogic::setGameStation_5(void* object, INT objectSize) {
    HNLOG("setGameStation_5: GS_WAIT_NEXT");
    auto station5=(GameStation_5*) object;
    m_gameState = GAME_FREE;
    for(int i=0; i<PLAY_COUNT; i++) {
        if(_deskUserList->getUserByDeskStation(i)==nullptr)
            continue;
        if (station5->bAgree[i])
            _gameUICallback->setAreadySpriteVisible(true, i);
        if(!station5->bAgree[i] && (USER_SITTING==_deskUserList->getUserByDeskStation(i)->bUserState)) {
            _gameUICallback->startTimer(i, _beginTimer);

            if(i == _mySeatNo)
                _gameUICallback->setStartBtnVisible(!station5->bAgree[i]);
        } else {
            _gameUICallback->stopTimer(i);
        }
    }
}

void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo) {
    HNLOG("dealGameInfoResp");
}
void GameTableLogic::dealGameStartResp(BYTE deskId) {
    HNLOG("dealGameInfoResp");
}

void GameTableLogic::sendDriveNianShou(int nUserID) {
//    NewYearActivityInGame::GanNianShouRequest msg;
//    msg.nUserID = nUserID;
//    //0活动1游戏
//    msg.bSceneType = 1;
//
//    HNRoomLogic::getInstance()->sendData(MDM_GM_GAME_NOTIFY, NewYearActivityInGame::C2S_DRIVE_NIANSHOUR_REQUEST, &msg, sizeof(msg));
}


}
