#ifndef __GoldenToad_MessageHead_h__
#define __GoldenToad_MessageHead_h__

//#include "HNNetExport.h"
#include "HNBaseType.h"
#include <string>

namespace GoldenToad
{
    //////////////////////////////////////////////////////////////////////////
    //#define GAME_NAME             TEXT("金蟾捕鱼")
    //////////////////////////////////////////////////////////////////////////
    
    //////////////////////////////////////////////////////////////////////////
    //	统一管理捕鱼多款游戏参数、开关
    //	BY HBC on 20160517
    //////////////////////////////////////////////////////////////////////////
    
#define TOAD_
    
#pragma pack(1)
    
    enum GoldenToad_CONST
    {
        /**
         *	@@desctiption
         *		游戏（备用） ID
         *	原有捕鱼ID：33003106		GAME_NAME_ID
         *  金蝉捕鱼ID：33003107		GAME_TOAD_NAME_ID
         *  李逵劈鱼ID：33003108		GAME_LIKUI_NAME_ID
         *  大闹天宫ID：33003109      GAME_MONKEY_NAME_ID
         *	以上ID需要与服务端保持一致
         *
         *	@author
         *		BY HBC on 20160517
         */
        GAME_NAME_ID            =           33003106,					//游戏 ID    33003106
        GAME_TOAD_NAME_ID		=			33003107,
        GAME_LIKUI_NAME_ID		=			33003108,
        GAME_MONKEY_NAME_ID     =           33003109,
        GAME_KIND_ID            =           1,
        PLAY_COUNT			    =			4,							//游戏人数
//        GAME_PLAYER				=			4,							//	TODO: 这个值为什么设定为 8 ？？？？？？？？？？？？？    by HBC
        
        //游戏状态
        GAME_STATUS_FREE		=			0,							//空闲状态
        GAME_STATUS_PLAY		=			100,						//游戏状态
        GAME_STATUS_WAIT		=			200,						//等待状态
        
        INVALID_FISHID			=			0,							//错误鱼id
    };
    
    const static bool canPrintDebugLog = true;
    
    static const float groupFishSpeed = 3.0f;
    
    static const FLOAT		PI					=	3.14159f;
    static const INT		BACKGROUND_ZORDER	=	0;
    static const INT		FISH_ZORDER			=	10;
    static const INT		NOTICE_ZORDER		=	50;
    static const INT  		BULLET_ZORDER		=	15;
    static const INT		WATER_ZORDER		=	9;
    static const INT		PLAYER_ZORDER		=	210;
    static const INT		UI_ZORDER			=	215;//游戏中UI的层级设定
    static const INT		Notify_ZORDER		=	220;//游戏中设置界面层级设定
    static const INT		MAX_ZORDER			=	280;
    
    
    typedef struct  tagFishSpirteInfo
    {
        std::string name;
        BYTE count;
        BYTE dieCount;
    } T_FISHSPRITEINFO;
    
    enum LockType
    {
        NORMAL = 0,
        REDFISH,
        BIGFISH
    };
    
    const FLOAT kResolutionWidth = 1440;
    const FLOAT kResolutionHeight = 900;
    
    /*------------------------------------------------------------------------------*/
#define DEF_SCREEN_WIDTH	  1440			// 默认屏宽
#define DEF_SCREEN_HEIGHT	  900			// 默认屏高
    
    /*------------------------------------------------------------------------*/
    //游戏窗口大小
    static float screen_width_	= DEF_SCREEN_WIDTH;
    static float screen_height_ = DEF_SCREEN_HEIGHT;
    //电脑显示屏大小
    static int screen_width_PC	= DEF_SCREEN_WIDTH;
    static int screen_height_PC = DEF_SCREEN_HEIGHT;
    
    static	float g_hscale = 1.0f;
    static	float g_vscale = 1.0f;
    /*------------------------------------------------------------------------------*/
    
    /**
     *	动画移动偏移量
     */
    const static float ACTION_UP_DOWN_OFFSET = 65.0f;
    const static int UP_DOWN_POSITION_NO_START = 2;
    const static int UP_DOWN_POSITION_NO_END = 3;
    
    struct Position {
        float x;
        float y;
    };
    
    enum TraceType {
        TRACE_LINEAR = 0,
        TRACE_BEZIER
    };
    
    struct FPoint {
        float x;
        float y;
    };
    
    struct FPointAngle {
        float x;
        float y;
        float angle;
    };
    
    enum SceneStyle {
        SCENE_STYLE_1 = 0,
        SCENE_STYLE_2,
        SCENE_STYLE_3,
        
        SCENE_STYLE_COUNT
    };
    
    //////////////////////////////////////////////////////////////////////////
    // 游戏定义
    //////////////////////////////////////////////////////////////////////////
    
    /*
     // 座位号
     -------------
     0   1   2
     
     5   4   3
     -------------
     */
    
    enum SceneKind {
        SCENE_KIND_1 = 0,
        SCENE_KIND_2,
        SCENE_KIND_3,
        SCENE_KIND_4,
        SCENE_KIND_5,
        
        SCENE_KIND_COUNT
    };
    
    /*********************************************/
    /**************** New Fish Kind **************/
    /*********************************************/
    enum FishKind
    {
        FISH_KIND_1 = 0 ,		//蜗牛鱼0			keep
        FISH_KIND_2,			//绿草鱼1			keep
        FISH_KIND_3,			//黄草鱼2			keep
        FISH_KIND_4,			//大眼鱼3			keep
        FISH_KIND_5,			//黄扁鱼4			keep
        FISH_KIND_6,			//小丑鱼5			keep
        FISH_KIND_7,			//河豚6				modified	小刺鱼	名字变化，无其他变动
        FISH_KIND_8,			//蓝鱼儿7			keep
        //FISH_KIND_9,			//红灯笼鱼
        FISH_KIND_10,			//蓝灯笼鱼8			modified	灯笼鱼	名字变化，去除原来的红灯笼鱼
        FISH_KIND_11,			//海龟9				keep
        FISH_KIND_12,			//花瓣鱼10			keep
        FISH_KIND_13,			//蝴蝶鱼11			keep
        FISH_KIND_14,			//孔雀鱼12			keep
        FISH_KIND_15,			//剑鱼儿13			keep
        FISH_KIND_16,			//蝙蝠鱼14			keep
        FISH_KIND_17,			//银鲨鱼15			modified	银龙鱼		名字变化，顺序变化
        FISH_KIND_18,			//金鲨鱼16			modified	金龙鱼		名字变化，顺序变化
        FISH_KIND_19,			//金箍棒17			modified	忠义堂		全屏静止		名字变化，顺序变化，资源变动，功能不变
        FISH_KIND_20,			//霸王鲸18			modified	金鲨鱼		名字变化，顺序变化
        //FISH_KIND_21,			//银龙
        FISH_KIND_22,			//金龙19			keep
        FISH_KIND_23,			//海藻20			modified	水浒传		局部炸弹		名字变化，功能不确实是否一致
        FISH_KIND_24,			//全屏炸弹21		modified	超级炸弹		全屏炸弹		名字变化
        FISH_KIND_25,			//金色金蟾22		modified	李逵						名字变化，资源变化
        FISH_KIND_26,			//绿色金蟾23		modified	企鹅						名字变化，资源变化
        
        FISH_KIND_27,			//三角大三元24		keep
        FISH_KIND_28,			//直线大三元25		keep
        FISH_KIND_29,			//四角大四喜26		keep
        FISH_KIND_30,			//直线大四喜27		keep
        
        //FISH_KIND_31,			//红鱼--鱼王		未发现与其功能类似的，暂时注释掉
        FISH_KIND_32,			//带盘子的鱼-鱼王28	keep	同类炸弹		同类型的鱼全部捕获	鱼类型要求与 “一网打尽” 上一致，该功能需要限制某些类型的鱼

        FISH_KIND_33,  //绿草鱼
        FISH_KIND_34,  //大眼鱼
        FISH_KIND_35,  //小刺鱼
        FISH_KIND_36,  //剑鱼
        FISH_KIND_37,  //银龙
        FISH_KIND_38,  //美人鱼
    
        FISH_KIND_COUNT
    };
    
    //不同倍数的鱼捕获时金币显示数量
    static unsigned int	G_Fish_Coin_Count_By_Multiple[4] =
    {
        3,//Multiple_1_Kind = 3,
        5,// Multiple_2_Kind = 5,
        8,//Multiple_3_Kind = 8,
        10,//Multiple_4_Kind = 10,
    };
    
    //环形鱼阵个数
    static	int G_Fish_Circle_Count[FISH_KIND_COUNT] = {60,60,50,35,30,35,30,25,20,20,10,8,0,0,0,0,0,0,0,0,0,};
    
    /**
     *	炮弹种类
     *	原始项目，炮弹有8种
     *
     *	金蝉捕鱼
     *	炮弹有10种（1000-10000）
     *
     *	李逵劈鱼
     *	炮弹有4种（1100，3300，6600，9900）
     */
    enum BulletKind
    {
        BULLET_KIND_1_NORMAL = 0,	//单管炮，资源1
        BULLET_KIND_2_NORMAL,		//双管炮，资源2
        BULLET_KIND_3_NORMAL,		//三管炮，资源3
        BULLET_KIND_4_NORMAL,		//三管土豪金炮，资源4
        BULLET_KIND_5_ION,		//三管天使能量炮，资源5
        
        BULLET_KIND_COUNT
    };
    
    enum BulletKindToad
    {
        BULLET_KIND_TOAD_1_NORMAL = 0,
        BULLET_KIND_TOAD_2_NORMAL,
        BULLET_KIND_TOAD_3_NORMAL,
        BULLET_KIND_TOAD_4_NORMAL,
        BULLET_KIND_TOAD_5_NORMAL,
        BULLET_KIND_TOAD_1_ION,
        BULLET_KIND_TOAD_2_ION,
        BULLET_KIND_TOAD_3_ION,
        BULLET_KIND_TOAD_4_ION,
        BULLET_KIND_TOAD_5_ION,
        
        BULLET_KIND_TOAD_COUNT
    };
    
    enum BulletKindLikui
    {
        BULLET_KIND_LIKUI_1_NORMAL = 0,
        BULLET_KIND_LIKUI_2_NORMAL,
        BULLET_KIND_LIKUI_1_ION,
        BULLET_KIND_LIKUI_2_ION,
        
        BULLET_KIND_LIKUI_COUNT
    };
    
    const INT kMaxCatchFishCount = 2;
    
    struct CMD_S_GameStatus
    {
        UINT 					game_version;
        SceneStyle				Scene_Style_Kind;					//场景背景ID
        INT						iUserFireBase[PLAY_COUNT];			//玩家炮火倍数
        LLONG 					bag_money[PLAY_COUNT];             //玩家身上金币数
//        LLONG 					exchange_fish_score[PLAY_COUNT];	//玩家身上兑换鱼币数，此游戏没什么用
    };
    
    struct CMD_S_GameConfig {
//        int exchange_ratio_userscore;
//        int exchange_ratio_fishscore;
//        int exchange_count;
        
        int baseMul;        //游戏房间炮火倍数
        
        int min_bullet_multiple;
        int max_bullet_multiple;
        
        int bomb_range_width;
        int bomb_range_height;
        
        int fish_multiple[FISH_KIND_COUNT];
        int fish_speed[FISH_KIND_COUNT];
        int fish_bounding_box_width[FISH_KIND_COUNT];
        int fish_bounding_box_height[FISH_KIND_COUNT];
        int fish_hit_radius[FISH_KIND_COUNT];
        
        int bullet_speed[BULLET_KIND_COUNT];
        int net_radius[BULLET_KIND_COUNT];
    };
    
    struct GameStation//断线重回用的数据包sdp
    {
        //游戏版本
        BYTE					iVersion;						//游戏版本号
        BYTE					iVersion2;						//游戏版本号
        BYTE					game_status;					//状态标识
        
        CMD_S_GameStatus		GameStatus;						//状态数据
        CMD_S_GameConfig		GameConfig;						//配置数据
        
        BYTE GameMessage[260];//游戏消息
    };
    
    //玩家进入  主要是把玩家的金币数发送过来
    struct CMD_S_GameUserCome
    {
        UINT					dwDeskStation;
        LLONG 					i64fish_score;
        
        CMD_S_GameUserCome()
        {
            memset(this, 0, sizeof(CMD_S_GameUserCome));
            dwDeskStation = 255;
        }
    };
    
    struct CMD_S_FishTrace
    {
        FPoint 					init_pos[5];
        INT 					init_count;
        FishKind 				fish_kind;			//鱼的主类型
        FishKind 				fish_kind_ex[5];	//鱼的辅助类型
        INT 					fish_id;
        INT						multiple;			//鱼的倍数
        TraceType 				trace_type;
        
        void init()
        {
            memset(this, 0, sizeof(CMD_S_FishTrace));
        }
    };
    
    struct	CMD_S_CirleFishGroup
    {
        bool					bHave;
        FishKind				iFishKind;
        INT						iIndex;
        INT						iCount;
        INT						iRedIndex;
        INT						iRedIndexIndex;
        
        CMD_S_CirleFishGroup()
        {
            bHave = false;
            iCount = 0;
            iFishKind = FISH_KIND_COUNT;
            iIndex = -1;
            iRedIndex = 0;
            iRedIndexIndex = 0;
        }
    };
    
//    static const INT Default_Score_Value = 0;
    
    struct CMD_C_AI_LOCK_RESULT
    {
        bool isSuccess;//S 通知 C(AI)是否锁定成功，失败则直接解除锁定
    };
    
//    struct CMD_S_ExchangeFishScore
//    {
//        WORD 					chair_id;
//        LLONG 					swap_fish_score;//当前玩家实际鱼币数 而不是增量
//        LLONG 					exchange_fish_score;//玩家的金币数
//        bool					isIncrease;//是否为上分结果
//        bool					isExchangeSuccess;//是否兑换成功，主要在上分失败的时候需要通过这个值进行判断--added by HBC
//    };
    
    struct CMD_S_ChangeCoin
    {
        WORD 					chair_id;			//座位号
        BulletKind 				bullet_kind;		//子弹类型
        INT 					bullet_mulriple;	//炮火倍数
    };
    
    struct CMD_S_UserFire
    {
        BulletKind 				bullet_kind;			//子弹类型
        INT 					bullet_id;				//子弹id
        WORD 					chair_id;				//座位号
        WORD 					android_chairid;		//机器人座位号
        FLOAT 					angle;					//开炮角度
        INT 					bullet_mulriple;		//炮火倍数
        INT 					lock_fishid;			//锁定鱼的id
        LLONG 					fish_score;				//鱼的奖励金币
        bool 					bIsRobot;				//是否机器人
        
        CMD_S_UserFire()
        {
            memset(this, 0, sizeof(CMD_S_UserFire));
        }
    };
    
    struct CMD_S_CatchFish
    {
        WORD 					chair_id;			//座位号
        INT 					fish_id;			//鱼id
        FishKind 				fish_kind;			//鱼类型
        bool 					bullet_ion;			//是否能量炮
        LLONG 					fish_score;			//打中鱼奖励金币
        INT						fish_multiple;		//鱼的倍数
        LLONG					i64UserFishScore_;	//玩家身上总积分  by HBC 原来为 总金币
    };
    
    struct CMD_S_BulletIonTimeout
    {
        WORD 					chair_id;
    };
    
    struct CMD_S_CatchSweepFish
    {
        BYTE					byIndex;
        WORD 					chair_id;
        INT 					fish_id;
        LLONG 					i64FishSocre;
        CMD_S_CatchSweepFish()
        {
            memset(this,0,sizeof(CMD_S_CatchSweepFish));
        }
    };
    
    struct CMD_S_CatchSweepFishResult
    {
        WORD					chair_id;
        INT						fish_id;
        INT						catch_fish_count;
        INT						catch_fish_id[200];
        INT						fish_score[200];
        INT						fish_multiple;//鱼的倍数，新增字段
        LLONG					fish_score_Sum;
        LLONG					i64UserFishScore_;
    };
    
    struct CMD_S_CatchSweepFishResultEx
    {
        CHAR    				cRoomName[61];				//房间名字
        INT						exchange_ratio_userscore_;	//换算比例中用户金币
        INT						exchange_ratio_fishscore_;	//换算比例中鱼币
        INT						iDeskID;					//桌子ID
        CHAR					szNickName[61]; 			//昵称
        INT						fish_kind;					//鱼类型ID
        LLONG					fish_score;					//打中鱼赚多少钱
        CHAR    				cFishName[260];				//鱼的名字
    };
    
    struct CMD_S_HitFishLK
    {
        WORD 					chair_id;
        INT 					fish_id;
        INT 					fish_mulriple;
    };
    
    struct CMD_S_SwitchScene
    {
        SceneKind 				scene_kind;
        SceneStyle				Scene_Style;	//场景ID
        INT 					fish_count;
        INT 					fish_id[245];
        FishKind 				fish_kindEx[10];
        FishKind 				fish_kind[245];
        
        CMD_S_SwitchScene()
        {
            memset(this, 0x0, sizeof(CMD_S_SwitchScene));
        }
    };
    
    struct CMD_S_GroupFishTrace
    {
        BYTE 					byIndex;
        INT 					fish_count;
        FishKind 				fish_kind[250];
        FishKind 				fish_red_kind[5];		//单独标记红色鱼王
        INT 					fish_id[250];
    };
    
//    struct CMD_C_ExchangeFishScore
//    {
//        bool increase;			//true 上分请求；false，下分请求，其他参数不需要
//        bool isSingle;			//true 单次上分；false，一次性转换一定金币为积分
//        LLONG exchange_fish_money;//一次性转换的金币数
//        double roomRate;			//房间倍率
//    };
    
    struct CMD_C_ChangeCoin
    {
        int     currentBulletMul;   //当前鱼炮倍率
        bool    isUp;               //true 调大请求，当达到最大，则继续变化成最小的
    };
    
    struct CMD_C_UserFire
    {
        BulletKind 				bullet_kind;
        FLOAT 					angle;
        INT 					bullet_mulriple;//消耗的金币
        INT 					lock_fishid;
        INT 					bullet_id;
        bool 					bIsRobot;
        
        CMD_C_UserFire()
        {
            memset(this, 0, sizeof(CMD_C_UserFire));
        }
    };
    
    struct CMD_C_CatchFish
    {
        WORD 					chair_id;
        INT 					fish_id;
        BulletKind 				bullet_kind;
        INT 					bullet_id;
        INT 					bullet_mulriple;
    };
    
    struct CMD_C_CatchSweepFish
    {
        WORD 					chair_id;
        INT 					fish_id;
        INT 					catch_fish_count;
        INT 					catch_fish_id[300];
        
        CMD_C_CatchSweepFish()
        {
            memset(this, 0, sizeof(CMD_C_CatchSweepFish));
        }
    };
    
    struct CMD_C_HitFishLK
    {
        INT 					fish_id;
    };
    
    struct CMD_C_Fish20Config
    {
        UINT 					game_id;
        INT 					catch_count;
        double 					catch_probability;
    };
    
    struct CMD_C_AndroidBulletMul
    {
        WORD					chair_id;
        INT						bullet_id;
        INT						bullet_mulriple;
        BulletKind				bullet_kind;
    };
    
    //连锁闪电
    struct CMD_C_LineInfo
    {
        FPoint	fpStart;
        FPoint	fpEnd;
        FLOAT	fChange;
        UINT	dwTime;
        
        CMD_C_LineInfo()
        {
            memset(this, 0, sizeof(CMD_C_LineInfo));
        }
    };
    
    
    /////////////////////////////////////////////////////////////////////////////
    //超端控制
    /////////////////////////////////////////////////////////////////////////////
    
    //奖池操作结果
    struct CMD_S_StockOperateResult
    {
        BYTE 					operate_code;	//	0查询 1 清除 2 增加 3 查询抽水
        LLONG 					stock_score;	//	操作结果
    };
    
    //奖池操作
    struct CMD_C_StockOperate
    {
        BYTE 					operate_code;	// 0查询 1 清除 2 增加 3 查询抽水
    };
    
    struct CMD_C_UserFilter
    {
        UINT 					game_id;
        BYTE 					operate_code;	// 0 黑名单 1 白名单 2 清除
    };
    
    struct S_C_SUPER_CONFIG
    {
        bool bSuperUser;
    };
    
    //在线玩家信息
    struct OnLineUserInfo
    {
        BYTE	deskNo;
        char	nickName[61];
        UINT	userID;
        LLONG	i64UserMoney;
        OnLineUserInfo()
        {
            memset(this, 0, sizeof(OnLineUserInfo));
        }
    };
    
    struct S_C_SWITCH_INFO
    {
        bool special_switch_;				//特殊鱼控制开关
        bool control_switch_;				//玩家概率控制开关
        S_C_SWITCH_INFO()
        {
            memset(this, 0, sizeof(S_C_SWITCH_INFO));
        }
    };
    
#define SEND_FISH_NUM						/*4*/3
    
    static BYTE G_bySpecialFishId[SEND_FISH_NUM] = { FISH_KIND_19, FISH_KIND_20, /*FISH_KIND_21, */FISH_KIND_22 };
    
    struct S_C_FISH_NAME
    {
        char	cFishName[4][260];
    };
    
    struct S_C_ON_LINE_USER
    {
        bool	bStart;
        OnLineUserInfo user_info_;
        S_C_ON_LINE_USER()
        {
            memset(this, 0, sizeof(S_C_ON_LINE_USER));
        }
    };
    
    struct SpecialFish
    {
        BYTE fish_kind_;					//鱼类型
        int left_num_;						//剩余数量
        int catch_rate_;					//抓捕概率
        SpecialFish()
        {
            memset(this, 0, sizeof(SpecialFish));
        }
    };
    
    struct SpecialUser
    {
        SpecialFish special_fish_[4];		//受控的鱼
        int user_id_;						//受控的玩家id
        SpecialUser()
        {
            memset(this, 0, sizeof(SpecialUser));
            user_id_ = -1;
        }
    };
    
    struct ControlInfo
    {
        bool win_or_lose_;					//控制玩家赢或输
        int user_id_;						//受控玩家id
        int catch_rate_;					//输赢概率
        LLONG limit_score_;					//输赢上限
        ControlInfo()
        {
            memset(this, 0, sizeof(ControlInfo));
            user_id_ = -1;
        }
    };
    
    struct C_S_DELETE_CONTROL_INFO 
    {
        int user_id_;
    };
    
    struct C_S_ADD_SPECIAL
    {
        BYTE fish_kind_;					//鱼类型
        int user_id_;						//玩家id
        int left_num_;						//剩余数量
        int catch_rate_;					//抓捕概率
        
        C_S_ADD_SPECIAL()
        {
            memset(this,0,sizeof(C_S_ADD_SPECIAL));
        }
    };
    
    struct C_S_DELETE_SPECIAL
    {
        int user_id_;
        int fish_kind_;
        C_S_DELETE_SPECIAL()
        {
            memset(this,0,sizeof(C_S_DELETE_SPECIAL));
        }
    };
    
    //更新控制玩家信息
    struct S_C_UPDATE_CONTROL_INFO
    {
        ControlInfo control_infos_[40];
        S_C_UPDATE_CONTROL_INFO()
        {
            memset(this, 0, sizeof(S_C_UPDATE_CONTROL_INFO));
        }
    };
    
    //更新特殊鱼控制信息
    struct S_C_UPDATE_SPECIAL_INFO
    {
        SpecialUser special_infos_[20];
        S_C_UPDATE_SPECIAL_INFO()
        {
            memset(this, 0, sizeof(S_C_UPDATE_SPECIAL_INFO));
        }
    };
    
    //同步S-C可锁定鱼信息
    struct CMD_COM_ALLOW_LOCK_FISH
    {
        bool isInsert;	//true，添加；否则删除
        int fishID;
        FishKind fishKind;
        
        void init()
        {
            memset(this, 0, sizeof(CMD_COM_ALLOW_LOCK_FISH));
        }
    };
    
    /////////////////////////////////////////////////////////////////////////
    
#pragma pack()
    
    enum GoldenToad_COMMAND
    {		
        //////////////////////////////////////////////////////////////////////////
        // 服务端命令
        //////////////////////////////////////////////////////////////////////////
        SUB_S_GAME_CONFIG = 100,//设置游戏参数
        SUB_S_FISH_TRACE = 101,//生成新鱼的轨迹
        SUB_S_FISH_GROUP_TRACE = 102,//生成鱼群的轨迹
//        SUB_S_EXCHANGE_FISHSCORE = 103,//兑换鱼币（金蟾捕鱼游戏无效）
        SUB_S_CHANGE_COIN = 103,
        SUB_S_USER_FIRE = 104,//玩家射击
        SUB_S_CATCH_FISH = 105,//抓到鱼
        SUB_S_BULLET_ION_TIMEOUT = 106,//能量炮失效
        SUB_S_LOCK_TIMEOUT = 107,//定屏炸弹失效时间
        SUB_S_CATCH_SWEEP_FISH = 108,//抓到特殊鱼（炸弹，鱼王）
        SUB_S_CATCH_SWEEP_FISH_RESULT = 109,//抓到特殊鱼结果
        SUB_S_HIT_FISH_LK = 110,//打中李逵（金蟾捕鱼游戏无效）
        SUB_S_SWITCH_SCENE = 111,//切换场景，5种鱼阵
        SUB_S_STOCK_OPERATE_RESULT = 112,//查询库存，抽水
        SUB_S_SCENE_END = 113,//场景切换结束
        SUB_S_GAMEOUT = 114,//服务端强行让客户端退出
        SUB_S_CATCH_SWEEP_FISH_RESULT_EX = 115,//广播通知所有玩家捕到大鱼
        
        SUB_S_UPDATE_LOCK_FISH = 116,//通知有增加、减少可以锁定的鱼
        SUB_S_LOCK_FAIL_AI = 117,//服务端通知AI锁定失败，使其解除锁定重新开始下次锁定
        
        SUB_S_USER_COME = 120,//玩家进入
        
        S_C_UPDATE_CONTROL_INFO_SIG = 121,//更新控制概率信息
        S_C_UPDATE_SPECIAL_INFO_SIG = 122,//更新控制特殊鱼信息
        S_C_UPDATE_ONLINEUSER_INFO_SIG = 123,//更新在线玩家信息
        S_C_SWITCH_INFO_SIG = 124,//控制开关信息
        S_C_UPDATE_FISH_NAME_SIG = 125,//更新可控制鱼名称
        S_C_SUPER_CONFIG_SIG = 126,//是否超端玩家
        S_C_END_UPDATE_ONLINEUSER_INFO_SIG = 127,//更新在线玩家信息列表
        
        //////////////////////////////////////////////////////////////////////////
        // 客户端命令
        //////////////////////////////////////////////////////////////////////////
//        SUB_C_EXCHANGE_FISHSCORE = 1, //兑换鱼币（渔夫码头游戏无效）
        SUB_C_CHANGE_COIN = 1,
        SUB_C_USER_FIRE = 2, //玩家开火
        SUB_C_CATCH_FISH = 3, //子弹打中鱼
        SUB_C_CATCH_SWEEP_FISH = 4, //打中特殊鱼，报告当前屏幕中鱼信息
        SUB_C_HIT_FISH_I = 5, //打中李逵（渔夫码头游戏无效）
        SUB_C_STOCK_OPERATE = 6, //查询库存，抽水
        SUB_C_USER_FILTER = 7, //开火
        SUB_C_ANDROID_STAND_UP = 8, //机器人站起（渔夫码头游戏无效）
        SUB_C_FISH20_CONFIG = 9, //老版超端控制（渔夫码头游戏无效）
        SUB_C_ANDROID_BULLET_MUL = 10,//机器人子弹数目（渔夫码头游戏无效）
        
        C_S_UPDATE_SUPER_INFO_SIG		   = 11,//更新所有信息
        C_S_ADD_CONTROL_INFO_SIG		   = 12,//添加概率控制名单
        C_S_DELETE_CONTROL_INFO_SIG		   = 13,//删除概率控制名单
        C_S_ADD_SPECIAL_SIG				   = 14,//添加特殊鱼控制
        C_S_DELETE_SPECIAL_SIG			   = 15,//删除特殊鱼控制
        C_S_SWITCH_CONTROL_SIG		       = 16,//概率控制名单开关
        C_S_SWITCH_SPECIAL_SIG			   = 17,//特殊鱼控制名单开关
        
        SUB_C_UPDATE_LOCK_FISH			   = 18,//通知有增加、减少可以锁定的鱼
        
    };
    
};

#endif // CMD_FISH_H_
