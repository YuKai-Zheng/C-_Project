#ifndef __ThirteenZhang_ResourceMgr_V88_H__
#define __ThirteenZhang_ResourceMgr_V88_H__

#include <string>
//#include <utility>
//#include "HNNetExport.h"
#include "HNBaseType.h"
#include "cocos2d.h"

namespace ThirteenZhang
{
    
    const static char* ThirteenZhang_HELP_CONTENT_PATH = "v66/common/help/13zhangshuoming.png";
    
    class ResourceMgrV88
    {
    public:
        static std::pair<std::string, std::string> getCardTypeResource(bool sex, BYTE cardType, bool forResult = true);
        static std::string getCardTextureFileName(BYTE cardValue);
        static std::string getTypeTextureFileName(std::pair<std::string, std::string> res);
//        static std::string getCardTyeCellResource(BYTE cardValue);
    };
}



#endif // !__ThirteenZhang_ResourceMgrV88_H__
