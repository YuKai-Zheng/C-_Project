#ifndef _RESULT_DIALOG_
#define _RESULT_DIALOG_

#include "HNNetExport.h"

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class ResultDialogV88 : public HNLayer
{
public:
	static ResultDialogV88* create(Node* parent, LLONG dealerWin, LLONG myWin);
	bool init(Node* parent, LLONG dealerWin, LLONG myWin);
	
protected:
	ResultDialogV88();
	~ResultDialogV88();
	bool onTouchBegan(Touch *touch, Event *unused_event);
private:
	LLONG _dealerWin;
	LLONG _myWin;
	ui::Widget* _widget;
	void initUI();
};

#endif // !_RESULT_DIALOG_
