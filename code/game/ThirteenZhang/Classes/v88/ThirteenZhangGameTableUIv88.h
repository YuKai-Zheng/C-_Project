#ifndef __ThirteenZhang_Game_Table_UI_v88_H__
#define __ThirteenZhang_Game_Table_UI_v88_H__

#include <array>
#include <memory>
#include "cocos2d.h"
//#include "HNNetExport.h"
#include "ThirteenZhangGameTableUICallback.h"
#include "ThirteenZhangPlayerWrapperv88.h"
#include "ThirteenZhangHandCardWrapperv88.h"
//#include "ThirteenZhangTimerWrapperv88.h"
#include "extensions/cocos-ext.h"
#include "UI/Base/AUIButton.h"
#include "HNLobby/GamePrompt.h"
#include "../../NN/Classes/GameMessage/NNCommonDef.h"
#include "HNLobby/GameNotice.h"
#include "TZhangCardControlv88.h"
#include "UI/TimerObject.h"

USING_NS_CC;
using namespace cocos2d::extension;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace ThirteenZhang {
//class ControllerLayer;
class GameTableLogic;

class GameTableUIV88 : public HN::HNGameUIBase, public IGameTableUICallback {
private:
    GameTableLogic* _logic;
	BYTE			_bDeskIndex;
	bool			_bAutoCreate;
    GamePromptLayer* _specialPrompt;
    bool            _canSendSpecialMsg;//默认为false，表明现在先不发送，此处的逻辑为了让玩家特殊牌信息在服务端通知开牌以后再执行

    PlayerWrapperV88 _players[PLAY_COUNT];
    HandCardWrapperv88 _handCard[PLAY_COUNT];
//    TimerWrapperV88 _timer[PLAY_COUNT];
    bool            _card_done_state[PLAY_COUNT];   //玩家物理位置

    TimerObject* _timer_self;
    TimerObject* _timer_whole;
    Sprite* _pair_state[PLAY_COUNT];//保留自己游戏状态逻辑，但是自己的状态不需要显示，因为知道自己跌状态，没必要做多余的工作展示出来--by HBC

    // 		std::array<PlayerWrapper, PLAY_COUNT> _players;
    // 		std::array<HandCardWrapper, PLAY_COUNT> _handCard;
    // 		std::array<TimerWrapper, PLAY_COUNT> _timer;

    ui::Button* _btnReady;
    ui::Button* _btnChangeTable;
    //AUIButton* _btnCall;
    //AUIButton* _btnUncall;

    Sprite* _toolbarBg;
	//桌子
	Sprite*						_tableBg;

    //倒水提示
    Sprite* _dun_first_b;
    Sprite* _dun_first_f;
    Sprite* _dun_middle_b;
    Sprite* _dun_middle_f;
    Sprite* _dun_last_b;
    Sprite* _dun_last_f;

    //低注
    Text* _dizhuValue;



	enum Em_TableBtnTag{
		em_toolbarBtn = 10000,
		em_toolbarChangeTableBtn,
		em_toolbarSetBtn,
		em_toolbarHelpBtn,
		em_toolbarStandupBtn,
		em_toolbarLeaveBtn,
		em_toolbarNextOutBtn,
		em_toolbarNextOutLight

	};



private:
    //打枪动画特效
    Sprite* _m_animation_OneByOne_dankong;
    Sprite* _m_animation_OneByOne_sheji;
	cocostudio::timeline::ActionTimeline* qiangAction;
	cocostudio::timeline::ActionTimeline* danhenAction;
	cocostudio::timeline::ActionTimeline* ziAction;
	Node* qiangZiNode;
	Node* danhenNode;
	Node* qiangNode;


	
//        Sprite* _m_animation_All_dankong;
//        Sprite* _m_animation_All_saoshe;

    //VS特效
    Sprite* v_sprite;
    Sprite* s_sprite;
    Sprite* particle_sprite;

private:
    std::vector<BYTE> _myCards;
    bool _isPlaying;
    bool _useSpecialShape;  //是否使用特殊牌型进行比牌
    bool _isWaitingSpecialShape;    //等待确认是否使用特殊牌
    bool _hasReadyClick;
    bool _autoLeaveNext;    //本局托管，下局自动退出


	// toolbar
	vector<Point>								_vecToolbarPoints;
	vector<Point>								_vecToolbarPoints2;
	vector<Button*>							_vecMenuButtonKids;
	Size												toolBar_Size;
	bool												toolBar_flag;
	Node*											center;


    //ui::ImageView* _gameStateLabel;
    //int _zhuangSeatNO;

public:
    static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

protected:
    GameTableUIV88(BYTE bDeskIndex, bool bAutoCreate);
    virtual ~GameTableUIV88();

public:
    virtual bool init() override;
    virtual void onEnter()override;
    virtual void onExitTransitionDidStart()override;
    virtual void onExit() override;
    virtual bool onTouchBegan(Touch* touch, Event* event) override;
    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
    virtual void forceLeaveTheGame() override;
    
    // 红包出现
    virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
    // 打开红包结果
    virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
public:
    void leaveTheGame();
    // UI方法
protected:
    //void onChatClick(cocos2d::Ref *pSender, Control::EventType event);
//    void onSetClick(cocos2d::Ref *pSender, Control::EventType event);
//    void onExitClick(cocos2d::Ref *pSender, Control::EventType event);
//    void onHelpClick(cocos2d::Ref *pSender, Control::EventType event);
//    void onNextGameClick(cocos2d::Ref *pSender, Control::EventType event);
	void showBaozheEffect(float dt);

protected:
//    void onOutCardClick(ControllerLayer* controller);
    void onHeadFrameClick(PlayerWrapperV88* player);
//    void onManualRequestCardArray(ControllerLayer* controller);

protected:
//    void onTimerEndEvent(TimerWrapperV88* timer);
//    void onTimerWholeEndEvent(TimerWrapperV88* timerWhole);
    void onSwitchTableClick(cocos2d::Ref *pSender);
    void onReadyClick(cocos2d::Ref *pSender);


private:
    void onForceStandDesk();
	void playBaozhaEffect(INT index, INT fromId);

    //protected:
    //    void onCallClick(cocos2d::Ref *pSender);

protected:
    void onSettlementTimer();

    // 逻辑接口
protected:
    virtual void onAddPlayer(BYTE lSeatNo, INT userID, bool self) override;
    virtual void onRemovePlayer(BYTE lSeatNo, LLONG userID, bool self) override;

protected:
    virtual void onUserChat(INT sendID, INT targetID, const std::string& msg) override;

protected:
    //空闲阶段
    virtual void onGSFree(const bool userReady[PLAY_COUNT]) override;
    ////叫庄阶段
    //virtual void onGSRobNt(BYTE currRobUser) override;
    //发牌阶段
    virtual void onGSSendCard(const std::vector<BYTE>& cards) override;
    //开牌阶段
    virtual void onGSOpenCard(const std::vector<BYTE>& cards, const BYTE heapCard[3][5]) override;
    //比牌阶段
    virtual void onGSCompareCard(const BYTE heapCard[PLAY_COUNT][3][5]) override;

protected:
    // 玩家准备
    virtual void onGameAgree(BYTE lSeatNo, bool self) override;
    //// 通知抢庄
    //virtual void onGameNoticeRobNt(BYTE lSeatNo, bool self) override;
    //// 通知抢庄结果
    //virtual void onGameRobNtResult(BYTE lSeatNo, bool robResult, bool self) override;
    //// 确定庄家位置
    //virtual void onGameMakeSureNt(BYTE lSeatNo, bool self) override;
    // 发牌
//        virtual void onGameSendCard(const std::vector<BYTE>& cards) override;
    void onGameSendCard();
    //载入自己手牌
    void onGameLoadControlCard(const std::vector<BYTE> cards);
    // 提示出现特殊牌型
    virtual void onGameHasSpecialCard(const std::vector<int>& specialShapeArray, const std::vector<BYTE>& cards) override;
//    //手动一键摊牌
//    virtual void onGameManualOpenCard(void* manualResult) override;
    // 通知开牌
    virtual void onGameNoticeOpenCard() override;
    // 开牌结果
    virtual void onGameOpenCardResult(BYTE lSeatNo) override;
    // 比牌
    virtual void onGameCompareCard(const BYTE heapCard[PLAY_COUNT][3][5]) override;
    // 开火
    virtual void onGameOpenFire(S_C_OpenFire* scOpenFire) override;
    //  特殊牌开牌
    virtual void onGameOpenSpecialShape(S_C_OpenSpecialShape* scSpecialShape) override;
    // 游戏结算
    virtual void onGameResult(const S_C_GameResult* pObject) override;
    void updateTip(float dt);
    //金币不足强退
    virtual void IShowTipExit(long long i64Money) override;
    // 更新游戏底注
    virtual void onGameDizhuValue(const int value) override;
	//是否播放开始动画
	virtual void onIsCanPlayBeginAni(bool isCan) override;
    //五花牛全服公告
    virtual void onGameNotifyWuHuaNiu(NN::tag_s2c_notify* msg) override;

private:
    //打枪动画特效
    void scheduleOpenFireOBO_CB(float delta);
    void animationOpenFireOneByOne(int fromX, int toY);//赢某个玩家
    void animationOpenFireToAll();//全垒打
	void onShowSpecialEffect(int speciaCardIndex);//特殊牌的效果

    //公告
    void showNotice(const std::string& message, int nCount, int nType);

    int _aniIndex;
	int baozhaoPosInde;
    int _fireAllAniIdx;//全垒打发起玩家物理位置索引
    bool _openFire[4][4];
    int _extraWater[PLAY_COUNT];
    int _extraWaterAll[PLAY_COUNT];

    Text* m_TextTip;
    long long m_i64TipMoney;

    GameNotice* m_notice;

    // 加载UI
protected:
    void doLoad();
    void initToolBar();

private:
//    ControllerLayer* openControlLayer();

    void restoreChildUI();
    
private:
    TZhangCardControlV88* openCardControl(const std::vector<BYTE> cards);

private:
    void controlCompareWithSpecialShapeOrNot(int specialShape, bool useShape);
    void onCompareWithSpecialShape();
    void onCompareWithCommonShape();

private:
    void doCompareCard(const BYTE heapCard[PLAY_COUNT][3][5]);
    void compareCard(const BYTE heapCard[PLAY_COUNT][3][5], float litghtCardTime);
    Sequence* createSeqForCompareCard(bool playing, int playIndex, int cardIndex);
    void doShowScore(int index);
    void doLightCard(int index);
    void doShowScoreOneByOne(int seatNo, int index);
    void doLightCardOneByOne(int seatNo, int index);

private:
//    void middleDaoshuiTip(ControllerLayer* controller);
//    void firstDaoshuiTip(ControllerLayer* controller);
//    void allDaoshuiTip(ControllerLayer* controller);
    void animationMiddleDaoshui();
    void animationFirstDaoshui();
    void animationAllDaoshui();
    void controlMiddleDaoshuiButton(bool show);
    void controlFirstDaoshuiButton(bool show);
    void controlAllDaoshuiButton(bool show);
    void restoreDaoshuiButton();
    void animationVSShow();

private:
    void switchCardStateSprite(int index, bool isDone);
    void restoreCardStateSprite(int index);
	void toolBarClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
	// 控制左上角控制按钮 点击状态
	void toolBarFlag(float dt);

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
