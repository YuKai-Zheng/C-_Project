#include "HNLobby/GameLists.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/PlatformDefine.h"
#include "HNLobby/GameDesk.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/PlatformConfig.h"
#include "HNLobby/GameRoom.h"
#include <string>
// spine所需要的文件
#include "spine/spine.h"
#include <spine/spine-cocos2dx.h>
#include "Sound/HNAudioEngine.h"
using namespace spine;
// end spine

//该类要在进入游戏时将游戏ID传递给对应游戏，所以需要导入相应游戏项目  by HBC
//#include "../../../../code/game/GoldenToad/Classes/GameMessage/GoldenToadVersion.h"

//using namespace Version;

//////////////////////////////////////////////////////////////////////////
//static const float enter_room_outtime_timer_time = 30.0f;
//////////////////////////////////////////////////////////////////////////
//static const char* request_room_list_text = "获取房间列表......";
//static const char* game_dev_text		  = "游戏正在开发中。";
//////////////////////////////////////////////////////////////////////////
//static const char* GAME_ITEM_UI	= "platform/gameList/gameItem.csb";


static const char* GAME_PAGESPROMPT_PATH = "platform/gameList/gameItemRes/page_Prompt.png";
//static const char* GAME_PAGE_ARROW = "platform/gameList/gameItemRes/arrow.png";

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
// animation json
//static const char* ICON_ANIMATION_SIXNN_JSON = "common/animation/six_cow/six_cow.json";
//static const char* ICON_ANIMATION_SIXNN_ATLAS = "common/animation/six_cow/six_cow.atlas";

static const char* ICON_ANIMATION_HUNDREDNN_JSON = "common/animation/hundred_cow/hundred_cow.json";
static const char* ICON_ANIMATION_HUNDREDNN_ATLAS = "common/animation/hundred_cow/hundred_cow.atlas";

static const Size pageViewGamesSize(1280, 500);

#define GAMELIST_POS_Y_SUB		25.0

GameLists::GameLists()
    : _pageViewGames(nullptr)
    , _currentSelectedGame(nullptr)
    , _currentPageIdx(0)
    , _isTouch(true)
    , _gameIdxLeft(0)
    , _arrowCount(0) {

}

GameLists::~GameLists() {

}

void GameLists::onEnter() {
    HNLayer::onEnter();
}

void GameLists::onExit() {
    HNLayer::onExit();
}

bool GameLists::init() {
    if (!HNLayer::init()) {
        return false;
    }
    /*
    	_pageViewGames = PageView::create();
    	this->addChild(_pageViewGames);*/

    //if (UserDefault::getInstance()->getDoubleForKey("ARROW_HIDE_COUNT"))
    //{
    //	_arrowCount = UserDefault::getInstance()->getDoubleForKey("ARROW_HIDE_COUNT") + 1;
    //	UserDefault::getInstance()->setDoubleForKey("ARROW_HIDE_COUNT", _arrowCount);
    //	if (UserDefault::getInstance()->getDoubleForKey("ARROW_HIDE_COUNT") >= 4)
    //	{
    //		UserDefault::getInstance()->setDoubleForKey("ARROW_HIDE_COUNT", 4);
    //	}
    //}
    //else
    //{
    //	UserDefault::getInstance()->setDoubleForKey("ARROW_HIDE_COUNT", 1);
    //	_arrowCount = UserDefault::getInstance()->getDoubleForKey("ARROW_HIDE_COUNT");
    //}

    this->setCascadeOpacityEnabled(true);

    auto gameType = UserDefault::getInstance()->getIntegerForKey("PLATFORM_GAMETYPE");
    _gameNames = GameCreator()->getValidGames();
    vector<int> vecGameId;
    switch (gameType) {
    case 0: {
        vecGameId.push_back(11105101);
        vecGameId.push_back(11105102);
        vecGameId.push_back(11105103);
        vecGameId.push_back(11105100);
        break;
    }
    case 1: {
        vecGameId.push_back(33003107);
        vecGameId.push_back(33003108);
        if (PlatformConfig::getInstance()->funcConfig.isIosSpapicVersion != true) {
            vecGameId.push_back(31000500);
        }
        break;
    }
    case 2: {
        vecGameId.push_back(12100202);
//        vecGameId.push_back(11100200);
//        vecGameId.push_back(12101106);
        break;
    }
    case 3: {
//        vecGameId.push_back(11100600);
        vecGameId.push_back(12100004);
//        vecGameId.push_back(33001111);
        break;
    }
    }
    _gameCount = vecGameId.size();

    _scrollView = cocos2d::ui::ScrollView::create();
    _scrollView->setBounceEnabled(true);
    _scrollView->setContentSize(Size(100, 50));
    _scrollView->setScrollBarEnabled(false);
    _scrollView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _scrollView->setContentSize(Size(WIN_SIZE.width, WIN_SIZE.height / 10 * 6));
    _scrollView->setPosition(Vec2(0, WIN_SIZE.height * 0.5 - _scrollView->getContentSize().height *0.5));
    this->addChild(_scrollView);

    auto inSize = Size(0, WIN_SIZE.height / 10 * 6);
    float fScaleX = WIN_SIZE.width / 1280;
    _fSideInstance = 32 * fScaleX;
    _fItemInstance = 20 * fScaleX;


    if (_gameNames.size() > 0) {
        for (int i = 0; i < _gameCount ; i++) {
            char str[50] = { 0 };
            sprintf(str, "platform/gameList/gameItemRes/%d.png", vecGameId.at(i));
            auto spr = Sprite::create(str);
            auto button = Button::create();
            button->setPressedActionEnabled(false);
            button->ignoreContentAdaptWithSize(false);
            button->setAnchorPoint(Point(0.5, 0.5));
            button->setContentSize(spr->getContentSize());
            button->addTouchEventListener(CC_CALLBACK_2(GameLists::enterRoomEventCallBack, this));
            button->setTag(vecGameId.at(i));
            float fBtnWidth = button->getContentSize().width;
            if (fBtnWidth < 290) {
                fBtnWidth = 290;
            }
            button->setPositionX(_fSideInstance + fBtnWidth / 2 + i  * (fBtnWidth + _fItemInstance));
            button->setPositionY(_scrollView->getContentSize().height *0.45);
            _scrollView->addChild(button);
            _vecButton.push_back(button);
            inSize.width += fBtnWidth;
            _gameWidth = button->getContentSize().width;

            addAnimation(vecGameId.at(i), button);
        }
        _scrollView->setInnerContainerSize(Size(inSize.width + _fItemInstance*(_gameCount) + _fSideInstance*2, inSize.height));
        //_oneGameDIstance = _fSideInstance + _fItemInstance + _gameWidth;
        //	_totleDistance = _oneGameDIstance;
    }

    // 读取上一次选择游戏列表位置  如果首次登陆 为percent = 0
    //_scrollView->scrollToPercentHorizontal(UserDefault::getInstance()->getDoubleForKey("curSelectedGameData"),0.0001f, false);


    //_prevArrowPrompt = ImageView::create(GAME_PAGE_ARROW);
    //addChild(_prevArrowPrompt);
    //_prevArrowPrompt->setAnchorPoint(Vec2(1, 0.5));
    //_prevArrowPrompt->setName("prevArrowPrompt");
    //_prevArrowPrompt->setPosition(Vec2(10, _scrollView->getPositionY() + _scrollView->getContentSize().height*0.45));
    //_prevArrowPrompt->setRotation(180);

    //_nextArrowPrompt = ImageView::create(GAME_PAGE_ARROW);
    //addChild(_nextArrowPrompt);
    //_nextArrowPrompt->setAnchorPoint(Vec2(1, 0.5));
    //_nextArrowPrompt->setName("nextArrowPrompt");
    //_nextArrowPrompt->setPosition(Vec2(WIN_SIZE.width - 10, _prevArrowPrompt->getPositionY()));
    ////_scrollView->addTouchEventListener(CC_CALLBACK_2(GameLists::scrollowCallBack,this));
    //if (UserDefault::getInstance()->getDoubleForKey("ARROW_HIDE_COUNT") < 4)
    //{
    //
    //	_nextArrowPrompt->setVisible(true);
    //	bool flag = _scrollView->getInnerContainer()->getPositionX() == 0 ? true : false;
    //	_prevArrowPrompt->setVisible(flag);
    //	_scrollView->addEventListenerScrollView(this, scrollvieweventselector(GameLists::scrollViewCallBack));
    //}
    //else
    //{
    //	_prevArrowPrompt->setVisible(false);
    //	_nextArrowPrompt->setVisible(false);
    //}
    return true;
}

void GameLists::addAnimation(INT gameId, Button* parent) {
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
        //addAnimationBones(ICON_ANIMATION_SIXNN_JSON, ICON_ANIMATION_SIXNN_ATLAS,parent, "paodong");
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
        sprite->setPosition(Vec2(140, 70));
        parent->addChild(sprite);
        break;
    }
    default:
        break;
    }
}

void GameLists::closeGameList() {
    this->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([&]() {
        if (nullptr != onCloseCallBack) {
            onCloseCallBack();
        }
        //this->removeFromParent();
    }), nullptr));
}

void GameLists::addAnimationBones(const char* json, const char* atlas, Button* parent, const std::string name) {
    auto skeletonNode = SkeletonAnimation::createWithJsonFile(json, atlas);
    skeletonNode->setPosition(Vec2(parent->getContentSize().width*0.5f, parent->getContentSize().height*0.5f));
    skeletonNode->setAnimation(0, name, true);
    parent->addChild(skeletonNode);
}

void GameLists::addAnimationFrame(const char* str, Node* parent,const std::string name) {
    auto csb = CSLoader::createNode(str);
    csb->setPosition(Vec2(parent->getContentSize().width*0.5f, parent->getContentSize().height*0.5f));
    parent->addChild(csb);

    auto antAction = CSLoader::createTimeline(str);
    parent->runAction(antAction);
    antAction->play(name, true);
}
void GameLists::scrollowCallBack(cocos2d::Ref * pObjc, Widget::TouchEventType type) {
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED: {
        float tmpX = 0.0;
        for (auto icon : _vecButton) {
            auto x = icon->getPositionX();
            x > tmpX;
            tmpX = x;
        }
        for (auto node : _vecButton) {
            auto x = _scrollView->convertToWorldSpaceAR(node->getPosition()).x;

            if (x < 0 - node->getContentSize().width * 0.5f) {

                node->setPositionX(tmpX + _fItemInstance + node->getContentSize().width);
            }
        }
        break;
    }
    case cocos2d::ui::Widget::TouchEventType::ENDED: {

        auto x = this->convertToWorldSpaceAR(_vecButton.at(0)->getPosition()).x;
        break;
    }

    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        break;
    default:
        break;
    }
}


void GameLists::scrollViewCallBack(Ref* ref, ScrollviewEventType type) {
    switch (type) {
    case  ScrollviewEventType::SCROLLVIEW_EVENT_BOUNCE_LEFT: {
        _nextArrowPrompt->setVisible(false);
        _prevArrowPrompt->setVisible(true);
        break;
    }
    case  ScrollviewEventType::SCROLLVIEW_EVENT_BOUNCE_RIGHT: {
        _nextArrowPrompt->setVisible(true);
        _prevArrowPrompt->setVisible(false);
        break;
    }
    default:
        break;
    }
}

Button * GameLists::createItem(int idx,int posIdx) {
    char str[50] = { 0 };
    sprintf(str, "platform/gameList/gameItemRes/%d.png", _gameNames.at(0)->uNameID);
    auto button = Button::create(str);
    button->setAnchorPoint(Point(0.5, 0.5));
    button->addTouchEventListener(CC_CALLBACK_2(GameLists::leftAndRightCallBack, this));
    button->setTag(idx);
    //button->setUserData(_gameNames.at(0));
    float fBtnWidth = button->getContentSize().width;
    button->setPositionX(_fSideInstance + fBtnWidth / 2 + posIdx  * (fBtnWidth + _fItemInstance));
    button->setPositionY(_scrollView->getContentSize().height *.45);
    _vecButton.push_back(button);
    _gameWidth = button->getContentSize().width;

    return button;
}

void GameLists::leftAndRightCallBack(cocos2d::Ref * pObjc, Widget::TouchEventType type) {
    auto image = static_cast<ImageView*>(pObjc);

    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:

        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED:

        CCLOG(" ============== ");
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED:
        break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        break;
    default:
        break;
    }

}

void GameLists::loadScrollVireItem(INT itemSIze) {
    std::vector<ComNameInfo*> gameNames = GameCreator()->getValidGames();
    if (itemSIze <= 4) {
        _scrollView->setInnerContainerSize(Size(WIN_SIZE.width*0.5, WIN_SIZE.height*0.7f));
    }
    int idx = 0;

    for (auto child : gameNames) {
        char str[128];
        sprintf(str, "platform/gameList/gameItemRes/%d.png", child->uNameID);

        Button* pButtonItem;

        if (FileUtils::getInstance()->isFileExist(str)) {
            pButtonItem = Button::create(str);
            pButtonItem->setUserData(child);
            pButtonItem->addTouchEventListener(CC_CALLBACK_2(GameLists::enterRoomEventCallBack, this));
        }
        auto size = _scrollView->getInnerContainerSize();
        size.width += (idx++) * size.width;
        _scrollView->setInnerContainerSize(Size(size.width, _scrollView->getInnerContainerSize().height));
    }
}


void GameLists::createGameList() {
    _pageViewGames = PageView::create();
    _pageViewGames->setContentSize(pageViewGamesSize);
    _pageViewGames->setAnchorPoint(Vec2(0.5f, 0.5f));
    _pageViewGames->setPosition(Vec2(WIN_SIZE.width * 0.5f, WIN_SIZE.height * 0.5f));
    _pageViewGames->setTouchEnabled(true);
    //_pageViewGames->setCustomScrollThreshold(15);
    _pageViewGames->setCascadeOpacityEnabled(true);

    addChild(_pageViewGames, 4, 4);

    std::vector<ComNameInfo*> gameNames = GameCreator()->getValidGames();

    int gameAllCount = gameNames.size();

    int currentIndex = 0;

    int pageCount = (gameAllCount / 4);
    int remainder = (gameAllCount % 4);

    _nextArrowPrompt->setVisible(gameAllCount > 4);

    int pageLen = (remainder == 0) ? pageCount : pageCount + 1;
    int central = -pageLen / 2;
    if (abs(central) > 0) {
        for (int i = 0; i < pageLen; i++) {
            auto page = Sprite::create(GAME_PAGESPROMPT_PATH);
            addChild(page);
            _pagesPrompt.push_back(page);

            page->setPosition(Vec2(WIN_SIZE.width / 2 + central * 25, WIN_SIZE.height * 0.12f));
            if (i > 0) {
                page->setScale(0.8f);
                page->setColor(Color3B(150, 150, 150));
            }
            central++;
        }
    }

    std::vector<ComNameInfo*> pages;

    if (pageCount > 0) {
        for (int currentPage  = 0; currentPage < pageCount; currentPage++) {
            pages.clear();
            for (int room = 0; room < 4; room ++, currentIndex ++) {
                ComNameInfo* roomInfo = gameNames[currentIndex];
                pages.push_back(roomInfo);
            }
            createGamePage(pages);
        }
    }


    if (remainder > 0) {
        pages.clear();
        for (int room = 0; room < remainder; room ++, currentIndex ++) {
            ComNameInfo* roomInfo = gameNames[currentIndex];
            pages.push_back(roomInfo);
        }
        createGamePage(pages);
    }
}

void GameLists::createGamePage(std::vector<ComNameInfo*> games) {
    // 创建游戏列表子页面
    auto gameLayout = Layout::create();
    gameLayout->setName("page");
    gameLayout->setContentSize(pageViewGamesSize);

    gameLayout->setCascadeOpacityEnabled(true);

    int idx = 0;
    for (auto game : games) {
        idx++;
        float posX = -0.1f + idx * 0.24f;

        auto gameButton = createGameItem(game);
        gameButton->setPosition(Vec2(gameLayout->getContentSize().width * posX, gameLayout->getContentSize().height * 0.5f - GAMELIST_POS_Y_SUB));
        gameLayout->addChild(gameButton, 3, idx);
    }

    //添加子页面进入列表中
    _pageViewGames->addPage(gameLayout);

    //拖动监听
    _pageViewGames->addEventListener(PageView::ccPageViewCallback(CC_CALLBACK_2(GameLists::pageViewMoveCallBack, this)));
}

Button* GameLists::createGameItem(ComNameInfo* game) {
    char str[128];
    sprintf(str, "platform/gameList/gameItemRes/%d.png", game->uNameID);

    Button* pButtonItem;

    if (FileUtils::getInstance()->isFileExist(str)) {
        pButtonItem = Button::create(str);
        pButtonItem->setUserData(game);
        pButtonItem->addTouchEventListener(CC_CALLBACK_2(GameLists::enterRoomEventCallBack, this));
    }
    return pButtonItem;
}

void GameLists::pageViewMoveCallBack(Ref* pSender, PageView::EventType type) {
    if (_currentPageIdx == _pageViewGames->getCurrentPageIndex()) return;
    _currentPageIdx = _pageViewGames->getCurrentPageIndex();

    auto iter = _pagesPrompt.begin();
    for (int i = 0; iter != _pagesPrompt.end(); iter++, i++) {
        auto page = (Sprite*)*iter;
        page->setColor(Color3B(150, 150, 150));
        page->setScale(0.8f);
        if (i == _currentPageIdx) {
            page->setScale(1.0f);
            page->setColor(Color3B(255, 255, 255));
        }
    }

    _prevArrowPrompt->setVisible(false);
    _nextArrowPrompt->setVisible(false);
}

void GameLists::enterRoomEventCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (_isTouch == false) return;

    _currentSelectedGame = dynamic_cast<Button*>(pSender);

    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::BEGAN:
        _currentSelectedGame->runAction(ScaleTo::create(0.2f, 1.1f));
        break;
    case cocos2d::ui::Widget::TouchEventType::MOVED:
        break;
    case cocos2d::ui::Widget::TouchEventType::ENDED: {
        _isTouch = false;

        HNAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        _currentSelectedGame->runAction(ScaleTo::create(0.2f, 1.0f));

        /**
         *	此处可以获得选择的游戏参数，将其值传递给捕鱼游戏
         *	by HBC
         */
        //Fish_Game_Version_ID = _currentSelectedGame->getTag();
        UserDefault::getInstance()->setIntegerForKey("Enter_Game_ID", _currentSelectedGame->getTag());

        // 参数校验
        //CCAssert(0 != Fish_Game_Version_ID, "gameID is not value!");
        if (0 == _currentSelectedGame->getTag()) {
            _isTouch = true;
            return;
        }

        GameCreator()->setCurrentGame(_currentSelectedGame->getTag());

        if (nullptr != onRunActionCallBack) {
            onRunActionCallBack();
        }

        this->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([&]() {
            if (nullptr != onEnterGameCallBack) {
                onEnterGameCallBack();
                //UserDefault::getInstance()->setIntegerForKey("PLATFORM_GAMETYPE_RETURN_", 1);
                /*	UserDefault::getInstance()->setDoubleForKey("curSelectedGameData", 0);
                	auto precent = -(_scrollView->getInnerContainer()->getPosition().x / (_scrollView->getInnerContainer()->getContentSize().width - _scrollView->getContentSize().width)) * 100;
                	UserDefault::getInstance()->setDoubleForKey("curSelectedGameData", precent);*/
            }
            _isTouch = true;
            //this->removeFromParent();
        }), nullptr));
    }
    break;
    case cocos2d::ui::Widget::TouchEventType::CANCELED:
        _currentSelectedGame->runAction(ScaleTo::create(0.2f, 1.0f));
        break;
    default:
        break;
    }
}

void GameLists::updateGamePeopleCount(UINT Id, UINT count) {
    //Vector<Layout*> pages = _pageViewGames->getPages();

    //char buffer[64];
    //for (auto iter = pages.begin(); iter != pages.end(); ++iter) {
    //    Vector<Node*> childs = (*iter)->getChildren();
    //    for (auto iter1 = childs.begin(); iter1 != childs.end(); ++iter1) {
    //        auto gameButton = dynamic_cast<Button*>(*iter1);
    //        auto TextBMFont_PeopleCount = (TextBMFont*)Helper::seekWidgetByName(gameButton, "BitmapFontLabel_PeopleCount");

    //        if (nullptr != gameButton) {
    //            ComNameInfo* names = static_cast<ComNameInfo*>(gameButton->getUserData());

    //            if (names->uNameID == Id) {
    //                if (nullptr != TextBMFont_PeopleCount) {
    //                    //sprintf(buffer, "%u人", count);
    //                    //TextBMFont_PeopleCount->setString(GBKToUtf8(buffer));
    //                }
    //            }
    //        }
    //    }
    //}
}

/**
 *	获取游戏ID对外仅提供获取方法，不提供修改方法，其修改仅允许该类内部实现
 *	by HBC
 */
UINT GameLists::getFishGameVersionID() {
    return Fish_Game_Version_ID;
}
