#ifndef __ThirteenZhang_Action_Board_V88_H__
#define __ThirteenZhang_Action_Board_V88_H__


//#include "cocos2d.h"
#include <string>
#include <queue>
//#include "HNNetExport.h"
#include "UI/Base/HNLayer.h"
#include "HNBaseType.h"
//USING_NS_CC;

namespace ThirteenZhang
{
    class CardSpriteV88;
    
    class CardBoardV88: public HN::HNLayer
    {
    public:
        static CardBoardV88* create(bool self);
        
    public:
        virtual bool init(bool self);
        
    public:
        void sendCardOneByOne(const std::vector<BYTE>& vtcards, float interval);
        void sendCard(const std::vector<BYTE>& cards);
        void removeCard(const std::vector<BYTE>& cards);
        void setMultSelected(bool multiSelect);
        
    public:
        std::vector<BYTE> getCards();
        std::vector<BYTE> getUpCards();
        std::vector<Vec2> getUpCardsPosition();
        unsigned int getCardSize() const;
        
        bool empty() const;
        void clear();
        void enableCardTouch(bool enableTouch);
        
        void downCards();
        void upCards(const std::vector<BYTE>& cards);
        
    protected:
        CardBoardV88();
        virtual ~CardBoardV88();
        
    private:
        bool onTouchBegan(Touch* touch, Event *event);
        void onTouchMoved(Touch* touch, Event *event);
        void onTouchEnded(Touch* touch, Event *event);
        
    private:
        void scheduleSendCardStart(float dt);
        
    private:
        void addCard(BYTE cardValue);
        void removeCard(BYTE cardValue);
        void resizeCardBoard();
        void touchCheck(const Vec2& pos);
        void touchDragCheck(const Vec2& startPos, const Vec2& endPos);
        void sortCard();
        void upCard(CardSpriteV88* card);
		void downCard(CardSpriteV88* card);
        void swtichUpDown();
        void updateDragState(float delta);
        
    private:
        std::vector<BYTE> searchCards(const std::string& cond);
        
    private:
        std::queue<BYTE> _tmpCardValues;
        std::vector<CardSpriteV88*> _pokers;
        bool _self;
        bool _multiSelect;
        bool _enableCardTouch;
        bool _isDrag;
        Vec2 _startPos;
        Vec2 _endPos;
    };
}




#endif
