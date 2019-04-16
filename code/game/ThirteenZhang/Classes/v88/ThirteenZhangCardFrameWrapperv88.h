#ifndef __THIRTEENZHANG_CardFrameWrapperV88_H__
#define __THIRTEENZHANG_CardFrameWrapperV88_H__

//#include "cocos2d.h"
#include <string>
#include "ThirteenZhangWrapperv88.h"
//#include "HNNetExport.h"
#include "HNBaseType.h"
//USING_NS_CC;

namespace ThirteenZhang
{
    class CardFrameWrapperV88;
    
    class CardFrameWrapperV88 : public IWrapperV88
    {
    public:
        typedef std::function<void(CardFrameWrapperV88* cardFrame)> ccCardFrameClick;
        
    public:
        enum class GROUP
        {
            DEF,
            HEAD,
            MID,
            LAST
        };
        
    private:
        GROUP _group;
        bool _selected;
        BYTE _cardValue;
        Sprite*	_mask;
        ccCardFrameClick _callback;
        bool _enableTouch;
    public:
        CardFrameWrapperV88();
        virtual ~CardFrameWrapperV88();
        
    public:
        virtual bool load() override;
        virtual void unload() override;
        virtual void restore() override;
        
    public:
        void setValue(BYTE cardValue);
        
        BYTE getValue() const { return _cardValue; }
        
        bool isValid() const { return 0xFF != _cardValue && 0x00 != _cardValue; }
        
        void setSelected(bool selected);
        bool isSelected() const { return _selected; }
        
        void setGroup(GROUP group);
        GROUP getGroup() const { return _group; }
        
        void enableCardTouch(bool enableTouch);
        
    public:
        void setCardFrameClickEvent(const ccCardFrameClick& callback);
        
    protected:
        void onCardFrameClick(cocos2d::Ref *pSender);
        
    private:
        Sprite* createCard(int tag);
        void removeCard(int tag);
    };
}

#endif // !__THIRTEENZHANG_CardFrameWrapperV88_H__
