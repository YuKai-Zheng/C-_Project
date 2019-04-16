#ifndef _IPROGRESSBAR_H_
#define _IPROGRESSBAR_H_

#include "cocos2d.h"
#include <string>

/*

*/
class ProgressbarDelegate {
public:
    virtual void updateProgressBar(std::string text,float percentage) = 0;
    virtual float getPercentage() = 0;
};


class IProgressbar:public cocos2d::Node,public ProgressbarDelegate {
public:
    //调整进度条与背景的Y轴偏移量
    virtual void setProgressBarOffsetY(float offsetY) = 0;
};

#endif //_IPROGRESSBAR_H_