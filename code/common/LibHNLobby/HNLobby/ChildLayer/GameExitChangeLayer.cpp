#include "HNLobby/ChildLayer/GameExitChangeLayer.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "../pattern/StateManager.h"
#include "../state/MenuState.h"
#include "HNLobby/pattern/CommandManager.h"
#include "VersionInfo/VersionInfo.h"
//#include "../v99/PlatformResV99.h"
#include "../v66/PlatformResV66.h"
#include "Sound/HNAudioEngine.h"

GameExitChangeLayer::GameExitChangeLayer() {

}

GameExitChangeLayer::~GameExitChangeLayer() {

}

void GameExitChangeLayer::showExitChoose(Node* parent, int zorder, int tag) {
    CCAssert(nullptr != parent, "parent is nullptr");
    if (0 != tag) {
        parent->addChild(this, zorder, tag);
    } else {
        parent->addChild(this, zorder);
    }
}

void GameExitChangeLayer::closeExitChoose() {
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    exitChooseLoader->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]() {
        this->removeFromParent();
    }), nullptr));
}

bool GameExitChangeLayer::init() {
    if ( !HNLayer::init()) return false;

    Size winSize = Director::getInstance()->getWinSize();

    //屏蔽后面的层
    colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
    addChild(colorLayer);
    //事件吞并
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch , Event* event) {

        closeExitChoose();
        return true;
    };
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);

    exitChooseLoader = CSLoader::createNode("csb/platform/exitChange/exitNode.csb");
    exitChooseLoader->setPosition(winSize/2);
    exitChooseLoader->setScale(0);
    addChild(exitChooseLoader);
    exitChooseLoader->runAction(ScaleTo::create(0.2, 1));
    auto exitChooseLayout = (Layout*)exitChooseLoader->getChildByName("Panel_1");

    //退出
    auto chooseExit = (Button*)exitChooseLayout->getChildByName("Button_exit");
    //圆形退出按钮
    auto curcularExit = (Button*)exitChooseLayout->getChildByName("exitBtn");
    //切换帐号
    auto chooseSwitch = (Button*)exitChooseLayout->getChildByName("Button_switch");
    //bg
    auto bg = dynamic_cast<ImageView*>(exitChooseLayout->getChildByName("Image_1"));
    //tips
    auto tips = dynamic_cast<ImageView*>(exitChooseLayout->getChildByName("Image_3"));
    auto exitWord = dynamic_cast<Sprite*>(exitChooseLayout->getChildByName("Sprite_3"));
    auto switchWord = dynamic_cast<Sprite*>(exitChooseLayout->getChildByName("Sprite_2"));
	//text
	auto textConent = dynamic_cast<Text*>(exitChooseLayout->getChildByName("Text_2"));
    //
    std::string bgString = "";
    std::string tipString = "";
    std::string exitString = "";
    std::string switchString = "";
//    if (VERSION_NINETY_NINE) {
//        if (curcularExit) {
//            curcularExit->loadTextureNormal(PROMPT_CLOSE_V99);
//            curcularExit->setVisible(false);
//        }
//        if (tips) {
//            tips->setVisible(false);
//        }
//        if (exitWord) {
//            exitWord->setVisible(false);
//        }
//        if (switchWord) {
//            switchWord->setVisible(false);
//        }
//        bgString = PROMPT_BG_V99;
//        exitString = PROMPT_EXIT_BTN_V99;
//        switchString = PROMPT_SWITCH_BTN_V99;
//    } else

	{
// 		bgString = "platform/curcularExit.png";
// 		tipString = "platform/prompt/promptRes/wenxintishi.png";
// 		exitString = 
	}
/*
	if (VERSION_NINETY_NINE || VERSION_EIGHTY_EIGHT)
	{
		auto bgSize = Size::ZERO;
		auto tipSize = Size::ZERO;
		auto btnSize = Size::ZERO;
		auto bgImage = ImageView::create(bgString);
		auto tipImage = ImageView::create(tipString);
		auto btnImage = ImageView::create(exitString);
		if (bgImage) {
			bgSize = bgImage->getContentSize();
			if (bg) {
				bg->loadTexture(bgString);
				bg->setContentSize(bgSize);
			}
		}
		if (tipImage) {
			tipSize = tipImage->getContentSize();
			if (tips) {
				tips->loadTexture(tipString);
				tips->setContentSize(tipSize);
				tips->setPositionY(tips->getPositionY() + 40);
			}
		}
		if (btnImage) {
			btnSize = btnImage->getContentSize();
		}
		if (chooseExit) {
			chooseExit->loadTextures(exitString, exitString);
			chooseExit->setContentSize(btnSize);
			chooseExit->setPosition(Vec2(exitChooseLayout->getContentSize().width * 0.3f, btnSize.height*0.2f));
		}
		if (chooseSwitch) {
			chooseSwitch->loadTextures(switchString, switchString);
			chooseSwitch->setContentSize(btnSize);
			chooseSwitch->setPosition(Vec2(exitChooseLayout->getContentSize().width * 0.7f, btnSize.height*0.2f));
		}
	}
*/
    chooseExit->addTouchEventListener(CC_CALLBACK_2(GameExitChangeLayer::exitCallBack, this));
    curcularExit->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            closeExitChoose();
        }
    });
    chooseSwitch->addTouchEventListener(CC_CALLBACK_2(GameExitChangeLayer::switchCallBack, this));

    return true;
}

void GameExitChangeLayer::exitCallBack(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        selectedBtn->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        selectedBtn->setColor(Color3B(255, 255, 255));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        exit(0);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        exit(0);
#endif
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }

}

void GameExitChangeLayer::switchCallBack(Ref* pSender, Widget::TouchEventType type) {
    Button* selectedBtn = (Button*)pSender;
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        selectedBtn->setColor(Color3B(170, 170, 170));
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        selectedBtn->setColor(Color3B(255, 255, 255));

        RoomLogic()->closeRoom();
        PlatformLogic()->closePlatform();
//        StateManager::getInstance()->setState(MenuState::create(), false);
        CommandManager::getInstance()->execute(GOTO_MENUSTATE_COM);
//          CommandManager::getInstance()->release();
        //GameMenu::returnMenu();
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        selectedBtn->setColor(Color3B(255, 255, 255));
        break;
    default:
        break;
    }

}
