//#include <propidl.h>
#include "UI/UserHead.h"
#include "pattern/factory/gameLand/IGameLand.h"
#include "pattern/factory/gameRegister/IGameRegister.h"
#include "pattern/factory/findPw/IFindPw.h"
#include "pattern/factory/gameLogOn/IGameLogOn.h"
#ifndef __PLATFORM_MESSAGE_H__
#define __PLATFORM_MESSAGE_H__

#include "ui/CocosGUI.h"
using namespace  cocos2d::ui;

// 说明：
//所有的层，节点的tag标签不能重复
/////////////////////////////////////////////////////////////////////////
static const int PLATFORM_ZORDER_UI = 100;
static const int PLATFORM_UI_ZORDER_TOP = PLATFORM_ZORDER_UI + 10;       //
static const int PLATFORM_MASKlAYER_ZORDER_TOP = PLATFORM_ZORDER_UI + 4; //
static const int PLATFORM_UI_ZORDER_BOTTOM = PLATFORM_ZORDER_UI + 1;     //
static const int PLATFORM_UI_ZORDER_NOTICE = PLATFORM_ZORDER_UI + 1;     //
static const int PLATFORM_UI_ZORDER_POPULARISE = PLATFORM_ZORDER_UI + 5; //推广
static const int ROOM_LAYER_ZORDER = PLATFORM_ZORDER_UI + 2; // 房间列表层级
static const int GAMELIST_LAYER_ZORDER = PLATFORM_ZORDER_UI + 2; // 游戏列表层级
static const int DESKLIST_LAYER_ZORDER = PLATFORM_ZORDER_UI + 3; // 桌子列表层级
static const int CHILD_LAYER_ZORDER = PLATFORM_ZORDER_UI + 4; // 子节点弹出框层级
//////////////////////////////////////////////////////////////////////////
static const int CHILD_LAYER_TAG = 100;
static const int CHILD_NEXTLAYER_TAG = 100;
static const int CHILD_LAYER_USERHEAD_TAG = CHILD_LAYER_TAG + 1; // 图像层标签
static const int CHILD_LAYER_STORE_TAG = CHILD_LAYER_TAG + 2; // 商店层标签
static const int CHILD_LAYER_SET_TAG = CHILD_LAYER_TAG + 3;   // 设置层标签
static const int CHILD_LAYER_BANK_TAG = CHILD_LAYER_TAG + 4;  // 银行层标签
static const int CHILD_LAYER_SERVICE_TAG = CHILD_LAYER_TAG + 5; // 服务层标签
static const int CHILD_LAYER_ROOM_TAG = CHILD_LAYER_TAG + 6;  // 房间层标签
static const int CHILD_LAYER_GAMES_TAG = CHILD_LAYER_TAG + 7; // 游戏层标签
static const int CHILD_LAYER_DESKS_TAG = CHILD_LAYER_TAG + 8; // 桌子层标签
static const int CHILD_LAYER_EXIT_TAG = CHILD_LAYER_TAG + 9;  // 退出层标签
static const int CHILD_LAYER_RANK_TAG = CHILD_LAYER_TAG + 12; // 排行榜层标签
static const int CHILD_LAYER_SIGN_TAG = CHILD_LAYER_TAG + 13; // 签到层标签
static const int CHILD_LAYER_ONLINE_TAG =
    CHILD_LAYER_TAG + 14; // 在线奖励层标签
static const int CHILD_LAYER_REBATE_TAG =
    CHILD_LAYER_TAG + 15; // 充值返利层标签
static const int CHILD_LAYER_EXITCHANGE_TAG =
    CHILD_LAYER_TAG + 16; // 退出选择层标签
static const int CHILD_LAYER_PASS_TAG = CHILD_NEXTLAYER_TAG; // 修改密码层标签
static const int CHILD_LAYER_PHONE_TAG =
    CHILD_NEXTLAYER_TAG + 1; // 绑定手机层标签

//////////////////////////////////////////////////////////////////////////
static const float request_game_room_timer_time = 0.5f;
//////////////////////////////////////////////////////////////////////////
static const char *PLATFORM_BG      = "platform/hallBg.png";
static const char *DECORATION_LEFT  = "platform/subViewBambooLeft.png";
static const char *DECORATION_RIGHT = "platform/subViewBambooRight.png";

static const char *USER_HEAD_FRAME = "platform/bottomHeadKuang.png";
static const char *USER_MEN_HEAD   = "common/23.png";
static const char *USER_WOMEN_HEAD = "common/24.png";

static const char *PLATFORM_NOTIC_BG = "platform/notice/MsgManager_BG.png";
static const char *PLATFORM_NOTIC_TITLE = "platform/notice/MsgManager_BG.png";
static const char *PLATFORM_NOTIC_SPEAKER = "platform/notice/Speaker.png";
static const char *PLATFORM_NOTIC_COLON = "platform/notice/Colon.png";
static const char *Notice_Bg_Stencil = "platform/notice/Msg_BG.png";

//////////////////////////////////////////////////////////////////////////

static const char *PLATFORMUI_BOTTOM_CSB = "csb/platform/lobbyUi/PlatformUi_BOTTOM/PlatformUi_BOTTOM.csb";
//static const char *PLATFORMUI_TOP_CSB ="csb/platform/lobbyUi/PlatformUi_TOP/PlatformUi_TOP.csb";
static const char *PLATFORMUI_TOP_LOGO ="platform/lobbyUi/PlatformUi_TOP/11105100.png";

//背景动画
static const char *BG_ANIMATION_CSB ="common/animation/background/background1.csb";
//////////////////////////////////////////////////////////////////////////

//后台开关
typedef struct tagBacksTageOpen {
    bool        bank;
    bool        recharge;
    bool		   rankList;

    tagBacksTageOpen() {
        memset(this, 0, sizeof(tagBacksTageOpen));
    }
} BacksTageOpen;

struct LOBBY_TOP {
    Text*               Label_UserID;
    Text*               Label_UserName;
    Text*               Label_UserMoney;
    Text*               Text_IdStr;
    Text*               Text_NickStr;
    Text*               Text_MoneyStr;

    Sprite*         Sprite_LogoSpr;
    Sprite*         Sprite_TypeSpr;
    Sprite*         Sprite_FishBiLi;

    UserHead*   UserHead_Head;
    Button*         Button_Return;
    Button*         Button_Service;
    Button*         Button_Bank;
    Button*         Button_Charge;
    Button*         Button_Exit;
    Button*         Button_Shop;
    Button*         Button_Gift;
    Button*         Button_Setting;
    Button*         Button_newYear;
    Button*         Button_popularize;
    Button*         Button_shop;
    Button*         Button_quicklyGame;
    Button*			Button_rankList;
    Layout*         layout;
    Node*           node;
    ImageView*  bgGlob;
    Button*         globButton;
    Sprite*         Sprite_id;
    Sprite*         Sprite_nickName;
    Sprite*         Sprite_platformLogo;
    Sprite*         Sprite_topTitle;
} ;


struct LOBBY_BOTTOM {
    UserHead*   UserHead_Head;
    Sprite*         bottomFrame;
    //Button* Button_Friend;
    //Button* Button_Task;
    //Button* Button_Activity;
    //Button* Button_Bank;
    //Button* Button_Store;
    //Button* Button_Rank;
    Text*               Label_UserID;
    Text*               Label_UserName;
    Text*               Label_UserMoney;
    Layout*			   layout;
    Node*			node;
} ;

struct LOGIN_UI {
    Node* node;
    Sprite* logoImage;
    IGameLand* loginLayout;
    IGameRegister* registerLayout;
    IFindPw* findLayout;
    IGameLogOn* logonLayout;
//    ImageView* girl;
//    ImageView* bg;
};

#endif //__PLATFORM_MESSAGE_H__
