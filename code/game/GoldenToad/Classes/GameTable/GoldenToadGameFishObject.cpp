#include "GoldenToadGameFishObject.h"
#include "GoldenToadGameMathAide.h"
//#include "HNLobbyExport.h"
#include "UI/HNShake.h"
#include "HNLobby/FontConfig.h"
#include "GoldenToadVersion.h"
#include "GoldenToadEnums.h"

using namespace Version;

namespace GoldenToad {
    
    static const char*	DASANYUAN			=	"csb/Games/GoldenToad/dasanyuan.csb";
    static const char*	DASIXI				=	"csb/Games/GoldenToad/dasixi.csb";
    static const char*	ZXDASANYUAN			=	"csb/Games/GoldenToad/ZXdasanyuan.csb";
    static const char*	ZXDASIXI			=	"csb/Games/GoldenToad/ZXdasixi.csb";
    static const char*  DI_CSB              =   "v66/goldenToad/animation/CornerTray/CornerTray.csb";
    static const int	Fish_Hit_Action_Tag = 99;

Fish::Fish(int fishId, FishKind fishKind, FishKind fishKind_ex[5])
    : _fishID (fishId), _fishKind (fishKind), onMoveOutCallBack (nullptr)
    , _isTouch(true), _isPause(false), _fishSize (Size::ZERO), _isLock (false)
    , _fishMoveAction (nullptr), _fishSpeed (0), _fishRect(0, 0, 0, 0)
    , _oldPosition (Vec2(0, 0)), _desPosition (Vec2(0, 0)), _isSwitch (false)
    , _gameBG (nullptr), _isUseFishTrace (true),_stop_index(0), _stop_count(0), _current_stop_count(0)
    , onUnLockCallBack(nullptr), _fish_multiple(1)/*, _fish_red_shadow(nullptr)*/, onUpdateFishScoreCallBack(nullptr), _isInsert(false) {
    _winSize = Director::getInstance()->getWinSize();
    memcpy(_fishKind_ex, fishKind_ex, sizeof(fishKind_ex));
}

Fish::~Fish() {
}

Fish* Fish::create(int fishId, FishKind fishKind, FishKind fishKind_ex[5]) {
    Fish* pRet = new Fish(fishId, fishKind, fishKind_ex);
    if (pRet && pRet->init()) {
        pRet->autorelease();
        //此处需要retain一次，因为重复利用的时候有把鱼移除父节点，会导致鱼被释
        pRet->retain();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool Fish::init() {
    if (!HNSprite::init()) return false;
    //设置子节点跟着父节点的透明度变化，如果不设置会导致鱼不能隐藏
    this->setCascadeOpacityEnabled(true);
    //设置帧动画图片信息
    switch (GoldenToadVersion::getGameVersionName()) {
        case Enums::GAME_VERSION_TOAD_FISH:
            onRender();
            break;
        case Enums::GAME_VERSION_LIKUI_FISH:
            onRenderLiKui();
            break;
        case Enums::GAME_VERSION_MONKEY_FISH:
            onRenderMonkey();
            break;
        default:
            onRender();
            break;
    }
    
    

    initFishRedShadowArray();
    // for testing by HBC
    //		_fishKind = FISH_KIND_3;

    switch (_fishKind) {
    case  FISH_KIND_27: //三角大三元
        createDaSanYuanFish(_fishKind_ex[0]);
        break;
    case  FISH_KIND_28: //直线大三元
        createZXDaSanYuanFish(_fishKind_ex[0]);
        break;
    case  FISH_KIND_29: //四角大四喜
        createDaSixiFish(_fishKind_ex[0]);
        break;
    case  FISH_KIND_30: //直线大四喜
        createZXDaSiXiFish(_fishKind_ex[0]);
        break;
    //case  FISH_KIND_31: //红色鱼王
    //	createRedKingFish(_fishKind_ex[0]);
    //	break;
    case  FISH_KIND_32: //带底盘鱼王
        createPlatekingFish(_fishKind_ex[0]);
        break;
    default:			//普通鱼
        createNormalFish(_fishKind);
        break;
    }
    
    setFishIsHandLock(false); //先设置手动切换锁定下没有被锁过
    return true;
}

Animation* Fish::addFishData(FishSpirteInfo fish, BYTE kind) {
    char str[32];

    //创建鱼游动动画
    Animation *bMove = Animation::create();
    bMove->setDelayPerUnit(fish.time);
    bMove->setLoops(-1);
    for (int i=1; i <= fish.count; i++) {
        std::string name(fish.name);
        sprintf(str, "%02d.png",i);
        name.append(str);
        bMove->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
    }
    sprintf(str, "fish_%d", kind);
    AnimationCache::getInstance()->addAnimation(bMove, str);
    return bMove;
}

//创建普通鱼（类型0-25）
void Fish::createNormalFish(BYTE fishKind) {
    char name[12];
    sprintf(name, "fish_%d", _fishKind);
    auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));

    if (nullptr == animation) {
        FishSpirteInfo fishInfo;
        if (fishKind >= FISH_KIND_33) { //大闹天宫新增的鱼
            fishInfo = _fishName[fishKind - 5];
        }
        else
        {
            fishInfo = _fishName[fishKind];
        }
       
        animation = addFishData(fishInfo,fishKind);
    }

    auto animate = Animate::create(animation);
    auto fish = Sprite::create();
    fish->runAction(animate);
    fish->setScale(getFighScaleData());
    this->addChild(fish, 2);
    

    //影子
    auto fish_Shadow = Sprite::create();
    fish_Shadow->runAction(animate->clone());
    fish_Shadow->setPosition(Vec2(fish->getPositionX() + 15, fish->getPositionY() + 15));
    fish_Shadow->setColor(Color3B(0, 0, 0));
    fish_Shadow->setOpacity(60);
    fish_Shadow->setScale(fish->getScale());
    this->addChild(fish_Shadow);

    createFishRedShadow(0, animate, fish);
}

//创建三角大三元鱼
void Fish::createDaSanYuanFish(BYTE fishKind) {
    auto node = CSLoader::createNode(DASANYUAN);
    this->addChild(node);
    char str[12];
    for (BYTE i = 0; i < 4; i++) {
        sprintf(str, "Fish%d", i);
        auto fishPos = static_cast<Armature*>(node->getChildByName(str));
        fishPos->setVisible(true);
        
        
        auto node = createSpecialFishDiAnimation();
        if (node && i != 0) {
            node->setPosition(fishPos->getPosition());
            addChild(node);
        }

        sprintf(str, "fish_%d", fishKind);
        auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(str));

        if (nullptr == animation) {
            FishSpirteInfo fishInfo;
            if (fishKind >= FISH_KIND_33) { //大闹天宫新增的鱼
                fishInfo = _fishName[fishKind - 5];
            }
            else
            {
                fishInfo = _fishName[fishKind];
            }
            animation = addFishData(fishInfo, fishKind);
        }

        auto animate = Animate::create(animation);
        auto fish = Sprite::create();
        fish->runAction(animate);
        fish->setPosition(fishPos->getPosition());
        fish->setScale(getFighScaleData());
        this->addChild(fish, 2);

        createFishRedShadow(i, animate, fish);
    }
}

//创建直线大三元鱼
void Fish::createZXDaSanYuanFish(BYTE fishKind) {
    auto node = CSLoader::createNode(ZXDASANYUAN);
    this->addChild(node);
    char str[12];
    for (BYTE i = 0; i < 3; i++) {
        sprintf(str, "Fish%d", i);
        auto fishPos = static_cast<Armature*>(node->getChildByName(str));
        fishPos->setVisible(true);

        auto node = createSpecialFishDiAnimation();
        if (node) {
            node->setPosition(fishPos->getPosition());
            addChild(node);
        }
        
        sprintf(str, "fish_%d", fishKind);
        auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(str));

        if (nullptr == animation) {
            FishSpirteInfo fishInfo;
            if (fishKind >= FISH_KIND_33) { //大闹天宫新增的鱼
                fishInfo = _fishName[fishKind - 5];
            }
            else
            {
                fishInfo = _fishName[fishKind];
            }
            animation = addFishData(fishInfo, fishKind);
        }

        auto animate = Animate::create(animation);
        auto fish = Sprite::create();
        fish->runAction(animate);
        fish->setPosition(fishPos->getPosition());
        fish->setScale(getFighScaleData());
        this->addChild(fish, 2);

        createFishRedShadow(i, animate, fish);
    }
}

//创建四角大四喜鱼
void Fish::createDaSixiFish(BYTE fishKind) {
    auto node = CSLoader::createNode(DASIXI);
    this->addChild(node);
    char str[12];
    for (BYTE i = 0; i < 5; i++) {
        sprintf(str, "Fish%d", i);
        auto fishPos = static_cast<Armature*>(node->getChildByName(str));
        fishPos->setVisible(true);

        auto node = createSpecialFishDiAnimation();
        if (node && i != 0) {
            node->setPosition(fishPos->getPosition());
            addChild(node);
        }
        
        sprintf(str, "fish_%d", fishKind);
        auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(str));
        if (nullptr == animation) {
            FishSpirteInfo fishInfo;
            if (fishKind >= FISH_KIND_33) { //大闹天宫新增的鱼
                fishInfo = _fishName[fishKind - 5];
            }
            else
            {
                fishInfo = _fishName[fishKind];
            }
            animation = addFishData(fishInfo, fishKind);
        }

        auto animate = Animate::create(animation);
        auto fish = Sprite::create();
        fish->runAction(animate);
        fish->setPosition(fishPos->getPosition());
        fish->setScale(getFighScaleData());
        this->addChild(fish, 2);

        createFishRedShadow(i, animate, fish);
    }
}

//创建直线大四喜鱼
void Fish::createZXDaSiXiFish(BYTE fishKind) {
    auto node = CSLoader::createNode(ZXDASIXI);
    this->addChild(node);
    char str[12];
    for (BYTE i = 0; i < 4; i++) {
        sprintf(str, "Fish%d", i);
        auto fishPos = static_cast<Armature*>(node->getChildByName(str));
        fishPos->setVisible(true);

        auto node = createSpecialFishDiAnimation();
        if (node) {
            node->setPosition(fishPos->getPosition());
            addChild(node);
        }
        
        sprintf(str, "fish_%d", fishKind);
        auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(str));
        if (nullptr == animation) {
            FishSpirteInfo fishInfo;
            if (fishKind >= FISH_KIND_33) { //大闹天宫新增的鱼
                fishInfo = _fishName[fishKind - 5];
            }
            else
            {
                fishInfo = _fishName[fishKind];
            }
            animation = addFishData(fishInfo, fishKind);
        }

        auto animate = Animate::create(animation);
        auto fish = Sprite::create();
        fish->runAction(animate);
        fish->setPosition(fishPos->getPosition());
        fish->setScale(getFighScaleData());
        this->addChild(fish, 2);

        createFishRedShadow(i, animate, fish);
    }
}

//创建红色鱼王
void Fish::createRedKingFish(BYTE fishKind) {
    char name[12];
    sprintf(name, "fish_%d", fishKind);

    auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
    if (nullptr == animation) {
        FishSpirteInfo fishInfo;
        if (fishKind >= FISH_KIND_33) { //大闹天宫新增的鱼
            fishInfo = _fishName[fishKind - 5];
        }
        else
        {
            fishInfo = _fishName[fishKind];
        }
        animation = addFishData(fishInfo, fishKind);
    }

    auto animate = Animate::create(animation);

    auto fish = Sprite::create();
    fish->runAction(animate);
    fish->setColor(colorRed);
    fish->setScale(getFighScaleData());
    this->addChild(fish);

    auto fish_Shadow = Sprite::create();
    fish_Shadow->runAction(animate->clone());
    fish_Shadow->setPosition(Vec2(fish->getPositionX() + 15, fish->getPositionY() + 15));
    fish_Shadow->setColor(Color3B(0, 0, 0));
    fish_Shadow->setOpacity(60);
    this->addChild(fish_Shadow, 1);
}

//创建带底盘的鱼王
void Fish::createPlatekingFish(BYTE fishKind) {
    char name[32];
    auto panziAnimation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation("panzi"));
    if (nullptr == panziAnimation) {
        //创建转盘动画
        panziAnimation = Animation::create();
        panziAnimation->setDelayPerUnit(0.5f);
        panziAnimation->setLoops(-1);
//            for (int i = 1; i <= 1; i++)
        {
            sprintf(name, "newzhuanpan%d.png", /*i*/1);
            panziAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
        }
        AnimationCache::getInstance()->addAnimation(panziAnimation, "panzi");
    }
    auto panziAnimate = Animate::create(panziAnimation);
    auto panzi = Sprite::create();
    panzi->runAction(panziAnimate);
    panzi->runAction(RepeatForever::create(RotateBy::create(0.15f, 30.0f)));
    this->addChild(panzi);

    createFishRedShadow(0, panziAnimate, panzi);

    sprintf(name, "fish_%d", fishKind);
    auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
    if (nullptr == animation) {
        FishSpirteInfo fishInfo;
        if (fishKind >= FISH_KIND_33) { //大闹天宫新增的鱼
            fishInfo = _fishName[fishKind - 5];
        }
        else
        {
            fishInfo = _fishName[fishKind];
        }
        animation = addFishData(fishInfo, fishKind);
    }

    auto animate = Animate::create(animation);
    auto smallFish = Sprite::create();
    smallFish->runAction(animate);
    this->addChild(smallFish, 1);

    createFishRedShadow(1, animate, smallFish);
}

/////////////////////////////////////////////////////////
//播放鱼儿死亡动画
void Fish::playFishDieAction() {
    //设置鱼不能被碰撞，防止鱼死亡动画还在进行然后会阻挡子弹
    this->setCanTouch(false);
    //TODO:目前鱼死亡后会在服务端将相关信息移除并通知到所有客户端，所以客户端没有必要重复检测  by HBC
    //if (onUpdateFishScoreCallBack)
    //{
    //	onUpdateFishScoreCallBack(false, _fishID, _fishKind);
    //}
    //移除Update让鱼停止前进
    this->unscheduleUpdate();

    //如果此鱼被锁定，当鱼死亡时需要解锁
    if (_isLock) {
        if (nullptr != onUnLockCallBack) {
            _isLock = false;
            onUnLockCallBack(_fishID);
        }
    }

    switch (_fishKind) {
    case  FISH_KIND_27: //三角大三元
    case  FISH_KIND_28: //直线大三元
    case  FISH_KIND_29: //四角大四喜
    case  FISH_KIND_30: //直线大四喜
    case  FISH_KIND_32: { //带底盘鱼王
        this->runAction(Sequence::create(DelayTime::create(1.0f),
                                         RemoveSelf::create(true), nullptr));
    }
    break;
    //case  FISH_KIND_31: //红色鱼王
    //	{
    //		this->runAction(Sequence::create(Shake::create(1.0f, 10, 10),
    //			RemoveSelf::create(true), nullptr));
    //	}
    //	break;
    default: {		//普通鱼
        this->runAction(Sequence::create(HN::Shake::create(1.0f, 10, 10),
        CallFunc::create([this]() {
            setHide();
        }), nullptr));
    }
    break;
    }
}

//鱼红色的影子
void Fish::createFishRedShadow(int index, Animate* animation, Sprite* fish) {
    _fish_red_shadow[index] = Sprite::create();
    _fish_red_shadow[index]->runAction(animation->clone());
    _fish_red_shadow[index]->setPosition(fish->getPosition());
    _fish_red_shadow[index]->setScale(fish->getScale());
    _fish_red_shadow[index]->setColor(Color3B(255, 0, 0));
    _fish_red_shadow[index]->setLocalZOrder(fish->getLocalZOrder() + 1);
    this->addChild(_fish_red_shadow[index]);
    //默认红色效果隐藏掉
    stopHitEffect();
}

void Fish::initFishRedShadowArray() {
    for (size_t i = 0; i < 5; i++) {
        _fish_red_shadow[i] = nullptr;
    }
}

void Fish::showHitEffect() {
    stopHitEffect();//防止同一条鱼产生多个动作，为其添加新动作之前需要将原来的动作停掉。
    //if (_fish_red_shadow != nullptr)
    auto seq = Sequence::create(DelayTime::create(0.1f), CallFunc::create([this]() {
        showRedShadow();
    }), DelayTime::create(0.1f), CallFunc::create([this]() {
        hideRedShadow();
    }), DelayTime::create(0.1f), CallFunc::create([this]() {
        showRedShadow();
    }), DelayTime::create(0.1f), CallFunc::create([this]() {
        hideRedShadow();
    }), nullptr);
    seq->setTag(Fish_Hit_Action_Tag);
    this->runAction(seq);
}

void Fish::stopHitEffect() {
    //TODO：鱼被移除前应该停止其身上的动画
    this->stopActionByTag(Fish_Hit_Action_Tag);
    hideRedShadow();
}

void Fish::showRedShadow() {
    for (Sprite* shadow : _fish_red_shadow) {
        if (shadow != nullptr) {
            shadow->setVisible(true);
        }
    }
}
void Fish::hideRedShadow() {
    for (Sprite* shadow : _fish_red_shadow) {
        if (shadow != nullptr) {
            shadow->setVisible(false);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////
void Fish::setPosition(const Vec2& pos) {
    Sprite::setPosition(pos);

    float tanX = pos.x - _oldPosition.x;
    float tanY = pos.y - _oldPosition.y;
    float radians = -1 * atan2(tanY, tanX);
    float degree = CC_RADIANS_TO_DEGREES(radians);

    this->setRotation(degree);
    _oldPosition = pos;
}

void Fish::setHide() {
    if (nullptr != _fishMoveAction) {
        _fishMoveAction->release();
        _fishMoveAction = nullptr;
    }
    refreshParams();
    if (nullptr != onMoveOutCallBack) {
        onMoveOutCallBack(_fishID);
    }
}

void Fish::refreshParams() {
    unscheduleUpdate();
    setVisible(false);
    _isTouch = false;
    _isSwitch = false;
    _isLock = false;
    _gameBG = nullptr;
    _FishTraceIndex = 0;
    _isUseFishTrace = true;
    _fishTraceList.clear();
    _stop_count = 0;
    _stop_index = -1;
    _current_stop_count = 0;
    memset(_fishKind_ex, 0, sizeof(_fishKind_ex));
}

void Fish::setFishTraceList(std::vector<FPointAngle> fishTraceList, bool isUseFishTtrace) {
    _fishTraceList.clear();
    _fishTraceList	= fishTraceList;
    _isUseFishTrace	= isUseFishTtrace;
}

void Fish::playFishActionByTraceList() {
    if (_fishTraceList.empty()) return;

    _oldPosition = Vec2(_fishTraceList[0].x, _winSize.height - _fishTraceList[0].y);
    this->setPosition(Vec2(_fishTraceList[1].x, _winSize.height - _fishTraceList[1].y));
    _FishTraceIndex = 2;

    scheduleUpdate();
}

//鱼按照路线游动，匀速直线类型时只有第一和第二两个点有参数，贝塞尔曲线有三个参数
FiniteTimeAction* Fish::getActionInThreePoint(Vec2 first, Vec2 second, Vec2 third, TraceType actionType) {
    FiniteTimeAction* action = nullptr;
    switch (actionType) {
    case TRACE_LINEAR: {	//匀速直线
        //求两点之间的直线距离 ： |Xa-Xb|^ + |Ya-Yb|^ = 距离的平方 （abs:绝对值，pow:平方，sqrt:开方）
        auto range = sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
        auto time = range / (30 * _fishSpeed);

        action = MoveTo::create(time, second);
    }
    break;
    case TRACE_BEZIER: {	//贝塞尔曲线
        float range1 = sqrt(pow(first.x - second.x, 2) + pow(first.y - second.y, 2));
        float range2 = sqrt(pow(second.x - third.x, 2) + pow(second.y - third.y, 2));
        float range = range1 + range2;
        auto time = range / (30 * _fishSpeed);

        ccBezierConfig bezier;
        bezier.controlPoint_1 = first;
        bezier.controlPoint_2 = second;
        bezier.endPosition	  = third;
        action = BezierTo::create(time, bezier);
    }
    break;
    default:
        break;
    }
    return action;
}

void Fish::playFishAction(std::vector<Vec2>* fishTrace, TraceType traceType, FLOAT speed) {
    _fishSpeed = speed;
    this->setCanTouch(true);
    this->setVisible(true);
    this->setPauseAction(false);
    this->setSwitchScene(false);

    if (!_isUseFishTrace) {
        //鱼潮动画
        playFishActionByTraceList();
        return;
    }

    if (nullptr == fishTrace) return;

    _oldPosition = (*fishTrace)[0];
    if (traceType == TRACE_LINEAR) {
        _desPosition = (*fishTrace)[1];
    } else {
        _desPosition = (*fishTrace)[2];
    }

    float go_degree = MathAide::CalcAngle1(_desPosition.x, _desPosition.y, _oldPosition.x, _oldPosition.y);
    float goX = cos(go_degree);
    float goY = sin(go_degree);

    this->setPosition(Vec2((*fishTrace)[0].x + goX, (*fishTrace)[0].y + goY));

    if (nullptr != _fishMoveAction) {
        _fishMoveAction->release();
        _fishMoveAction = nullptr;
    }

    if (traceType == TRACE_LINEAR) {
        _fishMoveAction = getActionInThreePoint((*fishTrace)[0], (*fishTrace)[1],  Vec2::ZERO, traceType);
    } else {
        _fishMoveAction = getActionInThreePoint((*fishTrace)[0], (*fishTrace)[1],  (*fishTrace)[2], traceType);
    }

    _fishMoveAction->retain();
    _fishMoveAction->startWithTarget(this);

    scheduleUpdate();
}

void Fish::setFishStop(std::vector<FPointAngle>::size_type stop_index, std::vector<FPointAngle>::size_type stop_count) {
    _stop_index = stop_index;
    _stop_count = stop_count;
    _current_stop_count = 0;
}

void Fish::update(float dt) {
    //场景切换的时候鱼碰到新背景图就消除掉
    if (_isSwitch) {
        if (this->getPositionX() < 0 || this->getPositionX() > _winSize.width ||
                this->getPositionY() < 0 || this->getPositionY() > _winSize.height) {
            if (_isLock && nullptr != onUnLockCallBack) {
                _isLock = false;
                onUnLockCallBack(_fishID);
            }
            setHide();
            return;
        }
        auto rect = _gameBG->getBoundingBox();
        if (rect.intersectsRect(this->getFishRect())) {
            if (_isLock && nullptr != onUnLockCallBack) {
                _isLock = false;
                onUnLockCallBack(_fishID);
            }
            setHide();
            return;
        }
    }

    //如果打中定海神针，鱼停止游动
    if (_isPause) return;

    auto winSize = Director::getInstance()->getWinSize();
    //更新鱼身上矩形的位置(用来和子弹进行碰撞检测)
    setFishRect(Rect(this->getPositionX() - _fishSize.width / 2, this->getPositionY() - _fishSize.height / 2, _fishSize.width, _fishSize.height));

    //如果此鱼被锁定，当游出屏幕时需要解锁
    if (_isLock) {
        if (this->getPositionX() < 0 || this->getPositionX() > winSize.width ||
                this->getPositionY() < 0 || this->getPositionY() > winSize.height) {
            if (nullptr != onUnLockCallBack) {
                _isLock = false;
                onUnLockCallBack(_fishID);
            }
        }
    }
    //如果此鱼可以被锁定，则当其游入屏幕时需要加入，当其游出屏幕时需要移除
    if (_fishKind >= FISH_KIND_19 && _fishKind <= FISH_KIND_32) {
        if (this->getPositionX() < 0 || this->getPositionX() > winSize.width ||
                this->getPositionY() < 0 || this->getPositionY() > winSize.height) {
            //不在屏幕内，移除
            _isInsert = false;
        } else {
            //在屏幕内，插入
            _isInsert = true;
        }
        if (onUpdateFishScoreCallBack != nullptr) {
            onUpdateFishScoreCallBack(_isInsert, _fishID, _fishKind);
        }
    }

    if (nullptr != _fishMoveAction) {
        if (_fishMoveAction->isDone()) {
            setHide();
            return;
        }
        _fishMoveAction->step(dt);
        return;
    }

    //手机分辨率过大时，为了防止鱼阵游动速度过慢，不限制30帧
    auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    if (!_isDoUpdateFUNC && frameSize.width <= 1920) {
        _isDoUpdateFUNC = true;
        return;
    }
    _isDoUpdateFUNC = false;

    //鱼潮2上下两排小鱼在一定时间内需要先停止
    if (_stop_count > 0 && _FishTraceIndex == _stop_index && _current_stop_count < _stop_count) {
        ++_current_stop_count;
        if (_current_stop_count >= _stop_count) setFishStop(0, 0);
        return;
    } else if (_FishTraceIndex >= _fishTraceList.size()) {
        setHide();
        return;
    }
    this->setPosition(Vec2(_fishTraceList[_FishTraceIndex].x, _winSize.height - _fishTraceList[_FishTraceIndex].y));
    _FishTraceIndex++;
}

FishKind Fish::getFishKindEx() {
    return _fishKind_ex[0];
}
    
    Node* Fish::createSpecialFishDiAnimation() {
        Node* di = nullptr;
        switch (GoldenToadVersion::getGameVersionName()) {
            case Enums::GAME_VERSION_MONKEY_FISH:
            di = CSLoader::createNode(DI_CSB);
            if (di) {
                di->setScale(0.8f);
                auto action = CSLoader::createTimeline(DI_CSB);
                if (action) {
                    di->runAction(action);
                    action->play("CornerTray", true);
                }
            }
            break;
        }
        return di;
    }

//Fish animations
void Fish::onRender() {
    _fishName[0].name = "woniuyu_";					//蜗牛鱼
    _fishName[0].count = 16;
    _fishName[0].time = 0.1f;

    _fishName[1].name = "lvcaoyu_";					//绿草鱼
    _fishName[1].count = 16;
    _fishName[1].time = 0.1f;

    _fishName[2].name = "huangcaoyu_";				//黄草鱼
    _fishName[2].count = 24;
    _fishName[2].time = 0.08f;

    _fishName[3].name = "dayanyu_";					//大眼鱼
    _fishName[3].count = 24;
    _fishName[3].time = 0.1f;

    _fishName[4].name = "huangbianyu_";				//黄鳊鱼
    _fishName[4].count = 24;
    _fishName[4].time = 0.1f;

    _fishName[5].name = "xiaochouyu_";				//小丑鱼
    _fishName[5].count = 25;
    _fishName[5].time = 0.07f;

    _fishName[6].name = "hetun_";					//河豚（小刺鱼）
    _fishName[6].count = 29;
    _fishName[6].time = 0.1f;

    _fishName[7].name = "lanyu_";					//蓝鱼儿
    _fishName[7].count = 20;
    _fishName[7].time = 0.08f;

    //_fishName[8].name = "hongdenglongyu_";			//红灯笼鱼
    //_fishName[8].count = 23;
    //_fishName[8].time = 0.08f;

    _fishName[8].name = "landenglongyu_";			//灯笼鱼
    _fishName[8].count = 24;
    _fishName[8].time = 0.08f;

    _fishName[9].name = "haigui_";					//海龟
    _fishName[9].count = 16;
    _fishName[9].time = 0.1f;

    _fishName[10].name = "huabanyu_";				//花瓣鱼
    _fishName[10].count = 24;
    _fishName[10].time = 0.1f;

    _fishName[11].name = "hudieyu_";				//蝴蝶鱼
    _fishName[11].count = 12;
    _fishName[11].time = 0.12f;

    _fishName[12].name = "kongqueyu_";				//孔雀鱼
    _fishName[12].count = 24;
    _fishName[12].time = 0.1f;

    _fishName[13].name = "jianyu_";					//剑鱼儿
    _fishName[13].count = 20;
    _fishName[13].time = 0.1f;

    _fishName[14].name = "bianfuyu_";				//蝙蝠鱼
    _fishName[14].count = 24;
    _fishName[14].time = 0.1f;

    _fishName[15].name = "yinsha_";					//银鲨鱼（银龙鱼）
    _fishName[15].count = 24;
    _fishName[15].time = 0.1f;

    _fishName[16].name = "huangjingyu_";			//金鲨鱼（金龙鱼）
    _fishName[16].count = 24;
    _fishName[16].time = 0.1f;

    _fishName[17].name = "ding_";					//金箍棒（忠义堂）
    _fishName[17].count = 11;
    _fishName[17].time = 0.3f;

    _fishName[18].name = "bawangjing_";				//霸王鲸（金鲨鱼）
    _fishName[18].count = 9;
    _fishName[18].time = 0.15f;

    //_fishName[20].name = "yinlong_";				//银龙
    //_fishName[20].count = 16;
    //_fishName[20].time = 0.08f;

    _fishName[19].name = "jinlong_";				//金龙
    _fishName[19].count = 8;
    _fishName[19].time = 0.12f;

    _fishName[20].name = "haizao_";					//海藻（水浒传）
    _fishName[20].count = 8;
    _fishName[20].time = 0.2f;

    _fishName[21].name = "zhadan_";					//全屏炸弹（超级炸弹）
    _fishName[21].count = 6;
    _fishName[21].time = 0.3f;

    _fishName[22].name = "jinjinchan_";				//金金蝉（李逵）
    _fishName[22].count = 18;
    _fishName[22].time = 0.3f;

    _fishName[23].name = "lvjinchan_";				//绿金蟾（企鹅）
    _fishName[23].count = 10;
    _fishName[23].time = 0.5f;
}
    
    void Fish::onRenderLiKui() {
        _fishName[0].name = "woniuyu_";					//蜗牛鱼
        _fishName[0].count = 16;
        _fishName[0].time = 0.1f;
        
        _fishName[1].name = "lvcaoyu_";					//绿草鱼
        _fishName[1].count = 16;
        _fishName[1].time = 0.1f;
        
        _fishName[2].name = "huangcaoyu_";				//黄草鱼
        _fishName[2].count = 24;
        _fishName[2].time = 0.08f;
        
        _fishName[3].name = "dayanyu_";					//大眼鱼
        _fishName[3].count = 24;
        _fishName[3].time = 0.1f;
        
        _fishName[4].name = "huangbianyu_";				//黄鳊鱼
        _fishName[4].count = 24;
        _fishName[4].time = 0.1f;
        
        _fishName[5].name = "xiaochouyu_";				//小丑鱼
        _fishName[5].count = 25;
        _fishName[5].time = 0.07f;
        
        _fishName[6].name = "hetun_";					//河豚（小刺鱼）
        _fishName[6].count = 29;
        _fishName[6].time = 0.1f;
        
        _fishName[7].name = "lanyu_";					//蓝鱼儿
        _fishName[7].count = 20;
        _fishName[7].time = 0.08f;
        
        _fishName[8].name = "landenglongyu_";			//灯笼鱼
        _fishName[8].count = 24;
        _fishName[8].time = 0.08f;
        
        _fishName[9].name = "haigui_";					//海龟
        _fishName[9].count = 16;
        _fishName[9].time = 0.1f;
        
        _fishName[10].name = "huabanyu_";				//花瓣鱼
        _fishName[10].count = 24;
        _fishName[10].time = 0.1f;
        
        _fishName[11].name = "hudieyu_";				//蝴蝶鱼
        _fishName[11].count = 12;
        _fishName[11].time = 0.12f;
        
        _fishName[12].name = "kongqueyu_";				//孔雀鱼
        _fishName[12].count = 24;
        _fishName[12].time = 0.1f;
        
        _fishName[13].name = "jianyu_";					//剑鱼儿
        _fishName[13].count = 20;
        _fishName[13].time = 0.1f;
        
        _fishName[14].name = "bianfuyu_";				//蝙蝠鱼
        _fishName[14].count = 24;
        _fishName[14].time = 0.1f;
        
        _fishName[15].name = "yinsha_";					//银鲨鱼（银龙鱼）
        _fishName[15].count = 24;
        _fishName[15].time = 0.1f;
        
        _fishName[16].name = "huangjingyu_";			//金鲨鱼（金龙鱼）
        _fishName[16].count = 24;
        _fishName[16].time = 0.1f;
        
        _fishName[17].name = "ding_";					//金箍棒（忠义堂）
        _fishName[17].count = 11;
        _fishName[17].time = 0.3f;
        
        _fishName[18].name = "bawangjing_";				//霸王鲸（金鲨鱼）
        _fishName[18].count = 9;
        _fishName[18].time = 0.15f;
        
        _fishName[19].name = "jinlong_";				//金龙
        _fishName[19].count = 9;
        _fishName[19].time = 0.2f;
        
        _fishName[20].name = "haizao_";					//海藻（水浒传）
        _fishName[20].count = 8;
        _fishName[20].time = 0.2f;
        
        _fishName[21].name = "zhadan_";					//全屏炸弹（超级炸弹）
        _fishName[21].count = 6;
        _fishName[21].time = 0.3f;
        
        _fishName[22].name = "jinjinchan_";				//金金蝉（李逵）
        _fishName[22].count = 18;
        _fishName[22].time = 0.3f;
        
        _fishName[23].name = "lvjinchan_";				//绿金蟾（企鹅）
        _fishName[23].count = 10;
        _fishName[23].time = 0.5f;
    }
    
    void Fish::onRenderMonkey() {
//        _fishName[0].name = "woniuyu_";                    //蜗牛鱼
//        _fishName[0].count = 16;
//        _fishName[0].time = 0.1f;
//
//        _fishName[1].name = "lvcaoyu_";                    //绿草鱼
//        _fishName[1].count = 16;
//        _fishName[1].time = 0.1f;
//
//        _fishName[2].name = "huangcaoyu_";                //黄草鱼
//        _fishName[2].count = 23;
//        _fishName[2].time = 0.1f;
//
//        _fishName[3].name = "dayanyu_";                    //大眼鱼
//        _fishName[3].count = 14;
//        _fishName[3].time = 0.1f;
//
//        _fishName[4].name = "huangbianyu_";                //黄鳊鱼
//        _fishName[4].count = 16;
//        _fishName[4].time = 0.1f;
//
//        _fishName[5].name = "xiaochouyu_";                //小丑鱼
//        _fishName[5].count = 17;
//        _fishName[5].time = 0.1f;
//
//        _fishName[6].name = "hetun_";                    //河豚（小刺鱼）
//        _fishName[6].count = 29;
//        _fishName[6].time = 0.1f;
//
//        _fishName[7].name = "lanyu_";                    //蓝鱼儿
//        _fishName[7].count = 17;
//        _fishName[7].time = 0.1f;
//
//        _fishName[8].name = "landenglongyu_";            //灯笼鱼
//        _fishName[8].count = 23;
//        _fishName[8].time = 0.1f;
//
//        _fishName[9].name = "haigui_";                    //海龟
//        _fishName[9].count = 16;
//        _fishName[9].time = 0.1f;
//
//        _fishName[10].name = "huabanyu_";                //花瓣鱼
//        _fishName[10].count = 24;
//        _fishName[10].time = 0.1f;
//
//        _fishName[11].name = "hudieyu_";                //蝴蝶鱼
//        _fishName[11].count = 12;
//        _fishName[11].time = 0.1f;
//
//        _fishName[12].name = "kongqueyu_";                //孔雀鱼
//        _fishName[12].count = 24;
//        _fishName[12].time = 0.1f;
//
//        _fishName[13].name = "jianyu_";                    //剑鱼儿
//        _fishName[13].count = 20;
//        _fishName[13].time = 0.1f;
//
//        _fishName[14].name = "bianfuyu_";                //蝙蝠鱼
//        _fishName[14].count = 24;
//        _fishName[14].time = 0.1f;
//
//        _fishName[15].name = "yinsha_";                    //银鲨鱼（银龙鱼）
//        _fishName[15].count = 24;
//        _fishName[15].time = 0.1f;
//
//        _fishName[16].name = "huangjingyu_";            //金鲨鱼（金龙鱼）
//        _fishName[16].count = 24;
//        _fishName[16].time = 0.1f;
//
//        _fishName[17].name = "ding_";                    //金箍棒（忠义堂）
//        _fishName[17].count = 10;
//        _fishName[17].time = 0.1f;
//
//        _fishName[18].name = "bawangjing_";                //霸王鲸（金鲨鱼）
//        _fishName[18].count = 9;
//        _fishName[18].time = 0.15f;
//
//        _fishName[19].name = "jinlong_";                //金龙
//        _fishName[19].count = 9;
//        _fishName[19].time = 0.2f;
//
//        _fishName[20].name = "haizao_";                    //海藻（水浒传）
//        _fishName[20].count = 8;
//        _fishName[20].time = 0.1f;
//
//        _fishName[21].name = "zhadan_";                    //全屏炸弹（超级炸弹）
//        _fishName[21].count = 6;
//        _fishName[21].time = 0.1f;
//
//        _fishName[22].name = "jinjinchan_";                //金金蝉（李逵）
//        _fishName[22].count = 15;
//        _fishName[22].time = 0.1f;
//
//        _fishName[23].name = "lvjinchan_";                //绿金蟾（企鹅）
//        _fishName[23].count = 10;
//        _fishName[23].time = 0.1f;
        
        
        
        _fishName[0].name = "00_";                    //蜗牛鱼
        _fishName[0].count = 5;
        _fishName[0].time = 0.1f;
        
        _fishName[1].name = "01_";                    //绿草鱼
        _fishName[1].count = 11;
        _fishName[1].time = 0.1f;
        
        _fishName[2].name = "02_";                //黄草鱼
        _fishName[2].count = 11;
        _fishName[2].time = 0.1f;
        
        _fishName[3].name = "03_";                    //大眼鱼
        _fishName[3].count = 12;
        _fishName[3].time = 0.1f;
        
        _fishName[4].name = "04_";                //黄鳊鱼
        _fishName[4].count = 9;
        _fishName[4].time = 0.1f;
        
        _fishName[5].name = "05_";                //小丑鱼
        _fishName[5].count = 11;
        _fishName[5].time = 0.1f;
        
        _fishName[6].name = "06_";                    //河豚（小刺鱼）
        _fishName[6].count = 7;
        _fishName[6].time = 0.1f;
        
        _fishName[7].name = "07_";                    //蓝鱼儿
        _fishName[7].count = 11;
        _fishName[7].time = 0.1f;
        
        _fishName[8].name = "08_";            //灯笼鱼
        _fishName[8].count = 5;
        _fishName[8].time = 0.1f;
        
        _fishName[9].name = "09_";                    //海龟
        _fishName[9].count = 11;
        _fishName[9].time = 0.1f;
        
        _fishName[10].name = "10_";                //花瓣鱼
        _fishName[10].count = 11;
        _fishName[10].time = 0.1f;
        
        _fishName[11].name = "11_";                //蝴蝶鱼
        _fishName[11].count = 6;
        _fishName[11].time = 0.1f;
        
        _fishName[12].name = "12_";                //孔雀鱼
        _fishName[12].count = 11;
        _fishName[12].time = 0.1f;
        
        _fishName[13].name = "13_";                    //剑鱼儿
        _fishName[13].count = 11;
        _fishName[13].time = 0.1f;
        
        _fishName[14].name = "14_";                //蝙蝠鱼
        _fishName[14].count = 6;
        _fishName[14].time = 0.1f;
        
        _fishName[15].name = "15_";                    //银鲨鱼（银龙鱼）
        _fishName[15].count = 8;
        _fishName[15].time = 0.1f;
        
        _fishName[16].name = "16_";            //金鲨鱼（金龙鱼）
        _fishName[16].count = 8;
        _fishName[16].time = 0.1f;
        
        //_fishName[17].name = "17_";                    //金箍棒（忠义堂）
        //_fishName[17].count = 7;
        //_fishName[17].time = 0.1f;
        _fishName[17].name = "new17_";                    //金箍棒换成850的镇妖金刚塔
        _fishName[17].count = 10;
        _fishName[17].time = 0.1f;
        
        
        _fishName[18].name = "18_";                //霸王鲸（金鲨鱼）
        _fishName[18].count = 12;
        _fishName[18].time = 0.15f;
        
        //_fishName[19].name = "19_";                //金龙
        //_fishName[19].count = 8;
        //_fishName[19].time = 0.2f;
        
        _fishName[19].name = "new19_";                //船
        _fishName[19].count = 8;
        _fishName[19].time = 0.1f;
        
        //_fishName[20].name = "20_";                    //海藻（水浒传）
        //_fishName[20].count = 4;
        //_fishName[20].time = 0.1f;
        
        _fishName[20].name = "new17_";                    //镇妖塔换成850的无敌风火轮
        _fishName[20].count = 10;
        _fishName[20].time = 0.1f;
        
        //_fishName[21].name = "21_";                    //全屏炸弹（超级炸弹）
        //_fishName[21].count = 5;
        //_fishName[21].time = 0.1f;
        
        _fishName[21].name = "new21_";                    //蟠桃换成850的万佛朝宗
        _fishName[21].count = 9;
        _fishName[21].time = 0.1f;
        
        //_fishName[22].name = "22_";                //金金蝉（李逵）
        //_fishName[22].count = 33;
        //_fishName[22].time = 0.1f;
        
        _fishName[22].name = "new22_";                //孙悟空
        _fishName[22].count = 15;
        _fishName[22].time = 0.1f;
        
        
        //_fishName[23].name = "23_";                //绿金蟾（企鹅）
        //_fishName[23].count = 10;
        //_fishName[23].time = 0.1f;
        _fishName[23].name = "new23_";                //玉皇大帝
        _fishName[23].count = 14;
        _fishName[23].time = 0.1f;
        
        //大闹天宫新增的六种鱼
        _fishName[24].name = "24_";                //绿草鱼
        _fishName[24].count = 7;
        _fishName[24].time = 0.1f;
        
        _fishName[25].name = "25_";                //大眼鱼
        _fishName[25].count = 11;
        _fishName[25].time = 0.1f;
        
        _fishName[26].name = "26_";                //小刺鱼
        _fishName[26].count = 11;
        _fishName[26].time = 0.1f;
        
        _fishName[27].name = "27_";                //剑鱼
        _fishName[27].count = 9;
        _fishName[27].time = 0.1f;
        
        _fishName[28].name = "28_";                //银龙
        _fishName[28].count = 8;
        _fishName[28].time = 0.1f;
        
        _fishName[29].name = "29_";                //美人鱼
        _fishName[29].count = 8;
        _fishName[29].time = 0.1f;
    }
    float Fish::getFighScaleData()
    {
        float _fishScale = 1.0f;
        if (GoldenToadVersion::getGameVersionName() == Enums::GAME_VERSION_MONKEY_FISH) {
            if (_fishKind == FISH_KIND_15) { //海豚
                _fishScale = 2.0f;
            }
            else if (_fishKind == FISH_KIND_22) { //船
                _fishScale = 2.0f;
            }
            else if (_fishKind == FISH_KIND_20 || _fishKind == FISH_KIND_23)
            {
                _fishScale = 1.20f;
            }
            else if (_fishKind == FISH_KIND_24)
            {
                _fishScale = 2.0f;
            }
            else if (_fishKind == FISH_KIND_25)
            {
                _fishScale = 1.80f;
            }
            else if (_fishKind == FISH_KIND_26)
            {
                _fishScale = 2.0f;
            }
        }
        
        return _fishScale;
    }
}
