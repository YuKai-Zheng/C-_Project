#include "VertifyCodeCom.h"
#include "HNLobby/PlatformConfig.h"
#include "HNOperator.h"
#include "base/VertifyCodeHelper.h"
#include "HNLobby/pattern/CommandManager.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "HNLobby/GamePrompt.h"

void VertifyCodeCom::execute() {
    PlatformConfig::getInstance()->funcConfig.isOpenVertifyCodeFunc = false;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

    if(PlatformConfig::getInstance()->funcConfig.isOpenVertifyCodeFunc && !m_passed) { //开启验证码服务

        std::string URL = "http://" + VertifyCodeHelper::getInstance()->getServerUrl() +"/Public/AuditConfig.ashx?";
        std::string params("action=GetCodeURL");
        //oprType,0为登录，1为注册，2.本机登录

        if(m_type == LOGIN_ACCOUNT_TYPE) { //登录
            params += (StringUtils::format("&oprType=%d",m_type));
            params += ("&userID=" + m_loginInfo.name);
            params += ("&pwd=" + m_loginInfo.password);

        } else if(m_type == REGISTER_TYPE) { //注册
            params += (StringUtils::format("&oprType=%d",m_type));
            params += ("&userID=" + m_registerInfo.name);

        } else if(m_type == LOGIN_DEVICE_TYPE) { //本机登录
            params += (StringUtils::format("&oprType=%d",m_type));;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            params += ("&userID=" + HN::Operator::requestChannel("sysmodule", "GetIMEI"));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            params += ("&userID=" + HN::Operator::requestChannel("sysmodule", "getimei"));
#endif
        }
        requestURL(URL,params);

    } else { //不开启，直接登录/本机登录/注册
        direct();
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    direct();
#endif

}

void VertifyCodeCom::requestURL(std::string url,std::string params) {
    HttpRequest *request = new (std::nothrow) HttpRequest();
    request->setUrl(url.c_str());
    request->setRequestData(params.c_str(), params.size());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(VertifyCodeCom::onHttpOpenFuncResponse, this));
    HttpClient::getInstance()->sendImmediate(request);
//    HttpClient::getInstance()->setTimeoutForConnect(3);

    request->release();
}

void VertifyCodeCom::onHttpOpenFuncResponse(network::HttpClient *client, network::HttpResponse *response) {
    if (response->isSucceed()) {
        std::vector<char> recv = *response->getResponseData();
        std::string data;
        data.assign(recv.begin(), recv.end());

        log("js url:%s",data.c_str());

        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content")) {
            return;
        }
        //验证
        std::string m_JSUrl = doc["content"][0]["url"].GetString();
        std::string ret = doc["content"][0]["ret"].GetString();
        bool isCheck =  ret.compare("1")==0;
        std::string errorCode = doc["content"][0]["msgtype"].GetString();
//        0为没有错误，1为注册时账号已经存在，2登录时用户名错误或密码错误
        if(errorCode.compare("0")==0) {//0为没有错误
            if (isCheck) {
                VertifyCodeHelper::getInstance()->setJSUrl(m_JSUrl);
                //    是否验证
                VertifyCodeHelper::getInstance()->vertify();
            } else {
                direct();
            }
        } else if(errorCode.compare("1")==0) { //1为注册时账号已经存在
            CommandManager::getInstance()->get(REMOVE_LOADER_COM)->execute();
            //CommandManager::getInstance()->showGamePrompt(GetText("Register_Account_Exist"));
			GamePromptLayer::create()->showPrompt(GetText("Register_Account_Exist"));
        } else if(errorCode.compare("2")==0) { //2登录时用户名错误或密码错误
            CommandManager::getInstance()->get(REMOVE_LOADER_COM)->execute();
            //CommandManager::getInstance()->showGamePrompt(GetText("Login_Error"));
			GamePromptLayer::create()->showPrompt(GetText("Login_Error"));
        }

//        m_JSUrl = "https://captcha.guard.qcloud.com/template/TCapIframeApi.js?appid=1252422255&clientype=1&lang=2052&asig=gPpJETMzFwAnJMVvzzVKgQTysxInN8OLvibD326uVhbA_4PbhvD_Ry8EYi9D17ipwQE0ufXjhyWq-gj_2q-PYYX-YkeZ1c96";
//        VertifyCodeHelper::getInstance()->setJSUrl(m_JSUrl);
//        //    是否验证
//        VertifyCodeHelper::getInstance()->vertify();

    } else {
        log("error code:%ld",response->getResponseCode());
    }
}

void VertifyCodeCom::direct() {
    if(m_type == LOGIN_ACCOUNT_TYPE) { //登录
        if (m_loginDelegate) {
            m_loginDelegate->loginByAccount(m_loginInfo);
        }
    } else if(m_type == REGISTER_TYPE) { //注册
        if(m_registerDelegate) {
            m_registerDelegate->registerByAccount(m_registerInfo);
        }
    } else if(m_type == LOGIN_DEVICE_TYPE) { //本机登录
        if (m_loginDelegate) {
            m_loginDelegate->loginByDevice();
        }
    } else if (m_type == LOGIN_WECHAT_TYPE) {
        if (m_loginDelegate) {
            m_loginDelegate->loginByWechat();
        }
    }
}

void VertifyCodeCom::setLoginDelegate(LoginDelegate* delegate) {
    m_loginDelegate = delegate;
}

void VertifyCodeCom::setRegisterDelegate(RegisterDelegate* delegate) {
    m_registerDelegate = delegate;
}

void VertifyCodeCom::setType(int type) {
    m_type = type;
}

void VertifyCodeCom::setLoginInfo(LoginInfo loginInfo) {
    m_loginInfo = loginInfo;
}

//void VertifyCodeCom::setLocalLoginInfo(std::string IMEI) {
//    m_IMEI = IMEI;
//}

void VertifyCodeCom::setRegisterInfo(RegisterInfo registerInfo) {
    m_registerInfo = registerInfo;
}

void VertifyCodeCom::setPassed(bool isPassed) {
    m_passed = isPassed;
}
