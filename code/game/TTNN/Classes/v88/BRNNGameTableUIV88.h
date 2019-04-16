#ifndef __BRNN_GameTable_BRNNTableUIV88_h__
#define __BRNN_GameTable_BRNNTableUIV88_h__

/*
                2     3     4     5     6     7     8     9    10     J     Q
   K     A     E     F
                0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
   0x0B, 0x0C, 0x0D			//方块 2 - A
                0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
   0x1B, 0x1C, 0x1D,			//梅花 2 - A
                0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
   0x2B, 0x2C, 0x2D,			//红桃 2 - A
                0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
   0x3B, 0x3C, 0x3D,			//黑桃 2 - A
                0x4E, 0x4F
                小鬼  小鬼
 */
#include "BRNNBankerLayerV88.h"
//#include "BRNNChatDialogV88.h"
#include "BRNNGameTableUICallback.h"
//#include "BRNNHelpLayerV88.h"
#include "BRNNMessageHead.h"
#include "FontSize.h"
#include "HNLobby/ChildLayer/GameBankLayer.h"
#include "HNNetExport.h"
#include "UI/Base/AUIButton.h"

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include <string>

#include "HNLobby/GameNotice.h"
#include "UI/DropdownList.h"
#include "BaseMessage.h"
#include "BRNNOtherPlayerV88.h"
#include "UI/UserHead.h"
#include "UI/TimerObject.h"

using namespace cocos2d;
using namespace ui;
using namespace cocostudio;
using namespace cocos2d::extension;

#define IN_TABLE_FONTSIZE_V88 25
#define RECORD_LATEST_SHOW_NUM_V88 10 //记录输赢的次数
#define BET_ZONE_NUM_V88 4            //可投注区域数(东西南北四个方位)
#define MAX_NON_BANKER_SEAT_NUM_V88 12 //非庄家的最大座位数（庄家座位另算）
#define MAX_VERSION_SHOW_NUM 6 //当前版本显示椅子最大数（庄家座位另算）
#define SYS_BANKER_STATION_V88 190 //系统坐庄座位号
#define MAX_JETTON_TYPE_V88 5      //最多的筹码类型数目

#define TIAN_AREA_V88 0
#define DI_AREA_V88 1
#define XUAN_AREA_V88 2
#define HUANG_AREA_V88 3

struct LuckyMoneyNotify;
struct LuckyMoneyResult;

namespace BRNN {


class GameTableLogic;
class PlayerSeatV88;
//class ClockV88;
class PokerCardV88;
class BetObjV88;
class BetV88;

#define WinSize Director::getInstance()->getWinSize()

static int ChoumaValueV88[6] = {1000, 10000, 100000, 1000000, 5000000, 0};

class GameTableUIV88 : public HNGameUIBase, public GameTableUICallback {
public:
    static HNGameUIBase *create(BYTE bDeskIndex, bool bAutoCreate);

public:
    GameTableUIV88();
    ~GameTableUIV88();
    bool init(BYTE bDeskIndex, bool bAutoCreate);
    virtual bool onTouchBegan(Touch *touch, Event *event) override;
    virtual void onEnter()override;
    virtual void onExitTransitionDidStart()override;
    virtual void onExit() override;

public:
    void checkBetPosBeforeSendCard();
    void setBetEndPos(Vector<BetV88 *> &betList);
    void checkAllBeforeXiaZhu();
    void cleanData();
    void cleanCardAndCowAndMultipleSprite();
    void cleanMoneyLabel();
    void cleanBet();
    void checkCardAndCowBeforeSettlement();
    //是否有最近的投注操作
    bool hasLastBet();
    //换座引起的筹码目标位置变化
    // byStation:玩家本身的位置号
    // byLastSeatPosition:原来的位置
    // bySeatPosition:玩家要坐的位置
    void betChangeStartPos(BYTE byStation, BYTE byLastStation, BYTE seatNo = 255);

public:
    virtual void leaveTheGame() override;
    //进入或者断线重连 恢复游戏桌子场景
    virtual void resumeTheGameFree(TagGameState_WaiteAgee *pcene,
                                   BYTE gameStation) override;
    virtual void resumeTheGameBet(TagGameState_Bet *pcene,
                                  BYTE gameStation) override;
    void updateNonBankerSeatUI(BYTE bySeatPostion[12]);
    virtual void resumeTheGameOpenCard(TagGameState_OpenCard *pcene,
                                       BYTE gameStation) override;
    virtual void resumeTheGameResult(TagGameState_ShowResult *pcene,
                                     BYTE gameStation) override;

    //发送投注请求
    void sendBet(INT betArea, LLONG betVal, INT betType);

    //开始新一轮游戏
    virtual void startNewTurn(BEGIN_DATA *pcene) override;
    //显示下注额
    virtual void showBet(TagXiaZhu *pcene) override;
    //开牌
    virtual void showCard(KAI_PAI *pcene) override;
    //结算
    virtual void showSettlement(tagResult *pcene) override;
    ////换庄
    // virtual void changeDealer(void* object);
    //无庄等待
    virtual void waitForNoDealer(DENG_DAI *pcene) override;
    //空闲等待
    virtual void waitForIdle(DENG_DAI *pcene) override;
    //不能下注（可能下注达到上限）
    virtual void showUnableBet() override;
    //上下庄,独立上下庄，不发联合上庄信息
    virtual void getOrQuitBanker(SHANG_ZHUANG *pcene) override;
    //系统消息
    virtual void showSystemWords(char *talk) override;
    //玩家聊天
    virtual void showUserWords(MSG_GR_RS_NormalTalk *normalTalk) override;
    //赠送礼物
    virtual void giveGift(MSG_PROP_GIVE *pcene, int resultCode) override;
    //获取合作上庄的组合列表
    virtual void getUionBankerList(SHANG_ZHUANG_UION_LIST *pcene) override;
    //联合上庄上下庄
    virtual void uionBankerUpOrDown(SHANG_ZHUANG_UION *pcene) override;
    ////玩家站起
    // virtual void userUp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
    //玩家坐下
    virtual void userSitDown(MSG_GR_R_UserSit *userSit,
                             UserInfoStruct *user) override;
    //设置下注上限
    virtual void setLIMIT(LLONG value) override;
    //开始提示
    virtual void showStartMes() override;
    //换座
    virtual void changeSeat(BYTE byStation, BYTE byLastStation,
                            BYTE seatNo) override;
    //站起
    virtual void playerStandUp(BYTE byStation, BYTE seatNo) override;
    //提前发牌
    virtual void reachMaxBetLimit() override;
    ////获取庄家的座位号
    virtual int getBankerSeat() override {
        return m_pBeginData->iBankerSeatNo;
    }
    //获取连庄次数
    virtual Label *getRemainDealer() override {

        return _zhuangBout;
    }
    //等待变更庄家，再判断坐庄人
    virtual void judgeZhuangJiaMessage() override;

    virtual void onGameTipExit(S_C_TIP_EXIT *msg) override;

    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;

    //通知
    virtual void onGameNotify(NN::tag_s2c_notify* msg) override;
    
    // 红包出现
    virtual void onLuckyMoneyNotify(LuckyMoneyNotify* msg) override;
    // 打开红包结果
    virtual void onLuckyMoneyResult(LuckyMoneyResult* msg) override;
    
    GameTableLogic *_GameTableLogic;
    //我当前的投注值
    LLONG _myBetMoney;


private:

    BEGIN_DATA *m_pBeginData;
    //当前玩家信息
    UserInfoStruct *_ownInfo;
    //庄家信息
    UserInfoStruct *_bankerInfo;

	UserHead*		m_mySelf;

    //游戏状态：开牌、结算、空闲、下注
    int _gameStatus;
	bool _isNote;
    //银行
    GameBankLayer *_bankeLayer;
    //上庄申请列表
    vecTmpBankerInfoV88 _vecBankerList;
    //聊天记录列表
    //vecHNChatStructInfoV88 _vecChatList;
    //聊天框对象
    //HNChatDialogV88 *_chatDialog;
    //桌子背景资源
    Sprite *_tableBg;
    //桌子大小
    Size _tableSize;

    //庄家座位
    PlayerSeatV88 *_bankerSeatUI;
    //其他玩家座位
    Vector<PlayerSeatV88 *> _vecPlayerSeatUI;
    //连庄次数
    Label *_zhuangBout;
    //当前玩家的金钱数
    Label *_myMoneyLabel;
    //闹钟对象
    TimerObject* _clockObj;
    //当局状态提示
	Sprite*_gameStateSpr;
    //无座玩家按钮
    AUIButton* _noSeatBtn;
    //百、千、万、十万、百万五个按钮的列表
    Vector<BetObjV88 *> _vecBetObj;

    //闲家在东（=天0）区域筹码
    Vector<BetV88 *> _betsInTianZone;
    //闲家在南（=地1）区域筹码
    Vector<BetV88 *> _betsInDiZone;
    //闲家在西（=玄2）区域筹码
    Vector<BetV88 *> _betsInXuanZone;
    //闲家在北（=黄3）区域筹码
    Vector<BetV88 *> _betsInHuangZone;

    //当前玩家在四区域投注的筹码类型
    std::vector<int> _myBetTypeInTianZone;
    std::vector<int> _myBetTypeInDiZone;
    std::vector<int> _myBetTypeInXuanZone;
    std::vector<int> _myBetTypeInHuangZone;

    Vector<Sprite *> logos;
    //区域总投注数
    Vector<LabelAtlas *> _allBetValInZone;
    //当前玩家各区域的投注数
    Vector<LabelAtlas *> _myBetValInZone;

    //牛数
    INT _cowValue[5];
    //牛数的资源（庄天地玄黄）
    Sprite *_cowRes[5];
    //牛数背景的资源（庄天地玄黄）
    Sprite *_cowBgRes[5];
    //区域倍率资源（天地玄黄）
    Sprite *_rateResInZone[4];
    //区域倍率数值（天地玄黄）
    INT _rateDataInZone[4];
    ////下拉列表背景【往里添加资源】
    // Sprite*					_toolbarBg;

    //五组牌的值（庄天地玄黄）
    BYTE _cardsValue[5][5];

    //五组牌的资源（庄天地玄黄）
    // parms: para1=第几张牌；para2=第几个区域
    PokerCardV88 *_cardsRes[5][5];

    //五组牌的位置（庄天地玄黄）
    Point _cardsPos[5][5];
    BankerLayerV88 *bankerLayer;
	OtherPlayerV88* m_pOtherPlayerV88;
    ////是否首次触摸投注区域
    //bool isFirstTouch;
    //=========================================
    int _getGiftMoneyNum;
    int _changedSeatNum;
    bool _bUpForChangeSeat;
    bool _bRepeatBetSataus; //是否处于重复下注状态

    //联合上庄列表
    INT _bUionBankerInfoArr[MAX_UION_COUNT][MAX_UION_COUNT];
    //天地玄黄四区域赢牌提示
    Vector<Sprite *> _vecWinTipSprite;
    //天地玄黄最近6局胜负界面标记
    Sprite *_recordGameInfoSpriteArr[BET_ZONE_NUM_V88][RECORD_LATEST_SHOW_NUM_V88];
    //最近72局信息
    INT _latest72GameInfoArr[GAME_SAVE_COUNT][MAX_NOTE_COUNT];
    //天地玄黄四区域最近的胜负比例值
    Vector<Label *> _vecWinOrLoseProp;
//    Sprite *_chatTips;
//    Button *_chatOpenBtn;
//    Label *_chatUserName;
//    Label *_chatLabel;
    INT _i_cardIdx; //当前发牌的轮数
    INT _j_cardIdx; //当前发牌的区域数
    bool _zhangqi;
    LLONG _xiazhu;
    //当前玩家的座位序列值
    INT _mySeatIndex;
    INT _bankerIndex;  //标记庄家做庄
    INT _FuctionTimes; //函数执行次数
    // BRNNSettlementLayer* settlementLayer;
    LLONG m_bankerFen;
    LLONG m_myselfFen;
    float _xScale;
    float _yScale;
    Sprite *m_AnimateImage;
//    DropdownList *dropdownList;

    GameNotice*		m_Notice;//广播

    Sprite* m_cowNumBg;
    Size toolBar_Size;
    Node* center;
    vector<Point> _vecToolbarPoints;
    vector<Point> _vecToolbarPoints2;
    bool toolBar_flag;
    int m_GameStatus;
    bool m_bNextOut;					//下局离开
    vector<Button*>							_vecMenuButtonKids;
    Sprite*											_toolbarBg;
	Sprite*	                                        _recordBg;

	Node* gameStart;
	cocostudio::timeline::ActionTimeline* gameStartAnimation;
private:
    void initTheNewGameData();
    //清理桌子
    void clearTheNewGameData();
    void removeTexture(const std::string &textureKeyName);

private: // ui函数
    //无逻辑
    //加载可复用资源
    void loadRes();
    //加载牌的位置
    void initCardPos();
    //初始化桌面资源
    void initTable();

    void initWinLosShowcase();
	void showWinLosShowcase();
	void hideWinLosShowcase();
    void initButtons();
    void initSeats();
    void initChat();
    void initPlayerInfo();
    void initDropdownList();
    void createToolBarUI();
    //==
    void updateBankerUI();
    void updateUI();
    //更新庄闲家的金钱数
    void updateSomeData();
    void updateLatest6GameInfo(); //更新最近几局输赢状况显示

    void getBankerInfo();
    //更新上庄列表
    void updateAppliedBankerList(short int list[PLAY_COUNT + 1][2]);

    void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);
    void toolBarClickCallback(Ref* pSender, Control::EventType event);
    void toolBarFlag(float dt);

	void playScanner();
	//没钱提示弹窗
	Text*	m_TextTip;
	long long    m_i64TipMoney;
	void updateTip(float dt);
private:
    //客户端送礼假象
    // void clientGiftRand(float dt);
    //赠送礼物实现
    //void giftMove(Point startPos, Point targetPos, int giftId);

    //投注假象，看到人少的时候，断断续续的投注 很不好看 做个假象
    void betFalse(float dt);

    Vec2 getScale();
    void showCowRun();
    //牌
    //保存四个区域牌的数据（由服务端下发）
    void savaCardValue(BYTE card[5][5]);
    //计算四个区域牌的牛数
    int findCowValue(int (&poke)[5]);
    void recoveryOpenCardScene();

    //发牌逻辑************************
    //发牌
    void faPai();
    //发牌计时器
    void faPaiTimer(float dt);
    //创建庄家的牌，包含用于同步的逻辑
    void createPokerCard(bool isSync);
    //创建牛数资源
    void createCowNum();
    // end************************

    //开牌逻辑************************
    //开牌
    void kaiPai(float dt);
    /*天地玄黄庄依次翻开最后一张牌*/
    void eastFlipLastPoke(float dt);
    void southFlipLastPoke(float dt);
    void westernFlipLastPoke(float dt);
    void northFlipLastPoke(float dt);
    void bankerFlipLastPoke(float dt);
    /*显示天地玄黄庄的牛数*/
    void showEastCardResult(float dt);
    void showSouthCardResult(float dt);
    void showWesternCardResult(float dt);
    void showNorthCardResult(float dt);
    void showBankerCardResult(float dt);
    // end************************

    //庄家最后翻完牌对比牛大小后，开始显示输赢区域和输赢倍数
    void showAllResult();

    //回收投注币
    void backZoneBet();
    void backEastZoneBet(float dt);
    void backSouthZoneBet(float dt);
    void backWesternZoneBet(float dt);
    void backNorthZoneBet(float dt);

    //播放清除赌注的声音
    void playCleanBet();

    //清理当局桌面
    void clearTable();
    //最后清桌
    void LastClean(float time);
    //设置筹码在区域是否可见
    void setBetInZoneVisible(bool isVisible, LLONG allBetVal[MAX_NOTE_COUNT],
                             LLONG myBetVal[MAX_NOTE_COUNT]);
    //玩家行为操作======
    //部分按钮响应
    void onBtnClick(Ref *pSender);
    void handleDropdownList(Ref *pSender, Control::EventType event);
    void handleLeaveBtn();
    //游戏中离开弹框
    void leaveBombBox();
    //取消重复投注
    void cancelRepeatBet();
    //====
    //是否可以重复投注
    bool isEnableRepeatBet();
    //四角箭头不断缩放的效果
    void showBankerSign(Vec2 pos);
    //
    int getSelectBet(); //返回0-4 分别代表百、千、万、十万、百万
    int getBetMoney(int idx);
    //是否可以下注
    void setBetsClick(bool bClick);

    // int viewToLogic(int lSeat);
    // int logicToView(int lSeat);
private:
//bool giftToMeAddMoney(HNSocketMessage* socketMessage);
    void updateMyData();
//int getEmptySeat();
//void userChangeSeat(int targetSeat, bool bViewToLogic = true);
    void updateOtherGiftMovey(int sendId, int targetId, int money);
//当前玩家在哪个列表里（上庄or联合）
    int isMySeatInBankerList();

    Vec2 convertPoint(float x,float y);
    Vec2 convertGL(float x, float y);
//显示牌的结果（即几牛）
    void showCardResult(int zoneNo);

    bool isPlayeTian;
    bool isPlayDi;
    bool isPlayXuan;
    bool isPlayHuang;

    void reset();
private:
//按钮标签标识
    enum Em_TableBtnTag {
        //下拉按钮
        em_toolbarBtn = 10000,
        //下拉列表的设置按钮
        em_toolbarSetBtn,
        //下拉列表的帮助按钮
        em_toolbarHelpBtn,
        //下拉列表的离开按钮
        em_toolbarLeaveBtn,
        //商店按钮
        em_shopBtn,
        //表情按钮
        em_emotionBtn,
        //无座玩家按钮
        em_noSeatBtn,
        //银-行取款
        em_openBank,
        //系统消息
        em_chatOpenBtn,
        //申请上庄
        em_makerBtn,
        //上庄列表
        em_banker_list,
        //合作上庄
        em_uionmakerBtn,
        //重复下注按钮
        em_repeatBetBtn,
        //取消重复下注按钮
        em_cancelRepeatBetBtn,
		//走势图按钮
		em_zoushituBtn,
		em_zoushituHideBtn,
        settlement_layer,
        em_toolbarNextOutBtn,
        em_toolbarNextOutLight
    };

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

#endif // __BRNN_GameTable_BRNNTableUI_h__
