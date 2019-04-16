#ifndef __NN_GameTable_NNTableUI_v88_h__
#define __NN_GameTable_NNTableUI_v88_h__

/*
 2     3     4     5     6     7     8     9    10     J     Q     K     A     E     F
 0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D			//方块 2 - A
 0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,			//梅花 2 - A
 0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,			//红桃 2 - A
 0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,			//黑桃 2 - A
 0x4E, 0x4F
 小鬼  小鬼
 
 */

#include "HNNetExport.h"

#include "NNCommonDef.h"

#include "NNGameTableUICallback.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include <string>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "HNLobby/GameNotice.h"
#include "UI/TimerObject.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;
using namespace ui;
//#include "../../TTNN/Classes/v88/BRNNClockV88.h"

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace NN
{   
    class GameTableLogic;
    class PokerCardV88;
    class CPlayerHeadV88;
    
    class GameTableUIV88 : public HNGameUIBase , public GameTableUICallback
    {
 
		enum Em_TableBtnTag{
			em_toolbarBtn = 10000,
			em_toolbarChangeTableBtn,
			em_toolbarSetBtn,
			em_toolbarHelpBtn,
			em_toolbarStandupBtn,
			em_toolbarLeaveBtn,
			em_toolbarNextOutBtn,
			em_shopBtn,
			em_emotionBtn,
			em_noSeatBtn,
			em_openBank,
			em_chatBtn,
			em_chatOpenBtn,
			em_makerBtn,
			em_uionmakerBtn,
			em_repeatBetBtn,
			em_cancelRepeatBetBtn,
			em_toolbarNextOutLight,
			em_zhuangTag,
			em_niuniuNote
		};
	
	public:
        GameTableUIV88();
        ~GameTableUIV88();
        
        bool init(BYTE bDeskIndex, bool bAutoCreate);
    public:
        static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);
        
        GameTableLogic*			_GameTableLogic;
		BYTE					_bDeskIndex;
		bool					_bAutoCreate;
    public:
        virtual bool onTouchBegan(Touch* touch, Event* event) override;
        virtual void onEnter() override;
        virtual void onExit() override;
        
    private:
        float						_fScaleX;
        float						_fScaleY;
        Size						_tableSize;
        
        //桌子
        Sprite*						_tableBg;
        Sprite*						_tableBgRight;
        
        Sprite*						_toolbarBg;
//        ImageView*                    m_ClockBg;
//        LabelAtlas*                    m_ClockTimeLabel;            //时钟label
        
        ImageView*					m_StageImage;				//阶段图标
//        ImageView*                    tongbiNiuBgImg;            //阶段图标
		Sprite*						m_AnimateImage;				//动画图片
		Node*						m_CsbNode;                  
        
        Button*						m_PrepareBtn;				//准备btn
        Button*						m_CatchBankerBtn;			//叫庄btn
        Button*						m_NotCatchBtn;				//不叫btn
        Button*						m_OpenCardBtn;				//开牌btn
        
        vector<CPlayerHeadV88*>		m_vPlayerHead;				//用户头像
        vector<ImageView*>			m_vPrepareImage;			//准备图标
        vector<ImageView*>			m_vOkImage;					//完成标志
        vector<Button*>				m_vBetBtn;					//投注按钮
        vector<Button*>				m_vBetValueBtn;				//所有玩家投注值
        vector<vector<PokerCardV88*>>	m_vPlayerPoker;				//所有玩家poker
        vector<ImageView*>			m_vPlayerPokerPointImage;	//所有玩家牛图片
		vector<Node*>			    m_vResultEffect;	//所有玩家牛图特效
		vector<ImageView*>			m_vPlayerPokerPointImageBg;	//所有玩家牛图片背景
        vector<Button*>				m_vPlayerResultValueBtn;	//所有结果值

		vector<vector<ImageView*>>	m_vPoolInfoImage;			//奖池信息图片
		vector<vector<Label*>>		m_vPoolLabel;				//奖池label
		list<tag_s2c_reward_get_record> m_PoolRecord;		//中奖记录

		GameNotice*					m_Notice;				//广播
		tag_s2c_catch_banker*      catchBankerMsg;
		int  playScannnerT;


		// toolbar
		vector<Point>								_vecToolbarPoints;
		vector<Point>								_vecToolbarPoints2;
		vector<Button*>							_vecMenuButtonKids;
		Size												toolBar_Size;
		bool												toolBar_flag;
		Node*											center;
		Sprite*						effctImg;				   //动画图片
		vector<int>                  readyCount;
		//闹钟对象
		TimerObject* _clockObj;
		Label* m_NickLabel;

    private:
        
        BYTE						m_bPrepareTime;				//准备时间
        BYTE						m_bCatchBankerTime;			//抢庄时间
        BYTE						m_bBetTime;					//下注时间
        BYTE						m_bSendCardTime;			//发牌时间
        BYTE						m_bCardTypeMul[11];			//0-10代表无牛-牛牛的倍率
        
//        float                        m_fClockTime;
        float						m_fTimeCount;
        BYTE						m_bTimerChairID;			//定时器椅子号
        int							m_iTimerID;					//定时器id
        bool						m_bDoTimeOutCallback;		//是否做超时处理
        int							m_iSendRoundNum;			//单张发轮数,为5轮
        int							m_iSendViewID;				//发方位
        int                          m_iLastViewID;             //最后方位
        
        int							m_iPlayerCount;
        int							m_GameStatus;
        bool						m_bGameEnd;					//记录一局结束
		bool						m_bAuto;					//是否托管
		bool						m_bNextOut;					//下局离开
		bool						m_bHasCowRun;				//有五花牛动画
        vector<BYTE>				m_vPlayerPrepare;
        BYTE						m_bCurUser;					//当前操作者
        BYTE						m_bNextUser;				//下次操作者
        BYTE						m_bBankerID;				//庄家
        
        long long					m_i64BetArr[4];				//押注值
        vector<long long>			m_vPlayerBetValue;			//所有玩家押注值
        vector<vector<BYTE>>		m_vPlayerPokerValue;		//所有玩家扑克值
        vector<BYTE>				m_vPlayerPokerPoint;		//所有玩家扑克点数
		long long					m_i64PlayerResultValue[6];	//目前就给四人用
		vector<int>					m_vPlayerState;				//玩家状态
	
        bool                        m_isMyNN;                   //自己拿到牛牛并且赢钱时
        long long m_i64Jackpot;
        std::string m_backGroundMusicPath;
    public:
        //侧边栏
        void createToolBarUI();
        void createPlayerHeadUI(int iPlayerCount);
		vector<Vec2> getPlayerHeadPos();
        void resetPlayerHeadUI();
        void createClock();
        void createReadyUI();
        Button* createNum(int iType);
        void createBetBtn();
        void createBetValue(BYTE bViewChairID);
        void createPoker(int iPlayerCount);
		vector<Vec2> getPokerPointPos(int iPlayerCount);
        void createPokerPointImage(int iPlayerCount);
        void createResultValue(BYTE bViewChairID, long long i64Money);
        
		void createRewardPool();
        //void onToolBarClick(Ref* pSender, Widget::TouchEventType type);

		// 控制左上角控制按钮 点击状态
		void toolBarFlag(float dt);
        void updateJackpot(float dt);
    public:
        
        virtual void leaveTheGame() override;
        //进入或者短线重连恢复游戏桌子场景
        virtual void resumeTheCatchBaker(tag_s2c_station_catch_banker* pMsg, BYTE bGameStation) override;
        virtual void resumeTheGameBet(tag_s2c_station_bet* pMsg, BYTE bGameStation) override;
        virtual void resumeTheGameSendCard(tag_s2c_station_send* pMsg, BYTE bGameStation) override;
		
        // 红包出现
        virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
        // 打开红包结果
        virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
        
//        void onBtnClick(Ref* pSender,Control::EventType event);
		
    private:
        void setGamePlayerCount();
        
        //时钟方法
//        void updateClock(float dt);
//        void setTimer(BYTE bChairID, int iTimerID, float fTime);
        void killTimer();
        void timeOutCallback(BYTE bChairID, int iTimerID);		//超时处理
        
        vector<vector<Vec2>> getCardPosVector(int iPlayerCount);
        void sendCard();
        void sendCardWithNoAction();
        void sendCardTimer(float dt);
		void showOpenCard(BYTE bChairID, BYTE bPoint);
        
        Vec2 getStateTipPos(int iPlayerCount, BYTE bDeskStation);

		//void showTimeTip(int iTimerID, bool bShow = true);
		Text*	m_TextTip;
		long long    m_i64TipMoney;
		void updateTip(float dt);

		void setBtnTouch(Button* btn, bool bTouch);
		void showOpenBtn(float dt);
		tag_s2c_tip_exit*	tipMsg;
		void tipCallback();
		void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);
		void resetUI();
	private:
		void setCoinMove();
		void showCoinMove(int chairID, long long goldNum);
		void showGoldRain();
		void showCowRun(long long wuhuaMoney);

		int m_iBankerViewID;
		void showBankerAnimation(Vec2 pos);

		typedef std::function<void()> ShowBankerCallback;
		ShowBankerCallback showBankerCallback;
    private:
        //按钮回调
        void prepareClickCallback(Ref* pSender, Widget::TouchEventType type);
        void catchBankerClickCallback(Ref* pSender, Widget::TouchEventType type);
        void betMoneyClickCallback(Ref* pSender, Widget::TouchEventType type);
        void openCardClickCallback(Ref* pSender, Widget::TouchEventType type);
		void autoClickback(Ref* pSender, Widget::TouchEventType type);
		void poolBtnClickCallback(Ref* pSender, Widget::TouchEventType type);
//		void toolBarClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
    private:
        //消息回调
        
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        //添加用户
        virtual void addUser(BYTE seatNo, bool bMe) override;

        //配置包
        virtual void onGameConfig(tag_s2c_config* msg) override;
        //准备
        virtual void onGamePrepare(tag_s2c_prepare* msg) override;
        //抢庄
        virtual void onGameCatchBanker(tag_s2c_catch_banker* msg) override;
        //确定庄家
        virtual void onGameConfirmBanker(tag_s2c_confirm_banker_two* msg) override;
        //押注消息
        virtual void onGameBet(tag_s2c_bet_two* msg) override;
        //发牌
        virtual void onGameSendCard(tag_s2c_send_card* msg)	override;
        //ok
        virtual void onGameOpenOk(tag_s2c_open_card_ok* msg) override;
        //开牌
        virtual void onGameOpenCard(tag_s2c_open_card* msg) override;
        //结算
        virtual void onGameResult(tag_s2c_result* msg) override;
		//提醒退出
		virtual void onGameTipExit(tag_s2c_tip_exit* msg) override;
		//通知
		virtual void onGameNotify(tag_s2c_notify* msg) override;
		//奖池总额
		virtual void onGamePoolMoney(tag_s2c_reward_pool* msg) override;
		//获奖记录
		virtual void onGameRewardGetRecord(tag_s2c_reward_get_record* msg) override;
		//个人贡献
		virtual void onGamePersonalContribution(tag_s2c_personal_contribution* msg) override;
		//自己状态
		virtual void onGameUpdatPlayerState(tag_s2c_player_state* msg) override;
        //////
		virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
        
        void resetNewGame();
        void resetData();
		//void onGameCatchBankerAfter(tag_s2c_catch_banker* msg);
    };
}

#endif // __BRNN_GameTable_BRNNTableUI_h__
