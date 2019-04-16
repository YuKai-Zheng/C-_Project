
#include"HNLobby/GameDeskUserInfoLayer.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "HNLobby/globel.h"

static const char* CSB = "csb/platform/common/userInfoPopup.csb";
//static const char* BACKGROUND = "platform/common/playerInfoKuang.png";
//static const char* HEAD_KUANG = "platform/common/playerInfoHead.png";

#define WINSIZE Director::getInstance()->getWinSize()
DeskUserInfoLayer::DeskUserInfoLayer() {

}

DeskUserInfoLayer::~DeskUserInfoLayer() {

}

void DeskUserInfoLayer::closePrompt() {
//	HNAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    this->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]() {
        this->removeFromParent();
    }), nullptr));
}

bool DeskUserInfoLayer::init() {
    if (!HNNode::init()) {
        return false;
    }

    //auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 0));
    //addChild(colorLayer);

    //touchListener = EventListenerTouchOneByOne::create();
    //touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
    //	closePrompt();
    //	return true;
    //};


    //touchListener->setSwallowTouches(true);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);

    _root = CSLoader::createNode(CSB);
    addChild(_root);
    _userInfo.name = static_cast<Text*>(_root->getChildByName("name"));
    _userInfo.id = static_cast<Text*>(_root->getChildByName("id"));
    _userInfo.money = static_cast<Text*>(_root->getChildByName("money"));
    _userInfo.win = static_cast<Text*>(_root->getChildByName("win"));
    _userInfo.lose = static_cast<Text*>(_root->getChildByName("lose"));
    _userInfo.head = static_cast<Sprite*>(_root->getChildByName("Sprite_1"));
    _userInfo.bg = static_cast<Sprite*>(_root->getChildByName("playerInfoKuang_1"));
    _userInfo.head->setScale(0.75);
    return true;
}

void DeskUserInfoLayer::openLayer(Node* parent, Vec2 point, int zorld) {
    this->setPosition(point);
    parent->addChild(this, zorld);
}

void DeskUserInfoLayer::closelayer() {
    this->removeFromParent();
}

void DeskUserInfoLayer::updateUserData(INT deskIndex,INT bDeskStation) {
    UserInfoStruct* user = UserInfoModule()->getInstance()->findUser(deskIndex, bDeskStation);
    _userInfo.name->setString(user->nickName);
    _userInfo.money->setString(to_string(user->i64Money));
    _userInfo.win->setString(to_string(user->uWinCount));
    _userInfo.lose->setString(to_string(user->uLostCount));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _userInfo.id->setString(ForceGBKToUtf8(user->nickName));
#else
    _userInfo.id->setString(GBKToUtf8(user->nickName));
#endif
    char str[50] = { 0 };
    sprintf(str, "common/head/%d.png", user->bLogoID);
    _userInfo.head->setTexture(str);
}
