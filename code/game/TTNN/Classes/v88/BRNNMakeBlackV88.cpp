#include "BRNNMakeBlackV88.h"
#include "Sound/HNAudioEngine.h"

//#define MakeBlackV88_IN_ACTION_TIME	0.3
//#define MakeBlackV88_OUT_ACTION_TIME	0.2
#define MOVE_THRESHOLD_DEFINE           1               //移动范围阀值定义，发现某些硬件设备过于敏感调用touchmove

MakeBlackV88::MakeBlackV88()
    : _bMoved(false)
    ,_bSelfMoved(false)
//,_black(nullptr)
{
}

MakeBlackV88::~MakeBlackV88() {
    /*if(_black)
       {
        _black->removeFromParent();
        _black = nullptr;
       }*/
}

bool MakeBlackV88::init() {
    if(!HNGameUIBase::init())
        return false;

    //创建一个点击事件
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->setSwallowTouches(true);//阻止触摸向下传递
    myListener->onTouchBegan = CC_CALLBACK_2(MakeBlackV88::onTouchBegan, this);
    myListener->onTouchMoved = CC_CALLBACK_2(MakeBlackV88::onTouchMoved, this);
    myListener->onTouchEnded = CC_CALLBACK_2(MakeBlackV88::onTouchEnded, this);
    myListener->onTouchCancelled = CC_CALLBACK_2(MakeBlackV88::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, this);

    auto scene = Director::getInstance()->getRunningScene();
    auto winSize = Director::getInstance()->getWinSize();
    /*_black = LayerColor::create(Color4B(0,0,0,155), winSize.width, winSize.height);
       scene->addChild(_black);*/

    this->setIgnoreAnchorPointForPosition(false);
    this->setPosition(winSize.width/2, winSize.height/2);
    //this->actionIn();
    BRNN::ActionsV88::actionIn(this);
    scene->addChild(this);

    return true;
}

bool MakeBlackV88::onTouchBegan(Touch *touch, Event *unused_event) {
    Point point = touch->getLocation();
    if (!this->getBoundingBox().containsPoint(point)) {
        this->moveSelfWithAction();
        _bSelfMoved = true;
    }

    _bMoved = false;
    _beginPos = point;
    return true;
}

void MakeBlackV88::onTouchMoved(Touch *touch, Event *unused_event) {
    Point point = touch->getLocation();
    if(abs(int(point.x - _beginPos.x) > MOVE_THRESHOLD_DEFINE) || abs(int(point.y - _beginPos.y) > MOVE_THRESHOLD_DEFINE))
        _bMoved = true;
}

void MakeBlackV88::onTouchEnded(Touch *touch, Event *unused_event) {

}

void MakeBlackV88::onTouchCancelled(Touch *touch, Event *unused_event) {

}

void MakeBlackV88::moveSelfWithAction() {
    HNAudioEngine::getInstance()->playEffect("platform/sound/sound_close.mp3");
    //this->actionOut();
    BRNN::ActionsV88::actionOut(this);
}

//void MakeBlackV88::actionIn() {
//    auto winSize = Director::getInstance()->getWinSize();
//    //int actionId = rand()%6;
//    int actionId = 0;
//    switch (actionId) {
//    case 0: { //小
//        this->setScale(0);
//        this->runAction(ScaleTo::create(MakeBlackV88_IN_ACTION_TIME, 1.0));
//    }
//    break;
//    case 1: { //大
//        this->setScale(3);
//        this->runAction(ScaleTo::create(MakeBlackV88_IN_ACTION_TIME, 1.0));
//    }
//    break;
//    case 2: { //上
//        this->runAction(Sequence::create(Place::create(Point(this->getPositionX(), winSize.height+this->getContentSize().height/2)),
//                                         MoveBy::create(MakeBlackV88_IN_ACTION_TIME, Point(0, -(winSize.height/2 + this->getContentSize().height/2))),
//                                         nullptr));
//    }
//    break;
//    case 3: { //下
//        this->runAction(Sequence::create(Place::create(Point(this->getPositionX(), -this->getContentSize().height/2)),
//                                         MoveBy::create(MakeBlackV88_IN_ACTION_TIME, Point(0, winSize.height/2+this->getContentSize().height/2)),
//                                         nullptr));
//    }
//    break;
//    case 4: { //左
//        this->runAction(Sequence::create(Place::create(Point(-this->getContentSize().width/2, this->getPositionY())),
//                                         MoveBy::create(MakeBlackV88_IN_ACTION_TIME, Point(winSize.width/2 + this->getContentSize().width/2, 0)),
//                                         nullptr));
//    }
//    break;
//    case 5: { //右
//        this->runAction(Sequence::create(Place::create(Point(winSize.width + this->getContentSize().width/2, this->getPositionY())),
//                                         MoveBy::create(MakeBlackV88_IN_ACTION_TIME, Point(-(winSize.width/2 + this->getContentSize().width/2), 0)),
//                                         nullptr));
//    }
//    break;
//    default:
//        break;
//    }
//}
//
//void MakeBlackV88::actionOut() {
//    //int actionId = rand()%6;
//    int actionId = 0;
//    switch (actionId) {
//    case 0: { //小
//        this->runAction(Sequence::create(ScaleTo::create(MakeBlackV88_OUT_ACTION_TIME, 0), RemoveSelf::create(), nullptr));
//    }
//    break;
//    case 1: { //大
//        this->runAction(Sequence::create(ScaleTo::create(MakeBlackV88_IN_ACTION_TIME, 3.0), RemoveSelf::create(), nullptr));
//    }
//    break;
//    case 2: { //上
//        this->runAction(Sequence::create(MoveBy::create(MakeBlackV88_IN_ACTION_TIME, Point(0, this->getPositionY()+this->getContentSize().height/2)),
//                                         RemoveSelf::create(),
//                                         nullptr));
//    }
//    break;
//    case 3: { //下
//        this->runAction(Sequence::create(MoveBy::create(MakeBlackV88_IN_ACTION_TIME, Point(0, -(this->getPositionY()+this->getContentSize().height/2))),
//                                         RemoveSelf::create(),
//                                         nullptr));
//    }
//    break;
//    case 4: { //左
//        this->runAction(Sequence::create(MoveBy::create(MakeBlackV88_IN_ACTION_TIME, Point(-(this->getPositionX()+this->getContentSize().width/2), 0)),
//                                         RemoveSelf::create(),
//                                         nullptr));
//    }
//    break;
//    case 5: { //右
//        this->runAction(Sequence::create(MoveBy::create(MakeBlackV88_IN_ACTION_TIME, Point(this->getPositionX()+this->getContentSize().width/2, 0)),
//                                         RemoveSelf::create(),
//                                         nullptr));
//    }
//    break;
//    default:
//        break;
//    }
//}
