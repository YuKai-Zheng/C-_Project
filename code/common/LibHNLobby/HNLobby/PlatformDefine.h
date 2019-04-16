//#include "..\game\TTNN\Classes\GameMessage\BaseMessage.h"
#ifndef __PlatformDefine_H__
#define __PlatformDefine_H__

//#include "HNNetExport.h"
#include "HNBaseType.h"

//////////////////////////////////////////////////////////////////////////
static const char* GAME_SOUND_BUTTON     = "v66/music/slot/fruitStart.mp3";//"music/btnEffect/button_click_sound.mp3";
static const char* GAME_SOUND_CLOSE      = "music/btnEffect/back_button_Click_Sound.mp3";  
static const char* GAME_BACKGROUND_MUSIC = "music/platformBackGround.mp3";                  //游戏背景音乐


//////////////////////////////////////////////////////////////////////////
//static const char* SOUND_VALUE_TEXT		= "sound";
//static const char* MUSIC_VALUE_TEXT		= "music";
static const char* USERNAME_TEXT		= "username";
static const char* PASSWORD_TEXT		= "password";
static const char* SAVE_TEXT			= "save";
static const char* GUEST_LOGIN_TEXT		= "guest";
static const char* GUEST_USERNAME_TEXT	= "guest_username";
static const char* GUEST_PASSWORD_TEXT	= "guest_password";

#define MUSIC_SWITCH "music_switch"
#define EFFECT_SWITCH "effect_switch"
#define IS_FIRST_REGIST "IS_FIRST_REGIST"
//////////////////////////////////////////////////////////////////////////
#define INVALID_SOUND_VALUE		-1
#define MAX_SOUND_VALUE			100
#define MIN_SOUND_VALUE			0

#define INVALID_MUSIC_VALUE		-1
#define MAX__MUSIC_VALUE		100
#define MIN_MUSIC_VALUE			0
//////////////////////////////////////////////////////////////////////////
static const char* LOADING				= "platform/loading.png";

//////////////////////////////////////////////////////////////////////////
enum GameLoadResType { //预加载游戏资源类型
    LOAD_JSXS_GAME,
    LOAD_CSLL_GAME,
    LOAD_EIGHT_GAME,
    LOAD_NINELINE_GAME,
    LOAD_SHZ_GAME,
};
class MoneyChangeNotify {
public:
    virtual ~MoneyChangeNotify() {}
    //更新钱包金币
    virtual void walletChanged(LLONG money) = 0;
    virtual void bankChanged(LLONG money) {};
    virtual void lotteryChanged(LLONG lottery) {}
};

#endif	//__PlatformDefine_H__

