//
//  GameHTTPRequestManager.h
//  LibHNLobby
//
//  Created by 黄百超 on 27/04/2017.
//  Copyright © 2017 redbird. All rights reserved.
//

#ifndef GameHTTPRequestManager_h
#define GameHTTPRequestManager_h

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"
#include "HNSocket/HNSocketMessage.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::network;

class GameHTTPRequestManager {
    
    typedef std::function<void()> FunctionSucceedCallBack;
    typedef std::function<void()> GamesSucceedCallBack;
    
	typedef std::function<void()> GetGamesSuccessCallBack;
public:
    GameHTTPRequestManager();
    ~GameHTTPRequestManager();
    
    static GameHTTPRequestManager* getInstance();
    
public:
    void setFunctionSucceedCallBack(std::function<void()> callBack);
    void setGamesSucceedCallBack(std::function<void()> callBack);
    
    void reqOpenFuncByVersionAndPlatform(std::string versionNumber);
    void onHttpOpenFuncResponse(network::HttpClient *client, network::HttpResponse *response);
    
    void reqGameTypeAndValidGames(std::string versionNumber);
    void onHttpValidGamesResponse(network::HttpClient *client, network::HttpResponse *response);

	//热门游戏请求
	void reqHotGameByVersionAndPlatformq();
	void onHttpHotGameResponse(network::HttpClient *client, network::HttpResponse *response);
    
    void reqWechatIconLink();
    void onHttpWechatIconLinkResponse(network::HttpClient *client, network::HttpResponse *response);

	//请求有效的游戏
	void reqValidGames();
	bool validGamesSelector(HN::HNSocketMessage* socketMessage);
    
    //帮助开关（审核）
    void reqHelpFuncByVersionAndPlatform(std::string versionNumber);
    void onHttpHelpFuncResponse(network::HttpClient *client, network::HttpResponse *response);
    
    //协议socket请求游戏功能开关
    void reqOpenFuncSocket();
    bool openFuncSocketSelector(HN::HNSocketMessage* socketMessage);

	void setEnterLobbyCallback(GetGamesSuccessCallBack callback);
    
    // 记录IP
    void ReqClienIp(long int id, const std::string& action);
    void ReqClienIpByName(const std::string& name, const std::string& action);
    void onHttpReqClientResponse(network::HttpClient *client, network::HttpResponse *response);

	//请求广告位信息;
	void ReqGetADSpaceInformation();
	void onHttpReqADSpaceInformationResponse(network::HttpClient *client, network::HttpResponse *response);
    
private:
    FunctionSucceedCallBack _funcCallBack;
    GamesSucceedCallBack    _gamesCallBack;

	GetGamesSuccessCallBack _getGamesCallBack;
    
};

#endif /* GameHTTPRequestManager_h */
