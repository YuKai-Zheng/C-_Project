#pragma  once
#include <memory.h>
//#include "HNNetExport.h"
#include "HNBaseType.h"
#include "NSAnimationTool.h"

namespace NewYearActivityInGame
{

enum{
	S2C_SHOW_NIANSHOU_MSG = 150,				//年兽出现
	S2C_DRIVE_NIANSHOU_RESULT = 151,			//赶年兽获得金币
	S2C_MATCH_GAIN_BANGER_MSG = 152,			//对局获得鞭炮

	C2S_DRIVE_NIANSHOUR_REQUEST = 190,			//赶年兽请求
};
    
    const static int NianShouZOrder = 99999;
    const static Vec2 NianShouButtonPosition = Vec2(1080, 250);

//赶年兽请求
struct GanNianShouRequest
{
	int		nUserID;			//玩家id
	BYTE	bPropType;			//道具类型0~2
	BYTE	bSceneType;			//使用类型0~1
	GanNianShouRequest()
	{
		::memset(this, 0, sizeof(GanNianShouRequest));
	}
};

//赶年兽结果通知
struct GanNianShouRlt
{
	int				nUserID;			//玩家编号
	long long			nRewardMoney;		//赶年兽获得的金币数量
	GanNianShouRlt()
	{
		::memset(this, 0, sizeof(GanNianShouRlt));
	}
};

//对局获得爆竹结果通知 //每次一个中型
struct MatchFireCrackerRlt
{
	int				nUserID;			//玩家编号
	MatchFireCrackerRlt()
	{
		::memset(this, 0, sizeof(MatchFireCrackerRlt));
	}
};

}
