#ifndef __GoldenFlowerGame_Table_UI_V88_H__
#define __GoldenFlowerGame_Table_UI_V88_H__

#include "GoldenFlowerGameTableUICallback.h"
#include "GoldenFlowerGamePlayerV88.h"
#include "GoldenFlowerPokerCardV88.h"
#include "GoldenFlowerGameDealerUIV88.h"
#include "GoldenFlowerGameCardTypeV88.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"
//#include "../../TTNN/Classes/v88/BRNNClockV88.h"


#include "HNNetExport.h"
#include "cocos2d.h"
#include <vector>
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "UI/Base/AUIButton.h"
#include "UI/TimerObject.h"

using namespace cocos2d::extension;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace goldenflower {
/*
* game ui unit.
*/

class GameTableLogic;
class DashboardV88;

class GameTableUIV88 : public HNGameUIBase, public GameTableUICallback {
public:
    static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

    virtual bool init(BYTE bDeskIndex, bool bAutoCreate);
    virtual void onEnterTransitionDidFinish() override;
    virtual void onEnter() override;
    virtual void onExit() override;

private:

    enum Em_TableBtnTag {
        em_toolbarBtn = 10000,
        em_toolbarSetBtn,
        em_toolbarHelpBtn,
        em_toolbarLeaveBtn,
        em_toolbarNextOutBtn,
		em_toolbarNextOutLight
    };

    typedef struct TTableUI {
        Node* lOutline;
        Node* Node_Card[PLAY_COUNT];
        ui::ImageView* bCompare[PLAY_COUNT];
        ui::ImageView* ReadySign[PLAY_COUNT];
        ImageView* WatchSign[PLAY_COUNT];
        ImageView* GiveUpSign[PLAY_COUNT];
        ImageView* next_game;
		Sprite* head_image[2];                //比牌时头像框
        Sprite* bSeat[PLAY_COUNT];
        ImageView* iDealer[PLAY_COUNT];
        ImageView* iCard[PLAY_COUNT];
        Node* Nbet[PLAY_COUNT];
//		ui::Button* bCardType;
        //ui::Button* bJumpMenu[4];
        //ui::Button* bBackMenu;
        //ui::Layout* nPullDownMenu;
        ImageView*  iTable;
        AUIButton* bReady;
//		ui::Text* lGuoDi;
//		ui::Text* lLimitNote;
        ui::Text* lNote;        //底注
//		ui::Text* lTopNote;
        ui::Text* lLastNote;
        ui::Text* lMyNote;
        Node* lTotalNode;
        ui::Text* lTotalNote;
        ui::Text* lOperateCount;
        // 计时器背景
        ui::ImageView* ImageView_TimerBg;
        // 计时器数字
        ui::TextAtlas* TextAtlas_TimerText;


		
        TTableUI()
//			: bCardType(nullptr)
        //, nPullDownMenu(nullptr)
        //, bBackMenu(nullptr)
            : iTable(nullptr)
            , bReady(nullptr)
//			, lGuoDi(nullptr)
//			, lLimitNote(nullptr)
            , lNote(nullptr)
//			, lTopNote(nullptr)
            , lTotalNode(nullptr)
            , lTotalNote(nullptr)
            , lOperateCount(nullptr)
            , next_game(nullptr)
            , ImageView_TimerBg(nullptr)
            , TextAtlas_TimerText(nullptr) {
            memset(Node_Card, 0, sizeof(Node_Card));
            memset(ReadySign, 0, sizeof(ReadySign));
            memset(bCompare, 0, sizeof(bCompare));
            memset(bSeat, 0, sizeof(bSeat));
            //memset(bJumpMenu, 0 , sizeof(bJumpMenu));
            memset(Nbet, 0, sizeof(Nbet));
            memset(WatchSign, 0, sizeof(WatchSign));
            memset(GiveUpSign, 0, sizeof(GiveUpSign));
            memset(head_image, 0, sizeof(head_image));
        }
    } TTableUI;

    //json文件读取位置
    cocos2d::Vec2 _seatPosition[PLAY_COUNT];
    cocos2d::Vec2 _nodeCardPosition[PLAY_COUNT];
    cocos2d::Vec2 _jettonPosition[PLAY_COUNT];
    BYTE _viewSeat[PLAY_COUNT];
    S_C_GameEnd _pendData;
    BYTE _viewOfWiner;
    cocos2d::Vec2 _dealerPostion;
    cocos2d::Vec2 _headImagePostion[2];                                     //记录比牌时的头像位置
    Sprite* _loadSprite;													//桌子预加载蒙版

    PlayerUIV88* _players[PLAY_COUNT];
    //Label* _lRoomInfo;
    GameTableLogic* _tableLogic;
    GameDelearUIV88* _dealer;
    DashboardV88* _dashboard;

	LayerColor* _colorLayer;

    //结算层
    Node* _winNode;
    Node* _loseNode;
    cocostudio::timeline::ActionTimeline* _winAction;
    cocostudio::timeline::ActionTimeline* _loseAction;
    Node* _endNode;

    TTableUI _tableUI;
    std::vector<PokerCardV88 *> _vecCard;
    int _visibleWidth, _visibleHeight;

    Sprite*						_toolbarBg;					//工具按钮背景
    bool						m_bNextOut;					//下局离开

    //VS Animation
    Node* _vsNode;
    Node* _hitNode;

    //金币不足退出提示
    tag_s2c_tip_exit*	tipMsg;
    Text*	m_TextTip;
    long long    m_i64TipMoney;

    //公告
    GameNotice*					m_Notice;   //广播

    //超时自动退出
//    bool        m_leaveTimeOut;
    BYTE m_DeskIndex;
    bool m_AutoCreate;

	TimerObject*             m_clockObj;

	float						_fScaleX;
	float						_fScaleY;
    
    ImageView*              m_myTipImage;
    
private:
    std::vector<bool> _playerReadyState;
    bool _canShowReadySign;

private:
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);

	void hideTips(float delat);


private:
    void createToolBarUI();
//    void onToolBtnClick(Ref* pSender, Control::EventType event);

    int getPlayerCount();
    //menu click callback.
    void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
    //void EndLayerClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
    //seat menu click callback.
//	void seatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
    //compare menu click callbakc.
    void compareClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

    //void pullDownItemCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
    //void menuTouchReset(Button* btn);

public:
    /*
     * user note aciton from dashboard.
     */
    void compareCardReq();
    void lookCard();
    void compareCard();
    void followNote();
    void giveUpNote();
    void addNote(LLONG money = 1000);
    void addBet(int multiple);
    void getReady();

public:
    // 红包出现
    virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
    // 打开红包结果
    virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
    
    
public:
    /*
     * dispatch user hand card.
     @ param cards users hand card
     */
    virtual void showHandCard(std::vector<THandCard>& cards) override;
    /*
     * show user's hand card.
     * @param seatNo user's seat number.
     * @param cards user's hand card.
     */
    virtual void showUserHandCard(BYTE seatNo, const std::vector<BYTE>& cards);
    virtual void showUserFlipCard(BYTE seatNo, const std::vector<BYTE>& cards);


    /*
     * show user when user sit in table.
     * @param seatNo user's seat number.
     * @param bMe if it is login user.
     */
    virtual void showUser(BYTE seatNo, bool bMe, bool sex) override;
    /*
     * clear user when user leave table.
     * @param seatNo user's seat number.
     * @param bMe if it is login user.
     */
    virtual void showUserUp(BYTE seatNo, bool bMe) override;
    virtual void showUserName(BYTE seatNo,CHAR* name) override;
    virtual void showUserMoney(BYTE seatNo,LLONG MONEY) override;
    virtual void showDealer(BYTE seatNo) override;
    virtual void showUserNoteMoney(BYTE seatNo, LLONG money/*, int DiZhu*/) override;
    virtual void showUserTotalBetMoney(BYTE seatNo, LLONG betMoney) override;
    void updateNoteMoney(BYTE seatNo,LLONG money);
    virtual void showWatchCard(BYTE seatNo, bool visible) override;
    virtual void showGiveUpCard(BYTE seatNo, bool visible) override;
    virtual void showReadySign(BYTE seatNo, bool visible) override;
    virtual void storeReadyState(BYTE seatNo, bool ready) override;
    //virtual void showAddRange(LLONG min, LLONG max);

    //send prize to winer.
    virtual void showWin(BYTE winnerSeatNo) override;

    //显示结算框
    virtual void showEndBox(S_C_GameEnd *pdata, BYTE *viewSeat, BYTE viewWiner) override;
    void startEndBox(S_C_GameEnd *pdata, BYTE *viewSeat, BYTE viewWiner);
    void playBigWinAnimation(BYTE cardShape);
    
    virtual void clearDesk() override;
    virtual void clearInvalidUser() override;
    virtual void leaveTheGame() override;
    virtual void showLoading(bool bVisible) override;
    virtual void rotateSeat(int offset) override;
    virtual void alertDialog(const std::string& title, const std::string& message) override;
    //virtual void showNotice(const std::string &message);
    virtual void showForbidLeaveNotice() override;
    virtual void showUserProfit(BYTE seatNo, LLONG money) override;
    //virtual void showTableInfo(const std::string& tableName);
    // 启动倒计时
    virtual void IStartTimer(int second, BYTE type = 0) override;
    /*
     * show wait time for user to ready.
     * @param seatNo user's seat number.
     * @param byTime wait time.
     */
    virtual void showWaitTime(BYTE seatNo, BYTE byTime, BYTE byTotalTime) override;
    /*
     * show note info when game start.
     */
//	virtual void showLimitNote(LLONG note);
    virtual void showBaseNote(LLONG note) override;
    virtual void updateOperateCount(int current, int maxLoop) override;
//	virtual void showLimitPerNote(LLONG note);
//	virtual void showGuoDi(LLONG note);
    virtual void controlShowOrHideTotalNode(bool show = true) override;
    virtual void showTotalNote(LLONG note) override;
    virtual void showTotalNoteFirst(LLONG note) override;
    //virtual void showMyTotalNote(LLONG note);
    virtual void showReady(bool visible) override;
    virtual void showReadyPrivate() override;

    virtual void delayShowReady(bool visible = true) override;
    virtual void showNextGame(bool visible) override;
    /*
     * show user's aciton
     */
    virtual void showUserLookCard(BYTE seatNo, bool isMe) override;
    virtual void showUserGiveUp(BYTE seatNo) override;
    virtual void updateBetButtonValues(int values[4]) override;
    virtual void showUserNote(BYTE seatNo) override;
    virtual void showUserCompare(BYTE fromSeatNo, BYTE toSeatNo, BYTE winSeatNo) override;
    virtual void showUserFollow(BYTE seatNo) override;
    //virtual void showUserAdd(BYTE seatNo);
    virtual void showUserOpenCard(BYTE seatNo) override;

    virtual void dealGameTipExit(tag_s2c_tip_exit* msg) override;

    /*
     * show user action button
     * @param byFlag each bit meaning each action
     */
    virtual void showDashboard(bool visible) override;

    virtual void setLookVisible(bool visible) override;
    virtual void setFollowVisible(bool visible) override;
    virtual void setAddVisible(bool visible, bool *CanAdd) override;
    virtual void setOpenVisible(bool visible) override;
    virtual void setGiveUpVisible(bool visible) override;
    /*
     *  show compare result
     */
    virtual void showCompareResult(BYTE winSeatNo, BYTE loseSeatNo) override;
    virtual void showCompareOption(const std::vector<bool> seats) override;


    //全服公告
    virtual void onGameNotify(tag_s2c_notify* msg) override;
    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
    void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);

    void playVSAnimation();
    void playHitAnimation(Vec2 hitPos);
    void playBombAnimate(float duration, bool leftWin);
    //void moveBomb(bool leftWiner);
    //比牌完后移除头像
    void moveHead(bool winnerIsLeft, BYTE winSeatNo);
    void boom(cocos2d::Ref* pRef);
    Vec2 getCardPosition(BYTE seatNo);
    Vec2 getSeatPosition(BYTE seatNo);



protected:
    //牌桌界面初始化
    void inflateLayout();
    bool seatNoIsOk(BYTE seatNo);
    void initGameData();
    void initPosition();
//    void timerUpdate(float delat);
    void releaseGameEndBox(float dt);
    void disapearEndBox(float dt);


 
//    void showCardType();
    GameTableUIV88();
    virtual ~GameTableUIV88();
    friend class GameDelearUI;
    // 计时器跑秒时间
    int _remainTime;
public:
	PlayerUIV88* getPlayer(int seatNo);
	PlayerUIV88* getPlayer(LLONG userId);
private:
    void initVSAnimation();
    void initHitAnimation();

    void resetNewGameUI(float dt);

    void tipCallback();
    void updateTip(float dt);
//    void loadingTextureCallback(Texture2D* textureData, std::string plist);


//public:
//    //年兽触发
//    virtual void onGameShowNianShou() override;
//    //赶年兽结果
//    virtual void onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) override;
//    //对局获得鞭炮
//    virtual void onGameMatchGainFirecraker(int nUserID) override;
//private:
//    //年兽动画
//    void showNianShouAnimation();
//    //赶年兽按钮
//    void driveNianShouClickCallback(Ref* pSender, Widget::TouchEventType type);
//    //移除年兽按钮
//    void removeNianShouButton();
//    //5s自动移除年兽按钮
//    void autoRemoveNianShouButton(float dt);

};
}


#endif	//__GoldenFlowerGame_Table_UI_V88_H__
