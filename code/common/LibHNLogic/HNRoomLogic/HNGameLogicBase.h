#ifndef __HNGameLogicBase_H__
#define __HNGameLogicBase_H__

#include "HNLogic/HNLogicBase.h"
#include "HNNetExport.h"

namespace HN {
/*
 * @brief: redbird game logic interface.
 */
class IHNGameLogicBase {
public:
    virtual void onGameDisconnect() {}
    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) {}
};

/*
 * @brief: redbird game logic.
 */
class HNGameLogicBase : public HNLogicBase,
    public IGameMessageDelegate,
    public IPlatformMessageDelegate {
public:
    /*
     * @brief: will start receive message after called.
     */
    virtual void start() override;
    /*
     * @brief: will stop receive message after called.
     */
    virtual void stop() override;
    /*
     * @brief: send message to room server from client.
     */
    virtual void sendGameInfo();
    virtual void sendUserUp();
    virtual void sendUserSit(BYTE lSeatNo);
    virtual void sendForceQuit();
    virtual void sendAgreeGame();
    virtual void sendChatMsg(const std::string &msg);

    /*
     * @brief: translate between view seat no and logic seat no.
     */
    virtual BYTE viewToLogicSeatNo(BYTE vSeatNO);
    virtual BYTE logicToViewSeatNo(BYTE lSeatNO);

    /*
     * @brief: get user id from logic seatNo.
     */
    INT getUserId(BYTE lSeatNo);
    /*
     * @brief: get my seatNo.
     */
    BYTE getMySeatNo() const;
    /*
     * @brief:  get user whether is boy.
     * @param:
     *       lSeatNo is user logic seatNo.
     * @return: true if it is boy, or false.
     */
    bool getUserIsBoy(BYTE lSeatNo);
    /*
     * @brief:  get user's info from userId.
     * @param:  userID is user's id.
     * @return: return user's info.
     */
    UserInfoStruct *getUserByUserID(INT userID);
    /*
     * @brief:  get user's info from logic seatNo.
     * @param:  lSeatNo is user's logic seatNo.
     * @return: return user's info.
     */
    UserInfoStruct *getUserBySeatNo(BYTE lSeatNo);

public:
    HNGameLogicBase(BYTE deskNo, INT maxPlayers, bool autoCreate,
                    IHNGameLogicBase *callback);
    virtual ~HNGameLogicBase();

protected:
    virtual void loadDeskUsers() final;
    virtual void clearDesk();

protected:
    /*
     * framework message deal function.
     */
    virtual void dealUserAgreeResp(MSG_GR_R_UserAgree *agree);
    virtual void dealGameStartResp(BYTE bDeskNO);
    virtual void dealGameEndResp(BYTE bDeskNO);
    virtual void dealUserSitResp(MSG_GR_R_UserSit *userSit, UserInfoStruct *user);
    virtual void
    dealQueueUserSitMessage(BYTE deskNo,
                            const std::vector<QUEUE_USER_SIT_RESULT *> &users);
    virtual void dealUserUpResp(MSG_GR_R_UserUp *userSit, UserInfoStruct *user);
    virtual void dealGameInfoResp(MSG_GM_S_GameInfo *pGameInfo);
    virtual void dealGameStationResp(void *object, INT objectSize) = 0;
    virtual void dealGameMessage(NetMessageHead *messageHead, void *object,
                                 INT objectSize) = 0;
    virtual void dealUserCutMessageResp(INT userId, BYTE seatNo);
    virtual void dealGamePointResp(void *object, INT objectSize);
    virtual void dealUserChatMessage(MSG_GR_RS_NormalTalk *normalTalk);
    // 比赛初始化
    virtual void
    dealGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT *contestChange);
    //比赛信息广播
    virtual void dealGameContestNotic(MSG_GR_I_ContestInfo *contestInfo);
    // 用户比赛信息
    virtual void dealGameUserContset(MSG_GR_ContestChange *contestChange);
    // 比赛结束
    virtual void dealGameContestOver(MSG_GR_ContestAward *contestAward);
    // 比赛淘汰
    virtual void dealGameContestKick();
    // 等待比赛结束
    virtual void dealGameContestWaitOver();
    // 比赛奖励
    virtual void
    dealGameContestAwards(NET_ROOM_CONTEST_AWARD_RESULT *contestAward);
    // 报名数量
    virtual void
    dealGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT *contestPeople);
    // 个人参赛纪录
    virtual void
    dealGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT *ContestRecord);

protected:
    /*
     * init member only once.
     */
    virtual void initParams();
    /*
     * refresh member before every game start.
     */
    virtual void refreshParams();
    /*
     * get next and last user seat no.
     * @param:  seatNo find seat no.
     * @return: return seat no if success, or return INVALID_DESKSTATION
     */
    /*
     * @brief:  get next user's logic seatNo.
     */
    virtual BYTE getNextUserSeatNo(BYTE lSeatNo);
    /*
     * @brief:  get last user's logic seatNo.
     */
    virtual BYTE getLastUserSeatNo(BYTE lSeatNo);

private:
    /*
     * @brief:  platfrom message.
     */
    virtual void onDisConnect() override;
    virtual void onUserAgreeMessage(MSG_GR_R_UserAgree *agree) override;
    virtual void onGameStartMessage(BYTE bDeskNO) override;
    virtual void onGameEndMessage(BYTE bDeskNO) override;
    virtual void onGameInfoMessage(MSG_GM_S_GameInfo *pGameInfo) override;
    virtual void onGameStationMessage(void *object, INT objectSize) override;
    virtual bool onGameMessage(NetMessageHead *messageHead, void *object,
                               INT objectSize) override;
    virtual void onGamePointMessage(void *object, INT objectSize) override;
    /*
     * user aciton intertace
     */
    // 排队用户坐下
    virtual void onQueueUserSitMessage(
        BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT *> &users) override;
    // 用户坐下
    virtual void onUserSitMessage(MSG_GR_R_UserSit *userSit,
                                  UserInfoStruct *user) override;
    // 用户站起
    virtual void onUserUpMessage(MSG_GR_R_UserUp *userUp,
                                 UserInfoStruct *user) override;
    // 用户断线
    virtual void onUserCutMessage(INT dwUserID, BYTE bDeskNO,
                                  BYTE bDeskStation) override;
    // 比赛初始化
    virtual void
    onGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT *contestChange) override;

    //比赛信息广播
    virtual void onGameContestNotic(MSG_GR_I_ContestInfo *contestInfo) override;
    // 用户比赛信息
    virtual void onGameUserContset(MSG_GR_ContestChange *contestChange) override;
    // 比赛结束
    virtual void onGameContestOver(MSG_GR_ContestAward *contestAward) override;
    // 比赛淘汰
    virtual void onGameContestKick() override;
    // 等待比赛结束
    virtual void onGameContestWaitOver() override;

    // 比赛奖励
    virtual void onGameContestAwards(
        std::vector<NET_ROOM_CONTEST_AWARD_RESULT *> *awards) override {}
    // 报名数量
    virtual void
    onGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT *contestPeople) override {}
    // 个人参赛纪录
    virtual void
    onGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT *contestRecord) override {}
    // 聊天消息
    virtual void onUserChatMessage(void *object, INT objectSize) override;

public:
    // 接收大厅的新闻系统消息
    virtual void onNewsMessage(MSG_GR_RS_NormalTalk *pData) override;

protected:
    IHNGameLogicBase *_callback;
    HNGameTableUsersData *_deskUserList;
    BYTE _deskNo;
    BYTE _mySeatNo;
    BYTE _watchOther; // 允许旁观

    // + is clockwise, - is opposite.
    int _seatOffset; // 座位偏移量
    // before auto action, player's action time.
    BYTE _waitTime; // 等待时间
    // max player count in the table.
    INT _maxPlayers;
    // if the table is auto created
    bool _autoCreate;
    std::vector<bool> _existPlayer;
    std::vector<bool> _playing;
public:
    BYTE						_gameStatus;	// 游戏状态
};
}

#endif