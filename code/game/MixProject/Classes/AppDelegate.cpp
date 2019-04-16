#include "AppDelegate.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "audio/include/AudioEngine.h"
//
#include "HNNetProtocol/HNBaseCommand.h"
#include "Products.h"
#include "HNLobbyExport.h"
#include "VersionInfo/VersionInfo.h"
#include "HNCommon/HNConverCode.h"
#include "base/SystemInfo.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#include "bugly/CrashReport.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "BuglyCocosPlugin/CrashReport.h"
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

#include "GoldenToadMessageHead.h"
#include "NNCommonDef.h"
#include "BRNNMessageHead.h"
#include "StudMessageHead.h"
#include "ThirteenZhangMessageHead.h"
#include "BBCommonDef.h"
#include "BJLMessageHead.h"
#include "GoldenFlowerMessageHead.h"
#include "FQZSCommonDef.h"
#include "BRXJMessageHead.h"
#include "FruitCommonDef.h"
#include "NewYearMessageHead.h"
#include "SHZCommonDef.h"
#include "FruitSlotMessageHead.h"
#include "OnePieceSlotMessageHead.h"
#include "NLCommonDef.h"
#include "JSXSCommonDef.h"
#include "CSLLCommonDef.h"
#include "MedusaCommonDef.h"
#include "TigerCommonDef.h"
#include "Pubg/Classes/GameMessage/PubgCommonDef.h"
#include "EightSlotMessageHead.h"
#include "HonorCommonDef.h"

#include "Platform/UUIDHelper.h"
#include "HNLobby/state/InitialState.h"
#include "HNLobby/state/MenuState.h"
#include "HNLobby/pattern/StateManager.h"

#include "base/VertifyCodeHelper.h"
#include "HNLobby/PlatformConfig.h"
#include "GameFactory.h"
#include "HNLobby/GameData.h"
#include "RedPacketCommonDef.h"
#include "D21MessageHead.h"

USING_NS_CC;
using namespace cocos2d::experimental;

// 更新授权码
#define APP_INFO_KEY "mixproject"

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };
    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, don't modify or remove this function
static int register_all_packages() {
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID )
   // CrashReport::initCrashReport("de464ca787",true); //900036332
#elif  (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
    CrashReport::initCrashReport("12914eb5a4", true);
#endif
    
    float width = designResolutionSize.width;
    float height = designResolutionSize.height;
    do {
        std::string filename = "config/config.json";
        if (FileUtils::getInstance()->isFileExist(filename)) {
            std::string json = FileUtils::getInstance()->getStringFromFile(filename);
            rapidjson::Document doc;
            doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
            if (doc.HasParseError()) {
                break;
            }

            if (!doc.IsObject()) {
                break;
            }

            if (doc.HasMember("width")) {
                width = doc["width"].GetInt();
            }
            if (doc.HasMember("height")) {
                height = doc["height"].GetInt();
            }

            if (doc.HasMember("PLATFORM_SERVER_ADDRESS")) {
                setPlatformServerAddress(doc["PLATFORM_SERVER_ADDRESS"].GetString());
            }
            if (doc.HasMember("WEB_SERVER_URL")) {
                setWebServerUrl(doc["WEB_SERVER_URL"].GetString());
            }
            if (doc.HasMember("API_SERVER_URL")) {
                setAPIServerUrl(doc["API_SERVER_URL"].GetString());
            }
            if (doc.HasMember("UPDATE_URL")) {
                setUpdateURL(doc["UPDATE_URL"].GetString());
            }
            if (doc.HasMember("PAY_TYPE")) {
                setPayType(doc["PAY_TYPE"].GetString());
            }
            if (doc.HasMember("APP_ID")) {
                setAppId(doc["APP_ID"].GetString());
            }
            if (doc.HasMember("APP_SHARE_KEY")) {
                setAppShareKey(doc["APP_SHARE_KEY"].GetString());
            }
            if (doc.HasMember("WX_APPID")) {
                setWXAppID(doc["WX_APPID"].GetString());
            }
            if (doc.HasMember("WX_SECRET_KEY")) {
                setWXSecretKey(doc["WX_SECRET_KEY"].GetString());
            }
            if (doc.HasMember("WX_IMG_URL")) {
                setWXImgURL(doc["WX_IMG_URL"].GetString());
            }
            if (doc.HasMember("Alipay_notify_url")) {
                setAlipayNotifyURL(doc["Alipay_notify_url"].GetString());
            }
            if (doc.HasMember("Alipay_Web_url")) {
                setAlipayWebURL(doc["Alipay_Web_url"].GetString());
            }
            if (doc.HasMember("EXTEND_VERSION")) {
                setExtendVersion(doc["EXTEND_VERSION"].GetBool());
            }
            if (doc.HasMember("ONLY_186_TRY")) {
                setOnly186Try(doc["ONLY_186_TRY"].GetBool());
            }
            if (doc.HasMember("UI_VERSION")) {
                PlatformConfig::getInstance()->setUIVersion(doc["UI_VERSION"].GetString());
            }
        }
    } while (0);
    //
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
        glview = GLViewImpl::createWithRect("RedBird", Rect(0, 0, width, height));
        director->setOpenGLView(glview);
    }

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	string uuid = UUIDHelper::getUUID();
	SystemInfo::setUUID(uuid);
	UserDefault::getInstance()->setStringForKey("UUID", uuid);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    UserDefault::getInstance()->setStringForKey("UUID", "");

#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    VertifyCodeHelper::getInstance()->setServerUrl(getPlatformServerHost());
	string uuid = UUIDHelper::getUUID();
	SystemInfo::setUUID(uuid);
    UserDefault::getInstance()->setStringForKey("UUID", uuid);
//    HNLog::logInfo("uuid:%s", UserDefault::getInstance()->getStringForKey("UUID").c_str());
#endif
    auto deviceSize = director->getWinSize();
    auto policy     = ResolutionPolicy::FIXED_HEIGHT;
    if (Application::getInstance()->getTargetPlatform() == Application::Platform::OS_IPAD) {
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, policy);
    }
    else {
        
        policy = ResolutionPolicy::FIXED_HEIGHT;
        if (deviceSize.width == 2436 && deviceSize.height == 1125) { //iphonex方案
            //policy = ResolutionPolicy::SHOW_ALL;
        }
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height,policy);
    }
    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

//    FileUtils::getInstance()->addSearchPath("Games/ThirteenZhang/game/card");
//    FileUtils::getInstance()->addSearchPath("Games");

//    register_all_packages();
    
    HN::HNConverCode::parseXML("config/strings.xml");
    PlatformConfig::getInstance()->setSupportIAP(true);
    PlatformConfig::getInstance()->setGameDesignSize(designResolutionSize);
    PlatformConfig::getInstance()->setGameLogo("game_logo.png");
    PlatformConfig::getInstance()->setAppKey(APP_INFO_KEY);
	PlatformConfig::getInstance()->setGoldDisplayRate(GetText("GOLD_DISPLAY_RATE"));
    
    if (PRESSED_TEXTURE) {
        //Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::PVRTC4);
        Image::setPVRImagesHavePremultipliedAlpha(true);
    } else {
        //Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);
        //Image::setPNGPremultipliedAlphaEnabled(true);
    }

    //游戏顺序  捕鱼 牛牛 抢红包 金鲨银鲨 百家乐  13水 梭哈 百人小九
    //金蟾捕鱼
    HNGameCreator::getInstance()->addGame(GoldenToad::GAME_TOAD_NAME_ID, GoldenToad::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(GoldenToad_GAME));
    //李逵捕鱼
    HNGameCreator::getInstance()->addGame(GoldenToad::GAME_LIKUI_NAME_ID, GoldenToad::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(Likui_GAME));
    //大闹天宫
    HNGameCreator::getInstance()->addGame(GoldenToad::GAME_MONKEY_NAME_ID, GoldenToad::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(Monkey_GAME));
    //二人牛牛
    HNGameCreator::getInstance()->addGame(NN::GAME_TWO_NAME_ID, NN::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(TWO_NN_GAME));
    //四人牛牛
    HNGameCreator::getInstance()->addGame(NN::GAME_FOUR_NAME_ID, NN::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(FOUR_NN_NAME));
    //六人牛牛
    HNGameCreator::getInstance()->addGame(NN::GAME_SIX_NAME_ID, NN::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(SIX_NN_GAME));
    //百人牛牛
    HNGameCreator::getInstance()->addGame(BRNN::GAME_NAME_ID, BRNN::GAME_KIND_ID, HNGameCreator::BR, GameFactory::create(BRNN_GAME));
    //梭哈
    HNGameCreator::getInstance()->addGame(STUD::GAME_NAME_ID, STUD::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(STUD_GAME));
    //十三张
    HNGameCreator::getInstance()->addGame(ThirteenZhang::GAME_NAME_ID, ThirteenZhang::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(ThirteenZhang_GAME));
    //奔驰宝马
    HNGameCreator::getInstance()->addGame(BB::GAME_NAME_ID, BB::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(BB_GAME));
    //金鲨银鲨
    HNGameCreator::getInstance()->addGame(FQZS::GAME_NAME_ID, FQZS::GAME_KIND_ID, HNGameCreator::BR, GameFactory::create(FQZS_GAME));
    //炸金花
    HNGameCreator::getInstance()->addGame(goldenflower::GAME_NAME_ID, goldenflower::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(GoldenFlower_GAME));
    //百家乐
    HNGameCreator::getInstance()->addGame(BJL::GAME_NAME_ID, BJL::GAME_KIND_ID, HNGameCreator::BR, GameFactory::create(BJL_GAME));
    //百人小九
    HNGameCreator::getInstance()->addGame(BRXJ::GAME_NAME_ID, BRXJ::GAME_KIND_ID, HNGameCreator::BR, GameFactory::create(BRXJ_GAME));
    //水果机
    HNGameCreator::getInstance()->addGame(Fruit::GAME_NAME_ID, Fruit::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(FRUIT_GAME));
    //水浒传
    HNGameCreator::getInstance()->addGame(SHZ::GAME_NAME_ID, SHZ::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(SHZ_GAME));
    //水果拉霸
    HNGameCreator::getInstance()->addGame(FruitSlot::GAME_NAME_ID, FruitSlot::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(FruitSlot_GAME));
    //海贼王拉霸
    HNGameCreator::getInstance()->addGame(OnePieceSlot::GAME_NAME_ID, OnePieceSlot::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(OnePieceSlot_GAME));
    //九线拉王
    HNGameCreator::getInstance()->addGame(NL::GAME_NAME_ID, NL::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(NL_GAME));
    //僵尸先生
    HNGameCreator::getInstance()->addGame(JSXS::GAME_NAME_ID, JSXS::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(JSXS_GAME));
    // 财神来了
    HNGameCreator::getInstance()->addGame(CSLL::GAME_NAME_ID, CSLL::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(CSLL_GAME));

    // 美杜莎
    HNGameCreator::getInstance()->addGame(Medusa::GAME_NAME_ID, Medusa::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(Medusa_GAME));
    
    // Tiger
    HNGameCreator::getInstance()->addGame(Tiger::GAME_NAME_ID, Tiger::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(Tiger_GAME));
    
    //绝地求生
    HNGameCreator::getInstance()->addGame(Pubg::GAME_NAME_ID, Pubg::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(Pubg_GAME));
    
    //888拉霸
    HNGameCreator::getInstance()->addGame(EightSlot::GAME_NAME_ID, EightSlot::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(EightSlot_GAME));
    
	//王者荣耀
	HNGameCreator::getInstance()->addGame(Honor::GAME_NAME_ID, Honor::GAME_KIND_ID, HNGameCreator::NORMAL, GameFactory::create(Honor_GAME));
	
	//微信红包
	HNGameCreator::getInstance()->addGame(RedPacket::GAME_NAME_ID, RedPacket::GAME_KIND_ID, HNGameCreator::BR, GameFactory::create(RedPacket_GAME));

	//21点
	HNGameCreator::getInstance()->addGame(D21::NAME_ID, D21::KIND_ID, HNGameCreator::NORMAL, GameFactory::create(D21_GAME));

	//轮盘
	HNGameCreator::getInstance()->addGame(NewYearActivity::GAME_NAME_ID, NewYearActivity::GAME_KIND_ID, HNGameCreator::BR, GameFactory::create(NewYearActivity_GAME));

    StateManager::getInstance()->start(InitialState::create());

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    //创建切换后台取消自动捕鱼监听事件
    EventCustom e("UnlockAutoFire");
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);

    pauseHeartBeatCheck();
    Configuration::getInstance()->setValue("bEnterBackground", Value(true));

    Director::getInstance()->stopAnimation();
    Director::getInstance()->pause();
    Director::getInstance()->stopAnimation();
    HNAudioEngine::getInstance()->pauseBackgroundMusic();
    HNAudioEngine::getInstance()->pauseEffect();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->stopAnimation();
    Director::getInstance()->resume();
    Director::getInstance()->startAnimation();
    HNAudioEngine::getInstance()->resumeBackgroundMusic();
    HNAudioEngine::getInstance()->resumeEffect();

    Configuration::getInstance()->setValue("bEnterBackground", Value(false));
    resumeHeartBeatCheck();
}

void AppDelegate::pauseHeartBeatCheck() {
    HNPlatformLogic::getInstance()->isStartCheck = false;
    HNPlatformLogic::getInstance()->isNeedReLogin = false;
    HNRoomLogic::getInstance()->isStartCheck = false;
    HNRoomLogic::getInstance()->isNeedReLogin = false;
    GameData::getInstance()->setIosDidEnterBackground();
}

void AppDelegate::resumeHeartBeatCheck() {
    HNPlatformLogic::getInstance()->isStartCheck = true;
    HNRoomLogic::getInstance()->isStartCheck = true;
#if(CC_TARGET_PLATFORM  == CC_PLATFORM_IOS)
    if (GameData::getInstance()->getIosBackgroundTimeThanTime()) { //苹果退到后台超时断线处理
        if (HNPlatformLogic::getInstance()->connected()) {
            HNPlatformLogic::getInstance()->onDisConnect();
        }
    }
#endif
    
}
