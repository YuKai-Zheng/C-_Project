//
//  BBNoteLabelCellV99.cpp
//  MixProject
//
//  Created by 黄百超 on 27/03/2017.
//
//

#include "BBNoteLabelCellV99.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Tool/Tools.h"

namespace BB {
    
    const static char* Cell_CSB = "Games/31000501/Node_NoteLabelCell.csb";
    
    BBNoteLabelCellV99::BBNoteLabelCellV99() {
        
    }
    
    BBNoteLabelCellV99::~BBNoteLabelCellV99() {
        
    }
    
    bool BBNoteLabelCellV99::init() {
        if (!Layer::init()) {
            return false;
        }
        initCell();
        return true;
    }
    
    void BBNoteLabelCellV99::setNotePosition(cocos2d::Vec2 pos) {
        if (_cell.cellNode) {
            _cell.cellNode->setPosition(pos);
        }
    }
    
    void BBNoteLabelCellV99::updateNotes(long long myMoney, long long allMoney) {
        if (myMoney < 0) {
            myMoney = 0;
        }
        this->setVisible(true);
        if (_cell.cellNode) {
            _cell.cellNode->setVisible(true);
        }
//        ostringstream ss;
        if (_cell.myNote) {
//            ss.str("");
//            ss << myMoney;
            _cell.myNote->setString(Tools::addComma(myMoney));
        }
        if (_cell.allNote) {
//            ss.str("");
//            ss << allMoney;
//            _cell.allNote->setString(ss.str().c_str());
            _cell.allNote->setString(Tools::addComma(allMoney));
        }
    }
    
    void BBNoteLabelCellV99::clearNotes() {
        this->setVisible(false);
        if (_cell.cellNode) {
            _cell.cellNode->setVisible(false);
        }
        if (_cell.myNote) {
            _cell.myNote->setString("");
        }
        if (_cell.allNote) {
            _cell.allNote->setString("");
        }
    }
    
    void BBNoteLabelCellV99::initCell() {
        auto node = CSLoader::createNode(Cell_CSB);
        if (node) {
            _cell.cellNode = node;
            addChild(node);
            //
            auto my = dynamic_cast<Text*>(node->getChildByName("my"));
            if (my) {
                _cell.myNote = my;
                _cell.myNote->setString("0");
            }
            auto all = dynamic_cast<Text*>(node->getChildByName("all"));
            if (all) {
                _cell.allNote = all;
                _cell.allNote->setString("0");
            }
        }
    }
    
}
