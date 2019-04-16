#ifndef __BRNNPlayerSeatV88_h__
#define __BRNNPlayerSeatV88_h__

#include "HNNetExport.h"
#include "BRNNGiveGiftV88.h"
#include "BRNNActionsV88.h"
#include "BRNNResV88.h"

#include "FontSize.h"

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "UI/UserHead.h"
#include "BRNNGameTableLogic.h"

USING_NS_CC;
USING_NS_CC_EXT;

extern int framesV88[27];

namespace BRNN {
typedef int SeatId;

//玩家座位
class PlayerSeatV88 : public HNGameUIBase {

private:
INT _seatIndex;
UserInfoStruct*             _user;
bool _bSetGirl;
Sprite*                             _girl;
Sprite*                             _seatBg;
/** @brief	玩家名字 */
Label*                              _nameLabel;
/** @brief	玩家金钱 */
Label*                              _moneyLabel;
//Size				_size;
Sprite*                             _emotionSprite;
bool isBanker;
//Sprite* blackQueue;

UserHead*	m_HeadImage;

public:
static PlayerSeatV88* create(INT seatIndex = 6, UserInfoStruct* user = nullptr);

PlayerSeatV88();
virtual ~PlayerSeatV88();
virtual bool init(INT seatIndex, UserInfoStruct* user);

void setGirl();
void setUser(UserInfoStruct* user);
void updateUser();
void userExit();
void setBankerBg(std::string  bgPath);
//void showEmotion(int emotionId);
//void showWords(std::string words);
void removeEmotionSprite();
UserInfoStruct* getUser() {
	return _user;
}

void setGameTableLogic(GameTableLogic* gameTableLogic){
	_GameTableLogic = gameTableLogic;
}

private:
	GameTableLogic* _GameTableLogic;

void initData();
int getEmotionFrame(int idx);    //根据idx索引资源帧数
};

}

#endif // __BRNNPlayerSeat_h__