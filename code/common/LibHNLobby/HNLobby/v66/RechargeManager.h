
#ifndef RechargeManager_h
#define RechargeManager_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"
#include "HNSocket/HNSocketMessage.h"

USING_NS_CC;

//充值中心界面
class RechargeManager : public Layer 
{
public:
	enum PageType
	{
		EPage_OfficialAgent = 0,	//官方代理
		EPage_Alipay,				//支付宝
		EPage_Unionpay,				//银联支付
		EPage_Number,				//分页的数量;
	};
	
	enum ButtonType
	{
		EBtn_OfficialAgent = 0,	//官方代理
		EBtn_Alipay,			//支付宝
		EBtn_Unionpay,			//银联支付
		EBtn_Number,			//按钮的数量;
	};

    RechargeManager();
    ~RechargeManager();
    CREATE_FUNC(RechargeManager);
    virtual bool init() override;
    
    void openAddToParent(Layer* parent, int zOrder, Vec2 pos);


	void pageButtonCallBack(Ref *pSender, Widget::TouchEventType type);
private:
	void activeCloseButtonSate(Node*pNode);
	void activePageButtonState();
	void switchToTouchBtnByTag(int nTag);
	void switchPageBtnByTag(int nBtnTag);

    void resetData();
    void createUI();
    
    void closeLayer();
    
    void requestState();
    bool signPopularizeSelector(HN::HNSocketMessage *socketMessage);
    
private:
    EventListenerTouchOneByOne*		touchListener;
	Node*					_csbRoot;
	Button*					_pBtnList[EBtn_Number];
	PageView*				_pPageView;
};


#endif /* RechargeManager_h */
