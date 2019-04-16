//
//  iOSClipboard.cpp
//  MixProject
//
//  Created by Sunsong Ye on 16/10/17.
//
//

#include "iOSClipboard.h"
#import <Foundation/Foundation.h>

void iOSClipboard::copy(std::string str) {
    //把string类型转换成为char*
    char*p=(char*)str.data();
    
    //把char*转换成OC的NSString
    NSString *nsMessage= [[NSString alloc] initWithCString:p encoding:NSUTF8StringEncoding];
    
    //获得iOS的剪切板
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    
    //改变剪切板的内容
    pasteboard.string = nsMessage;
}
