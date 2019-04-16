#ifndef __BRNNUionBankerLayerV88_h__
#define __BRNNUionBankerLayerV88_h__

#include "HNNetExport.h"

#include "BRNNMakeBlackV88.h"
#include "BRNNTableViewV88.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "BRNNMessageHead.h"

USING_NS_CC;
USING_NS_CC_EXT;


namespace BRNN {
struct tmpShangZhuangUionList {
	int bIsAi;                  //是否是机器人
	int uionarea;                //联合上庄区域
	int count;                    //联合上各区域人数
	bool bIsFull;                                   //是否已经达到上庄条件
	UserInfoStruct* users[MAX_UION_COUNT]; //申请联合上庄的玩家
	tmpShangZhuangUionList() {
		count = 0;
		bIsFull = false;
		memset(users, 0, sizeof(users));
	}
};
typedef std::vector<tmpShangZhuangUionList> vecTmpShangZhuangUionList;

struct UION_BANKER_INFO {
	std::string name;
	LLONG money;
};
typedef std::vector<UION_BANKER_INFO> vecUionBankerInfo;

class UionBankerMemberV88;

class UionBankerListV88 : public MakeBlackV88 {
private:
vecTmpShangZhuangUionList _vecInfo;
LLONG _moneyLimit;
cocos2d::extension::ScrollView*             _scrollview;
public:
static UionBankerListV88* create(vecTmpShangZhuangUionList vecInfo, LLONG moneyLimit);
public:
UionBankerListV88();
virtual ~UionBankerListV88();
public:
virtual bool init(vecTmpShangZhuangUionList vecInfo, LLONG moneyLimit);
void showList();
virtual void onTouchEnded(Touch *touch, Event *unused_event);
};


class UionBankerMemberV88 : public MakeBlackV88, public HNTableViewV88 {
private:
enum Em_BtnTag {
	em_upBtn,
	em_downBtn
};

vecUionBankerInfo _vecInfo;
LLONG _limitMoney;
bool _bMeIn;                                   //我有么有在上庄列表里
int _uionarea;
bool _isFull;
public:
static UionBankerMemberV88* create(vecUionBankerInfo vecInfo, LLONG limitMoney, bool bMeIn, int uionarea, bool isFull);
public:
	UionBankerMemberV88();
	virtual ~UionBankerMemberV88();
public:
virtual bool init(vecUionBankerInfo vecInfo, LLONG limitMoney, bool bMeIn, int uionarea, bool isFull);
private:
void showList();
public:
//生成cell
virtual cocos2d::extension::TableViewCell* tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
private:
void onBtnClick(Ref* pSender,Control::EventType event);
};

}

#endif // __BRNNUionBankerLayer_h__