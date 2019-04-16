//
//  RedPacketSetPacketBombLayer.cpp
//  MixProject_189
//
//  Created by 黄百超 on 08/08/2017.
//
//

#include "RedPacketSetPacketBombLayer.h"
#include "HNLobby/v88/PlatformResV88.h"
#include "HNCommon/HNConverCode.h"
#include "HNPlatform/HNPlatformLogic.h"
#include "HNLobby/globel.h"

namespace RedPacket {
    
    RedPacketSetPacketBombLayer::RedPacketSetPacketBombLayer() {
        
    }
    
    RedPacketSetPacketBombLayer::~RedPacketSetPacketBombLayer() {
        
    }
    
    bool RedPacketSetPacketBombLayer::init() {
        if (!Layer::init()) {
            return false;
        }
        
        this->setContentSize(WIN_SIZE);
        this->setIgnoreAnchorPointForPosition(false);
        
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketSetPacketBombLayer::openAddToParent(RedPacket::RedPacketSendPacketLayer *parent, Vec2 pos, int bombNumber) {
        if (parent) {
            m_sendPacketLayer = parent;
            this->setPosition(pos);
            parent->addChild(this);
            m_bombNumber = bombNumber >= 0 ? bombNumber : 0;
            if (m_selectBombList) {
                m_selectBombList->jumpToItem(m_bombNumber, Vec2(0, 0.5), Vec2::ANCHOR_MIDDLE);
                setListViewSelectedItem(m_bombNumber);
            }
        }
    }
    
    void RedPacketSetPacketBombLayer::resetData() {
        m_bg = nullptr;
        touchListener = nullptr;
        m_sendPacketLayer = nullptr;
        m_selectBombList = nullptr;
        m_bombNumber = 0;
        m_closeButton = nullptr;
    }
    
    void RedPacketSetPacketBombLayer::createUI() {
        auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 100));
        if (colorLayer) {
            addChild(colorLayer);
        }
        //
        touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = [=](Touch* touch, Event* event) {
            return true;
        };
        touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
            closeLayer();
        };
        touchListener->setSwallowTouches(true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, colorLayer);
        //
        m_bg = ImageView::create("v88/fahongbao/fasong/14.png");
        if (m_bg) {
            m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_bg->setPosition(WIN_SIZE/2);
            m_bg->setTouchEnabled(true);
            addChild(m_bg);
            //
            m_selectBombList = ListView::create();
            if (m_selectBombList) {
                //
                auto kuang = ImageView::create("v88/fahongbao/fasong/13.png");
                if (kuang) {
                    kuang->setPosition(m_bg->getContentSize()*0.5f);
                    m_bg->addChild(kuang);
                    Size kuangSize = Size(289, 89);
                    Size si = Size(289, 267);
                    //
                    m_selectBombList->setGravity(cocos2d::ui::ListView::Gravity::TOP);
                    m_selectBombList->setDirection(ListView::Direction::VERTICAL);
                    m_selectBombList->setContentSize(si);
                    m_selectBombList->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    m_selectBombList->setPosition(Vec2(kuang->getContentSize().width*0.505f,kuang->getContentSize().height*0.425f));
                    m_selectBombList->setScrollBarAutoHideEnabled(false);
                    m_selectBombList->setBounceEnabled(true);

					ListView::ccListViewCallback func = [=](Ref* pSender, ListView::EventType type) {
						if (type == ListView::EventType::ON_SELECTED_ITEM_END) {
							m_bombNumber = (int)m_selectBombList->getCurSelectedIndex();
							setListViewSelectedItem(m_bombNumber);
						}
					};
					m_selectBombList->addEventListener(func);
                    m_bg->addChild(m_selectBombList);
                    //
                    for (int i = 0; i < 10; i++) {
                        auto layout = Layout::create();
                        if (layout) {
                            layout->setTag(i);
                            layout->setContentSize(kuangSize);
                            layout->setTouchEnabled(true);
                            
                            auto text = Text::create(to_string(i), FONT_TTF_MSYH, 30);
                            if (text) {
                                text->setTextAreaSize(layout->getContentSize());
                                text->setTextColor(Color4B::BLACK);
                                text->setTextVerticalAlignment(TextVAlignment::CENTER);
                                text->setTextHorizontalAlignment(TextHAlignment::CENTER);
                                text->setPosition(layout->getContentSize()*0.5f);
                                layout->addChild(text);
                                //
                                auto select = ImageView::create("v88/fahongbao/fasong/12_1.png");
                                if (select) {
                                    select->setName("select");
                                    select->setPosition(layout->getContentSize()*0.5f);
                                    layout->addChild(select);
                                }
                            }
                            m_selectBombList->pushBackCustomItem(layout);
                        }
                    }
                    //close button
                    m_closeButton = Button::create("v88/fahongbao/fasong/17.png", "v88/fahongbao/fasong/17_1.png");
                    if (m_closeButton) {
                        m_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
                        m_closeButton->setPosition(Vec2(m_bg->getContentSize().width, m_bg->getContentSize().height));
                        m_closeButton->addTouchEventListener(CC_CALLBACK_2(RedPacketSetPacketBombLayer::closeButtonCallBack, this));
                        m_bg->addChild(m_closeButton);
                    }
                    
//                    auto topKuang = ImageView::create("v88/fahongbao/fasong/12.png");
//                    if (topKuang) {
//                        topKuang->setPosition(m_bg->getContentSize()/2);
//                        m_bg->addChild(topKuang);
//                    }
                }
            }
        }
    }
    
    void RedPacketSetPacketBombLayer::closeLayer() {
        if (m_sendPacketLayer) {
            m_sendPacketLayer->setBombNumber(m_bombNumber);
        }
        this->removeFromParent();
    }
    
    void RedPacketSetPacketBombLayer::setListViewSelectedItem(int selectedItem) {
        if (selectedItem >= 0 && selectedItem < 10) {
            if (m_selectBombList) {
                for (int i = 0; i < m_selectBombList->getItems().size(); i++) {
                    Layout* layout = (Layout*)m_selectBombList->getItems().at(i);
                    if (layout) {
                        ImageView* select = (ImageView* )layout->getChildByName("select");
                        if (select) {
                            select->setVisible(selectedItem == layout->getTag());
                        }
                    }
                }
            }
        }
    }
    
    void RedPacketSetPacketBombLayer::closeButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        closeLayer();
    }
    
}
