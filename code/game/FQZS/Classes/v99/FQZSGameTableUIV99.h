#ifndef __FQZS_GameTable_NNTableUIV99_h__
#define __FQZS_GameTable_NNTableUIV99_h__

#include "HNNetExport.h"

#include "FQZSCommonDef.h"

#include "FQZSGameTableUICallback.h"
#include "HNLobby/GamePrompt.h"
//#include "../v99/BRNNClockV99.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "UI/TimerObject.h"
#include "HNLobby/GameNotice.h"
using namespace cocostudio;
using namespace ui;
using namespace cocos2d;
using namespace cocos2d::extension;


namespace FQZS {

class GameTableLogic;
class PokerCard;
class CPlayerHead;

class GameTableUIV99 : public HNGameUIBase, public GameTableUICallback {
	//////////////////////////////////////////////////////////////////////////
	//控件结构体
private:

	//下注按钮操作区域
	struct Bet_Area {
		Node*	areaNode;				//节点
		Node*	clearResetNode;
		vector<Sprite*> btnLights;		//十一个按钮选中后的光效
		vector<Button*> betButtons;		//11个下注按钮
		vector<Text*>	myCoinText;		//11个自己下注金额展示面板
		vector<Text*>	allCoinText;	//11个所有玩家下注金额展示面板
		Button*			clearBtn;		//清空下注按钮
		Button*			resetBtn;		//续注按钮
		Sprite*			myGoldSprite;	//
		Sprite*			myWinSprite;	//
		Text*			myWin;			//自己进入游戏后游戏输赢金额
		Text*			myGold;			//自己身上携带的金币
		Bet_Area() {
			areaNode = nullptr;
			clearResetNode = nullptr;
			clearBtn = nullptr;
			resetBtn = nullptr;
			myGoldSprite = nullptr;
			myWinSprite = nullptr;
			myWin = nullptr;
			myGold = nullptr;
			btnLights.clear();
			betButtons.clear();
			myCoinText.clear();
			allCoinText.clear();
		}
	} _betArea;

	//结果转盘
	struct Result_Loop {
		Node* loopNode;
		vector<Sprite*> animals;
		Result_Loop() {
			loopNode = nullptr;
			animals.clear();
		}
	} _resultLoop;

	//结果
	struct Result_Label {
		Node* labelNode;
		TextBMFont* font;
		Result_Label() {
			labelNode = nullptr;
			font = nullptr;
		}
	} _resultLabel;

	//下注操作控件
	struct Bet_Buttons {
		Node* node;
		vector<Button*> buttons;
		vector<Sprite*> btnLights;
		Bet_Buttons() {
			node = nullptr;
			buttons.clear();
			btnLights.clear();
		}
	} _betButton;

	//彩金池
	struct Gold_Pool {
		Node* poolNode;
		//Sprite* bg;
		Text* poolMoney;
		long long i64Money;
		Gold_Pool() {
			poolNode = nullptr;
			//bg = nullptr;
			poolMoney = nullptr;
			i64Money = 0;
		}
	} _goldPool;

	//开牌记录
	struct Open_Record {
		Node* node;
		Button* arrowLeft;
		Button* arrowRight;
		vector<Sprite*> animals;
		Open_Record() {
			node = nullptr;
			arrowLeft = nullptr;
			arrowRight = nullptr;
			animals.clear();
		}
	} _openRecord;



private:
	float						_fScaleX;
	float						_fScaleY;
	Size						_tableSize;

	//桌子
	Sprite*						_tableBg;
	Sprite*						_tableBgRight;

	TimerObject*             m_clockObj;

	//Sprite*						_toolbarBg;
	//     ImageView*					m_ClockBg;
	//     LabelAtlas*					m_ClockTimeLabel;			//时钟label
		//BRNN::ClockV99*					m_Clock;

		//ImageView*					m_StageImage;				//阶段图标
	Sprite*						m_AnimateImage;				//动画图片


	GameNotice*					m_Notice;                   //广播

	BYTE						m_bPrepareTime;				//准备时间
	BYTE						m_bCatchBankerTime;			//抢庄时间
	BYTE						m_bSetMoneyTime;			//庄家设置红包金额时间
	BYTE						m_bSetBombTime;				//庄家设置红包雷数字时间
	BYTE						m_bOpenLETime;				//闲家开红包时间
	BYTE						m_bShowResultTime;			//显示游戏结果时间

	float						m_fClockTime;
	float						m_fTimeCount;
	BYTE						m_bTimerChairID;			//定时器椅子号
	int							m_iTimerID;					//定时器id
	bool						m_bDoTimeOutCallback;		//是否做超时处理
	int							m_iSendRoundNum;			//单张发轮数,为5轮
	int							m_iSendViewID;				//发方位

	int							m_GameStatus;
	bool						m_bGameEnd;					//记录一局结束
	bool						m_bNextOut;					//下局离开
	//vector<BYTE>				m_vPlayerPrepare;
	BYTE						m_bCurUser;					//当前操作者
	BYTE						m_bNextUser;				//下次操作者
	//BYTE						m_bBankerID;				//庄家

	bool m_canPrepareForNew;

	vector<Vec2>					m_betAreaPosition;			//飞禽、走兽、鲨鱼下载控件位置
	vector<Size>					m_betAreaKindSize;			//种类、鲨鱼下注区域控件大小

	vector<Vec2>					m_loopAnimalPosition;		//28种动物的位置
	vector<Sprite*>					m_loopAnimationSprite;		//3个动画起始点的精灵
	int								m_loopAnimationRemainTime;	//精灵需要移动的数量
	int								m_loopAnimationMaxTime;		//精灵需要移动的最大数量
	int								m_loopAnimationEndIndex;	//精灵移动的终点

	//OpenRecord
	vector<BYTE>					m_openRecordAnimal;			//服务端开启过的动物类型，目前只记录十个

	//OpenResult
	vector<BYTE>					m_openResultAnimalKind;		//当局服务端开奖的动物类型
	vector<BYTE>					m_openResultAnimalIndex;	//开动物Button的位置

	//进入游戏后总输赢
	long long						m_allMyWinGold;				//进入游戏后总的输赢
	BYTE							m_betButtonTag;				//下注阶段选中的下注按钮索引值

	//游戏阶段
	Node*							m_gameStateNode;			//游戏阶段显示node
	Sprite*							m_gameStateSprite;			//游戏阶段显示label

	//动画
	Node*							m_animalAnimationNode;		//播放动物动画的统一节点

private:
	//le_s2c_envelope_info _open_envelope_info;
	Text*	m_TextTip;
	long long    m_i64TipMoney;
	le_s2c_tip_exit	tipMsg;

	typedef std::function<void()> ShowBankerCallback;
	ShowBankerCallback showBankerCallback;

public:
	GameTableLogic*			_GameTableLogic;

	//////////////////////////////////////////////////////////////////////////
public:
	GameTableUIV99();
	~GameTableUIV99();
	static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

	bool init(BYTE bDeskIndex, bool bAutoCreate);

	virtual bool onTouchBegan(Touch* touch, Event* event) override;
	virtual void onEnter() override;
	virtual void onExit() override;

	virtual void onExitTransitionDidStart()override;

	// 红包出现
	virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
	// 打开红包结果
	virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
    //数据
public:
    int				m_iFreeTime;					//空闲时间
    int				m_iBetTime;						//下注时间
    int				m_iPrevHasNote;					//上局是否有下注，若上局没有下注，则此局续投无效
    int				m_iCircleTime;
    int				m_iShowResultTime;				//显示开奖时间

    long long		m_i64SingleNoteLimit;			//单人下注限额
    int				m_iMultiplying[Rate_Count];		//倍率 11区域+金鲨、通赔、通吃
    int				m_iHistory[10];					//开奖记录
    long long		m_i64OwnNotes[GAME_NOTE_COUNT]; //我的各区域押注
    long long		m_i64AreaNotes[GAME_NOTE_COUNT]; //我的各区域押注


    int				m_iBetType;						//下注类型
    int				m_iBetMoneyType;				//下注钱类型
	GamePromptLayer* _layer;
public:
    //侧边栏
    void createToolBarUI();
    //void createPlayerHeadUI(int iPlayerCount);
    //vector<Vec2> getPlayerHeadPos();
    //void resetPlayerHeadUI();
    void createClock();
    void createReadyUI();

    void createBetArea();
    void initBetButtonsPosition();
    void createBetButtons();
    void createPool();
    void createResult();
    void createResultLoop();
    void createState();
    void createGameState();	//显示游戏阶段

    //void createBetValue(BYTE bViewChairID);
    //vector<Vec2> getPokerPointPos(int iPlayerCount);

public:

    virtual void leaveTheGame() override;
    //进入或者短线重连恢复游戏桌子场景
    virtual void resumeTheGameFree(GameStation_Base* pcene, BYTE gameStation) override;
    virtual void resumeTheGameBet(GameStation_Bet* pcene, BYTE gameStation) override;
    virtual void resumeTheGameKaiJiang(GameStation_Kaijiang* pcene, BYTE gameStation) override;
    virtual void resumeTheGameOver(GameStation_Base* pcene, BYTE gameStation) override;

	void showToolBar(bool bShow);
    void onBtnClick(Ref* pSender,Control::EventType event);
private:
    //时钟方法
    void updateClock(float dt);
    void setTimer(BYTE bChairID, int iTimerID, float fTime);
    void killTimer();
    void timeOutCallback(BYTE bChairID, int iTimerID);		//超时处理
    void updateUIForResume();
    
    void updatePoolMoney(long long poolMoney);


    //Vec2 getSettlementHeadMiddlePosition(BYTE bViewID);
    //      Vec2 getStateTipPos(BYTE bViewID);
    //Vec2 getPlayerHeadMiddlePos(BYTE bViewID);
    //Vec2 getSettlementHeadMiddleAnchor(BYTE bViewID);
    //Vec2 getStateTipAnchor(BYTE bViewID);
    //Vec2 getTimerPosition(bool isMiddle = true);

    //void showTimeTip(int iTimerID, bool bShow = true);

    void updateTip(float dt);

    void setBtnTouch(Button* btn, bool bTouch);
//		void showOpenBtn(float dt);


    void tipCallback();
    void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);
    //void resetUI();

private:
//		void setCoinMove();
//		void showCoinMove(int chairID, long long goldNum);
//		void showGoldRain();
//		void showCowRun();

    //int m_iBankerViewID;
    //void showBankerAnimation(Vec2 pos);
    //void showOpenLE(Button* btn);
    //void showHaveOpenLE();



private:
    //红包处理相关逻辑
    //void autoUpdateSetBombUI(Slider* sliderNode, int bomb);
    //void showBombOperationAnimation(bool isShow = true);
    //void updateBombValue();
    //void showSendLEButton();
    //void hideSendLEButton();
    void updateMoneyValue(long long value);
    void autoUpdateSetMoneyUI(long long money);
    void showOpenListForBanker();
    void showBankerListUI();



    void initData();
    void resetNewGame();
    void resetUINewGame();
    void resetData();

private:
    //按钮回调
    void betAreaCallBack(Ref* pSender, Widget::TouchEventType type);		//下注区域按钮回调
    void betButtonsCallBack(Ref* pSender, Widget::TouchEventType type);		//下注金额按钮回调
    void clearRebetCallBack(Ref* pSender, Widget::TouchEventType type);		//取消、续注按钮回调
    void leftRightArrowCallBack(Ref* pSender, Widget::TouchEventType type);	//左右按钮回调
    //void sendLEButtonCallBack(Ref* pSender, Widget::TouchEventType type);       //庄家红包设置完发红包按钮回调
    //void sendOpenLEButtonCallBack(Ref* pSender, Widget::TouchEventType type);   //闲家收到红包后拆红包按钮回调
    //      void prepareClickCallback(Ref* pSender, Widget::TouchEventType type);
    //      void catchBankerClickCallback(Ref* pSender, Widget::TouchEventType type);
    //      void betMoneyClickCallback(Ref* pSender, Widget::TouchEventType type);
    //void sliderButtonCallBack(Ref* pSender, ui::Slider::EventType type);

private:
    //void controlBetMoneyArea(BYTE areaKind, bool isShow = false);
    void controlBetValueButtonsEnable(bool canTouch = true);
    void controlBetButtonEnable(bool canTouch = true);
    void hideBetButtonSelectState();//隐藏金额按钮选择效果
    void controlClearButtonEnable(bool canTouch = true);
    void controlRebetButtonEnable(bool canTouch = true);
    void hideBetAreaSelectState();
    void resetTextValue();
    //void autoHideBetButtons();

private:
    void showRunLoopAnimation(BYTE endIndex, BYTE loopTime, float delayTime = 2.0f);
    void updateLoopSpritePosition(float dt);
    void updateRandomBlinkLightFree(float dt);
    void showResultAnimalAnimation(BYTE animalKind, float showTime = 4.0f);
    void updateResultAnimation(float dt);
    void updateBlinkOpenAnimalLight();
    void updateGameStateLabelContent();
    void showGameStateLabelAnimation(float dt);

    void stopBlinkAnimalLight();


    void showOpenAnimalAnimation(BYTE index);
    void playAnimalsAnimation(std::string res, std::string tagName);
    void stopAnimalsAnimation();
    void tongpeiTongshaAnimation(bool flag);
    void showResultAnimation(long long changeMoney);
    void updateStateImage(int iHistory[]);

    void animationMoveOpenItemToStateLabel(int endIdx);
    void animationTipBetBegin();
    void animationTipStartCircle();
    void animationTipEndIndexAnimal();		//所开动物光环定时闪烁

private:
    void playFreeMusic();
    void playBetMusic();
    void playStopBetMusic();
    void playBetMoneyEffect();
    void playOpenMusic();
    void playOpenEffectAudio(BYTE kind);
    void playLoopEffectAudio(BYTE type);

private:
    BYTE getBetButtonIndexByKindID(BYTE kindID);
    BYTE getAnimalKindByLoopIndex(BYTE endIndex);

public:
    void updateMyWinLabel(long long money);
    void updateGoldLabel(long long money);

private:
    //玩家坐下
    virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
    //玩家站起
    virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
    //添加用户
    virtual void addUser(BYTE seatNo, bool bMe) override;
    //游戏开始
    virtual void onGameBegin(GameBeginStruct* msg) override;
    //通知下注
    virtual void onGameBeginBet(GameNote* msg) override;
    //下注结果
    virtual void onGameBetResult(GameXiaZhuResponse* msg) override;
    //开始转圈开奖
    virtual void onGameStartCircle(GamePlay* msg) override;
    //显示开奖结果
    virtual void onGameShowResult(GamePlayResult* msg) override;
    //计算结果
    virtual void onGameCalcuResult(GameEndMsg* msg) override;
    //提醒退出
    virtual void onGameTipExit(le_s2c_tip_exit* msg) override;
    //通知
    virtual void onGameNotify(tag_s2c_notify* msg) override;
    ////
    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;


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

#endif // __BRNN_GameTable_BRNNTableUI_h__
