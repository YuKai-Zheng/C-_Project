#ifndef _GLOBEL_H_
#define _GLOBEL_H_

#include <string>
#include <sstream>
#include <iosfwd>

#include "cocos2d.h"
USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
template<typename T>
std::string to_string(const T& value) {
    std::ostringstream os;
    os << value;
    return os.str();
}
#endif

enum LayerType {
    //游戏类型
    GAMETYPE = 0,
    //游戏列表
    GAMELIST,
    //房间列表
    ROOMLIST,
    //桌子列表
    DESKLIST,
    //游戏&房间列表
    GAMELIST_ROOM
};

//iOS审核
#define IOS_REVIEW          "hasIOSReview"
#define OPEN_HELP_FUNC      "openHelpFunc"

//游戏类型
#define GAME_TYPE           "Game_type"
#define FISH_TYPE           "fish"
#define NN_TYPE             "nn"
#define HAPPY_TYPE          "happy"
#define MULTI_TYPE          "multi"
#define HOT_START_GAME      "hotgame"                   //热门游戏
#define QUICK_START_TYPE    "quickstart"                //8868快速开始游戏
#define NOTICE_LIST_TITLE   "notice_list_title"         //933游戏大厅公告
#define NOTICE_LIST_CONTENT "notice_list_content"       //933游戏大厅公告
//#define HTTP_REQUEST_FAILED "http_fail"                 //HTTP请求失败
//游戏ID
//百人
#define  BRNN_ID			11105100
//2人
#define  TWO_NN_ID			11105101
//4人
#define  FOUR_NN_ID			11105102
//通比
#define  SIX_NN_ID			11105103
//13张
#define  THIRTEENZHANG_ID	12100004
//梭哈
#define STUD_ID				12100202
//金蚕
#define  GOLDEN_TOAD_ID		33003107
//李逵
#define  GAME_LIKUI_ID		33003108
//大闹天宫
#define  GAME_MONKEY_ID		33003109
//百家乐
#define  BJL_ID				11100200
//扫雷红包
#define  LUCKYENVELOPE_ID	33001111
//百人小九
#define  BRXJ_ID			11100600
//飞禽走兽
#define  FQZS_ID			31000500
//炸金花
#define  GOLDEN_FLOWER_ID	12101106
//奔驰宝马
#define BB_ID               31000501
//水果机
#define FRUIT_ID            31000502
//21点
#define D21_ID              12100404
//九线拉王
#define NL_ID               31000506
//财神来了
#define CS_ID               31000507
//僵尸先生
#define JSXS_ID             31000508

//微信登录相关信息
#define WX_LOGIN                "wx_login"                  //是否是微信快速登录
#define WX_ACCESS_TOKEN         "wx_accesstoken"            //接口调用凭证
#define WX_ACCESS_TOKEN_TIME    "wx_accesstokentime"        //接口调用凭证time 超过30天需要重新授权，每29天进行一次授权
#define WX_REFRESH_TOKEN        "wx_refreshtoken"           //用户刷新access_token
#define WX_REFRESH_OVERTIME     (30*24*3600)                //单位 秒
#define WX_OPEN_ID              "wx_openid"                 //授权用户唯一标识
#define WX_UNION_ID             "wx_unionid"                //用户统一标识。针对一个微信开放平台帐号下的应用，同一用户的unionid是唯一的。
#define WX_NICK_NAME            "wx_nickname"               //普通用户昵称
#define WX_HEAD_IMG_URL         "wx_headimgurl"             //用户头像，最后一个数值代表正方形头像大小（有0、46、64、96、132数值可选，0代表640*640正方形头像），用户没有头像时该项为空
#define WX_HEAD_IMG_NAME        "wechatImg.jpg"             //微信用户头像名字
#define WX_IMG_DEFAULT_WIDTH    150.0f                      //默认头像尺寸150，该参数仅用作微信头像上

#define WX_HEAD_IMG_ID          100                         //微信头像的id
#define WX_THUMB_IMAGE_MAX_SIZE (32*1024)

#define ROOT_VIEW_IS_VERTICAL   "root_view_is_vertical"


//void checkFrameCache(std::string frame_name,std::string plistName) {
//    if (SpriteFrameCache::getInstance()->getSpriteFrameByName(frame_name) == nullptr) {
//        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistName);
//    }
//}

#define CHECK_FRAME_CACHE(frameName,plistName)	\
	if (SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName) == nullptr) {	\
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plistName);	\
	}	\

#endif // !_GLOBEL_H_
