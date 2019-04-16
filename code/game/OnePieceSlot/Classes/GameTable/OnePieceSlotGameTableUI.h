#ifndef __OnePieceSlotGameTableUI_h__
#define __OnePieceSlotGameTableUI_h__

#include "HNNetExport.h"

#include "OnePieceSlotGameTableLogic.h"

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocostudio/ActionTimeline/CCActionTimeline.h"
using namespace cocostudio;
using namespace ui;

using namespace cocos2d;
using namespace cocos2d::extension;

class GameNotice;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace OnePieceSlot
{
    class GameTableLogic;
    class GameTableUI : public HNGameUIBase , public GameTableUICallback
    {
    public:
        GameTableUI();
        ~GameTableUI();
        
        bool init(BYTE bDeskIndex, bool bAutoCreate);
    public:
        static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);
        
		GameTableLogic*			_GameTableLogic;
        
    public:
        virtual void onEnter() override;
        virtual void onExit() override;
		virtual void onExitTransitionDidStart() override;
      
		BYTE		m_bDeskIndex;
		bool		m_bAutoCreate;
    private:
        //离开
		virtual void leaveTheGame() override;
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        //添加用户
        virtual void addUser(BYTE seatNo, bool bMe) override;
		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) override;
		//公告
		virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
		//游戏状态
		virtual void onGameState() override;
		//基础消息包
		virtual void onGameBaseInfo(struct_s2c_base* msg) override;
		//更新奖池
		virtual void onGamePoolMoney(struct_s2c_pool* msg) override;
		//游戏开始
		virtual void onGameStart(struct_s2c_startGame* msg) override;
		//是否猜大小
		virtual void onGameDoCaiDaXiao(struct_s2c_doCaiDaXiao* msg) override;
		//猜大小
		virtual void onGameCaiDaXiao(struct_s2c_caiDaXiao* msg) override;
		//猜大小结算
		virtual void onGameCaiResult(struct_s2c_caiResult* msg) override;
		//游戏结算结束
		virtual void onGameFinish(struct_s2c_caiFinish* msg) override;
        
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
        
        void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);
	public:
		//ui
		Node*			m_csb;
		Button*			m_btnAuto;
		Button*			m_btnCancel;
		Button*			m_btnBet;
		Button*			m_btnBetMax;
		Button*			m_btnStart;
		Button*			m_btnScore;
		Button*			m_btnDouble;
		Button*			m_btnBar;
		cocostudio::timeline::ActionTimeline*	m_barAction;

		Text*			m_textBetUnit;
		Text*			m_textBetMul;
		Text*			m_textPoolMoney;
		Text*			m_textMyMoney;
		Text*			m_textScoreMoney;

		ImageView*		m_imageOpenTypeBlink;
		ImageView*		m_imageScoreFrame;
		ImageView*		m_imageDoubleFrame;

		vector<Layout*> m_vPanelTurnArea;
		vector<vector<ImageView*>> m_vTurnIcon;
		vector<ImageView*> m_vImageTurnBlink;
		vector<Node*>	   m_vTurnBlinkCsb;
		vector<cocostudio::timeline::ActionTimeline*>	   m_vTurnBlinkAction;
		vector<ImageView*> m_vImageBetAreaBlink;
		
		Layout*			m_panelDouble;
		Button*			m_btnSmall;
		Button*			m_btnBig;
		Text*			m_textOpenPoint;
        GameNotice*     m_Notice;

		//猜大小输赢特效
		Node* m_caiWinCsb;
		cocostudio::timeline::ActionTimeline* m_caiWinAction;
		Node* m_caiLoseCsb;
		cocostudio::timeline::ActionTimeline* m_caiLoseAction;
		Node* m_caiResultCsb0;
		cocostudio::timeline::ActionTimeline* m_caiResultAction0;
		Node* m_caiResultCsb1;
		cocostudio::timeline::ActionTimeline* m_caiResultAction1;
	public:
		//本地
		bool			m_bAuto;				//是否自动
		int				m_nCurBetMul;			//压倍数1-3

		vector<Vec2>	m_vOpenTypePos;
		vector<Node*>	m_vBlinkNode;

		vector<Vec2>	m_vTurnIconPos;
        vector<int>     m_vOpenLotteryType;         // 开奖ICON TYPE
		vector<int>		m_vTurnUnitCount;			//转多少个icon单元
		int				m_nHasTurnUnitCount;		//已经转的单元
		float			m_fTurnUnitTime;			//每个单元时间
		int				m_nOpenShowTime;			//开完奖其他展示时间
		int				m_nOpenLineType;			//开的类型
		
		int				m_nTurnMusicID;				//转动
	public:
		//下发
		long long		m_i64BetUnit;			//单注

	public:
		void autoBtnCallback(Ref* pSender, Widget::TouchEventType type);
		void betBtnCallback(Ref* pSender, Widget::TouchEventType type);
		void startBtnCallback(Ref* pSender, Widget::TouchEventType type);
		void doubleBtnCallback(Ref* pSender, Widget::TouchEventType type);
		void guessBtnCallback(Ref* pSender, Widget::TouchEventType type);
	public:
		
		void loadButton();
		void loadText();
		void loadTurn();
		void loadDoublePanel();
		void createToolBarUI();

		//turn
		void turnUpdate(float dt);
		void setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize);
		void setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum);
		void checkTurnIsSuccess();

		void showCaiResultEffect(bool bSuccess, int nPoint);
		void showBetAreaBright(int nBetMul);
		void blinkUpdate(float dt);
		void setBink(Node* target, bool bAdd);
		void setBtnsTouchEnabled(bool bTouch);
		void numJumpUpdate(float dt);
		void setNumJump(bool bJump);
		void cleanData();

	public:

		void sendGameStart();
		void sendDoCaiDaXiao(int nDo);
		void sendCaiDaXiao(int nDo, int nType);
        
    private:
        void playBigWinAnimation(int gameID, int openLogo, int logoRate, LLONG money);
    
    private:
        vector<ImageView*> _resultImg;
        
        void cleanResults();
    };
}

#endif // __OnePieceSlot_GameTableUI_h__
