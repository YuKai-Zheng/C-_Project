#include "PlatformConfig.h"
#include "ApplicationMacro.h"
#include "HNNetProtocol/HNBaseCommand.h"

static PlatformConfig* sPlatformConfig = nullptr;

PlatformConfig* PlatformConfig::getInstance() {
    if (nullptr == sPlatformConfig) {
        sPlatformConfig = new PlatformConfig();
    }
    return sPlatformConfig;
}

void PlatformConfig::setGameLogo(const std::string& logoGame) {
    _gameLogo = logoGame;
}

std::string PlatformConfig::getGameLogo() const {
    return _gameLogo;
}

void PlatformConfig::setSplashLogo(const std::string& splashLogo) {
    _splashLogo = splashLogo;
}

std::string PlatformConfig::getSplashLogo() const {
    return _splashLogo;
}

void PlatformConfig::setGameDesignSize(Size designSize) {
    _gameDesignSize = designSize;
}

Size PlatformConfig::getGameDesignSize() const {
    return _gameDesignSize;
}

void PlatformConfig::setPlatformDesignSize(Size designSize) {
    _platformDesignSize = designSize;
}

std::string PlatformConfig::getGoodsListUrl(const std::string& platForm_Type) {
    auto url_prefix = StringUtils::format("/public/xmlhttpuser.aspx?type=GetMobilePayConfig&GameType=%s", platForm_Type.c_str());
    return buildHttp(getAPIServerUrl(), url_prefix.c_str());
}

std::string PlatformConfig::getPayCallbackUrl_iOS() {
    return buildHttp(getAPIServerUrl(), "/public/xmlhttpuser.aspx");
}

Size PlatformConfig::getPlatformDesignSize() const {
    return _platformDesignSize;
}

void PlatformConfig::setAccountType(PlatformConfig::ACCOUNT_TYPE type) {
    _accountType = type;
}

PlatformConfig::ACCOUNT_TYPE PlatformConfig::getAccountType() const {
    return _accountType;
}

void PlatformConfig::setSupportIAP(bool supportIAP) {
    _supportIAP = supportIAP;
}

bool PlatformConfig::getSupportIAP() const {
    return _supportIAP;
}

void PlatformConfig::setDownloadName(const std::string& name) {
    _downloadName = name;
}

std::string PlatformConfig::getDownloadName() {
    return _downloadName;
}

void PlatformConfig::setVersionKey(const std::string& key) {
    _versionKey = key;
}
std::string PlatformConfig::getVersionKey() {
    return _versionKey;
}

PlatformConfig::PlatformConfig()
    : _gameDesignSize(800, 480)
    , _platformDesignSize(800, 480)
    , _accountType(UNKNOWN)
    , fullClose(false)
	, _fGoldDisplayRate(1.0f)
	, _fGoldDisplayMultiple(1.0f)
{
}

PlatformConfig::~PlatformConfig() {
}

std::string PlatformConfig::getPromoterUrl() {
    return buildHttp(getAPIServerUrl(), "/api/tuiguang/Tuigung.ashx");
}

std::string PlatformConfig::getPrizeUrl() {
    return buildHttp(getAPIServerUrl(), "/public/prizedata.ashx?");
}

std::string PlatformConfig::buildHttp(const std::string& url, const std::string& path) {
    std::string http("http://");
    http.append(url);
    http.append(path);
    return http;
}

std::string PlatformConfig::buildHttps(const std::string& url, const std::string& path) {
    std::string https("https://");
    https.append(url);
    https.append(path);
    return https;
}

std::string PlatformConfig::getOrderUrl() {
    return buildHttp(getAPIServerUrl(), "/public/xmlhttpuser.aspx?");
}

int PlatformConfig::getPlatformType() {
    int platformType = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    platformType = ANDROID_PLATFROM;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (getExtendVersion()) {
        platformType = IOS_PLATFORM_IN_HOUSE;
    } else {
        platformType = IOS_PLATFORM;
    }

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    platformType = WINDOWS_PLATFORM;
#endif
    return platformType;
}

void PlatformConfig::setUIVersion(std::string version) {
    m_UIVersion = version;
}

std::string PlatformConfig::getUIVersion() {
    return m_UIVersion;
}

void PlatformConfig::setGoldDisplayRate(std::string strRate)
{
	std::vector<std::string> listSplit;
	Tools::SplitString(strRate, listSplit, ":");
	
	if (listSplit.size() == 2)
	{
		float fDenominator = atof(listSplit[1].c_str());
		float fMember = atof(listSplit[0].c_str());
		if (fMember > 0.0f)
			_fGoldDisplayMultiple = fDenominator / fMember;
		if(fDenominator > 0.0f)
			_fGoldDisplayRate = fMember / fDenominator;
	}
}

float PlatformConfig::getGoldDisplayRate()
{
	return _fGoldDisplayRate;
}

float PlatformConfig::getGoldDisplayMultiple()
{
	return _fGoldDisplayMultiple;
}
