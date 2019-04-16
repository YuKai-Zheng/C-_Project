#include "WeChatAndroid.h"
#include "VertifyCodeHelper.h"
#include "network/HttpRequest.h"
#include "cocos2d.h"
#include <vector>

#include "json/rapidjson.h"
#include "json/document.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "HNNetProtocol/HNBaseCommand.h"
#include "HNLobby/globel.h"
#include "HNCommon/HNLog.h"

USING_NS_CC;
using namespace cocos2d::network;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

static std::string PACKAGE_PATH = "com/jlx/MixProject";
static std::string WECHAT_PATH = "com/game/new8le/wxapi";

static WeChatAndroid* m_instance = nullptr;
WeChatAndroid* WeChatAndroid::getInstance() {
    if (!m_instance) {
		m_instance = new WeChatAndroid();
    }
    return m_instance;
}

bool WeChatAndroid::isAppInstalled()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	bool b = JniHelper::callStaticBooleanMethod(WECHAT_PATH + "/WXEntryActivity", "IsInstallWeChat", false);
	return b;
#endif
	return false;
}

void WeChatAndroid::goToWeiXin()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	std::string accessToken = UserDefault::getInstance()->getStringForKey(WX_ACCESS_TOKEN);
	std::string openId = UserDefault::getInstance()->getStringForKey(WX_OPEN_ID);

	if (accessToken.length() != 0 && openId.length() != 0) {
		sendRefreshTokenRequest();
	}
	else {
		goToWeiXinLoginUI();	
	}
#endif
}

void WeChatAndroid::goToWeiXinLoginUI()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelper::callStaticVoidMethod(PACKAGE_PATH + "/AppActivity", "loginToWX");
#endif
}

void WeChatAndroid::getAccessToken(std::string code)
{
	std::string appID = getWXAppID();
	std::string secretKey = getWXSecretKey();
	std::string URL = "https://api.weixin.qq.com/sns/oauth2/access_token?appid=" + appID + "&secret=" + secretKey + "&code=" + code + "&grant_type=authorization_code";

	//HN::HNLog::logInfoCn(URL);

	cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
	request->setUrl(URL);
	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
		if (response->isSucceed()) {
			std::vector<char>* buffer = response->getResponseData();
			std::string data;
			data.assign(buffer->begin(), buffer->end());

			if (data.empty()) {
				return;
			}

			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

			if (doc.HasParseError()) {
				return;
			}

			std::string accessToken = doc["access_token"].GetString();
			if (accessToken.length() != 0) {
				cocos2d::UserDefault::getInstance()->setStringForKey(WX_ACCESS_TOKEN, accessToken);
			}

			std::string openID = doc["openid"].GetString();
			if (openID.length() != 0) {
				cocos2d::UserDefault::getInstance()->setStringForKey(WX_OPEN_ID, openID);
			}

			std::string refreshToken = doc["refresh_token"].GetString();
			if (refreshToken.length() != 0) {
				cocos2d::UserDefault::getInstance()->setStringForKey(WX_REFRESH_TOKEN, refreshToken);
			}

			std::string unionID = doc["unionid"].GetString();
			if (unionID.length() != 0) {
				//广告渠道标记
				bool bAdPackage = false;

				if (bAdPackage)
				{
					std::string platformTag = "a-";
					std::string id = platformTag + unionID;
					cocos2d::UserDefault::getInstance()->setStringForKey(WX_UNION_ID, id);
				}
				else
				{
					cocos2d::UserDefault::getInstance()->setStringForKey(WX_UNION_ID, unionID);
				}
			}

			cocos2d::UserDefault::getInstance()->flush();

			sendUserInfoRequest(accessToken, openID);
		}
	});
	cocos2d::network::HttpClient::getInstance()->send(request);
	cocos2d::network::HttpClient::getInstance()->setTimeoutForConnect(3);
	request->release();
}

void WeChatAndroid::sendRefreshTokenRequest()
{
	std::string freshToken = UserDefault::getInstance()->getStringForKey(WX_REFRESH_TOKEN);
	std::string appID = getWXAppID();
	std::string URL = "https://api.weixin.qq.com/sns/oauth2/refresh_token?appid=" + appID + "&grant_type=refresh_token&refresh_token=" + freshToken;

	//HN::HNLog::logInfoCn(URL);

	cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
	request->setUrl(URL);
	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
		if (response->isSucceed()) {
			std::vector<char>* buffer = response->getResponseData();
			std::string data;
			data.assign(buffer->begin(), buffer->end());

			if (data.empty()) {
				return;
			}

			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

			if (doc.HasParseError()) {
				return;
			}
			//HN::HNLog::logInfoCn(data);

			if (doc.HasMember("errcode") || doc.HasMember("errmsg")) {
				//需要重新授权
				goToWeiXinLoginUI();
			}
			else {
				std::string nAccessToken = doc["access_token"].GetString();
				if (nAccessToken.length() != 0) {
					cocos2d::UserDefault::getInstance()->setStringForKey(WX_ACCESS_TOKEN, nAccessToken);
				}
				
				std::string openID = doc["openid"].GetString();
				if (openID.length() != 0) {
					cocos2d::UserDefault::getInstance()->setStringForKey(WX_OPEN_ID, openID);
				}
				
				std::string refreshToken = doc["refresh_token"].GetString();
				if (refreshToken.length() != 0) {
					cocos2d::UserDefault::getInstance()->setStringForKey(WX_REFRESH_TOKEN, refreshToken);
				}

				cocos2d::UserDefault::getInstance()->flush();

				sendUserInfoRequest(nAccessToken, openID);
			}
		}
		else {
			//http通信失败，重新授权
			goToWeiXinLoginUI();
		}
	});
	cocos2d::network::HttpClient::getInstance()->send(request);
	cocos2d::network::HttpClient::getInstance()->setTimeoutForConnect(3);
	request->release();
}

void WeChatAndroid::sendUserInfoRequest(std::string ACCESS_TOKEN, std::string openID) {

	std::string URL = "https://api.weixin.qq.com/sns/userinfo?access_token=" + ACCESS_TOKEN + "&openid=" + openID;

	cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
	request->setUrl(URL);
	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
		if (response->isSucceed()) {
			std::vector<char>* buffer = response->getResponseData();
			std::string data;
			data.assign(buffer->begin(), buffer->end());

			if (data.empty()) {
				return;
			}

			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());

			if (doc.HasParseError()) {
				return;
			}

			std::string nickName = doc["nickname"].GetString();
			if (nickName.length() != 0) {
				cocos2d::UserDefault::getInstance()->setStringForKey(WX_NICK_NAME, emojiFliter(nickName));
			}

			std::string headImgUrl = doc["headimgurl"].GetString();
			if (headImgUrl.length() != 0) {
				cocos2d::UserDefault::getInstance()->setStringForKey(WX_HEAD_IMG_URL, headImgUrl);
			}

			VertifyCodeHelper::getInstance()->loginWithWechat();
		}
	});
	cocos2d::network::HttpClient::getInstance()->send(request);
	cocos2d::network::HttpClient::getInstance()->setTimeoutForConnect(3);
	request->release();
}

std::string WeChatAndroid::emojiFliter(std::string str)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string fliterStr = JniHelper::callStaticStringMethod(WECHAT_PATH + "/EmojiFilter", "filterEmoji", str);
	return fliterStr;
#endif
	return str;
}

void WeChatAndroid::shareText(std::string text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (isAppInstalled())
	{
		JniHelper::callStaticVoidMethod(WECHAT_PATH + "/WXEntryActivity", "shareText", text);
	}
#endif
}

void WeChatAndroid::shareWebpage(int type, std::string title, std::string describ, std::string webUrl)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (isAppInstalled())
	{
		JniHelper::callStaticVoidMethod(WECHAT_PATH + "/WXEntryActivity", "ShareWebPage", type, title, describ, webUrl);
	}
#endif
}

void WeChatAndroid::shareScreenShot(int type)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (isAppInstalled())
	{
		//CCSize size = CCDirector::sharedDirector()->getWinSize();
		//CCRenderTexture* in_texture = CCRenderTexture::create(size.width, size.height, kCCTexture2DPixelFormat_RGBA8888);
		//in_texture->setPosition(ccp(size.width / 2, size.height / 2));
		//in_texture->setAnchorPoint(ccp(0.5f, 0.5f));

		//in_texture->begin();
		//CCDirector::getInstance()->getRunningScene()->visit();
		//in_texture->end();
		//in_texture->saveToFile("share.png", kCCImageFormatPNG);

		//std::function<void(float)> func = [](float dt){

		//HN::HNLog::logInfoCn("shareScreenShot");
		//std::string path = FileUtils::getInstance()->getWritablePath();
		//JniHelper::callStaticVoidMethod(WECHAT_PATH + "/WXEntryActivity", "ShareScreenShot", "11");
		//};

		//auto scheduler = CCDirector::getInstance()->getRunningScene()->getScheduler();
		//scheduler->schedule(func, this, 0.0f,0,0.0f, false, "screenshot");

		std::function<void(bool, const std::string&)> callback = [&](bool bResult, const std::string& file){

			if (!bResult)
			{
				//CommandManager::getInstance()->showGamePrompt(GBKToUtf8("分享失败,请重试."));
				return;
			}

			//HN::HNLog::logInfoCn(file);
			JniHelper::callStaticVoidMethod(WECHAT_PATH + "/WXEntryActivity", "ShareScreenShot", type, file);
		};

		utils::captureScreen(callback, "share.png");
	}
#endif
}

void WeChatAndroid::changeScreenDirection(int iDirection)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelper::callStaticVoidMethod("com/jlx/MixProject/AppActivity", "changeScreenDirection", iDirection);

	bool b = (iDirection == 0) ? false:true;
	cocos2d::UserDefault::getInstance()->setBoolForKey(ROOT_VIEW_IS_VERTICAL, b);
	cocos2d::UserDefault::getInstance()->flush();

	Size designResolutionSize = Size(1280, 720);

	if (iDirection == 1)
	{
		auto glview = cocos2d::Director::getInstance()->getOpenGLView();
		if (glview) {
			auto size = glview->getFrameSize();
			glview->setFrameSize(size.height, size.width);
			glview->setDesignResolutionSize(designResolutionSize.height, designResolutionSize.width, ResolutionPolicy::FIXED_HEIGHT);
		}

	}
	else if (iDirection == 0)
	{
		auto glview = cocos2d::Director::getInstance()->getOpenGLView();
		if (glview) {
			auto size = glview->getFrameSize();
			glview->setFrameSize(size.height, size.width);
			glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
		}
	}
	
#endif
}

void WeChatAndroid::changeScreenDirection(int iDirection,  int width, int height)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniHelper::callStaticVoidMethod("com/jlx/MixProject/AppActivity", "changeScreenDirection", iDirection);

	bool b = (iDirection == 0) ? false : true;
	cocos2d::UserDefault::getInstance()->setBoolForKey(ROOT_VIEW_IS_VERTICAL, b);
	cocos2d::UserDefault::getInstance()->flush();

	Size designResolutionSize = Size(width, height);

	if (iDirection == 1)
	{
		auto glview = cocos2d::Director::getInstance()->getOpenGLView();
		if (glview) {
			auto size = glview->getFrameSize();
			glview->setFrameSize(size.height, size.width);
			glview->setDesignResolutionSize(designResolutionSize.height, designResolutionSize.width, ResolutionPolicy::FIXED_HEIGHT);
		}

	}
	else if (iDirection == 0)
	{
		auto glview = cocos2d::Director::getInstance()->getOpenGLView();
		if (glview) {
			auto size = glview->getFrameSize();
			glview->setFrameSize(size.height, size.width);
			glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
		}
	}

#endif
}






