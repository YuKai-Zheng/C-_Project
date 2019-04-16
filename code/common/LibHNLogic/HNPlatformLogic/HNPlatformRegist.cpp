#include "HNPlatformRegist.h"
#include "HNMarketExport.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/pattern/CommandManager.h"
#include "HNLobby/globel.h"

namespace HN {
enum RIGIST_RESULT_CODE {
    REGIST_FAILE     = 0, // regist faile.
    REGIST_SUCCESS   = 1, // regist success.
    REGIST_SAME_NAME = 2, // same name.
    REGIST_SAME_NICK = 3, // same Nick.
    REGIST_NICK_DEFAULT = 4,//昵称敏感词
    REGIST_INVITE_CODE_ERROR = 5,//邀请码错误
    REGIST_INVITE_CODE_USE = 6,//邀请码已使用
};

HNPlatformRegist::~HNPlatformRegist() {
    m_delegate = nullptr;
}

HNPlatformRegist::HNPlatformRegist(RegisterDelegate* delegate)
    : HNPlatformLogicBase(delegate)
    , _fastRegist(true)
    , _wechatRegist(false)
    {
    CCAssert(delegate != nullptr, "delegate is null.");
    //_callback = this;
    m_delegate = delegate;
    name = "HNPlatformRegist";
}

void HNPlatformRegist::platformRegist() {
    if (PlatformLogic()->connected()) {
        if (_fastRegist) {
            std::string onlyString = Operator::requestChannel("sysmodule", "GetSerialNumber");
            PlatformLogic()->platformTouristRegister(0, GameCreator()->getCurrentGameNameID(), 0, "", onlyString);
        } else {
            PlatformLogic()->platformRegister(1, GameCreator()->getCurrentGameNameID(), _gender, _nickName, "", _name, _pwd, _inviteCode);
        }
    } else {
        PlatformLogic()->connectPlatform();
    }
}

    void HNPlatformRegist::wechatRegist() {
        if (PlatformLogic()->connected()) {
            if (_fastRegist) {
                std::string unionID = UserDefault::getInstance()->getStringForKey(WX_UNION_ID);
                std::string nickName = UserDefault::getInstance()->getStringForKey(WX_NICK_NAME);
                std::string imgUrl = UserDefault::getInstance()->getStringForKey(WX_HEAD_IMG_URL);
                PlatformLogic()->wechatTouristRegister(0, GameCreator()->getCurrentGameNameID(), 0, nickName, "", unionID, imgUrl);
            }
        } else {
            PlatformLogic()->connectPlatform();
        }
    }
    
bool HNPlatformRegist::requestRegist(const std::string &name, const std::string pwd, BYTE gender, const std::string nickName, const std::string inviteCode, bool fastRegist) {
    if(!fastRegist) {
        if(name.empty() || pwd.empty()) {
            onPlatformRegistCallback(false, fastRegist, "empty name or pwd.", name, pwd, 0);
            return false;
        }
    }

    _fastRegist = fastRegist;
    _name       = name;
    _pwd        = pwd;
    _gender = gender;
    _nickName = nickName;
    _inviteCode = inviteCode;

    platformRegist();
    return true;
}

bool HNPlatformRegist::requestTouristRegist(const std::string &name, const std::string pwd, BYTE gender, const std::string nickName, bool fastRegist) {
    platformRegist();
    return true;
}

    bool HNPlatformRegist::requestWechatRegist(const std::string &name, const std::string pwd, BYTE gender, const std::string nickName, bool fastRegist) {
        wechatRegist();
        return true;
    }
    
void HNPlatformRegist::onHandleConnectMessage(bool result) {
    if(!result) {
        onPlatformRegistCallback(false, _fastRegist, GetText("SEVER_IN_MAINTENANCE"), _name, _pwd, 0);
        return;
    }
    if (_wechatRegist) {
        wechatRegist();
    }
    else {
        platformRegist();
    }
}

void HNPlatformRegist::onHandleUserRegisterMessage(MSG_GP_S_Register* registerStruct, UINT ErrorCode) {
    _name = registerStruct->szName;
    _pwd = registerStruct->szPswd;

    switch (ErrorCode) {
    case REGIST_FAILE:
        onPlatformRegistCallback(false, _fastRegist, GetText("Register_Failure"), _name, _pwd, registerStruct->LogonTimes);
        break;
    case REGIST_SUCCESS:
        onPlatformRegistCallback(true, _fastRegist, GetText("Register_Success"), _name, _pwd, registerStruct->LogonTimes);
        break;
    case REGIST_SAME_NAME:
        onPlatformRegistCallback(false, _fastRegist, GetText("Register_Account_Exist"), _name, _pwd, registerStruct->LogonTimes);
        break;
    case REGIST_SAME_NICK:
        onPlatformRegistCallback(false, _fastRegist, GetText("Register_Nick_Exist"), _name, _pwd, registerStruct->LogonTimes);
        break;
    case REGIST_NICK_DEFAULT:
        onPlatformRegistCallback(false, _fastRegist, GetText("Register_Nick_Default"), _name, _pwd, registerStruct->LogonTimes);
        break;
    case  REGIST_INVITE_CODE_ERROR:
        onPlatformRegistCallback(false, _fastRegist, GetText("Register_Invite_Code_Error"), _name, _pwd, registerStruct->LogonTimes);
        break;
    case REGIST_INVITE_CODE_USE:
        onPlatformRegistCallback(false, _fastRegist, GetText("Register_Invite_Code_Use"), _name, _pwd, registerStruct->LogonTimes);
        break;
    default:
        break;
    }
}

void HNPlatformRegist::onPlatformRegistCallback(bool success, bool fastRegist,
        const std::string &message,
        const std::string &name,
        const std::string &pwd,
        int loginTimes) {
    auto userDefault = UserDefault::getInstance();

    stop();

    if (success) {
        
       /* std::string _url = std::string("http://") + getWebServerUrl() + std::string("/Public/UpdateInfo.aspx?");
        std::string str;
        str += "UserName=";
        str += name;
        str += "&type=register";
        network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
        request->setUrl(_url.c_str());
        request->setRequestType(network::HttpRequest::Type::POST);
        request->setRequestData(str.c_str(), str.size());
        network::HttpClient::getInstance()->sendImmediate(request);
        request->release();*/
        
        
        
        char title[256] = { 0 };

        if (0 == loginTimes && !fastRegist) {
            userDefault->setBoolForKey(SAVE_TEXT, true);
            //_isAccountLogin = true;

            /* sprintf(title, GBKToUtf8("请牢记你的账号和密码。/n用户名称: %s/n用户密码: %s"), name.c_str(), _userPsd.c_str());
             auto prompt = GamePromptLayer::create();
             prompt->showPrompt(title);
             prompt->setOkayCallBack(CC_CALLBACK_0(GameMenu::enterGame, this, name, pwd));*/
        } else {
            //enterGame(name, pwd);
        }
    } else {
        //GamePromptLayer::create()->showPrompt(message);
    }

    CommandManager::getInstance()->get(REMOVE_LOADER_COM)->execute();
    m_delegate->registerResult(success, fastRegist, message, name, pwd, loginTimes);
}

void HNPlatformRegist::visitorRegister() {
    _wechatRegist = false;
    start();
    requestTouristRegist("", "", 1, "", true); // 9 21 2016
}
    
    //微信注册
    void HNPlatformRegist::wechatUnionRegister() {
        _wechatRegist = true;
        start();
        requestWechatRegist("", "", 1, "", true); // Jun 9 2017
    }

void HNPlatformRegist::accountRegister(const std::string &name, const std::string pwd, BYTE gender, const std::string nickName, const std::string inviteCode, bool fastRegist) {
    //注册
    start();
    requestRegist(name,pwd, gender, nickName, inviteCode, fastRegist);
}

}
