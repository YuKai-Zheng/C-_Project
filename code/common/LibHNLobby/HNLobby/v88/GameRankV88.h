
#ifndef __GAMERANKV88_H__
#define __GAMERANKV88_H__

#include "HNNetExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"
#include "HNLobby/pattern/factory/gameRank/IGameRank.h"
#include <vector>

USING_NS_CC;
USING_NS_CC_EXT;

using namespace HN;
using namespace ui;
using namespace std;
using namespace extension;

class GameRankV88 : public IGameRank, public TableViewDataSource, public TableViewDelegate
{

public:
	GameRankV88();
	virtual ~GameRankV88();

//public:
//	typedef std::function<void ()> CloseCallBack;
//	CloseCallBack	onCloseCallBack;

public:
	virtual bool init() override;
	virtual void onExit() override;

	virtual void open(Node* parent, Vec2 vec, int zorder, int tag) override;
	virtual void close() override;
    
    virtual bool onTouchBegan(Touch *touch, Event *unused_event) override;
    virtual void onTouchEnded(Touch *touch, Event *unused_event) override;

	void ControlSwitchCallBack(Ref* pSender, Control::EventType event);
	void canTouchTime(float dt);

//	void closeEventCallBack(Ref* pSender, Widget::TouchEventType type);
	void listViewItemCallBack(Ref* pSender, Widget::TouchEventType type);
	CREATE_FUNC(GameRankV88);

	/*****************************必须继承****************************/
	virtual void scrollViewDidScroll(extension::ScrollView* view) override;//滑动
	virtual void scrollViewDidZoom(extension::ScrollView* view) override{};  //放大

	/***************************TableView**********************************/

	virtual void tableCellTouched(TableView* table, TableViewCell* cell)override;//触摸事件
	virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx)override;//Cell大小
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx)override;//添加Cell
	virtual ssize_t numberOfCellsInTableView(TableView *table)override;//设置Cell个数

private:
	void addItem(INT idx);

private:
    Node*       m_parent;
    
	vector<ListView*> _vecListview;
	ControlSwitch* pControl;
	int					_rankType;
	int					_curRankType;
	Sprite*			_btnTitle;

	int pageIdx = 1;
	int _toutleRecord;
	vector<MSG_GP_MoneyPaiHangBang_Item> _iList;
	int MyRankNum;
	ImageView* rankingFrame;
	Text* rankingText;
	int time;

	bool _isClose;
	bool _canClose;
	vector<MSG_GP_MoneyPaiHangBang_Item> _veMsg;
private:
	//获取排行数据
	void getRankListData(int reqType);
	//申请打开排行榜回调
	bool openRankListEventSelector(HNSocketMessage* socketMessage);
	//显示我的排名
	void showMyRank();
	void selectedItemEvent(Ref* pSender, cocos2d::ui::ListView::EventType type);
};


#endif // __GAMERANKING_LAYER_H__
