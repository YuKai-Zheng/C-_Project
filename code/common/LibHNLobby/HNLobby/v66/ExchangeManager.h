
#ifndef ExchangeManager_h
#define ExchangeManager_h

#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "UI/Base/AUIButton.h"
#include "HNSocket/HNSocketMessage.h"

USING_NS_CC;

//充值中心界面
class ExchangeManager : public Layer 
{
public:
	enum PageType
	{
		EPage_Unionpay = 0,			//银联兑换
		EPage_Alipay,				//支付宝兑换
		EPage_Number,				//分页的数量;
	};
	
	enum ButtonType
	{
		EBtn_Unionpay = 0,		//银联兑换
		EBtn_Alipay,			//支付宝兑换
		EBtn_Number,			//按钮的数量;
	};

    ExchangeManager();
    ~ExchangeManager();
    CREATE_FUNC(ExchangeManager);
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


#endif /* ExchangeManager_h */
