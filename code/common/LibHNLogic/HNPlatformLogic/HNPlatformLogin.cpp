#include "HNPlatformLogin.h"
#include "HNNetExport.h"
#include "cocos2d.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/PlatformDefine.h"

USING_NS_CC;

namespace HN {
static const std::string Word_Wrong_Name    = GBKToUtf8("登陆名字错误");
static const std::string Word_Wrong_Pass    = GBKToUtf8("用户密码错误");
//static const std::string Word_Logined       = GBKToUtf8("账号重复登陆");
static const std::string Word_logined_Nick = GBKToUtf8("昵称已经存在");
static const std::string Word_Disable = GBKToUtf8("该账户已被冻结");
static const std::string Word_SEVER = GBKToUtf8("服务器正在维护中");

HNPlatformLogin::HNPlatformLogin(LoginDelegate* delegate) :
    HNPlatformLogicBase(delegate) {
    //HNPlatformLogicBase(this);
    CCAssert(delegate != nullptr, "delegate is null.");
    //HNPlatformBase
    m_delegate = delegate;
    name = "HNPlatformLogin";
}

HNPlatformLogin::~HNPlatformLogin() {
    m_delegate = nullptr;
}

bool HNPlatformLogin::requestLogin(const std::string& name, const std::string& pwd) {
    if(name.empty() || pwd.empty()) {

        onPlatformLoginCallback(false, "name or password empty.", name, pwd, 0);
        return false;
    }
    _name = name;
    _pwd  = pwd;
    platformLogin();

    return true;
}

void HNPlatformLogin::platformLogin() {
    if (PlatformLogic()->connected()) {
        PlatformLogic()->platformLogin(_name, _pwd, GameCreator()->getCurrentGameNameID());
    } else {
        PlatformLogic()->connectPlatform();
    }
}

void HNPlatformLogin::onHandleConnectMessage(bool result) {
    if(!result) {
        onPlatformLoginCallback(false, GetText("SEVER_IN_MAINTENANCE"), _name, _pwd, 0);
        return;
    }
    platformLogin();
}

    void HNPlatformLogin::onHandleLoginMessage(bool result, UINT dwErrorCode, const std::string& msg) {
    if(!result) {
        char message[50] = {0};
        switch (dwErrorCode) {
        case ERR_GP_USER_NO_FIND:
            sprintf(message, "%s", Word_Wrong_Name.c_str());
            break;
        case ERR_GP_USER_PASS_ERROR:
            sprintf(message, "%s", Word_Wrong_Pass.c_str());
            break;
        case ERR_GP_USER_LOGON:
            sprintf(message, "%s", GetText("BEEN_LOGIN"));
            break;
        case ERR_GP_USERNICK_EXIST:
            sprintf(message, "%s", Word_logined_Nick.c_str());
            break;
        case ERR_GP_USER_VALIDATA:
            sprintf(message, "%s", Word_Disable.c_str());
            break;
        case ERR_GP_USER_BaiMingDan:
            sprintf(message, "%s", Word_SEVER.c_str());
            break;
        case ERR_GP_ACCOUNT_HAS_LOCK:
            sprintf(message, "%s", msg.c_str());
            break;
        default:
            break;
        }
        onPlatformLoginCallback(false, message, _name, _pwd, dwErrorCode);
        return;
    }
    onPlatformLoginCallback(true, "login success.", _name, _pwd, dwErrorCode);
}

void HNPlatformLogin::onPlatformLoginCallback(bool success, const std::string &message, const std::string &name, const std::string &pwd, int errCode) {
    if (success) {
        if (_isAccountLogin) {
            saveUserInfo(name, pwd);
        }
        
        /*MSG_GP_R_LogonResult &LogonResult = PlatformLogic()->loginResult;
        std::string _url = std::string("http://") + getWebServerUrl() + std::string("/Public/UpdateInfo.aspx?");
        std::string str;
        str += "UserID=";
        str += LogonResult.dwUserID;
        str += "&type=login";
        network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
        request->setUrl(_url.c_str());
        request->setRequestType(network::HttpRequest::Type::POST);
        request->setRequestData(str.c_str(), str.size());
        network::HttpClient::getInstance()->sendImmediate(request);
        request->release();*/
    }
    stop();

    //get game menu one instance
    //	instace.

    m_delegate->loginResult(success, message, errCode);
}

void HNPlatformLogin::saveUserInfo(const std::string &userName, const std::string &userPswd) {
    auto userDefault = UserDefault::getInstance();

    if (userDefault->getBoolForKey(SAVE_TEXT)) {
        if (!userName.empty()) {
            userDefault->setStringForKey(USERNAME_TEXT, userName);
        }

        if (!userPswd.empty()) {
            userDefault->setStringForKey(PASSWORD_TEXT, userPswd);
        }
    } else {
        userDefault->setStringForKey(USERNAME_TEXT, "");
        userDefault->setStringForKey(PASSWORD_TEXT, "");
    }
    userDefault->flush();
}

void HNPlatformLogin::enterGame(const std::string &userName, const std::string &userPswd) {
    start();
    requestLogin(userName, userPswd);
}
}
