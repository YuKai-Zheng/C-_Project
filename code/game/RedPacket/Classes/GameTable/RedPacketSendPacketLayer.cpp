//
//  RedPacketSendPacketLayer.cpp
//  MixProject_VIP
//
//  Created by 黄百超 on 04/08/2017.
//
//

#include "RedPacketSendPacketLayer.h"
#include "HNLobby/v88/PlatformResV88.h"
#include "HNCommon/HNConverCode.h"
#include "RedPacketSetPacketBombLayer.h"

#include "Tool/Tools.h"
#include "HNLobby/GamePrompt.h"
#include "HNLobby/globel.h"
//#define RED_WIN_SIZE Size(WIN_SIZE.height, WIN_SIZE.width)

namespace RedPacket {
    
    RedPacketSendPacketLayer::RedPacketSendPacketLayer() {
        
    }
    
    RedPacketSendPacketLayer::~RedPacketSendPacketLayer() {
        
    }
    
    bool RedPacketSendPacketLayer::init() {
        if (!Layer::init()) {
            return false;
        }
        
        this->setContentSize(WIN_SIZE);
        this->setIgnoreAnchorPointForPosition(false);
        
        resetData();
        createUI();
        
        return true;
    }
    
    void RedPacketSendPacketLayer::editBoxReturn(ui::EditBox *editBox) {
        if (editBox) {
            if (m_packetMoneyEditBox) {
                LLONG money = atoll(m_packetMoneyEditBox->getString().c_str()) * 10000;
                m_setPacketMoney = money;
                auto moneyString = Tools::addComma(money);
                char str[128];
                sprintf(str, GetText("SEND_PACKET_MONEY"), moneyString.c_str());
                m_packetMoneyText->setString(str);
            }
        }
    }
    
    void RedPacketSendPacketLayer::openAddToParent(HNLayer *parent, cocos2d::Vec2 pos) {
        if (parent) {
            this->setPosition(pos);
            parent->addChild(this);
        }
    }
    
    void RedPacketSendPacketLayer::setBombNumber(int number) {
        if (m_bombButton) {
            m_setBombNumber = number;
            m_bombButton->setTitleText(to_string(number));
        }
    }
    
    void RedPacketSendPacketLayer::setSendPacketCallBack(std::function<void (LLONG packetMoney, int packetPart, int thunderNumber)> callBack) {
        m_sendPacketCallBack = callBack;
    }
    
    void RedPacketSendPacketLayer::updateRoomBaseData(RedPacket::tag_s2c_BaseConfigMsgReq baseData) {
        m_setPacketPart = baseData.nCanSplitCount;
        char str[128];
        if (m_multiText) {
            //个数
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("SEND_PACKET_PART"), m_setPacketPart);
            m_multiText->setString(str);
        }
        if (m_rateText) {
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("SEND_PACKET_RATE"), baseData.nLeiRate);
            m_rateText->setString(str);
        }
        if (m_packetMoneyEditBox) {
            m_minMoney = baseData.i64MinMoney;
            m_maxMoney = baseData.i64MaxMoney;
            memset(str, 0, sizeof(str));
            sprintf(str, GetText("SEND_PACKET_MONEY_TIP"), m_minMoney/10000, m_maxMoney/10000);
            m_packetMoneyEditBox->setPlaceHolder(str);
        }
    }
    
    void RedPacketSendPacketLayer::resetData() {
        m_bg = nullptr;
        m_closeButton = nullptr;
//        m_helpButton = nullptr;
        m_title = nullptr;
        m_sendButton = nullptr;
        m_packetMoneyBG = nullptr;
        m_packetMoneyEditBox = nullptr;
        m_packetBombBG = nullptr;
        m_bombButton = nullptr;
        m_rateText = nullptr;
        m_packetMoneyText = nullptr;
        m_multiText = nullptr;
        m_setBombNumber = -1;
        m_setPacketMoney = 0;
        m_setPacketPart = 0;
        m_minMoney = 0;
        m_maxMoney = 0;
    }
    
    void RedPacketSendPacketLayer::createUI() {
        m_bg = ImageView::create("v88/fahongbao/fasong/110.png");
        if (m_bg) {
            m_bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            m_bg->setPosition(WIN_SIZE/2);
            m_bg->setTouchEnabled(true);
			m_bg->setContentSize(WIN_SIZE);
			m_bg->setScale9Enabled(true);
            addChild(m_bg);
            //
            m_closeButton = Button::create("v88/fahongbao/1.png", "v88/fahongbao/0.png");
            if (m_closeButton) {
                m_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                m_closeButton->setPosition(Vec2(0, m_bg->getContentSize().height));
                m_closeButton->addTouchEventListener(CC_CALLBACK_2(RedPacketSendPacketLayer::backButtonCallBack, this));
                addChild(m_closeButton);
                //
                m_title = Text::create("发红包", FONT_TTF_MSYH, 32);
                if (m_title) {
                    m_title->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
                    m_title->setPosition(Vec2(m_closeButton->getPositionX()+m_closeButton->getContentSize().width, m_closeButton->getPositionY()-m_closeButton->getContentSize().height/2));
                    m_title->setColor(Color3B(255, 225, 176));
                    addChild(m_title);
                }
            }
//            m_helpButton = Button::create("v88/fahongbao/fasong/9.png", "v88/fahongbao/fasong/10.png");
//            if (m_helpButton) {
//                m_helpButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
//                m_helpButton->setPosition(Vec2(WIN_SIZE.width, m_bg->getContentSize().height));
//                m_helpButton->addTouchEventListener(CC_CALLBACK_2(RedPacketSendPacketLayer::helpButtonCallBack, this));
//                addChild(m_helpButton);
//            }
            m_sendButton = Button::create("v88/fahongbao/fasong/6.png", "v88/fahongbao/fasong/8.png", "v88/fahongbao/fasong/7.png");
            if (m_sendButton) {
                m_sendButton->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.2f));
                m_sendButton->addTouchEventListener(CC_CALLBACK_2(RedPacketSendPacketLayer::sendButtonCallBack, this));
                m_sendButton->setBright(true);
                m_bg->addChild(m_sendButton);
                //
                m_packetMoneyText = Text::create("0 金币", FONT_TTF_MSYH, 70);
                if (m_packetMoneyText) {
                    m_packetMoneyText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    m_packetMoneyText->setTextAreaSize(Size(m_bg->getContentSize().width-20, m_bg->getContentSize().height));
                    m_packetMoneyText->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.35f));
                    m_packetMoneyText->setTextVerticalAlignment(TextVAlignment::CENTER);
                    m_packetMoneyText->setTextHorizontalAlignment(TextHAlignment::CENTER);
                    m_packetMoneyText->setColor(Color3B::BLACK);
                    m_bg->addChild(m_packetMoneyText);
                }
                //
                auto sendTip = Text::create("", FONT_TTF_MSYH, 26);
                if (sendTip) {
                    sendTip->setString(GetText("OPEN_PACKET_TIP"));
                    sendTip->setContentSize(m_sendButton->getContentSize());
                    sendTip->setTextAreaSize(m_sendButton->getContentSize());
                    sendTip->setPosition(Vec2(m_sendButton->getPositionX(), m_sendButton->getPositionY()-m_sendButton->getContentSize().height*0.8f));
                    sendTip->setTextVerticalAlignment(TextVAlignment::CENTER);
                    sendTip->setTextHorizontalAlignment(TextHAlignment::CENTER);
                    sendTip->setColor(Color3B(138, 138, 138));
                    m_bg->addChild(sendTip);
                }
            }
            //packet money edit box
            m_packetMoneyBG = ImageView::create("v88/fahongbao/fasong/3.png");
            if (m_packetMoneyBG) {
                m_packetMoneyBG->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_packetMoneyBG->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.85f));
                m_bg->addChild(m_packetMoneyBG);
                //
                auto textField = TextField::create("", FONT_TTF_MSYH, 28);
                if (textField) {
                    textField->setTextAreaSize(Size(m_packetMoneyBG->getContentSize().width*0.45f, m_packetMoneyBG->getContentSize().height));
                    textField->setTextHorizontalAlignment(TextHAlignment::RIGHT);
                    textField->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                    textField->setPosition(Vec2(m_packetMoneyBG->getContentSize().width*0.75f, m_packetMoneyBG->getContentSize().height*0.5f));
                    textField->setPlaceHolder("请设定红包金额");
                    textField->setVisible(false);
                    m_packetMoneyBG->addChild(textField);
                    m_packetMoneyEditBox = HNEditBox::createEditBox(textField, this);
                    if (m_packetMoneyEditBox) {
                        m_packetMoneyEditBox->setContentSize(Size(m_packetMoneyBG->getContentSize().width*0.45f, m_packetMoneyBG->getContentSize().height));
                        m_packetMoneyEditBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                        m_packetMoneyEditBox->setInput(HNEditBox::InputMode::PHONE_NUMBER);
                        m_packetMoneyEditBox->setPosition(textField->getPosition());
                        m_packetMoneyEditBox->setFontColor(Color3B::GRAY);
                    }
                }
                //
                auto moneyTip = Text::create("每人抽到的金额随机。", FONT_TTF_MSYH, 26);
                if (moneyTip) {
                    moneyTip->setTextAreaSize(Size(m_packetMoneyBG->getContentSize().width-33*2, m_packetMoneyBG->getContentSize().height));
                    moneyTip->setPosition(Vec2(m_packetMoneyBG->getPositionX(), m_packetMoneyBG->getPositionY()-m_packetMoneyBG->getContentSize().height*0.75f));
                    moneyTip->setTextVerticalAlignment(TextVAlignment::CENTER);
                    moneyTip->setTextHorizontalAlignment(TextHAlignment::LEFT);
                    moneyTip->setColor(Color3B(138, 138, 138));
                    m_bg->addChild(moneyTip);
                }
            }
            m_packetBombBG = ImageView::create("v88/fahongbao/fasong/5.png");
            if (m_packetBombBG) {
                m_packetBombBG->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.7f));
                m_bg->addChild(m_packetBombBG);
                //
                m_bombButton = Button::create("v88/fahongbao/fasong/16.png", "v88/fahongbao/fasong/16.png");
                if (m_bombButton) {
                    m_bombButton->setTitleColor(Color3B::BLACK);
                    m_bombButton->setTitleFontSize(36);
                    m_bombButton->setTitleAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
                    m_bombButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
                    m_bombButton->setPosition(Vec2(m_packetBombBG->getContentSize().width-33, m_packetBombBG->getContentSize().height/2));
                    m_bombButton->addTouchEventListener(CC_CALLBACK_2(RedPacketSendPacketLayer::setBombCallBack, this));
                    m_packetBombBG->addChild(m_bombButton);
                }
                //
                auto bombTip = Text::create("请在此处设置红包地雷。", FONT_TTF_MSYH, 26);
                if (bombTip) {
                    bombTip->setTextAreaSize(Size(m_packetBombBG->getContentSize().width-33*2, m_packetBombBG->getContentSize().height));
                    bombTip->setPosition(Vec2(m_packetBombBG->getPositionX(), m_packetBombBG->getPositionY()-m_packetBombBG->getContentSize().height*0.75f));
                    bombTip->setTextVerticalAlignment(TextVAlignment::CENTER);
                    bombTip->setTextHorizontalAlignment(TextHAlignment::LEFT);
                    bombTip->setColor(Color3B(138, 138, 138));
                    m_bg->addChild(bombTip);
                }
                //
                m_rateText = Text::create("倍数", FONT_TTF_MSYH, 30);
                if (m_rateText) {
                    m_rateText->setContentSize(m_packetBombBG->getContentSize());
                    m_rateText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    m_rateText->setTextAreaSize(Size(m_packetBombBG->getContentSize().width-33*2, m_packetBombBG->getContentSize().height));
                    m_rateText->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.49f));
                    m_rateText->setTextVerticalAlignment(TextVAlignment::CENTER);
                    m_rateText->setTextHorizontalAlignment(TextHAlignment::LEFT);
                    m_rateText->setColor(Color3B::BLACK);
                    m_bg->addChild(m_rateText);
                    auto rateTip = Text::create("倍数根据红包个数而定。", FONT_TTF_MSYH, 26);
                    if (rateTip) {
                        rateTip->setTextAreaSize(Size(m_packetBombBG->getContentSize().width-33*2, m_packetBombBG->getContentSize().height));
                        rateTip->setPosition(Vec2(m_rateText->getPositionX(), m_rateText->getPositionY()-m_rateText->getContentSize().height*0.5f));
                        rateTip->setTextVerticalAlignment(TextVAlignment::CENTER);
                        rateTip->setTextHorizontalAlignment(TextHAlignment::LEFT);
                        rateTip->setColor(Color3B(138, 138, 138));
                        m_bg->addChild(rateTip);
                    }
                }
                m_multiText = Text::create("个数", FONT_TTF_MSYH, 30);
                if (m_multiText) {
                    m_multiText->setContentSize(m_packetBombBG->getContentSize());
                    m_multiText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    m_multiText->setTextAreaSize(Size(m_packetBombBG->getContentSize().width-33*2, m_packetBombBG->getContentSize().height));
                    m_multiText->setPosition(Vec2(m_bg->getContentSize().width/2, m_bg->getContentSize().height*0.58f));
                    m_multiText->setTextVerticalAlignment(TextVAlignment::CENTER);
                    m_multiText->setTextHorizontalAlignment(TextHAlignment::LEFT);
                    m_multiText->setColor(Color3B::BLACK);
                    m_bg->addChild(m_multiText);
                    auto rateTip = Text::create("个数根据场次确定。", FONT_TTF_MSYH, 26);
                    if (rateTip) {
                        rateTip->setTextAreaSize(Size(m_packetBombBG->getContentSize().width-33*2, m_packetBombBG->getContentSize().height));
                        rateTip->setPosition(Vec2(m_multiText->getPositionX(), m_multiText->getPositionY()-m_multiText->getContentSize().height*0.5f));
                        rateTip->setTextVerticalAlignment(TextVAlignment::CENTER);
                        rateTip->setTextHorizontalAlignment(TextHAlignment::LEFT);
                        rateTip->setColor(Color3B(138, 138, 138));
                        m_bg->addChild(rateTip);
                    }
                }

            }
            
        }
    }
    
    void RedPacketSendPacketLayer::closeLayer() {
        this->removeFromParent();
    }
    
    void RedPacketSendPacketLayer::backButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        closeLayer();
    }
    
//    void RedPacketSendPacketLayer::helpButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
//        if (type != Widget::TouchEventType::ENDED) {
//            return;
//        }
//    }
    
    void RedPacketSendPacketLayer::sendButtonCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        if (m_setPacketMoney < m_minMoney || m_setPacketMoney > m_maxMoney) {
            auto layer = GamePromptLayer::create();
            layer->showPrompt(GetText("SEND_PACKET_ERROR_5"));
            return;
        }
        if (m_setBombNumber < 0 || m_setBombNumber > 9) {
            auto layer = GamePromptLayer::create();
            layer->showPrompt(GetText("SEND_PACKET_ERROR_3"));
            return;
        }
        if (m_sendPacketCallBack) {
            m_sendPacketCallBack(m_setPacketMoney, m_setPacketPart, m_setBombNumber);
        }
        closeLayer();
    }
    
    void RedPacketSendPacketLayer::setBombCallBack(cocos2d::Ref *pSender, Widget::TouchEventType type) {
        if (type != Widget::TouchEventType::ENDED) {
            return;
        }
        auto setBomb = RedPacketSetPacketBombLayer::create();
        if (setBomb) {
            setBomb->openAddToParent(this, WIN_SIZE/2, m_setBombNumber);
        }
    }
    
}
