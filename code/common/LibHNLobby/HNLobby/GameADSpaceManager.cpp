#include "GameADSpaceManager.h"
#include "GameData.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "globel.h"
#include "Tool/Tools.h"
#include "Tool/DirUtils.h"

using namespace HN;

GameADSpaceManager* mInstance = nullptr;

static std::string g_strZipName = std::string("ad.zip");
static std::string g_strJsonName = std::string("config_ad.json");

GameADSpaceManager* GameADSpaceManager::getInstance() 
{
    if (!mInstance) 
	{
        mInstance = new GameADSpaceManager();
		mInstance->init();
    }
    return mInstance;
}

GameADSpaceManager::GameADSpaceManager()
{
	
}

GameADSpaceManager::~GameADSpaceManager() 
{
    
}

void GameADSpaceManager::setADSpaceInfo(const std::string & strUrl, const std::string& strVersionCode)
{
	_strADSpaceDownLoadUrl = strUrl;
	_strADSpaceVersionCode = strVersionCode;

	checkADSpaceData();
}

std::string GameADSpaceManager::getADSpaceDownLoadUrl()
{
	return _strADSpaceDownLoadUrl;
}

std::string GameADSpaceManager::getADSpaceVersionCode()
{
	return _strADSpaceVersionCode;
}

void GameADSpaceManager::init()
{
	_storePath = FileUtils::getInstance()->getWritablePath();
	if (!Tools::IsSearchPathExist(_storePath))
	{
		FileUtils::getInstance()->addSearchPath(_storePath);
	}
	GameData::getInstance()->SetStorePath(_storePath);
	_strADSpaceRootDir = _storePath + "v66/AD/";
	_strADSpaceCurDir = getDefaultDir();
	_strADSpaceConfigPath = getDefaultDir() + g_strJsonName;
	//读取默认的广告配置信息;
	readConfig(_strADSpaceConfigPath);
}

void GameADSpaceManager::readConfig(const std::string & strPath)
{
	_listADInfo.clear();
	std::string filename = strPath;
	bool bExist = FileUtils::getInstance()->isFileExist(strPath);
	if (!bExist)
	{
		filename = getDefaultDir() + g_strJsonName;
	}
	do 
	{
		if (bExist || FileUtils::getInstance()->isFileExist(filename))
		{
			std::string json = FileUtils::getInstance()->getStringFromFile(filename);
			
			rapidjson::Document doc;
			doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
			if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("data") || !doc["data"].IsArray()) {
				return;
			}

			// 解析;
			std::lock_guard<std::mutex> autoLock(_dataMutex);
			for (auto i = 0; i < doc["data"].Size(); i++)
			{
				ADSpaceInfo item;
				item.strFileName = doc["data"][i]["file_name"].GetString();
				item.strAppUrl = doc["data"][i]["app_url"].GetString();
				_listADInfo.push_back(item);
			}
		}
	} while (0);
	
	
	
}

GameADSpaceManager::ADSpaceInfoVector GameADSpaceManager::getADSpaceInfo()
{
	std::lock_guard<std::mutex> autoLock(_dataMutex);
	return _listADInfo;
}

void GameADSpaceManager::checkADSpaceData()
{
	_strADSpaceZip = getRootDir() + getADSpaceVersionCode() + "/" + g_strZipName;

	_strADSpaceConfigPath = getRootDir() + getADSpaceVersionCode() + "/" + g_strJsonName;

	if (!FileUtils::getInstance()->isDirectoryExist(getRootDir() + getADSpaceVersionCode()))
	{
		//删除除default文件夹外的其他广告位资源;
		std::vector<std::string> listDir;
		Tools::GetFileDirListByPath(listDir, getRootDir(), Tools::EFIND_FOLDER);
		for (int i = 0; i < listDir.size(); ++i)
		{
			std::string stdDelFolder = listDir[i];
			std::string stdDefaultPath = FileUtils::getInstance()->fullPathForFilename(getDefaultDir());
			if (stdDelFolder.compare(stdDefaultPath) != 0)
			{
				//DirUtils::rmdir(stdDelFolder.c_str());
			}
		}

		_downloader.reset(new network::Downloader());
		_downloader->onFileTaskSuccess = CC_CALLBACK_1(GameADSpaceManager::onTaskDownLoadSuccess, this);
		_downloader->createDownloadFileTask(getADSpaceDownLoadUrl(), getADSpaceZip(), "download");
	}
	else
	{
		//读取已经存在的广告配置信息;
		readConfig(getConfigPath());
	}
}

void GameADSpaceManager::onTaskDownLoadSuccess(const DownloadTask& task)
{
	//解压;
	if (Tools::Decompress(getADSpaceZip()))
	{
		//读取最新版本的广告配置信息;
		readConfig(getConfigPath());
	}
}

std::string GameADSpaceManager::getRootDir()
{
	return _strADSpaceRootDir;
}

std::string GameADSpaceManager::getDefaultDir()
{
	return _strADSpaceRootDir + "default/";
}


std::string GameADSpaceManager::getCurrentDir()
{
	return _strADSpaceCurDir;
}

std::string GameADSpaceManager::getADSpaceZip()
{
	return _strADSpaceZip;
}

std::string GameADSpaceManager::getConfigPath()
{
	return _strADSpaceConfigPath;
}

