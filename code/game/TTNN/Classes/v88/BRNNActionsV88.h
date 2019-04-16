#ifndef __BRNN_ActionsV88_H__
#define __BRNN_ActionsV88_H__

#include "cocos2d.h"

USING_NS_CC;

#define MAKEBLACK_IN_ACTION_TIME        0.3f
#define MAKEBLACK_OUT_ACTION_TIME       0.2f

namespace BRNN {
class ActionsV88 {
public:
/*
   帧动画播放动作，注意：真序列从1开始的，所以得注意资源名称。是否是永久执行就不封装了，自己在使用的时控制即可。
   第一个参数为资源路劲，第二个参数为总帧数，第三个参数为帧时间，第四个参数为是否删除对象。
   第一个参数pFile的格式大概是这样的"file%d.png"
 */
static ActionInterval * FrameAnimation(const char * pFile, int frameCount, float frameTime, bool bMoveSelf = false);

/*若隐若现动作
   参数1是隐藏或者显示时间，参数2是动作执行次数
 */
static ActionInterval* LoomingAction(float fDuration, unsigned int times);

/*
   弹出，用于首先设置scale为0的对象
 */
static ActionInterval* SmallToBig(float fDuration = 0.3);

/*
   缩小消失并且移除自身动作,参数为了统一，设默认参数
 */
static ActionInterval* SmallAndRemove(float fDuration = 0.3);

static void actionIn(Node* target);

static void actionOut(Node* target);
};
}


#endif  //__BRNN_Actions_H__
