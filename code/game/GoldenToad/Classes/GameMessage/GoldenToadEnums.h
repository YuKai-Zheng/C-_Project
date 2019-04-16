#ifndef __GoldenToad_Enums_h__
#define __GoldenToad_Enums_h__

#include "GoldenToadMessageHead.h"
#include "GoldenToadVersion.h"
namespace Enums
{
    
    /**
     *	定义捕鱼多个版本，不同版本需要在初始化时传入不同参数
     *	通过下述 枚举 GameVersionName 及 静态常量 GAME_VERSION_NAME 两个条件共同判断得出结论
     *	by HBC
     */
    enum GameVersionName
    {
        GAME_VERSION_ORIGNAL_FISH = GoldenToad::GAME_NAME_ID,			//项目最原始版本
        GAME_VERSION_TOAD_FISH = GoldenToad::GAME_TOAD_NAME_ID,			//金蝉捕鱼
        GAME_VERSION_LIKUI_FISH = GoldenToad::GAME_LIKUI_NAME_ID,		//李逵劈鱼
        GAME_VERSION_MONKEY_FISH = GoldenToad::GAME_MONKEY_NAME_ID,     //大闹天宫
    };
    
    /**
     *	GAME_VERSION_NAME 该值仅在 LOADING 阶段进行赋值，取值来源于大厅进入游戏时使用的参数
     *	游戏内部仅访问该值，不能在内部再次修改该值，除非重新返回游戏大厅，需注意！！！
     *	by HBC
     */
    static UINT GAME_VERSION_NAME = GAME_VERSION_ORIGNAL_FISH;
    
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
    /*enum BulletKind
     {
     BULLET_KIND_1_NORMAL = 0,
     BULLET_KIND_2_NORMAL,
     BULLET_KIND_3_NORMAL,
     BULLET_KIND_4_NORMAL,
     BULLET_KIND_1_ION,
     BULLET_KIND_2_ION,
     BULLET_KIND_3_ION,
     BULLET_KIND_4_ION,
     
     BULLET_KIND_COUNT
     };*/
    
    /**
     *	不同版本游戏 武器耗费金币 定义
     *	by HBC
     */
    enum BulletCostGold_ToadFish
    {
        BulletOffsetValue_ToadFish = 1000,
        BulletKind_Toad_Edge = 9000,
        BulletKind_Toad_MAX = 9900
    };
    
    enum BulletCostGold_LikuiFish
    {
        BulletKind_1_Value = 1100,
        BulletKind_2_Value = 3300,
        BulletKind_3_Value = 6600,
        BulletKind_4_Value = 9900,
        
        BulletOffsetValue_LikuiFish = 3300
    };
    
}

#endif
