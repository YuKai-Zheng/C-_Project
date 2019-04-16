#ifndef _GameData_h__
#define _GameData_h__

#include <vector>
#include <string>
#include <map>

class GameData {
    
public:
    GameData();
    ~GameData();
    static GameData* getInstance();
public:
    void init();
    void insertGameList(std::pair<std::string, std::vector<int>> gamesPair);
    std::map<std::string, std::vector<int>> getGameListData();

    bool hasValuesWithKey(std::string key);
    std::vector<int> getValues(std::string key);
    
    void appendingNoticeData(std::string& stringBuffer);
    
    void spliteNoticeData();
    void insertGameResourcePath(int uNameId,std::string pathStr);
    std::string getGameResourcePath(int uNameId);
	void setValidGames(std::map<int, std::vector<int>>&  mapGames);
	std::vector<int> getValidGames(int nType);
	void setGamesVer(std::map<int, int>&  mapGames,int hallVer);
	void setDonateCoin(long long coin);
	long long getDonateCoin();
	std::map<int, int> getGamesVer();
	int getHallsVer();
    bool getIsNotUpdateNewVerison();
    void setIsNotUpdateNewVerison(bool value);
    
    long long getGameJackpotByGameId(int gameId);
    long long getGameJackpotByGameRoom(int gameId,int roomIdx);
    void setGameJackPotData(int gameId,long long data[5]);
    
    int getGameJackPotId(int arrayIdx);
    bool getGameHasJackpot(int uNameId);
    void saveUserBankPassWord(std::string passWord);
    std::string getUserBankPassWord();
    
    void saveUserBankPassWordMD5(std::string passWord);
    std::string getUserBankPassWordMD5();

	void SetStorePath(std::string path);
	std::string getStorePath();

    void setRoomJackpotData(long long jackpot);
    long long getRoomJackpotData();
    void setIosDidEnterBackground();
    bool getIosBackgroundTimeThanTime();//ios退出后台超过两分钟断线

	void setSubPackageUrl(std::string strUrl);
	std::string getSubPackageUrl();

	void setGameIconSpineName(unsigned int uGameID, const std::string & strSpineName);
	bool getGameIconSpineName(unsigned int uGameID,std::string & strSpineName);

	bool isFirstSlideGameListFlag()const {return _bIsFirstSlideGameList;}
	void setFirstSlideGameListFlag(bool bFirst) { _bIsFirstSlideGameList = bFirst; }

	std::string getCodeString()const { return _codeString; }
	void setCodeString(const std::string & code) { _codeString = code; }

private:
    static GameData* mInstance;
    std::map<std::string, std::vector<int>> gameListDatas;
    std::map<int,std::vector<long long>> jackpotDatas;
//    char noticeBufferData[MAX_SEND_SIZE];
//    char* noticeBufferData;
    std::string noticeBufferData;
	std::string _storePath;
	std::string _subPackageUrl;
	std::string _codeString;//手机验证码;


	std::map<int, std::vector<int>> _mapValidGames;
	std::map<int, int> _mapGamesVer;
	int _hallVer;
	long long _donatecoin;
    std::map<int,std::string> gameResourcePathMap;
    bool isNotUpdateNewVersion;  //是否有新版本没有下载

	bool _bIsFirstSlideGameList; //是否是第一次滑动游戏列表;
    
    std::vector<int> vecJackpotId;
    std::string userBankPassword;
    std::string userBankPasswordMD5;
    long long m_jackpotValue;
    time_t iosBackgroudTime; //ios退到后台的时间
	std::map<unsigned int,std::string> m_mapGameIconSpineName;
};

#endif // _GameData_h__
