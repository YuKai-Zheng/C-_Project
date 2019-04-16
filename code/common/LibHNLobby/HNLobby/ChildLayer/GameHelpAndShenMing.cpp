
#include"GameHelpAndShenMing.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "../FontConfig.h"
#include "VersionInfo/VersionInfo.h"
#include "UI/LoadingLayer.h"

static char* HELP_CSB = "csb/platform/setting/help_view.csb";
static char* SHENMING_CSB = "csb/platform/setting/shenMing_view.csb";

Node * HelpAndShenMing::createView(INT key) {
    auto node = new HelpAndShenMing();

    if (node && node->init(key)) {
        node->autorelease();
    } else {
        delete node;
        node = nullptr;
    }
    return node;
}

HelpAndShenMing::HelpAndShenMing() {
}

HelpAndShenMing::~HelpAndShenMing() {

}

bool HelpAndShenMing::init(INT key) {
    if (!HNNode::init()) {
        return false;
    }

    switch (key) {
    case HELP: {
        m_root = CSLoader::createNode(HELP_CSB);
        auto listview = static_cast<ListView*>(m_root->getChildByName("listview"));
        listview->setScrollBarEnabled(false);
        if (VERSION_NINETY_NINE) {
            listview->setContentSize(Size(990,540));
            listview->setPositionY(-640 / 2 + 200);
        }
        //listview->setScrollBarAutoHideEnabled(false);
//        if (getForAppStore()) {
//            if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) {
//                listview->removeItem(12);
//                listview->removeItem(11);
//                listview->removeItem(10);
//                listview->removeItem(9);
//                listview->removeItem(8); //这个数值是根据资源listview排列顺序
//                listview->removeItem(7);
//                listview->removeItem(6);
//                listview->requestDoLayout();
//            }
//        }
        if (Tools::isiOSReview() && VERSION_NINETY_NINE) {
            listview->removeItem(12);
            listview->removeItem(11);
            listview->removeItem(10);
            listview->removeItem(9);
            listview->removeItem(8); //这个数值是根据资源listview排列顺序
            listview->removeItem(7);
            listview->removeItem(6);
            listview->removeItem(5);
            listview->removeItem(4);
            listview->removeItem(3);
            listview->removeItem(1);
            listview->removeItem(0);
            listview->requestDoLayout();
        }

		//android审核 v3
		if (GD_NEED_OPEN && VERSION_THREE)
		{
			//保留牛牛和捕鱼
			int length = listview->getItems().size();

			for (int i = length - 1; i > 5; i--)
			{
				listview->removeItem(i);
			}
			listview->requestDoLayout();
		}

        addChild(m_root);
        break;
    }
    case SHEN_MING: {
        m_root = CSLoader::createNode(SHENMING_CSB);
        addChild(m_root);

        auto listview = static_cast<ListView*>(m_root->getChildByName("listview"));
        listview->setScrollBarEnabled(false);
        if (VERSION_NINETY_NINE) {
            listview->setContentSize(Size(990, 540));
            listview->setPositionY(-640/2+200);
        }
        break;
    }
    default:
        break;
    }

    loadUI();
    return true;
}


void HelpAndShenMing::listViewCallBack(Ref* pShnder, cocos2d::ui::ListView::EventType type) {
    switch (type) {
    case cocos2d::ui::ListView::EventType::ON_SELECTED_ITEM_END: {
        break;
    }
    default:
        break;
    }
}

void HelpAndShenMing::updateTime(float dt) {
    LoadingLayer::removeLoading(Director::getInstance()->getRunningScene());
}


void HelpAndShenMing::loadUI() {
    _listView = static_cast<ListView*>(m_root->getChildByName("listview"));

    _arrow = static_cast<Sprite*>(m_root->getChildByName("arrow"));
    _arrow->setVisible(false);

}
