//
//  TZhangCardTypeV88Cell.h
//  MixProject
//
//  Created by 黄百超 on 22/02/2017.
//
//

#ifndef TZhangCardTypeCell_V88_h
#define TZhangCardTypeCell_V88_h

//#include "HNUIExport.h"
//#include "cocos2d.h"
#include "UI/Base/HNLayer.h"
#include "HNBaseType.h"
#include "cocostudio/CocoStudio.h"

//USING_NS_CC;

namespace ThirteenZhang {
    class TZhangCardTypeV88 : public Layer {
        
	public:   struct TypeCell {
            Button*     bg;
			Sprite*  type0;
			Sprite*  type1;
			Sprite*  type2;
			Sprite*  type3;
            
            TypeCell() {
                bg = nullptr;
                type0 = nullptr;
                type1 = nullptr;
                type2 = nullptr;
                type3 = nullptr;
            }
        } _typeCell;
        
    public:
        CREATE_FUNC(TZhangCardTypeV88);
        
        TZhangCardTypeV88();
        ~TZhangCardTypeV88();
        
        virtual bool init() override;
        
        
        void setTagValue(int tag);
        void setSwallowTouches(bool swallow);
        void addTouchEventListener(const Widget::ccWidgetTouchCallback& callback);
//        bool onTouchBegan(Touch* touch, Event* event);
//        void onTouchEnd(Touch* touch, Event* event);
        
    public:
        void updateThreeTypes(int idx, BYTE types[3]);
        
        Size getSize();
        
    private:
        void initValues();
        void loadUI();
        
        void updateCellBg(int idx);
        void updateCellContents(BYTE types[3]);
        
    private:
        BYTE types[3];  //三墩牌的类型
        std::pair<std::string, std::string> cardTypeResource;
        
    };
}

#endif /* TZhangCardTypeV88Cell_h */
