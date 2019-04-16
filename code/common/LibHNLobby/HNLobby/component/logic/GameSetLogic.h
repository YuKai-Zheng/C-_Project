#ifndef _GAMESET_LOGIC_H_
#define _GAMESET_LOGIC_H_

#include "../delegate/InitialDelegate.h"
#include <string>
#include "HNSocket/HNSocketMessage.h"
#include "network/HttpClient.h"
#include "HNNetProtocol/HNPlatformMessage.h"
////#include "../../v99/GameSetDelegateV99.h"
using namespace HN;
using namespace cocos2d::network;
/*

*/
//class GameSetLogic {
//    CC_SYNTHESIZE(GameSetDelegateV99*,m_delegate,Delegate);
//public:
//    GameSetLogic();
//    //修改密码
//    void modifyPassword(std::string oldPwd, std::string newPwd, std::string surePwd);
//
//    //获取验证码
//    void getVertifyCode(std::string phone_num);
//    void onHttpCodeResponse(HttpClient* client, HttpResponse* response);
//    //发送验证码
//    void sendVertifyCode(std::string code);
//    void onHttpBindResponse(HttpClient* client, HttpResponse* response);
//
//private:
//    std::string phoneNumber;
//    std::string newUserPwd;
//    MSG_GP_R_LogonResult LogonResult;
//    bool modifyPasswordSelector(HNSocketMessage* socketMessage);
//};

#endif //_GAMESET_LOGIC_H_
