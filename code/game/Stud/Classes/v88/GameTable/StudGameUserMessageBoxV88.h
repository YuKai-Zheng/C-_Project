#ifndef __STUD_GAMEUSERMESSAGE_LAYER_V88_H__
#define __STUD_GAMEUSERMESSAGE_LAYER_V88_H__

#include "cocos2d.h"
#include "HNNetExport.h"

#include "StudGamePlayerV88.h"
#include "UI/HNDialogBase.h"

USING_NS_CC;
using namespace HN;

namespace STUD {
class GameUserMessageBoxV88 : public HNDialogBase {
    Rect _userDataBoxRect;
public:
    bool init(cocos2d::Node* parent, INT userID);
    static GameUserMessageBoxV88* createMessageBox(cocos2d::Node* parent,INT userID);
};
}


#endif // __STUD_GAMEUSERMESSAGE_LAYER_V88_H__
