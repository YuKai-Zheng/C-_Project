//
//  XCaseStringUtil.hpp
//  LibHNUI
//
//  Created by mac on 2018/8/10.
//  Copyright © 2018年 redbird. All rights reserved.
//

#ifndef XCaseStringUtil_hpp
#define XCaseStringUtil_hpp

#include <stdio.h>
#include "cocos2d.h"


USING_NS_CC;
using namespace std;

class XCaseStringUtil : public Ref{
public:
    XCaseStringUtil();
    ~XCaseStringUtil();
    static XCaseStringUtil* create(std::string str);
    
private:
    virtual bool init(std::string str);
    // 解析字符串
    void parseString();
    
public:
    // 判断是中文还是英文
    bool isEnglishChar(char ch);
    // 计算字符串的长度，中文英文都算作一个字节
    int getLength();
    // 截取字符串，返回截取之后的字符串
    std::string sub(int start, int end = -1, bool isNeedPoint = false);
    
private:
    std::string _target;    // 存储传入的字符串
    vector<string> _result;    // 存储解析之后的字符串
};
#endif /* XCaseStringUtil_hpp */
