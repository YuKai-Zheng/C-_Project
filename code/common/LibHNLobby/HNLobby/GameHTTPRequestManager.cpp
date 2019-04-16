//
//  GameHTTPRequestManager.cpp
//  LibHNLobby
//
//  Created by 黄百超 on 27/04/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#include "GameHTTPRequestManager.h"
#include "GameData.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "globel.h"
#include "HNNetProtocol/HNBaseCommand.h"
#include "HNLobby/PlatformConfig.h"
#include "VersionInfo/VersionInfo.h"
#include "HNLobby/pattern/CommandManager.h"
#include "HNLobby/GameADSpaceManager.h"

GameHTTPRequestManager* mInstance = nullptr;

GameHTTPRequestManager* GameHTTPRequestManager::getInstance() {
    if (!mInstance) {
        mInstance = new GameHTTPRequestManager();
    }
    return mInstance;
}

GameHTTPRequestManager::GameHTTPRequestManager()
: _funcCallBack(nullptr)
, _gamesCallBack(nullptr)
, _getGamesCallBack(nullptr)
{
    
}

GameHTTPRequestManager::~GameHTTPRequestManager() {
    
}

void GameHTTPRequestManager::setFunctionSucceedCallBack(std::function<void ()> callBack) {
    _funcCallBack = callBack;
}

void GameHTTPRequestManager::setGamesSucceedCallBack(std::function<void ()> callBack) {
    _gamesCallBack = callBack;
}

//根据版本号、平台处理是否开启某些功能
void GameHTTPRequestManager::reqOpenFuncByVersionAndPlatform(std::string versionNumber) {
    std::string URL = "http://" + getWebServerUrl() +"/Public/AuditConfig.ashx?";
    std::string str("action=GetConfigByKeyBanBen&banBen=");
    str += versionNumber;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    str += "&platForm=ios";
#elif  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    str += "&platForm=adr";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    str += "&platForm=pc";
#endif
    
    HttpRequest *request = new (std::nothrow) HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameHTTPRequestManager::onHttpOpenFuncResponse, this));
    request->setRequestData(str.c_str(), str.size());
    HttpClient::getInstance()->send(request);
    HttpClient::getInstance()->setTimeoutForConnect(3);
    request->release();
}

//根据版本号请求功能开启状态回调
void GameHTTPRequestManager::onHttpOpenFuncResponse(network::HttpClient *client, network::HttpResponse *response) {
    if (response->isSucceed()) {
        std::vector<char> recv = *response->getResponseData();
        std::string data;
        data.assign(recv.begin(), recv.end());
        
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        
        if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray()) {
            return;
        }
        
        if (_funcCallBack)
            _funcCallBack();
        
        //推广 promotion			    //兑换 exchange
        //公告 notice					//签到 signin
        //客服 service					//分享 share
        //任务 task						//商城 mall
        //保险箱 coffer					//转账 transfer
        //排行榜 ranklist				//活动 activity
        
        // 解析
        for (auto i = 0; i < doc["content"].Size(); i++) {
            if (!doc["content"][i].HasMember("AttrKey") ||
                !doc["content"][i].HasMember("IosOpen") ||
                !doc["content"][i].HasMember("AdrOpen") ||
                !doc["content"][i].HasMember("PcOpen")) {
                continue;
            }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            bool open = doc["content"][i]["IosOpen"].GetBool();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            bool open = doc["content"][i]["AdrOpen"].GetBool();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
            bool open = doc["content"][i]["PcOpen"].GetBool();
#endif
            
            std::string buttonKey = doc["content"][i]["AttrKey"].GetString();
            if (buttonKey.empty()) {
                continue;
            }
            if (buttonKey.compare("coffer") == 0) {
                PlatformConfig::getInstance()->funcConfig.isOpenBankFunc = open;
            } else if (buttonKey.compare("mall") == 0) {
                //186体验包屏蔽充值
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                if (getOnly186Try())
                    open = false;
#endif
                PlatformConfig::getInstance()->funcConfig.isOpenRechargeFunc = open;
            } else if (buttonKey.compare("ranklist") == 0) {
                PlatformConfig::getInstance()->funcConfig.isOpenRankFunc = open;
            } else if (buttonKey.compare("iscode") == 0) {//验证码
                PlatformConfig::getInstance()->funcConfig.isOpenVertifyCodeFunc = open;
            } else if (buttonKey.compare("helptag") == 0) { //游戏帮助界面
                setForAppStore(open);
                UserDefault::getInstance()->setBoolForKey(IOS_REVIEW, true);
                UserDefault::getInstance()->setBoolForKey(OPEN_HELP_FUNC, open);
//                PlatformConfig::getInstance()->funcConfig.isOpenHelpFunc = open;
            } else if (buttonKey.compare("isyear") == 0) { //游戏帮助界面
                PlatformConfig::getInstance()->funcConfig.isOpenYearFunc = open;
            } else if (buttonKey.compare("isIosShouXin") == 0) { //是否是IOS授信版本
                PlatformConfig::getInstance()->funcConfig.isIosSpapicVersion = open;
            }
        }
        //android审核
        if (GD_NEED_OPEN) {
            PlatformConfig::getInstance()->funcConfig.isOpenRechargeFunc = false;
            PlatformConfig::getInstance()->funcConfig.isOpenYearFunc = false;
            PlatformConfig::getInstance()->funcConfig.isOpenRankFunc = false;
        }
        
    } else {
        log("error code:%ld",response->getResponseCode());
        
        PlatformConfig::getInstance()->funcConfig.isOpenBankFunc = false;
        PlatformConfig::getInstance()->funcConfig.isOpenRankFunc = false;
//        PlatformConfig::getInstance()->funcConfig.isOpenHelpFunc = true;
        PlatformConfig::getInstance()->funcConfig.isOpenRechargeFunc = true;
        PlatformConfig::getInstance()->funcConfig.isOpenYearFunc = false;
        UserDefault::getInstance()->setBoolForKey(OPEN_HELP_FUNC, true);
        
        GameData::getInstance()->init();
    }
    
    UserDefault::getInstance()->flush();
    
}

//根据游戏版本获取服务端开启的游戏类型及游戏
void GameHTTPRequestManager::reqGameTypeAndValidGames(std::string versionNumber) {
    std::string URL = "http://" + getWebServerUrl() +"/Public/AuditConfig.ashx?";
    std::string str("action=GetGameTypeInfo&game=");
    if (VERSION_NINETY_NINE) {
        str+="933";
    } else if (VERSION_SIXTY_SIX) {
        str+="8868";
    } else {
        str+="8090";
    }
    
    HttpRequest *request = new (std::nothrow) HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameHTTPRequestManager::onHttpValidGamesResponse, this));
    request->setRequestData(str.c_str(), str.size());
    HttpClient::getInstance()->send(request);
    HttpClient::getInstance()->setTimeoutForConnect(3);
    request->release();
}

void GameHTTPRequestManager::onHttpValidGamesResponse(network::HttpClient *client, network::HttpResponse *response) {
    GameData::getInstance()->init();
    
    std::vector<int>    nnIds;
    std::vector<int>    fishIds;
    std::vector<int>    happyIds;
    std::vector<int>    multiIds;
	std::vector<int>    quickIds;
    
    if (response->isSucceed()) {
        std::vector<char> recv = *response->getResponseData();
        std::string data;
        data.assign(recv.begin(), recv.end());
        
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        
        if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray()) {
            /*
            if (VERSION_NINETY_NINE) {
                nnIds.push_back(TWO_NN_ID);
            } else if (VERSION_EIGHTY_EIGHT) {
                nnIds.push_back(TWO_NN_ID);
                nnIds.push_back(FOUR_NN_ID);
            }
            */
            return;
        }
        
        if (_gamesCallBack)
            _gamesCallBack();
        
        //  经典牛牛 1
        //  街机捕鱼 2
        //  欢乐系列 3
        //  多人游戏 4
        //  快速开始 5
        //  解析
        for (auto i = 0; i < doc["content"].Size(); i++) {
            if (!doc["content"][i].HasMember("GameTypeID") ||
                !doc["content"][i].HasMember("Game") ||
                !doc["content"][i].HasMember("GameID")) {
                continue;
            }
            switch (doc["content"][i]["GameTypeID"].GetInt()) {
                case 1:
                    //牛牛
                    nnIds.push_back(doc["content"][i]["GameID"].GetInt());
                    break;
                case 2:
                    //街机捕鱼
                    if (getOnly186Try() && doc["content"][i]["GameID"].GetInt() == GOLDEN_TOAD_ID) {
                        //186体验包屏蔽金蟾捕鱼
                    }
                    else {
                        fishIds.push_back(doc["content"][i]["GameID"].GetInt());
                    }
                    break;
                case 3:
                    //欢乐系列
                    happyIds.push_back(doc["content"][i]["GameID"].GetInt());
                    break;
                case 4:
                    //多人游戏
                    multiIds.push_back(doc["content"][i]["GameID"].GetInt());
                    break;
                case 5:
                    //快速开始
                    quickIds.push_back(doc["content"][i]["GameID"].GetInt());
                    break;
                default:
                    break;
            }
        }
        
        //android审核 v3
        if (GD_NEED_OPEN && VERSION_THREE)
        {
            happyIds.clear();
            multiIds.clear();
            quickIds.clear();
        }
        
    } else {
        log("error code:%ld",response->getResponseCode());
        
        /*
        if (VERSION_NINETY_NINE) {
            nnIds.push_back(TWO_NN_ID);
        } else if (VERSION_EIGHTY_EIGHT) {
            nnIds.push_back(TWO_NN_ID);
            nnIds.push_back(FOUR_NN_ID);
        } else
        */
         {
            //android审核 v3
            if (GD_NEED_OPEN && VERSION_THREE)
            {
                nnIds.push_back(TWO_NN_ID);
                nnIds.push_back(FOUR_NN_ID);
                nnIds.push_back(SIX_NN_ID);
                nnIds.push_back(BRNN_ID);
                //
                fishIds.push_back(GOLDEN_TOAD_ID);
                fishIds.push_back(GAME_LIKUI_ID);
            }
            else
            {
                nnIds.push_back(TWO_NN_ID);
                nnIds.push_back(FOUR_NN_ID);
                nnIds.push_back(SIX_NN_ID);
                nnIds.push_back(BRNN_ID);
                //
                fishIds.push_back(GOLDEN_TOAD_ID);
                fishIds.push_back(GAME_LIKUI_ID);
                fishIds.push_back(FQZS_ID);
                //
                happyIds.push_back(STUD_ID);
                happyIds.push_back(BJL_ID);
                happyIds.push_back(GOLDEN_FLOWER_ID);
                //
                multiIds.push_back(THIRTEENZHANG_ID);
                multiIds.push_back(BRXJ_ID);
                multiIds.push_back(LUCKYENVELOPE_ID);
            }
        }
    }
    
    GameData::getInstance()->insertGameList(std::pair<std::string, std::vector<int>>(NN_TYPE, nnIds));
    GameData::getInstance()->insertGameList(std::pair<std::string, std::vector<int>>(FISH_TYPE, fishIds));
    GameData::getInstance()->insertGameList(std::pair<std::string, std::vector<int>>(HAPPY_TYPE, happyIds));
    GameData::getInstance()->insertGameList(std::pair<std::string, std::vector<int>>(MULTI_TYPE, multiIds));
    GameData::getInstance()->insertGameList(std::pair<std::string, std::vector<int>>(QUICK_START_TYPE, quickIds));
}

//热门游戏请求
void GameHTTPRequestManager::reqHotGameByVersionAndPlatformq() {
	std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/AuditConfig.ashx?");//
	std::string str("action=GetHotGame");

	network::HttpRequest *request = new (std::nothrow) network::HttpRequest();
	request->setUrl(URL.c_str());
	request->setRequestType(network::HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(GameHTTPRequestManager::onHttpHotGameResponse, this));
	request->setRequestData(str.c_str(), str.size());

	HttpClient::getInstance()->send(request);

	request->release();
}
void GameHTTPRequestManager::onHttpHotGameResponse(network::HttpClient *client, network::HttpResponse *response) {
	std::vector<int>    hotGame;

	std::vector<char> recv = *response->getResponseData();
	std::string data;
	data.assign(recv.begin(), recv.end());

	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

	if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray()) {
		// 处理无当前用户
		return;
	}

	// 解析
	for (auto i = 0; i < doc["content"].Size(); i++) {
		if (!doc["content"][i].HasMember("GameID")) {
			continue;
		}
		int nameID = doc["content"][i]["GameID"].GetInt();
		hotGame.push_back(nameID);
		GameData::getInstance()->insertGameList(std::pair<std::string, std::vector<int>>(HOT_START_GAME, hotGame));
		return;
		// 添加热门游戏
	}
}

void GameHTTPRequestManager::reqWechatIconLink() {
    std::string URL = "http://" + getAPIServerUrl() +"/Public/AuditConfig.ashx?";
    std::string str("action=GetTouXiangUrl");
    
    HttpRequest *request = new (std::nothrow) HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameHTTPRequestManager::onHttpWechatIconLinkResponse, this));
    request->setRequestData(str.c_str(), str.size());
    HttpClient::getInstance()->send(request);
    HttpClient::getInstance()->setTimeoutForConnect(3);
    request->release();
}

void GameHTTPRequestManager::onHttpWechatIconLinkResponse(network::HttpClient *client, network::HttpResponse *response) {
    if (response->isSucceed()) {
        std::vector<char> recv = *response->getResponseData();
        std::string data;
        data.assign(recv.begin(), recv.end());
        
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        
        if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray()) {
            return;
        }
        
        for (auto i = 0; i < doc["content"].Size(); i++) {
            if (doc["content"][i].HasMember("GetTouXiangUrl")) {
                std::string string = doc["content"][i]["GetTouXiangUrl"].GetString();
                setWXImgURL(string);
            }
        }
    }
    else {
        
    }
}

//请求有效的游戏
void GameHTTPRequestManager::reqValidGames(){
	CommandManager::getInstance()->addLoaderCommand(GetText("LODING_REQUEST_ROOMLIST"));
	string platformKey = "8868";

	MSG_GP_S_GetGameTypeID getGamesMsg;
	memset(getGamesMsg.szGetGameType, 0, sizeof(MSG_GP_S_GetGameTypeID));
	memcpy(getGamesMsg.szGetGameType, platformKey.c_str(), sizeof(platformKey));
	PlatformLogic()->sendData(MDM_GP_GetGameTypeID, ASS_GP_GetGameTypeID, &getGamesMsg, sizeof(MSG_GP_S_GetGameTypeID), HN_SOCKET_CALLBACK(GameHTTPRequestManager::validGamesSelector, this));
}

bool GameHTTPRequestManager::validGamesSelector(HN::HNSocketMessage* socketMessage){
	CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
	if (sizeof(MSG_GP_R_GetGameTypeID) != socketMessage->objectSize)
	{
		return false;
	}

	MSG_GP_R_GetGameTypeID* msg = (MSG_GP_R_GetGameTypeID*)socketMessage->object;


	std::map<int, std::vector<int>> mapGames;
	std::map<int, int> mapGamesVer;

	for (int i = 0; i < 30/* sizeof(*msg) / (sizeof(int)*2)*/; i++)
	{
		if (msg->gameID[i] > 0)
		{
			auto iter = mapGames.find(msg->gameTypeID[i]);
			if (iter == mapGames.end())
			{
				std::vector<int> vGames;
				vGames.push_back(msg->gameID[i]);
				mapGames.insert(make_pair(msg->gameTypeID[i], vGames));
			}
			else
			{
				iter->second.push_back(msg->gameID[i]);
			}

			mapGamesVer.insert(make_pair(msg->gameID[i], msg->gameVer[i]));
		}

	}

	GameData::getInstance()->setValidGames(mapGames);
	GameData::getInstance()->setGamesVer(mapGamesVer,msg->wallVer);
	GameData::getInstance()->setDonateCoin(msg->nDonateCoin);
	GameData::getInstance()->setSubPackageUrl(msg->szDownUrl);
	if (_getGamesCallBack)
	{
		_getGamesCallBack();
		_getGamesCallBack = nullptr;
	}
	return true;
}

//根据版本号、平台处理是否开启某些功能
void GameHTTPRequestManager::reqHelpFuncByVersionAndPlatform(std::string versionNumber) {
    std::string URL = "http://" + getWebServerUrl() +"/Public/AuditConfig.ashx?";
    std::string str("action=GetHelpConfig&banBen=");
    str += versionNumber;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    str += "&platForm=ios";
#elif  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    str += "&platForm=adr";
#endif
    
    HttpRequest *request = new (std::nothrow) HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameHTTPRequestManager::onHttpHelpFuncResponse, this));
    request->setRequestData(str.c_str(), str.size());
    HttpClient::getInstance()->send(request);
    HttpClient::getInstance()->setTimeoutForConnect(3);
    request->release();
}

//根据版本号请求功能开启状态回调
void GameHTTPRequestManager::onHttpHelpFuncResponse(network::HttpClient *client, network::HttpResponse *response) {
    if (response->isSucceed()) {
        std::vector<char> recv = *response->getResponseData();
        std::string data;
        data.assign(recv.begin(), recv.end());
        
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
        if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray()) {
            return;
        }
        
        // 解析
        for (auto i = 0; i < doc["content"].Size(); i++) {
            if (!doc["content"][i].HasMember("AttrKeyValue")) {
                continue;
            }
            bool open = doc["content"][i]["AttrKeyValue"].GetInt();
            PlatformConfig::getInstance()->funcConfig.isOpenHelpFunc = open;
        }
    } else {
        PlatformConfig::getInstance()->funcConfig.isOpenHelpFunc = true;
    }
    
    UserDefault::getInstance()->flush();
}

void GameHTTPRequestManager::reqOpenFuncSocket() {
    MSG_GP_S_GetBanBenConfig config;
    memset(config.szBanBen, 0, sizeof(config.szBanBen));
    std::string version = APP_VERSION;
    memcpy(config.szBanBen, version.c_str(), version.size());
    std::string  plat = "";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    plat = "ios";
#elif  (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    plat = "adr";
#endif
    memset(config.szPlatForm, 0, sizeof(config.szPlatForm));
    memcpy(config.szPlatForm, plat.c_str(), sizeof(plat));
    PlatformLogic()->sendData(MDM_GP_GetBanBenConfig, ASS_GP_GetBanBenConfig, &config, sizeof(MSG_GP_S_GetBanBenConfig), HN_SOCKET_CALLBACK(GameHTTPRequestManager::openFuncSocketSelector, this));
}

bool GameHTTPRequestManager::openFuncSocketSelector(HN::HNSocketMessage *socketMessage) {
    if (sizeof(MSG_GP_R_GetBanBenConfig) != socketMessage->objectSize) {
        PlatformConfig::getInstance()->funcConfig.isOpenBankFunc = false;
        PlatformConfig::getInstance()->funcConfig.isOpenRankFunc = false;
        PlatformConfig::getInstance()->funcConfig.isOpenRechargeFunc = true;
        PlatformConfig::getInstance()->funcConfig.isOpenYearFunc = false;
        PlatformConfig::getInstance()->funcConfig.isPopularize = false;
        return true;
    }
    
    MSG_GP_R_GetBanBenConfig* msg = (MSG_GP_R_GetBanBenConfig*)socketMessage->object;
    PlatformConfig::getInstance()->funcConfig.isOpenBankFunc = msg->Coffer;
    PlatformConfig::getInstance()->funcConfig.isOpenRankFunc = msg->RankList;
    PlatformConfig::getInstance()->funcConfig.isOpenRechargeFunc = msg->Mall;
    PlatformConfig::getInstance()->funcConfig.isOpenYearFunc = msg->IsYear;
    PlatformConfig::getInstance()->funcConfig.isPopularize = msg->BeiYong1;
    PlatformConfig::getInstance()->funcConfig.isOpenHelpFunc = msg->BeiYong2;   //帮助（审核）
    
    return true;
}

void GameHTTPRequestManager::setEnterLobbyCallback(GetGamesSuccessCallBack callback){
	_getGamesCallBack = callback;
}

void GameHTTPRequestManager::ReqClienIp(long int id, const std::string& action)
{
    std::string URL = "http://" + getWebServerUrl() +"/Public/UpdateInfo.aspx?";
    string str = "UserID=" + to_string(id) + "&type=" + action;
    
    HttpRequest *request = new (std::nothrow) HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameHTTPRequestManager::onHttpReqClientResponse, this));
    request->setRequestData(str.c_str(), str.size());
    HttpClient::getInstance()->send(request);
}
void GameHTTPRequestManager::ReqClienIpByName(const std::string& name, const std::string& action)
{
    std::string URL = "http://" + getWebServerUrl() +"/Public/UpdateInfo.aspx?";
    string str = "UserName=" + name + "&type=" + action;
    
    HttpRequest *request = new (std::nothrow) HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameHTTPRequestManager::onHttpReqClientResponse, this));
    request->setRequestData(str.c_str(), str.size());
    HttpClient::getInstance()->send(request);
}
void GameHTTPRequestManager::onHttpReqClientResponse(network::HttpClient *client, network::HttpResponse *response)
{
    if (!response->isSucceed()) {
        
        return;
    }
    
}

void GameHTTPRequestManager::ReqGetADSpaceInformation()
{
	std::string URL = "http://" + getWebServerUrl() + "/public/AppAdvert.ashx";

	HttpRequest *request = new (std::nothrow) HttpRequest();
	request->setUrl(URL.c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(CC_CALLBACK_2(GameHTTPRequestManager::onHttpReqADSpaceInformationResponse, this));
	HttpClient::getInstance()->send(request);
}

void GameHTTPRequestManager::onHttpReqADSpaceInformationResponse(network::HttpClient *client, network::HttpResponse *response)
{
	if (!response->isSucceed()) {
		return;
	}

	std::vector<char> recv = *response->getResponseData();
	std::string data;
	data.assign(recv.begin(), recv.end());

	rapidjson::Document doc;
	doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());
	if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray()) {
		return;
	}

	// 解析;
	if (doc["content"].Size() == 1)
	{
		std::string strUrl = doc["content"][0]["url"].GetString();
		std::string strData = doc["content"][0]["data"].GetString();
		GameADSpaceManager::getInstance()->setADSpaceInfo(strUrl, strData);
	}
}
