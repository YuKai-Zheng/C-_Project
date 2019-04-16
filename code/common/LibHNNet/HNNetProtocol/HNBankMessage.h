#ifndef __HNBankMessage_h__
#define __HNBankMessage_h__


#include "HNBaseType.h"
#include "HNCommon/HNCommonMarco.h"

//////////////////////////////////////////////////////////////////////////
#define		MDM_GP_BANK							121

//////////////////////////////////////////////////////////////////////////
//打开银行
#define		ASS_GP_BANK_OPEN					21			        //打开银行获取钱包信息

#define		HC_GP_BANK_OPEN_SUC					1			//打开钱柜成功
#define		HC_GP_BANK_OPEN_ERR					2			//打开钱柜错误
#define		HC_GP_BANK_OPEN_ERR_PSW				3		//打开钱框错误

// 客户端发送请求
typedef struct TMSG_GP_BankOpenReq
{
	char szMD5Pass[50]; //加密密码
} GNUC_PACKED TMSG_GP_BankOpenReq;


// 服务端返回
typedef struct TMSG_GP_BankInfo
{
	UINT	        dwUserID;
	LLONG			i64Bank;									//钱柜
	LLONG			i64Wallet;									//钱包的钱
	CHAR			szTwoPassword[50];					//银行密码
	INT				nVer;											//银行版本
} GNUC_PACKED TMSG_GP_BankInfo;

//////////////////////////////////////////////////////////////////////////
// 修改密码
#define		ASS_GP_BANK_CHPWD					28				   //修改银行密码
														    
#define		HC_GP_BANK_CHPWD_SUC				1				//修改成功
#define		HC_GP_BANK_CHPWD_ERR_PWD			2		    //修改失败，密码错误

//客户端发送请求
typedef struct TMSG_GP_BankChPwd
{
	UINT				user_id;									//用户ID
	CHAR			MD5Pass_old[50];					//银行密码旧
	CHAR			MD5Pass_new[50];					//银行密码新
} GNUC_PACKED TMSG_GP_BankChPwd;


//////////////////////////////////////////////////////////////////////////
//存取钱
//服务端返回
//没有数据，通过handlecode判断修改成功或失败

#define		ASS_GP_BANK_CHECK					23			  //银行存取钱

#define		HC_GP_BANK_CHECK_SUC				0				//操作成功
#define		HC_GP_BANK_CHECK_NO_USER			1				  //没有找到用户
#define		HC_GP_BANK_CHECK_ERR_PWD			2				  //错误密码
#define		HC_GP_BANK_CHECK_NOT_MONEY			4		    //余额不足

//客户端发送请求
typedef struct TMSG_GP_BankCheck
{
	INT				operate_type;	//1-取钱，2-存钱
	INT				user_id;
	INT				game_id;
	CHAR			szMD5Pass[50];  //银行密码
	LLONG			money;
} GNUC_PACKED TMSG_GP_BankCheck;
//服务端返回与发送一致
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//银行转账
#define		ASS_GP_BANK_TRANSFER					24		//银行转账

#define		HC_GP_BANK_TRANSFER_ERROR				1		//转帐错误
#define		HC_GP_BANK_TRANSFER_SUC					2		//转帐成功
#define		HC_GP_BANK_TRANSFER_NO_DEST				3		//转帐目标不存在
#define		HC_GP_BANK_TRANSFER_PASSWORD_ERROR		4		//转帐密码错误
#define		HC_GP_BANK_TRANSFER_TOTAL_LESS			5		//银行总额太小，不够资格
#define		HC_GP_BANK_TRANSFER_TOO_LESS			6		//单笔转帐数目太少
#define		HC_GP_BANK_TRANSFER_MULTIPLE			7		//单笔转帐数目必须是某数的倍数
#define		HC_GP_BANK_TRANSFER_NOT_ENOUGH			8		//银行金额不够本次转帐
#define		HC_GP_BANK_TRANSFER_TOO_MANY_TIME		9		//当天转账的次数太多了
#define		HC_GP_BANK_TRANSFER_UPDATE_REBATE		10		//更新返利数据
#define		HC_GP_BANK_TRANSFER_UPDATE_USER_REBATE	11		//更新玩家返利数据
#define		HC_GP_BANK_TRANSFER_SET_REBATE_RES		12		//修改返利数据
#define		HC_GP_BANK_TRANSFER_Disable_SXF			13		//禁止用户跨代理上下分
#define     HC_GP_BANK_TRANSFER_BIND_PHONE          14      //需要绑定手机才能下分

// 转帐
typedef struct TMSG_GP_BankTransfer
{
	int                             UserID;							//用户ID
	int                             destUserID;						//转给谁
	LLONG							i64Money;						//欲转帐金额
	LLONG							i64ActualTransfer;				//实际到帐金额
	INT								bTranTax;						//是否扣税，非0不扣税
	CHAR							szMD5Pass[50];					//银行密码
	CHAR							szNickName[50];                 //用户昵称
	CHAR							szDestNickName[50];             //被转账用户昵称
	bool							bUseDestID;
	UINT							uHandleCode;					//用来区分是否是“全转”功能
	 
	int								year;							//long:2016/6/26
	int								month;
	int								day;
	int								hour;
	int								minute;
	int								second;
	LLONG							TransTime;						 //转账时间
	LLONG							transid;						 //转账成功的编号
	LLONG							i64TaxMoney;					  //转账成功的手续费

	TMSG_GP_BankTransfer()
	{
		memset(this, 0x0, sizeof(TMSG_GP_BankTransfer));
	}
} GNUC_PACKED TMSG_GP_BankTransfer;

//////////////////////////////////////////////////////////////////////////
#define		ASS_GP_BANK_TRANS_RECORD			25		//银行转账记录
#define		HC_GP_BANK_TRANS_RECORD_SUC			1		//银行转账记录正常获取

/// 转帐记录查询请求
typedef struct TMSG_GP_BankTranRecordReq
{
	INT		dwUserID;	///用户 ID
	INT		pageSize;   //每页显示多少条记录
	INT		curPage;	//当前第几页
	INT		reqType;	//请求类型、PC端还是手机端、0是手机1是PC端

	TMSG_GP_BankTranRecordReq()
	{
		dwUserID = INVALID_USER_ID;
	}

} GNUC_PACKED TMSG_GP_BankTranRecordReq;



//用户转帐记录结果
typedef struct TMSG_GP_BankTranRecord
{	
	INT					nSeriNo;									// 序号
	UINT					dwUserID;									// 用户 ID
	UINT					destUserID;								// 转给谁
	LLONG				i64Money;									// 欲转帐金额
	LLONG				i64ActualTransfer;						// 实际到帐金额
	INT					year;											//long:日期
	INT					month;
	INT					day;
	INT					hour;
	INT					minute;
	INT					second;
	LLONG				oleDateTime;
	//BYTE                 hanldeType;							///< 操作类型
	//LLONG              handlePreMoney;                    ///< 操作前金额
	//LLONG              handleFinMoney;                    ///< 操作后金额
	CHAR				szNickName[50];						///用户昵称
	CHAR				szDestNickName[50];					///被转账用户昵称
	LLONG				i64BankMoney;							///赠送金身后银行的金币
	INT					totalRecord;
	INT					curPage;
	CHAR				szRemark[25];							///备注

	TMSG_GP_BankTranRecord()
	{
		memset(this, 0x0, sizeof(TMSG_GP_BankTranRecord));
	}
} GNUC_PACKED TMSG_GP_BankTranRecord;

typedef struct TMSG_GP_BankTranRecord_Result
{
	TMSG_GP_BankTranRecord items[10];
} GNUC_PACKED TMSG_GP_BankTranRecord_Result;

#endif // __HNBankMessage_h__
