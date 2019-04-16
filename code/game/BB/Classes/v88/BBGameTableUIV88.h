#ifndef BBGameTableUIV88_h
#define BBGameTableUIV88_h

#include <string>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"
#include "BBGameTableUICallback.h"
#include "UI/Base/AUIButton.h"
//#include "../../TTNN/Classes/v88/BRNNClockV88.h"
#include "UI/TimerObject.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;
using namespace ui;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace BB {

	class GameTableLogic;
	class BBLoopSpriteCellV88;
	class BBPlayerHeadV88;
	class BBGameStateV88;
	class BBSettlementV88;
	class BBNoteLabelCellV88;

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
			std::vector<Button*> betMoneyBtn;   //5个下注金额
			std::vector<Sprite*> betMoneyLight; //5个下注金额按钮选中状态
			std::vector<BBNoteLabelCellV88*> noteLabels;    //每个区域的下注总额（个人总额及所有玩家总额）
			//        std::vector<Text*> myCoinText;      //每个区域自己的下注额
			//        std::vector<Text*> allCoinText;     //每个区域的下注总额
			AUIButton* reBetBtn;                //续投按钮，按照上局的下注数据进行下注
			AUIButton* clearBtn;                //清空当局下注数据按钮
			Bet_Area() {
				betNode = nullptr;
				moneyNode = nullptr;
				tableBg = nullptr;
				betButtons.clear();
				betMoneyBtn.clear();
				betMoneyLight.clear();
				//            myCoinText.clear();
				//            allCoinText.clear();
				noteLabels.clear();
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

		//上庄区域
		struct Banker_Area {
			//        Node* bankerNode;
			Button* requestBankerBtn;    //上庄按钮
			Text* bankerName;               //庄家名字
			Text* bankerCoin;               //庄家的金币
			Text* requestBankerCount;       //申请上庄人数
			Text* requestBankerNeeds;       //上庄需金币
			bool isShangZhuang;             //显示上庄上庄
			Text* requestBankerNeedsLabel;  //
			Banker_Area() {
				//            bankerNode = nullptr;
				requestBankerBtn = nullptr;
				bankerName = nullptr;
				bankerCoin = nullptr;
				requestBankerCount = nullptr;
				requestBankerNeeds = nullptr;
				isShangZhuang = true; //没有请求上庄，显示上庄提示文字
				requestBankerNeedsLabel = nullptr;
			}
		} _bankerArea;

		//结果展示区域
		struct Loop_Area {
			Node* loopNode;
			std::vector<Vec2> loopCellPositions;    //展示区域基本元素的位置
			std::vector<BBLoopSpriteCellV88*> loopSprites;   //结果展示区域基本单位
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
			vector<BBLoopSpriteCellV88*> openLogos;
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
			long long betMoneyEachArea[LoopK_A_BetA_Count];     //个人每个下注区域下注金额
			bool betMoneyEnable[Bet_Coin_Count];                //每个下注筹码有效性
			Bet_Data() {
				isBanker = false;
				betMoney = 0;
				memset(betMoneyEachArea, 0, sizeof(betMoneyEachArea));
				memset(betMoneyEnable, 1, sizeof(betMoneyEnable));
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
		BBPlayerHeadV88* _playerHead;
		BBGameStateV88* _gameState;
		BBSettlementV88* _gameSettlement;

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



		//
		//    //结果转盘
		//    struct Result_Loop {
		//        Node* loopNode;
		//        vector<Sprite*> animals;
		//        Result_Loop() {
		//            loopNode = nullptr;
		//            animals.clear();
		//        }
		//    } _resultLoop;
		//
		//    //结果
		//    struct Result_Label {
		//        Node* labelNode;
		//        TextBMFont* font;
		//        Result_Label() {
		//            labelNode = nullptr;
		//            font = nullptr;
		//        }
		//    } _resultLabel;
		//
		//    //下注操作控件
		//    struct Bet_Buttons {
		//        Node* node;
		//        vector<Button*> buttons;
		//        vector<Sprite*> btnLights;
		//        Bet_Buttons() {
		//            node = nullptr;
		//            buttons.clear();
		//            btnLights.clear();
		//        }
		//    } _betButton;
		//
		//    //彩金池
		//    struct Gold_Pool {
		//        Node* poolNode;
		//        //Sprite* bg;
		//        Text* poolMoney;
		//        long long i64Money;
		//        Gold_Pool() {
		//            poolNode = nullptr;
		//            //bg = nullptr;
		//            poolMoney = nullptr;
		//            i64Money = 0;
		//        }
		//    } _goldPool;
		//




	


	private:
		//资源
		//    Sprite*						_tableBg;
		Sprite*						_toolbarBg;
		TimerObject*                m_clockObj;
		Vec2                        m_clockPos;

		//数据
		float						_fScaleX;
		float						_fScaleY;
		int							m_GameStatus;
		bool						m_bNextOut;					//下局离开



//        ImageView*                    m_ClockBg;
//        LabelAtlas*                    m_ClockTimeLabel;            //时钟label

		//    //ImageView*					m_StageImage;				//阶段图标
		//    Sprite*						m_AnimateImage;				//动画图片
		//
		//
        GameNotice*					m_Notice;                   //广播
		//
		//    BYTE						m_bPrepareTime;				//准备时间
		//    BYTE						m_bCatchBankerTime;			//抢庄时间
		//    BYTE						m_bSetMoneyTime;			//庄家设置红包金额时间
		//    BYTE						m_bSetBombTime;				//庄家设置红包雷数字时间
		//    BYTE						m_bOpenLETime;				//闲家开红包时间
		//    BYTE						m_bShowResultTime;			//显示游戏结果时间

		float						m_fClockTime;
//        float                        m_fTimeCount;
		BYTE						m_bTimerChairID;			//定时器椅子号
		int							m_iTimerID;					//定时器id
		bool						m_bDoTimeOutCallback;		//是否做超时处理
		//    int							m_iSendRoundNum;			//单张发轮数,为5轮
		//    int							m_iSendViewID;				//发方位
		//

		//    //vector<BYTE>				m_vPlayerPrepare;
		//    BYTE						m_bCurUser;					//当前操作者
		//    BYTE						m_bNextUser;				//下次操作者
		//    //BYTE						m_bBankerID;				//庄家
		//
		//    bool m_canPrepareForNew;
		//
		//    vector<Vec2>					m_betAreaPosition;			//飞禽、走兽、鲨鱼下载控件位置
		//    vector<Size>					m_betAreaKindSize;			//种类、鲨鱼下注区域控件大小
		//
		//    vector<Vec2>					m_loopAnimalPosition;		//28种动物的位置
		//    vector<Sprite*>					m_loopAnimationSprite;		//3个动画起始点的精灵

		int								m_loopAnimationRemainTime;	//精灵仍然需要移动的数量
		int								m_loopAnimationMaxTime;		//精灵需要移动的最大数量
		int								m_loopAnimationEndIndex;	//精灵移动的终点
		//    int                             m_loopAnimationStartIndex;  //精灵移动的起点，即上局结束时的终点
		int                             m_loopAnimationCurrentIndex;    //当前高亮的精灵的位置，值为0-31

		unsigned int    m_audioEffectID;


		//OpenRecord
		vector<int>					m_openRecordAnimal;			//服务端开启过的动物类型，目前只记录十个，实际记录动物在圆盘上的位置，0-31
		//
		//    //OpenResult
		//    vector<BYTE>					m_openResultAnimalKind;		//当局服务端开奖的动物类型
		//    vector<BYTE>					m_openResultAnimalIndex;	//开动物Button的位置
		//
		//    //进入游戏后总输赢
		//    long long						m_allMyWinGold;				//进入游戏后总的输赢
		//    BYTE							m_betButtonTag;				//下注阶段选中的下注按钮索引值
		//
		//    //游戏阶段
		//    Node*							m_gameStateNode;			//游戏阶段显示node
		//    Text*							m_gameStateLabel;			//游戏阶段显示label
		//

	private:
		//动画
		std::vector<std::vector<Sprite*>>   m_AllNotesMoveAnimations;

		std::vector<Vec2>                   m_notesPosition;
		std::vector<Vec2>                   m_betAreaPosition;
		std::vector<Vec2>                   m_betLabelPos;

		//    std::vector<Sprite*>                m_LamMaxNoteAnimations;
		//    std::vector<Sprite*>                m_BMWMaxNoteAnimations;
		//    std::vector<Sprite*>                m_BenMaxNoteAnimations;
		//    std::vector<Sprite*>                m_VMMaxNoteAnimations;
		//    std::vector<Sprite*>                m_VMMinNoteAnimations;
		//    std::vector<Sprite*>                m_BenMinNoteAnimations;
		//    std::vector<Sprite*>                m_BMWMinNoteAnimations;
		//    std::vector<Sprite*>                m_LamMinNoteAnimations;

		//    Node*							m_animalAnimationNode;		//播放动物动画的统一节点


		//private:
		//    //le_s2c_envelope_info _open_envelope_info;
		//    Text*	m_TextTip;
		//    long long    m_i64TipMoney;
		//    tag_s2c_tip_exit	tipMsg;
		//
		//    typedef std::function<void()> ShowBankerCallback;
		//    ShowBankerCallback showBankerCallback;

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

		//数据
	public:
		int				m_iFreeTime;					//空闲时间
		int				m_iBetTime;						//下注时间
		int				m_iPrevHasNote;					//上局是否有下注，若上局没有下注，则此局续投无效
		int				m_iCircleTime;
		int				m_iShowResultTime;				//显示开奖时间
		int             m_iSettlementTime;                  //结算时间

		long long		m_i64SingleNoteLimit;			//单人下注限额
		int				m_iMultiplying[LoopK_A_BetA_Count];		//倍率 11区域+金鲨、通赔、通吃
		int				m_iHistory[Open_History_Logo_Count];					//开奖记录
		long long		m_i64OwnNotes[LoopK_A_BetA_Count]; //我的各区域押注
		long long		m_i64AreaNotes[LoopK_A_BetA_Count]; //我的各区域押注


		int				m_iBetType;						//下注类型
		int				m_iBetMoneyType;				//下注钱类型
		long long       m_i64BankerNeedMoney;           //做庄限制

		Text*	m_TextTip;
		long long    m_i64TipMoney;
		ImageView*    clearImg;
		ImageView*    xutouImg;

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
		//void createCarLayer();
		//
		void createPlayerInfon();
		//
		void createGameStateLayer();
		//
		void createGameSettlementLayer();
		//游戏记录
		void createState();
		//游戏时间
		void createClock();
		tag_s2c_tip_exit*	tipMsg;
		void tipCallback();
		void updateTip(float dt);
		

	public:
		virtual void leaveTheGame() override;
		//第一次进入游戏时同步基础数据
		virtual void syncBasicDataFirstTime(SyncDataStruct* msg) override;
		//进入或者短线重连恢复游戏桌子场景
		virtual void resumeTheGameFree(GameStation_Base* pcene, BYTE gameStation) override;
		virtual void resumeTheGameBet(GameStation_Bet* pcene, BYTE gameStation) override;
		virtual void resumeTheGameKaiJiang(GameStation_Kaijiang* pcene, BYTE gameStation) override;
		virtual void resumeTheGameOver(GameStation_Base* pcene, BYTE gameStation) override;

        
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);



//		void onBtnClick(Ref* pSender, Control::EventType event);
	private:
		//时钟方法
//        void updateClock(float dt);
		void setTimer(BYTE bChairID, int iTimerID, float fTime);
		void killTimer();
	
        void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);

	private:
		void initData();
		void resetNewGame();
		void resetUINewGame();
		void resetData();
		void resetAllNotesAnimationAndClean();


		//Tools
	private:
		void setButtonEnabled(ui::Button* pButton, bool enabled, bool IsSetOpacity);
		void updateAllNoteButtonsState(bool enable);
		void updateBetMoneyEnable();
		void updateGameState(int gameState);
		void hideGameState(bool hideState = false);

	private:
		//按钮回调
		void requestBankerCallBack(Ref* pSender, Widget::TouchEventType type);//请求上庄按钮回调
		void betAreaButtonsCallBack(Ref* pSender, Widget::TouchEventType type);//下注区域8个下注按钮回调
		void betMoneyCallBack(Ref* pSender, Widget::TouchEventType type);//5个下注金额按钮回调
		void betClearCallBack(Ref* pSender, Widget::TouchEventType type);//清空按钮回调
		void betRebetCallBack(Ref* pSender, Widget::TouchEventType type);//续投按钮回调

		//
		private:
			bool            m_iSelfIsBanker;                //自己是否是庄家
			char            m_iBankerNickName[61];          //庄家的名字

	private:

		void betMoneyToAreaAnimation(int betMoneyIndex, int betAreaIndex, bool isMe = false);
		void updateBetMoneySelectedState(int betMoneyIndex);

		void showRunLoopAnimation(int endIndex, int loopTime, float delayTime = 5.0f);
		void updateLoopSpritePosition(float dt);
		void updateRandomBlinkLightFree(float dt);
		//    void showResultAnimalAnimation(BYTE animalKind, float showTime = 4.0f);
		//    void updateResultAnimation(float dt);
		void updateBlinkOpenAnimalLight();
		//    void updateGameStateLabelContent();
		//    void showGameStateLabelAnimation(float dt);
		//
		//    void stopBlinkAnimalLight();
		//
		//
		void showOpenAnimalAnimation(int index);

		void startLoopAnimationAtIndex(int index);
		void playOpenLogoAnimationAtIndex(int index);
		void stopAllLoopAnimation();
		//    void playAnimalsAnimation(std::string res, std::string tagName);
		//    void stopAnimalsAnimation();
		//    void tongpeiTongshaAnimation(bool flag);
		//    void showResultAnimation(long long changeMoney);
		void updateStateImage(int iHistory[]);
        void playBigWinAnimation(int gameID, int openLogo, LLONG money);
		//
		void animationMoveOpenItemToStateLabel(int endIdx);

		//车灯动画
		void animationOpenCarLight();
		void animationStopCarLight();
		void enableAllBetMoney();
		void disableAllBetMoney();


		void updateTestContent(float dt);


		//private:
		//玩家坐下
		virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		//玩家站起
		virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		//添加用户
		virtual void addUser(BYTE seatNo, bool bMe) override;

		//游戏空闲状态
		virtual void onGameFree(GameFreeStruct* msg) override;
		//庄家确定
		virtual void onConfirmBanker(GameBeganAndConfirmBanker* msg) override;
		//开始下注
		virtual void onGameBeginNote(GameBeganBet* msg) override;
		//        //通知下注
		//        virtual void onGameBeginBet(GameBeganBet* msg) override;
		//服务端返回下注结果
		virtual void onGameNoteResponse(GameXiaZhuResponse* msg) override;
		//服务端返回清空结果
		virtual void onGameClearResponse(GameClearResponse* msg) override;
		//服务端返回续投结果
		virtual void onGameReBetResponse(GameReBetResponse* msg) override;
		//请求上庄消息
		virtual void onGameRequestBankerResponse(GameRequestBankerResponse* msg) override;
		//下注结束
		virtual void onGameNoteFinish(GameXiaZhuFinish* msg) override;
		//    //下注结果
		//    virtual void onGameOpenLogo(GameLoopResult* msg) override;
		//    //开始转圈开奖
		//    virtual void onGameStartCircle(GamePlay* msg) override;
		//显示开奖结果
		virtual void onGameOpenLogo(GameLoopResult* msg) override;
		//接收结算消息
		virtual void onGameSettlementMsg(GameSettlementMsg* msg) override;

		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) override;
		//通知
		virtual void onGameNotify(tag_s2c_notify* msg) override;
		////
		virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
		//机器人下注
		virtual void onGameRobotNoteMsg(GameRobotXiaZhuResponse* msg) override;
		//下注失败
		virtual void onGameNoteFail(GameXiaZhuFaileMsg* msg) override;
		//请求下庄
		virtual void onGameXiaZhuangResponse(GameXiaZhuangResponse* msg) override;



	};
}

#endif // __BRNN_GameTable_BRNNTableUI_h__
