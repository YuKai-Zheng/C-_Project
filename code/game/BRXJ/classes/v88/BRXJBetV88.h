
#ifndef  __BRXJBetV88_H__
#define __BRXJBetV88_H__

#include "HNNetExport.h"
#include "UI/Base/HNNode.h"
using namespace ui;

namespace BRXJ {
class BetV88 : public HNNode {
public:

	CREATE_FUNC(BetV88);
private:

	BetV88();
	~BetV88();

    bool init();

    //初始化BET按钮
    void initBet();


    //callback
    void btnCallBack(Ref* pShender, Widget::TouchEventType  type);
public:
    LLONG getCurBetValue() {
        return curValue;
    }
    int			getCurSeleteBet() {
        return _curSeleteBtnTag;
    }
    void		cleanBetValue() {
        _curSeleteBtnTag = -1;
    };
    void      grayEffectBet(bool flag);
private:

    struct BetStruct {
        Button*					 one;				//百
        Button*					 two;				//千
        Button*					 three;				//万
        Button*					 four;				//10万
        Button*					 five;				//百万
        Button*					 six;					//千万

        Sprite*					oneEffect;				//百
        Sprite*					twoEffect;;				//千
        Sprite*					threeEffect;;				//万
        Sprite*					fourEffect;;				//10万
        Sprite*					fiveEffect;;				//百万
        Sprite*					sixEffect;;					//千万


        Layout		*Panel;
    } _bet;

    LLONG		qian;
    LLONG		wan;
    LLONG		shiwan;
    LLONG		baiwan;
    LLONG		qianwan;
    LLONG		curValue;
    int				_curSeleteBtnTag;
    vector<Sprite*>			_vecEffects;
};
}

#endif //__BRXJBetV88_H__