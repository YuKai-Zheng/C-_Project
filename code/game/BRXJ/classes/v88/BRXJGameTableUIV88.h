#ifndef  __RBXJ_GAMETABLEUIV88_H__
#define __RBXJ_GAMETABLEUIV88_H__

#include "ui/CocosGUI.h"
#include "platform/CCCommon.h"
#include "cocostudio/CocoStudio.h"
#include "GUI/CCControlExtension/CCControlButton.h"

#include "HNNetExport.h"
#include "BRXJBetV88.h"
#include "BRXJPockerV88.h"
//#include "BRXJClockV88.h"
#include "BRXJLuZhiUiV88.h"
#include "BRXJGameLogic.h"
#include "BRXJGameTableUICallback.h"
#include "BJLGameTableUIData.h"
//#include "BJLPokerCard.h"
#include "../v88/BJLResultCardV88.h"

#include "HNLobby/GameNotice.h"
#include "UI/Base/AUIButton.h"
#include "UI/TimerObject.h"

using namespace cocostudio;
using namespace ui;
using namespace std;
using namespace extension;


#define  CARD_INTERVAL   29
#define	 NUMOFBETAREA	 3
#define  CHOUMANUM		 5

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

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace BRXJ {

class GameTableUIV88 : public HNGameUIBase, public GameTableUICallback {
public:
    // 创建
    static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

    // 初始化
    bool init(BYTE bDeskIndex, bool bAutoCreate);

    // 推出
    virtual void onExit() override;

	//进入
    virtual void onEnter() override;

    // 构造
	GameTableUIV88();

    // 析构
	virtual ~GameTableUIV88();

private:

    void initUI();
    //庄家UI
    void initBankUI();
   //按钮空间
    void initBottomUI();
    //左上角下拉框
    void createToolBarUI();
    //下注筹码按钮
    void initBet();
    //路子UI
    void initLuZhi();
    //计时器
    void initClock();
    //庄家列表
    void initNTList();

    void showToolBar(bool flag);
	//获取区域筹码数据
    void getAreaBetPixData();
private:
    //离开游戏
    virtual void levelGame() override;
    //下局离开游戏
    virtual void nextLevelGame() override;
    //显示提示信息
    virtual void IShowNoticeMessage(const std::string& message) override;
    //更新庄家列表
    void IUpdateNTList();
    //设置庄家
    void IApplyNT(bool isNT);
    //更新玩家信息
    virtual void upDataPlayerInfo(std::string name, LLONG money, LLONG totle);
    virtual void upDataPlayerInfo(LLONG money, LLONG totle) override;
    //更新庄家信息
    virtual void upDataNTInfo(int userid,int logoid ,std::string name, LLONG money, LLONG totle, int zhuangCount) override;
    virtual void upDataNTInfo(LLONG money, LLONG totle) override;
    //隐藏庄家信息
    virtual void hideNtInfo(bool flag) override;
    /*-------分隔----------*/
    //计时器开始
    virtual void starTime(int time) override;
    //计时器结束
    virtual void killTime() override;
    //游戏状态
    virtual void gameStation(BYTE station) override;
    //清理桌面
    virtual void clearTable(bool flag) override;
    //发牌
    virtual void ISendCard(bool animate) override;
    //更新区域总筹码
    virtual void upDataAreaTotleMoney(LLONG* quyuzhu) override;
    //更新区域金币
    virtual void upDataAreaMoney(LLONG*quyuzhu) override;
    //胜利区域显示
    virtual void showWinArea(BYTE idx) override;
    //初始化卡牌值
    virtual void initCardValue(BYTE cardValus, int turn, int idx) override;
    //筹码动画
    virtual void chipAnimation(bool flag, int type, int area) override;
    //全部动画
    virtual void allAnimation(bool flag) override;
    //更新路子信息
    virtual void upDataLuzi(TGameLuzi *data) override;
    //灰化效果
    virtual void grayEffectBet(bool flag) override;
    //显示牌型
    virtual void showCardShap(int area, int count) override;
    //
    virtual void recoveryCards(BYTE value) override;
    //重复下注
    virtual void repeatNote() override;
    //不放音效
    virtual void playEffectOnStation() override;
    void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);
    virtual void onGameNotify(NN::tag_s2c_notify* msg) override;
    //游戏消息提示
    virtual void onGameTipExit(tag_s2c_tip_exit* msg) override;
    //重复下注开关
    virtual void openRepeatNote(bool flag) override;
    
    // 红包出现
    virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
    // 打开红包结果
    virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
private:
    //分发牌
    void dispatchcard(float dt);
	//显示结算
    void showSettlementDialog(S_C_GameResult* pData);
	//广播
    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
private:

    //按钮回调
    void onBtnClick(Ref* pSender, Control::EventType event);
  //功能菜单
    void onMenuBtnCallBack(Ref* pShender, Widget::TouchEventType  type);
	//筹码
    void onNoteAreaCallBack(Ref* pShender, Widget::TouchEventType  type);

    void  onTouchEnded(Touch* touch, Event* event);
	//上庄
    void onApplyNTClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);
	//重复下注
    void onRepeatBetClickCallback(cocos2d::Ref* pSender, Widget::TouchEventType touchtype);

    // «ø÷∆ÕÀ≥ˆ
    void tipCallback();
    void updateTip(float dt);

    void girlAnimation(float dt);

private:

    /*----------------------------  UI -----------------------------------------*/
    Node*							m_root;						//根节点
    BetV88*								m_BetRoot;				//筹码根节点
    LuZhiV88*							m_LuZhi;					//路子
    TimerObject*							m_Clock;					//计时器
    BJL::T_DEALER_LIST*      _dealerList;				//
    BJL::ResultCardV88*				_resultCard;
    GameNotice*					m_Notice;
    tag_s2c_tip_exit*			tipMsg;
    GameTableLogic*		   _logic;						//逻辑类
    cocostudio::timeline::ActionTimeline* _antAction;
    //上庄列表
    struct NTLIST {
        ImageView*		ImageView_ListBg;				//bg
        ListView*			ListView_dealerList;			//LIST
        ImageView*		Image_item;
        Button*				Button_ApplyDown;			//申请上庄
    } _ntList;
    //桌面
    struct TABLEUI {
        Layout*		Panel;   //

		Text*			Text_gameStation;							//”Œœ∑◊¥Ã¨Ã·–—
		Sprite*		Sprite_gameStation;							//”Œœ∑◊¥Ã¨Ã·–—

		Text*			AtlasLabel_top;						//ÃÏ√≈Ω±“
		Text*			AtlasLabel_bottom;				//œ¬√≈—π◊¢◊‹Ω±“
		Text*			AtlasLabel_sky;						//…œ√≈—π◊¢◊‹Ω±“
        TextAtlas*			AtlasLabel_topTotle;			    //…œ√≈Ω±“
        TextAtlas*			AtlasLabel_skyTotle;				//œ¬√≈Ω±“
        TextAtlas*			AtlasLabel_bottomTotle;			//ÃÏ√≈—π◊¢◊‹Ω±“

        Sprite*		Sprite_top;								//闪烁区域
        Sprite*		Sprite_sky;								//闪烁区域
        Sprite*		Sprite_bottom;						//闪烁区域
        Sprite*		Sprite_bankPrompt;				//闪烁区域

        ImageView* xiazhu0;								//…œ
        ImageView* xiazhu1;								//ÃÏ
        ImageView* xiazhu2;								//µÿ

        Sprite*		puke;									//∑¢≈∆∆˜∆ÀøÀ
        Sprite*		girl;										//∑¢≈∆∆˜∆ÀøÀ

        vector<ImageView*>			_vecImageBg;
        Sprite*		Sprite_fapaiqi;						//œ÷º€LOGO

    } _tableUi;
    //◊Øº“
    struct BANKER {
        Layout*			panel;

        Text*				Text_leiJiMoney;					//¿€º∆Ω«Æ
        Text*				Text_zhuangCount;				//¡¨◊Ø¥Œ ˝
        Text*				Text_money;						//◊Øº“Ω±“
        Text*				Text_name;						//◊Øº“Í«≥∆

        Sprite*				Sprite_head;					//◊Øº“Õ∑œÒ
    } _banker;
    //µ◊≤ø
    struct BOTTOMUI {
        Layout*			Panel;   //

        Text*				Text_name;						//◊‘º∫√˚◊÷
        Text*				Text_money;						//◊‘º∫Ω±“
        Text*				Text_leiJi;							//¿€º∆ ‰”Æ

        AUIButton*	Button_regist;					//…Í«Î…œ◊Ø
        AUIButton*	Button_repeat;					//÷ÿ∏¥—π◊¢
        AUIButton*	Button_repeatNo;				//»°œ˚÷ÿ∏¥—π◊¢
        AUIButton*	Button_ZhuangList;				//…œ◊Ø¡–±Ì

        Sprite*			name_bg;
        Sprite*			money_bg;
		Sprite*			bg;						//底框

        Point				bet_pos;
        Point				luzi_pos;
        Point				repeat_pos;
        Point				register_pos;
    } _bottomUi;

    bool										bShow;
    bool										m_bAutoBet;
    bool										m_bNextOut;
    bool										m_isNote;

    ImageView*							_tableBg;
    Sprite*									_toolbarBg;
    vector<Vec2>						_vecCardPos;						//ø®≈∆∑…≥ˆƒø±Íµ„s
    vector<ImageView*>				_vecNoteArea;
    vector<Image*>					m_vImage;
    vector<unsigned char*>			_vecNoteAreaData;			//œ¬◊¢ ˝æ›
    vector<Text*>				_vecAreaMoney;				//«¯”Ú—π◊¢
    vector<TextAtlas*>				_vecAreaTotleMoney;			//◊‹—π◊¢
    vector<Sprite*>						_vecPromptWin;					//«¯”ÚÃ· æøÚ
    vector<PockerV88*>					_vecPockers;						//∆ÀøÀ
    vector<Sprite*>						_vecChips;							//≥Ô¬Î
    vector<TextAtlas*>				_vecTextAlts;						//µ„ ˝Œƒ±æ
    vector<Sprite*>						_vecDianSprs;					//µ„æ´¡È
    vector<Sprite*>						_vecBaoZiSprs;					//µ„æ´¡È
    vector<Layout*>					_vecChipsArea;					//≥Ô¬Î«¯”Ú  //0 …œ “ª¥ŒƒÊ ±’Î
    BYTE										_arrayCardValues[4][2];		//¥Ê¥¢≈∆÷µ
    int											_pockerIdx;						// ∑¢µΩƒƒ¿Ô
    int											_cardTurn;							// µ⁄º∏¬÷∑¢≈∆
    LLONG									_areaNoteSave[3];				//¥Ê≥…œ¬◊¢÷µ±æµÿ

    Text*	m_TextTip;
    long long    m_i64TipMoney;
    long long   m_i64WinMoney;


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

#endif //__RBXJ_GAMETABLEUIV99_H__
