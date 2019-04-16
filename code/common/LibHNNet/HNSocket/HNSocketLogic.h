#ifndef __HN_SocketLogic_H__
#define __HN_SocketLogic_H__

#include "HNCommon/HNCommonMarco.h"
#include "HNCommon/HNUIDelegateQueue.h"
#include "HNSocketMessage.h"
#include "HNSocketEventDelegate.h"
#include "HNSocketSelector.h"
#include "cocos2d.h"
#include <list>
#include <map>
#include <vector>

USING_NS_CC;

namespace HN {
class HNSocketThread;
class ISocketMessageDelegate;
class HNSocketSelectorItem;


class HNSocketLogic : public Ref, ISocketEventDelegate {
    typedef HNSocketSelector<std::string> HNSocketMessageSelectorHandler;

public:
    HNSocketLogic(ISocketMessageDelegate* delegate, const std::string& tag);
    virtual ~HNSocketLogic(void);

public:
    bool init();
    //是否已连接
    bool connected() const;

public:
    // connect server
    bool openWithIp(const CHAR* ip, INT port);

    // connect server
    bool openWithHost(const CHAR* host, INT port);

    // close socket
    bool close();

    // send data
    INT send(UINT MainID, UINT AssistantID, UINT CheckCode, void* object, INT objectSize);

    // set handler callback event
    void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

	void addEventSelector(std::string strKey, SEL_SocketMessage selector);

    // remove handler callback event
    void removeEventSelector(UINT MainID, UINT AssistantID);

    // send a heartbeat packet
    void heartBeat(UINT MainID, UINT AssistantID, UINT CheckCode);

    void resetData();

    // 增加大厅的心跳次数.
    void addPlatformHearBeatCount();
    // 增加游戏的心跳次数.
    void addGameHearBeatCount();
    void resumePlatformHearBeatCount();
    void resumeGameHearBeatCount();
    int getPlatformHeartBeatCount();
    int getGameHeartBeatCount();
public:
    //
    virtual void onReadComplete() override;

    virtual void onHearBeat() override;
    // reading network data delegate
    //virtual void onReadSocketData(HNSocketMessage* socketMessage) override;

    // the server connection success delegate
    //virtual void onConnected(bool connect, emSocketStatus status) override;

    // disconnect the network delegate
    //virtual void onDisConnect() override;
private:
	void printfMessageInfo(HNSocketMessage* socketMessage);

private:
    HNSocketThread*					_socketThread;

    HNSocketMessageSelectorHandler*	_selectorQueue;

    ISocketMessageDelegate*			_delegate;

private:
    /** 网络消息通知处理函数*/
    void socketDataDispatch(float dt);


private:
	//MDM_GP 游戏平台
	//MDM_GR 游戏房间 MDM_GM 游戏消息 同一组消息
	typedef std::map<int, std::string> MDM_TYPE_MAP;
	MDM_TYPE_MAP m_mdmGP_MessageInfo;
	MDM_TYPE_MAP m_mdmGR_MessageInfo;
	MDM_TYPE_MAP m_mdmGM_MessageInfo;
#define DEFINE_MDM_GP_TYPE_MAP(number,centent) m_mdmGP_MessageInfo[int(number)] = std::string(centent)
#define DEFINE_MDM_GR_TYPE_MAP(number,centent) m_mdmGR_MessageInfo[int(number)] = std::string(centent)
//#define DEFINE_MDM_GM_TYPE_MAP(number,centent) m_mdmGM_MessageInfo[int(number)] = std::string(centent)
	
	
	
};
}

#endif	//__HN_SocketLogic_H__

