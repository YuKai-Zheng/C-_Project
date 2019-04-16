#include "ExchangeManager.h"
#include "HNLobby/GamePrompt.h"
#include "HNPlatform/HNPlatformLogic.h"

ExchangeManager::ExchangeManager() 
{
	_csbRoot = nullptr;

	for (int i = 0; i < EBtn_Number; i++)
	{
		_pBtnList[i] = nullptr;
	}

	_pPageView = nullptr;
}

ExchangeManager::~ExchangeManager() 
{
    
}

bool ExchangeManager::init()
{
    if (!Layer::init())
	{
        return false;
    }

	_csbRoot = CSLoader::createNode("csb/v66/exchange/Node_Popup_Exchange.csb");
	addChild(_csbRoot);

	Layout* layout = (Layout *)_csbRoot->getChildByName("Panel_Layout_bg");


	_pBtnList[EPage_Unionpay]	= static_cast<Button*>(Tools::getNodeByName(layout, "Button_unionpay"));
	_pBtnList[EBtn_Alipay]		= static_cast<Button*>(Tools::getNodeByName(layout, "Button_alipay"));


	_pPageView = static_cast<PageView*>(Tools::getNodeByName(layout, "PageView_content"));
	_pPageView->setTouchEnabled(false);
	
	_pBtnList[EBtn_Unionpay]->setTag(EBtn_Unionpay);
	_pBtnList[EBtn_Alipay]->setTag(EBtn_Alipay);


	activePageButtonState();
	activeCloseButtonSate(layout);
	
	
    return true;
}

void ExchangeManager::openAddToParent(Layer *parent, int zOrder, cocos2d::Vec2 pos)
{
    if (parent) 
	{
        this->setPosition(pos);
        parent->addChild(this, zOrder);
    }
}

void ExchangeManager::activeCloseButtonSate(Node*pNode)
{
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* event)
	{
		return true;
	};
	touchListener->onTouchEnded = [=](Touch* touch, Event* event)
	{
		closeLayer();
	};
	touchListener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, pNode);
}

void ExchangeManager::activePageButtonState()
{
	for (int i = 0; i < EBtn_Number; i++)
	{
		auto pBtn = _pBtnList[i];
		if (!pBtn)
			continue;

		pBtn->addTouchEventListener(CC_CALLBACK_2(ExchangeManager::pageButtonCallBack, this));
	}
}

void ExchangeManager::switchToTouchBtnByTag(int nTag)
{
	for (int i = 0; i < EBtn_Number; i++)
	{
		auto pBtn = _pBtnList[i];
		if (!pBtn)
			continue;
		if (i == nTag)
		{
			//将选中的按钮设置为禁止状态
			pBtn->setBright(false);
			//pBtn->setEnabled(false);
			//pBtn->setTouchEnabled(false);
		}
		else
		{
			//将非选中的按钮设置为正常状态
			pBtn->setBright(true);
			pBtn->setEnabled(true);
			pBtn->setTouchEnabled(true);
		}
	}
}

void ExchangeManager::switchPageBtnByTag(int nBtnTag)
{
	if (nBtnTag < EBtn_Unionpay && nBtnTag >= EBtn_Number)
		return;

	_pPageView->setCurrentPageIndex(nBtnTag);
}


void ExchangeManager::pageButtonCallBack(Ref *pSender, Widget::TouchEventType type)
{
	auto pBtn = static_cast<Button*>(pSender);
	if (Widget::TouchEventType::ENDED != type)
	{
		if (Widget::TouchEventType::BEGAN == type)
		{
			switchToTouchBtnByTag(pBtn->getTag());
		}
		else if (Widget::TouchEventType::CANCELED == type)
		{
			
		}

		return;
	}

	switchPageBtnByTag(pBtn->getTag());
	
}

void ExchangeManager::resetData()
{
    touchListener = nullptr;
}

void ExchangeManager::createUI() 
{
    
}

void ExchangeManager::closeLayer() 
{
    this->removeFromParent();
}

void ExchangeManager::requestState()
{
    MSG_GP_TuiGuang_ShenQing shenqing;
    shenqing.iUserID = PlatformLogic()->loginResult.dwUserID;
    shenqing.iType = 0;     //状态查询
    PlatformLogic()->sendData(MDM_GP_TUIGUANG_SHENQING, ASS_GP_TUIGUANG_SHENQING, &shenqing, sizeof(MSG_GP_TuiGuang_ShenQing), HN_SOCKET_CALLBACK(ExchangeManager::signPopularizeSelector, this));
}

bool ExchangeManager::signPopularizeSelector(HN::HNSocketMessage *socketMessage)
{
//    @ret=1--设置成功
//    @ret=2--用户ID不存在
//    @ret=3--审核中
//    @ret=4--已经通过审核
//    @ret=5--系统拒绝
//    @ret=6--未申请
    CCAssert(sizeof(MSG_GP_TuiGuang_ShenQing) == socketMessage->objectSize, "sizeof(MSG_GP_TuiGuang_ShenQing) != objectSize");
    if (sizeof(MSG_GP_TuiGuang_ShenQing) != socketMessage->objectSize)    return false;
    MSG_GP_TuiGuang_ShenQing* msg = (MSG_GP_TuiGuang_ShenQing*)socketMessage->object;
    if (msg == nullptr) {
        return true;
    }
    if (socketMessage) 
	{
        switch (socketMessage->messageHead.bHandleCode)
		{
            case 3: 
			{
                //审核中
                //auto signLayer = PopularizeSignLayer::create();
                //if (signLayer) {
                  //  signLayer->openAddToParent(this, WIN_SIZE/2);
                //    signLayer->setIsSigning(true);
               // }
            }
                break;
            case 4:
			{
                //审核通过
                //auto myLayer = PopularizeOfMy::create();
                //if (myLayer) {
               //     myLayer->openAddToParent(this, WIN_SIZE/2);
                //}
            }
            break;
            case 5: 
			{
                //系统拒绝
                //auto resultLayer = PopularizeSignResult::create();
                //if (resultLayer) {
                  //  resultLayer->openAddToParent(this, WIN_SIZE/2);
                  //  if (msg) {
                  //      resultLayer->setRefuseInfo(msg->szMessageInfo);
                 //   }
                //}
            }
            break;
            case 6:
			{
                //未申请
                //auto signLayer = PopularizeSignLayer::create();
               // if (signLayer) {
                //    signLayer->openAddToParent(this, WIN_SIZE/2);
                //}
            }
                break;
            default:
                //closeLayer();
                break;
        }
    }
    
    return true;
}

