#ifndef __HN_HNTableViewV88_H__
#define __HN_HNTableViewV88_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;


namespace HN {
class HNTableViewV88 : public cocos2d::extension::TableViewDataSource,
	public cocos2d::extension::TableViewDelegate {
protected:
int m_curClickIdx;
ssize_t m_cellNum;
bool m_isMove;
Size m_cellSize;
cocos2d::extension::TableView* m_pTableView;

public:
HNTableViewV88();
virtual ~HNTableViewV88();

protected:
cocos2d::extension::TableView* createTableView(cocos2d::Size tableviewSize, cocos2d::extension::ScrollView::Direction eDirection);

public:
//scrollview滚动的时候会调用
virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view);
//scrollview缩放的时候会调用
virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view);
//点击哪个cell
virtual void tableCellTouched(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
//每个cell的size
virtual cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView *table, ssize_t idx);
//生成cell
virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
//cell的数量
virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView *table);
//按下去的时候，就是高亮显示，这里可以设置高亮状态
virtual void tableCellHighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
//松开的时候，取消高亮状态
virtual void tableCellUnhighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell);
};
}

#endif //__HN_HNTableView_H__