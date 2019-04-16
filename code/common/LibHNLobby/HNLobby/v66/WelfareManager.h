
#ifndef WelfareManager_h
#define WelfareManager_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"
#include "HNSocket/HNSocketMessage.h"

USING_NS_CC;

//福利界面
class WelfareManager : public Layer 
{
public:
	enum PageType
	{
		EPage_WelfarePromotion = 0,	//推广福利
		EPage_MyPromotion,			//我的推广
		EPage_InvitationCode,		//邀请码
		EPage_NonWednesdayWelfare,	//今天不是周三
		EPage_WednesdayWelfare,		//周三福利
		EPage_LuckyWheel,			//幸运转盘
		EPage_Number,				//分页的数量;
	};
	
	enum ButtonType
	{
		EBtn_WelfarePromotion = 0,	//推广福利
		EBtn_MyPromotion,			//我的推广
		EBtn_InvitationCode,		//邀请码
		EBtn_WednesdayWelfare,		//周三福利
		EBtn_LuckyWheel,			//幸运转盘
		EBtn_Number,				//按钮的数量;
	};

	enum TitleType
	{
		ETitle_WelfarePromotion = 0,	//推广福利
		ETitle_InvitationCode,			//邀请码
		ETitle_NonWednesdayWelfare,		//今天不是周三
		ETitle_WednesdayWelfare,		//周三福利
		ETitle_LuckyWheel,				//幸运转盘
		ETitle_Number,					//标题的数量;
	};

    WelfareManager();
    ~WelfareManager();
    CREATE_FUNC(WelfareManager);
    virtual bool init() override;
    

	void openAddToParent(Layer* parent, int zOrder, Vec2 pos);


	void pageButtonCallBack(Ref *pSender, Widget::TouchEventType type);
private:
	void activeCloseButtonSate(Node*pNode);
	void activePageButtonState();
	void switchToTouchBtnByTag(int nTag);
	void switchPageBtnByTag(int nBtnTag);
	void switchTitleByTag(TitleType eType);

    void resetData();
    void createUI();
    
    void closeLayer();
    
    void requestState();
    bool signPopularizeSelector(HN::HNSocketMessage *socketMessage);
    
private:
    EventListenerTouchOneByOne*		touchListener;
	Node*					_csbRoot;
	Button*					_pBtnList[EBtn_Number];
	ImageView*				_pTitleList[ETitle_Number];
	PageView*				_pPageView;
	//false:今天是周三，true:今天不是周三;
	bool					_bNonWednesday;
};


#endif /* WelfareManager_h */
