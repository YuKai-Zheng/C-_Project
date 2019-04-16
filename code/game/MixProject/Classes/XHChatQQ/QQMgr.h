//
//  QQMgr.h
//  1099_DEMO iOS
//
//  Created by cocos on 2017/12/20.
//

#ifndef QQMgr_h
#define QQMgr_h

#ifdef __OBJC__
#include "XHChatQQ.h"
#import <UIKit/UIKit.h>
#endif

class QQMgr
{
public:
    static bool HasInstalledQQ();
    static void OpenQQ(std::string phoneNumber);
};

#endif /* QQMgr_h */
