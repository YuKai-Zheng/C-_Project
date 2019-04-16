//
//  BBSettlementV88.cpp
//  MixProject
//
//  Created by 黄百超 on 26/03/2017.
//
//

#include "SHZSettlementV88.h"
#include "SHZGameTableUIV88.h"
#include "ScorePanelCellV88.h"
#include "SHZCommonDef.h"
#include "HNLobby/v66/PlatformResV66.h"
#include "HNCommon/HNConverCode.h"
#include "Tool/Tools.h"
#include "HNLobby/globel.h"

namespace SHZ {
    
    enum {
		ZOrder_Color_Layer = 10,
		ZOrder_Content_Layer = 11,
		ZOrder_Cell_Layer = 12,
		BIBEI_BTN_TAG = 20,
		CLOSE_BTN_TAG = 21,

    };
	//int rateNumDatas[9][4] = { { 7, 220, 100, 250 }, { 10, 30, 160, 400 }, { 15, 40, 200, 500 }, { 2, 5, 20, 50 }, { 5, 10, 40, 100 }, { 50, 200, 1000, 2500 }, { 20, 80, 400, 1000 }, { 0, 0, 0, 0 } };
    
    static const char* Settlement_CSB = "Games/31000503/SettlementPanel.csb";

	int numRate[4] = { 3, 4, 5, 15 };

	
    
	SHZSettlementV88::SHZSettlementV88() {
        
    }
    
	SHZSettlementV88::~SHZSettlementV88() {
        
    }
    
	bool SHZSettlementV88::init() {
        if (!Layer::init()) {
            return false;
        }
        initSettleLayer();
		_logic = nullptr;

		EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(false);
		listener->onTouchBegan = CC_CALLBACK_2(SHZSettlementV88::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(SHZSettlementV88::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

        return true;
    }
    
	void SHZSettlementV88::setSettlementLayerPosition(cocos2d::Vec2 pos) {
		if (_root.settlementNode) {
			_root.settlementNode->setPosition(pos);
		}
	}

	bool SHZSettlementV88::onTouchBegan(Touch *touch, Event *unused_event) {
		Point point = touch->getLocation();
		if (!this->getBoundingBox().containsPoint(point)) {
			this->moveSelfWithAction();
			_logic->sendIsGuessSize(0);
		}
		return true;
	}

	void SHZSettlementV88::moveSelfWithAction() {
		
		this->runAction(Sequence::create(ScaleTo::create(0.2f, 0.0f), RemoveSelf::create(), nullptr));
		this->setVisible(false);
	}

	void SHZSettlementV88::onTouchEnded(Touch *touch, Event *unused_event) {
	}



	void SHZSettlementV88::setLogic(GameTableLogic *logic) {
		_logic = logic;
	}
    

	void SHZSettlementV88::hideSettlementLayer() {
        this->setVisible(false);
		for (size_t i = 0; i < socreCellVec.size(); i++)
		{
			_root.settlementNode->removeChildByTag(i);
		}
    }
    
	//设置数据
	void SHZSettlementV88::setSettlementData(int logoType[9], int rate[9],int rateNum[9]) {

		int time=0;
		int index=0;
		int count=0;
        m_totalRate=0;
		socreCellVec.clear();
		for (int i = 0; i < LINE_COUNT; i++)
		{
			if (logoType[i] != -1)
			{	
                
				ScorePanelCellV88* newCell = ScorePanelCellV88::create();
				newCell->setCellValue(logoType[i], rateNum[i], rate[i]);
				newCell->setPosition(-304 + (count%3)* 236, 125 - 73 * (count / 3));
				newCell->setTag(count);
				_root.settlementNode->addChild(newCell, 1000);
				socreCellVec.push_back(newCell);
				count++;
                m_totalRate += rate[i];
			}
	
		}

		this->setVisible(true);
        
    }

	void SHZSettlementV88::setAddTotalMoney(long long addMoney)
	{
		char str[100];
		sprintf(str, "%lld", addMoney);
		_root.totalAddMoney->setString(str);
        //
        if (m_totalRate>=100) {
            Layout* layout = Layout::create();
            if (layout) {
                layout->setContentSize(WIN_SIZE);
                layout->setTouchEnabled(true);
                layout->setSwallowTouches(true);
                layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layout->setPosition(WIN_SIZE/2);
                layout->runAction(Sequence::create(DelayTime::create(2.2f), RemoveSelf::create(), nullptr));
                addChild(layout,ZOrder_Content_Layer+1);
                //
                std::string path="";
                std::string name="";
                std::string fontPath="";
                if (m_totalRate>=100 && m_totalRate<200) {
                    path = "v88/animation/watermargin/water_jackpot/water_jackpot_low/water_jackpot_low.csb";
                    name = "water_jackpot_low";
                    fontPath = "v88/common/zi/2222.png";
                }
                else if (m_totalRate>=200 && m_totalRate<500) {
                    path = "v88/animation/watermargin/water_jackpot/water_jackpot_middle/water_jackpot_middle.csb";
                    name = "water_jackpot_middle";
                    fontPath = "v88/common/zi/1111.png";
                }
                else if (m_totalRate>= 500) {
                    path = "v88/animation/watermargin/water_jackpot/water_jackpot_high/water_jackpot_high.csb";
                    name = "water_jackpot_high";
                    fontPath = "v88/common/zi/1111.png";
                }
                auto node = CSLoader::createNode(path);
                if (node) {
                    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
                    layout->addChild(colorLayer);
                    colorLayer->runAction(Sequence::create(DelayTime::create(2.5f), RemoveSelf::create(), nullptr));
                    //
                    node->setPosition(WIN_SIZE/2);
                    layout->addChild(node);
                    auto tankuang = node->getChildByName("tankuang");
                    if (tankuang) {
                        auto labelBg = tankuang->getChildByName("tankuang");
                        if (labelBg) {
                            TextAtlas* _textScore = TextAtlas::create("", fontPath, 33, 48, "0");
                            _textScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                            _textScore->setString(to_string(addMoney));
                            _textScore->setPosition(Vec2(labelBg->getContentSize().width*0.25f, labelBg->getContentSize().height*0.64f));
                            labelBg->addChild(_textScore);
                        }
                    }
                    //
                    auto action = CSLoader::createTimeline(path);
                    if (action) {
                        node->runAction(action);
                        action->play(name, false);
                    }
                    
                }
            }

        }
	}
    
	void SHZSettlementV88::initSettleLayer() {
		//
		auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 150));
		if (colorLayer) {
			colorLayer->setContentSize(WIN_SIZE);
			this->addChild(colorLayer, ZOrder_Color_Layer);
		}
		//
		auto root = CSLoader::createNode(Settlement_CSB);
		if (root) {
			_root.settlementNode = root;
			addChild(root, ZOrder_Content_Layer);
			//
			auto text = dynamic_cast<Text*>(root->getChildByName("totalAddMoney"));
			if (text) {
				_root.totalAddMoney = text;
				text->setString("");
			}

			auto bibeiBtn= dynamic_cast<Button*>(root->getChildByName("bibeiBtn"));
			if (bibeiBtn) {
				_root.bibeiBtn = bibeiBtn;
				_root.bibeiBtn->setTag(BIBEI_BTN_TAG);
				_root.bibeiBtn->addTouchEventListener(CC_CALLBACK_2(SHZSettlementV88::guessSizeCallBack, this));
                Tools::addAnimationFrame("v88/animation/watermargin/bibei_button/bibei_button.csb", _root.bibeiBtn, "bibei_button", Vec2(0, bibeiBtn->getContentSize().height*0.1f));
			}
			auto closeBtn = dynamic_cast<Button*>(root->getChildByName("closeBtn"));
			if (closeBtn) {
				_root.closeBtn = closeBtn;
				_root.closeBtn->setTag(CLOSE_BTN_TAG);
				_root.closeBtn->addTouchEventListener(CC_CALLBACK_2(SHZSettlementV88::guessSizeCallBack, this));
			}
		}
	}

	//下注增加筹码减少筹码
	void SHZSettlementV88::guessSizeCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
		if (type != Widget::TouchEventType::ENDED) {
			return;
		}
		int typeGuess = 0;
		auto btn = (Button*)pSender;
		if (btn->getTag() == BIBEI_BTN_TAG) {
			typeGuess = 1;
			_logic->sendIsGuessSize(typeGuess);
			this->hideSettlementLayer();
		}
		else
		{
			_logic->sendIsGuessSize(typeGuess);
			this->hideSettlementLayer();
		}


	}


    
}
