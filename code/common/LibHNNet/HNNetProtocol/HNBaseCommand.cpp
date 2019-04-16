#include "HNBaseCommand.h"
#include "HNSocket/HNSocket.h"

LLONG getSecrectKey() {
    return 20150611;
}

INT getPlatformServerPort() {
    return 3015;
}

HN_Game_Type getGameType() {
    return HN_Game_Type::eMixGame;
}

void setPlatformServerAddress(std::string url) {
    PLATFORM_SERVER_ADDRESS = url;
}

std::string getPlatformServerAddress() {
    std::string address(PLATFORM_SERVER_ADDRESS);
    //by HBC
    return HN::HNSocket::domainToIP(address.c_str());

    if (HN::HNSocket::isValidIP(address)) {
        return address;
    } else {
        std::vector<std::string> ips = HN::HNSocket::getIpAddress(address.c_str());
        return ips.empty() ? "" : ips.at(0);
    }
}

std::string getPlatformServerHost() {
    std::string host(PLATFORM_SERVER_ADDRESS);
    return host;
}

void setAPIServerUrl(std::string url) {
    API_SERVER_URL = url;
}

std::string getAPIServerUrl() {
    return API_SERVER_URL;
}

void setWebServerUrl(std::string url) {
    WEB_SERVER_URL = url;
}

std::string getWebServerUrl() {
    return WEB_SERVER_URL;
}

std::string getUpdateURL() {
    return UPDATE_URL;
}

void setUpdateURL(std::string url) {
    UPDATE_URL = url;
}

void setAlipayNotifyURL(std::string url) {
    alipayNotifyURL = url;
}
std::string getAlipayNotifyURL() {
    return alipayNotifyURL;
}
void setAlipayWebURL(std::string url) {
    alipayWebURL = url;
}
std::string getAlipayWebURL() {
    return alipayWebURL;
}

std::string getPayType() {
    return PAY_TYPE;
}

void setPayType(std::string type) {
    PAY_TYPE = type;
}

//IAP内购参数相关
void setAppId(std::string appId) {
    APP_ID = appId;
}
std::string getAppId() {
    return APP_ID;
}

void setAppShareKey(std::string key) {
    APP_SHARE_KEY = key;
}
std::string getAppShareKey() {
    return APP_SHARE_KEY;
}

void setWXAppID(std::string appID) {
    WX_APPID = appID;
}

std::string getWXAppID() {
    return WX_APPID;
}

void setWXSecretKey(std::string key) {
    WX_SECRET_KEY = key;
}

std::string getWXSecretKey() {
    return WX_SECRET_KEY;
}

void setWXImgURL(std::string imgURL) {
    WX_IMG_URL = imgURL;
}
std::string getWXImgURL() {
    return WX_IMG_URL;
}

//是否是App Store上架版本
void setForAppStore(bool isAppStore) {
    FOR_APP_STORE = isAppStore;
}
bool getForAppStore() {
    return FOR_APP_STORE;
}

//是否是授信版本
void setExtendVersion(bool extendVersion)
{
    EXTEND_VERSION = extendVersion;
}
bool getExtendVersion()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return EXTEND_VERSION;
#else
    return false;   //android不走这个逻辑，不做判断
#endif
}

//是否开启检查更新
void setOnly186Try(bool update)
{
    ONLY_186_TRY = update;
}
bool getOnly186Try()
{
    return ONLY_186_TRY;
}
