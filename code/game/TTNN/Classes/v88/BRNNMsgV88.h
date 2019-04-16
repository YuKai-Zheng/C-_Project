#ifndef __BRNNMsgV88_h__
#define __BRNNMsgV88_h__



#include "HNNetExport.h"
#include "BRNNResV88.h"

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "UI/Base/HNNode.h"


USING_NS_CC;
using namespace cocos2d::extension;

namespace BRNN {
//消息提示条
class MsgTipV88 : public HNNode {
public:
    static MsgTipV88* create(std::string msg);
    virtual bool init(std::string msg);
};

}

#endif // __BRNNMsg_h__