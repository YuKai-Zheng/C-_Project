#ifndef  NoticeList_h__
#define NoticeList_h__

#include "HNUIExport.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
#include "UI/Base/HNLayer.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

// 公告弹窗
class NoticeList : public HNLayer, public extension::TableViewDataSource, public extension::TableViewDelegate {
public:
    // 初始化
    virtual bool init() override;

    // 显示弹窗
    virtual void onEnter() override;

    // 构造函数
    NoticeList();

    // 析构函数
    virtual ~NoticeList();

    // 关闭弹窗回调
    void onCloseClickCallback(Ref* pSender, Widget::TouchEventType type);

    // 创建弹窗
    CREATE_FUNC(NoticeList);

protected:
    // 列表项大小
    virtual Size tableCellSizeForIndex(extension::TableView *table, ssize_t idx) override;

    // 列表项内容
    virtual extension::TableViewCell* tableCellAtIndex(extension::TableView *table, ssize_t idx) override;

    // 列表项数量
    virtual ssize_t numberOfCellsInTableView(extension::TableView *table) override;

    // 列表项被点击
    virtual void tableCellTouched(extension::TableView* table, extension::TableViewCell* cell) override;

    // 查询公告记录
    void queryNoticeList();

    // 获取公告回调
    void onHttpResponse(network::HttpClient* client, network::HttpResponse* response);
private:
    // 数据节点
    struct DataNode {
        std::string title;
        std::string time;
        std::string content;
    };

    // 数据列表
    std::vector<DataNode> _datas;

    // 列表
    extension::TableView* _tableView;

    //处理多次点击公告bug
    bool	_isCloseUI;
};

#endif // NoticeList_h__
