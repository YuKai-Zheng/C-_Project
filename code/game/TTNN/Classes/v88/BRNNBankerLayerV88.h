#ifndef __BRNNBankerLayerV88_h__
#define __BRNNBankerLayerV88_h__

#include "HNNetExport.h"

#include "BRNNMakeBlackV88.h"
#include "BRNNTableViewV88.h"
#include "BRNNResV88.h"

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

/************************************************************************/
/* 申请上庄列表的界面                                                                     */
/************************************************************************/

namespace BRNN {
struct TMP_BANKER_INFOV88 {
	int dwUserId;
	LLONG money;
	std::string name;
};
typedef std::vector<TMP_BANKER_INFOV88> vecTmpBankerInfoV88;

//申请上庄列表的界面
class BankerLayerV88 : /*public MakeBlackV88, */public HNGameUIBase, public HNTableViewV88 {
public:
BankerLayerV88();
virtual ~BankerLayerV88();
static BankerLayerV88* create(vecTmpBankerInfoV88 vecInfo, LLONG limitMoney, bool bMeIn);
virtual bool init(vecTmpBankerInfoV88 vecInfo, LLONG limitMoney, bool bMeIn);
//生成cell
//virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);


public:
	ListView* bankerListViewItem;
public:
void setBankerList(vecTmpBankerInfoV88 list);
void closeBankerLayer();
virtual bool onTouchBegan(Touch *touch, Event *event);
private:
//申请上庄的玩家列表
vecTmpBankerInfoV88 _vecInfo;
//申请上庄的金额下限
LLONG _limitMoney;
//我有么有在上庄列表里
bool _bMeIn;
//显示申请上庄的玩家列表
void showList();
void onBtnClick(Ref* pSender,Control::EventType event);
//申请上庄
void applyBanker();
//申请下庄
void handleXiazhuang(Ref *pSender, Widget::TouchEventType type);

Node* createItem(std::string name);
private:
	int colHeight = 0;   //每行的间隔
	int rowWidth = 0;    //每列的间隔
	
enum Em_BtnTag {
	//我要上庄 按钮
	em_upBtn,
	//我要下庄 按钮
	em_downBtn
};
};



}

#endif // __BRNNBankerLayerV88_h__