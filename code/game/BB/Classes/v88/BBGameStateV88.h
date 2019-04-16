//
//  BBGameStateV88.h
//  MixProject
//
//  Created by 黄百超 on 24/03/2017.
//
//

#ifndef BBGameStateV88_h
#define BBGameStateV88_h

#include "UI/Base/HNLayer.h"
#include "cocos2d.h"
#include "BBCommonDef.h"



USING_NS_CC;

namespace BB {
    
    class BBGameStateV88 : public Layer {
        
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
            ImageView* stateLabel;
            ImageView* stateLabelContent;
			Text* stateContent;
            StateLabel() {
                stateLabelNode = nullptr;
                stateLabel = nullptr;
                stateLabelContent = nullptr;
            }
        } _stateContent;
        
        int _gameState;
        
    public:
        BBGameStateV88();
        ~BBGameStateV88();
        
        CREATE_FUNC(BBGameStateV88);
        
        virtual bool  init() override;
        
        
		void updateGirlState(int state);
        void hideStateLayer();
//        void setNodeVisible(bool isShow);
        
        void setStateGirlPosition(Vec2 pos);
        void setStateLabelPosition(Vec2 pos);
		void updateGameStateContent(int stateID);
        
    private:
        void initValues();
        void initStateLayer();
//        void showStateLayer();
        
    };
    
}

#endif /* BBGameStateV88_h */
