#include "HNScene.h"
#include "HNNetExport.h"
#include "UI/TextureLayer.h"

namespace HN {

HNScene::HNScene(void) : isMoved(false), downCount(0), upCount(0) {}

HNScene::~HNScene(void) {
    log("");
}

HNScene *HNScene::create() {
    HNScene *scene = new HNScene();
    if (scene && scene->init()) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

void HNScene::onEnter() {
    Scene::onEnter();
    schedule(schedule_selector(HNScene::releaseLastScene),1.0f);
}

void HNScene::onExit() {
    Scene::onExit();
}

void HNScene::onExitTransitionDidStart() {
    Scene::onExitTransitionDidStart();
    AnimationCache::getInstance()->destroyInstance();
    SpriteFrameCache::getInstance()->removeSpriteFrames();
    Director::getInstance()->getTextureCache()->removeAllTextures();
    unscheduleReleaseScene();
}

void HNScene::onEnterTransitionDidFinish() {
    Scene::onEnterTransitionDidFinish();
}

void HNScene::unscheduleReleaseScene() {
    this->unschedule(schedule_selector(HNScene::releaseLastScene));
}

void HNScene::releaseLastScene(float dt) {
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
    //showCachedTextureInfo();
}

bool HNScene::init() {
    if (!Scene::init()) {
        return false;
    }

    if (TEXTURE_ONOFF) {
        EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(false);
        listener->onTouchBegan = CC_CALLBACK_2(HNScene::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(HNScene::onTouchEnded, this);
        listener->onTouchMoved = CC_CALLBACK_2(HNScene::onTouchMoved, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
    return true;
}

bool HNScene::onTouchBegan(Touch *touch, Event *unused_event) {
    return true;
}

void HNScene::onTouchMoved(Touch *touch, Event *unused_event) {
    isMoved = true;
}

void HNScene::onTouchEnded(Touch *touch, Event *unused_event) {
    //if (!isMoved) {
    int height = Director::getInstance()->getWinSize().height;
    Vec2 endPosition = touch->getLocation();
    if (endPosition.y < height / 3) { //下区域
        downCount++;
    } else if (endPosition.y > height * 2 / 3 &&
               endPosition.y < height) { //上区域
        upCount++;
    }

    if (downCount >= 2 && upCount >= 2) {
        addChild(TextureLayer::create());
        downCount = 0;
        upCount = 0;
    }
    //}
    //isMoved = false;
}

void HNScene::showCachedTextureInfo() {
    std::string cachedTextureInfo = Director::getInstance()->getTextureCache()->getCachedTextureInfo();
    HNLog::logInfo("%s","description:\n");
    //
    std::vector<string> infos;
    string delim = "\"";
    split(cachedTextureInfo, delim, infos);
    vector<string>::iterator iter;
    for (iter = infos.begin(); iter != infos.end(); iter++) {
        string tmp = *iter;
        if (/*tmp.find("rc=") != -1 || */tmp.compare("") != -1) {
            HNLog::logInfo("%s",tmp.c_str());
        }
    }
}

//注意：当字符串为空时，也会返回一个空字符串
void HNScene::split(std::string &s, std::string &delim, std::vector<std::string> &ret) {
    size_t last = 0;
    size_t index = s.find_first_of(delim, last);
    while (index != std::string::npos) {
        ret.push_back(s.substr(last, index - last));
        last = index + 1;
        index = s.find_first_of(delim, last);
    }
    if (index - last > 0) {
        ret.push_back(s.substr(last, index - last));
    }
}

}
