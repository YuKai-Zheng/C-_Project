#ifndef SHZGameTableUIV88_h
#define SHZGameTableUIV88_h

#include <string>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"
#include "SHZGameTableUICallback.h"
#include "UI/Base/AUIButton.h"
//#include "../../TTNN/Classes/v88/BRNNClockV88.h"
#include "SHZGameTableLogic.h"
#include "SHZGuessSizeV88.h"
#include "SHZSettlementV88.h"
#include "SHZMaryTableUIV88.h"
#include "HNLobby/GameNotice.h"
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;
using namespace ui;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace SHZ {

	class GameTableLogic;


	class GameTableUIV88 : public HNGameUIBase, public GameTableUICallback {

		//////////////////////////////////////////////////////////////////////////
		//控件结构体
	private:
	
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


		unsigned int    m_audioEffectID;




//      Button*                     autoBtn;
		Button*                     jiazhuBtn;
        Button*                     jianzhuBtn;
		Button*                     bibeiBtn;
		Button*                     startBtn;
        Button*                     shoudongBtn;
        Button*                     cancelAutoBtn;
        Button*                     xiafenBtn;
        Button*                     zuidayafen;
		Text*                       myName;
        Text*                        scoreText;
//        BRNN::ClockV88*             m_clockObj;
//        Vec2                        m_clockPos;

		Text*                 yazhuText;
		Text*                 guessNumText;
		Text*                 myLastMoneyText;
		Text*                 zongyazhuText;

		Text*					gameStateText;
		ImageView*              costTypeImg;

        GameNotice*					m_Notice;                   //广播
        
        spine::SkeletonAnimation*   topSpine;
        int                         clickTime;
		bool                        yazhongBool;
		bool                        isCanGuessB;
		bool                        isGuessIng;

//        float                        m_fClockTime;
		float						m_fTimeCount;
		BYTE						m_bTimerChairID;			//定时器椅子号
		int							m_iTimerID;					//定时器id
		bool						m_bDoTimeOutCallback;		//是否做超时处理

        bool                        isAutoGame;//是否自动游戏

        Text*	m_TextTip;
		long long    m_i64TipMoney;
        ImageView* m_titleImg;
        ImageView* m_winImg;
        TextAtlas* m_winNum;
        ImageView* m_bibeiTip;
        ImageView* m_defenTip;

	private:
		//动画
		std::vector<std::vector<Sprite*>>   m_AllNotesMoveAnimations;

		std::vector<Vec2>                   m_notesPosition;
		std::vector<Vec2>                   m_betAreaPosition;
		std::vector<Vec2>                   m_betLabelPos;
        
        vector<Layout*>             m_vPanelTurnArea;
        vector<vector<ImageView*>>  m_vTurnIcon;
        vector<Vec2>                m_vTurnIconPos;
        vector<int>                 m_vTurnUnitCount;           //转多少个icon单元
        int                         m_nHasTurnUnitCount;        //已经转的单元
        
        vector<ImageView*> _resultImg;
        int                 nOpenLotteryTypeColRow[SHZ_COLUMN][3];
        
        bool m_shzHasReward;
        bool m_bAnimationFinished;
	public:
		GameTableLogic*			_GameTableLogic;

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
        void showMoneyFunc(long long winMoney);
        
        void loadTurn();
        void setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize);
        void transformOpenLotteryType();
        void cleanResults();
        void randomColumnDatasAndCalcTurnCount();
        void turnUpdate(float dt);
		//数据
	public:
		int				m_iFreeTime;					//空闲时间
		int				m_iBetTime;						//下注时间
		int				m_iPrevHasNote;					//上局是否有下注，若上局没有下注，则此局续投无效
		int				m_iCircleTime;
		int				m_iShowResultTime;				//显示开奖时间
		int             m_iSettlementTime;                  //结算时间


		SHZSettlementV88*   _gameSettlement;
		SHZGuessSizeV88*    _gameGuessSizeLayer;
        SHZMaryTableUIV88*  _gameMaryLayer;

		Node*                      root;
		Node*                      resultAnimationNode;
		cocostudio::timeline::ActionTimeline*            lighBarKaijiangAct;//跑马灯




		long long		i64Reward;		//当局奖金
		long long		i64MyMoney;		//背包金币




		int             addMoneyTimes;                     //游戏加注减住的次数
		long long             i64NoteUnit;                     //每次押注的单元
		long long             i64NoteMin;                     //每次押注最少金币
		long long             i64NoteMax;                   //每次押注最大金币
		long long             yazhuMoney;
		long long             zongyazhuMoney;

		long                  showEffectTime;
		int                   countLine;


		int 				allLogoDatas[15];			//服务端发送的15个图标

		int                 lineResultData[LINE_COUNT];//开奖的数据 总的15个图标
		int                 lineResultNumData[LINE_COUNT];//开奖的的线
		int                 logoResultRate[LINE_COUNT];//开奖的倍率 中奖的倍率
		int                 logoResultType[LINE_COUNT];//开奖的类型  林冲

		int                 tempLineResultData[LINE_COUNT];//开奖的数据

		int                 logoOpenNumCount[LINE_COUNT];	//开奖图标的倍数

		Sprite*             showEffectLine;           //显现的那条闲

		std::vector<Sprite*>   allLogoSpriteVec;
		std::vector<Sprite*>   allLineSpriteVec;

        long long m_i64WinMoney;
        
        vector<Sprite*> m_AnimationNodeVec;

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

		void showOpenBtn(float dt);
		tag_s2c_tip_exit*	tipMsg;
		void tipCallback();
		void updateTip(float dt);

	public:
		virtual void leaveTheGame() override;
		void setNick(char* nick);
		//第一次进入游戏时同步基础数据
		//    virtual void syncBasicDataFirstTime(SyncDataStruct* msg) override;
		//进入或者短线重连恢复游戏桌子场景
		//    virtual void resumeTheGameFree(GameStation_Base* pcene, BYTE gameStation) override;
		//    virtual void resumeTheGameBet(GameStation_Bet* pcene, BYTE gameStation) override;
		//    virtual void resumeTheGameKaiJiang(GameStation_Kaijiang* pcene, BYTE gameStation) override;
		//    virtual void resumeTheGameOver(GameStation_Base* pcene, BYTE gameStation) override;

        void onBtnClick(Ref* pSender, Control::EventType event);
		void delayShowOpenLogoEffect(float time);
        virtual void update(float dt) override;
	private:
		//时钟方法
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

	private:
		//按钮回调

		void betAreaButtonsCallBack(Ref* pSender, Widget::TouchEventType type);//下注
		void guessSizeCallBack(Ref* pSender, Widget::TouchEventType type);//猜大小下注回调
		void chipInButtonCallBack(Ref* pSender, Widget::TouchEventType type);//增加金币 减少金币毁掉
		void startButtonCallBack(Ref* pSender, Widget::TouchEventType type);//开始游戏

		void autoGameButtonCallBack(Ref* pSender, Widget::TouchEventType type);//自动开始游戏

		//接收结算消息
		virtual void syncBasicDataFirstTime(struct_s2c_base* msg) override;
		virtual void updateGameState(struct_game_state* msg) override;//更新游戏状态
		virtual void openLineResultIsCanGuess(struct_s2c_caiDaXiao* msg) override;//
		virtual void gueessSizeStar(struct_s2c_doCaiDaXiao* msg) override;//
		virtual void guessSizeResult(struct_s2c_caiResult* msg) override;//
		virtual void guessSizeOver(struct_s2c_caiFinish* msg) override;//猜大小结束
		virtual void openLineResultData(struct_s2c_startGame* msg) override;
        virtual void openHasMaryTip(struct_s2c_hasMary* msg) override;
        virtual void updateMaryData(struct_s2c_maryData* msg) override;
        virtual void maryFinishTip(struct_s2c_maryFinish* msg) override;
		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) override;
        virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);

		//
	private:
        void updateMoneyState();//更新钱的状态
	

		//private:
		//玩家坐下
		virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		//玩家站起
		virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		//添加用户
		virtual void addUser(BYTE seatNo, bool bMe) override;

		private:
			void showLineEffect(int lineNum);
			void showLogoAnimation(int line[9], int logoTye[9]);
			void showOpenAnimalAnimation(BYTE index, int logoIndex);
			void playLogoAnimation(std::string res, std::string tagName,int logoIndex);
			void createGameSettlementLayer();
			void createGameGuessSizeLayer();
            void createGameMaryLayer();
			void showStartGameEffect();
			void showAllOpenLineVisble(bool isShow);
			void setAllLineVisblefalse();
			bool isEqualLogoType(int type1, int type2);
            void playDragonAnimation();
            void playFlagAnimation();
        
            void createAnimationRotate();
            void stopRotateAnimation(int nCol);
	};
}

#endif // __BRNN_GameTable_BRNNTableUI_h__
