#ifndef __NEWYEAR_COMMON_DEFINE_h__
#define __NEWYEAR_COMMON_DEFINE_h__

#include <memory.h>
//#include "HNNetExport.h"
#include "HNBaseType.h"
#include <vector>
//#pragma pack(push,1)

namespace NewYearActivity
{
#define	WIN_SIZE	Director::getInstance()->getWinSize()
#define DESIGN_SIZE Size(1280, 720)
 
//签到特殊天数宝箱
	#define SPECIAL_DAY_COUNT 4
	static int g_BoxSpecialSignNum[SPECIAL_DAY_COUNT] = { 3, 7, 15, 30 };

	//////////////////////////////////////////////////////////////////////////
    //游戏信息
    enum
    {
        GAME_NAME_ID	=	11118888,
        GAME_KIND_ID	=	1,
		PLAYER_COUNT	=	180
	};
    
	enum Activity_Type
	{
		Activity_Null,
		Sign_Activity,
		DriveNianShou_Activity,
		Recharge_Activity
	};

	//消息id
	enum
	{
		C2S_ActivityType = 0,
		C2S_Sign = 1,

		
		S2C_SignResult = 3,
		S2C_DriveNianShouInfo = 4,

		//这两接口和游戏中一致
		S2C_DRIVE_NIANSHOU_RESULT = 151,				//赶年兽获得金币
		C2S_DRIVE_NIANSHOUR_REQUEST = 190,				//赶年兽请求
		C2S_DRIVE_TURNTABLE_REQUEST = 191,				//赶年兽请求
		C2S_DRIVE_ISTURNTABLE_TODAY = 192,
        S2C_SignInfo = 233,
	};


	//c2s********************************
	struct tag_c2s_Activity
	{
		int nActivityType;
		tag_c2s_Activity()
		{
			nActivityType = 0;
		}
	};

	//签到
	struct tag_c2s_Sign
	{
		int nSign;
		tag_c2s_Sign()
		{
			nSign = 0;
		}
	};

	//赶年兽请求
	struct GanNianShouRequest
	{
		int		nUserID;			//玩家id
		BYTE	bPropType;			//道具类型1~3
		BYTE	bSceneType;			//使用类型0~1
		GanNianShouRequest()
		{
			::memset(this, 0, sizeof(GanNianShouRequest));
		}
	};
	//s2c********************************
	struct tag_s2c_SignInfo
	{
		int nSignDay;
		int nIsTodaySign;
		tag_s2c_SignInfo()
		{
			nSignDay = 0;
			nIsTodaySign = 0;
		}
	};

	struct tag_s2c_SignResult
	{
		int nSignResult;
		tag_s2c_SignResult()
		{
			nSignResult = 0;
		}
	};

	struct tag_s2c_DriveNianShouInfo
	{
		int		 nMinNum;
		int		 nMidNum;
		int		 nMaxNum;

		tag_s2c_DriveNianShouInfo()
		{
			nMinNum = 0;
			nMidNum = 0;
			nMaxNum = 0;
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

	 //轮盘结果
	 struct GanTurntableRlt
	 {
		 INT			userId;
		 INT			nReward;		//奖品类型  0 - 11		 0/6 百万 1/7 1W	2/8	再来一次	3/9 10W 4/10 谢谢 5 /11 1W
		 GanTurntableRlt()
		 {
			 ::memset(this, 0, sizeof(GanTurntableRlt));
		 }
	 };

	 struct tag_c2s_TurnTableRequest
	 {
		 int		 nUserID;
		 tag_c2s_TurnTableRequest()
		 {
			 nUserID = 0;
		 }
	 };

	 //转盘判断 是否进行过游戏
	 struct GanTurn_Today
	 {
		 INT		 nResult;	//结果： 1-今天还没玩过转盘；2-其他账号今天已经玩过转盘；3-今天已经玩过转盘

		 GanTurn_Today()
		 {
			 nResult = 0;
		 }
	 };
}
//#pragma pack(pop)


#endif
