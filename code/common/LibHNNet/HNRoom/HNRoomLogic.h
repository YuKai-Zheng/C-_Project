#ifndef __HN_HNRoomLogic_H__
#define __HN_HNRoomLogic_H__

#include "HNCommon/HNCommonMarco.h"
#include "HNCommon/HNUIDelegateQueue.h"
#include "HNNetProtocol/HNProtocolExport.h"
#include "HNSocket/HNSocketMessage.h"
#include "HNSocket/HNSocketMessageDelegate.h"
#include "cocos2d.h"
#include <time.h>

USING_NS_CC;

namespace HN {
class IGameMessageDelegate;
class IRoomMessageDelegate;
class HNSocketLogic;

// room message distribution statement a callback function
typedef std::function<bool(IRoomMessageDelegate *)> RoomFrameMessageFun;

// game message distribution statement a callback function
typedef std::function<bool(IGameMessageDelegate *)> GameMessageFun;

#define RoomLogic() HNRoomLogic::getInstance()

// 房间网络处理逻辑类=the room network processing logic class
class HNRoomLogic : public Ref, ISocketMessageDelegate {
public:
  static HNRoomLogic *getInstance();

public:
  // the initialization data
  bool init();

  // connecting room
  bool connectRoom(const CHAR *ip, INT port);

  // close room
  bool closeRoom(bool bReconnect=false);

  bool connected() const;

  bool isReconnect() const { return _reconnect; }

  bool login() const;

  ComRoomInfo *getSelectedRoom() const { return _selectedRoom; }

  void setSelectedRoom(ComRoomInfo *room) { _selectedRoom = room; }

  UINT getRoomRule() const { return _gameRoomRule; }

  void setRoomRule(UINT roomRule) { _gameRoomRule = roomRule; }

  //断线重连
  void reconnect(bool bPlatfrom = false);
  bool reconnectRoom();
  void reconnectTimeOut();

public:
  INT sendData(UINT MainID, UINT AssistantID, void *object = nullptr,INT objectSize = 0);
  // send data
  void sendData(UINT MainID, UINT AssistantID, void *object, INT objectSize,SEL_SocketMessage selector);
  // set handler callback event
  void addEventSelector(UINT MainID, UINT AssistantID,SEL_SocketMessage selector);

  // remove handler callback event
  void removeEventSelector(UINT MainID, UINT AssistantID);

public:
  // the logon server room
  void roomLogin(UINT uGameID);

public:
  // 添加游戏事件观察者=add game event viewer
  void addObserver(IGameMessageDelegate *delegate);

  // 移除游戏事件观察者=remove game event viewer
  void removeObserver(IGameMessageDelegate *delegate);

  // add room event viewer
  void addRoomObserver(IRoomMessageDelegate *delegate);

  //  remove room event viewer
  void removeRoomObserver(IRoomMessageDelegate *delegate);

public:
  virtual void onSocketMessage(HNSocketMessage *socketMessage) override;

  virtual void onConnected(bool connect, emSocketStatus status) override;

  virtual void onDisConnect() override;

  virtual void onHearBeat() override;

private:
  // connection room  processing
  void onConnectMessage(NetMessageHead *messageHead, void *object,INT objectSize);

  // login room treatment
  void onLoginMessage(NetMessageHead *messageHead, void *object,INT objectSize);

  // user list processing
  void onUserListMessage(NetMessageHead *messageHead, void *object,INT objectSize);

  // user action processing
  void onUserActionMessage(NetMessageHead *messageHead, void *object,INT objectSize);

  //房间消息处理=room message processing
  void onRoomMessage(NetMessageHead *messageHead, void *object, INT objectSize);

  void onManageMessage(NetMessageHead *messageHead, void *object,INT objectSize);

  void onGameServerFrameMessage(NetMessageHead *messageHead, void *object,INT objectSize);

  void printfSitError(NetMessageHead *messageHead);

private:
  //分发房间消息=distribution of the framework of the game news
  bool dispatchFrameMessage(const RoomFrameMessageFun &func);
  //分发游戏消息=distributed game news
  bool dispatchGameMessage(const GameMessageFun &func);

protected:
  //房间银行消息
  void H_R_M_Bank_Message(const NetMessageHead *messageHead, void *object,
                          INT objectSize);

  void heartBeat(float dt);

public:
  // table state
  MSG_GR_DeskStation deskStation;
  // the room log results
  MSG_GR_R_LogonResult loginResult;

    bool isStartCheck;
    bool isNeedReLogin;

private:
  // 当前选中的房间
  ComRoomInfo *_selectedRoom;
  // the logic of the network processing
  HNSocketLogic *_socketLogic;
  // ip
  std::string _serverIp;
  // port
  INT _serverPort;
  //
  bool _roomConnected;

  //
  bool _roomLogin;

  //断线重连
  bool _reconnect;
  time_t _reconnect_time;

  UINT _gameRoomRule; // 游戏房间规则

  // game notification message queue
  HNUIDelegateQueue<IGameMessageDelegate *> *_gameNotifyQueue;
  HNUIDelegateQueue<IGameMessageDelegate *> *_addGameNotifyQueue;
  HNUIDelegateQueue<IGameMessageDelegate *> *_removeGameNotifyQueue;
  // room notification message queue
  HNUIDelegateQueue<IRoomMessageDelegate *> *_roomNotifyQueue;
  HNUIDelegateQueue<IRoomMessageDelegate *> *_addRoomNotifyQueue;
  HNUIDelegateQueue<IRoomMessageDelegate *> *_removeRoomNotifyQueue;

  void startHeartBeat();
  void stopHeartBeat();
 
  //接收心跳的时刻
  time_t receive_time;

private:
    HNRoomLogic(void);
    virtual ~HNRoomLogic(void);
};


}

#endif	//__HN_HNPlatformLogic_H__

