#include "MenuState.h"

#include "VersionInfo/VersionInfo.h"
//#include "../v100/GameMenuV100.h"
//#include "../v2/GameMenuV2.h"
//#include "../v3/GameMenuV3.h"
//#include "../v1/GameMenuV1.h"
#include "../model/one/GameMenuOne.h"
//#include "../v99/GameMenuv99.h"
//#include "../v88/GameMenuV88.h"
#include "../v66/GameMenuV66.h"
#include "../globel.h"
#include "../GameHTTPRequestManager.h"

void MenuState::enter() {
    if (VERSION_ONE) {//v1进入大厅时，显示 游戏列表，此时该值要为false
        UserDefault::getInstance()->setBoolForKey("FIRST_LOGIN", false);
    } else {//其他版本进入大厅时，显示 游戏类型，此时该值要为true
        UserDefault::getInstance()->setBoolForKey("FIRST_LOGIN", true);
    }

//    GameHTTPRequestManager::getInstance()->reqOpenFuncByVersionAndPlatform(APP_VERSION);
    GameHTTPRequestManager::getInstance()->reqHelpFuncByVersionAndPlatform(APP_VERSION);
    //GameHTTPRequestManager::getInstance()->reqGameTypeAndValidGames(APP_VERSION);
    //热门游戏
	GameHTTPRequestManager::getInstance()->reqHotGameByVersionAndPlatformq();
    GameHTTPRequestManager::getInstance()->reqWechatIconLink();

//    UserDefault::getInstance()->setBoolForKey(HTTP_REQUEST_FAILED, false);
    UserDefault::getInstance()->flush();
}

void MenuState::exit() {
}

void MenuState::execute() {
    if (VERSION_SIXTY_SIX)
    {
		auto layer = GameMenuV66::create();
		addChild(layer);
	}
}

bool MenuState::init() {
    if (!HNScene::init()) {
        return false;
    }

    return true;
}
