
#include"GameTypeV3.h"
#include "GameLists.h"
#include "PlatformDefine.h"
#include "network/HttpRequest.h"
#include "HNLobby/FontConfig.h"
#include "CommonMacro.h"
#include "pattern/CommandManager.h"
#include "PlatformConfig.h"
#include "VersionInfo/VersionInfo.h"

// "PLATFORM_GAMETYPE' 游戏大厅类型
static const char* PLATFORM_TNT = "common/animation/Newyear/bianpao/Fireworks.csb";
static const char* PLATFORM_UP = "common/animation/newmain/platform/platform_up.csb";
static const char* PLATFORM_BUTTON = "common/animation/newmain/button/button.csb";

GameTypeV3::GameTypeV3() {
    m_idxCount = 0;
    _isTouch = true;
    _isMove = false;
}

GameTypeV3::~GameTypeV3() {
}

bool GameTypeV3::init() {
    if (!HNLayer::init())
        return false;

    m_typeCount = 0;
    if (PlatformConfig::getInstance()->funcConfig.isIosSpapicVersion == true) {
        m_typeCount = 2;
    } else {
        m_typeCount = 4;
    }

	if (GD_NEED_OPEN)
	{
		m_typeCount = 2;
	}

    for (int i = 0; i < m_typeCount; i++) {
        char path[50] = { 0 };
        sprintf(path, "v3/button/%d.png", i);
        auto button = Button::create(path, path);
        button->setTag(i);
        auto size = button->getContentSize();
        button->setPosition(Vec2(0 - WIN_SIZE.width, (WIN_SIZE.height *0.54f + (1.5f*size.height) - i *(size.height) + i * 5 - size.height*0.5f)));
        button->addTouchEventListener(CC_CALLBACK_2(GameTypeV3::enterGameTypeEventCallBack, this));
        button->setTouchEnabled(true);

        addChild(button);
        _vecTypeButton.push_back(button);
        if (i == 0) {
            auto diBgcsb = CSLoader::createNode(PLATFORM_BUTTON);
            diBgcsb->setPosition(Vec2(button->getContentSize().width*0.5f, button->getContentSize().height*0.5f));
            button->addChild(diBgcsb);

            auto antAction = CSLoader::createTimeline(PLATFORM_BUTTON);
            button->runAction(antAction);
            antAction->play("saoguang", true);
        }
    }
    schedule(schedule_selector(GameTypeV3::animationMove), 0.15);

    reqOpenFuncByVersionAndPlatformq();
    return true;
}

void GameTypeV3::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void GameTypeV3::initUI() {

    menu = RotateMenu::create();

    for (int i = 0; i < _vecHotGameType.size(); i++) {
        char farmPath[50] = { 0 };
        sprintf(farmPath, "platform/gameList/gameItemRes/%d.png", _vecHotGameType.at(i));
        auto button = Button::create(farmPath, farmPath);
        button->setTag(_vecHotGameType.at(i));
        auto size = button->getContentSize();
        button->setPosition(Vec2(0 - WIN_SIZE.width, (WIN_SIZE.height *0.5f + (1.5f*size.height + 20 + 10) - i *(size.height + 20) - size.height*0.5f)));
        button->addTouchEventListener(CC_CALLBACK_2(GameTypeV3::enterGameCallBack, this));
        button->setTouchEnabled(true);
        button->setSwallowTouches(true);
        button->setPressedActionEnabled(false);
        button->ignoreContentAdaptWithSize(false);
        menu->addMenuItem(button, i);
    }
    menu->setPosition(Vec2(WIN_SIZE.width / 4 * 3 -50, WIN_SIZE.height / 4 * 2.0f + 30.0));
    this->addChild(menu, 2);
    /************************************************************************/

    auto bottombg = Sprite::create("v3/taizi.png");
    bottombg->setPosition(Vec2(menu->getPositionX() - 5, menu->getPositionY() - 23));
    addChild(bottombg);

    if (VERSION_THREE) {
        auto diBgcsb = CSLoader::createNode(PLATFORM_TNT);
        diBgcsb->setPosition(Vec2(75, 353));
        bottombg->addChild(diBgcsb);

        auto antAction = CSLoader::createTimeline(PLATFORM_TNT);
        bottombg->runAction(antAction);
        antAction->play("bianpao", true);
    }

    /************************************************************************/
    for (int i = 0; i < 5; i++) {
        char str[20];
        auto spr = Sprite::create("platform/gameList/gameItemRes/11100600.png");
        spr->setAnchorPoint(Vec2(0.5f, 0.5f));
        spr->setPosition(Vec2(WIN_SIZE.width / 2, WIN_SIZE.height / 0.5f));
        this->addChild(spr);
        _vecSprite.push_back(spr);
    }
    hideAllSprite();

}

void GameTypeV3::menuItem1Callback(cocos2d::Ref* pSender) {
    hideAllSprite();
    _vecSprite.at(0)->setVisible(true);
}
void GameTypeV3::menuItem2Callback(cocos2d::Ref* pSender) {
    hideAllSprite();
    _vecSprite.at(1)->setVisible(true);
}
void GameTypeV3::menuItem3Callback(cocos2d::Ref* pSender) {
    hideAllSprite();
    _vecSprite.at(2)->setVisible(true);
}
void GameTypeV3::menuItem4Callback(cocos2d::Ref* pSender) {
    hideAllSprite();
    _vecSprite.at(3)->setVisible(true);
}
void GameTypeV3::menuItem5Callback(cocos2d::Ref* pSender) {
    hideAllSprite();
    _vecSprite.at(4)->setVisible(true);
}

void GameTypeV3::addAnimationFrame(const char* str, Node* parent, const std::string name) {
    auto csb = CSLoader::createNode(str);
    csb->setPosition(Vec2(parent->getContentSize().width*0.5f, parent->getContentSize().height*0.5f));
    parent->addChild(csb);

    auto antAction = CSLoader::createTimeline(str);
    parent->runAction(antAction);

    antAction->play(name, true);
}

void GameTypeV3::hideAllSprite() {
    for (auto p : _vecSprite) {
        if (p->isVisible())
            p->setVisible(false);
    }
}

void GameTypeV3::closeGameTypeLayer() {
    this->runAction(Sequence::create(FadeOut::create(0.3), CallFunc::create([&]() {
        if (nullptr != onCloseCallBack) {
            onCloseCallBack();
        }
        this->removeFromParent();
    }), nullptr));
}

void GameTypeV3::animationMove(float dt) {
    if (m_idxCount == m_typeCount) {
        //unschedule(schedule_selector(GameType::animationMove));
        return;
    }
    auto y = _vecTypeButton.at(m_idxCount)->getPositionY();
    _vecTypeButton.at(m_idxCount)->runAction(Sequence::create(
                MoveTo::create(0.2f, Vec2(_vecTypeButton.at(m_idxCount)->getContentSize().width*0.5+ m_idxCount * 60 + 50, y)),
                MoveTo::create(0.2f, Vec2(_vecTypeButton.at(m_idxCount)->getContentSize().width*0.5 + m_idxCount * 60 + 25, y))
                , nullptr));
    m_idxCount++;
}

void  GameTypeV3::enterGameTypeEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;
    auto button = static_cast<Button *>(pSender);
    ComNameInfo* game = static_cast<ComNameInfo*>(button->getUserData());
    //auto flag = menu->IsMoveded();
    onEnterGameCallBack();
    auto ta = button->getTag();
    UserDefault::getInstance()->setIntegerForKey("PLATFORM_GAMETYPE", button->getTag());
    UserDefault::getInstance()->setIntegerForKey("PLATFORM_GAMETYPE_RETURN", -1);  // -1正常返回
}

void  GameTypeV3::enterGameCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (_isTouch == false) return;

    auto currentSelectedGame = dynamic_cast<Button*>(pSender);

    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        _isMove = true;
       // menu->setClickButton(_isMove);
        //currentSelectedGame->runAction(ScaleTo::create(0.2f, 1.1f));
        CCLOG(" ==============");
        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED: {
        _isMove = false;
        break;
    }
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        //	 _isTouch = false;
        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        // currentSelectedGame->runAction(ScaleTo::create(0.2f, 1.0f));


        if (_isMove) {
            /**
            *	此处可以获得选择的游戏参数，将其值传递给捕鱼游戏
            *	by HBC
            */
            //Fish_Game_Version_ID = currentSelectedGame->getTag();
            UserDefault::getInstance()->setIntegerForKey("Enter_Game_ID", currentSelectedGame->getTag());

            // 参数校验
            //CCAssert(0 != Fish_Game_Version_ID, "gameID is not value!");
            if (0 == currentSelectedGame->getTag()) {
                // _isTouch = true;
                return;
            }
            //获取 rotemenu 是否进行移动鼠标动作做判定
            GameCreator()->setCurrentGame(currentSelectedGame->getTag());

            //if (nullptr != onRunActionCallBack) {
            // onRunActionCallBack();
            //}
            this->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([&]() {
                if (nullptr != quickEnterGameCallBack) {
                    UserDefault::getInstance()->setIntegerForKey("PLATFORM_GAMETYPE_RETURN", 0);  // -1 时正常返回 否则直接调回 类型界面
                    //menu->setClickButton(_isMove);
                    quickEnterGameCallBack();
                }
                // _isTouch = true;
                this->removeFromParent();
            }), nullptr));
            _isMove = true;
        }
    }
    break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        //	currentSelectedGame->runAction(ScaleTo::create(0.2f, 1.0f));
        break;
    default:
        break;
    }
}

//根据版本号、平台处理是否开启某些功能
void GameTypeV3::reqOpenFuncByVersionAndPlatformq() {
    //LoadingLayer::createLoading(this, gFontConfig_22, GetText("LODING_REQUEST_ROOMLIST"), "platform/loading.png");

    std::string URL = std::string("http://") + getWebServerUrl() + std::string("/Public/AuditConfig.ashx?");//
    std::string str("action=GetHotGame");

    network::HttpRequest *request = new (std::nothrow) network::HttpRequest();
    request->setUrl(URL.c_str());
    request->setRequestType(network::HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GameTypeV3::onHttpOpenFuncResponse, this));
    request->setRequestData(str.c_str(), str.size());

    HttpClient::getInstance()->sendImmediate(request);

    request->release();
}

void GameTypeV3::onHttpOpenFuncResponse(network::HttpClient *client, network::HttpResponse *response) {

    std::vector<char> recv = *response->getResponseData();
    std::string data;
    data.assign(recv.begin(), recv.end());

    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(data.c_str());


    if (doc.HasParseError() || !doc.IsObject() || !doc.HasMember("content") || !doc["content"].IsArray()) {
        // 处理无当前用户
        return;
    }

    // 解析
    for (auto i = 0; i < doc["content"].Size(); i++) {
        if (!doc["content"][i].HasMember("GameID")) {
            continue;
        }
		int nameID = doc["content"][i]["GameID"].GetInt();
		_vecHotGameType.push_back(nameID);
    }

	if (GD_NEED_OPEN)
	{
		_vecHotGameType.clear();
		_vecHotGameType.push_back(33003107);
		_vecHotGameType.push_back(33003108);
		_vecHotGameType.push_back(11105100);
		_vecHotGameType.push_back(11105101);
		_vecHotGameType.push_back(11105102);
		_vecHotGameType.push_back(11105103);
	}

    //LoadingLayer::removeLoading(this);
    CommandManager::getInstance()->execute(REMOVE_LOADER_COM);
    initUI();
}
