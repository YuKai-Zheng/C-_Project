
#ifndef  __BRXJLuZhiV88_H__
#define __BRXJLuZhiV88_H__

#include "HNNetExport.h"
#include "BRXJMessageHead.h"
#include "UI/Base/HNNode.h"
#include <vector>

using namespace ui;

namespace BRXJ {
class LuZhiV88 : public HNNode {
public:

	CREATE_FUNC(LuZhiV88);
private:

	LuZhiV88();
	~LuZhiV88();

    bool init();

private:

    void initLuzhi();
    //callback
    void btnCallBack(Ref* pShender, Widget::TouchEventType  type);

public:
	void updateLuzi(TGameLuzi *data); 
	Size GetLuziLayoutSize();
private:

    struct LUZHIV99 {
        Layout*			Panel;

        Node*			nt;								//zhuang家路子信息
        Node*			xian;						//xian家路子信息
        Node*			he;					//he家路子信息

        Button*			leftButton;						//按钮
        Button*			rightButton;

        Text*				Text_zhuangPer;				//庄胜率
        Text*				Text_xianPer;					//闲胜率
        Text*				Text_hePer;					//和胜率
    } _luZhiUi;

    vector<Sprite*>				_vecNtTitles;
    vector<Sprite*>				_vecXianTitles;
    vector<Sprite*>				_vecHeTitles;

    float shang;
    float tian;
    float di;
    int count;
    int luziIdx;

    vector<TGameLuzi> _vecTGameLuZi;
};
}

#endif //__BRXJLuZhiV88_H__