#include "ThirteenZhangCardFrameWrapperv88.h"
#include "UI/Base/HNLayer.h"

namespace ThirteenZhang
{
#define CARD_SPRITE_TAG	100
    
    static std::string getCardTextureFileName(BYTE cardValue)
    {
        char filename[64];
        if(PRESSED_TEXTURE){
            sprintf(filename, "v66/common/card_AMax/%02x.png", cardValue);
        }else{
        sprintf(filename, "%02x.png", cardValue);
            }
        return filename;
    }
    
    CardFrameWrapperV88::CardFrameWrapperV88()
    : _callback(nullptr)
    , _mask(nullptr)
    , _cardValue(0xFF)
    , _selected(false)
    , _enableTouch(false)
    {
        
    }
    
	CardFrameWrapperV88::~CardFrameWrapperV88()
    {
        
    }
    
	bool CardFrameWrapperV88::load()
    {
        _mask = Sprite::create("Games/ThirteenZhang/game/common/card_mask.png");
        _mask->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        _mask->setPosition(getContentSize().width / 2, getContentSize().height / 2);
        addChild(_mask, 2);
        
        _mask->setVisible(false);
        
        return true;
    }
    
	void CardFrameWrapperV88::unload()
    {
        
    }
    
	void CardFrameWrapperV88::restore()
    {
        setValue(0xFF);
        setSelected(false);
    }
    
	void CardFrameWrapperV88::enableCardTouch(bool enableTouch)
    {
        _enableTouch = enableTouch;
        if (_enableTouch)
        {
            getComponent<ImageView>()->addClickEventListener(CC_CALLBACK_1(CardFrameWrapperV88::onCardFrameClick, this));
        }
        else
        {
            getComponent<ImageView>()->addClickEventListener(nullptr);
        }
    }
    
    void CardFrameWrapperV88::setValue(BYTE cardValue)
    {
        if (_cardValue != cardValue)
        {
            removeCard(CARD_SPRITE_TAG);
            
            _cardValue = cardValue;
            
            if (_cardValue != 0xFF)
            {
                createCard(CARD_SPRITE_TAG);
            }
        }
    }
    
    void CardFrameWrapperV88::setCardFrameClickEvent(const ccCardFrameClick& callback)
    {
        _callback = callback;
    }
    
    void CardFrameWrapperV88::setSelected(bool selected)
    {
        if (_selected != selected)
        {
            _selected = selected;
        }
        _mask->setVisible(_selected);
    }
    
    void CardFrameWrapperV88::setGroup(GROUP group)
    {
        if (_group != group)
        {
            _group = group;
        }
    }
    
    void CardFrameWrapperV88::onCardFrameClick(cocos2d::Ref *pSender)
    {
        _selected = true;
        
        if (_selected)
        {
            _mask->setVisible(true);
        }
        
        if (nullptr != _callback)
        {
            _callback(this);
        }
    }
    
    Sprite* CardFrameWrapperV88::createCard(int tag)
    {
        auto cardSprite = Sprite::create();
        cardSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        cardSprite->setPosition(getContentSize().width / 2, getContentSize().height / 2);
        cardSprite->setContentSize(getContentSize());
        cardSprite->setScale(0.9f);
        addChild(cardSprite, 1, tag);
        
        cardSprite->setSpriteFrame(getCardTextureFileName(_cardValue));
        
        return cardSprite;
    }
    
    void CardFrameWrapperV88::removeCard(int tag)
    {
        removeChild(tag); 
    }
}
