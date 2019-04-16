#ifndef __D21GameUserMessageBox_h__
#define __D21GameUserMessageBox_h__

//#include "cocos2d.h"
//#include "HNUIExport.h"
#include "UI/HNDialogBase.h"

//USING_NS_CC;

namespace D21
{
	class GameUserMessageBox : public HNDialogBase
	{
	public:
		Rect _userDataBoxRect;
	public:
		bool init(cocos2d::Node* parent, int userID);
		static GameUserMessageBox* createMessageBox(cocos2d::Node* parent, int userID);
	};
}

#endif
