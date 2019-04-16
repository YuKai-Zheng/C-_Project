#ifndef __TURNTABLE_GameTable_NNTableUIV88_h__
#define __TURNTABLE_GameTable_NNTableUIV88_h__

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

class LcukyTurnTableV88 : public HNGameUIBase , public GameTableUICallback {
public:
    GameTableLogic*			_GameTableLogic;

    //////////////////////////////////////////////////////////////////////////
public:
	LcukyTurnTableV88();
	~LcukyTurnTableV88();
    static HNGameUIBase* create(BYTE bDeskIndex, bool bAutoCreate);

    bool init(BYTE bDeskIndex, bool bAutoCreate);

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onExitTransitionDidStart()override;

public:
	virtual void onGameUpdateSignInfo(tag_s2c_SignInfo* msg){};
	virtual void onGameUpdateSignResult(tag_s2c_SignResult* msg) {};
    //赶年兽结果
	virtual void onGameUpdateDriveNianShouInfo(tag_s2c_DriveNianShouInfo* msg) {};
	virtual void onGameDriveNianShouResult(int nUserID, long long i64RewardMoney) {};
	virtual void onGameTurnTableResult(int winArea)override;
	virtual void onGameTurnTableToday(int flag){};
private:
    void initUI();
	void levelGame();
private:

    //公告
    void showNotice(const std::string& message, int nCount, int nType, bool bVisible = true);

	void onHandleNewMessage(int iType, const std::string &message, int iCount);	

	void closeEvent(Ref* pSender, Widget::TouchEventType type);
	void btnCallBack(Ref* pSender, Widget::TouchEventType type);
	void reseatData();

	//time
	void blinkTime(float dt);
	void animationCallBack(float dt);
private:

	 struct VIEW
	{
		Node*			root;
		Layout*		layout;

		Button*		Button_arrow;

		Sprite*		Sprite_turnBg;
		Sprite*		blinkWenzi;
	}_view; 

	//blink
	vector<Sprite*>		_vecBlink;

    //公告
    GameNotice*					m_Notice;                   //广播

	int _blinkIdx;
	int _isLasturnCount;	// 均速旋转计数
	int _islastCount;			//最后减速旋转计算
	int _toutleLastTurnCount;	//最后需要旋转总数
	bool	_isLastTage;		//是否为旋转最后阶段
	bool	_isRandomTurn;		//是否随机转动
	bool	_onceMore;
	bool _isPlayer;			//是否进行过转盘游戏今天
	bool _isSelect;			//是否点击开始
	//初始跳动次数
	int _initalJumpCount;

	int m_winArea;
};
} //__TURNTABLE_GameTable_NNTableUIV88_h__

#endif
