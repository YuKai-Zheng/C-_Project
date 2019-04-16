#include "NewYearGameTableUI.h"
#include "NewYearGameTableLogic.h"
//#include "HNUIExport.h"
//#include "HNLobbyExport.h"
#include "HNLobby/FontConfig.h"
#include "HNLobby/state/PlatformState.h"
#include "HNLobby/pattern/StateManager.h"
#include "HNLobby/command/ShowGamePrompt.h"
#include "HNLobby/pattern/CommandManager.h"
#include "NSAnimationTool.h"

namespace NewYearActivity {

#define WINSIZE Director::getInstance()->getWinSize()

//csb
static const char * ROOT_VIEW_CSB = "csb/platform/newYear/activityView.csb";
static const char * ROOT_ACTIVITY_ONE_CSB = "csb/platform/newYear/activityOne/activity_one.csb";
static const char * ROOT_ACTIVITY_TWO_CSB = "csb/platform/newYear/activityTwo/activiy_Two.csb";
static const char * ROOT_ACTIVITY_THREE_CSB = "csb/platform/newYear/activityThree/activity_three.csb";

static const char * CLOSE_MENU_N = "platform/yearActivity/06.png";

static const char* AUDIO_BG_MUSIC_PATH = "music/NewYearActivity/NewYearBG.mp3";

HNGameUIBase* GameTableUI::create(BYTE bDeskIndex, bool bAutoCreate) {
    GameTableUI* tableUI = new GameTableUI();
    if (tableUI && tableUI->init(bDeskIndex, bAutoCreate)) {
        tableUI->autorelease();
    } else {
        delete tableUI;
        tableUI = nullptr;
    }
    return tableUI;
}

GameTableUI::GameTableUI()
    : m_Notice(nullptr) {
//    TextureCache::getInstance()->addImage(NN_GOLD_IMAGE);
}

GameTableUI::~GameTableUI() {
    CC_SAFE_DELETE(_GameTableLogic);
}

bool GameTableUI::init(BYTE bDeskIndex, bool bAutoCreate) {
    if (!HNGameUIBase::init()) {
        return false;
    }
    _GameTableLogic = new GameTableLogic(this, bDeskIndex, bAutoCreate, PLAYER_COUNT);

    setBackGroundImage("platform/hallBg.png");
    initUI();
    //_GameTableLogic->enterGame();

    return true;
}

void GameTableUI::onEnter() {
    HNGameUIBase::onEnter();
    HNAudioEngine::getInstance()->playBackgroundMusic(AUDIO_BG_MUSIC_PATH, true);
}

void GameTableUI::onExit() {
    HNGameUIBase::onExit();
}

void GameTableUI::onExitTransitionDidStart() {
    HNLayer::onExitTransitionDidStart();
    HNAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameTableUI::initUI() {
    auto root = CSLoader::createNode(ROOT_VIEW_CSB);
    root->setPosition(WINSIZE / 2.0f);
    this->addChild(root);

    _view.Panel = static_cast<Layout*>(root->getChildByName("Panel"));

    _view.Menu_node = root->getChildByName("btn_node");
    _view.Menu_node->setLocalZOrder(1000);
    int i = 0;
    for (auto node : _view.Menu_node->getChildren()) {
        auto nor = static_cast<Sprite*>(node->getChildByName("normal"));
        auto sel = static_cast<Sprite*>(node->getChildByName("select"));
        auto btn = static_cast<Button*>(node->getChildByName("btn"));
        btn->setTag(i++);
        btn->addTouchEventListener(CC_CALLBACK_2(GameTableUI::menuCallBack, this));
        sel->setVisible(false);
        nor->setVisible(true);
        _vecItem.push_back(btn);
        _vecNormalSpr.push_back(nor);
        _vecSelectSpr.push_back(sel);
    }

    if (_vecSelectSpr.at(0) && _vecNormalSpr.at(0)) {
        _vecSelectSpr.at(0)->setVisible(true);
        _vecNormalSpr.at(0)->setVisible(false);
    }

    // 关闭按钮
    auto closeBtn = Button::create(CLOSE_MENU_N, CLOSE_MENU_N);
    closeBtn->setShowEffect(false);
    closeBtn->setPosition(Vec2(WINSIZE.width * 0.95f, WINSIZE.height * 0.92f));
    closeBtn->setPosition(Vec2(WINSIZE.width - closeBtn->getContentSize().width*0.55, WINSIZE.height - closeBtn->getContentSize().height*0.55));
    closeBtn->addTouchEventListener(CC_CALLBACK_2(GameTableUI::closeEvent, this));
    addChild(closeBtn);

    m_viewParent = Node::create();
    m_viewParent->setPosition(WINSIZE / 2.0f);
    addChild(m_viewParent);


    _GameTableLogic->sendGameInfo();
    createActiviThree(m_viewParent);
}

void GameTableUI::onGameUpdateSignInfo(tag_s2c_SignInfo* msg) {
    LoadingLayer::removeLoading(this);

    m_iSignDay = msg->nSignDay;
    for (int i = 0; i < _vecStar.size(); i++) {
        if (i < msg->nSignDay) {
            _vecStar.at(i)->getChildByName("normal")->setVisible(false);
            _vecStar.at(i)->getChildByName("select")->setVisible(true);
        } else {
            _vecStar.at(i)->getChildByName("normal")->setVisible(true);
            _vecStar.at(i)->getChildByName("select")->setVisible(false);
        }
    }
    _activityOne.Button_qianDao->setTouchEnabled(!msg->nIsTodaySign);
    _activityOne.Button_qianDao->setBright(!msg->nIsTodaySign);
}

void GameTableUI::onGameUpdateSignResult(tag_s2c_SignResult* msg) {
    LoadingLayer::removeLoading(this);

    ShowGamePrompt* prompt = (ShowGamePrompt*)CommandManager::getInstance()->get(SHOW_GAME_PROMPT);

    if (msg->nSignResult == 0) {
        //失败
        prompt->setText(GetText("SIGN_FAIL"));
        prompt->execute();
        //
        if (_activityOne.Button_qianDao) {
            //特效表现
            _activityOne.Button_qianDao->setTouchEnabled(false);
            _activityOne.Button_qianDao->setBright(false);
        }
    } else if (msg->nSignResult == 1) {
        //本地保存的
        m_iSignDay++;

        bool bRunAnimate = false;
        for (int i = 0; i < SPECIAL_DAY_COUNT; i++) {
            if (g_BoxSpecialSignNum[i] == m_iSignDay) {
                showBoxAnimation();
                bRunAnimate = true;
                break;
            }
        }
        if (!bRunAnimate) {
            int index = m_iSignDay - 1;
            showStarAnimation(index);
        }

        //
        if (_activityOne.Button_qianDao) {
            //特效表现
            _activityOne.Button_qianDao->setTouchEnabled(false);
            _activityOne.Button_qianDao->setBright(false);
        }
        LoadingLayer::createLoading(this, gFontConfig_22, GetText("请稍后"), "platform/loading.png");
        _GameTableLogic->sendActivityType(Sign_Activity);


        Text* money = Text::create("+20000", "platform/fonts/simhei.ttf", 32);
        if (money) {
            money->setTextColor(Color4B::YELLOW);
            money->setPosition(_activityOne.Button_qianDao ? _activityOne.Button_qianDao->getPosition() : Vec2(WIN_SIZE.width * 0.35, WIN_SIZE.height * 0.48));
            addChild(money);

            Spawn* spawn = Spawn::create(MoveTo::create(0.8f, money->getPosition() + Vec2(0, 110)), FadeIn::create(0.5f), nullptr);
            Sequence* seq = Sequence::create(spawn, DelayTime::create(1.5f), RemoveSelf::create(), nullptr);
            money->runAction(seq);
        }


    } else if (msg->nSignResult == 2) {
        //其他号签到
        prompt->setText(GetText("SIGN_DONE"));
        prompt->execute();
        //
        if (_activityOne.Button_qianDao) {
            //特效表现
            _activityOne.Button_qianDao->setTouchEnabled(false);
            _activityOne.Button_qianDao->setBright(false);
        }
    } else if (msg->nSignResult == 3) {
        //当天签到
        prompt->setText(GetText("SIGN_DONE"));
        prompt->execute();
        //
        if (_activityOne.Button_qianDao) {
            //特效表现
            _activityOne.Button_qianDao->setTouchEnabled(false);
            _activityOne.Button_qianDao->setBright(false);
        }
    }
}

void GameTableUI::onGameUpdateDriveNianShouInfo(tag_s2c_DriveNianShouInfo* msg) {
    LoadingLayer::removeLoading(this);

    m_vFireCount.clear();
    m_vFireCount.push_back(msg->nMinNum);
    m_vFireCount.push_back(msg->nMidNum);
    m_vFireCount.push_back(msg->nMaxNum);

    _activityTwo.Button_small->setTouchEnabled(msg->nMinNum > 0);
    _activityTwo.Button_small->setBright(msg->nMinNum > 0);

    if (msg->nMinNum >= 0) {
        if (_activityTwo.Button_small->getChildByName("min")) {
            _activityTwo.Button_small->removeAllChildrenWithCleanup(true);
        }

        Label* label = Label::create();
        char str[64] = { 0 };
        sprintf(str, "%d", msg->nMinNum);
        label->setString(str);
        label->setPosition(_activityTwo.Button_small->getContentSize().width / 2, -13);
        label->setSystemFontSize(20);
        label->setName("min");
        _activityTwo.Button_small->addChild(label);
    }

    _activityTwo.Button_mid->setTouchEnabled(msg->nMidNum > 0);
    _activityTwo.Button_mid->setBright(msg->nMidNum > 0);

    if (msg->nMidNum >= 0) {
        if (_activityTwo.Button_mid->getChildByName("mid")) {
            _activityTwo.Button_mid->removeAllChildrenWithCleanup(true);
        }

        Label* label = Label::create();
        char str[64] = { 0 };
        sprintf(str, "%d", msg->nMidNum);
        label->setString(str);
        label->setPosition(_activityTwo.Button_mid->getContentSize().width / 2, -13);
        label->setSystemFontSize(20);
        label->setName("mid");
        _activityTwo.Button_mid->addChild(label);
    }

    _activityTwo.Button_big->setTouchEnabled(msg->nMaxNum > 0);
    _activityTwo.Button_big->setBright(msg->nMaxNum > 0);

    if (msg->nMaxNum >= 0) {
        if (_activityTwo.Button_big->getChildByName("max")) {
            _activityTwo.Button_big->removeAllChildrenWithCleanup(true);
        }

        Label* label = Label::create();
        char str[64] = { 0 };
        sprintf(str, "%d", msg->nMaxNum);
        label->setString(str);
        label->setPosition(_activityTwo.Button_big->getContentSize().width / 2, -13);
        label->setSystemFontSize(20);
        label->setName("max");
        _activityTwo.Button_big->addChild(label);
    }
}

//赶年兽结果
void GameTableUI::onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) {
    //过滤只有自己看的到
    if (nUserID != _GameTableLogic->getUserId(_GameTableLogic->getMySeatNo())) {
        return;
    }

    //有鞭炮赶年兽奖励
    if (i64RewardMoney > 0) {
        //年兽爆炸
        auto daijiNode = NSAnimationTool::createNSGanNodeAni("hit", false);
        if (daijiNode) {
            _activityTwo.animation_node->removeAllChildrenWithCleanup(true);
            _activityTwo.animation_node->addChild(daijiNode);
            //文本显示
            Label* label = Label::create();
            label->setSystemFontSize(30);
            label->setColor(Color3B(255, 252, 0));
            label->setPosition(Vec2(160, 0));
            _activityTwo.animation_node->addChild(label);

            char str[64] = { 0 };
            sprintf(str, "+%lld", i64RewardMoney);
            label->setString(str);

            CallFunc* func = CallFunc::create([&]() {
                showNianShouAnimation();
                _GameTableLogic->sendActivityType(DriveNianShou_Activity);
            });

            label->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 50)), DelayTime::create(0.7f), func, RemoveSelf::create(), nullptr));
        }

        //刷新下数据
        _activityTwo.Button_small->setTouchEnabled(false);
        _activityTwo.Button_mid->setTouchEnabled(false);
        _activityTwo.Button_big->setTouchEnabled(false);
    } else { //无鞭炮

    }
}

void GameTableUI::menuCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;

    auto curSelectBtn = static_cast<Button*>(pSender);

    for (auto node : _view.Menu_node->getChildren()) {
        auto nor = static_cast<Sprite*>(node->getChildByName("normal"));
        auto sel = static_cast<Sprite*>(node->getChildByName("select"));
        auto btn = static_cast<Button*>(node->getChildByName("btn"));

        if (btn != curSelectBtn) {
            sel->setVisible(false);
            nor->setVisible(true);
        } else {
            sel->setVisible(true);
            nor->setVisible(false);
        }
    }

    loadActivity(curSelectBtn->getTag());
}


void GameTableUI::qianDaoCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;

    LoadingLayer::createLoading(this, gFontConfig_22, GetText("请稍后"), "platform/loading.png");
    _GameTableLogic->sendSign();
}

void GameTableUI::tntMenuCallBack(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;

    int nTag = ((Button*)pSender)->getTag();

    HNAudioEngine::getInstance()->playEffect(nTag == 1 ? SMALL_TNT_EFFECT_AUDIO : nTag == 2 ? MID_TNT_EFFECT_AUDIO : nTag == 3 ? BIG_TNT_EFFECT_AUDIO : SMALL_TNT_EFFECT_AUDIO);

    if (nTag > m_vFireCount.size() || nTag < 1) {
        return;
    }

    if (m_vFireCount.at(nTag - 1) <= 0) {
        return;
    }

    _GameTableLogic->sendDriveNianShou(_GameTableLogic->getUserId(_GameTableLogic->getMySeatNo()), nTag);
}

//关闭回调
void GameTableUI::closeEvent(Ref* pSender, Widget::TouchEventType type) {
    if (type != Widget::TouchEventType::ENDED)
        return;
    //退出当前房间
    RoomLogic()->closeRoom();
    PlatformState* state = PlatformState::create();
    state->setType(GAMETYPE);
    StateManager::getInstance()->setState(state);
}


void GameTableUI::loadActivity(BYTE idx) {
    m_viewParent->removeAllChildren();
    switch (idx) {
    case 0: {
        createActiviThree(m_viewParent);
        break;
    }
    case 1: {
        createActiviOne(m_viewParent);
        break;
    }
    case 2: {
        createActiviTwo(m_viewParent);
        break;
    }
    default:
        break;
    }
}

void GameTableUI::createActiviOne(Node * parent) {
    auto root = CSLoader::createNode(ROOT_ACTIVITY_ONE_CSB);
    parent->addChild(root);
    root->setName("Sign_Panel");

    _activityOne.Panel = static_cast<Layout*>(root->getChildByName("Panel"));
    _activityOne.Panel->setSwallowTouches(false);

    auto starlist = static_cast<Layout*>(_activityOne.Panel->getChildByName("starArrayNode"));

    _vecStar.clear();
    for (auto star : starlist->getChildren()) {
        _vecStar.push_back(star);
        static_cast<Sprite*>(star->getChildByName("normal"))->setVisible(true);
        static_cast<Sprite*>(star->getChildByName("select"))->setVisible(false);
    }

    _activityOne.Button_qianDao = static_cast<Button*>(_activityOne.Panel->getChildByName("Button_qianDao"));
    _activityOne.Button_qianDao->addTouchEventListener(CC_CALLBACK_2(GameTableUI::qianDaoCallBack, this));

    LoadingLayer::createLoading(this, gFontConfig_22, GetText("请稍后"), "platform/loading.png");
    _GameTableLogic->sendActivityType(Sign_Activity);
}

void GameTableUI::createActiviTwo(Node * parent) {
    auto root = CSLoader::createNode(ROOT_ACTIVITY_TWO_CSB);
    parent->addChild(root);

    _activityTwo.Panel = static_cast<Layout*>(root->getChildByName("Panel"));

    _activityTwo.Button_small = static_cast<Button*>(root->getChildByName("0"));
    _activityTwo.Button_mid = static_cast<Button*>(root->getChildByName("1"));
    _activityTwo.Button_big = static_cast<Button*>(root->getChildByName("2"));

    _activityTwo.animation_node = static_cast<Node*>(root->getChildByName("nianShou_node"));

    if (_activityTwo.animation_node) {
        showNianShouAnimation();
    }

    _activityTwo.Button_small->addTouchEventListener(CC_CALLBACK_2(GameTableUI::tntMenuCallBack, this));
    _activityTwo.Button_small->setTag(1);
    _activityTwo.Button_mid->addTouchEventListener(CC_CALLBACK_2(GameTableUI::tntMenuCallBack, this));
    _activityTwo.Button_mid->setTag(2);
    _activityTwo.Button_big->addTouchEventListener(CC_CALLBACK_2(GameTableUI::tntMenuCallBack, this));
    _activityTwo.Button_big->setTag(3);

    LoadingLayer::createLoading(this, gFontConfig_22, GetText("请稍后"), "platform/loading.png");
    _GameTableLogic->sendActivityType(DriveNianShou_Activity);
}

void GameTableUI::createActiviThree(Node * parent) {
    auto root = CSLoader::createNode(ROOT_ACTIVITY_THREE_CSB);
    root->setPositionY(-20);
    //root->setPosition(WINSIZE / 2.0f);
    parent->addChild(root);
}

void GameTableUI::showStarAnimation(int index) {
    auto starNode = NSAnimationTool::createStarNodeAni("sign", false);
    if (starNode) {
        _vecStar.at(index)->addChild(starNode);
    }
}

void GameTableUI::showBoxAnimation() {
    auto boxNode = NSAnimationTool::createBoxNodeAni("baoxiang", false);
    if (boxNode) {
        Vec2 pos = _activityOne.Panel->getContentSize() / 2;
        boxNode->setPosition(pos);
        _activityOne.Panel->addChild(boxNode);
    }
}

void GameTableUI::showNianShouAnimation() {
    auto daijiNode = NSAnimationTool::createNSDaijiNodeAni("daiji", true);
    if (daijiNode) {
        _activityTwo.animation_node->addChild(daijiNode);
    }
}

    void GameTableUI::onHandleNewMessage(int iType, const std::string &message, int iCount)
    {
        //全服公告
        if (iType == 0)
        {
            showNotice(message, 100, iType);
        }
        else if (iType == -11)//银商
        {
            showNotice(message, iCount, iType);
        }
    }

void GameTableUI::showNotice(const std::string& message, int nCount, int nType, bool bVisible) {
    if (bVisible) {
        if (!m_Notice) {
            m_Notice = GameNotice::create();
            m_Notice->setLocalZOrder(10000);
            m_Notice->setName("GameNotice");
        }
        m_Notice->openNotic(this, 10000, message, nCount, nType);
    }
}

}



