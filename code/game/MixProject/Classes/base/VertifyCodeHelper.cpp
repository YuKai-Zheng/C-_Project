#include "VertifyCodeHelper.h"
#include "network/HttpRequest.h"
#include "cocos2d.h"
#include <vector>

#include "json/rapidjson.h"
#include "json/document.h"

#include "HNLobby/pattern/CommandManager.h"
#include "HNLobby/PlatformConfig.h"

USING_NS_CC;
using namespace cocos2d::network;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iOSVertifyCodeHelper.h"

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#endif


static VertifyCodeHelper* m_instance = nullptr;
VertifyCodeHelper* VertifyCodeHelper::getInstance() {
    if (!m_instance) {
        m_instance = new VertifyCodeHelper();
    }
    return m_instance;
}

void VertifyCodeHelper::vertify() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)    //iOS平台
//    [iOSVertifyCode vertify];
//    iOSVertifyCode::vertify();
    iOSVertifyCodeHelper::vertify();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)    //Android平台

    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/jlx/MixProject/VertifyCode","getInstance","()Ljava/lang/Object;");
    jobject instance;
    if (isHave) {
        instance = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);

//        isHave = JniHelper::getMethodInfo(minfo,"com/jlx/MixProject/VertifyCode","setServerUrl", "()Ljava/lang/String;");
//        if (isHave) {
//            jstring serverUrl = minfo.env->NewStringUTF("www.baidu.com");
//            minfo.env->CallVoidMethod(instance,minfo.methodID,serverUrl);
//        }
//
//        isHave = JniHelper::getMethodInfo(minfo,"com/jlx/MixProject/VertifyCode","getJSURL", "()V");
//        if (isHave) {
//            minfo.env->CallVoidMethod(instance,minfo.methodID);
//        }

        isHave = JniHelper::getMethodInfo(minfo,"com/jlx/MixProject/VertifyCode","vertify", "(Ljava/lang/String;)V");
        if (isHave) {
            // jstring serverUrl = minfo.env->NewStringUTF(m_JSUrl.c_str());
            //minfo.env->CallVoidMethod(instance,minfo.methodID);
        }
    }

#endif

}

void VertifyCodeHelper::requestJSURL() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)    //iOS平台
//    std::string URL = "http://" + m_ServerUrl +"/Public/AuditConfig.ashx?";
//    std::string str("action=GetCodeURL");
//     //oprType,0为登录，1为注册
//    str += ("oprType="+0);
//    str += ("userID=123456");
//
//    HttpRequest *request = new (std::nothrow) HttpRequest();
//    request->setUrl(URL.c_str());
//    request->setRequestData(str.c_str(), str.size());
//    request->setRequestType(HttpRequest::Type::POST);
//    request->setResponseCallback(CC_CALLBACK_2(VertifyCodeHelper::onHttpOpenFuncResponse, this));
//    HttpClient::getInstance()->sendImmediate(request);
//    HttpClient::getInstance()->setTimeoutForConnect(3);
//
//    request->release();

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)    //Android平台
//    JniMethodInfo minfo;
//    bool isHave = JniHelper::getStaticMethodInfo(minfo,"com/jlx/MixProject/VertifyCode","getInstance","()Ljava/lang/Object;");
//    jobject instance;
//    if (isHave) {
//        instance = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
//
//        isHave = JniHelper::getMethodInfo(minfo,"com/jlx/MixProject/VertifyCode","setServerUrl", "()Ljava/lang/String;");
//        if (isHave) {
//            jstring serverUrl = minfo.env->NewStringUTF("www.baidu.com");
//            minfo.env->CallVoidMethod(instance,minfo.methodID,serverUrl);
//        }
//
//        isHave = JniHelper::getMethodInfo(minfo,"com/jlx/MixProject/VertifyCode","getJSURL", "()V");
//        if (isHave) {
//            minfo.env->CallVoidMethod(instance,minfo.methodID);
//        }
//    }
#endif
}

void VertifyCodeHelper::onHttpOpenFuncResponse(network::HttpClient *client, network::HttpResponse *response) {
//    if (response->isSucceed()) {
//        std::vector<char> recv = *response->getResponseData();
//        std::string data;
//        data.assign(recv.begin(), recv.end());
//
//        log("js url:%s",data.c_str());
//
//        rapidjson::Document doc;
//        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
//        if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content")) {
//            return;
//        }
//        //验证
//        m_JSUrl = doc["content"][0]["url"].GetString();
//
//        vertify();
//    }else {
//        log("error code:%ld",response->getResponseCode());
//    }
}

void VertifyCodeHelper::setServerUrl(std::string url) {
    m_ServerUrl = url;
}

std::string VertifyCodeHelper::getServerUrl() {
    std::string tmp =m_ServerUrl;
    return  tmp;
}

void VertifyCodeHelper::setJSUrl(std::string url) {
    m_JSUrl = url;
}

std::string VertifyCodeHelper::getJSUrl() {
    return m_JSUrl;
}

void VertifyCodeHelper::vertifyTicket(std::string ticket) {
    std::string URL = "http://" + m_ServerUrl +"/Public/AuditConfig.ashx?";
    std::string str("action=CheckTicket");
    str += "&key=";
    str += ticket;

    HttpRequest *request = new (std::nothrow) HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestData(str.c_str(), str.size());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(VertifyCodeHelper::handleVertifyTicket, this));
    HttpClient::getInstance()->sendImmediate(request);
    HttpClient::getInstance()->setTimeoutForConnect(3);

    request->release();
}

void VertifyCodeHelper::handleVertifyTicket(network::HttpClient *client, network::HttpResponse *response) {
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
        if (doc["content"].GetInt() == 0) {//成功
            log("成功");

            VertifyCodeCom* command = (VertifyCodeCom*)CommandManager::getInstance()->get(VERTIFY_CODE);
            command->setPassed(true);
            command->execute();
//            CommandManager::getInstance()->get(VERTIFY_CODE)->execute();
        }
    } else {
        log("error code:%ld",response->getResponseCode());
    }

}

void VertifyCodeHelper::loginWithWechat() {
    VertifyCodeCom* command = (VertifyCodeCom*)CommandManager::getInstance()->get(VERTIFY_CODE);
    command->setPassed(true);
    command->execute();
}

//bool VertifyCodeHelper::isDisableWechatLogin() {
//    return PlatformConfig::getInstance()->funcConfig.isOpenHelpFunc;
//}
