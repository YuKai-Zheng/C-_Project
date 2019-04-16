#include "GoldenToadGameLoadingV88.h"
//#include "GoldenToadGameTableUIV88.h"
#include "GoldenToadEnums.h"

#include "GoldenToadVersion.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameLists.h"
#include "VersionInfo/VersionInfo.h"

#include "HNLobby/v66/PlatformResV66.h"
#include "HNLobby/pattern/factory/progressBar/ProgressBarFactory.h"

using namespace GoldenToad;
using namespace Version;

namespace GoldenToad {

    static const char*	NET_PLIST_PATH			=	"v66/buyu/bullet/netImg_V88.plist";
    static const char*	NET_IMAGE_PATH			=	"v66/buyu/bullet/netImg_V88.pvr.ccz";
    
    static const char*	HAICAO_PLIST_PATH		=	"v66/goldenToad/Scene/haicao/haicaoBeike.plist";
    static const char*	HAICAO_IMAGE_PATH		=	"v66/goldenToad/Scene/haicao/haicaoBeike.pvr.ccz";

    
    static const char*	FISH_IMAGE_PATH_TOAD	=   "v66/goldenToad/fishAnimation_TOAD/fishImg_TOAD.pvr.ccz";
    static const char*	FISH_PLIST_PATH_TOAD	=   "v66/goldenToad/fishAnimation_TOAD/fishImg_TOAD.plist";
    static const char*	FISH_IMAGE_PATH_LIKUI	=   "v66/goldenToad/fishAnimation_LIKUI/fishImg_LIKUI.pvr.ccz";
    static const char*	FISH_PLIST_PATH_LIKUI	=	"v66/goldenToad/fishAnimation_LIKUI/fishImg_LIKUI.plist";
    static const char*    FISH_IMAGE_PATH_MONKEY    =   "v66/goldenToad/fishAnimation_MONKEY/fishImg_MONKEY.pvr.ccz";
    static const char*    FISH_PLIST_PATH_MONKEY    =    "v66/goldenToad/fishAnimation_MONKEY/fishImg_MONKEY.plist";
    static const char*	FISH_PATH				=   "v66/goldenToad/fishAnimation/bigFishAnimation.ExportJson";
    static const char*	WATER_PLIST_PATH		=	"v66/goldenToad/Scene/water.plist";
    static const char*	WATER_IMAGE_PATH		=	"v66/goldenToad/Scene/water.pvr.ccz";

	//
	static const char*	BUYU_LOADING_CSB       = "v66/animation/loding/loding.csb";

#define  WINSIZE		Director::getInstance()->getWinSize()

GoldenToadGameLoadingV88::GoldenToadGameLoadingV88()
    : fileNum(0)
    ,_percent(25)
    ,_loading(true) {
    GoldenToadVersion::setGameVersionName(UserDefault::getInstance()->getIntegerForKey("Enter_Game_ID"));
}

GoldenToadGameLoadingV88::~GoldenToadGameLoadingV88() {
}

/**	@param loading
 *		true	异步加载资源
 *	@author
 *		hbc
 */
GoldenToadGameLoadingV88* GoldenToadGameLoadingV88::create(bool loading) {
    GoldenToadGameLoadingV88* pRet = new GoldenToadGameLoadingV88();
    if (nullptr != pRet && pRet->init(loading)) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool GoldenToadGameLoadingV88::init(bool loading) {
    if (!HNLayer::init()) {
        return false;
    }
    srand(time(NULL));
    
    std::string path = "";
    if (Application::getInstance()->getTargetPlatform() == cocos2d::Application::Platform::OS_IPAD) {
        path = GD_NEED_OPEN != false ? "platform/lodingUI/gd_Loding.png" : LOADING_BG_V66;
    }
    else {
        path = GD_NEED_OPEN != false ? "platform/lodingUI/gd_Loding.png" : LOADING_BG_IPHONE_V66;
    }
    
    setBackGroundImage(path);
    auto winSize = Director::getInstance()->getWinSize();

    _loading = loading;

    return true;
}
void GoldenToadGameLoadingV88::loadRes() {
    if(_loading) {
    /*    auto progressBarLeaf = ProgressBarFactory::create(LOADING_BAR_BG_V88, LOADING_BAR_PRE_V88);
        addChild(progressBarLeaf);
        progressBarLeaf->setPosition(Vec2(WIN_SIZE.width / 2, progressBarLeaf->getContentSize().height));*/
        
       // _progressBarDelegate = progressBarLeaf;

		//开奖待机
	    loadNode = CSLoader::createNode(BUYU_LOADING_CSB);
		addChild(loadNode, 100);
		loadNode->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height*0.6));
		auto loadAct = CSLoader::createTimeline(BUYU_LOADING_CSB);
		if (loadNode) {
			loadNode->runAction(loadAct);
			loadAct->play("loding", true);
		}


        switch (GoldenToadVersion::getGameVersionName()) {
            case Enums::GAME_VERSION_TOAD_FISH:
                Director::getInstance()->getTextureCache()->addImageAsync(FISH_IMAGE_PATH_TOAD, CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, FISH_PLIST_PATH_TOAD));
                break;
            case Enums::GAME_VERSION_LIKUI_FISH:
                Director::getInstance()->getTextureCache()->addImageAsync(FISH_IMAGE_PATH_LIKUI, CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, FISH_PLIST_PATH_LIKUI));
                break;
            case Enums::GAME_VERSION_MONKEY_FISH:
                Director::getInstance()->getTextureCache()->addImageAsync(FISH_IMAGE_PATH_MONKEY, CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, FISH_PLIST_PATH_MONKEY));
                break;
            default:
                Director::getInstance()->getTextureCache()->addImageAsync(FISH_IMAGE_PATH_TOAD, CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, FISH_PLIST_PATH_TOAD));
                break;
        }
        
        Director::getInstance()->getTextureCache()->addImageAsync(HAICAO_IMAGE_PATH, CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, HAICAO_PLIST_PATH));
        Director::getInstance()->getTextureCache()->addImageAsync(WATER_IMAGE_PATH, CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, WATER_PLIST_PATH));
        Director::getInstance()->getTextureCache()->addImageAsync(NET_IMAGE_PATH, CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, NET_PLIST_PATH));
        Director::getInstance()->getTextureCache()->addImageAsync("v66/buyu/bullet/dntg_bullet0.png", CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, "v66/buyu/bullet/dntg_bullet0.plist"));
        Director::getInstance()->getTextureCache()->addImageAsync("v66/buyu/bullet/dntg_bullet1.png", CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, "v66/buyu/bullet/dntg_bullet1.plist"));
        Director::getInstance()->getTextureCache()->addImageAsync("v66/buyu/bullet/850newFish.png", CC_CALLBACK_1(GoldenToadGameLoadingV88::loadingTextureCallback, this, "v66/buyu/bullet/850newFish.plist"));//大闹天宫新增850的鱼

        ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(FISH_PATH, this, schedule_selector(GoldenToadGameLoadingV88::loadingCallback));
    }
}

void GoldenToadGameLoadingV88::loadingTextureCallback(Texture2D* textureData, std::string plist) {
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist , textureData);

    //if (_progressBarDelegate) {
    //    char str[100] = { 0 };
    //    _percent += 25.0f;
    //    if (_percent > 100.0f) {
    //        _percent = 100.0f;
    //    }
    //    sprintf(str, "- %d -", _percent);
    //    _progressBarDelegate->updateProgressBar(str, _percent);
    //}

    loadingCallback(0.0f);
}

void GoldenToadGameLoadingV88::loadingCallback(float dt) {
    fileNum++;
    if (fileNum >= 8) {
      /*  if (_progressBarDelegate) {
            _progressBarDelegate->updateProgressBar("- 100 -", 100.0f);
        }*/
        if (nullptr != onCloseCallBack) {

            onCloseCallBack();
        }
        //this->removeFromParent();

	
    }
}
}
