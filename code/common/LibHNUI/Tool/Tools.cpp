#include "Tools.h"
#include "math.h"
#include <regex>
#include "HNLobby/globel.h"
#include "network/HttpResponse.h"
#include "network/HttpClient.h"
#include "HNNetProtocol/HNBaseCommand.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "HNLobby/PlatformConfig.h"
#include "cocostudio/CocoStudio.h"
#include <spine/spine.h>
#include <spine/spine-cocos2dx.h>
#include "DirUtils.h"
#include "external/unzip/unzip.h"

namespace HN {

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

#define LSCANF(X) scanf("%I64d", &X)

bool Tools::verifyEmailAddress(const std::string &email) {
  std::regex pattern(
      "([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
  return std::regex_match(email, pattern);
}

bool Tools::verifyPhone(const std::string &phone) { return true; }

bool Tools::verifyChinese(const std::string &word) {
  std::regex pattern("^[\u4e00-\u9fa5]+$");
  return std::regex_match(word, pattern);
}

bool Tools::verifyNumber(const std::string &word) {
  std::regex pattern("^[0-9]*$");
  return std::regex_match(word, pattern);
}

bool Tools::verifyNumberAndEnglish(const std::string &word) {
  std::regex pattern("^[A-Za-z0-9]+$");
  return std::regex_match(word, pattern);
}

std::string Tools::base64urlencode(const std::string &str) {
  std::string encode = str;
  std::string::size_type pos(0);
  while ((pos = encode.find("+")) != std::string::npos) {
    encode.replace(pos, 1, "%2B");
  }
  pos = 0;
  while ((pos = encode.find("/")) != std::string::npos) {
    encode.replace(pos, 1, "%2F");
  }
  return encode;
}

int Tools::getLianxianGameData(long long winMoney,long long noteMoney,int minMutiple,int maxMutiple) //根据连线游戏的赢钱倍数来换取特效等级
{
    long long multipleNum = winMoney/noteMoney;
    if (multipleNum >= minMutiple && multipleNum < maxMutiple) {
        return 1;
    }
    else if (multipleNum >= maxMutiple)
    {
        return 2;
    }
    
    return 0;
}

Node* Tools::getNodeByName(Node*root, std::string name)
{
	Node* node = NULL;
	getNodeByRoot(root, name, node);
	return node;
}

Size Tools::GetNodesContentSize(std::vector<Node*>&nodes)
{
	Size retSize = Size::ZERO;
	for (int i = 0; i < nodes.size(); i++)
	{
		Node * pNode = nodes[i];
		if (pNode == nullptr)
			continue;
		retSize = retSize + pNode->getContentSize();
	}
	return retSize;
}

float Tools::CalcScaleValue(float s, float c, float f)
{
	return (c - f) / (s - f);
}

void Tools::AutomaticLayout(std::vector<Node*>&nodes, float fBegin, float fSpace, AutomaticLayoutType eALTType, LayoutDirectionType eDir, LayoutArrayType eLAType)
{
	//设置锚点
	for (int i = 0; i < nodes.size(); i++)
	{
		Node * pNode = nodes[i];
		if (pNode == nullptr)
			continue;
		pNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	}

	float fIndex = fBegin;
	for (int i = 0; i < nodes.size(); i++)
	{
		Node * pNode = nodes[i];
		if (pNode == nullptr)
			continue;
		Size size = pNode->getContentSize();
		if (eALTType == EALT_HORIZONTAL)
		{
			if (eLAType == ELAT_EQUIDISTANCE)
			{
				fIndex += (eDir* size.width / 2);
				pNode->setPositionX(fIndex);
				fIndex += (eDir* size.width / 2) + (eDir * fSpace);
			}
			else if (eLAType == ELAT_BISECTION)
			{
				fIndex += (eDir * fSpace);
				pNode->setPositionX(fIndex);
			}
		}
		else if (eALTType == EALT_VERTICAL)
		{
			if (eLAType == ELAT_EQUIDISTANCE)
			{
				fIndex += (eDir* size.height / 2);
				pNode->setPositionY(fIndex);
				fIndex += (eDir* size.height / 2) + (eDir * fSpace);
			}
			else if (eLAType == ELAT_BISECTION)
			{
				fIndex += (eDir * fSpace);
				pNode->setPositionY(fIndex);
			}
		}

	}
}
    
void Tools::getNodeByRoot(Node*root, std::string name, Node*&node)
{
	if (!root)
	{
		node = nullptr;
		return ;
	}

	if (root->getName().compare(name) == 0)
	{
		node = root;
		return ;
	}

	for (const auto& child : root->getChildren())
	{
		if (child->getName().compare(name) == 0)
		{
			node = child;
			break;
		}
		getNodeByRoot(child, name, node);
	}
}

std::string Tools::basename(const std::string &path)
{
	size_t found = path.find_last_of("/\\");

	if (std::string::npos != found)
	{
		return path.substr(0, found);
	}
	else
	{
		return path;
	}
}

std::string Tools::getUserNameFunc(const std::string name)
{
    auto len = name.length();
    if (len > 0) {
        if (len > 10)
        {
            std::string tempName = name.substr(0,3);
            tempName.append("...");
            return tempName;
            
        }
        else
        {
            return name;
        }
    }
    return "";
}
std::string Tools::addComma(LLONG value) {
    bool isNegative = value >= 0 ? false : true; // true 是负数
    if (isNegative) {
        value = llabs(value);
    }
    char buffer[64] = {0};
    sprintf(buffer, "%lld", value);
    char xin[100] = {0}; // 逗号分隔
    sprintf(xin, "%lld", value);

    int len = strlen(buffer);
    if (len > 4) { //超过四位数
        int wei = len % 4 > 0 ? len % 4 : 4;
        int j = 0;
        int idx = 0;
        for (int i = 0; i < len; i++) {
          if (wei == 0) { // 4的倍数
            if (idx++ == 3) {
              xin[j++] = ',';
              idx = 0;
            }
          } else { //非4的倍数
            if (idx++ == wei) {
              xin[j++] = ',';
              idx = 0;
              wei = 3;
            }
          }
          xin[j++] = buffer[i];
        }
    }
    if (isNegative) {
        char ret[100] = {0};
        ret[0] = '-';
        for (int i = 0; i < strlen(xin); i++) {
            ret[i+1] = xin[i];
        }
        return std::string(ret);
    }
    return std::string(xin);
}

std::string Tools::convertNumberOnlySignificantDigits(LLONG number)
{
	char buffer[128] = { 0 };

	//不用格式化直接返回字符串
	sprintf(buffer, "%lld", number);
	//return std::string(buffer);

	//如果为4位数字
	if (number <= 9999) 
	{
		sprintf(buffer, "%lld", number);
	}
	// 5位数字
	else if (number > 9999 && number <= 99999)
	{
		sprintf(buffer, "%0.3f 万", number * 1.0f / 10000.0f);
	}
	// 6位数字
	else if (number > 99999 && number <= 999999) 
	{
		sprintf(buffer, "%0.2f 万", number * 1.0f / 10000.0f);
	}
	// 7位数字
	else if (number > 999999 && number <= 9999999) 
	{
		sprintf(buffer, "%0.1f 万", number * 1.0f / 10000.0f);
	}
	// 8位数字
	else if (number > 9999999 && number <= 99999999)
	{
		sprintf(buffer, "%lld 万", number / 10000);
	}
	// 9位数字
	else if (number > 99999999 && number <= 999999999)
	{
		sprintf(buffer, "%0.3f 亿", number * 1.0f / 100000000.0f);
	}
	// 10位数字
	else if (number > 999999999 && number <= 9999999999) 
	{
		sprintf(buffer, "%0.2f 亿", number * 1.0f / 100000000.0f);
	}
	// 11位数字
	else if (number > 9999999999 && number <= 99999999999)
	{
		sprintf(buffer, "%0.1f 亿", number * 1.0f / 100000000.0f);
	}
	// 12位数字及以上
	else if (number > 99999999999) 
	{
		sprintf(buffer, "%lld 亿", number / 100000000);
	}
	return std::string(ForceGBKToUtf8(buffer));
}

std::string Tools::convertNumberSignificantDigitsOfFourDecimalPlaces(LLONG number)
{
	char buffer[128] = { 0 };

	//不用格式化直接返回字符串
	sprintf(buffer, "%lld", number);
	if (number > 0)
	{
		sprintf(buffer, "%0.2f", number * PlatformConfig::getInstance()->getGoldDisplayRate());
	}


	return std::string(ForceGBKToUtf8(buffer));
}

std::string Tools::goldDisplayByType(LLONG number, bool bFromConfig, GoldDisplayStyle eType)
{
	GoldDisplayStyle eReal = eType;
	if (bFromConfig)
	{
		eReal = (GoldDisplayStyle)atoi(GetText("GOLD_DISPLAY_STYLE"));
	}

	//number = number * PlatformConfig::getInstance()->getGoldExchangeRate();
	
	char szBuffer[64] = { 0 };
	sprintf(szBuffer, "%lld", number);
	std::string strGold = std::string(szBuffer);
	if (eReal == GDS_FOUR_SIGNIFICANT_DIGITS)
	{
		//数字转换成带单位的字符串，保留4位有效数字
		strGold = convertNumberOnlySignificantDigits(number); 
	}
	else if (eReal == GDS_A_COMMA_SEPARATED_FOUR)
	{
		//用逗号将每个四位的数分开;
		strGold = fourSeparateMoney(number);
	}
	else if (eReal == GDS_FOUR_DECIMAL_PLACES)
	{
		//数字转换带单位的字符串，除以10000，保留小数点后四位有效;
		strGold = convertNumberSignificantDigitsOfFourDecimalPlaces(number);
	}
	return strGold;
}

std::string Tools::fourSeparateMoney(LLONG money) 
{
  char buffer[100] = {0};
  sprintf(buffer, "%lld", money);
  char xin[100] = {0}; // 逗号分隔
  {
    if (strlen(buffer) > 4)
	{
      int wei = strlen(buffer) % 4 > 0 ? strlen(buffer) % 4 : 4;
      int j = 0;
      int idx = 0;
      for (int i = 0; i < strlen(buffer); i++)
	  {
        if (wei == 0)
		{
          if (idx++ == 3)
		  {
            xin[j++] = ',';
            idx = 0;
          }
        } 
		else
		{
          if (idx++ == wei)
		  {
            xin[j++] = ',';
            idx = 0;
            wei = 3;
          }
        }
        xin[j++] = buffer[i];
      }
      return xin;
    } 
	else
	{
      return buffer;
    }
  }
}

std::string Tools::numberToChinese(LLONG money) {
  char *sNum[] = {"零", "壹", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖"};
  char *sBit[] = {"", "拾", "佰", "仟"};
  char *sTep[] = {"", "万", "亿", "万", "亿", "万", "亿"};
  char *blank = " ";
  char input[100];
  char output[500];
  LLONG m, n, i, j, isZero = 0;

  memset(output, 0, 500);
  j = money;
  n = (LLONG)log10((LLONG)j) + 1;
  for (i = 0; i < n; i++, j = j / 10)
    input[i] = (char)(j % 10);
  for (i = n - 1; i >= 0; i--) {
    j = input[i];
    if (j == 0)
      isZero = 1;
    else if (j > 0 && isZero == 1) { // N 个零完事了
      strcat(output, sNum[0]);
      isZero = 0;
    }
    if (j > 0) {
      strcat(output, sNum[j]);
      strcat(output, sBit[(i) % 4]);
    }
    if (i % 4 == 0) {
      int m = i + 4;
      if (m > n) {
        m = n;
      }
      bool bAllZero = true;
      for (int k = i; k < m; k++) {
        if (input[k] > 0) {
          bAllZero = false;
        }
      }

      if (!bAllZero) {
        strcat(output, sTep[i / 4]);
        isZero = 0;
      }
    }
  }
  if (strlen(output) == 0)
    strcat(output, sNum[0]);
  return std::string(ForceGBKToUtf8(output));
}

bool Tools::headFromWX(unsigned int iHeadID)
{
    return (iHeadID == WX_HEAD_IMG_ID);
}
    
Image *Tools::getImageData(const char *filename, Size &size) {
  Image *image = new Image();
  image->initWithImageFile(filename);
  size.width = image->getWidth();
  size.height = image->getHeight();
  return image;
}

//是否含有特殊字符 标点符号
bool Tools::isSpecialCharacter(const char *s)
{
	for (int i = 0; i < strlen(s); i++)
	{
		if (s[i] >= 0 && s[i] < 48)
			return false;
		if (s[i] >= 58 && s[i] <= 64)
			return false;
		if (s[i] >= 91 && s[i] <= 96)
			return false;
		if (s[i] >= 123 && s[i] <= 127)
			return false;
	}
	return true;
}

Color4B Tools::getPixelColor(unsigned char *data, const Vec2 &pos, Size size)
{
	unsigned int x = pos.x, y = pos.y, width = size.width, height = size.height;
	Color4B c = { 0, 0, 0, 0 };
	unsigned int *pixel = (unsigned int *)data;
	pixel = pixel + ((height - y - 1) * width + x);
	c.r = *pixel & 0xff;
	c.g = (*pixel >> 8) & 0xff;
	c.b = (*pixel >> 16) & 0xff;
	c.a = (*pixel >> 24) & 0xff; //这个值  就是透明度
	log("r:%d,g:%d,b:%d,a:%d", c.r, c.g, c.b, c.a);
	return c;
}

int Tools::findCharCountOnString(char tmp, std::string str)
{
	if (str == "")
		return 0;
	int count = 0;
	for (auto tmp : str)
	{
		if (tmp == '_')
			count++;
	}
	return count;
}

std::string Tools::getNewTotleAssStr(std::string str)
{
	std::string totleStr;
//	auto count = 0;
	auto flag = false;
	for (auto tmpChar : str)
	{
		if (tmpChar == '_')
		{
			flag = true;
		}
		if (flag)
			totleStr += tmpChar;
	}
	return totleStr;
}
    
    bool Tools::needLoadWechatImage() {
        bool isWechatLogin = UserDefault::getInstance()->getBoolForKey(WX_LOGIN);
        auto fileName = FileUtils::getInstance()->getWritablePath() + WX_HEAD_IMG_NAME;
        bool localImgExist = FileUtils::getInstance()->isFileExist(fileName);
        return isWechatLogin && localImgExist && headFromWX(PlatformLogic()->loginResult.bLogoID);
    }
    
    float Tools::loadWechatImageFromLocal(Texture2D* texture) {
        float scale = 1.0f;
        auto fileName = FileUtils::getInstance()->getWritablePath() + WX_HEAD_IMG_NAME;
        if (UserDefault::getInstance()->getBoolForKey(WX_LOGIN) && FileUtils::getInstance()->isFileExist(fileName)) {
            auto data = FileUtils::getInstance()->getDataFromFile(fileName);
            if (!data.isNull()) {
                Image* image = new (std::nothrow) Image();
                if (image) {
                    image->initWithImageData(data.getBytes(), data.getSize());
                    int imgWidth = image->getWidth();
                    int imgHeight = image->getHeight();
                    int scaleWidth = imgWidth < imgHeight ? imgHeight : imgWidth;
                    scale = WX_IMG_DEFAULT_WIDTH / scaleWidth;
                    texture->initWithImage(image);
#if CC_ENABLE_CACHE_TEXTURE_DATA
					VolatileTextureMgr::addImage(texture, image);
#endif
                    if (texture) {
                        texture->autorelease();
                    }
                    CC_SAFE_RELEASE(image);
                }
            }
        }
        return scale;
    }
    
    //机器人头像处理方法
    void Tools::requestRobotIcon(std::string string, ccHttpRequestCallback &callBack, bool forRobot) {
        char imgURL[200] = {0};
        if (forRobot) {
            sprintf(imgURL, getWXImgURL().c_str(), string.c_str());
        }
        else {
            sprintf(imgURL, "%s", string.c_str());
        }
        cocos2d::network::HttpRequest *request = new (std::nothrow) cocos2d::network::HttpRequest();
        request->setUrl(imgURL);
        request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
        request->setResponseCallback(callBack);
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    
    void Tools::getImageByData(HttpClient* client, HttpResponse* response, Image* image) {
        if (client && response && response->isSucceed()) {
            std::vector<char>* buffer = response->getResponseData();
            char *buff = (char *)malloc(buffer->size());
            std::copy(buffer->begin(), buffer->end(), buff);
            
            if (buffer->size() > 0) {
                if (image) {
                    image->initWithImageData((unsigned char*)buffer->data(),buffer->size());
                }
            }
        }
    }
    
    bool Tools::isiOSReview() {
		if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS) {
			return false;
		}

        return PlatformConfig::getInstance()->funcConfig.isOpenHelpFunc;
    }
    
    void Tools::addAnimationFrame(const char *resPath, cocos2d::Node *parent, const char *actionName, cocos2d::Vec2 pos, float fScale)
	{
        auto csb = CSLoader::createNode(resPath);
        csb->setPosition(Vec2(parent->getContentSize().width*0.5f + pos.x, parent->getContentSize().height*0.5f + pos.y));
        
        csb->setScale(fScale);
        parent->addChild(csb);
        csb->setName(actionName);
        auto antAction = CSLoader::createTimeline(resPath);
        parent->runAction(antAction);
        antAction->play(actionName, true);
    }

	void Tools::PlayFrameAnimation(Node * root, const std::string & filename, Vec2 pos, float fScale)
	{
		if (root == nullptr)
			return;
		auto pCsb		= CSLoader::createNode(filename);
		auto pAction	= CSLoader::createTimeline(filename);
		if (pCsb == nullptr || pAction == nullptr)
			return;
		auto sSize = root->getContentSize();

		auto vPos = Vec2(sSize.width, sSize.height)*0.5f + pos;

		pCsb->setPosition(vPos);
		pCsb->setScale(fScale);
		
		root->addChild(pCsb);
		root->runAction(pAction);

		pAction->gotoFrameAndPlay(0);
	}

	Ref * Tools::PlaySkeletonAnimation(Node * root,const std::string & pathname,
		const std::string & filename, int localZOrder,Vec2 vPos, std::string strNodeName, bool bAuto)
	{
		Ref * pRef = nullptr;

		if (root == nullptr)
			return pRef;

		std::string jsonPath = pathname + filename + std::string(".json");
		std::string atlasPath = pathname + filename + std::string(".atlas");

		Vec2 vOldAP = root->getAnchorPoint();
		root->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		auto pSpine = spine::SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath);
		if (pSpine)
		{
			pSpine->setLocalZOrder(localZOrder);
			pSpine->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			spSkeleton * pSkeleton = pSpine->getSkeleton();
			auto sSize = pSpine->getContentSize();
			Vec2 vSetPos = Vec2(0.0f, -(pSkeleton->data->height / 2)) + vPos;
			pSpine->setPosition(vSetPos);


			spTrackEntry* pEntry = nullptr;

			if(bAuto)
				pEntry = pSpine->setAnimation(0, filename, true);

			if(strNodeName.compare("default_name") == 0)
				pSpine->setName(filename);
			else
				pSpine->setName(strNodeName);

			root->addChild(pSpine, localZOrder);

			pRef = pSpine;
		}

		root->setAnchorPoint(vOldAP);

		return pRef;
	}
	/*
	spine::SkeletonAnimation * Tools::GetSkeletonAnimation(const std::string & pathname, const std::string & filename)
	{
		std::string jsonPath = pathname + filename + std::string(".json");
		std::string atlasPath = pathname + filename + std::string(".atlas");
		auto pSpine = spine::SkeletonAnimation::createWithJsonFile(jsonPath, atlasPath);
		if (pSpine)
		{

		}

		return pSpine;
	}
	*/

	bool Tools::IsSearchPathExist(const std::string& path)
	{
		bool bExist = false;
		std::string strPath = path;
		if (strPath.size() <= 0)
			return bExist;

		
		if (strPath[strPath.size() - 1] != '/')
			strPath.append("/");

		const std::vector<std::string> paths = FileUtils::getInstance()->getSearchPaths();
		auto it = paths.begin();
		while (strPath.size() > 0 && it != paths.end())
		{
			if ((*it).compare(strPath) == 0)
			{
				bExist = true;
				break;
			}
			it++;
		}
		return bExist;
	}

	void Tools::GetFileDirListByPath(std::vector<std::string>&listFileName, const std::string strFilePath,FindDirType e)
	{
		std::string strFullFilePath = FileUtils::getInstance()->fullPathForFilename(strFilePath);
		if (strFullFilePath.empty())
			return;
		DirUtils dir;
		if (dir.open(strFullFilePath.c_str()))
		{
			if (e == EFIND_FILE)
			{
				for (int i = 0; i < dir.getNumFiles(); i++)
				{
					listFileName.push_back(dir.getFileName(i));
				}
			}
			else if (e == EFIND_FOLDER)
			{
				for (int i = 0; i < dir.getNumDirs(); i++)
				{
					listFileName.push_back(dir.getDirName(i));
				}
			}
			dir.close();
		}
		
	}

	void Tools::SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
	{
		v.clear();
		string::size_type pos1, pos2;
		pos2 = s.find(c);
		pos1 = 0;
		while (string::npos != pos2)
		{
			v.push_back(s.substr(pos1, pos2 - pos1));

			pos1 = pos2 + c.size();
			pos2 = s.find(c, pos1);
		}
		if (pos1 != s.length())
			v.push_back(s.substr(pos1));
	}

	bool Tools::Decompress(const std::string &zip)
	{
		// Find root path for zip file
		size_t pos = zip.find_last_of("/\\");
		if (pos == std::string::npos)
		{
			CCLOG("AssetsManagerEx : no root path specified for zip file %s\n", zip.c_str());
			return false;
		}
		const std::string rootPath = zip.substr(0, pos + 1);

		// Open the zip file
		unzFile zipfile = unzOpen(FileUtils::getInstance()->getSuitableFOpen(zip).c_str());//
		if (!zipfile)
		{
			CCLOG("AssetsManagerEx : can not open downloaded zip file %s\n", zip.c_str());
			return false;
		}

		// Get info about the zip file
		unz_global_info global_info;
		if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
		{
			CCLOG("AssetsManagerEx : can not read file global info of %s\n", zip.c_str());
			unzClose(zipfile);
			return false;
		}

		// Buffer to hold data read from the zip file
		char readBuffer[BUFFER_SIZE];
		// Loop to extract all files.
		uLong i;
		for (i = 0; i < global_info.number_entry; ++i)
		{
			// Get info about current file.
			unz_file_info fileInfo;
			char fileName[MAX_FILENAME];
			if (unzGetCurrentFileInfo(zipfile,
				&fileInfo,
				fileName,
				MAX_FILENAME,
				NULL,
				0,
				NULL,
				0) != UNZ_OK)
			{
				CCLOG("AssetsManagerEx : can not read compressed file info\n");
				unzClose(zipfile);
				return false;
			}


			const std::string fullPath = rootPath + fileName;
			// Check if this entry is a directory or a file.
			const size_t filenameLength = strlen(fileName);
			if (fileName[filenameLength - 1] == '/')
			{
				//There are not directory entry in some case.
				//So we need to create directory when decompressing file entry
				if (!FileUtils::getInstance()->createDirectory(basename(fullPath)))
				{
					// Failed to create directory
					CCLOG("AssetsManagerEx : can not create directory %s\n", fullPath.c_str());
					unzClose(zipfile);
					return false;
				}
			}
			else
			{
				// Entry is a file, so extract it.
				// Open current file.
				if (unzOpenCurrentFile(zipfile) != UNZ_OK)
				{
					CCLOG("AssetsManagerEx : can not extract file %s\n", fileName);
					unzClose(zipfile);
					return false;
				}

				// Create a file to store current file.
				FILE *out = fopen(FileUtils::getInstance()->getSuitableFOpen(fullPath).c_str(), "wb");
				if (!out)
				{
					CCLOG("AssetsManagerEx : can not create decompress destination file %s\n", fullPath.c_str());
					unzCloseCurrentFile(zipfile);
					unzClose(zipfile);
					return false;
				}

				// Write current file content to destinate file.
				int error = UNZ_OK;
				do
				{
					error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
					if (error < 0)
					{
						CCLOG("AssetsManagerEx : can not read zip file %s, error code is %d\n", fileName, error);
						fclose(out);
						unzCloseCurrentFile(zipfile);
						unzClose(zipfile);
						return false;
					}

					if (error > 0)
					{
						fwrite(readBuffer, error, 1, out);
					}
				} while (error > 0);

				fclose(out);
			}

			unzCloseCurrentFile(zipfile);

			// Goto next entry listed in the zip file.
			if ((i + 1) < global_info.number_entry)
			{
				if (unzGoToNextFile(zipfile) != UNZ_OK)
				{
					CCLOG("AssetsManagerEx : can not read next file for decompressing\n");
					unzClose(zipfile);
					return false;
				}
			}
		}

		unzClose(zipfile);
		if (remove(zip.c_str()) != 0)
		{
			CCLOG("can not remove downloaded zip file %s", zip.c_str());
		}

		return true;
	}

}





