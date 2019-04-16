#ifndef __D21_Game_Table_UIV99_H__
#define __D21_Game_Table_UIV99_H__

#include "D21GameTableUICallback.h"
#include "D21GamePlayerV99.h"
#include "D21PokerCardV99.h"

#include "HNUIExport.h"
#include "HNNetExport.h"
#include "HNGame\HNGameUIBase.h"

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "GUI\CCControlExtension\CCControl.h"

#include <vector>
//引用百人牛牛
//#include "BRNNClockV99.h"
//#include "BRNNResV99.h"

#include "UI/Base/AUIButton.h"

using namespace cocos2d::ui;
using namespace cocos2d::extension;

namespace D21
{
	class GameTableLogic;
	class ClockV99;

	class GameTableUIV99 : public HN::HNGameUIBase, public GameTableUICallback
	{
		enum {
			em_toolbarBtn = 1000,
			em_toolbarSetBtn,
			em_toolbarHelpBtn,
			em_toolbarLeaveBtn,
			em_toolbarNextOutBtn,
			em_toolbarNextOutLight
		};

	public:

		static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);
		virtual bool init(BYTE bDeskIndex, bool bAutoCreate);
		virtual void onEnter();
		virtual void onExit();

	private:

		GameTableUIV99();
		virtual ~GameTableUIV99();
	public:
		//*******创建
		//初始化桌面
		void createTableUI();
		//创建牌相关
		void createCard();
		//保险界面
		void createInsurance();
		//初始左上角下拉框
		void createToolBarUI();
		//右上角计分板
		void createRecordingBoard();
		//初始化数据
		void initParams();
	public:
		//*********牌相关
		//牌位置(转化坐标)
		Vec2 getCardPos(BYTE seatNo, int cardIndex, int iGroupIndex);
		//发牌定时器
		void beginSendCard();
		//分发牌
		void displayCard(float delta);
		//添加手牌
		void addHandCardList(BYTE seatNo, BYTE value, bool bSecGroup = false);
		//分牌动作
		void divideCardAnimation(BYTE seatNo, BYTE card[]);
		//计算点数
		void calculatePoint(BYTE seatNo, BYTE value, int iGroupIndex);
		//点数显示
		bool isPointVisible(BYTE seatNo, int iGroupIndex);
		//显示牌点数
		void showPoint(BYTE seatNo, int iGroupIndex, bool bShow);
	private:
		//倒计时
		BYTE _timerID;
		BYTE _timerSeatNo;
		void stopTimer();
		void starTimer(BYTE timeID, float dt, BYTE seateNo = INVALID_BYTE);

	public:
		//抢庄按钮显示
		void setCatchBankerBtnVisible(bool bVisible);
		//下注按钮显示
		void setBetBtnVisible(bool bVisible, bool bTouch = true);
		//游戏按钮显示
		void setOperateBtnVisibleState(bool open);
		void setOperateBtnVisibleState(bool pdouble, bool pStopCard, bool watCard, bool pBranchCard);

		void showToolBar(bool bShow);


		//座位号是否合理
		bool seatNoIsOk(BYTE seatNo);
		
		//筹码动画
		void playChipAnimation(BYTE seatNo, BYTE chipType);

	public:
		//*******回调
		void onBtnClick(Ref* pSender, Control::EventType event);
		//下拉框显示
		void hintClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		//点击事件
		bool onTouchBegan(Touch* touch, Event* event);

		//开始按钮回调
		void btnStartCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		//操作
		void controllBtnClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		//保险
		void insuranceCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
		//下注
		void betBtnCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
	public:
		//*********处理消息
		//抢庄
		void dealCatchBankerMsg(GameMessageCatch* msg) override;
		//确定庄家
		void dealConfirmBankerMsg(GameMessageNt* msg) override;
		//处理下注
		void dealBetMsg(GameMessageBetResult* msg) override;
		//发前两张
		void dealStartSendCardMsg(GameMessageSendCard* msg) override;
		//购买保险
		void dealBuyInsuranceMsg(GameMessageInsurance* msg) override;
		//操作通知
		void dealOperateNotifyMsg(GameMessageOperateNotify* msg) override;
		//处理分牌
		void dealDivideResultMsg(GameMessageDivideResult* msg) override;

		//创建下注按钮
		void createBetButton(long long i64BetUnit[4]) override;
		//显示玩家准备
		virtual void showUserReady(BYTE seatNo, bool visible);
		virtual void showUserReady(BYTE seatNo, bool isMe, bool visible){};
		//设置玩家准备
		virtual void showBtnStart(BYTE seatNo);
		//设置庄家
		virtual void setNt(BYTE seatNo, bool IsMe);
		//添加玩家
		virtual void addUser(BYTE seatNo, bool bMe,LLONG userId);
		//更新玩家计分面板
		virtual void upDataRecordBoard(LLONG historyMoney, LLONG lastMoney, LLONG allPlayerMoney);
		virtual void upDataRecordBoard(LLONG allPlayerMoney);
		//添加结算面板
		virtual void openResultView(bool open);
		virtual void upDataResult(std::string name, LLONG money, LLONG cardShap, bool winrolose, int seatnum);
		//移除玩家
		virtual void removeUser(BYTE seatNo, bool bMe);
		//隐藏计时器
		virtual void hideClock(BYTE seatNo) {};
		//处理操作
		virtual void dealOperateResp(GameMessageNeedCard* msg);
		//开始操作
		void startOperate(BYTE seatNo, bool bCanDivide);
		//加倍
		void addDouble(BYTE seatNo, BYTE value, LLONG money);
		//游戏中弃牌结果
		void giveUpCard(BYTE seatNo);
		//要牌结果
		void needCard(BYTE seatNo, BYTE value, int points);
		//停牌结果
		void stopCard(BYTE seatNo);
		//设置下一个是自己叫牌时按钮状态
		virtual void setCallingbtnStatus(bool MeIsNt, bool bCanDivide);
		//隐藏庄家标志
		virtual void hideNtstates(BYTE seatNo);
		//提示信息
		virtual void setMsg(const std::string &msg);
		//离开桌子
		virtual void leaveDesk();
		//重置数据
		virtual void refreshData();
		virtual void setWaitTime(BYTE seatNo, BYTE time, bool visible){};

		//移除手牌
		void removeCards();
		private:

		GameTableLogic*		_tableLogic;
		bool							_bePlaying[PLAY_COUNT];					//区分玩家正在玩还是中途进来的
		BYTE							_NtSeatNo;										//庄家座位号
		
		LLONG						_betMax;											//下注最大限注筹码
		LLONG						_betMoney;										//下注的钱
		Sprite*						_betjeton[PLAY_COUNT];					//下注展示的筹码
		BYTE							_MySeatNo;										//玩家座位号

		
		BYTE							_GameStatus;										//游戏状态
		

		BYTE							_operateSeatNo;

		//右上计分板
		struct RECORD_BOARD
		{
			Text*											Text_last;									//上一句
			Text*											Text_history;								//总数
			Text*											Text_curTurnTotle;						//当钱玩家总下注
		}_recordBoard;

		//筹码按钮 与文本
		struct BETBTN_TEXT
		{
			Layout*										layout;

			Text*											Text_0;									
			Text*											Text_1;									
			Text*											Text_2;									
			Text*											Text_3;

			Button*										Button_0;
			Button*										Button_1;
			Button*										Button_2;
			Button*										Button_3;
		}_bet;

		//桌子
		struct TABLE
		{
			Node*										tableRoot;									//根节点
			Layout*										tableLayout;								//layout

			Sprite*										chipArea;									//筹码区域
			Sprite*										fapaiqi;										//fapaiqi
			Sprite*										card;											//cardOrign
		}_table;

		bool												_isOpenHelp;							//下拉框开关
		bool												m_bNextOut;								//下局退出开关
		Sprite*												_toolbarBg;								// toolbar
			
		vector<Sprite*>			_vecChips;									//筹码精灵


		//********UI
		//BRNN::ClockV99*			_clockObj;						//计时器
		ImageView*				_faPaiQiImg;					//发牌器

		ImageView*				_betBg;							//押注底图
		vector<AUIButton*>		_vecBetBtn;						//押注按钮
		vector<AUIButton*>		_vecOperateBtn;					//操作牌的btn
		AUIButton*				_readyBtn;						//准备按钮
		AUIButton*				_catchBankerBtn;				//叫庄
		AUIButton*				_cancelBankerBtn;				//不叫

		vector<Node*>			_vecPlayerNode;					//玩家节点
		PlayerUIV99*			_arrPlayer[PLAY_COUNT];			//玩家界面
		vector<vector<ImageView*>>	_vecPointBg;				//点数背景
		vector<vector<Text*>>	_vecPointText;					//点数文本
		vector<vector<ImageView*>>	_vecPointSpecial;			//特殊牌

		PokerCardV99*			_HandCardList[PLAY_COUNT][2][CARD_COUNT];	//手牌,最多为5
		//***********DATA
		//发牌
		int						_cardSendCount;					//记录发牌的张数(从0开始计数，所以发牌张数要加1)
		int						_cardIndex;						//当前发第几张
		int						_seatNum[8];			//发牌的座位号顺序
		BYTE					_CardValue[8];			//发牌对应座位号牌值的顺序

		int						_points[PLAY_COUNT][2];			//点数
		int						_HandCardCount[PLAY_COUNT][2];	//玩家手牌数量
		vector<Vec2>			_vecCardPanelPos;				//牌面板位置
		vector<vector<Vec2>>	_vecCardStartPos;				//各位置两组牌坐标

		BYTE _bDeskIndex;
		bool _bAutoCreate;
	};
}
#endif