#ifndef GAME_AD_SPACE_MANAGER_H
#define GAME_AD_SPACE_MANAGER_H

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::network;

class GameADSpaceManager
{
public:
	struct ADSpaceInfo
	{
		ADSpaceInfo()
		{
			strFileName.clear();
			strAppUrl.clear();
		}
		std::string strFileName;
		std::string strAppUrl;
	};

	typedef std::vector<ADSpaceInfo> ADSpaceInfoVector;
	GameADSpaceManager();
    ~GameADSpaceManager();
    
    static GameADSpaceManager* getInstance();

	void setADSpaceInfo(const std::string & strUrl, const std::string& strVersionCode);
	std::string getADSpaceDownLoadUrl();
	std::string getADSpaceVersionCode();
	void init();
	
	void readConfig(const std::string & strPath);
	ADSpaceInfoVector getADSpaceInfo();
	std::string getCurrentDir();

private:
	
	void checkADSpaceData();

	std::string getRootDir();
	std::string getDefaultDir();
	std::string getADSpaceZip();
	std::string getConfigPath();
protected:
	void onTaskDownLoadSuccess(const DownloadTask& task);
	
	
private:
	

	ADSpaceInfoVector			_listADInfo;

	//广告轮播版本下载地址Url;
	std::string					_strADSpaceDownLoadUrl;
	//广告轮播版本信息是更新的时间也是广告资源的所在文件;
	std::string					_strADSpaceVersionCode;

	std::string					_strADSpaceZip;

	std::string					_strADSpaceRootDir;

	std::string					_strADSpaceCurDir;

	std::string					_strADSpaceConfigPath;

	std::unique_ptr<network::Downloader> _downloader;
	std::string					_storePath;

	std::mutex					_dataMutex;
    
};

#endif /* GAME_AD_SPACE_MANAGER_H */
