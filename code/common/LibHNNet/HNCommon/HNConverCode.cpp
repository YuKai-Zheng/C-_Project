#include "HNConverCode.h"
#include "HNSingleAutoPtr.h"
#include <errno.h>
#include <assert.h>
#include "Tool/XMLParser.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    #include "win32-specific/icon/include/iconv.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    #include "../../../../cocos2d/libiconv/include/iconv.h"
#elif CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_WINRT && CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
#include <iconv.h>
//    #include "iconv.h"
#endif


namespace HN {

void HNConverCode::parseXML(const char* xml) {
    XMLParser::parseWithFile(xml);
}

std::string HNConverCode::getString(const char* key) {
    return UserDefault::getInstance()->getStringForKey(key);
}

int code_convert(const char *from_charset, const char *to_charset,
                 const char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    memset(outbuf, 0, outlen);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    const char *_inbuf = (char*)inbuf;
#else
    char *_inbuf = (char*)inbuf;
#endif

    size_t outbytes = outlen;

    size_t ret = -1;
    iconv_t ic = NULL;
    ic = iconv_open(to_charset, from_charset);
    if (ic == (iconv_t *) - 1) {
        CCLOG ("iconv_open failed: from: %s, to: %s: %s", from_charset, to_charset, strerror(errno));
        return -1;
    }

    while (inlen > 0) {
        ret = iconv (ic, &_inbuf, &inlen, &outbuf, &outbytes);

        if (ret == -1) {
            CCLOG ("iconv failed: from: %s, to: %s: %s", from_charset, to_charset, strerror(errno));
            return -1;
        }
    }

    ret = iconv_close (ic);
    if (ret == -1) {
        CCLOG ("iconv_close failed: from: %s, to: %s: %s", from_charset, to_charset, strerror(errno));
        return -1;
    }
    
#endif
    
    return 0;
}

//UTF8->GB2312
std::string HNConverCode::Utf8ToGB2312(const char *gb2313) {
    if (nullptr == gb2313) return "";

    size_t inlen = strlen(gb2313);
    if (0 == inlen) return "";

    size_t outlen = inlen * 2 + 2;
    HNPointerArray<char> outbuf(new char[outlen]);
    std::string strRet("");
    if (code_convert("utf-8", "gbk", gb2313, inlen, outbuf, outlen) == 0) {
        strRet = outbuf;
    }
    return strRet;
}

//gbk->UTF8
std::string HNConverCode::GB2312ToUtf8(const char *utf8) {
    if (nullptr == utf8) return "";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return utf8;
#endif

    size_t inlen = strlen(utf8);
    if (0 == inlen) return "";

    size_t outlen = inlen * 2 + 2;
    HNPointerArray<char> outbuf(new char[outlen]);
    std::string strRet(utf8);
    if (code_convert("gbk", "utf-8", utf8, inlen, outbuf, outlen) == 0) {
        strRet = outbuf;
    }
    return strRet;
}

//Force gbk->UTF8
std::string HNConverCode::ForceGB2312ToUtf8(const char *utf8) {
    if (nullptr == utf8) return "";

    size_t inlen = strlen(utf8);
    if (0 == inlen) return "";

    size_t outlen = inlen * 2 + 2;
    HNPointerArray<char> outbuf(new char[outlen]);
	memset(outbuf, 0, outlen);
    std::string strRet(utf8);
    if (code_convert("gbk", "utf-8", utf8, inlen, outbuf, outlen) == 0) {
        strRet = outbuf;
    }
    return strRet;
}
    //解析串封装
    std::vector<std::string> HNConverCode::StrSplit(const std::string& src, const std::string& sep)
    {
        std::vector<std::string> r;
        std::string s;
        for (std::string::const_iterator i = src.begin(); i != src.end(); ++i)
        {
            if (sep.find(*i) != std::string::npos)
            {
                if (s.length())
                    r.push_back(s);
                s = "";
            } 
            else
            {
                s += *i;
            }
        }
        
        if (s.length())
            r.push_back(s);
        
        return r;
    }
    
    //解析游戏公告
    bool HNConverCode::SplitGameNotice(const std::string& strNoticeData, std::string& strTitle, std::vector<std::string>& strContent)
    {
        std::vector<std::string> token = StrSplit(strNoticeData, "#");
        if (2 == token.size())	//有标题的情况
        {
            std::vector<std::string> tk1 = StrSplit(token.at(0), ",");
            if (tk1.size() > 0)
                strTitle = tk1.at(0);
            
            strContent = StrSplit(token.at(1), "|");
        }
        else if (1 == token.size())	//无标题的情况
        {
            strContent = StrSplit(token.at(0), "|");
        }
        else
        {
            return false;
        }
        return true;
    }

}
