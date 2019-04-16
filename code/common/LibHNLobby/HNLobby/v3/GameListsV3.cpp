#include "GameListsV3.h"

//static const char* ICON_ANIMATION_SIX_CSB = "common/animation/main_icon/six_cow/six_cow.csb";
//static const char* ICON_ANIMATION_SH_CSB = "common/animation/main_icon/suoha/suoha.csb";
//static const char* ICON_ANIMATION_BJL_CSB = "common/animation/main_icon/baijiale/baijiale.csb";
//static const char* ICON_ANIMATION_LIKUI_CSB = "common/animation/main_icon/likuiifishing/likuiifishing.csb";
//static const char* ICON_ANIMATION_TWONN_CSB = "common/animation/main_icon/two_cow/two_cow.csb";
//static const char* ICON_ANIMATION_FOURNN_CSB = "common/animation/main_icon/four_cow/four_cow.csb";
//static const char* ICON_ANIMATION_THIRTYCARD_CSB = "common/animation/main_icon/thirtycard/shisanzhang.csb";
//static const char* ICON_ANIMATION_BRXJ_CSB = "common/animation/main_icon/nine/nine.csb";
//static const char* ICON_ANIMATION_GLODFINISH_CSB = "common/animation/main_icon/chanchufishing/chanchufishing.csb";
//static const char* ICON_ANIMATION_LE_CSB = "common/animation/main_icon/redbag_icon/redbag_icon.csb";
//static const char* ICON_ANIMATION_FQZS_CSB = "common/animation/main_icon/FQZS_logo/FQZS_logo.csb";
//static const char* ICON_ANIMATION_ZJH_CSB = "common/animation/main_icon/goldflower/goldflower_icon/goldflower_icon.csb";
//static const char* ICON_ANIMATION_HUNDREDNN_JSON = "common/animation/main_icon/hundred_cow/hundred_cow.json";
//static const char* ICON_ANIMATION_HUNDREDNN_ATLAS = "common/animation/main_icon/hundred_cow/hundred_cow.atlas";

//animation csb
static const char* ICON_ANIMATION_SIX_CSB = "common/animation/six_cow/six_cow.csb";
static const char* ICON_ANIMATION_SH_CSB = "common/animation/suoha/suoha.csb";
static const char* ICON_ANIMATION_BJL_CSB = "common/animation/baijiale/baijiale.csb";
static const char* ICON_ANIMATION_LIKUI_CSB = "common/animation/likuiifishing/likuiifishing.csb";
static const char* ICON_ANIMATION_TWONN_CSB = "common/animation/two_cow/two_cow.csb";
static const char* ICON_ANIMATION_FOURNN_CSB = "common/animation/four_cow/four_cow.csb";
static const char* ICON_ANIMATION_THIRTYCARD_CSB = "common/animation/thirtycard/shisanzhang.csb";
static const char* ICON_ANIMATION_BRXJ_CSB = "common/animation/nine/nine.csb";
static const char* ICON_ANIMATION_GLODFINISH_CSB = "common/animation/chanchufishing/chanchufishing.csb";
static const char* ICON_ANIMATION_LE_CSB = "common/animation/redbag_icon/redbag_icon.csb";
static const char* ICON_ANIMATION_FQZS_CSB = "common/animation/FQZS_logo/FQZS_logo.csb";
static const char* ICON_ANIMATION_ZJH_CSB = "common/animation/goldflower_icon/goldflower_icon.csb";
static const char* ICON_ANIMATION_HUNDREDNN_JSON = "common/animation/hundred_cow/hundred_cow.json";
static const char* ICON_ANIMATION_HUNDREDNN_ATLAS = "common/animation/hundred_cow/hundred_cow.atlas";

bool GameListsV3::init() {
    if (!GameLists::init()) {
        return false;
    }
    return true;
}

void GameListsV3::addAnimation(INT gameId, Button* parent) {
    switch (gameId) {
    case 11105100: { //百人
        addAnimationBones(ICON_ANIMATION_HUNDREDNN_JSON, ICON_ANIMATION_HUNDREDNN_ATLAS,parent, "bairen");
        break;
    }
    case 11105101: { //2人
        addAnimationFrame(ICON_ANIMATION_TWONN_CSB, parent, "puke");
        break;
    }
    case 11105102: { //4人
        addAnimationFrame(ICON_ANIMATION_FOURNN_CSB, parent, "zhayan");
        break;
    }
    case 11105103: { //通比
        addAnimationFrame(ICON_ANIMATION_SIX_CSB, parent, "coin");
        break;
    }
    case 12100004: { //13张
        addAnimationFrame(ICON_ANIMATION_THIRTYCARD_CSB, parent, "star");
        break;
    }
    case 12100202: { //梭哈
        addAnimationFrame(ICON_ANIMATION_SH_CSB, parent, "paima");
        break;
    }
    case 33003107: { //金蚕
        addAnimationFrame(ICON_ANIMATION_GLODFINISH_CSB, parent, "jinchan");
        break;
    }
    case 33003108: { //李逵
        addAnimationFrame(ICON_ANIMATION_LIKUI_CSB, parent, "daoguang");
        break;
    }
    case 11100200: { //百家乐
        addAnimationFrame(ICON_ANIMATION_BJL_CSB, parent, "piaofu");
        break;
    }
    case 33001111: { //扫雷红包
        addAnimationFrame(ICON_ANIMATION_LE_CSB, parent, "lihua");
        break;
    }
    case 11100600: { //百人小九
        addAnimationFrame(ICON_ANIMATION_BRXJ_CSB, parent, "nine");
        break;
    }
    case 31000500: { //飞禽走兽
        addAnimationFrame(ICON_ANIMATION_FQZS_CSB, parent, "FQZS");
        break;
    }
    case 12101106: { //炸金花
        addAnimationFrame(ICON_ANIMATION_ZJH_CSB, parent, "Around");
		auto sprite = Sprite::create("common/animation/goldflower_icon/2.png");
		sprite->setPosition(Vec2(140,70));
		parent->addChild(sprite);
        break;
    }
    }
}