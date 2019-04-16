//
//  VersionInfo.h
//  LibHNLogic
//
//  Created by 黄百超 on 7/19/16.
//  Copyright © 2016 redbird. All rights reserved.
//

#ifndef VersionInfo_h
#define VersionInfo_h

#include "HNOperator.h"
#include "HNLobby/PlatformConfig.h"
#include <string>

#define RELEASE_MODE		true
#define DEBUG_MODE		!RELEASE_MODE

#define LOGIN_ONE_KEY		false

#define GD_NEED_OPEN		false // 特版本开关

#define APP_VERSION HN::Operator::requestChannel("sysmodule", "getversion")

#define  VER_ONE			"v1"
#define  VER_TWO			"v2"
#define  VER_THREE			"v3"
#define  VER_SIXTY_SIX	    "v66"
#define  VER_EIGHTY_EIGHT	"v88"
#define  VER_NINETY_NINE	"v99"
#define  VER_HUNDRED		"v100"


#define UI_VERSION (PlatformConfig::getInstance()->getUIVersion())

#define  VERSION_ONE		(UI_VERSION.compare(VER_ONE)==0)
#define  VERSION_TWO		(UI_VERSION.compare(VER_TWO)==0)
#define  VERSION_THREE	(UI_VERSION.compare(VER_THREE)==0)
#define  VERSION_HUNDRED	(UI_VERSION.compare(VER_HUNDRED)==0)
#define  VERSION_NINETY_NINE	(UI_VERSION.compare(VER_NINETY_NINE)==0)
#define  VERSION_EIGHTY_EIGHT	(UI_VERSION.compare(VER_EIGHTY_EIGHT)==0)
#define  VERSION_SIXTY_SIX	(UI_VERSION.compare(VER_SIXTY_SIX)==0)

////游戏不同渠道控制信息
//static std::string APP_PAY_TYPE_MANLAND("5");//大陆App Store Game8090
//static std::string APP_PAY_TYPE_MACAO("6");//澳门App Store 8090娱乐中心
//
//static std::string APP_PAY_TYPE(APP_PAY_TYPE_MANLAND);

#endif /* VersionInfo_h */
