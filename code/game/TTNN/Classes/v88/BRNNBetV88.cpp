#include "BRNNBetV88.h"

namespace BRNN {
#define BET_MOVE_TIME   0.5

BetV88* BetV88::create(INT typeBet, INT typeZone, Point startPos) {
    BetV88* bet = new BetV88(typeBet, typeZone, startPos);
    if (bet && bet->init())
        bet->autorelease();
    else
        CC_SAFE_DELETE(bet);
    return bet;
}

BetV88::BetV88(INT typeBet, INT typeZone, Point startPos)
    : _typeBet(typeBet)
    ,_typeZone(typeZone)
    ,_startPos(startPos) {
}

BetV88::~BetV88() {
}

bool BetV88::init() {
    if(!HNSprite::init())
        return false;

    if(!(0 <= _typeBet && _typeBet <= 4))
        return false;
    if(!(0<= _typeZone && _typeZone <= 3))
        return false;
    Director::getInstance()->getTextureCache()->addImage(StringUtils::format(BRNN_TABLERES_BET_V88, _typeBet));
    this->initWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(StringUtils::format(BRNN_TABLERES_BET_V88, _typeBet)));
    this->setPosition(_startPos);

    return true;
}

void BetV88::moveToBetZone(Point pos) {
    //HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_DOWN_COIN_V88);
    int maxOffsetX = 16;
    int maxOffsetY = 16;
    //170,195

    //float	minx = rect.getMinX() + rect.size.width * 0.2;
    //float maxx = rect.getMaxX() - rect.size.width * 0.2;
    //float miny = rect.getMinY() + rect.size.height * 0.2;
    //float maxy = rect.getMaxY() - rect.size.height * 0.2;

    //Time::gettimeofdayCocos2d(&psv, NULL);
    //unsigned long int rand_seed = psv.tv_sec * 1000 + psv.tv_usec / 1000;
    //srand(time(NULL)+1);

    srand((int)time(0));

    //random
    int offsetX = rand() % maxOffsetX;
    int offsetY = rand() % maxOffsetY;
    if(0 == rand()%2)
        offsetX = -offsetX;
    if(0 == rand()%2)
        offsetY = -offsetY;

    Point newPos = Point(pos.x + offsetX, pos.y + offsetY);
    _xiazhuAreaPos = newPos;
    this->runAction(MoveTo::create(BET_MOVE_TIME, newPos));
}

void BetV88::moveToBetZone2(Rect rect) {
    //HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_DOWN_COIN_V88);

    int minx = rect.getMinX() + rect.size.width * 0.2;
    int maxx = rect.getMaxX() - rect.size.width * 0.2;
    int miny = rect.getMinY() + rect.size.height * 0.2;
    int maxy = rect.getMaxY() - rect.size.height * 0.2;

    //Point newPos = Point(pos.x + offsetX, pos.y + offsetY);
//    float f1 = (maxx - minx);
    Vec2 pt;
    pt.x = minx + (rand() % (maxx - minx));
    pt.y = maxy - (rand()%(maxy - miny));

    _xiazhuAreaPos = pt;
    this->runAction(MoveTo::create(BET_MOVE_TIME, pt));
}

void BetV88::moveBack() {
    //HNAudioEngine::getInstance()->playEffect(BRNN_SOUND_CLEAR_COIN_V88);
    this->runAction(Sequence::create(MoveTo::create(BET_MOVE_TIME, _startPos), RemoveSelf::create(), nullptr));
}

void BetV88::moveToBanker(Point pos) {
    this->runAction(Sequence::create(MoveTo::create(BET_MOVE_TIME, pos), RemoveSelf::create(),nullptr));
}
}
