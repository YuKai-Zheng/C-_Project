#ifndef VertifyCodeHelper_h
#define VertifyCodeHelper_h

#include <string>
#include "network/HttpClient.h"

class VertifyCodeHelper{

public:
     void vertify();
     void vertifyTicket(std::string ticket);
    
     void setJSUrl(std::string url);
     std::string getJSUrl();
    
    void requestJSURL();
    
    void setServerUrl(std::string url);
    std::string getServerUrl();
    
    void loginWithWechat();
//    bool isDisableWechatLogin();
    
    static VertifyCodeHelper* getInstance();

private:

    std::string m_JSUrl;
    std::string m_ServerUrl;
    
    void onHttpOpenFuncResponse(cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response);
    void handleVertifyTicket(cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response);
};

#endif /* VertifyCodeHelper_h */
