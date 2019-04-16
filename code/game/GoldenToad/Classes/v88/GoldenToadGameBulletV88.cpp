#include "GoldenToadGameBulletV88.h"
#include "GoldenToadGamePlayerV88.h"
#include "GoldenToadGameMathAide.h"
#include "GoldenToadGameAudio.h"

namespace GoldenToad {
GameBulletV88* GameBulletV88::create(const std::string & file, GamePlayerV88* player, GameFishManagerV88* fishManager, BulletKind bulletKind, INT bulletId, INT bulletMulriple, FLOAT net_radius, FLOAT speed) {
    if (file.empty()) {
        return nullptr;
    }
    GameBulletV88* pRet = new GameBulletV88(player, fishManager, bulletKind, bulletId, bulletMulriple, net_radius, speed);
    if (nullptr != pRet && pRet->initWithSpriteFrameName(file)) { //
        
        pRet->setColor(Color3B(255,255,255));
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

GameBulletV88::GameBulletV88(GamePlayerV88* player, GameFishManagerV88* fishManager, BulletKind bulletKind, INT bulletId, INT bulletMulriple, FLOAT net_radius, FLOAT speed):
    _player(player), _fishManager (fishManager), _bullet_kind( bulletKind),
    _bullet_id(bulletId), _bullet_mulriple(bulletMulriple), _netRadius(net_radius),
    _speed(speed), onHitFishCallBack (nullptr), _isDoUpdateFUNC (true) {
    _lockFishId = INVALID_FISHID;
}

GameBulletV88::~GameBulletV88() {
}
void GameBulletV88::setBulletScale(float scales)
{
    this->setScale(scales);
}
void GameBulletV88::runStartAction(float radians) {
    _radians = radians;
    _WinSize = Director::getInstance()->getWinSize();

    scheduleUpdate();
}

void GameBulletV88::collisionDetection() {
    auto rect = this->getBoundingBox();
    auto currentSize = Size(rect.size.width / 2, rect.size.height / 2);
    rect.size = currentSize;

    for (auto ite = _fishManager->_fishList.begin(); ite != _fishManager->_fishList.end(); ite++) {
        auto fish = (Fish*)*ite;
        if (fish == nullptr) {
            continue;
        }
        if (!fish->isVisible() || !fish->getCanTouch()) continue;

        Fish* touchFish = nullptr;
        //如果有锁定的鱼，必须是鱼id跟锁定id相同才检测碰撞
        if (_lockFishId != INVALID_FISHID) {
            if (fish->getFishID() == _lockFishId) {
                touchFish = fish;
            }
        } else {
            touchFish = fish;
        }

        if (!touchFish) continue;

        Rect rt = touchFish->getFishRect();
        if (rt.intersectsRect(rect)) { //子弹和鱼碰撞
            //子弹停止前进
            unscheduleUpdate();
            //子弹爆炸
            bulletExplosion();
//                int anskind = (int)_bullet_kind;
            if (onHitFishCallBack) {
                auto meteor = ParticleSystemQuad::create("v66/goldenToad/particle/dazhongyu.plist");
                meteor->setPosition(this->getPosition());
                meteor->setAutoRemoveOnFinish(true);
                this->getParent()->addChild(meteor, 100);

                //向上一级报告 子弹和鱼 产生碰撞
                onHitFishCallBack(touchFish->getFishID(), _bullet_id, _bullet_kind, _bullet_mulriple);

                //播放鱼被击中的效果
                touchFish->showHitEffect();
            }
            break;
        }
    }
}

/**
 *	子弹碰到障碍物之后的处理
 *	by HBC
 */
void GameBulletV88::bulletExplosion() {

    this->initWithSpriteFrameName("net2_01.png");

    auto cache = SpriteFrameCache::getInstance();
    Animation *animation = Animation::create();
    animation->setDelayPerUnit(0.08f);
    animation->setLoops(1);

    int tmp = (_bullet_kind == BULLET_KIND_5_ION) ? 3 : 2;
    for (BYTE i = 1; i <= 8; i++)		{
        std::string name = StringUtils::format("net%d_%02d.png", tmp, i);
        animation->addSpriteFrame(cache->getSpriteFrameByName(name));
    }
    //this->setScale(0.7f);
    auto animate = Animate::create(animation);
    this->runAction(Sequence::create(animate, CallFunc::create([this]() {

        for (auto ite = _player->_bulletList.begin(); ite != _player->_bulletList.end(); ite++) {
            if (*ite == this) {
                _player->_bulletList.erase(ite);
                break;
            }
        }

        removeFromParent();
    }), nullptr));
}

void GameBulletV88::update(float dt) {
    if (!_isDoUpdateFUNC && _lockFishId == INVALID_FISHID) {
        _isDoUpdateFUNC = true;
        return;
    }
    _isDoUpdateFUNC = false;

    //检测与鱼边界碰撞
    collisionDetection();
    auto currentPos = this->getPosition();
    //检测与屏幕左右边界碰撞
    if (currentPos.x <= 0 || currentPos.x >= _WinSize.width) {
        setRotation(-getRotation());
    }
    //检测与屏幕上下边界碰撞
    if (currentPos.y <= 0 || currentPos.y >= _WinSize.height) {
        setRotation(180 - getRotation());
    }

    if (_lockFishId == INVALID_FISHID) {
        //运动弧度
        _radians = CC_DEGREES_TO_RADIANS(90 - getRotation());
    } else {
        //锁定状态子弹不再按照出枪管时的路线飞行，而是根据锁定的鱼的位置跟踪移动
        auto fish = dynamic_cast<Fish*>(_fishManager->getFishById(_lockFishId));
        if (nullptr != fish) {
            Vec2 fishPos = fish->getPosition();
            //目标鱼和子弹直接的距离
//                float range = sqrt(pow(abs(fishPos.x - currentPos.x), 2) + pow(abs(fishPos.y - currentPos.y), 2));
            //目标鱼和子弹的弧度
            _radians = atan2(fishPos.y - currentPos.y, fishPos.x - currentPos.x);
        }
    }

    //利用已知的弧度和位移速度获得下一个坐标
    currentPos.x = getPositionX() + cosf(_radians) * _speed;
    currentPos.y = getPositionY() + sinf(_radians) * _speed;

    setPosition(currentPos);
}
}
