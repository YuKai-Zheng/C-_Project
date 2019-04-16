#ifndef __BJLGameTableUIV88_h__
#define __BJLGameTableUIV88_h__

#include "HNNetExport.h"
#include "platform/CCCommon.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GUI/CCControlExtension/CCControlButton.h"
#include "BJLGameLogic.h"
#include "BJLMessageHead.h"
#include "BJLGameTableUICallback.h"
#include "BJLResultCardV88.h"
//#include "BJLHistoryDialog.h"
#include "BJLGameTableUIData.h"
//#include "../v88/BRNNClockV88.h"

#include "HNLobby/GameNotice.h"
#include "../../NN/Classes/GameMessage/NNCommonDef.h"

#include <string>
#include "UI/Base/AUIButton.h"
#include "UI/UserHead.h"
#include "UI/TimerObject.h"

using namespace cocostudio;
using namespace ui;
using namespace std;
using namespace extension;

#define CHOUMANUM			 5									//筹码的按钮的个数
#define NUMOFBETAREA		 8									//多少个下注区域

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace BJL {
#define	WIN_SIZE	Director::getInstance()->getWinSize()

class GameTableLogic;

//游戏界面UI管理
struct GameViewV88 {
    Layout*      myLayout;
    Layout*      dashbroad_layout;
    Layout*      gametable_layout;
    ImageView*   table;

    ImageView* dash_bg_imgview;

    //庄家列表(准备删除  不用了)
    ui::ScrollView* scrollView;
    Text* svDealerName[4];

    //列表bg
    ImageView* ImageView_ListBg;

    // 庄家列表
    ListView* ListView_NTList;

    // 庄家列表内容
    ImageView* ImageView_ListItem;

    // 8个下注区域
    Button* Button_BetArea[8];

    // 8个下注提示区域
    ImageView* ImageView_BetWarning[8];

    // 8个赢牌提示区域 闪烁赢的区域
    ImageView* ImageView_BetWin[8];

    // 请下注
    ImageView* ImageView_betHint;

    // 等待时间
    ImageView* ImageView_WaitHint;

    // 7个筹码按钮
    Button* Button_Bet[7];

    // 7个筹码按钮背景
    ImageView* ImageView_BetBg[7];

    //重复下注
    Button* Button_PrevBet;

    //取消重复下注
    Button*	Button_CancelPrevBet;

    // 申请上庄
    Button* Button_applyNT;

    // 申请下庄
    Button* Button_applyNoNT;

    //上庄列表
    Button*	Button_getListNT;

    //无座按钮
	AUIButton* Button_NoSeat;

	//帮助
	AUIButton* Button_help;

    //记录箭头按钮
    ImageView* Image_LeftArrow;
    ImageView* Image_RightArrow;

    //点记录背景
    ImageView*	Image_PointRecordBg;

    // 退出
    Button* Button_Exit;

    // 路子信息
    Button* Button_Records;

    // 8个下注按钮 筹码摆放按钮区，0-7下标分别表示8个区别
    NoteViewInfo pnoteview[8];

    // 总下注筹码，0-7下标分别表示8个区别
    TextAtlas* TextAtlas_TotalBet[8];

    // 玩家下注筹码，0-7下标分别表示8个区别
    TextAtlas* TextAtlas_UserBet[8];

    // 庄家信息视图
    Text* Label_BankNum;
    Text* Label_BankToatal;
    ImageView* ImageView_HeadBg;
    UserHead* ImageView_BankHead;
    Text* Label_BankName;
    Text* Label_BankMoney;

    Text* Label_UserToatal;

    // 计时器背景
    TimerObject* m_timerObj;
//    ImageView* ImageView_TimerBg;
//    LabelAtlas* TextAtlas_TimerText;

//    Node*                                                m_pClock;
//    cocostudio::timeline::ActionTimeline* m_pClockAnimation;
};


typedef struct tagBetAndTypeV88 {
    int whichPeople;	//谁的0:庄家1:自己2:其他人
    int whichJetton;	//哪一个筹码
    int whichType;		//哪一个区域
} BET_TYPEV88;

class GameTableUIV88 : public HNGameUIBase, public GameTableUICallback {
public:

    enum Em_TableBtnTag {
        em_toolbarBtn = 10000,
        em_toolbarSetBtn,
        em_toolbarHelpBtn,
        em_toolbarLeaveBtn,
        em_toolbarNextOutBtn,
		em_toolbarNextOutLight,
        em_recordBackBtn,
        em_recordOutBtn
    };
    // 获取游戏界面
    static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

    // 初始化
    bool init(BYTE bDeskIndex, bool bAutoCreate);

    // 退出
    virtual void onExit() override;

    // 进入
    virtual void onEnter() override;

    // 构造函数
	GameTableUIV88();

    // 析构函数
    virtual ~GameTableUIV88();
private:
    bool initUI();
    void createBetAreaUI();
    //自己面板
    void createMyControlUI();
    //结果记录ui
    void createResultRecordUI();
    //点记录和押注值
    void createPointRecordUI();

    void createToolBarUI();
//    void showToolBar(bool bShow);
    void getAreaBetPixData();

    void loadRes();
    void initTheNewGameData();
private:
    //记录箭头
    void recordBtnClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
private:
    // 下注区域
    void onBetAreaClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

    // 下注筹码
    void onBetClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);
    //重复下注
    void onRepeatBetClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

    // 申请上庄、下庄
    void onApplyNTClickCallback(cocos2d::Ref* pSender,Widget::TouchEventType touchtype);

    //无座玩家
    void onNoSeatClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

	//帮助
	void onHelpBtnCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

    // 下注动画回调
    void betAnimationCallback(cocos2d::Node* pSender);

    // 发牌
    void dispatchCard(float delta);

    //路子回调
    void luziscrollview_scroll(cocos2d::Ref* pobj, ui::ScrollView::EventType type);

//    void onBtnClick(Ref* pSender, Control::EventType event);

    virtual void onTouchEnded(Touch* touch, Event* event) override;
private:
    void stopBetAnimation();
    void hideXiaZhuBTN();
    void showSettlementDialog(S_C_GameResult* pData);
    void showMyBetAnimation(BYTE whichBTN , BYTE whichArea);
    void showOtherBetAnimation(BYTE whichBTN, BYTE whichArea);
    void showBankerBetAnimation(BYTE whichBTN, BYTE whichArea, BYTE whichPeoPle, float delay);
    void resetDealerList(BYTE dealerStation[], SHORT count);

    //广播
    void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);

    //筹码是否在赢的区域
    bool inWinArea(int area);
    //清空筹码
    void cleanBetAnimation();
private:
//		GameHistoryDialog* _Dialog;
    GameTableLogic*    _logic;
    ResultCardV88*        _resultCard;

    //游戏界面UI管理
    GameViewV88*          _gameView;

    //上庄列表模块
    T_DEALER_LIST*     _dealerList;
    BYTE m_DeskIndex;
    bool m_AutoCreate;

public:
    virtual void leaveTheGame() override;

    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;

    //第一个上庄者 游戏直接进入下注
    virtual void OnHandleBegin(S_C_GameBegin * pGameBeginData);

    //结算
    virtual void showSettlement(S_C_GameResult* pGameResult);

    // 显示提示信息
    virtual void IShowNoticeMessage(const std::string& msg) override;

    // 申请上、下庄按钮
    virtual void IApplyNT(bool isNT) override;

    //更新记录
    virtual void refreshRecord(int iIdex = -1) override;

    //更新point记录
    virtual void refreshPoint() override;

    //更新押注值
    virtual void refreshCanBetNum(long long i64CanBetMoney[]) override;

    //刷新累计值
    virtual void refreshBankerTotalWinNum(long long i64BankerWin) override;

    //
    virtual void refreshMyTotalWinNum(long long i64UserWin) override;

    // 更新上庄列表
    virtual void IUpdateNTList() override;

	// 显示庄家信息
	virtual void IUpdateNTInfo(int userId, const std::string name, LLONG money, int logoID, int bankCount) override;

    // 显示自己下注额
    virtual void IDisplayMyBetMoney(int noteArea, LLONG money) override;

	// 显示其他玩家 总下注额
	virtual void IDisplayOtherBetMoney(int noteArea, LLONG money) override;

    //保存下注过程
    virtual void saveBetProcess() override;

    // 更新玩家金币
    virtual void IUpdateUserMoney(LLONG money) override;

    // 显示玩家下注动画
    virtual void IPlayNoteAnimation(BYTE noteType, int noteArea, BYTE bDeskStation) override;

    // 设置筹码按钮是否可用
    virtual void ISetBetEnable(bool enabled[], int count) override;

    // 启动倒计时
    virtual void IStartTimer(int second) override;

    // 在指定区域下注筹码
    virtual void IAreaBet(int area, LLONG betMoney) override;

    // 初始化牌
    virtual void IInitCard(BYTE cards[][3], int playerCount, int NTCount, int playerCardType[], int NTCardType[]) override;

    // 显示牌
    virtual void IShowCards(bool animate) override;

    // 发牌
    virtual void ISendCard(bool animate) override;

    // 提示下注
    virtual void IShowBetHint(bool visible, bool resume) override;

    // 提示等待时间
    virtual void IShowWaitHint(bool visible) override;

    // 游戏开始
    virtual void IGameStart() override;

    // 显示中间区域
    virtual void IShowWinArea(bool visible) override;

    // 开奖面板
    virtual void IShowOpenBoard(bool visible) override;

    //倒计时图移动新位置
    virtual void SetImageTimeMoveNewPostion()override;

    //倒计时图移动到老位置
    virtual void SetImageTimeMoveOldPostion()override;

    void updateTip(float dt);
    void hideClock();
    virtual void IShowTipExit(long long i64Money) override;

    //通知
    virtual void onGameNotify(NN::tag_s2c_notify* msg) override;

	//游戏阶段提醒
	virtual void onGameStateTip(BYTE gameState){};
    
    // 红包出现
    virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
    // 打开红包结果
    virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
private:
    //动态创建的一些精灵
    Text* _ownName;
    Text* _ownMoney;
    LLONG _currentMaxZhu[NUMOFBETAREA + 1];         // 每个区域还能下多少注
	LLONG _currentMyZhu[NUMOFBETAREA];				// 玩家下注情况
    LLONG _currentDealerLimit;
    Vector<Sprite*> vConinSprite;
private:
    // 计时器跑秒时间
    int _remainTime;

    // 选中的下注按钮
    int _betIndex;

//    void timerUpdate(float delat);

    void animationUpdate(float dt);
    
    //big win animation
    void playBigWinAnimation(int gameID, int openLogo, int winRate, LLONG money);
    
private:
    ImageView*					_tableBg;
    Sprite*						_toolbarBg;
    GameNotice*					m_Notice;

    Vector<ImageView*>			_vRecordYes;
    Vector<Label*>				_vRecordNum;
    Vector<Label*>				_vPointNum;
    Vector<Label*>				_vCanBetNum;	//可下注值

    Text* m_TextTip;
    long long m_i64TipMoney;

    bool m_bNextOut;
    bool m_bAutoBet;
    vector<int> m_vPrevBetArea1;
    vector<long long> m_vPrevBetMoney1;
    vector<int> m_vPrevBetArea2;
    vector<long long> m_vPrevBetMoney2;
    vector<unsigned char*> m_vPixData;
    vector<Image*> m_vImage;
    int m_iRecordIndex;
//    int                                    _countForOneSec;
    int _t;
//    int _allTime;
//    unsigned int                                    _clockMusicId;            // number of cards will be sent

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

#endif // __BJLGameTableUIV88_h__
