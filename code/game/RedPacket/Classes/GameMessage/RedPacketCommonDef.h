#ifndef __RedPacket_COMMON_DEFINE_h__
#define __RedPacket_COMMON_DEFINE_h__

#include <memory.h>
#include "HNBaseType.h"
#include <vector>



//#pragma pack(push,1)

namespace RedPacket
{

#define	WIN_SIZE	Size(Director::getInstance()->getWinSize().height,Director::getInstance()->getWinSize().width)
#define DESIGN_SIZE Size(1280, 720)

    
	enum
	{
		S2C_NOTIFY_MSG = 149
	};
    
    //////////////////////////////////////////////////////////////////////////
    //游戏信息
    enum
    {
        GAME_NAME_ID	=	33001112,
        GAME_KIND_ID	=	1,
        PLAY_COUNT      =   20,
    };
    
    
#define Max_RobRedPacketCount		11		//最多抢红包的人数
    
    //群红包消息id
    enum
    {
        C2S_RedEnvelope_MsgType,						//起始消息号
        
        C2S_RedEnvelope_RedPacketListMsg,				//红包列表请求
        C2S_RedEnvelope_EmitRedPacketMsg,				//发红包
        C2S_RedEnvelope_NewRedPacketMsg,				//有新的红包通知
        C2S_RedEnvelope_RobRedPacketMsg,				//抢红包
        C2S_RedEnvelope_RobRedPacketLogMsg,				//红包被抢记录信息
        C2S_RedEnvelope_RobRedPacketLogMsgEx,			//红包被抢记录信息(附属信息回包)
        C2S_RedEnvelope_MyHistoryRobLogMsg,				//获取本人历史抢红包记录
        C2S_RedEnvelope_PlayerListMsg,					//玩家列表请求
        C2S_RedEnvelope_PlayerWalletMoneyMsg,			//玩家钱包金币请求
        C2S_RedEnvelope_BaseConfigMsg,					//红包基础配置信息请求
        C2S_RedEnvelope_RobRedPacketOverMsg,			//红包过期或者被抢完通知
        
        C2S_RedEnvelope_MsgType_Max,					//最大消息号
    };
    
    //红包列表回复包 (使用分包方式回复) <有新的红包通知也使用这个结构，此时不使用分包方式>
    struct tag_s2c_RedPacketListReq
    {
        unsigned int	nRedPacketID;				//红包的唯一ID
        int				nUserID;					//发红包的玩家ID
        char			szUserName[50];				//发红包的玩家昵称
        int				nIsRobot;					//是否是机器人	0-不是，1-是
        int				nLogoID;					//默认头像ID
        char			szWXTouXiang[200];			//微信头像
        unsigned int	nRedPacketBeginTime;		//发红包的时间
        unsigned int	nRedPacketEndTime;			//红包结束的时间
        
        tag_s2c_RedPacketListReq()
        {
            nRedPacketID = 0;
            nUserID = 0;
            memset(szUserName, 0, sizeof(szUserName));
            nIsRobot = 0;
            nLogoID = 0;
            memset(szWXTouXiang, 0, sizeof(szWXTouXiang));
            nRedPacketBeginTime = 0;
            nRedPacketEndTime = 0;
        }
    };
    
    //发红包请求
    struct tag_c2s_EmitRedPacketRes
    {
        LLONG			i64RedPacketMoney;		//发红包的金额
        int				nRedPacketPart;			//将红包分成几份
        int				nThunderNumber;			//雷红包的尾数
        
        tag_c2s_EmitRedPacketRes()
        {
            i64RedPacketMoney = 0;		//发红包的金额
            nRedPacketPart = 0;			//将红包分成几份
            nThunderNumber = 0;			//雷红包的尾数
        }
    };
    
    //发红包回复
    struct tag_c2s_EmitRedPacketReq
    {
        int		 nResult;			//发红包结果:	0-成功 1-金币不足 2-红包的份数不满足规则 3-雷的数字设定不满足规则 4-桌面红包数量过多 5-红包金额不在范围内
        
        tag_c2s_EmitRedPacketReq()
        {
            nResult = 0;
        }
    };
    
    //抢红包请求
    struct tag_c2s_RobRedPacketRes
    {
        unsigned int	nRedPacketID;				//红包的唯一ID
        
        tag_c2s_RobRedPacketRes()
        {
            nRedPacketID = 0;		//红包的唯一ID
        }
    };
    
    //抢红包回复
    struct tag_c2s_RobRedPacketReq
    {
        int				nResult;			//发红包结果:	0-成功 1-红包已被抢完 2-您的金币不足，拆不开这大红包！ 3-你已经抢过这个红包 4-不能抢自己的红包
        unsigned int	nRedPacketID;		//红包的唯一ID
        int				nIsThunder;			//是否有踩中雷  0-未踩中 1-踩中
        LLONG			i64Money;			//获得的金额
        
        tag_c2s_RobRedPacketReq()
        {
            nResult = 0;
            nRedPacketID = 0;
            nIsThunder = 0;
            i64Money = 0;
        }
    };
    
    //红包被抢记录信息请求
    struct tag_c2s_RedPacketRobLogRes
    {
        unsigned int	nRedPacketID;				//红包的唯一ID
        
        tag_c2s_RedPacketRobLogRes()
        {
            nRedPacketID = 0;		//红包的唯一ID
        }
    };
    
    //当前抢到红包的玩家(使用分包方式)
    struct RobRedPacketUserInfo
    {
        int				nUserID;					//抢红包的玩家ID
        char			szUserName[50];				//抢红包的玩家昵称
        int				nIsRobot;					//是否是机器人	0-不是，1-是
        int				nLogoID;					//默认头像ID
        char			szWXTouXiang[200];			//微信头像
        LLONG			i64RobMoney;				//已经被抢了几份
        unsigned int	nRobTime;					//抢红包的时间
        int				nIsThunder;					//是否有踩中雷 0-未踩中 1-踩中
        int				nIsLucky;					//是否是运气王 0-不是 1-是
        
        RobRedPacketUserInfo()
        {
            nUserID = 0;					//抢红包的玩家ID
            memset(szUserName, 0, sizeof(szUserName));		//抢红包的玩家昵称
            nIsRobot = 0;					//是否是机器人	0-不是，1-是
            nLogoID = 0;					//默认头像ID
            memset(szWXTouXiang, 0, sizeof(szWXTouXiang));	//微信头像
            i64RobMoney = 0;				//已经被抢了几份
            nRobTime = 0;					//抢红包的时间
            nIsThunder = 0;					//是否有踩中雷 0-未踩中 1-踩中
            nIsLucky = 0;					//是否是运气王 0-不是 1-是
        }
    };
    
    //红包被抢记录信息回复
    struct tag_c2s_RedPacketRobLogReq
    {
        unsigned int	nRedPacketID;				//红包的唯一ID
        int				nUserID;					//发红包的玩家ID
        char			szUserName[50];				//发红包的玩家昵称
        int				nIsRobot;					//是否是机器人	0-不是，1-是
        int				nLogoID;					//默认头像ID
        char			szWXTouXiang[200];			//微信头像
        LLONG			i64RedPacketMoney;			//发红包的金额
        LLONG			i64SurplusMoney;			//红包剩余未抢的金币
        int				nRedPacketPart;				//将红包分成几份
        int				nHaveRobPart;				//已经被抢了几份
        int				nThunderNumber;				//雷红包的尾数
        unsigned int	nSendRedPacketTime;			//发红包的时间
        
        //当前抢到红包的玩家
//        RobRedPacketUserInfo	mRobRedPacketUserInfo[Max_RobRedPacketCount];		//当前一个红包最多分成11份
        
        tag_c2s_RedPacketRobLogReq()
        {
            nRedPacketID = 0;		//红包的唯一ID
            nUserID = 0;
            memset(szUserName, 0, sizeof(szUserName));
            nIsRobot = 0;
            nLogoID = 0;
            memset(szWXTouXiang, 0, sizeof(szWXTouXiang));
            i64RedPacketMoney = 0;			//发红包的金额
            i64SurplusMoney = 0;			//红包剩余未抢的金币
            nRedPacketPart = 0;				//将红包分成几份
            nHaveRobPart = 0;				//已经被抢了几份
            nThunderNumber = 0;				//雷红包的尾数
            nSendRedPacketTime = 0;			//发红包的时间
//            memset(mRobRedPacketUserInfo, 0, sizeof(mRobRedPacketUserInfo));
        }
    };
    
    
    //获取本人历史抢红包记录回复(使用分包方式回复)
    struct tag_s2c_MyHistoryRobLogReq
    {
        unsigned int	nRedPacketID;				//红包的唯一ID
        int				nUserID;					//发红包的玩家ID
        char			szUserName[50];				//发红包的玩家昵称
        LLONG			i64RedPacketMoney;			//红包的总金额
        LLONG			i64GainMoney;				//红包收益
        int				nIsThunder;					//是否有踩中雷 0-未踩中 1-踩中
        int				nThunderCount;				//踩雷人数
        unsigned int	tLootTime;					//抢红包的时间
        
        tag_s2c_MyHistoryRobLogReq()
        {
            nRedPacketID = 0;
            nUserID = 0;
            memset(szUserName, 0, sizeof(szUserName));
            i64RedPacketMoney = 0;
            i64GainMoney = 0;
            nIsThunder = 0;
            nThunderCount = 0;
            tLootTime = 0;
        }
    };
    
    
    //玩家列表回复包 (使用分包方式回复)
    struct tag_s2c_PlayerListMsgReq
    {
        int				nUserID;					//玩家ID
        char			szUserName[50];				//玩家昵称
        int				nIsRobot;					//是否是机器人	0-不是，1-是
        int				nLogoID;					//默认头像ID
        char			szWXTouXiang[200];			//微信头像
        
        tag_s2c_PlayerListMsgReq()
        {
            nUserID = 0;
            memset(szUserName, 0, sizeof(szUserName));
            nIsRobot = 0;
            nLogoID = 0;
            memset(szWXTouXiang, 0, sizeof(szWXTouXiang));
        }
    };
    
    //玩家钱包金币请求
    struct tag_s2c_PlayerWalletMoneyMsgReq
    {
        LLONG		i64WalletMoney;
        
        tag_s2c_PlayerWalletMoneyMsgReq()
        {
            i64WalletMoney = 0;
        }
    };
    
    
    //红包基础配置信息请求
    struct tag_s2c_BaseConfigMsgReq
    {
        LLONG		i64MinMoney;		//红包的最小金额
        LLONG		i64MaxMoney;		//红包的最大金额
        int			nCanSplitCount;		//当前场次一个红包能拆分的数量
        int			nLeiRate;			//踩雷的倍率
        
        tag_s2c_BaseConfigMsgReq()
        {
            i64MinMoney = 0;
            i64MaxMoney = 0;
            nCanSplitCount = 0;
            nLeiRate = 0;
        }
    };

    //红包过期或者被抢完通知
    struct tag_s2c_RobRedPacketOverReq
    {
        unsigned int	nRedPacketID;				//红包的唯一ID
        int				nOverType;					//结束类型	1-被抢完，2-红包过期
        
        tag_s2c_RobRedPacketOverReq()
        {
            nRedPacketID = 0;
            nOverType = 0;
        }
    };

    //没钱退出
    struct tag_s2c_tip_exit
    {
        BYTE    bIsExit;
        long long i64Money;
        tag_s2c_tip_exit()
        {
            bIsExit = BYTE(-1);
            i64Money = 0;
        }
    };
	
}
//#pragma pack(pop)

#endif
