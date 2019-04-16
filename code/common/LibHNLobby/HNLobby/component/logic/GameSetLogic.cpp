#include "GameSetLogic.h"
//#include "HNExternal/MD5/MD5.h"
//#include "HNPlatform/HNPlatformLogic.h"
//#include "HNCommon/HNConverCode.h"
//#include "../../GamePrompt.h"
//#include "HNSocket/HNSocketMessage.h"
//#include "../../PlatformDefine.h"
//#include "../../globel.h"
//#include "../../pattern/CommandManager.h"
//
//GameSetLogic::GameSetLogic() {
//    LogonResult = PlatformLogic()->loginResult;
//}
//
//void GameSetLogic::modifyPassword(std::string oldPwd, std::string newPwd, std::string surePwd) {
//    std::string newPassword = newPwd;
//    std::string surePassword = surePwd;
//    std::string oldPassword = oldPwd;
//    std::string MD5oldPass = MD5_CTX::MD5String(oldPassword).c_str();
//
//    std::string tmp(PlatformLogic()->loginResult.szMD5Pass);
//
//    do {
//        if (oldPassword.empty()) {
//            CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("请输入旧密码。"));
//            break;
//        }
//        if (newPassword.empty()) {
//            CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("请输入新密码。"));
//            break;
//        }
//
//        if (surePassword.empty()) {
//            CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("请输入确认密码。"));
//            break;
//        }
//        if (0 != tmp.compare(MD5oldPass)) {
//            CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("旧密码输入错误。"));
//            break;
//        }
//        if (0 != newPassword.compare(surePassword)) {
//            CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("新密码和确认密码不相同。"));
//            break;
//        }
//
//        if (newPwd.length() < 6) {
//            CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("新密码长度少于6位"));
//            break;
//        }
//
//        if (newPwd.compare(oldPwd) == 0) {
//            CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("新旧密码一致"));
//            break;
//        }
//
//        newUserPwd = MD5_CTX::MD5String(newPassword).c_str();
//
//        MSG_GP_S_ChPassword ChPassword;
//        ChPassword.dwUserID = PlatformLogic()->loginResult.dwUserID;
//        strcpy(ChPassword.szMD5OldPass, PlatformLogic()->loginResult.szMD5Pass);
//        strcpy(ChPassword.szMD5NewPass, newUserPwd.c_str());
//        PlatformLogic()->sendData(MDM_GP_USERINFO, ASS_GP_USERINFO_UPDATE_PWD, &ChPassword, sizeof(ChPassword));
//        PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_ACCEPT, HN_SOCKET_CALLBACK(GameSetLogic::modifyPasswordSelector, this));
//        PlatformLogic()->addEventSelector(MDM_GP_USERINFO, ASS_GP_USERINFO_NOTACCEPT, HN_SOCKET_CALLBACK(GameSetLogic::modifyPasswordSelector, this));
//    } while (0);
//}
//
//bool GameSetLogic::modifyPasswordSelector(HNSocketMessage* socketMessage) {
//    if (ASS_GP_USERINFO_ACCEPT == socketMessage->messageHead.bAssistantID) {	// 服务端已接受
//        auto userDefault = UserDefault::getInstance();
//        userDefault->setStringForKey(PASSWORD_TEXT, newUserPwd);
//        strcpy(PlatformLogic()->loginResult.szMD5Pass, newUserPwd.c_str());
//        userDefault->flush();
//        newUserPwd.clear();
//        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("密码修改成功。"));
//        m_delegate->modifyPasswordResult(true);
//    } else if (ASS_GP_USERINFO_NOTACCEPT == socketMessage->messageHead.bAssistantID) {	// 服务端未能接受
//        CommandManager::getInstance()->showGamePrompt(ForceGBKToUtf8("密码修改失败。"));
//        m_delegate->modifyPasswordResult(false);
//    } else {
//    }
//    return true;
//}
//
//void GameSetLogic::getVertifyCode(std::string phone_num) {
//    phoneNumber = phone_num ;
//
//    if (phoneNumber.empty()) {
//        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_NUMBER"));
//        return;
//    }
//
//    std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
//    std::string str("type=sendcode&mobile=");
//    str += phoneNumber;
//
//    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
//    request->setUrl(URL.c_str());
//    request->setRequestType(network::HttpRequest::Type::POST);
//    request->setResponseCallback(CC_CALLBACK_2(GameSetLogic::onHttpCodeResponse, this));
//
//    request->setRequestData(str.c_str(), str.size());
//    network::HttpClient::getInstance()->sendImmediate(request);
//}
//
//void GameSetLogic::onHttpCodeResponse(network::HttpClient* client, network::HttpResponse* response) {
//    std::vector<char> recv = *response->getResponseData();
//    std::string data;
//    data.assign(recv.begin(), recv.end());
//
//    rapidjson::Document doc;
//    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
//
//    if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("status")) {
//        return;
//    }
//
//    int result = doc["status"].GetInt();
//    if (result == 0) {
//        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_SEND_LOSER"));
//    } else if (result == 1) {
//        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_SEND_OK"));
//    }
//}
//
//void GameSetLogic::sendVertifyCode(std::string code) {
//    //std::string phoneNumber;
//    std::string verifyCode = code;
//    std::string str;
//    if (!LogonResult.iBindMobile) {
//        //phoneNumber = _ui.TextField_PhoneNumber->getString();
//        str = "type=verifycode&mobile=";
//    } else {
//        //phoneNumber = _ui.Text_PhoneNumber->getString();
//        str = "type=clearbind&mobile=";
//    }
//
//    int userID = PlatformLogic()->loginResult.dwUserID;
//
//    if (phoneNumber.empty()) {
//        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_NUMBER"));
//        return;
//    }
//
//    if (verifyCode.empty()) {
//        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_INPUT_VERIFICATION"));
//        return;
//    }
//
//    if (phoneNumber.length() != 11) {
//        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_NUMBER_LENGTH_ISNOT"));
//        return;
//    }
//
//
//    std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/yx.aspx?");
//    str += phoneNumber;
//    str += "&code=" + verifyCode;
//    str += "&userid=" + to_string(userID);
//
//    network::HttpRequest* request = new (std::nothrow) network::HttpRequest();
//    request->setUrl(URL.c_str());
//    request->setRequestType(network::HttpRequest::Type::POST);
//    request->setResponseCallback(CC_CALLBACK_2(GameSetLogic::onHttpBindResponse, this));
//
//    request->setRequestData(str.c_str(), str.size());
//    network::HttpClient::getInstance()->sendImmediate(request);
//
//    request->release();
//}
//
//void GameSetLogic::onHttpBindResponse(network::HttpClient* client, network::HttpResponse* response) {
//    std::vector<char> recv = *response->getResponseData();
//    std::string data;
//    data.assign(recv.begin(), recv.end());
//
//    rapidjson::Document doc;
//    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
//
//    if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("status")) {
//        return;
//    }
//
//    int result = doc["status"].GetInt();
//    if (result == 0) {
//        if (!LogonResult.iBindMobile) {//绑定成功
//            GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BING_OK"));
//            m_delegate->bindPhoneResult(BIND_SUCCESS);
//        } else {//解绑失败
//            GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BINGJIE_LOSER"));
//            m_delegate->bindPhoneResult(UNBIND_FAILURE);
//        }
//    } else if (result == 1) {
//        LogonResult.iBindMobile = !LogonResult.iBindMobile;
//        PlatformLogic()->loginResult.iBindMobile = LogonResult.iBindMobile;
//        if (!LogonResult.iBindMobile) {//解绑成功
//            GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BINGJIE_OK"));
//            strcpy(LogonResult.szMobileNo, phoneNumber.c_str());
//            m_delegate->bindPhoneResult(UNBIND_SUCCESS);
//        } else {//绑定成功
//            GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BING_OK"));
//            strcpy(LogonResult.szMobileNo, "");
//            strcpy(LogonResult.szMobileNo, phoneNumber.c_str());
//            strcpy(PlatformLogic()->loginResult.szMobileNo, Utf8ToGB(phoneNumber.c_str()));
//            m_delegate->bindPhoneResult(BIND_SUCCESS);
//        }
//
//    } else if (result == 2) {//重复绑定
//        m_delegate->bindPhoneResult(REBIND);
//        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_BING_REPORT"));
//    } else if (result == 3) {//验证码验证失败
//        GamePromptLayer::create()->showPrompt(GetText("BINDPHONE_VERIFICATION_LOSER"));
//    }
//}
