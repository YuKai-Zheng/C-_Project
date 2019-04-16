//
//  TZhangCardControlV88.h
//  MixProject
//
//  Created by 黄百超 on 22/02/2017.
//
//

#ifndef TZhangCardControlv88_h
#define TZhangCardControlv88_h

//#include "HNUIExport.h"
#include "UI/Base/HNLayer.h"
//#include "cocos2d.h"
//#include "ui/CocosGUI.h"
#include "GUI/CCScrollView/CCScrollView.h"
#include "TZhangCardTypeCellv88.h"
#include "ThirteenZhangGameLogic.h"
#include "ThirteenZhangCardFrameWrapperv88.h"
#include "UI/Base/AUIButton.h"
//USING_NS_CC;
//using namespace cocos2d::ui;

namespace ThirteenZhang {
    
    class CardSpriteV88;
    class GameTableLogic;
    
    class TZhangCardControlV88 :public HNLayer/*, public ScrollViewDelegate*/ {
        
        struct CardContainer
        {
            int     iHeapShape[3];          //玩家每一堆牌的类型 前面的是尾墩，数组末尾的是首墩
            int     iHeapCount[3];          //玩家每一堆牌的数量
            BYTE    byHeapCard[3][5];       //玩家每一堆牌的数据
            CardContainer() {
                memset(iHeapShape, 0, sizeof(iHeapShape));
                memset(iHeapCount, 0, sizeof(iHeapCount));
                memset(byHeapCard, 0, sizeof(byHeapCard));
            }
        };
        
        struct CardType
        {
            int firstType;      //尾墩
            int secondType;
            int thirdType;      //首墩
            CardType() {
                memset(this, 0, sizeof(this));
            }
        };
        
        struct PokerData
        {
            int dunType;
            BYTE card;
            PokerData() {
                memset(this, 0, sizeof(this));
            }
        };

        struct SwapPokerData {
            int startTag;
            int endTag;
            bool canSwap;
            SwapPokerData() {
                memset(this, 0, sizeof(this));
            }
        } _swapPokerData;
        
        static const unsigned int CARD_FRAME_COUNT = 13;
        
        
    public:
        TZhangCardControlV88();
        ~TZhangCardControlV88();
        
        CREATE_FUNC(TZhangCardControlV88);
        
        virtual bool init() override;
        
//        void scrollViewDidScroll(ScrollView* view);
//        void scrollViewDidZoom(ScrollView* view);
        
    public:
        void loadCards(const std::vector<BYTE>& cards);
        void setLogic(GameTableLogic* logic);
//        std::function<void(TZhangCardControlV88* sender, Vec2 touchVec)> onClickEvent;
//        
//    protected:
//        bool onTouchBegan(Touch* touch, Event* event);
//        void onTouchEnd(Touch* touch, Event* event);
        
        
        
    private:
        void recheckCardType();
        
        void loadPokers();
        void updatePokers(int selectedCards);
        
        void swapTwoPokers(int startCard, int endCard);
        
    private:
        void initValues();
        void loadUI();
        void loadListener();
        
        void addCells();
        void showCellsInView();
        
        int onCheckingCardShape(BYTE resultCards[], int & resultCount, int startShape = SHAPE_STRAIGHT_FLUSH);
        
        void pokerListener(Ref* sender, Widget::TouchEventType type);
        void outCardCallBack(Ref* pSender, Widget::TouchEventType type);
        
        bool checkDaoShui();
        
    private:
        
        std::vector<BYTE> _cards;               //所有原始牌数据
        std::vector<BYTE> _operateCards;        //所有操作牌数据
        std::vector<CardContainer> _cardsArray; //牌的所有排列方式
        std::vector<CardType> _cardsType;       //三墩的排列组合数据
        
        BYTE _outCardValues[3][5];              //左侧三墩摆牌数据
//        std::vector<ImageView*> _outPokers;     //左侧的13张扑克牌
        
//        CardFrameWrapper* _arrCardFrame[CARD_FRAME_COUNT];
        
        int _selectedCardsArray;                //选中的牌的排列方式，默认为0，点击选中哪个后会更新
        std::vector<CardSpriteV88*> _pokers;       //所有扑克牌
//        Vec2 _pokersConstPos[3][5];
        std::vector<Vec2> _pokersConstPos;
        
        std::vector<TZhangCardTypeV88*> _cells;    //牌数据最多六种组合
        cocos2d::ui::ScrollView* _scrollView;
        Node* _cardOperate;
        
        AUIButton* _outCardButton;
        
        GameTableLogic* _logic;
        
        void cardTypeCellCallBack(Ref* pSender, Widget::TouchEventType type);
        
    };
}

#endif /* TZhangCardControlV88_h */
