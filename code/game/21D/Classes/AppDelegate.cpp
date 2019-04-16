#include "AppDelegate.h"
#include "D21MessageHead.h"
#include "D21GameTableUI.h"
#include "Products.h"
#include "HNLobbyExport.h"
#include "json/rapidjson.h"
#include "json/document.h"

USING_NS_CC;

cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		float width  = designResolutionSize.width;
		float height = designResolutionSize.height;
		do 
		{
			std::string filename = "config.json";
			if (FileUtils::getInstance()->isFileExist(filename))
			{
				std::string json = FileUtils::getInstance()->getStringFromFile(filename);
				rapidjson::Document doc;
				doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
				if (doc.HasParseError())
				{
					break;
				}

				if (doc.IsObject() && doc.HasMember("width"))
				{
					width = doc["width"].GetInt();
				}
				if (doc.IsObject() && doc.HasMember("height"))
				{
					height = doc["height"].GetInt();
				}
		}
		} while (0);
		glview = glview = GLViewImpl::createWithRect("RedBird", Rect(0, 0, width, height));
		director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    register_all_packages();

 	FileUtils::getInstance()->addSearchPath("Games");
	FileUtils::getInstance()->setXXTeaKey("G9w0BAQEFAASCAl8wgg");

	std::vector<std::string> paths = FileUtils::getInstance()->getSearchPaths();

	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);

	// add products info
	ProductManger::getInstance()->addProducts(prodecuInfos, HN_ARRAY_COUNT(prodecuInfos));

	PlatformConfig::getInstance()->setGameDesignSize(designResolutionSize);

	PlatformConfig::getInstance()->setGameLogo("game_logo.png");
	PlatformConfig::getInstance()->setDownloadName("esyd");
	PlatformConfig::getInstance()->setVersionKey("version_esyd");
	PlatformConfig::getInstance()->setPopulariseUrl("http://hn.kl78.com/api/tuiguang/Tuigung.ashx");


	HNGameCreator::getInstance()->addGame(D21::Game_ID::NAME_ID, HNGameCreator::NORMAL, GAME_CREATE_SELECTOR(D21::GameTableUI::create));

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	auto scene = Scene::create();
//	auto mainLayer = GameMenu::create();
//	scene->addChild(mainLayer);
//	TransitionCrossFade* st = TransitionCrossFade::create(0.8f, scene);
//	Director::getInstance()->runWithScene(st);
//#else 
	auto scene = GameInitial::createScene();
	// run
	director->runWithScene(scene);
//#endif

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
	//Director::getInstance()->pause();
    Director::getInstance()->stopAnimation();
	HNAudioEngine::getInstance()->pauseBackgroundMusic();

    // if you use SimpleAudioEngine, it must be pause
    //SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	//Director::getInstance()->resume();
    Director::getInstance()->startAnimation();
	HNAudioEngine::getInstance()->resumeBackgroundMusic();

    // if you use SimpleAudioEngine, it must resume here
    //SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
