#ifndef __BRNN_MESSAGEHEAD_H__
#define __BRNN_MESSAGEHEAD_H__

//#include <memory.h>
#include "HNNetExport.h"
#include "BaseMessage.h"

//#pragma pack(push,1)

namespace BRNN {

//游戏信息
enum {
	GAME_NAME_ID                                                =               11105100,           // 名字ID
	GAME_KIND_ID                                                =               1,                          // 种类ID
};

//直接赠送道具
typedef struct {
	int dwUserID;                           //用户ID
	int dwTargetID;                 //目标用户ID
	int nPropID;                            //道具ID
	int nCount;                             //赠送数量
	int dwCost;                             //花费多少钱
} MSG_PROP_GIVE;

}
//#pragma pack(pop)
#endif // __BRNN_MESSAGEHEAD_H__
