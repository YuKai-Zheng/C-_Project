#ifndef __NEWYEAR_GameTable_NNTableUI_h__
#define __NEWYEAR_GameTable_NNTableUI_h__

//#include "HNNetExport.h"

#include "NewYearMessageHead.h"
#include "NewYearGameTableUICallback.h"

#include "cocostudio/CocoStudio.h"
//#include "ui/CocosGUI.h"
using namespace cocostudio;
using namespace ui;

#include <string>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "../../../../code/common/LibHNLobby/HNLobby/GameNotice.h"

using namespace cocos2d;
using namespace cocos2d::extension;


namespace NewYearActivity {
class GameTableLogic;

class GameTableUI : public HNGameUIBase , public GameTableUICallback {
public:
    GameTableLogic*			_GameTableLogic;

    //////////////////////////////////////////////////////////////////////////
public:
    GameTableUI();
    ~GameTableUI();
    static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

    bool init(BYTE bDeskIndex, bool bAutoCreate);

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onExitTransitionDidStart()override;

public:
    virtual void onGameUpdateSignInfo(tag_s2c_SignInfo* msg) override;
    virtual void onGameUpdateSignResult(tag_s2c_SignResult* msg) override;
    //赶年兽结果
    virtual void onGameUpdateDriveNianShouInfo(tag_s2c_DriveNianShouInfo* msg) override;
    virtual void onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) override;
	virtual void onGameTurnTableResult( int winArea){};
	virtual void onGameTurnTableToday(int flag){};
private:

    void initUI();
    //回调
    void menuCallBack(Ref* pSender, Widget::TouchEventType type);
    //签到回调
    void qianDaoCallBack(Ref* pSender, Widget::TouchEventType type);
    //活动3按钮回调
    void tntMenuCallBack(Ref* pSender, Widget::TouchEventType type);
    //关闭回调
    void closeEvent(Ref* pSender, Widget::TouchEventType type);

    void loadActivity(BYTE idx);

    //活动1-3
    void createActiviOne(Node * parent);
    void createActiviTwo(Node * parent);
    void createActiviThree(Node * parent);

    //星星
    void showStarAnimation(int index);
    //宝箱
    void showBoxAnimation();
    //年兽动画
    void showNianShouAnimation();

public:
    virtual void onHandleNewMessage(int iType, const std::string &message, int iCount) override;
private:
    //公告
    void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);

private:

    struct VIEW {
        Layout*  Panel;

        Node*	 Menu_node;
    } _view;

    struct VIEWONE {
        Layout*  Panel;

        Button*	Button_qianDao;		//签到按钮

        Node*	 Menu_node;
    } _activityOne;

    struct VIEWTWO {
        Layout*  Panel;

        Node*	 Menu_node;

        Button*	Button_small;
        Button*	Button_mid;
        Button*	Button_big;

        Node*	animation_node;

    } _activityTwo;

    Node*				m_viewParent;
    vector<Button*>			_vecItem;					//子界面切换按钮

    vector<Sprite*>				_vecNormalSpr;   // 子界面选择按钮正常状态vec
    vector<Sprite*>				_vecSelectSpr;	 // 子界面选择按钮点击状态vec

    vector<Node*>             _vecStar; //任务2 星星LIST

    int					m_iSignDay;
    vector<int>			m_vFireCount;

    //公告
    GameNotice*					m_Notice;                   //广播
};
}

#endif
