//
//  BBNoteLabelCellV99.h
//  MixProject
//
//  Created by 黄百超 on 27/03/2017.
//
//

#ifndef BBNoteLabelCellV99_h
#define BBNoteLabelCellV99_h

#include "UI/base/HNLayer.h"
#include "cocos2d.h"
#include "BBCommonDef.h"

USING_NS_CC;

namespace BB {
    
    class BBNoteLabelCellV99 : public Layer {
        
        //UI Struct
        struct Cell {
            Node* cellNode;
            Text* myNote;
            Text* allNote;
            Cell() {
                cellNode = nullptr;
                myNote = nullptr;
                allNote = nullptr;
            }
        } _cell;
        
    public:
        BBNoteLabelCellV99();
        ~BBNoteLabelCellV99();
        
        CREATE_FUNC(BBNoteLabelCellV99);
        
        virtual bool init() override;
        
        void setNotePosition(Vec2 pos);
        void updateNotes(long long myMoney, long long allMoney);
        void clearNotes();
        
    private:
        void initCell();
        
    };
    
}

#endif /* BBNoteLabelCellV99_h */
