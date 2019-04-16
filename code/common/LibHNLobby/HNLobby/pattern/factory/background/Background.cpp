#include "Background.h"
#include "cocos2d.h"
USING_NS_CC;

bool Background::init(std::string filename) {
    if (!Node::init()) {
        return false;
    }

    auto sprite = Sprite::create(filename);
    sprite->setTag(1);
    addChild(sprite);
    setContentSize(sprite->getContentSize());

    return true;
}

Background* Background::create(std::string filename) {
    Background* pRet = new (std::nothrow)Background();
    if (pRet && pRet->init(filename)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

void Background::setTexture(std::string filename){
    Sprite* sprite = (Sprite*)this->getChildByTag(1);
    if(sprite){
        sprite->setTexture(filename);
    }
}
