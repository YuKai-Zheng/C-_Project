#ifndef GameDesk_h__
#define GameDesk_h__

//#include "HNNetExport.h"
#include "HNUIExport.h"
#include "HNLogicExport.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <vector>
#include "HNNetProtocol/HNComStruct.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

//桌子信息
typedef struct tagDeskInfo {
    INT		deskID;						 //桌号
    INT		seatID;						 //椅子号
    INT		multiple;					 //倍率
    INT		goldMin;					 //最小携带金钱
    INT		goldMax;					 //最大携带金钱
    INT		peopleValue;				 //桌子当前人数
    INT		peopleMax;				 //桌子最大人数
    std::vector<UserInfoStruct*> deskUsers;

} DeskInfo;

#define CURGAME_DESK_COUNT_ROW					3					//当前每行座子数
#define DEFOUT_LOAD_ITEM_COUNT_TWO			9					//二人游戏默认每次滑动到底部加载item最大数
#define DEFOUT_LOAD_ITEM_COUNT_OTHER		6					//其他游戏默认每次滑动到底部加载item最大数

#define LISTVIEW_ITEM_X_SPACING						3.1					//listview item x轴间距调整值
class DeskUserInfoLayer;
class GameDesk : public HNLayer, public IHNRoomDeskLogic {
public:
    typedef std::function<void ()> CloseCallBack;
    CloseCallBack									onCloseCallBack;
private:
    ComRoomInfo*									_roomInfo;
    PageView*											_pageViewDesks;							// 房间列表
    Button*												_currentSelectedDesk;					// 列表按钮
    std::vector<DeskInfo*>						_deskinfos;
    bool													_isTouch;
    bool													_pageEven;									//是否整数页
    bool													_canCreate;									//是否还需要创建页面
    Node*												_root;											//根节点


    std::vector<Sprite*>							_pagesPrompt;
    vector<Node*>									_vecdeskNode;								//座子vec
    vector<MSG_GR_R_UserSit>				_vecUserSit;								//已经进入玩家数据；

    ListView	*											_listView;										//拖动
    Slider*												_slider;											//滑动···
    Size													_layoutSize;											//item Size大小

    const char*										_curGameDeskCsbPath;					//椅子CSB路径
    UINT													_idx;
    INT													_deskNO;										// 桌子号
    INT													_currentPageIdx;
    INT													_defoutLoadCount;						//初始加载list 中item最大数
    INT													_pageLen;										//计算出来的总页数
    INT													_selectorBtnIdx;
    INT													_scheduleIdx;
    INT													_deskCount;
    INT													_playerCount;
    float													_listOrignY;

    DeskUserInfoLayer*						_deskUserInfoLayer;

public:
    static GameDesk* createDesk(ComRoomInfo* roomInfo);

public:
    GameDesk();
    virtual ~GameDesk();

    bool initData(ComRoomInfo* roomInfo);
    virtual void onExit() override;

public:
    void closeDeskList();
private:
    void scheCallBack(float dt);

    void loadListView();
    void loadSlider();
    void loadListViewItem(UINT &idx);													//listview 回弹触发默认最少加载3个椅子 参数下标为当前已经加载的item位置
    void loadUserRoomIfon(ComRoomInfo* roomInfo);
    void setDefoutValueByMaxPlayer(UINT maxPlayer);			//通过最大人数动态设置 _defoutLoadCount,_layout,_curGameDeskCsbPath,

    void listViewItemCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type);
    void sliderEventCallBack(Ref * pSender, Slider::EventType type);

    //点击椅子上的玩家头像 显示用户信息层
    void openUserInfoLayer();
    void closerUserInfoLayer();
public:
    //listviewin..size
    Size getListViewSize() {
        return _listView->getInnerContainerSize();
    }
    //获取当前房间桌子数 快速加入需要
    INT  getCurRoomDeskCount() {
        return _deskCount;
    }
    INT  getPlayerCount() {
        return _playerCount;
    }
    // 创建牌桌列表pageView
    void createDeskList(ComRoomInfo* roomInfo);
    // 分页创建牌桌列表page
    void createDeskPage();
    // 创建牌桌页面数据
    void createDeskPageInfo(ComRoomInfo* roomInfo);


public:
    // pageView拖动回调函数
    void pageViewMoveCallBack(Ref* pSender, PageView::EventType type);
    // 进入游戏桌点击回调
    void enterTableEventCallBack(Ref* pSender, Widget::TouchEventType type);
    //快速加入游戏回调
    void quickEnterGameCallBack(INT deskNo, INT deskSetation);

    ComRoomInfo* getCurRoomInfo() {
        return _roomInfo;
    }

public:
    virtual void onRoomSitCallback(bool success, const std::string& message, UINT roomID, BYTE deskNo, BYTE seatNo) override;
    virtual void onRoomDeskLock(BYTE deskNo) override;
    virtual void onRoomDeskUnLock(BYTE deskNo) override;
    virtual void onDeskState(MSG_GR_DeskStation * pData) override;
	virtual void onDeskState(BYTE bDeskIndex) override;
    //virtual void onRoomDeskUserCountChanged() override;
    // 2016 7 4 修改 更新人数函数 整加 修改为更新座子界面显示
    virtual void onRoomDeskUserCountChanged(MSG_GR_R_UserSit * pData) override;

    //更新游戏人数
    void updateDeskPeopleCount(BYTE deskNo, bool isLock);
    //更新座子界面
    void updateDeskView(MSG_GR_R_UserSit* pData, bool isLock);

    void upDateDeskFilterFunciton(MSG_GR_R_UserSit* pData, Node*desk);

    void updateDeskHead(INT bDeskNo,INT bDeskStation, Sprite* parent);

protected:
    HNRoomDeskLogic* _deskLogic;
};

#endif // GameDesk_h__