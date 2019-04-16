//
//  FruitGameStateV88.h
//  MixProject
//
//  Created by 黄百超 on 24/03/2017.
//
//

#ifndef FruitGameStateV88_h
#define FruitGameStateV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "FruitCommonDef.h"

USING_NS_CC;

namespace Fruit {
    
    class FruitGameStateV88 : public Layer {
        
        //UI Struct
        struct StateGirlLayer {
            Node* stateNode;
            ImageView* girlBg;
            ImageView* stateBg;
            ImageView* state;
            StateGirlLayer() {
                stateNode = nullptr;
                girlBg = nullptr;
                stateBg = nullptr;
                state = nullptr;
            }
        } _stateGirl;
        
        struct StateLabel {
            Node* stateLabelNode;
            ImageView* bg;
            ImageView* stateLabelContent;
            Text* stateContent;
            StateLabel() {
                stateLabelNode = nullptr;
                bg = nullptr;
                stateLabelContent = nullptr;
                stateContent = nullptr;
            }
        } _stateContent;
        
        int _gameState;
        
    public:
        FruitGameStateV88();
        ~FruitGameStateV88();
        
        CREATE_FUNC(FruitGameStateV88);
        
        virtual bool  init() override;
        
        
        void updateGirlState(int state);
        void updateGameStateContent(int stateID);
        void hideStateLayer();
//        void setNodeVisible(bool isShow);
        
        void setStateGirlPosition(Vec2 pos);
        void setStateLabelPosition(Vec2 pos);
        
    private:
        void initValues();
        void initStateLayer();
//        void showStateLayer();
        
    };
    
}

#endif /* FruitGameStateV88_h */
