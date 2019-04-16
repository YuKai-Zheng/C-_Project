#include "BRNNTableViewV88.h"

USING_NS_CC;
USING_NS_CC_EXT;

namespace HN {
HNTableViewV88::HNTableViewV88()
    : m_isMove(false)
    ,m_cellNum(0)
    ,m_curClickIdx(-1)
    ,m_cellSize(Size(0, 0))
    ,m_pTableView(nullptr) {
}

HNTableViewV88::~HNTableViewV88() {
}

cocos2d::extension::TableView* HNTableViewV88::createTableView(cocos2d::Size tableviewSize, cocos2d::extension::ScrollView::Direction eDirection) {
    cocos2d::extension::TableView* tableView = TableView::create(this, tableviewSize);
    tableView->setDelegate(this);
    tableView->setDirection(eDirection);
    tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    tableView->setIgnoreAnchorPointForPosition(false);
    return tableView;
}

//scrollview滚动的时候会调用
void HNTableViewV88::scrollViewDidScroll(cocos2d::extension::ScrollView* view) {
    m_isMove = true;
    m_curClickIdx = -1;
}

//scrollview缩放的时候会调用
void HNTableViewV88::scrollViewDidZoom(cocos2d::extension::ScrollView* view) {
}

//点击哪个cell
void HNTableViewV88::tableCellTouched(TableView* table, TableViewCell* cell) {
    m_isMove = false;
}

//每个cell的size
Size HNTableViewV88::tableCellSizeForIndex(TableView *table, ssize_t idx) {
    return m_cellSize;
}

//生成cell
TableViewCell* HNTableViewV88::tableCellAtIndex(TableView *table, ssize_t idx) {
    TableViewCell * Cell = table->dequeueCell();
    if(!Cell) {
        Cell  = new TableViewCell();
        Cell->autorelease();
    }
    Cell->removeAllChildrenWithCleanup(true);

    return Cell;
}

//cell的数量
ssize_t HNTableViewV88::numberOfCellsInTableView(TableView *table) {
    return m_cellNum;
}

//按下去的时候，就是高亮显示，这里可以设置高亮状态
void HNTableViewV88::tableCellHighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) {
}
//松开的时候，取消高亮状态
void HNTableViewV88::tableCellUnhighlight(cocos2d::extension::TableView* table, cocos2d::extension::TableViewCell* cell) {
}
}