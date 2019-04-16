#ifndef __Tools_h__
#define __Tools_h__

#include "HNBaseType.h"
#include "cocos2d.h"
#include "network/HttpRequest.h"
#include <string>
USING_NS_CC;
using namespace network;
namespace HN 
{
class Tools
{
public:
	//金币显示风格;
	enum GoldDisplayStyle
	{
		GDS_FOUR_SIGNIFICANT_DIGITS,		//数字转换成带单位的字符串，保留4位有效数字;
		GDS_A_COMMA_SEPARATED_FOUR,			//用逗号将每个四位的数分开;
		GDS_FOUR_DECIMAL_PLACES,			//数字转换带单位的字符串，除以10000，保留小数点后四位有效;
	};
	
	enum AutomaticLayoutType
	{
		EALT_HORIZONTAL,		//水平布局
		EALT_VERTICAL			//垂直布局
	};

	enum FindDirType
	{
		EFIND_FILE,				//查找路劲的文件名称;
		EFIND_FOLDER			//查找路劲的文件夹名称;
	};
	
	
	enum LayoutDirectionType
	{
		ELDT_MINUS_DIRECTION	= -1,		//负方向 （向左或是向下）
		ELDT_DUE_DIRECTION		= 1			//正方向 （向右或是向上）
	};

	enum LayoutArrayType
	{
		ELAT_EQUIDISTANCE,			//等距排列
		ELAT_BISECTION				//锚点之间的间隔相等
	};
	
    static bool verifyEmailAddress(const std::string &email);
    static bool verifyPhone(const std::string &phone);
    static bool verifyChinese(const std::string &word);
    static bool verifyNumber(const std::string &word);
    static bool verifyNumberAndEnglish(const std::string &word);
    static std::string fourSeparateMoney(LLONG money);
    static std::string base64urlencode(const std::string &str);
    static std::string numberToChinese(LLONG money);
    static bool isSpecialCharacter(const char *s);
    //增加逗号
    static std::string addComma(LLONG value);
    static int findCharCountOnString(char tmp, std::string str);
    static std::string getNewTotleAssStr(std::string str);
    /************************************************************************/
    /* 数字转换成带单位的字符串，保留4位有效数字  by HBC                    */
    /************************************************************************/
    static std::string convertNumberOnlySignificantDigits(LLONG number);
	//数字转换带单位的字符串，除以10000，保留小点后四位有效;
	static std::string convertNumberSignificantDigitsOfFourDecimalPlaces(LLONG number);
	//统一封装金币显示函数，通过类型选择显示类型
	static std::string goldDisplayByType(LLONG number,bool bFromConfig=true, GoldDisplayStyle eType = GDS_FOUR_SIGNIFICANT_DIGITS);
    
    static bool headFromWX(unsigned int iHeadID);
    
    static Image *getImageData(const char *filename, Size &size);
    static Color4B getPixelColor(unsigned char *data, const Vec2 &pos, Size size);
    
    static bool needLoadWechatImage();
    static float loadWechatImageFromLocal(Texture2D* texture);
    
    static void requestRobotIcon(std::string string, ccHttpRequestCallback &callBack, bool forRobot = true);
    static void getImageByData(HttpClient* client, HttpResponse* response, Image* image);
    //static Texture2D* getTextureByImage(Image* image);
    
    //是否是iOS审核包
    static bool isiOSReview();
    
    //加动画 pos偏移量
    static void addAnimationFrame(const char* resPath, Node* parent, const char* actionName, Vec2 pos, float fScale = 1.0f);
    static int getLianxianGameData(long long winMoney,long long noteMoney,int minMutiple,int maxMutiple);
    static std::string getUserNameFunc(const std::string name);

	//父节点通过节点名称查找子节点;
	static Node* getNodeByName(Node*root, std::string name);
	//自动布局;
	static void AutomaticLayout(std::vector<Node*>&nodes, float fBegin, float fSpace, AutomaticLayoutType eALTType, LayoutDirectionType eDir, LayoutArrayType eLAType);
	//获取一组node的整体大小;
	static Size GetNodesContentSize(std::vector<Node*>&nodes);
	//计算缩放比率;
	static float CalcScaleValue(float fStandardLength, float fCurrentLength, float fFixLength);

	static void PlayFrameAnimation(Node * root, const std::string & filename, Vec2 pos = Vec2::ZERO, float fScale = 1.0f);

	static Ref * PlaySkeletonAnimation(Node * root,const std::string & pathname,
		const std::string & filename,int localZOrder=-1,
		Vec2 vPos = Vec2::ZERO,std::string strNodeName = "default_name",bool bAuto=true);

	//static spine::SkeletonAnimation * GetSkeletonAnimation(const std::string & pathname, const std::string & filename);

	static bool IsSearchPathExist(const std::string& path);

	static void GetFileDirListByPath(std::vector<std::string>&listFileName, const std::string strFilePath,FindDirType e = EFIND_FILE);

	static void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

	static bool Decompress(const std::string &zipfile);

private:
	static void getNodeByRoot(Node*root, std::string name, Node*&node);
	static std::string basename(const std::string &path);
};
}


#endif // __Tools_h__
