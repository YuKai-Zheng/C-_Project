#include "GamePrompt.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "VersionInfo/VersionInfo.h"
#include <string>

//static const char* PROMPT_CSB = "csb/platform/prompt/promptNode.csb";
static const int GAMEPROMPT_LAYER_ZORDER = 100000000;		// 子节点弹出框层级
static const int GAMEPROMPT_LAYER_TAG = 100000000;		// 子节点弹出框层级

//const static char* Double_Sure_Name = "Double_Sure";
//const static char* Double_Cancel_Name = "Double_Cancel";

GamePromptLayer::GamePromptLayer()
    : promptLoader(nullptr)
    , promptText(nullptr)

//    , _cancel(nullptr)
    , promptButtonOK(nullptr)
//    , promptButtonSure1(nullptr)
//    , promptButtonSure2(nullptr)
//    , promptButtonCancel(nullptr)
//    , promptLessCoinImage(nullptr)
    , promptTitle(nullptr)
//    , _doubleSure(nullptr)
//    , _doubleCancel(nullptr)
    , _sure(nullptr)
    , _closeCB(nullptr)
    ,closeBtnExecuteEnsure(true)
//    , doubleSure(nullptr)
//    , doubleCancel(nullptr)
{
}

GamePromptLayer::~GamePromptLayer() {
}

void GamePromptLayer::showPrompt(const std::string& prompt) {
    Node* root = Director::getInstance()->getRunningScene();
    CCAssert(nullptr != root, "root is null");

    if (root->getChildByTag(GAMEPROMPT_LAYER_TAG) == nullptr) {
        root->addChild(this, GAMEPROMPT_LAYER_ZORDER, GAMEPROMPT_LAYER_TAG);
    }
    setPrompt(prompt);
}

//void GamePromptLayer::showPromptWithTwoButton(const std::string &prompt) {
//    Node* root = Director::getInstance()->getRunningScene();
//    CCAssert(nullptr != root, "root is null");
//
//    root->addChild(this, GAMEPROMPT_LAYER_ZORDER, GAMEPROMPT_LAYER_TAG);
//    promptButtonSure1->setVisible(false);
//    promptButtonSure2->setVisible(false);
//    promptButtonCancel->setVisible(false);
////    doubleSure->setVisible(true);
////    doubleCancel->setVisible(true);
//    setPrompt(prompt);
//}

//void GamePromptLayer::showLessCoinPrompt(bool lessCoin, const std::string& prompt) {
//    Node* root = Director::getInstance()->getRunningScene();
//    CCAssert(nullptr != root, "root is null");
//
//    root->addChild(this, GAMEPROMPT_LAYER_ZORDER, GAMEPROMPT_LAYER_TAG);
//    showLessCoin(lessCoin);
//    promptButtonSure1->setVisible(false);
//    promptButtonSure2->setVisible(false);
//    promptButtonCancel->setVisible(true);
//    doubleSure->setVisible(false);
//    doubleCancel->setVisible(false);
//    setPrompt(prompt);
//}

void GamePromptLayer::closePrompt() {
    HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
	this->removeFromParent();
    //promptLoader->runAction(Sequence::create(ScaleTo::create(0.0f, 0.3f), CallFunc::create([&]() {
    //    this->removeFromParent();
    //}), nullptr));
}

void GamePromptLayer::setOkayCallBack(std::function<void()> sure) {
    _sure = sure;
}

//void GamePromptLayer::setCancelCallBack(std::function<void()> cancel) {
//    _cancel = cancel;
//}

//void GamePromptLayer::setDoubleSureCallBack(std::function<void()> doubleSureCB) {
//    _doubleSure = doubleSureCB;
//}
//
//void GamePromptLayer::setDoucleCancelCallBack(std::function<void()> doubleCancelCB) {
//    _doubleCancel = doubleCancelCB;
//}

//点击空白区域关闭窗口时执行的回调
void GamePromptLayer::setCloseCallBack(std::function<void()> closeCB) {
    _closeCB = closeCB;
}

bool GamePromptLayer::init() {
    if (!HNLayer::init())
        return false;

    //屏蔽后面的层
    srand(unsigned(time(0)));

	auto rootnode = CSLoader::createNode("csb/newplatform/messagebox/node_messagebox.csb");	//    _login_node.node->setPositionY(-50);
	addChild(rootnode);
	rootnode->setPosition(WIN_SIZE / 2);
	auto blacknode = rootnode->getChildByName("Panel_9");
	blacknode->setScaleX(WIN_SIZE.width / 1600.0);
	blacknode->setScaleY(WIN_SIZE.height / 720.0);

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
        if (_closeCB) {
            _closeCB();
        }
        closePrompt();
        return true;
    };
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, rootnode);

	promptText = (Text*)rootnode->getChildByName("bg")->getChildByName("Text_1");
	promptText->setVisible(true);
	promptButtonOK = (Button*)rootnode->getChildByName("bg")->getChildByName("Button_OK");
	promptButtonOK->addTouchEventListener(CC_CALLBACK_2(GamePromptLayer::menuEventCallBack, this));

    closeBtnExecuteEnsure = true;
    return true;
}

void GamePromptLayer::setTextFontSize(int size) {
//    promptText->setFontSize(size);
}

void GamePromptLayer::setPrompt(const std::string& prompt) {
    if (nullptr != promptText) {
        promptText->setString(prompt);
    }
}

void GamePromptLayer::setPrompt(const std::string& prompt, const std::string& font, int size) {
    if (nullptr != promptText) {
        promptText->setFontName(font);
        promptText->setFontSize(size);
        promptText->setString(prompt);
    }
}

void GamePromptLayer::showLessCoin(bool show) {
//    if (promptTitle != nullptr) {
//        promptTitle->setVisible(!show);
//    }
//    if (promptLessCoinImage != nullptr) {
//        promptLessCoinImage->setVisible(show);
//    }
}

void GamePromptLayer::setPromptCanSelect() {
//    promptButtonSure1->setVisible(true);
//    promptButtonSure2->setVisible(true);
//    promptButtonCancel->setVisible(false);
}

void GamePromptLayer::menuEventCallBack(Ref* pSender, Widget::TouchEventType type) {
	Button* selectedBtn = (Button*)pSender;
    if (type != Widget::TouchEventType::ENDED) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			selectedBtn->setScale(0.8f);
		}
		else if (type == Widget::TouchEventType::CANCELED)
		{
			selectedBtn->setScale(1.0f);
		}
        return;
    }    
    auto name = selectedBtn->getName();

	selectedBtn->setScale(1.0f);
    if (name.compare("Button_OK") == 0) {
        if (nullptr != _sure) {
            _sure();
        }
    }
//    if (name.compare("Button_cancel") == 0) {
//        if (nullptr != _cancel) {
//            _cancel();
//        }
//    }
    closePrompt();
    
}

void GamePromptLayer::doubleButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
//    Button* selectedBtn = (Button*)pSender;
//    auto name = selectedBtn->getName();
//
//    switch (type) {
//    case cocos2d::ui::Widget::TouchEventType::BEGAN:
//        break;
//    case cocos2d::ui::Widget::TouchEventType::ENDED: {
//        closePrompt();
//        if (name.compare(Double_Sure_Name) == 0) {
//            if (nullptr != _doubleSure) {
//                _doubleSure();
//            }
//        }
//        if (name.compare(Double_Cancel_Name) == 0) {
//            if (nullptr != _doubleCancel) {
//                _doubleCancel();
//            }
//        }
//    }
//    break;
//    case cocos2d::ui::Widget::TouchEventType::CANCELED:
//        break;
//    default:
//        break;
//    }
}

//void GamePromptLayer::setCloseExitBtn() {
//    if (_exitBtn) {
//        _exitBtn->setVisible(false);
//    }
//}

void GamePromptLayer::setColorLayerTouchEnabled(bool bTouch) {
    if (!bTouch) {
        touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
            return true;
        };
    } else {
    }
}

Text* GamePromptLayer::getPromptText() {
    return promptText;
}

void GamePromptLayer::setTime(int time) {
    if (!this->getChildByTag(250)) {
        timeText = Label::createWithTTF("", DEFAULT_FONT, FONT_SIZE_25);
        timeText->setTag(250);
        addChild(timeText, 3);
    }
//    auto promptLayout = (Layout*)promptLoader->getChildByName("Panel_1");

    this->mTime = time;
    this->schedule(schedule_selector(GamePromptLayer::updateTime), 1.0f);
    this->timeText->setString(StringUtils::format("(%d)", mTime));
    //this->timeText->setPosition(promptLoader->getPosition() - Vec2(0, promptText->getContentSize().height / 2));
}

void GamePromptLayer::updateTime(float dt) {
    mTime--;
    this->timeText->setString(StringUtils::format("(%d)", mTime));
    if (mTime <= 0) {
        this->unschedule(schedule_selector(GamePromptLayer::updateTime));
        m_callback();
    }
}

void GamePromptLayer::setTimeCallback(TimeCallback callback) {
    this->m_callback = callback;
}

////////////////////////////////////////////////////////////////////////////
//软提示
GamePromptTip::GamePromptTip()
{
}

GamePromptTip::~GamePromptTip()
{
}

bool GamePromptTip::init()
{
	return false;
}

GamePromptTip* GamePromptTip::create(Node * pParent, const Color4B& color, GLfloat width, GLfloat height)
{
	GamePromptTip * layer = new (std::nothrow) GamePromptTip();
	if (layer && layer->initWithColor(color, width, height))
	{
		layer->autorelease();

		pParent->addChild(layer, 10001);

		return layer;
	}
	CC_SAFE_DELETE(layer);
	return nullptr;
}

void GamePromptTip::setGamePromptTip(const std::string & prompt, GLfloat xPos, GLfloat yPos)
{
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(xPos - getContentSize().width / 2, yPos);

	auto textInfo = Label::createWithTTF(prompt, DEFAULT_FONT, FONT_SIZE_25);
	textInfo->setColor(Color3B::WHITE);
	textInfo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	textInfo->setHorizontalAlignment(TextHAlignment::LEFT);
	textInfo->setVerticalAlignment(TextVAlignment::TOP);
	textInfo->setPosition(this->getContentSize() / 2);
	this->addChild(textInfo);

	this->runAction(Sequence::create(MoveBy::create(2.0f, Vec2(0, 100)), CCCallFunc::create([&]() {
		this->removeFromParent();
	}), nullptr));
}

///////////////////////////////////////////////////////////////////////////

