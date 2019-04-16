#ifndef __D21GameUserMessageBoxV99_h__
#define __D21GameUserMessageBoxV99_h__

#include "cocos2d.h"
#include "HNUIExport.h"
#include "UI\HNDialogBase.h"

USING_NS_CC;

namespace D21
{
	class GameUserMessageBoxV99 : public HNDialogBase
	{
	public:
		Rect _userDataBoxRect;
	public:
		bool init(cocos2d::Node* parent, int userID);
		static GameUserMessageBoxV99* createMessageBox(cocos2d::Node* parent, int userID);
	};
}

#endif //__D21GameUserMessageBoxV99_h__
