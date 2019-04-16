#ifndef __HN_GoldenToad_GameTableUI_V88_H__
#define __HN_GoldenToad_GameTableUI_V88_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "GoldenToadGameTableLogic.h"
#include "GoldenToadGameTableUICallback.h"
#include "GoldenToadMessageHead.h"
#include "GoldenToadGamePlayerV88.h"
#include <vector>
#include "../../NN/Classes/GameMessage/NNCommonDef.h"
#include "HNLobby/GameNotice.h"
#include "extensions/cocos-ext.h"
USING_NS_CC;
using namespace extension;
using namespace cocos2d;
using namespace cocostudio;
using namespace std;
using namespace ui;

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace GoldenToad {
    
class GameFishManager;
class SceneFishTrace;
class GoldenToadToolbarV88;
class GoldenToadGameAudio;

class GameTableUIV88 : public HN::HNGameUIBase, public GameTableUICallback {

    struct catchResultInfo {
        LLONG userGoldMoney;							//玩家当前捕鱼获得的金币
        INT numberOfKindFIsh[FISH_KIND_COUNT];			//玩家当前每种鱼捕到的数量
    };

//    constexpr static const FLOAT		PI					=	3.14159f;
//    static const INT		BACKGROUND_ZORDER	=	0;
//    static const INT		FISH_ZORDER			=	10;
//    static const INT		NOTICE_ZORDER		=	50;
//    static const INT  		BULLET_ZORDER		=	15;
//    static const INT		WATER_ZORDER		=	9;
//    static const INT		PLAYER_ZORDER		=	210;
//    static const INT		UI_ZORDER			=	215;//游戏中UI的层级设定
//    static const INT		Notify_ZORDER		=	220;//游戏中设置界面层级设定
//    static const INT		MAX_ZORDER			=	280;

    enum GT_TableBtnTag {
        gt_toolbarBtn = 100,
//        gt_toolbarChangeTableBtn,
        gt_toolbarSetBtn,
        gt_toolbarHelpBtn,
//        gt_toolbarStandupBtn,
        gt_toolbarLeaveBtn
//        gt_toolbarNextOutBtn
    };

protected:
    //工具栏
    GoldenToadToolbarV88*			_toolBar;

public:
    bool						_isLockFish;//是否为锁定状态 锁定状态会有自动切换鱼的逻辑

private:
    BYTE						_deskIndex;
    bool						_bAutoCreate;
    bool						_isAllowFire;
    bool						_isAutoFire;
    bool						_isReadyLock;
    bool						_isReadySwitchLock;

//    bool						_isShowMenu;
    //手动切换目标鱼
    bool						_switchTouchEnd;//true 默认值；false 切换检测开始，需要该值保持 false 0.7s 以上才算作切换成功的一个条件
    bool						_switchLockSuccess;
    bool						_switchFirstCheckFinish;
    bool						_switchSecondCheckFinish;
    bool						_switchingLockedFish;
    bool						_isSwitchScene;
    Sprite*						_switch_tip_sprite;
    INT							_switchLockFishID;
    long						_switchTimer;
    //手动切换目标鱼end
    INT							_iBullet_id;
    Vec2						_touchPoint;
    ImageView*					_currentBg;
    Layout*						_tableUi;
    Button*						_autoBtn;
//    Button*						_showHideBtn;
    Button*						_lockBtn;

    CC_SYNTHESIZE(INT,			_lockFishId, LockRishId);
    Vec2						_fpCircleFishCenter[2];	//环形鱼群中心点

    //设置等菜单
    Sprite*						_toolbarBg;

public:
    GoldenToadGameAudio*        _audioEngine;

private:
    GameFishManagerV88*			_fishManager;
    GameTableLogic*				_tableLogic;
    SceneFishTrace*				_fishTrace;
    GameNotice*					m_Notice;
//        Sprite*						_NoticeBG;
    GamePlayerV88*					_players[PLAY_COUNT];   //物理位置
    catchResultInfo*			_resultInfo;		//玩家捕鱼结果信息
    CMD_S_SwitchScene			_switchScene;
//    bool						_canTouchWhileTeaching;
    bool						_delaySwitchLock;	//touchBegan事件中启动延时更改这个值的状态，如果其状态被更改，则表明延时成功
    bool                        _showNotify;

private:
    float                        _ownOperateVerticalLine;    //用于限制自己点击区域，如果点击到了自己炮台后方，不允许射击，即炮台最大角度正前方180度。
    bool                        _isOwnOnTop;    //自己的位置在上面还是下面  true  上面
    std::string                 _bgMusicPath;

public:
    GameTableUIV88();
    virtual ~GameTableUIV88();
    virtual void onEnter() override;
    virtual void onExitTransitionDidStart() override;
    virtual void onExit() override;
    virtual void cleanup() override;
    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;

public:
    //手机返回键监听回调函数 added by HBC
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent) override;

    static GameTableUIV88* create(BYTE bDeskIndex, bool autoCreate);
    bool init(BYTE bDeskIndex, bool bAutoCreate);
    bool initGameUI();

    void initToolBarData();
    void asyncBuildData();
    void delayedLoadAction(float dt);
    void publicControlLockedFish(bool lock);

private:
    void createToolBarUI();
    void privateControlLockFishFunc(bool lock);

//    void onBtnClick(Ref* pSender,Control::EventType event);

private:
    void privateControlAutoFire(bool start);
    void privateResetSwitchFishID();
//    void privateShowToolBarTips(bool isShow);//独立封装的判断是否显示上分提示的方法，仅在积分不足单次炮弹时才出现上分提示

private:
    void showNotice(const std::string& message, int nCount, int nType);

public:
    virtual void addUser(BYTE seatNo, bool bMe) override;									//添加玩家
    virtual void removeUser(BYTE seatNo, bool bMe) override;								//移除玩家
    virtual void dealSetUserMoney(BYTE seatNo, LLONG value) override;						//设置显示玩家积分
    virtual void dealSetUserCoin(BYTE seatNo, LLONG value) override;						//设置显示玩家金币
    virtual void dealSetUserCannonNum(BYTE seatNo, INT value) override;						//设置玩家炮火威力
    virtual void dealSetGameConfig(CMD_S_GameConfig* gConfig) override;						//设置子弹速度等参数
    virtual void dealSetSceneStyle(SceneStyle Scene) override;								//游戏场景id
    virtual void dealGameFire(CMD_S_UserFire* pFire, bool bMeFire) override;				//收到服务端开火消息
    virtual void dealGameFireWhenLessScore(CMD_S_UserFire* pFire, bool bMeFire) override;	//当金币不足时，仍然要处理部分射击的逻辑
    virtual void dealCatchFish(CMD_S_CatchFish*	pFish) override;							//捕到鱼消息
    virtual void dealAddFishInfo(void* object, WORD objectSize) override;					//添加单鱼信息（长度可能为多条鱼）
    virtual void dealAddGroupFishInfo(CMD_S_GroupFishTrace* pFish) override;				//添加鱼群信息
    virtual void dealCatchSweepFish(CMD_S_CatchSweepFish* pSweep) override;					//抓捕到特殊鱼（鱼王，炸弹）通知
    virtual void dealCatchSweepFishResult(CMD_S_CatchSweepFishResult* pResult) override;	//抓捕到特殊鱼产生的结果（满屏死亡或同类死亡）
    virtual void dealCatchSweepFishResultEx(CMD_S_CatchSweepFishResultEx* pResult) override;//抓到大鱼系统广播通知
    virtual void dealSwitchScene(CMD_S_SwitchScene* pScene) override;						//切换场景
    virtual void dealLockFishTimeOut() override;											//被定海神针定住的鱼恢复运动
    virtual void dealBulletTimeOut(BYTE seatNo) override;									//取消能量炮
//    virtual void dealExchangeFishCoinResult(bool isIncrease, bool isSuccess) override;		//提示玩家上、下分操作结果
//    virtual void dealShowScoreUpTips(bool isIncrease, bool isSuccess) override;				//提示是否显示上分提醒
    virtual void dealUserChangeCoin(CMD_S_ChangeCoin* changeCoin) override;
    virtual void dealUpdateAllowFish(CMD_COM_ALLOW_LOCK_FISH* allowFish) override;			//更新服务端下发的可以锁定的鱼的数组

    //五花牛全服公告
    virtual void dealGameNotifyWuHuaNiu(NN::tag_s2c_notify* msg) override;

    ////////////////////////////////////////////////////////////////////
//    //超端
//    ////////////////////////////////////////////////////////////////////
//    virtual void dealSetAdminConfig(bool bAdmin)  override;									//是否超端用户
//    virtual void dealStockOperateResult(CMD_S_StockOperateResult* pStock)  override;		//奖池操作结果
//    virtual void dealUpdateOnlinePlayerList(std::vector<OnLineUserInfo> users) override;	//更新在线玩家列表
//    virtual void dealUpdateContorlList(std::vector<ControlInfo> controls) override;			//更新控制列表信息
//    virtual void dealUpdateSpecialList(std::vector<SpecialUser> specials) override;			//更新特殊鱼控制列表信息
//    virtual void dealSetSwitchInfo(S_C_SWITCH_INFO* pSwitch) override;						//控制开关
//    virtual void dealUpdateFishName(S_C_FISH_NAME* pName) override;							//更新控制鱼的名字

    //取消上分时重置玩家积分、金币
    virtual void resetUserMoneyAndScore(long long money, long long score) override;

    ////////////////////////////////////////////////////////////////////

public:
    virtual void dealGameStart() override;
    virtual void dealLeaveDesk() override;
    virtual void onDisconnect() override;
    void returnPlatform();

public:
    //发送子弹碰到鱼消息
    void sendHitFish(BYTE bSeatNo, INT fishId , INT bulletId , BulletKind bulletKind , INT  bulletMuriple);
    //发送加减金币的消息
    void sendChangeCoin(BYTE bSeatNo, bool isAdd);
    //播放抓到鱼后奖励特效
    void playAwardFishAnimation(BYTE bSeatNo, LLONG money, LLONG userMoney, Fish* fish);
    //切换场景动画
    void switchScene();
    //播放海草贝壳动画
    void playSeaweedAnimation(SceneStyle style);
    //创建动画
    Animate* buildAnimation(string name, int num, int loops, float speed);

private:
    //结算退出
    void gameSettlementAndExit();
//    //设置界面
//    void gameSettingWindow();
    //弹出结算界面
    void openExitWindow();
    //update players' position stations
//    void updatePlayerPosition(bool isUp);
    //update player's position and tool bar station
//    void updateToolBarStationAndPlayerSPosition(bool forceShow);
//    void showOrHideToolBarStationAndPlayerSPositionAuto(bool autoHide);
    void shoeNoticeWhenLessScoreThenNeed();
//    void timerAutoHideToolBarStation();
    void nofityLockFailUI();

private:
    //点击事件
    virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
    virtual void onTouchMoved(Touch *touch, Event *unused_event) override;
    virtual void onTouchEnded(Touch *touch, Event *unused_event) override;
    void resetValueForNextTouch();
    //自动射击
    void setAtuoFire(float dt);
    //触摸屏幕射击
    void openFireByTouch(const Vec2& touchPoint);
    //超时不开火自动退出
    void gameOutTime(bool bShow);

    //手动切换锁定目标鱼
    bool switchTargetFishBegan(Touch* touch);
    bool switchTargetFishFirstCheck(Touch* touch);
    bool switchTargetFishSecondCheck(Touch* touch);
    bool switchTargetFishEnd();
    void showSwitchLockTips(Vec2 pos);
    void updateSwitchLockTips(Vec2 pos);
    void hideSwitchLockTips();
    //手动切换锁定目标鱼end

private:
    void playBigWinAnimation(int gameID, std::string fishName, LLONG money);
    
private:
    //计时器
    //void timing(Node* node, int dt, std::function<void()> callFunc);
    //结算界面计时器
    void timingSettlement(Node* node, int dt, std::function<void()> callFunc);
    void timingSettlementEx(Node* node, int dt, std::function<void()> callFunc);

private:
    bool isValidSeat(BYTE seatNo);

private:
    //显示或隐藏聊天界面
    void showOrHideGameChat(bool bShow);
    //聊天
    virtual void showUserChatMsg(BYTE seatNo, CHAR msg[]) override;
    //显示通知信息
    void showNoticeMessage(const std::string& message);

    
    // 红包出现
    virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg);
    // 打开红包结果
    virtual void onLuckyMoneyResult(LuckyMoneyResult* msg);
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

private:
    //声音控制定时器
    void scheduleUpdatePlayEffectTag(float dt);

};
}


#endif // __HN_GoldenToad_GameTableUI_H__
