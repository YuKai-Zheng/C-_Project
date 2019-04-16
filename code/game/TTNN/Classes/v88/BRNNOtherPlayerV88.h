#ifndef __BRNNOtherPlayerV88_h__
#define __BRNNOtherPlayerV88_h__

#include "HNNetExport.h"
#include "BRNNMakeBlackV88.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

#include "BRNNResV88.h"

USING_NS_CC;
USING_NS_CC_EXT;

namespace BRNN {
//无座玩家界面
class OtherPlayerV88 : public MakeBlackV88 {
private:
cocos2d::extension::ScrollView*             _scrollview;


public:
OtherPlayerV88();
virtual ~OtherPlayerV88();
public:
	static OtherPlayerV88* create(std::vector</*UserInfoStruct*/char*> vecOthers);

public:
	virtual bool init(std::vector</*UserInfoStruct**/char*> vecOthers);
virtual void onTouchEnded(Touch *touch, Event *unused_event);

void setTitleImage(const char* imageStr);
protected:
	std::vector</*UserInfoStruct**/char*>                _vecOthers;

private:
	Sprite*           m_noSeatBg;

};

}

#endif // __BRNNOtherPlayerV88_h__