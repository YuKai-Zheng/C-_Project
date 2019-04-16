#include "GameData.h"
#include "PlatformConfig.h"
#include "globel.h"
GameData* GameData::mInstance = nullptr;

GameData* GameData::getInstance() {
    if (!mInstance) {
        mInstance = new GameData();
    }
    return mInstance;
}

GameData::GameData() {
    noticeBufferData = "";
    isNotUpdateNewVersion = false;
    vecJackpotId.clear();
    vecJackpotId = {31000504,31000506,31000512,11105101,11105102,11105103};
    userBankPassword = "";
    m_jackpotValue   = 0;
    iosBackgroudTime = 0; //ios退到后台的时间
    userBankPasswordMD5 = "";
	_donatecoin = -1;
	_subPackageUrl = "";
	_codeString = "";
	setFirstSlideGameListFlag(true);
}

GameData::~GameData() {
    
}

void GameData::init() {
    
}

void GameData::insertGameList(std::pair<std::string, std::vector<int> > gamesPair) {
    gameListDatas.insert(gamesPair);
}

std::map<std::string, std::vector<int>> GameData::getGameListData() {
    return gameListDatas;
}

void GameData::SetStorePath(std::string path)
{
	_storePath = path;
}

std::string GameData::getStorePath()
{
	return _storePath;
}

bool GameData::hasValuesWithKey(std::string key) {
    return gameListDatas.find(key) != gameListDatas.end();
}

std::vector<int> GameData::getValues(std::string key) {
    std::vector<int> result;
    result.clear();
    if (hasValuesWithKey(key)) {
        result = gameListDatas.at(key);
    }
    return result;
}

void GameData::spliteNoticeData() {
    std::string strTitle;
    std::vector<std::string> strContent;
    
    noticeBufferData = ForceGBKToUtf8(noticeBufferData.c_str());
    if (HNConverCode::SplitGameNotice(noticeBufferData, strTitle, strContent)) {
        UserDefault::getInstance()->setStringForKey(NOTICE_LIST_TITLE, strTitle);
        std::string str = "";
        for (int i = 0; i < strContent.size(); i++) {
            str += strContent.at(i);
            if (i != strContent.size() - 1)
                str += "\n";
        }
        UserDefault::getInstance()->setStringForKey(NOTICE_LIST_CONTENT, str);
        UserDefault::getInstance()->flush();
    }
    noticeBufferData = "";
}

void GameData::appendingNoticeData(std::string& stringBuffer) {
    noticeBufferData += stringBuffer;
}

void GameData::setValidGames(std::map<int, std::vector<int>>&  mapGames)
{
	_mapValidGames = mapGames;
}

std::vector<int> GameData::getValidGames(int nType){
	auto iter = _mapValidGames.find(nType);
	if (iter != _mapValidGames.end())
	{
		return iter->second;
	}

	std::vector<int> vEmpty;
	return vEmpty;
}

void GameData::setGamesVer(std::map<int, int>&  mapGamesVer, int hallVer) {
	_mapGamesVer = mapGamesVer;
	_hallVer = hallVer;
}

void GameData::setDonateCoin(long long coin)
{
	_donatecoin = coin;
}

long long GameData::getDonateCoin()
{
	return _donatecoin;
}

std::map<int,int> GameData::getGamesVer() {
	return _mapGamesVer;
}

int GameData::getHallsVer()
{
	return _hallVer;
}

void GameData::insertGameResourcePath(int uNameId,std::string pathStr)
{
    gameResourcePathMap[uNameId] = pathStr;
    
}

std::string GameData::getGameResourcePath(int uNameId)
{
    map<int,std::string>::iterator iter;
    for (iter = gameResourcePathMap.begin(); iter != gameResourcePathMap.end(); iter++) {
        if (iter->first == uNameId) {
            return iter->second;
        }
    }
    return "";
}
bool GameData::getIsNotUpdateNewVerison()
{
    
    return isNotUpdateNewVersion;
}
void GameData::setIsNotUpdateNewVerison(bool value)
{
    isNotUpdateNewVersion = value;
    
}
void GameData::setGameJackPotData(int gameId,long long data[5])
{
    vector<long long> jackPotNum;
    for (int i = 0; i < 5; i++) {
        jackPotNum.push_back(data[i]);
    }
    
    int jackpotGameId = getGameJackPotId(gameId);
    if (jackpotGameId != -1) {
        jackpotDatas[jackpotGameId] = jackPotNum;
    }
    
    
}
long long GameData::getGameJackpotByGameId(int gameId)
{
    map<int,vector<long long>>::iterator iter;
    for (iter = jackpotDatas.begin(); iter != jackpotDatas.end(); iter++) {
        if (iter->first == gameId) {
            vector<long long> data = iter->second;
            
            long long jackpot = 0;
            for (int i = 0; i < data.size(); i++) {
                jackpot += data.at(i);
            }
            return jackpot;
        }
    }
    return 0;
}
long long GameData::getGameJackpotByGameRoom(int gameId,int roomIdx)
{
    map<int,vector<long long>>::iterator iter;
    for (iter = jackpotDatas.begin(); iter != jackpotDatas.end(); iter++) {
        if (iter->first == gameId) {
            vector<long long> data = iter->second;
            
            if (roomIdx < data.size()) {
                return data.at(roomIdx);
            }
            
        }
    }
    return 0;
}
int GameData::getGameJackPotId(int arrayIdx)
{
    for (int i = 0; i < vecJackpotId.size(); i++) {
        int id = vecJackpotId.at(i);
        if (i == arrayIdx) {
            return id;
        }
    }
    
    return -1;
}
bool GameData::getGameHasJackpot(int uNameId)
{
    
    bool isHasJackpot = false;
    for (int j = 0; j < vecJackpotId.size(); j++) {
        if (vecJackpotId.at(j) == uNameId) {
            isHasJackpot = true;
            break;
        }
    }
    
    return isHasJackpot;
}
void GameData::saveUserBankPassWord(std::string passWord)
{
     userBankPassword = passWord;
    
}
void GameData::saveUserBankPassWordMD5(std::string passWord)
{
    userBankPasswordMD5 = passWord;
    
}
std::string GameData::getUserBankPassWord()
{
    return userBankPassword;
}
std::string GameData::getUserBankPassWordMD5()
{
    return userBankPasswordMD5;
}
void GameData::setRoomJackpotData(long long jackpot)
{
    m_jackpotValue = jackpot;
}
long long GameData::getRoomJackpotData()
{
    return m_jackpotValue;
}
void GameData::setIosDidEnterBackground()
{
    iosBackgroudTime = time(NULL);
}
bool GameData::getIosBackgroundTimeThanTime()
{
    if (time(NULL) - iosBackgroudTime > 120) { //超过2分钟强制掉线
        
        return true;
    }
    return false;
}

void GameData::setSubPackageUrl(std::string strUrl)
{
	_subPackageUrl = strUrl;
}
std::string GameData::getSubPackageUrl()
{
	return _subPackageUrl;
}

void GameData::setGameIconSpineName(unsigned int uGameID, const std::string & strSpineName)
{
	if (strSpineName.empty() || uGameID == 0)
		return;

	m_mapGameIconSpineName[uGameID] = strSpineName;
}

bool GameData::getGameIconSpineName(unsigned int uGameID, std::string & strSpineName)
{
	bool bRet = false;
	if (m_mapGameIconSpineName.find(uGameID) != m_mapGameIconSpineName.end())
	{
		strSpineName = m_mapGameIconSpineName[uGameID];
		bRet = true;
	}
	return bRet;
}
