#ifndef FruitGameTableUIV88_h
#define FruitGameTableUIV88_h

#include <string>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"
#include "FruitGameTableUICallback.h"
#include "UI/Base/AUIButton.h"
//#include "../../TTNN/Classes/v88/BRNNClockV88.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;
using namespace ui;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace Fruit {

	class GameTableLogic;
	class FruitLoopSpriteCellV88;
	class FruitPlayerHeadV88;
	class FruitGameStateV88;
	//class FruitSettlementV88;
	class FruitNoteLabelCellV88;
	class FruitBetMoneyCellV88;

	class GameTableUIV88 : public HNGameUIBase, public GameTableUICallback {

		//////////////////////////////////////////////////////////////////////////
		//控件结构体
	private:
		//下注区域
		struct Bet_Area {
			Node* betNode;                      //节点
			Node* moneyNode;                    //下注筹码节点
			Sprite* tableBg;                    //桌子背景
			std::vector<Button*> betButtons;    //8个下注按钮
			std::vector<Vec2> betButtonsVec;    //8个下注次数的位置
			std::vector<FruitBetMoneyCellV88*> betMoneySp;   //5个下注金额
			std::vector<Sprite*> betMoneyLight; //5个下注金额按钮选中状态
			//        std::vector<Text*> myCoinText;      //每个区域自己的下注额
			//        std::vector<Text*> allCoinText;     //每个区域的下注总额
			AUIButton* reBetBtn;                //续投按钮，按照上局的下注数据进行下注
			AUIButton* clearBtn;                //清空当局下注数据按钮
			Bet_Area() {
				betNode = nullptr;
				moneyNode = nullptr;
				tableBg = nullptr;
				betButtons.clear();
				betMoneySp.clear();
				betMoneyLight.clear();
				//            myCoinText.clear();
				//            allCoinText.clear();
				reBetBtn = nullptr;
				clearBtn = nullptr;
			}
		} _betArea;

		struct Car_Layer {
			ImageView* cardImage;
			Node* carNode;
			cocostudio::timeline::ActionTimeline* carAction;
			Car_Layer() {
				carNode = nullptr;
				cardImage = nullptr;
				carAction = nullptr;
			}
		} _car;

        //结果展示区域
		struct Loop_Area {
			Node* loopNode;
			std::vector<Vec2> loopCellPositions;    //展示区域基本元素的位置
			std::vector<FruitLoopSpriteCellV88*> loopSprites;   //结果展示区域基本单位
			Loop_Area() {
				loopNode = nullptr;
				loopCellPositions.clear();
				loopSprites.clear();
			}
		} _loopArea;

		//开牌记录
		struct Open_Record {
			Node* node;
			vector<Vec2> openLogosPositions;
			vector<FruitLoopSpriteCellV88*> openLogos;
			Open_Record() {
				node = nullptr;
				openLogosPositions.clear();
				openLogos.clear();
			}
		} _openRecord;

		//单局下注相关数据
		struct Bet_Data {
			bool isBanker;                                      //自己是否做庄
			long long betMoney;                                 //个人下注总额
			long long betMoneyEachArea[BetA_Count];     //个人每个下注区域下注金额
			//        bool betMoneyEnable[Bet_Coin_Count];                //每个下注筹码有效性
			Bet_Data() {
				isBanker = false;
				betMoney = 0;
				memset(betMoneyEachArea, 0, sizeof(betMoneyEachArea));
				//            memset(betMoneyEnable, 1, sizeof(betMoneyEnable));
			}
		} _betData;

		//单次下注需要的数据
		struct Bet_Single_Operate {
			int moneyIndex;
			int betAreaIndex;
			Bet_Single_Operate() {
				moneyIndex = -1;
				betAreaIndex = -1;
			}
		} _BetSingleOperate;

		//////////////////////////////////////////////////////////////////////////
		//UI
	private:
		//BBPlayerHeadV88* _playerHead;
		// BBGameStateV88* _gameState;
		//FruitSettlementV88* _gameSettlement;

		//////////////////////////////////////////////////////////////////////////
		//游戏数据
		struct GameData {
			LLONG myMoney;
			std::vector<Vec2> resultLabelPositions;     //32个结果的位置
			GameData() {
				myMoney = 0;
				resultLabelPositions.clear();
			}
		} _gameData;



	private:
		//资源
		//    Sprite*						_tableBg;
		Sprite*						_toolbarBg;

		//数据
		float						_fScaleX;
		float						_fScaleY;
		int							m_GameStatus;
		//    bool						m_bNextOut;					//下局离开

		unsigned int    m_audioEffectID;



		Button*                     smallBtn;
		Button*                     bigBtn;
		Button*                     leftBtn;
		Button*                     rightBtn;
		Button*                     startBtn;
//        BRNN::ClockV88*             m_clockObj;
//        Vec2                        m_clockPos;

		Text*                 jackpotText;
		Text*                 guessNumText;
		Text*                 myLastMoneyText;
		Text*                 currentGetText;

		Text*					gameStateText;
		ImageView*              costTypeImg;

		//
		Vec2 jackpotSpPoint;                    //奖池
		Vec2 jackpotSpPointLb;                    //奖池
		Vec2 guessResultSpPonint;                    //猜大小结果
		Vec2 cureentMoneySpPoint;                    //本轮中奖
		Vec2 lastMoneySpPoint;                    //当前余额
		//
		GameNotice*					m_Notice;                   //广播

		Node*                      lighBarDaijiNode;
		cocostudio::timeline::ActionTimeline*            lighBarDaijiAct;//跑马灯

		Node*                      lighBarKaijiangNode;
		cocostudio::timeline::ActionTimeline*            lighBarKaijiangAct;//跑马灯


		//Node*                      jackpotDaijiNode;
		//cocostudio::timeline::ActionTimeline*            jackpotDaijiAct;//奖池

		//Node*                      jackpotKaijiangNode;
		//cocostudio::timeline::ActionTimeline*            jackpotKaijiangAct;//奖池


		Node*                      guessDaijiNode;
		cocostudio::timeline::ActionTimeline*            guessDaijiAct;//猜大小

		Node*                      guessKaijiangNode;
		cocostudio::timeline::ActionTimeline*            guessKaijiangAct;//猜大小

		Node*                      betSizeNode;
		cocostudio::timeline::ActionTimeline*            betSizeAct;//猜大小



		int                         clickTime;
		bool                        yazhongBool;
		bool                        isCanGuessB;
		bool                        isGuessIng;

		float						m_fClockTime;
		float						m_fTimeCount;
		BYTE						m_bTimerChairID;			//定时器椅子号
		int							m_iTimerID;					//定时器id
		bool						m_bDoTimeOutCallback;		//是否做超时处理

		int								m_loopAnimationRemainTime;	//精灵仍然需要移动的数量
		int								m_loopAnimationMaxTime;		//精灵需要移动的最大数量
		int								m_loopAnimationEndIndex;	//精灵移动的终点
		//    int                             m_loopAnimationStartIndex;  //精灵移动的起点，即上局结束时的终点
		int                             m_loopAnimationCurrentIndex;    //当前高亮的精灵的位置，值为0-31


		//OpenRecord
		vector<int>					m_openRecordAnimal;			//服务端开启过的动物类型，目前只记录十个，实际记录动物在圆盘上的位置，0-31
		//

		Text*	m_TextTip;
		long long    m_i64TipMoney;
		long long    resultAllMoney;

        bool m_bLuck;
	private:
		//动画
		std::vector<std::vector<Sprite*>>   m_AllNotesMoveAnimations;

		std::vector<Vec2>                   m_notesPosition;
		std::vector<Vec2>                   m_betAreaPosition;
		std::vector<Vec2>                   m_betLabelPos;

	public:
		GameTableLogic*			_GameTableLogic;
		BYTE					_bDeskIndex;
		bool					_bAutoCreate;
		//////////////////////////////////////////////////////////////////////////
	public:
		GameTableUIV88();
		~GameTableUIV88();
		static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

		bool init(BYTE bDeskIndex, bool bAutoCreate);

		virtual bool onTouchBegan(Touch* touch, Event* event) override;
		virtual void onEnter() override;
		virtual void onExit() override;

		virtual void onExitTransitionDidStart() override;

		void updatePlayerInfo();

		void jumpGuessNum(float time);


		//数据
	public:
		int				m_iFreeTime;					//空闲时间
		int				m_iBetTime;						//下注时间
		int				m_iPrevHasNote;					//上局是否有下注，若上局没有下注，则此局续投无效
		int				m_iCircleTime;
		int				m_iShowResultTime;				//显示开奖时间
		int             m_iSettlementTime;                  //结算时间

		long long		m_i64SingleNoteLimit;			//单人下注限额
		long long       m_i64BankerNeedMoney;           //做庄限制

		int				m_iMultiplying[BetA_Count];		//倍率 11区域+金鲨、通赔、通吃
		int				m_iHistory[Open_History_Logo_Count];					//开奖记录
		long long		m_i64OwnNotes[BetA_Count]; //我的各区域押注
		long long		m_i64AreaNotes[BetA_Count]; //所有人各区域押注

		long long		i64Reward;		//当局奖金
		long long		i64MyMoney;		//背包金币
        long long       m_i64BaseCoin;  //基础金币单位


		int				m_iBetType;						//下注类型
		int				m_iBetMoneyType;				//下注钱类型
		int             luckT;                           //执行次数
		int             luckOpenT;
		int				iOpenLucky[6];
		bool            inInLoop;

    private:
		//侧边栏
		void createToolBarUI();
		//大厅底节点
		void createRootLayer();
		//添加Loop
		void addLoopSprites();
		//筹码按钮区域
		void createMoneyArea();
		//筹码金额展示
		void createNoteLabels();
		//
		//    void createCarLayer();
		//
		void createPlayerInfon();
		//
		//    void createGameStateLayer();
		//
		void createGameSettlementLayer();
		//游戏记录
		void createState();
//        //游戏时间
//        void createClock();
		//
		void createBetLb();
		void stopLuckLoopAnimation();//保留lucky开奖的结果

		void showOpenBtn(float dt);
		tag_s2c_tip_exit*	tipMsg;
		void tipCallback();
		void updateTip(float dt);

	public:
		virtual void leaveTheGame() override;
		//第一次进入游戏时同步基础数据
		//    virtual void syncBasicDataFirstTime(SyncDataStruct* msg) override;
		//进入或者短线重连恢复游戏桌子场景
		//    virtual void resumeTheGameFree(GameStation_Base* pcene, BYTE gameStation) override;
		//    virtual void resumeTheGameBet(GameStation_Bet* pcene, BYTE gameStation) override;
		//    virtual void resumeTheGameKaiJiang(GameStation_Kaijiang* pcene, BYTE gameStation) override;
		//    virtual void resumeTheGameOver(GameStation_Base* pcene, BYTE gameStation) override;





//		void onBtnClick(Ref* pSender, Control::EventType event);
		void delayShowOpenLogoEffect(float time);
	private:
		//时钟方法
//        void updateClock(float dt);
//        void setTimer(BYTE bChairID, int iTimerID, float fTime);
//        void killTimer();
		//    void timeOutCallback(BYTE bChairID, int iTimerID);		//超时处理
		//    void updateUIForResume();
		void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);

	private:
		void initData();
		void resetNewGame();
		void resetUINewGame();
		void resetData();
		void resetAllNotesAnimationAndClean();


		//Tools
	private:
		void setButtonEnabled(ui::Button* pButton, bool enabled);
		void updateAllNoteButtonsState(bool enable);
		void updateBetMoneyEnable();
		void enableAllBetMoney();
		void disableAllBetMoney();
		void updateGameState(int gameState);
		void hideGameState(bool hideState = false);

		void hideGameStateText(float time);

	private:
		//按钮回调
		//    void requestBankerCallBack(Ref* pSender, Widget::TouchEventType type);//请求上庄按钮回调
		void betAreaButtonsCallBack(Ref* pSender, Widget::TouchEventType type);//下注
		void guessSizeCallBack(Ref* pSender, Widget::TouchEventType type);//猜大小下注回调
		void chipInButtonCallBack(Ref* pSender, Widget::TouchEventType type);//增加金币 减少金币毁掉
		void starButtonCallBack(Ref* pSender, Widget::TouchEventType type);//增加金币 减少金币毁掉

		//接收结算消息
		virtual void syncBasicDataFirstTime(SyncDataStruct* msg) override;
		//    void betMoneyCallBack(Ref* pSender, Widget::TouchEventType type);//5个下注金额按钮回调
		//    void betClearCallBack(Ref* pSender, Widget::TouchEventType type);//清空按钮回调
		//    void betRebetCallBack(Ref* pSender, Widget::TouchEventType type);//续投按钮回调
		//
	private:

		void betMoneyToAreaAnimation(int betMoneyIndex, int betAreaIndex, bool isMe = false);
		void updateBetMoneySelectedState(int betMoneyIndex);

		void showRunLoopAnimation(int endIndex, int loopTime, float delayTime = 5.0f);
		void updateLoopSpritePosition(float dt);
		void updateRandomBlinkLightFree(float dt);

		void updateTestContent(float dt);

		void updateBlinkOpenAnimalLight();
		//
		void showOpenAnimalAnimation(int index);

		void startLoopAnimationAtIndex(int index);
		void playOpenLogoAnimationAtIndex(int index);
		void stopAllLoopAnimation();
		void updateStateImage(int iHistory[]);
		//
		void animationMoveOpenItemToStateLabel(int endIdx);

		//车灯动画
		void animationOpenCarLight();
		void animationStopCarLight();
		void showLuckyEffectLoop(float time);
		void updateMoneyState();//更新钱的状态
        
        void playBigWinAnimation(int gameID, int openLogo, int logoRate, LLONG money);

		//private:
		//玩家坐下
		virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		//玩家站起
		virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		//添加用户
		virtual void addUser(BYTE seatNo, bool bMe) override;

		//服务端返回下注结果
		virtual void onGameNoteResponse(GameNoteResponse* msg) override;
		//服务端下注失败结果
		virtual void onGameNoteFail(GameNoteFailedMsg* msg) override;
		//显示开奖结果
		virtual void onGameOpenLogo(GameLoopResult* msg) override;
		//
		virtual void onGameNotifyGuess(GameNotifyGuessNumber* msg) override;
		//
		virtual void onGameGuessOverTime(GameGuessOverTime* msg) override;
		//
		virtual void onGameGuessResult(GameGuessResult* msg) override;
		//
		virtual void onGameSwapScoreResponse(GameSwapScoreReseponse* msg) override;
		//
		virtual void onGameNotifyLucky(GameNotifyLucky* msg) override;
		//展示开Lucky结果
		virtual void onGameOpenLuckyLogos(GameLuckyOpen* msg) override;
		//接收结算消息
		virtual void onGameSettlementMsg(GameSettlementMsg* msg) override;

		//奖池
		virtual void onGameRewardsPool(GameRewardsPool* msg) override;

		//奖池
		virtual void onGameBeginFailLackCoin(GameBeginFailLackCoin* msg) override;

		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) override;
		//通知
		virtual void onGameNotify(tag_s2c_notify* msg) override;
		////
		virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
        
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
	};
}

#endif // __BRNN_GameTable_BRNNTableUI_h__
