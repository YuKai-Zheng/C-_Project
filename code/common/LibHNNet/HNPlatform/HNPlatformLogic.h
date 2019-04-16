#ifndef __HN_HNPlatformLogic_H__
#define __HN_HNPlatformLogic_H__

#include "HNCommon/HNCommonMarco.h"
#include "HNCommon/HNUIDelegateQueue.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "HNSocket/HNSocketMessage.h"
#include "HNSocket/HNSocketMessageDelegate.h"
#include "cocos2d.h"
#include <vector>

#include "HNCommon/HNLog.h"
#include <time.h>

USING_NS_CC;

namespace HN {
class IPlatformMessageDelegate;
class HNSocketLogic;

typedef std::function<bool(IPlatformMessageDelegate *)> PlatformNotifyFun;

class HNPlatformLogic : public Ref, ISocketMessageDelegate {

public:
    // returns the login information platform
    MSG_GP_R_LogonResult loginResult;
    MSG_GP_S2C_SystemInfo systemInfo; //qq配置
public:
public:
    static HNPlatformLogic *getInstance();

public:
    bool init();

public:
    // connect platform function
    bool connectPlatform();

    // close platform function
    bool closePlatform(bool bReconnect = false);

    // platform connecting success function
    bool connected() const;

    // platform login success function
    bool login() const;

    // platform login is success function
    bool loginSuccess() const;

    // send data
    void sendData(UINT MainID, UINT AssistantID, void *object = nullptr,INT objectSize = 0);

    // send data
    void sendData(UINT MainID, UINT AssistantID, void *object, INT objectSize,SEL_SocketMessage selector);

	void sendData(UINT MainID, UINT AssistantID, UINT AssistantIDFromSID,void *object, INT objectSize,SEL_SocketMessage selector);

    // set handler callback event
    void addEventSelector(UINT MainID, UINT AssistantID,SEL_SocketMessage selector);

    // remove handler callback event
    void removeEventSelector(UINT MainID, UINT AssistantID);

public:
    // add a network event viewer function
    void addObserver(IPlatformMessageDelegate *delegate);

    // delete a network event viewer function
    void removeObserver(IPlatformMessageDelegate *delegate);


public:
    // get the room list function
    void requestRoomListInfo(UINT uKindID, UINT uNameID);
    
    void requestAllRoomListInfo();
    
    // platform login function
    void platformLogin(const std::string &name, const std::string &password,UINT uNameID);

    // platform register
    void platformRegister(BYTE byFastRegister, UINT uNameID, BYTE gender,
                          const std::string &nickName, const std::string &usn,
                          const std::string &name = "",
                          const std::string &password = "",
                          const std::string &inviteCode = "");

	//大厅断线重连
	void reconnect();
	void platformReconnect();
	void reconnectSchedul(float ft);

    // platform Tourist register
    void platformTouristRegister(BYTE byFastRegister, UINT uNameID, BYTE gender, const std::string &nickName, const std::string &usn, const std::string &name = "", const std::string &password = "");
    // wechat register
    void wechatTouristRegister(BYTE byFastRegister, UINT uNameID, BYTE gender, const std::string &nickName, const std::string &usn, const std::string &unionID, const std::string &imgUrl, const std::string &name = "", const std::string &password = "");

public:
    virtual void onConnected(bool connect, emSocketStatus status) override;

    virtual void onDisConnect() override;

    virtual void onSocketMessage(HNSocketMessage *socketMessage) override;

    virtual void onHearBeat() override;

public:
    bool isStartCheck;
    bool isNeedReLogin;

	int m_connectTimes;
	bool m_bReconnect;
	time_t m_reconnect_time;

private:
    //大厅连接处理=platform connecting processing delegate
    void onConnectMessage(NetMessageHead *messageHead, void *object,INT objectSize);

    // platform login process delegate
    void onLoginMessage(NetMessageHead *messageHead, void *object,INT objectSize);

    // the room list processing
    void onRoomListMessage(NetMessageHead *messageHead, void *object,INT objectSize);

    // update the online user information
    void onUpdateOnLineUserMessage(NetMessageHead *messageHead, void *object,INT objectSize);

    // platform register
    void onUserRegisterMessage(NetMessageHead *messageHead, void *object,INT objectSize);

    // prop
    void onPropMessage(NetMessageHead *messageHead, void *object, INT objectSize);

    // push message
    void onSystemMessage(NetMessageHead *messageHead, void *object,INT objectSize);

    // the distribution of the platform news
    bool dispatchMessage(const PlatformNotifyFun &func);

    void heartBeat(float dt);

private:
private:
    HNUIDelegateQueue<IPlatformMessageDelegate *> *_addQueue;
    HNUIDelegateQueue<IPlatformMessageDelegate *> *_removeQueue;
    HNUIDelegateQueue<IPlatformMessageDelegate *> *_notifyQueue;

    HNSocketLogic *_socketLogic;

    bool _loginResult;

    bool _platformConnected;

    //
    bool _platformLogin;

    INT _heartBeatFrequency;

    std::string _platformAddress;
    std::string _platformHost;

    INT _platformPort;

    void startHeartBeat();
    void stopHeartBeat();
    
    //接收心跳的时刻
    time_t receive_time;

	//断线重连用
	std::string _reconnectName;
	std::string _reconnectPassword;

private:
    HNPlatformLogic(void);
    virtual ~HNPlatformLogic(void);
};

#define PlatformLogic() HNPlatformLogic::getInstance()
}

#endif //__HN_HNPlatformLogic_H__
