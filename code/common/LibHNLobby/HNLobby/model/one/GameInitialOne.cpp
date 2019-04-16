#include "GameInitialOne.h"
#include "UI/TestCase.h"
#include "../../pattern/factory/initial/InitialCompositeFactory.h"
#include "../../pattern/CommandManager.h"
#include "HNLobby/GameData.h"
#include "HNLobby/GameHTTPRequestManager.h"

#include "HNLobby/GamePrompt.h"



HNScene *GameInitialOne::createScene() {
    auto scene = HN::HNScene::create();
    auto layer = GameInitialOne::create();
    scene->addChild(layer);
    return scene;
}

void GameInitialOne::onExit() {
    HNLayer::onExit();

    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeAllTextures();
}

bool GameInitialOne::init() 
{
    if (!HNLayer::init()) 
	{
        return false;
    }

	GameHTTPRequestManager::getInstance()->ReqGetADSpaceInformation();

    auto initialComposite = InitialCompositeFactory::createWithVersion();
    addChild(initialComposite);
    m_initialCompositeDelegate = initialComposite;

    initialLogic = new InitialLogic();
    initialLogic->setDelegate(this);

	_storePath = FileUtils::getInstance()->getWritablePath();
	if (!Tools::IsSearchPathExist(_storePath))
	{
		FileUtils::getInstance()->addSearchPath(_storePath);
	}
	GameData::getInstance()->SetStorePath(_storePath);

	if (!FileUtils::getInstance()->isDirectoryExist(_storePath + "v66"))
	{
		//tipLabel = Label::createWithTTF("更新资源中，请稍等...", "platform/fonts/simhei.ttf", 30, Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
		//tipLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		//float fTemp = 210;
		//tipLabel->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 2 - fTemp));
		//tipLabel->setColor(Color3B::RED);
		//this->addChild(tipLabel);

		downloader.reset(new network::Downloader());
		//_downloadUrl = "https://client-1257384861.cos.ap-guangzhou.myqcloud.com/v66.zip";
		//_downloadUrl = "http://testsh1.game5186.net/fenbaoGameMobile/v66.zip";
		_downloadUrl = "http://game-mobile-1255630682.cosgz.myqcloud.com/v66.zip";


		downloader->onTaskProgress = [this](const DownloadTask& task,
			int64_t bytesReceived,
			int64_t totalBytesReceived,
			int64_t totalBytesExpected) 
		{

			float percent = totalBytesExpected ? float(totalBytesReceived * 100 / totalBytesExpected) : 0;
			char str[20] = { 0 };
			//sprintf(str, "- %2.0f -", percent);
			if (m_initialCompositeDelegate) 
			{
				m_initialCompositeDelegate->updateProgressBar(str, percent);
			}
		};
		downloader->onFileTaskSuccess = [this](const DownloadTask& task)
		{
			//解压
			//tipLabel->setString("解压资源中");
			if (Tools::Decompress(_storePath + "v66.zip"))
			{
				//tipLabel->setString("解压成功");
				scheduleUpdate();
			}
			else
			{
				//tipLabel->setString("解压失败");

			}
		};

		downloader->onTaskError = [this](const DownloadTask& task,
			int errorCode,
			int errorCodeInternal,
			const std::string& errorStr) 
		{

			//tipLabel->setString(StringUtils::format("更新资源失败,错误代号:%d", errorCode));
		};

		downloader->createDownloadFileTask(_downloadUrl, _storePath + "v66.zip", "download");
	}
	else
	{
		scheduleUpdate();
	}

    if (TESTCASE_ONOFF)
	{
        TestCase *testCase = TestCase::create();
        addChild(testCase);
    }

    // showVideo();

    return true;
}

void GameInitialOne::update(float dt)
{
    initialLogic->update();
}

void GameInitialOne::showVideo()
{
//    Size size = Director::getInstance()->getVisibleSize();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    HNAudioEngine::getInstance()->playEffect("platform/video/audio.mp3");
//    auto videoPlayer = cocos2d::experimental::ui::VideoPlayer::create();
//    videoPlayer->setPosition(Point(size.width / 2, size.height / 2));
//    videoPlayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    videoPlayer->setContentSize(Size(size.width, size.height));
//    this->addChild(videoPlayer, 1);
//
//    float scale = size.width / size.height;
//    if (videoPlayer) {
//        if (scale <= 1.5) {
//            videoPlayer->setFileName("platform/video/launch_video_iphone.mp4");
//        } else {
//            videoPlayer->setFileName("platform/video/launch_video_iphone586.mp4");
//        }
//        videoPlayer->play();
//        videoPlayer->setFullScreenEnabled(true);
//    }
//    videoPlayer->addEventListener(
//        CC_CALLBACK_2(GameInitialOne::videoEventCallback, this));
//
//#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 ||                              \
//       CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//
//    const int COUNT = 84;
//    int namgTag[COUNT] = {1,  1,  1,  1,  1,  1,  7,  7,  9,  9,  9,  12, 12, 12,
//                          15, 15, 15, 18, 18, 18, 21, 21, 21, 24, 24, 24, 27, 27,
//                          27, 30, 30, 30, 33, 33, 33, 36, 36, 36, 39, 39, 39, 42,
//                          42, 42, 45, 45, 45, 48, 48, 48, 51, 51, 51, 54, 54, 54,
//                          57, 57, 57, 60, 60, 60, 63, 63, 63, 66, 66, 66, 69, 69,
//                          69, 72, 72, 72, 75, 75, 75, 75, 75, 75, 75, 75, 75, 75
//                         };
//    char name[64];
//    auto birdAnimation = Animation::create();
//    birdAnimation->setDelayPerUnit(0.05f);
//    birdAnimation->setLoops(1);
//    for (int i = 0; i < COUNT; i++) {
//        sprintf(name, "platform/video/animaiton/bird%d.jpg", namgTag[i]);
//        birdAnimation->addSpriteFrameWithFile(name);
//    }
//
//    HNAudioEngine::getInstance()->playEffect("platform/video/audio.mp3");
//    auto animate = Animate::create(birdAnimation);
//    auto bird = Sprite::create();
//    bird->setPosition(Vec2(size.width / 2, size.height / 2));
//    bird->runAction(Sequence::create(
//                        animate,
//                        CallFunc::create(CC_CALLBACK_0(GameInitialOne::videoPlayOverCallback, this)),
//                        nullptr));
//    this->addChild(bird);
//
//#endif
}

void GameInitialOne::updateProgressBar(std::string text, float percent) {
    m_initialCompositeDelegate->updateProgressBar(text, percent);

    if (percent >= 100) {
        if (!TESTCASE_ONOFF) {
            this->unscheduleUpdate();
        }
    }
}

float GameInitialOne::getPercentage() {
    return m_initialCompositeDelegate->getPercentage();
}

/**
* 视频播放完成的回调函数
*/
void GameInitialOne::videoPlayOverCallback() {
    CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
    //StateManager::getInstance()->setState(MenuState::create());
    //GameMenu::createMenu();
}

/**
*  视频播放的状态
*  注意这里的代码，此处代码只有在android平台和Ios平台有效
*/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||                              \
     CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void GameInitialOne::videoEventCallback(
    Ref *sender, cocos2d::experimental::ui::VideoPlayer::EventType eventType) {
    switch (eventType) {
    case cocos2d::experimental::ui::VideoPlayer::EventType::PLAYING:
        break;
    case cocos2d::experimental::ui::VideoPlayer::EventType::PAUSED:
        break;
    case cocos2d::experimental::ui::VideoPlayer::EventType::STOPPED:
        break;
    case cocos2d::experimental::ui::VideoPlayer::EventType::COMPLETED:
        videoPlayOverCallback();
        break;
    default:
        break;
    }
}
#endif
