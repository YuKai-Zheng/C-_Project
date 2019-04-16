#ifndef __STUD_Game_Table_UI_V88_H__
#define __STUD_Game_Table_UI_V88_H__

#include "cocos2d.h"
#include "HNNetExport.h"

#include "StudGamePlayerV88.h"
#include "StudPokerCardV88.h"
#include "StudGameUserMessageBoxV88.h"
#include "StudGameTableUICallback.h"

#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "UI/Base/AUIButton.h"
#include "UI/Base/AControlButton.h"
#include "UI/TimerObject.h"
#include "HNLobby/GameNotice.h"
#include "../../NN/Classes/GameMessage/NNCommonDef.h"

using namespace cocos2d::ui;
using namespace cocos2d::extension;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace STUD {
/*
 * game ui unit.
 */
const INT  HANDCARD_OFFSET_V88	= 	20;

class PokerCardV88;
class GameTableLogic;
class PlayerUIV88;

class GameTableUIV88 : public HN::HNGameUIBase, public GameTableUICallback {
public:
    static HNGameUIBase* create(BYTE deskId, bool autoCreate);
    virtual bool init(BYTE deskId, bool autoCreate);
    virtual void onExit() override;
    virtual void onEnter()override;
    GameTableUIV88();
    virtual ~GameTableUIV88();

    // update & show desk user(sitting)
    virtual void loadUser(BYTE seatId, LLONG userId) override;
    void createClock();
    void startClock(BYTE seatID, int time);
    void stopClock();
    float readTimer();
    // show user up
    virtual void showUserUp(BYTE seatId) override;

    // set timer
//    virtual void startTimer(BYTE seatId, BYTE leftTime, BYTE totalTime) override;
    virtual void startTimer(BYTE seatId, BYTE leftTime) override;
    virtual void stopTimer(BYTE seatId) override;

    virtual void initGameUi(GameBeginStruct* buff) override;

    //清空
    virtual void resetNote() override;
    
    // update note
    virtual void updateNote(BYTE seatId, LLONG money) override;
//    void    upDateClockTimer(INT time);
//    void    hideClock();
    virtual void playNoteAudio(BYTE seatId, BYTE audioType) override;
    virtual void recycleChips(BYTE seat) override;
    virtual void recycleChipsById(LLONG userId) override;
    // 处理玩家金币不足
    Text*	m_TextTip;
    long long    m_i64TipMoney;
    tag_s2c_tip_exit*	tipMsg;
    void updateTip(float dt);
    void tipCallback();
    virtual void userMoneyNotEnouth(tag_s2c_tip_exit* pData) override;
	virtual void playScanner();

    // update handcard
    virtual void updateHandcard(SendOneTurnCardStruct* buff) override;
    virtual void updateHandcard(SendOneTurnCard* buff) override;
    virtual void updateHandcard(BYTE handcardCount, BYTE handcards[PLAY_COUNT][HANDCARD_COUNT]) override;
    virtual void shuffleCards(int hide) override;
    // show max card mark
    virtual void showMaxCardMark(bool visible=false, BYTE buttonSeat=0) override;

	string LongToString(LLONG number);
    // dashboard - update the text of the 3 bet button according to the pot limit
    virtual void setBetBtnText(LLONG baseNote) override;
    virtual void setBetBtnEnabled(bool follow, bool allin, bool note, bool quit) override;
    virtual void setBetBtnEnabled(bool isEnable) override;
    virtual void setBetAndOtherBtnOpen(bool isEnable) override;
    virtual void setMesaVisible(bool visible) override;
    virtual void userPlayerCardTypeEffect(BYTE type,float dt) override;
    virtual void nextTurnLeavl() override;
    virtual void recordDeskInfo(LLONG zhu[]) override;
    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
    // enable/disable start button
    virtual void setStartBtnVisible(bool visible) override;
    // 单个准备显示
    virtual void setAreadySpriteVisible(bool visible,BYTE bDeskNo) override;
    //所有准备精灵显示
    virtual void setAllAreadySpriteVisible(bool visible) override;

    //通知
    virtual void onGameNotify(NN::tag_s2c_notify* msg) override;
	//更新历史输赢和单笔输赢
	virtual void upDataHistoyAndLastMoney(LLONG history, LLONG last) override;

    void getReady();
    void HidTopPlayerOneCard() override;
    
    // 红包出现
    virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
    // 打开红包结果
    virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
private:
    void seatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
    void startClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
	void menuClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
	void toolBarClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
    void setClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
//    void hintClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
//    void onBtnClick(Ref* pSender,Control::EventType type);
    void aControlBtnClick(Ref* pSender, Control::EventType event); //AcontronButton button call back
    void recoveryUserDataCallBack(BYTE seatId,bool isVisible) override;//恢复庄玩家信息
//    void clockHide(bool visibility) override;


    //广播
    void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);

    // chip
    void throwChip(BYTE seat, LLONG money);
    void throwChipBase(BYTE seat, LLONG money);
    Sprite* getChip(LLONG money, LLONG base);

    void dealcards(float delta);
    void dealUserUpResp(float dealta);

//    void refreshMyTimer(float delta);
    bool isSittingRight(BYTE seat);
    void showLoading();
	void nextTurnLeavlDealCallBack(float dt);
	
    bool onTouchBegan(Touch* touch, Event* event) override;

private:
    void loadBackGround();
    void loadAreadySprite();
    void loadCardPosition();
    //侧边栏
    void createToolBarUI();

    vector<vector<Vec2>> getCardPosVector();
private:
    typedef struct TtableUi {
        Widget* outline;

        Button* dealer;			// dealer anchor
        Button* pot;			// pot
        Button* seats[PLAY_COUNT];	// seats button
        Button* start;				// start button

        Sprite*	topTotalBg;
        Sprite*	bottomTotalBg;
        Sprite*	fapaiqi;
        Sprite*	betArea;

        Text* limit;									//历史成绩
        Text* base;								//单次输赢
        Text*		topTotal;
        Text*		bottomTotal;
        Text*		totalMoney;					//单局玩家总下注
    } TtableUi;

    typedef struct TdashboardUi {
        Button* bet[3];		// dashboard note button
        Button* call;			// dashboard follow button
        Button* allIn;			// dashboard show button
        Button* fold;			// dashboard give up button
        AUIButton* start;				// start button
    } TdashboardUi;

private:
    enum {
		em_toolbarBtn = 1000,
		em_toolbarSetBtn,
		em_toolbarHelpBtn,
		em_toolbarLeaveBtn,
		em_toolbarNextOutBtn,
		em_toolbarNextOutLight
    };

    int													m_iPlayerCount;
    bool												m_bNextOut;

    // ui
    MSG_GP_R_LogonResult					_loginResult;

    TimerObject*                            m_timerObj;
//    ImageView*                                    m_ClockBg;
//    LabelAtlas*                                    m_ClockTimeLabel;            //时钟label间

    float												m_fClockTime;
//    float                                                m_fTimeCount;

//    Node*                                                m_pClock;
//    cocostudio::timeline::ActionTimeline* m_pClockAnimation;

    TdashboardUi								_dashboardCanvas;
    TtableUi										_tableCanvas;
    Widget*										_tableMenuOutline;
    Layer*											_chipPool;			// pool for chips
    Sprite*											_hint;

    vector<Point>								_vecCardPos;
    vector<Point>								_vecAreadyPos;
	vector<Sprite*>								_vecAreadySprite;
    vector<ImageView*>						_pockerCard;
	vector<vector<PokerCardV88*>>		_vecPockerCards;

	// toolbar
	vector<Point>								_vecToolbarPoints;
	vector<Point>								_vecToolbarPoints2;
	vector<Button*>							_vecMenuButtonKids;
	Size												toolBar_Size;
	bool												toolBar_flag;
	Node*											center;

    GameNotice*									m_Notice;

    // data
    GameTableLogic*							_tableLogic;
    BYTE												_sentStart;			// the start seat of cards sent
    BYTE												_sentCounter;			// number of cards sent
    BYTE												_handcardCounter;		// number of cards visible
	BYTE												_sentTotal;			// number of cards will be sent

//    unsigned int                                    _clockMusicId;            // number of cards will be sent

    Sprite*											_toolbarBg;

    LLONG											_pot;					// pot - Total note
    LLONG											_baseNote;

    std::list<SendOneTurnCard*>			_sendCardQueue;
    bool												_sendingCard;
    bool												_isOpenHelp;

	Layout*											dashboard_panel;

    BYTE m_deskId;
    bool m_autoCreate;

	PokerCardV88*								_cardSuite[TOTAL_CARDS];
	PlayerUIV88*									_players[PLAY_COUNT];

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


#endif
