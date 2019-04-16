//
//  QQMgr.c
//  1099_DEMO iOS
//
//  Created by cocos on 2017/12/20.
//

#include "QQMgr.h"

bool QQMgr::HasInstalledQQ()
{
    if([XHChatQQ haveQQ])
    {
        return YES;
    }
    
    return NO;
}

void QQMgr::OpenQQ(std::string phoneNumber)
{
    if (QQMgr::HasInstalledQQ())
    {
        NSString * aString = [NSString stringWithUTF8String:phoneNumber.c_str()];
        
        NSString * numberString = [NSString stringWithFormat:@"%@",aString];
        //传入用来接收临时消息的QQ号码
        //调用QQ客户端,发起QQ临时会话
        [XHChatQQ chatWithQQ:numberString];//@"1686913913"
    }
}
