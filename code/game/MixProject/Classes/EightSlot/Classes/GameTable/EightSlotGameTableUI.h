#ifndef __EightSlotGameTableUI_h__
#define __EightSlotGameTableUI_h__

#include "HNNetExport.h"

#include "EightSlotGameTableLogic.h"

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "cocostudio/ActionTimeline/CCActionTimeline.h"
#include "HNLobby/v66/IGameLoadingUI.hpp"
#include "HNLobby/GameNotice.h"

using namespace cocostudio;
using namespace ui;

using namespace cocos2d;
using namespace cocos2d::extension;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace EightSlot
{
	class GameTableLogic;
	class GameTableUI : public HNGameUIBase, public GameTableUICallback
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
		//游戏开始
		virtual void onGameStart(struct_s2c_startGame* msg) override;
		
        virtual void onGameUpdateMoney(struct_s2c_update_money* msg) override;
	public:
		//ui
		Node*			m_csb;
        
        Layout*         m_panelBg;
		Button*			m_btnStart;
        Button*         m_btnStop;
		Button*			m_btnScore;
		Button*         m_btnAddNote;
        Button*         m_btnSubNote;
        Button*         m_btnRule;
        Button*         m_btnSet;
        
		Text*			m_textNote;		
		Text*			m_textMyMoney;
		
        
        
        Text*      m_jackpot;
        Text*      m_getScoreBig;
        Text*      m_getScore;
        Text*      m_myName;
		vector<Layout*> m_vPanelTurnArea;
		vector<vector<ImageView*>> m_vTurnIcon;
        
        Node*                                    m_jackpotEffectNode;
        Node*                                    m_moneyEffectNode;
        
        Node*                                    m_getScoreBox;
        Node*                                    m_getScoreDoubleNode;
        Node*                                    m_rotateNode;
        
        TextAtlas*                               m_getScoreBigNodeShow;
	public:
		//本地
		bool			m_bAuto;				//是否自动
		
		vector<Vec2>	m_vTurnIconPos;
        vector<Vec2>    m_vTurnIconPosVirual;
		vector<int>		m_vTurnUnitCount;			//转多少个icon单元
		int				m_nHasTurnUnitCount;		//已经转的单元
    
        int				m_nTurnMusicID;
        long long       m_currentNoteValue;               //当前押注值
	public:
		void stopBtnCallback(Ref* pSender, Widget::TouchEventType type);
		void startBtnCallback(Ref* pSender, Widget::TouchEventType type);
        void getScoreBtnCallback(Ref* pSender, Widget::TouchEventType type);
        void setBtnCallback(Ref* pSender, Widget::TouchEventType type);
        void ruleBtnCallback(Ref* pSender, Widget::TouchEventType type);
        void addNoteBtnCallback(Ref* pSender, Widget::TouchEventType type);
        void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);
        
        
	public:

		void loadButton();
		void loadTurn();
		void createToolBarUI();

		//turn
		void turnUpdate(float dt);
        void jackpotUpdate(float dt);
		void setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize);
        void setTurnIcomPositionEx(vector<Vec2> &vIconPos, Size parentSize);
		void setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum);

		void setBtnsTouchEnabled(Button* btnNode,bool bTouch);
		void cleanData();
        
        void setMyMoney();
        void setJackpot();
        void setNote();
        void addNoteLogic();
        void showIconEffect();
        void showStartBtnEffect(int level);
        void showGetScoreBtnEffect(int level);
        void checkHasReward();
        
        void showGetScoreDoubleEffect(int level,int boxLevel);
        void showRotateEffect(int level);
        void showGetScoreBoxEffect(int level);
        void endAnimation();
        void showCaijinEffect();
        void showTipGetScoreEffect();
        void updateGetScoreTimer(float dt);
        void updateCashOutTimer(float dt);
        
        void showBigWinner();
	public:

		void sendGameStart();
		

	private:
		
    private:
        vector<ImageView*> _resultImg;
        
        void cleanResults();
        
        long long m_i64NoteUnit;                        //加注单位
        long long m_i64NoteMin;                        //每条线最少压注值
        long long m_i64NoteMax;                        //每条线最大压注值
        long long m_i64MyMoney;                        //用户当前金币
        long long m_i64Jackpot;                        //当前奖池
        int m_nLotteryRate[LT_ThreeDollar];            //7种基础图案对应的倍率(百分比)
        
        long long m_i64WinScore;            //本局中奖的得分
        long long m_i64AllScore;            //当前累计的得分
        long long m_i64oldAllScore;         //当前累计的上一次用来计算差值
        int m_nFreeCount;                    //免费次数
        int m_nOpenLotteryType;            //本局中奖的图案，-1是没中图案
        
        bool m_bStop;
        bool m_bAnimation;
        
        long long m_cashOutNum; //下分滚动减少单次金额;
        long long m_myMoneyAddNum; //下分我的金币每次增加;
        long long m_i64OldMyMoney;
        long long m_cashInNum;//得奖增加的金额;
        
        int nFreeCount;
        int m_nOtherEffectID;
        
        GameNotice*                    m_Notice;                   //广播
	};
}

#endif // __FruitSlot_GameTableUI_h__
