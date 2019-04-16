//
//  BBGameStateV99.h
//  MixProject
//
//  Created by 黄百超 on 24/03/2017.
//
//

#ifndef BBGameStateV99_h
#define BBGameStateV99_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "BBCommonDef.h"

USING_NS_CC;

namespace BB {
    
    class BBGameStateV99 : public Layer {
        
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
        BBGameStateV99();
        ~BBGameStateV99();
        
        CREATE_FUNC(BBGameStateV99);
        
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

#endif /* BBGameStateV99_h */
