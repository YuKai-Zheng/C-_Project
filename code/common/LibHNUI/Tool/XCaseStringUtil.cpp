//
//  XCaseStringUtil.cpp
//  LibHNUI
//
//  Created by mac on 2018/8/10.
//  Copyright © 2018年 redbird. All rights reserved.
//

#include "XCaseStringUtil.hpp"
#include <vector>
#define CHINESE_CHAR_LENGTH_UTF8 3  // 根据编码格式定义一个中文字符占几个字节(默认为UTF-8,三个字节)


XCaseStringUtil::XCaseStringUtil():
_target("")
{
    
    
}


XCaseStringUtil::~XCaseStringUtil()
{
    
    
}


XCaseStringUtil* XCaseStringUtil::create(std::string str)
{
    auto pRet = new XCaseStringUtil();
    if (pRet && pRet->init(str))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_RELEASE_NULL(pRet);
    return nullptr;
}


bool XCaseStringUtil::init(std::string str)
{
    bool isInit = false;
    do
    {
        _target = str;
        _result.clear();
        
        
        parseString();
        isInit = true;
    } while (0);
    return isInit;
}


/*
 brief 解析字符串，将中英文字符都处理成一个“字节”
 */
void XCaseStringUtil::parseString()
{
    int i = 0;
    while (i < _target.length())
    {
        if (!isEnglishChar(_target.at(i)))
        {
            _result.push_back(_target.substr(i, CHINESE_CHAR_LENGTH_UTF8));  // 一个汉字三个字节
            i = i + CHINESE_CHAR_LENGTH_UTF8;
            
            
        }
        else
        {
            _result.push_back(_target.substr(i, 1));  // 一个英文一个字节
            i = i + 1;
        }
    }
}


/*
 brief 判断字符是英文还是汉字
 param ch 字符（字节）
 return true：是英文；false：是中文
 */
bool XCaseStringUtil::isEnglishChar(char ch)
{
    
    
    /*汉字的三个字节（有些编码格式是两个字节）的最高为都为1,这里采用判断最高位的方法
     将ch字节进行移位运算，右移8位，这样，如果移位后是0，
     则说明原来的字节最高位为0，不是1那么也就不是汉字的一个字节
     */
    if (~(ch >> 8) == 0)
    {
        return false;  //代表不是汉字
    }
    
    
    return true;
}


/*
 brief 获取字符串长度
 param str    目标字符串
 return 字符串的长度
 */
int XCaseStringUtil::getLength()
{
    return _result.size();  // 返回字符串长度
}


/*
 brief 截取字符串
 param start 起始下标，从1开始
 param end    结束下标
 param isNeedPoint 是否需要在末尾添加“...”
 return 截取之后的字符串
 */
string XCaseStringUtil::sub(int start, int end, bool isNeedPoint)
{
    CCASSERT(getLength() != 0, "string is null");
    CCASSERT(start >= 1 && getLength() >= start, "start is wrong");
    CCASSERT(start <= end, "end is wrong");
    
    
    // 容错处理，如果end大于字符串长度，则舍弃多余部分
    end = getLength() >= end ? end : getLength();
    
    
    string temp = "";
    //直接从_result里取即可
    for (int i = start; i <= end; i++)
    {
        temp += _result[i - 1];
    }
    
    
    // 如果字符串太长，在末尾添加“...”
    if (isNeedPoint)
    {
        if (getLength() > end)
        {
            temp += "...";
        }
    }
    return temp;
}
