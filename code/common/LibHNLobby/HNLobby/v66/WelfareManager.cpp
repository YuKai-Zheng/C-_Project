#include "WelfareManager.h"
#include "HNLobby/GamePrompt.h"
#include "HNPlatform/HNPlatformLogic.h"

WelfareManager::WelfareManager() 
{
	_csbRoot = nullptr;

	_bNonWednesday = true;
	for (int i = 0; i < EBtn_Number; i++)
	{
		_pBtnList[i] = nullptr;
	}

	_pPageView = nullptr;
}

WelfareManager::~WelfareManager() 
{
    
}

bool WelfareManager::init()
{
    if (!Layer::init())
	{
        return false;
    }

	_csbRoot = CSLoader::createNode("csb/v66/welfare/Node_Popup_Welfare.csb");
	addChild(_csbRoot);

	Layout* layout = (Layout *)_csbRoot->getChildByName("Panel_Layout_bg");


	_pBtnList[EBtn_WelfarePromotion]	= static_cast<Button*>(Tools::getNodeByName(layout, "Button_welfare_promotion"));
	_pBtnList[EBtn_MyPromotion]			= static_cast<Button*>(Tools::getNodeByName(layout, "Button_my_popul"));
	_pBtnList[EBtn_InvitationCode]		= static_cast<Button*>(Tools::getNodeByName(layout, "Button_invitation_code"));
	_pBtnList[EBtn_WednesdayWelfare]	= static_cast<Button*>(Tools::getNodeByName(layout, "Button_wednesday_welfare"));
	_pBtnList[EBtn_LuckyWheel]			= static_cast<Button*>(Tools::getNodeByName(layout, "Button_lucky_wheel"));

	_pTitleList[ETitle_WelfarePromotion]	= static_cast<ImageView*>(Tools::getNodeByName(layout, "Image_page_title_promotion"));
	_pTitleList[ETitle_InvitationCode]		= static_cast<ImageView*>(Tools::getNodeByName(layout, "Image_page_title_invitation_code"));
	_pTitleList[ETitle_NonWednesdayWelfare] = static_cast<ImageView*>(Tools::getNodeByName(layout, "Image_page_title_non_wednesday"));
	_pTitleList[ETitle_WednesdayWelfare]	= static_cast<ImageView*>(Tools::getNodeByName(layout, "Image_page_title_wednesday"));
	_pTitleList[ETitle_LuckyWheel]			= static_cast<ImageView*>(Tools::getNodeByName(layout, "Image_page_title_lucky_wheel"));

	_pPageView = static_cast<PageView*>(Tools::getNodeByName(layout, "PageView_content"));
	_pPageView->setTouchEnabled(false);
	
	_pBtnList[EBtn_WelfarePromotion]->setTag(EBtn_WelfarePromotion);
	_pBtnList[EBtn_MyPromotion]->setTag(EBtn_MyPromotion);
	_pBtnList[EBtn_InvitationCode]->setTag(EBtn_InvitationCode);
	_pBtnList[EBtn_WednesdayWelfare]->setTag(EBtn_WednesdayWelfare);
	_pBtnList[EBtn_LuckyWheel]->setTag(EBtn_LuckyWheel);

	activePageButtonState();
	activeCloseButtonSate(layout);
	
	
    return true;
}

void WelfareManager::openAddToParent(Layer *parent, int zOrder, cocos2d::Vec2 pos)
{
    if (parent) 
	{
        this->setPosition(pos);
        parent->addChild(this, zOrder);
    }
}

void WelfareManager::activeCloseButtonSate(Node*pNode)
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

void WelfareManager::activePageButtonState()
{
	for (int i = 0; i < EBtn_Number; i++)
	{
		auto pBtn = _pBtnList[i];
		if (!pBtn)
			continue;

		pBtn->addTouchEventListener(CC_CALLBACK_2(WelfareManager::pageButtonCallBack, this));
	}
}

void WelfareManager::switchToTouchBtnByTag(int nTag)
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

void WelfareManager::switchPageBtnByTag(int nBtnTag)
{
	if (nBtnTag < EBtn_WelfarePromotion && nBtnTag >= EBtn_Number)
		return;

	switch (nBtnTag)
	{
	case EBtn_WelfarePromotion:
	{
		switchTitleByTag(ETitle_WelfarePromotion);
		_pPageView->setCurrentPageIndex(EPage_WelfarePromotion);
	}
		break;
	case EBtn_MyPromotion:
	{
		switchTitleByTag(ETitle_WelfarePromotion);
		_pPageView->setCurrentPageIndex(EPage_MyPromotion);
	}
		break;
	case EBtn_InvitationCode:
	{
		switchTitleByTag(ETitle_InvitationCode);
		_pPageView->setCurrentPageIndex(EPage_InvitationCode);
	}
		break;
	case EBtn_WednesdayWelfare:
	{
		if (_bNonWednesday)
		{
			switchTitleByTag(ETitle_NonWednesdayWelfare);
			_pPageView->setCurrentPageIndex(EPage_NonWednesdayWelfare);
		}
		else
		{
			switchTitleByTag(ETitle_WednesdayWelfare);
			_pPageView->setCurrentPageIndex(EPage_WednesdayWelfare);
		}
	}
		break;
	case EBtn_LuckyWheel:
	{
		switchTitleByTag(ETitle_LuckyWheel);
		_pPageView->setCurrentPageIndex(EPage_LuckyWheel);
	}
		break;
	default:
		break;
	}
	
}

void WelfareManager::switchTitleByTag(TitleType eType)
{
	if (eType < ETitle_WelfarePromotion && eType >= ETitle_Number)
		return;

	for (int i = 0; i < ETitle_Number; i++)
	{
		auto pTitle = _pTitleList[i];
		if (!pTitle)
			continue;

		if (i == (int)eType)
		{
			//显示;
			pTitle->setVisible(true);
		}
		else
		{
			pTitle->setVisible(false);
		}
	}
}

void WelfareManager::pageButtonCallBack(Ref *pSender, Widget::TouchEventType type)
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

void WelfareManager::resetData()
{
    touchListener = nullptr;
}

void WelfareManager::createUI() 
{
    
}

void WelfareManager::closeLayer() 
{
    this->removeFromParent();
}

void WelfareManager::requestState()
{
    MSG_GP_TuiGuang_ShenQing shenqing;
    shenqing.iUserID = PlatformLogic()->loginResult.dwUserID;
    shenqing.iType = 0;     //状态查询
    PlatformLogic()->sendData(MDM_GP_TUIGUANG_SHENQING, ASS_GP_TUIGUANG_SHENQING, &shenqing, sizeof(MSG_GP_TuiGuang_ShenQing), HN_SOCKET_CALLBACK(WelfareManager::signPopularizeSelector, this));
}

bool WelfareManager::signPopularizeSelector(HN::HNSocketMessage *socketMessage)
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

