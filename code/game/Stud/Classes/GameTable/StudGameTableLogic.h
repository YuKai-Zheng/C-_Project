#ifndef __STUD_Game_Table_Logic_H__
#define __STUD_Game_Table_Logic_H__

#include "HNNetExport.h"

#include "../../NN/Classes/GameMessage/NNCommonDef.h"

//#include "NewYearInGameMessageHead.h"

namespace STUD {
class GameTableUICallback;

/*
 * Stud
 * game table logic deal unit.
 */
class GameTableLogic: public HN::HNGameLogicBase {
public:
    GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
    virtual ~GameTableLogic();

    virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize);


public:
    //void sendUserSit(BYTE seatId);
    void enterDesk();
    void leaveDesk();
    void loadDeskUsersUI();
    void stopAllTimer();

    void sendUserUp();
    void sendNote(LLONG money);
    void sendFollow();
    void sendGiveUp();
    void sendShuttle();

    void setDashboard(BYTE noteTimes, BYTE lastNoteStyle, LLONG notedMoney, BYTE playerNoteTimes[], bool newTurn);

    BYTE getGameStatus();

protected:
    void dealUserSetResp(void* object, INT objectSize);				// 50
    void dealUserSetChangeResp(void* object, INT objectSize);		// 51
    void dealUserGameBegin(void* object, INT objectSize);			// 52
    void dealUserSendCard(void* object, INT objectSize);			// 53
    void dealUserSendFinish(void* object, INT objectSize);			// 54
    void dealUserGamePlay(void* object, INT objectSize);			// 55
    void dealUserNoteLimit(void* object, INT objectSize);			// 56

    void dealUserNote(void* object, INT objectSize);				// 60
    void dealUserVrebCheck(void* object, INT objectSize);			// 61
    void dealUserReplaceVrebCheck(void* object, INT objectSize);	// 62
    void dealUserNoteResult(void* object, INT objectSize);			// 63

    void dealUserCollectJeton(void* object, INT objectSize);		// 70
    void dealUserAiStation(void* object, INT objectSize);			// 71

    void dealUserNewTurn(void* object, INT objectSize);				// 80
    void dealUserSendAcard(void* object, INT objectSize);			// 81

    void dealUserContinueEnd(void* object, INT objectSize);			// 90
    //void dealUserNoContinueEnd(void* object, INT objectSize);		// 91
    void dealUserCutEnd(void* object, INT objectSize);				// 92
    void dealUserSaleEnd(void* object, INT objectSize);				// 93
    void dealUserNoWinner(void* object, INT objectSize);			// 94
    void dealUserAheadEnd(void* object, INT objectSize);			// 95

    void dealUserAiWin(void* object, INT objectSize);				// 100
    void dealUserLeftResult(void* object, INT objectSize);			// 101
    void dealUserMessage(void* object, INT objectSize);				// 102
    void dealUserStopThing(void* object, INT objectSize);			// 103
    void dealUserAgreeStop(void* object, INT objectSize);			// 104
    void dealUserPlayFlash(void* object, INT objectSize);			// 105
    void dealUserBeginFlash(void* object, INT objectSize);			// 106
    void dealMoenyNotEnouth(void* object, INT objectSize);					//107

    //virtual void dealUserUpResp(MSG_GR__UserSit * userSit, UserInfoStruct* user);

    //virtual void dealGameStationResp(void* object, INT objectSize);
    virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
    virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit,  UserInfoStruct* user);
    virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
    //virtual void dealGameEndResp(BYTE deskNo);
    virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
    virtual void dealGameStartResp(BYTE deskId);
    virtual void dealGamePointResp(void* object, INT objectSize);
    virtual void dealGameEndResp(BYTE bDeskNO);

    virtual void dealGameStationResp(void* object, INT objectSize);
    //virtual void setGameStation_1(void* object, INT objectSize);
    virtual void setGameStation_2(void* object, INT objectSize);
    virtual void setGameStation_3(void* object, INT objectSize);
    virtual void setGameStation_4(void* object, INT objectSize);
    virtual void setGameStation_5(void* object, INT objectSize);

    void onGameNotify(NN::tag_s2c_notify* msg);
protected:
    GameTableUICallback* _gameUICallback;

    // note
    LLONG _thisTurnNote[PLAY_COUNT];//
    LLONG _thisGameNote[PLAY_COUNT];
    LLONG _nowBigNote;
    LLONG _baseNote;
	LLONG _limitNote;
	LLONG _hisToryMoney;
	LLONG _lastMoney;

    // dashboard
    BYTE _followTimesLimit;
    BYTE _showHaTimesLimit;
    BYTE _noteTimesLimit;
    BYTE _noteTimesCount;
    bool _pk;

    // current active player(who should take actions this turn)
    BYTE _nowOutPeople;
    BYTE _firstOutPeople;

    bool _userState[PLAY_COUNT];

    // timer
    BYTE _thinkTimer;
    BYTE _beginTimer;

    bool _autoCreate;								// the game is auto created or not

    //BYTE _buttonSeat;								// the seat who will note first
    BYTE _startSeat;								// the seat who will be sent card first
    BYTE _handcardCount;							// total handcards received per person
    BYTE _cardList[PLAY_COUNT][HANDCARD_COUNT];	// handcards

    //LimitStruct* _noteLimit;

protected:
    LLONG getMaxNote(LLONG notes[]);

    void initGameData();
public:
    BYTE m_gameState;

public:
    //赶年兽
    void sendDriveNianShou(int nUserID);

};
}


#endif
