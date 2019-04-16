#include "HNPlatformLogic.h"
#include "HNCommon/HNConverCode.h"
#include "HNCommon/HNLog.h"
#include "HNData/HNGamesInfoModule.h"
#include "HNData/HNRoomInfoModule.h"
#include "HNData/HNUserInfoModule.h"
//#include "../../common/LibHNMarket/HNMarketExport.h"
#include "HNPlatformNotifyDelegate.h"
#include "HNRoom/HNRoomExport.h"
#include "HNSocket/HNSocketExport.h"
#include "ApplicationMacro.h"
#include "../LibHNLobby/HNLobby/PlatformConfig.h"
#include "HNRoomLogic/HNRoomLogicBase.h"
#include "HNLobby/pattern/CommandManager.h"
#include "HNLobby/globel.h"
#include "HNLobby/component/delegate/GameTypeDelegate.h"
#include "HNLobby/pattern/factory/gameType/IGameType.h"
#include "HNLobby/pattern/factory/gameType/GameTypeFactory.h"
#include "HNLobby/GameData.h"
#include "HNLobby/Shop/ShopManager.h"
#include "../LibHNMarket/HNOperator.h"
//#include "HNNetProtocol/HNPlatformMessage.h"

namespace HN {
static HNPlatformLogic *sHNPlatformLogic = nullptr;

#define MAX_HEARTBEAT_COUNT 2
//3.0f时间间隔
#define HEARTBEAT_INTERVAL  4.0f
#define RECONNECT_TIME      1.0f
#define RECONNECT_TIME_MAX  5.0f

HNPlatformLogic *HNPlatformLogic::getInstance() {
    if (nullptr == sHNPlatformLogic) {
        sHNPlatformLogic = new HNPlatformLogic();
        sHNPlatformLogic->init();
    }
    return sHNPlatformLogic;
}

HNPlatformLogic::HNPlatformLogic(void)
    : _loginResult(false), _heartBeatFrequency(0), _platformConnected(false),
      _platformPort(INVALID_VALUE), isStartCheck(false), isNeedReLogin(false), m_bReconnect(false){
    memset(&loginResult, 0x00, sizeof(loginResult));
    _socketLogic = new HNSocketLogic(this, "platform");
    _notifyQueue = new HNUIDelegateQueue<IPlatformMessageDelegate *>();
    _addQueue = new HNUIDelegateQueue<IPlatformMessageDelegate *>();
    _removeQueue = new HNUIDelegateQueue<IPlatformMessageDelegate *>();
}

HNPlatformLogic::~HNPlatformLogic(void) {
    HN_SAFE_DELETE(_removeQueue);
    HN_SAFE_DELETE(_addQueue);
    HN_SAFE_DELETE(_notifyQueue);
    HN_SAFE_DELETE(_socketLogic);
    stopHeartBeat();
}

bool HNPlatformLogic::init() {
    return true;
}

bool HNPlatformLogic::connectPlatform() {
	if (!m_bReconnect) {
		RoomInfoModule()->clear();
		GamesInfoModule()->clear();
	}
    _platformPort = getPlatformServerPort();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _platformHost = getPlatformServerHost();
//    _platformPort = 88;
    return _socketLogic->openWithIp(_platformHost.c_str(), _platformPort);
//#else
//    _platformAddress = getPlatformServerAddress();
//    return _socketLogic->openWithIp(_platformAddress.c_str(), _platformPort);
//#endif
}

bool HNPlatformLogic::closePlatform(bool bReconnect) {
    isStartCheck = false;
    _platformConnected = false;
    isNeedReLogin = false;
	m_bReconnect = bReconnect;
	stopHeartBeat();
	if(!bReconnect) Director::getInstance()->getScheduler()->unschedule(schedule_selector(HNPlatformLogic::reconnectSchedul), this);
    return _socketLogic->close();
}

bool HNPlatformLogic::connected() const {
    return _socketLogic->connected() && _platformConnected;
}

bool HNPlatformLogic::login() const {
    return _platformLogin;
}

bool HNPlatformLogic::loginSuccess() const {
    return _loginResult;
}

void HNPlatformLogic::sendData(UINT MainID, UINT AssistantID, void *object, INT objectSize) 
{
	if (isNeedReLogin)
	{
		return;
	}
	_socketLogic->send(MainID, AssistantID, HNSocketProtocolData::PlatformCheckCode, object, objectSize);
}

void HNPlatformLogic::sendData(UINT MainID, UINT AssistantID, void *object, INT objectSize, SEL_SocketMessage selector)
{
	if (isNeedReLogin)
	{
		return;
	}
	_socketLogic->send(MainID, AssistantID,HNSocketProtocolData::PlatformCheckCode, object,objectSize);

	_socketLogic->addEventSelector(MainID, AssistantID, selector);
}


void HNPlatformLogic::sendData(UINT MainID, UINT AssistantID, UINT AssistantIDFromSID, void *object, INT objectSize, SEL_SocketMessage selector)
{
	if (isNeedReLogin)
	{
		return;
	}
	CHAR messageKey[16] = { 0 };
	sprintf(messageKey, SELECTER_KEY_FORMAT, MainID, AssistantIDFromSID);

	_socketLogic->send(MainID, AssistantID, HNSocketProtocolData::PlatformCheckCode, object, objectSize);

	_socketLogic->addEventSelector(messageKey, selector);
}

void HNPlatformLogic::addEventSelector(UINT MainID, UINT AssistantID,SEL_SocketMessage selector)
{
    _socketLogic->addEventSelector(MainID, AssistantID, selector);
}

void HNPlatformLogic::removeEventSelector(UINT MainID, UINT AssistantID)
{
    _socketLogic->removeEventSelector(MainID, AssistantID);
}

void HNPlatformLogic::addObserver(IPlatformMessageDelegate *delegate)
{
    // delegate->retain();
    _addQueue->addObserver(delegate);
    //_notifyQueue->addObserver(delegate);

	startHeartBeat();
}

void HNPlatformLogic::removeObserver(IPlatformMessageDelegate *delegate)
{
    //_notifyQueue->activeObserver(delegate, true);
    _removeQueue->addObserver(delegate);
}

void HNPlatformLogic::heartBeat(float dt)
{
    if (isNeedReLogin)
	{
        onDisConnect();
        return;
    }

    if (isStartCheck)
	{
        time_t currTime = time(NULL);
        if (difftime(currTime, receive_time) > HEARTBEAT_INTERVAL) {
            //if (_socketLogic->getPlatformHeartBeatCount() > MAX_HEARTBEAT_COUNT) {
                _socketLogic->resumePlatformHearBeatCount();
                //stopHeartBeat();

				//重新连接
				reconnect();

                return;
            //}

            //if (connected()) {
            //    _socketLogic->addPlatformHearBeatCount();
            //}
        }
        else {
            if (connected()) {
                _socketLogic->resumePlatformHearBeatCount();
            }
        }
    }
}

void HNPlatformLogic::platformLogin(const std::string &name,
                                    const std::string &password, UINT uNameID) {
    MSG_GP_S_LogonByNameStruct data;
    data.uRoomVer = 4;
    data.iUserID = -1;
    data.gsqPs = 5471;
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
    data.uNameID = uNameID;
#endif
    strcpy(data.TML_SN, "EQ4gG6vEUL06ajaGn4EAuXDa662vaeeqL6UdoOQatxuujAlnqovO6Vnd"
           "vXT4Tv0l4a28XGoDxqde4El6XUAXLXe66lg2o6gQN4tlOgeAoV6gulE2"
           "jTNneUulE");
    strcpy(data.szName, name.c_str());
    strcpy(data.szMD5Pass, password.c_str());
    strcpy(data.szMathineCode, "000266-703028-103FA5-C05024-006BBE-007F50");
    strcpy(data.szCPUID, "612826255");
    strcpy(data.szHardID, "2222222");
    strcpy(data.szIDcardNo, "123456789");
    strcpy(data.szMathineCode, "123456789");
    strcpy(data.szIDcardNo, "*");
    strcpy(data.szMobileVCode, "*");
    strcpy(data.szUDID, UserDefault::getInstance()->getStringForKey("UUID").c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    strcpy(data.szImei, Operator::requestChannel("sysmodule", "GetIMEI").c_str());
    //网络类型,地区，平台类型
    strcpy(data.szNetType,StringUtils::format("%s,%s,%d",HN::Operator::requestChannel("sysmodule", "getNetType").c_str(),getPayType().c_str(),PlatformConfig::getInstance()->getPlatformType()).c_str());

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    strcpy(data.szImei,
           HN::Operator::requestChannel("sysmodule", "getimei").c_str());
    //网络类型,地区，平台类型
    strcpy(data.szNetType,StringUtils::format("%s,%s,%d",HN::Operator::requestChannel("sysmodule", "getNetType").c_str(),getPayType().c_str(),PlatformConfig::getInstance()->getPlatformType()).c_str());

    HN::HNLog::logInfo("====imei:%s",HN::Operator::requestChannel("sysmodule", "getimei").c_str());

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    strcpy(data.szImei, HN::Operator::requestChannel("sysmodule", "getIMEI").c_str());
    //网络类型,地区，平台类型
    strcpy(data.szNetType, StringUtils::format("%s,%s,%d", "1", getPayType().c_str(), PlatformConfig::getInstance()->getPlatformType()).c_str());
#endif

    sendData(MDM_GP_LOGON, ASS_GP_LOGON_BY_NAME, &data, sizeof(data));

	//保存账号密码
	_reconnectName = name;
	_reconnectPassword = password;
}

void HNPlatformLogic::platformRegister(BYTE byFastRegister, UINT uNameID,
                                       BYTE gender, const std::string &nickName,
                                       const std::string &usn,
                                       const std::string &name,
                                       const std::string &password,
                                       const std::string &inviteCode) {
    MSG_GP_S_Register data = {0};
    data.byFromPhone = 1;
    data.byFastRegister = byFastRegister;
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
    data.uNameID = uNameID;
#endif
    data.gender = gender;

    strcpy(data.szNickName, nickName.c_str());
    strcpy(data.szName, name.c_str());
    strcpy(data.szPswd, password.c_str());
    strcpy(data.szHardID, usn.c_str());
    strcpy(data.szUDID, UserDefault::getInstance()->getStringForKey("UUID").c_str());
    strcpy(data.szInviteCode, inviteCode.c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    strcpy(data.szImei,
           HN::Operator::requestChannel("sysmodule", "GetIMEI").c_str());
    //网络类型,地区，平台类型
    strcpy(data.szNetType, StringUtils::format("%s,%s,%d", HN::Operator::requestChannel("sysmodule", "getNetType").c_str(), getPayType().c_str(), PlatformConfig::getInstance()->getPlatformType()).c_str());
    strcpy(data.szIP,
           HN::Operator::requestChannel("sysmodule", "getNetIP").c_str());

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    strcpy(data.szImei,
           HN::Operator::requestChannel("sysmodule", "getimei").c_str());
    strcpy(data.szIP,
           HN::Operator::requestChannel("sysmodule", "getNetIp").c_str());
    //网络类型,地区，平台类型
    strcpy(data.szNetType,StringUtils::format("%s,%s,%d",HN::Operator::requestChannel("sysmodule", "getNetType").c_str(),getPayType().c_str(),PlatformConfig::getInstance()->getPlatformType()).c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    strcpy(data.szImei, HN::Operator::requestChannel("sysmodule", "getIMEI").c_str());
    strcpy(data.szIP, "");
    //网络类型,地区，平台类型
    strcpy(data.szNetType, StringUtils::format("%s,%s,%d", "1", getPayType().c_str(), PlatformConfig::getInstance()->getPlatformType()).c_str());
#endif

    PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_REGISTER, &data,
                              sizeof(data));
}

void HNPlatformLogic::reconnect()
{
	stopHeartBeat();
	m_connectTimes = 0;
	m_reconnect_time = time(NULL);
	CommandManager::getInstance()->execute(ADD_LOADER_COM);

	Director::getInstance()->getScheduler()->schedule(schedule_selector(HNPlatformLogic::reconnectSchedul), this, RECONNECT_TIME, kRepeatForever, 0.0f, false);

}

void HNPlatformLogic::reconnectSchedul(float ft)
{
	time_t currTime = time(NULL);
	if (difftime(currTime, m_reconnect_time) > RECONNECT_TIME_MAX)
	{
		stopHeartBeat();
		closePlatform();
		CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
		Director::getInstance()->getScheduler()->unschedule(schedule_selector(HNPlatformLogic::reconnectSchedul), this);
		dispatchMessage([](IPlatformMessageDelegate *delegate) -> bool {
			delegate->onHandleDisConnect();
			return false;
		});

		if (RoomLogic()->isReconnect())
			RoomLogic()->reconnectTimeOut();

		return;
	}

	closePlatform(true);
	connectPlatform();
	CommandManager::getInstance()->execute(ADD_LOADER_COM);

}

void HNPlatformLogic::platformReconnect()
{
	platformLogin(_reconnectName, _reconnectPassword, GameCreator()->getCurrentGameNameID());
}

void HNPlatformLogic::platformTouristRegister(BYTE byFastRegister, UINT uNameID,
        BYTE gender, const std::string &nickName,
        const std::string &usn,
        const std::string &name,
        const std::string &password) {
    MSG_GP_S_Register data = { 0 };
    data.byFromPhone = 1;
    data.byFastRegister = byFastRegister;
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
    data.uNameID = uNameID;
#endif
    data.gender = gender;

    strcpy(data.szNickName, nickName.c_str());
    strcpy(data.szName, name.c_str());
    strcpy(data.szPswd, password.c_str());
    strcpy(data.szHardID, usn.c_str());
    strcpy(data.szUDID, UserDefault::getInstance()->getStringForKey("UUID").c_str());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    strcpy(data.szImei,
           HN::Operator::requestChannel("sysmodule", "GetIMEI").c_str());
    strcpy(data.szIP,
           HN::Operator::requestChannel("sysmodule", "getNetIP").c_str());
    //网络类型,地区，平台类型
    strcpy(data.szNetType,StringUtils::format("%s,%s,%d",HN::Operator::requestChannel("sysmodule", "getNetType").c_str(),getPayType().c_str(),PlatformConfig::getInstance()->getPlatformType()).c_str());

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    strcpy(data.szImei,
           HN::Operator::requestChannel("sysmodule", "getimei").c_str());
    strcpy(data.szIP,
           HN::Operator::requestChannel("sysmodule", "getNetIp").c_str());
    //网络类型,地区，平台类型
    strcpy(data.szNetType,StringUtils::format("%s,%s,%d",HN::Operator::requestChannel("sysmodule", "getNetType").c_str(),getPayType().c_str(),PlatformConfig::getInstance()->getPlatformType()).c_str());
    HN::HNLog::logInfo("====imei:%s",HN::Operator::requestChannel("sysmodule", "getimei").c_str());

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    strcpy(data.szImei, HN::Operator::requestChannel("sysmodule", "getIMEI").c_str());
    strcpy(data.szIP, "");
    //网络类型,地区，平台类型
    strcpy(data.szNetType, StringUtils::format("%s,%s,%d", "1", getPayType().c_str(), PlatformConfig::getInstance()->getPlatformType()).c_str());
#endif
    PlatformLogic()->sendData(MDM_GP_IMEI, ASS_GP_IMEI, &data,
                              sizeof(data));
}

    void HNPlatformLogic::wechatTouristRegister(BYTE byFastRegister,
                                                UINT uNameID,
                                                BYTE gender,
                                                const std::string &nickName,
                                                const std::string &usn,
                                                const std::string &unionID,
                                                const std::string &imgUrl, 
                                                const std::string &name,
                                                const std::string &password) {
        
        MSG_GP_S_Register data = { 0 };
        data.byFromPhone = 2;   //微信注册
        data.byFastRegister = byFastRegister;
#if (HN_TAREGET_PLATFORM == HN_TARGET_PHONE_ONLY)
        data.uNameID = uNameID;
#endif
        data.gender = gender;
        
        strcpy(data.szNickName, Utf8ToGB(nickName.c_str()));
        strcpy(data.szName, name.c_str());
        strcpy(data.szPswd, password.c_str());
        strcpy(data.szHardID, usn.c_str());
        strcpy(data.szUDID, unionID.c_str());   //微信注册与本机登录复用相同字段
        strcpy(data.szWXTouXiang, imgUrl.c_str());
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        strcpy(data.szImei,
               HN::Operator::requestChannel("sysmodule", "GetIMEI").c_str());
        strcpy(data.szIP,
               HN::Operator::requestChannel("sysmodule", "getNetIP").c_str());
        //网络类型,地区，平台类型
        strcpy(data.szNetType,StringUtils::format("%s,%s,%d",HN::Operator::requestChannel("sysmodule", "getNetType").c_str(),getPayType().c_str(),PlatformConfig::getInstance()->getPlatformType()).c_str());
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        strcpy(data.szImei,
               HN::Operator::requestChannel("sysmodule", "getimei").c_str());
        strcpy(data.szIP,
               HN::Operator::requestChannel("sysmodule", "getNetIp").c_str());
        //网络类型,地区，平台类型
        strcpy(data.szNetType,StringUtils::format("%s,%s,%d",HN::Operator::requestChannel("sysmodule", "getNetType").c_str(),getPayType().c_str(),PlatformConfig::getInstance()->getPlatformType()).c_str());
        HN::HNLog::logInfo("====imei:%s",HN::Operator::requestChannel("sysmodule", "getimei").c_str());
        
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        strcpy(data.szImei, HN::Operator::requestChannel("sysmodule", "getIMEI").c_str());
        strcpy(data.szIP, "");
        //网络类型,地区，平台类型
        strcpy(data.szNetType, StringUtils::format("%s,%s,%d", "1", getPayType().c_str(), PlatformConfig::getInstance()->getPlatformType()).c_str());
#endif
        PlatformLogic()->sendData(MDM_GP_IMEI, ASS_GP_IMEI, &data, sizeof(data));
        
    }
    
void HNPlatformLogic::requestRoomListInfo(UINT uKindID, UINT uNameID) {
    RoomInfoModule()->clear();
    MSG_GP_SR_GetRoomStruct GetRoomStruct;
    GetRoomStruct.uKindID = uKindID;
    GetRoomStruct.uNameID = uNameID;
    sendData(MDM_GP_LIST, ASS_GP_LIST_ROOM, &GetRoomStruct,
             sizeof(GetRoomStruct));
}
    
void HNPlatformLogic::requestAllRoomListInfo()
{
    RoomInfoModule()->clear();
    PlatformLogic()->sendData(MDM_GP_LIST, ASS_GP_LIST_NAME);
}

void HNPlatformLogic::onHearBeat() {
    if (!isStartCheck) {
        isStartCheck = true;
    }
    if(isStartCheck){
        receive_time = time(NULL);
    }
    //  HNLog::logInfo("receive platform heart beat");
}

void HNPlatformLogic::onConnected(bool connect, emSocketStatus status) 
{

    HNLog::logInfoCn(StringUtils::format("=======================大厅连接状态：%s，状态值：%d",(connect ? ("已连接") : ("未连接")), status));
    
	dispatchMessage([status](IPlatformMessageDelegate *delegate) -> bool {
        delegate->onHandleConnectMessage(SocketStatus_STCCESS == status);
        return false;
    });

	//连接状态
	if (SocketStatus_STCCESS == status)
	{
		if (connected()) platformReconnect();
	}
	else
	{
		time_t currTime = time(NULL);
		if (m_bReconnect && difftime(currTime, m_reconnect_time) > RECONNECT_TIME)
		{
			stopHeartBeat();
			closePlatform();
			CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
			Director::getInstance()->getScheduler()->unschedule(schedule_selector(HNPlatformLogic::reconnectSchedul), this);
			dispatchMessage([](IPlatformMessageDelegate *delegate) -> bool {
				delegate->onHandleDisConnect();
				return false;
			});

			if (RoomLogic()->isReconnect())
				RoomLogic()->reconnectTimeOut();
		}
		else
		{
			reconnect();
		}
	}
}

void HNPlatformLogic::onDisConnect() {
    HNLog::logInfoCn(StringUtils::format("=======================大厅断线"));

    stopHeartBeat();
    //_platformConnected = false;
    
    //if (RoomLogic()->connected()) { //临时加的
    //    RoomLogic()->onDisConnect();
    //}

    //dispatchMessage([](IPlatformMessageDelegate *delegate) -> bool {
    //    delegate->onHandleDisConnect();
    //    return false;
    //});
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	reconnect();
#endif
}

void HNPlatformLogic::onSocketMessage(HNSocketMessage *socketMessage)
{
    if (MDM_CONNECT == socketMessage->messageHead.bMainID && ASS_NET_TEST_1 == socketMessage->messageHead.bAssistantID)
	{
        //_socketLogic->heartBeat(MDM_CONNECT, ASS_NET_TEST_1,
        // HNSocketProtocolData::PlatformCheckCode);
        HNLog::logInfo("receive ASS_NET_TEST_1");
        return;
    }

	HNLog::logInfoCn(StringUtils::format("=======================platform-main:%d,sub:%d",socketMessage->messageHead.bMainID,socketMessage->messageHead.bAssistantID));

    if (MDM_CONNECT == socketMessage->messageHead.bMainID && ASS_NET_TEST_2 == socketMessage->messageHead.bAssistantID)
	{
        _heartBeatFrequency--;
        return;
    }
    // main id
    switch (socketMessage->messageHead.bMainID)
	{
    // platform connect
    case MDM_GP_CONNECT:
	{
		HNLog::logInfoCn(StringUtils::format("=======================大厅连接成功消息"));
		onConnectMessage(&socketMessage->messageHead, socketMessage->object,socketMessage->objectSize);

		if (m_bReconnect && connected())
			platformReconnect();

		startHeartBeat();
	}  
    break;
    // platform login
    case MDM_GP_LOGON:
	{
		HNLog::logInfoCn(StringUtils::format("=======================登录大厅消息"));
		onLoginMessage(&socketMessage->messageHead, socketMessage->object,socketMessage->objectSize);
	}
	break;
    // platform rooms list
    case MDM_GP_LIST:
	{
		HNLog::logInfoCn(StringUtils::format("=======================房间列表消息"));
		onRoomListMessage(&socketMessage->messageHead, socketMessage->object,socketMessage->objectSize);
	}
    break;
    // platform all user count
    case MDM_GP_LOGONUSERS:
	{
		HNLog::logInfoCn("=======================统计在线玩家人数消息");
		onUpdateOnLineUserMessage(&socketMessage->messageHead,socketMessage->object, socketMessage->objectSize);
	}
    break;
    // platform register
    case MDM_GP_REGISTER:
	{
		HNLog::logInfoCn(StringUtils::format("=======================注册消息"));
		onUserRegisterMessage(&socketMessage->messageHead, socketMessage->object,socketMessage->objectSize);
	}
    break;
    case MDM_GP_PROP:
        HNLog::logInfoCn(StringUtils::format("=======================道具消息"));
        onPropMessage(&socketMessage->messageHead, socketMessage->object,socketMessage->objectSize);
        break;
    case MDM_GP_MESSAGE:
	{
		HNLog::logInfoCn(StringUtils::format("=======================系统消息"));
		onSystemMessage(&socketMessage->messageHead, socketMessage->object,socketMessage->objectSize);
	}
    break;
    case MDM_GP_USER_MONEY:
	{ //在游戏中进行银行操作 单向发包 维护 下面两个数据值
        HNLog::logInfoCn(StringUtils::format("=======================银行消息"));

        MSG_GP_Get_User_WalletMoney_BankMoney *data = (MSG_GP_Get_User_WalletMoney_BankMoney *)socketMessage->object;
        PlatformLogic()->getInstance()->loginResult.i64Money = data->i_Wallet;
        PlatformLogic()->getInstance()->loginResult.i64Bank = data->i_Bank;
    }
	break;//add by zengwc;
    case MDM_GP_IMEI:
	{ // 游客登录 2016 9 21 wzx
		HNLog::logInfoCn(StringUtils::format("=======================游客登录"));
        onUserRegisterMessage(&socketMessage->messageHead, socketMessage->object,socketMessage->objectSize);
    }
    break;
    default:
        break;
    }
    HNLog::logInfoCn(StringUtils::format("\n"));
}

void HNPlatformLogic::onConnectMessage(NetMessageHead *messageHead,void *object, INT objectSize)
{
    // 效验数据
    CHECK_SOCKET_DATA(MSG_S_ConnectSuccess, objectSize,"MSG_S_ConnectSuccess size is error.");

    MSG_S_ConnectSuccess *pConnectSuccess = (MSG_S_ConnectSuccess *)object;

    HNSocketProtocolData::PlatformCheckCode =((UINT)pConnectSuccess->i64CheckCode - getSecrectKey()) / 23;
	
	HNLog::logInfoCn(StringUtils::format("HNSocketProtocolData::PlatformCheckCode >> [%d]", HNSocketProtocolData::PlatformCheckCode));

    HNLOG_WARNING("PlatformCheckCode >> [%d]",HNSocketProtocolData::PlatformCheckCode);

    _platformConnected = true;

    dispatchMessage([&](IPlatformMessageDelegate *delegate) -> bool {
        delegate->onHandleConnectMessage(_platformConnected);
        return false;
    });
}

void HNPlatformLogic::onLoginMessage(NetMessageHead *messageHead, void *object,
                                     INT objectSize) {
    std::string mobileID("");
    
    switch (messageHead->bAssistantID)
	{
    case ASS_GP_LOGON_SYSTEM_INFO:
	{
		MSG_GP_S2C_SystemInfo *info = (MSG_GP_S2C_SystemInfo*)object;
		systemInfo = *info;
		break;
	}
    case ASS_GP_LOGON_SUCCESS:
    case ASS_GP_LOGON_ERROR: 
	{
        if (ERR_GP_LOGON_SUCCESS == messageHead->bHandleCode)
		{
            // 效验数据
            CHECK_SOCKET_DATA(MSG_GP_R_LogonResult, objectSize,
                              "MSG_GP_R_LogonResult size is error.");

            MSG_GP_R_LogonResult *pLogonResult = (MSG_GP_R_LogonResult *)object;
            loginResult = *pLogonResult;
            
            UserDefault::getInstance()->setIntegerForKey("CUSTOMER_USERID_LOCAL",loginResult.dwUserID);
            //  登录成功以后请求公告信息 by HBC 20170424
            sendData(MDM_GP_LIST, ASS_GP_GET_NOTICELIST);
            
            //请求商品信息
            ProductManger::getInstance()->addProducts(getPayType());

			//重连需发送列表
			if (m_bReconnect) {
				m_bReconnect = false;

				sendData(MDM_GP_LIST, ASS_GP_LIST_KIND);

				CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
				Director::getInstance()->getScheduler()->unschedule(schedule_selector(HNPlatformLogic::reconnectSchedul), this);
			}
        }
        else if (ERR_GP_MOBILE_VERIFY == messageHead->bHandleCode)
        {
            
            auto size = sizeof(MSG_GP_R_MobileVerify);
            if (objectSize != sizeof(MSG_GP_R_MobileVerify))
            {
                
                return ;
            }
            auto msg = (MSG_GP_R_MobileVerify*)object;
            
            mobileID = msg->szMobileNo;
            loginResult.dwUserID   = msg->dwUserID;
            loginResult.iBindMobile = 1;
            strcpy(loginResult.szMobileNo, msg->szMobileNo);
//            
//            auto bindPhone = BindPhoneGui::create();
//            bindPhone->SetOpType(BindPhoneOpType::ChangeBind, false, msg->dwUserID, msg->szMobileNo);
//            bindPhone->Open(this, "验证手机号码", WIN_SIZE/2, 100, 0);
        }

        _platformLogin = (ERR_GP_LOGON_SUCCESS == messageHead->bHandleCode);

        dispatchMessage([&](IPlatformMessageDelegate *delegate) -> bool {
            delegate->onHandleLoginMessage(_platformLogin, messageHead->bHandleCode, mobileID);
            return false;
        });
    }
    break;
    default:
        break;
    }
}

void HNPlatformLogic::onRoomListMessage(NetMessageHead *messageHead,
                                        void *object, INT objectSize) {
    if (ASS_GP_LIST_KIND == messageHead->bAssistantID) { // 游戏类型列表
        ComKindInfo *kingInfo = (ComKindInfo *)object;
        INT Count = objectSize / sizeof(ComKindInfo);
        Count = Count < 0 ? 0 : Count;
        while (Count-- > 0) {
            ComKindInfo *tmp = kingInfo++;
            GamesInfoModule()->addGameKind(tmp);
        }

		if (RoomLogic()->isReconnect())
			RoomLogic()->reconnect(true);

    } else if (ASS_GP_LIST_NAME == messageHead->bAssistantID) { // 游戏名称列表
        cocos2d::log("recv game list");

        ComNameInfo *nameInfo = (ComNameInfo *)object;
        INT Count = objectSize / sizeof(ComNameInfo);
        Count = Count < 0 ? 0 : Count;

        cocos2d::log("count>%d", Count);

        while (Count-- > 0) {
            ComNameInfo *tmp = nameInfo++;
            GamesInfoModule()->addGameName(tmp);
        }

        if (ERR_GP_LIST_FINISH == messageHead->bHandleCode)
		{
            dispatchMessage([](IPlatformMessageDelegate *delegate) -> bool{
                delegate->onHandleGameListMessage();
                return false;}
			);
        }
    } else if (ASS_GP_LIST_COUNT == messageHead->bAssistantID) { //大厅房间人数
        if (objectSize < sizeof(DL_GP_RoomListPeoCountStruct))
            return;

        if (messageHead->bHandleCode == 0) { // 大厅房间人数
            DL_GP_RoomListPeoCountStruct *PeoCountStruct =
                (DL_GP_RoomListPeoCountStruct *)object;
            INT Count = objectSize / sizeof(DL_GP_RoomListPeoCountStruct);
            Count = Count < 0 ? 0 : Count;
            while (Count-- > 0) {
                DL_GP_RoomListPeoCountStruct *tmp = PeoCountStruct++;
                ComRoomInfo *pRoom = RoomInfoModule()->getByRoomID(tmp->uID);
                if (nullptr != pRoom) {
                    pRoom->uPeopleCount = tmp->uOnLineCount;
                    pRoom->uVirtualUser = tmp->uVirtualUser;
                    dispatchMessage([&tmp](IPlatformMessageDelegate *delegate) -> bool {
                        delegate->onHandleRoomUserCountMessage(tmp->uID, tmp->uOnLineCount,
                        tmp->uVirtualUser);
                        return false;
                    });
                }
            }
        } else { // 游戏名称人数
            DL_GP_RoomListPeoCountStruct *PeoCountStruct =
                (DL_GP_RoomListPeoCountStruct *)object;
            INT Count = objectSize / sizeof(DL_GP_RoomListPeoCountStruct);
            Count = Count < 0 ? 0 : Count;
            while (Count-- > 0) {
                DL_GP_RoomListPeoCountStruct *tmp = PeoCountStruct++;
                dispatchMessage([&tmp](IPlatformMessageDelegate *delegate) -> bool {
                    delegate->onHandleGameUserCountMessage(tmp);
                    return false;
                });
            }
        }
    } else if (ASS_GP_LIST_ROOM == messageHead->bAssistantID) {
        if (objectSize >= sizeof(MSG_GP_SR_GetRoomStruct)) {
            MSG_GP_SR_GetRoomStruct *pGetRoomStruct = (MSG_GP_SR_GetRoomStruct *)object;

            // 跳过房间ComRoomInfo数据前包含MSG_GP_SR_GetRoomStruct结构体，要跳过MSG_GP_SR_GetRoomStruct头。
            CHAR *pData = ((CHAR *)object) + sizeof(MSG_GP_SR_GetRoomStruct);
            INT userCount =  (objectSize - sizeof(MSG_GP_SR_GetRoomStruct)) / sizeof(ComRoomInfo);
            if (userCount > 0) {
                //RoomInfoModule()->clear();
                ComRoomInfo *pComRoomInfo = (ComRoomInfo *)(pData);
                INT i = 0;
                while (i++ < userCount) {
                    RoomInfoModule()->addRoom(pComRoomInfo++);
                }
            }

            if (ERR_GP_LIST_FINISH == messageHead->bHandleCode) {
                dispatchMessage([](IPlatformMessageDelegate *delegate) -> bool {
                    delegate->onHandleRoomListMessage();
                    return false;
                });
            }

            if (UserDefault::getInstance()->getBoolForKey("NEW_YEAR")) {
                ComRoomInfo* roomInfo = RoomInfoModule()->getRoom(0);
                if (roomInfo) {
                    HNRoomLogicBase* _roomLogic = new HNRoomLogicBase(new IHNRoomLogicBase());
                    _roomLogic->requestLogin(roomInfo->uRoomID);
                    RoomLogic()->setSelectedRoom(roomInfo);
                    RoomLogic()->connectRoom(roomInfo->szServiceIP, roomInfo->uServicePort);
                } else {
                    UserDefault::getInstance()->setBoolForKey("NEW_YEAR", false);
                    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
                }
            }

        }
    } else if (ASS_GP_GET_NOTICELIST == messageHead->bAssistantID) {
        if (objectSize < sizeof(MSG_GetNoticeListInfo))
            return;
        
        MSG_GetNoticeListInfo *nameInfo = (MSG_GetNoticeListInfo *)object;
        INT Count = objectSize / sizeof(MSG_GetNoticeListInfo);
        Count = Count < 0 ? 0 : Count;

        
        for (int i = 0; i<Count; ++i)
        {
            MSG_GetNoticeListInfo *tmp = nameInfo++;
            std::string str = tmp->szNoticeStr;
            GameData::getInstance()->appendingNoticeData(str);
            
        }
        
        if (ERR_GP_LIST_FINISH == messageHead->bHandleCode) {
            GameData::getInstance()->spliteNoticeData();
            dispatchMessage(
                            [&nameInfo, &messageHead](IPlatformMessageDelegate *delegate) -> bool {
                                if (delegate)
                                    delegate->onHandlePlatformNotice(nameInfo->szNoticeStr);
                                return false;
                            });
            
        }
    

    }
}

void HNPlatformLogic::onUpdateOnLineUserMessage(NetMessageHead *messageHead,void *object, INT objectSize)
{
    if (messageHead->bAssistantID == ASS_GP_LOGONUSERS_COUNT) 
	{
        // 效验数据
        CHECK_SOCKET_DATA(ONLINEUSERSCOUNT, objectSize,"ONLINEUSERSCOUNT size is error.");

        ONLINEUSERSCOUNT *ponlineCount = (ONLINEUSERSCOUNT *)object;

        RoomInfoModule()->setOnlineAllCount(ponlineCount->uiLogonPeopCount);

        dispatchMessage(
        [&ponlineCount](IPlatformMessageDelegate *delegate) -> bool {
            delegate->onHandlePlatformUserCountMessage(
                ponlineCount->uiLogonPeopCount);
            return false;
        });
    }
}

void HNPlatformLogic::onUserRegisterMessage(NetMessageHead *messageHead,
        void *object, INT objectSize) {
    // 效验数据
    CHECK_SOCKET_DATA(MSG_GP_S_Register, objectSize,
                      "MSG_GP_S_Register size is error.");
    MSG_GP_S_Register *userRegister = (MSG_GP_S_Register *)object;
    dispatchMessage([&userRegister,
    &messageHead](IPlatformMessageDelegate *delegate) -> bool {
        delegate->onHandleUserRegisterMessage(userRegister,
        messageHead->bHandleCode);
        return false;
    });
}

void HNPlatformLogic::onPropMessage(NetMessageHead *messageHead, void *object,
                                    INT objectSize) {
    switch (messageHead->bAssistantID) {
    case ASS_PROP_GETUSERPROP:
        break;
    default:
        break;
    }
}

void HNPlatformLogic::onSystemMessage(NetMessageHead *messageHead, void *object,
                                      INT objectSize) {
    switch (messageHead->bAssistantID) {
    case ASS_GP_NEWS_SYSMSG:
        {
            HN::HNLog::logInfoCn(StringUtils::format("-------------------收到系统广播消息-------------"));
        MSG_GR_RS_NormalTalk *pData = (MSG_GR_RS_NormalTalk *)object;
        dispatchMessage(
        [&pData, &messageHead](IPlatformMessageDelegate *delegate) -> bool {
            delegate->onNewsMessage(pData);
            return false;
        });
        }
            break;
    case ASS_UPDATE_JACKPOTINFO:
        {
            MSG_S2C_JACKPOTINFO *pData = (MSG_S2C_JACKPOTINFO*)object;
            
            for (int i = 0; i < GAME_COUNT; i++) {
                 GameData::getInstance()->setGameJackPotData(i,pData->i64Jackpots[i]);
            }
            
        }
            break;
    case ASS_GP_ONLINE_USER_COUNT:
        {
            HN::HNLog::logInfoCn(StringUtils::format("收到在线人数数据！"));
        }
        break;
    default:
            HN::HNLog::logInfoCn(StringUtils::format("系统消息: %d 无人订阅!", messageHead->bAssistantID));
        break;
    }
}

bool HNPlatformLogic::dispatchMessage(const PlatformNotifyFun &func) {
    // add new observer.
    auto iterA = _addQueue->begin();
    while (iterA != _addQueue->end()) {
        auto next = iterA;
        next++;
        _notifyQueue->addObserver(iterA->second);
        _addQueue->removeObserver(iterA->second);
        iterA = next;
    }

    // remove not active observer.
    auto iterR = _removeQueue->begin();
    while (iterR != _removeQueue->end()) {
        auto next = iterR;
        next++;
        _notifyQueue->removeObserver(iterR->second);
        _removeQueue->removeObserver(iterR->second);
        iterR = next;
    }

    // dispatch message
    for (auto iter = _notifyQueue->begin(); iter != _notifyQueue->end(); iter++) {
        bool removed = false;
        for (auto iterR = _removeQueue->begin(); iterR != _removeQueue->end();
                iterR++) {
            if (iterR->second == iter->second) {
                removed = true;
                break;
            }
        }
        if (!removed) {
            func(iter->second);
        }
    }
    return true;
}

void HNPlatformLogic::startHeartBeat() {
    isNeedReLogin = false;
    Director::getInstance()->getScheduler()->schedule(schedule_selector(HNPlatformLogic::heartBeat), this, 0.5f, kRepeatForever, 0.0f, false);
	Director::getInstance()->getScheduler()->unschedule(schedule_selector(HNPlatformLogic::reconnectSchedul), this);
}

void HNPlatformLogic::stopHeartBeat() {
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(HNPlatformLogic::heartBeat), this);
}

}
