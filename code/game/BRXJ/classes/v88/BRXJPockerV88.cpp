#include "BRXJPockerV88.h"
#include "Sound/HNAudioEngine.h"

namespace BRXJ {


//扑克数据
const BYTE m_cbCardListData[POKER_CARD_NUM] = {
    //	  2	   3	4	5	 6	  7    8	9	 10
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0D,	//方块 A - 10
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1D,	//梅花 A - 10
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2D,	//红桃 A - 10
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3D,	//黑桃 A - 10
    //0x4E,0X4F,
};

static const char* BJL_GAME_MUSIC_FAPAI = "v66/music/BJL/fapai.mp3";


PockerV88* PockerV88::create(Vec2 position, float scal, Vec2 targetPos, float angle = 0, BYTE value = 0) {
	PockerV88* ptr = new PockerV88();
    if (nullptr != ptr) {
        ptr->init(position, scal, targetPos, angle, value);
        ptr->_pos = position;
        ptr->_targetPos = targetPos;
        ptr->_scal = scal;
        ptr->_angle = angle;
        ptr->_value = value;
        ptr->autorelease();
        return ptr;
    }
    CC_SAFE_DELETE(ptr);
    return nullptr;
}

PockerV88::PockerV88() {
    _value = 0;
}

PockerV88::~PockerV88() {

}

bool PockerV88::init(Vec2 position, float scal, Vec2 targetPos, float angle, BYTE value) {
    if (!ImageView::init())
        return false;
    char str[30] = { 0 };

    auto name = Transformation(value);
    loadTexture(name);
    setPosition(position);
    setRotation(angle);
    return true;
}

std::string PockerV88::Transformation(int num) {
    char str[50] = { 0 };
    BYTE byColor = num & 0xf0;
    BYTE byValue = num & 0x0f;
    byValue = byValue == 13 ? 1 : byValue + 1;
//    if (PRESSED_TEXTURE){
		sprintf(str, "v66/common/card/%02x.png", byColor | byValue);
//    }
//    else{
//        sprintf(str, "%02x.png", byColor | byValue);
//    }
    return str;
}

void PockerV88::setCardValue(BYTE value) {
    _value = value;
}

void PockerV88::setTargetPos(Vec2 pos) {
    _pos = pos;
}

void PockerV88::moveTagertPos() {
    HNAudioEngine::getInstance()->playEffect(BJL_GAME_MUSIC_FAPAI);
}
}
