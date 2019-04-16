﻿#ifndef CSLLGameTableUIV88_h
#define CSLLGameTableUIV88_h

#include <string>
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"
#include "CSLLGameTableUICallback.h"
#include "UI/Base/AUIButton.h"
//#include "../../TTNN/Classes/v88/BRNNClockV88.h"
#include "CSLLGameTableLogic.h"
//#include "CSLLGuessSizeV88.h"
#include "CSLLSettlementV88.h"
//#include "CSLLMaryTableUIV88.h"
#include "HNLobby/GameNotice.h"
#include "UI/RobotInfo.hpp"
#include "CSLLMary.hpp"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;
using namespace ui;

namespace CSLL {

	class GameTableLogic;
    
	class GameTableUIV88 :  public GameTableUICallback {

        struct Root_UI {
            ImageView*  bg;
            Layout   *  gamePanel;
			Layout   *  linesPanel;
            ImageView*  poolBg;
            Text*       poolText;
            
            Node*       middleNode;
            Button*     startBtn;
            Button*     autoBtn;
            Button*     cancelBtn;
            Button*     maxNote;
            
            Button*     addNote;
            Button*     subNote;
            Button*     addLine;
            Button*     subLine;
            Text*       myName;
            Text*       myMoney;
            Text*       score;
            Text*       jackpot;
            Text*       addLineCount;
            Text*       addNoteCount;
            Text*       NoteTotal;
            Node*       m_flower;
            Node*       m_startBtnEffect;
            ImageView*  m_freeGetTip;
            int         rootnFreeNum;
            Button*   stopBtn;
            Root_UI() {
                bg = nullptr;
                gamePanel = nullptr;
				linesPanel = nullptr;
                poolBg = nullptr;
                poolText = nullptr;
                middleNode = nullptr;
                startBtn = nullptr;
                autoBtn = nullptr;
                cancelBtn = nullptr;
                maxNote = nullptr;
                
                addNote = nullptr;
                subNote = nullptr;
                addLine = nullptr;
                subLine = nullptr;
                myName = nullptr;
                myMoney = nullptr;
                score = nullptr;
                jackpot = nullptr;
                addLineCount = nullptr;
                addNoteCount = nullptr;
                NoteTotal     = nullptr;
                
                m_flower = nullptr;
                m_startBtnEffect = nullptr;
                m_freeGetTip  = nullptr;
                rootnFreeNum  = 0;
                stopBtn = nullptr;
            }
            
            void EnableButtons(bool flag)
            {
                startBtn->setTouchEnabled(flag);
                startBtn->setBright(flag);
                
                m_startBtnEffect->setVisible(flag);
                
                if (flag && rootnFreeNum > 0)
                {
                    flag = false;
                }
                
                maxNote->setTouchEnabled(flag);
                maxNote->setBright(flag);
                
                addNote->setTouchEnabled(flag);
                addNote->setBright(flag);
                
                subNote->setTouchEnabled(flag);
                subNote->setBright(flag);
                
                addLine->setTouchEnabled(flag);
                addLine->setBright(flag);
                
                subLine->setTouchEnabled(flag);
                subLine->setBright(flag);
                
               
            }
        } _root;
        
        bool    m_bAuto;
        bool    m_bHasCashIn;
        bool    m_bAnimationFinished;   // 动画是否播放完成
        bool    m_bGameFinished;        // 游戏是否结束
        long long m_i64WinMoney;        // 当前局奖励
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
        
        //
        //消息同步
        virtual void leaveTheGame() override;
        //玩家坐下
        virtual void userSitDown(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        //玩家站起
        virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
        //添加用户
        virtual void addUser(BYTE seatNo, bool bMe) override;

        //
        //
        virtual void dealGameState(struct_s2c_game_state* msg) override;
        //
        virtual void dealGameBaseInfo(struct_s2c_base_info* msg) override;
        //
        virtual void dealGameBegin(struct_s2c_game_begin* msg) override;
        //
        virtual void dealGameFinish(struct_s2c_game_finish* msg) override;
        //提醒退出
        virtual void onGameTipExit(tag_s2c_tip_exit* msg) override;
        virtual void dealGameFreeCount(struct_s2c_free_count * msg) override;
        virtual void dealGameRobotInfo(struct_s2c_robot_info * msg) override;
        virtual void dealGameMaryInfo(struct_s2c_open_result * msg) override;
        virtual void dealGameMaryEndInfo(struct_s2c_mary_result * msg) override;
        void TestLuckyUI();
        void TestLuckyUICallBack(Ref* pSender, Widget::TouchEventType type);
        
        //
        void setNick(char* nick);
        //
		void updatePlayerInfo();
        
        //预加载结束回调
        virtual void closeLoadingCallBack() override;
        //数据
	public:
        long long           m_i64TipMoney;
//        int                 m_GameStatus;
		CSLLSettlementV88*    _gameSettlement;
        unsigned int        m_audioEffectID;
        
//        Node*               root;
        Text*               m_TextTip;
        
		long long		    i64Reward;		//当局奖金
		long long		    i64MyMoney;		//背包金币

		int 				nOpenLotteryType[LOTTERY_COUNT_CSLL];			//服务端发送的15个图标
        int                 nOpenLotteryTypeColRow[LOTTERY_COLUMN_COUNT][3];

        vector<Layout*>             m_vPanelTurnArea;
        vector<vector<ImageView*>>  m_vTurnIcon;
        vector<Vec2>                m_vTurnIconPos;
        vector<int>                 m_vTurnUnitCount;           //转多少个icon单元
        int                         m_nHasTurnUnitCount;        //已经转的单元
        float                       m_fTurnUnitTime;            //每个单元时间
        
        ImageView*              m_vAllLines[LINE_COUNT];        //九条线
        unsigned int            m_currentMaxLine;               //当前压的最大的线
        int                     lineResultNumData[LINE_COUNT];  //每条线的中奖水果，-1是不中奖
        int                     nOpenLinePrizeType[LINE_COUNT];         //每条线的奖品类型：倍率、免费次数、奖池比率
        int                     nOpenLineCount[LINE_COUNT];             //每条线连格的个数：2、3、4、5连     *N
        int                     nOpenLineRate[LINE_COUNT];              //每条线的得奖倍率（or免费次数or奖池比率） 倍数
        
        //基础信息同步
//        long long i64MyMoney;
        long long i64NoteUnit;                      //加注单位
        long long i64NoteMin;                       //每条线最少压注值
        long long i64NoteMax;                       //每条线最大压注值
        long long i64Jackpot;                       //当前奖池
        long long m_currentNoteValue;               //当前押注值
        
        int nLotteryTypeCount;                      //当前游戏的水果类型数
        int nLineConstitute[LINE_COUNT][5];
        vector<ImageView*> openLineVec;     //中奖线放在特效上
    private:
        void initData();
        void resetUINewGame();
        void resetData();
        
		//侧边栏
		void createToolBarUI();
		//大厅底节点
		void createRootLayer();
        //create turn
        void loadTurn();
        void loadTurnEx();
        //create settlement layer
        void createGameSettlementLayer();
//        //添加Loop
//        void addLoopSprites();
//        //筹码按钮区域
//        void createMoneyArea();
//        //筹码金额展示
//        void createNoteLabels();
//        //
//        void createPlayerInfon();

//        void showOpenBtn(float dt);
		tag_s2c_tip_exit*	tipMsg;
		void tipCallback();
		void updateTip(float dt);

	private:
		//按钮回调
		void addLineBtnCallBack(Ref* pSender, Widget::TouchEventType type);     //加线回调
		void addNoteBtnCallBack(Ref* pSender, Widget::TouchEventType type);     //加注回调
		void maxLineBtnCallBack(Ref* pSender, Widget::TouchEventType type);     //满线回调
        void nineLineBtnCallBack(Ref* pSender, Widget::TouchEventType type);    
		void maxNoteBtnCallBack(Ref* pSender, Widget::TouchEventType type);     //满注回调
        void autoBtnCallBack(Ref* pSender, Widget::TouchEventType type);        //自动回调
        void startBtnCallBack(Ref* pSender, Widget::TouchEventType type);       //开始回调
        void stopBtnCallBack(Ref* pSender, Widget::TouchEventType type);       //停止回调
        
        void turnUpdate(float dt);
		void turnUpdateEx(float dt);
        void setTurnIcomPosition(vector<Vec2> &vIconPos, Size parentSize);
        void setTurnUnitCount(vector<int> &vTurnUnitCount, vector<int> vCurBeginIcon, vector<int> vCurEndIcon, int nRoundMinNum);
        void checkTurnIsSuccess();
        void transformOpenLotteryType();
        void randomColumnDatasAndCalcTurnCount();
        void updateOpenLinesEffect();
        void updateSettlementLayer();
        void addLineLogic();
        void maxLineLogic();
        void updateSelectLines();
        void hideAllLines();
        virtual void updateMyMoney(long long money) override;
        
        void addNoteLogic();
        void maxNoteLogic();
        void updateSelectNotes();
        
        // 更新奖池
        void updateJackpot();
        virtual void update(float dt) override;
        
        void updateMyRealWinMoney(long long money);
        
        void showFlower(bool show);
        void showGetRewardEffect();
        void showMoneyFunc(long long winMoney);
        void checkHasReward(long long winMoney);
        void createYunDuo();
        void startYunDuoMove(ImageView* yunduo);
        
        void createAnimationRotate();
        void stopRotateAnimation(int nCol);
    private:
        long long i64WinJackpot;
        
        void showJackpot();
        void showFreeGetTip(int num);
        vector<std::string> getGameIconSpineName(int _typeId);
        Vec2 getGameIconSpinePos(int _typeId);
    private:
        vector<ImageView*> _resultImg;
        
        void cleanResults();
        vector<RobotInfo*> _robotVec;
        vector<Node*> _robotParentNodeVec;
        
        CSLLMary*     _maryLayer;
        int nMaryCount;
        int nFreeNum;
        
        vector<Sprite*> m_AnimationNodeVec;
	};
}

#endif // __BRNN_GameTable_BRNNTableUI_h__