//
//  TZhangCardTypeV88Cell.cpp
//  MixProject
//
//  Created by 黄百超 on 22/02/2017.
//
//

#include "TZhangCardTypeCellv88.h"
#include "ThirteenZhangGameLogic.h"
#include "ThirteenZhangResourceMgrv88.h"
#include "cocostudio/CocoStudio.h"

namespace ThirteenZhang {

const static char* Cell_Layer_CSB = "Games/12100004/CardControlCell.csb";

TZhangCardTypeV88::TZhangCardTypeV88() {
//        this = Button::create();
}

TZhangCardTypeV88::~TZhangCardTypeV88() {

}

bool TZhangCardTypeV88::init() {
    if (!Layer::init()) {
        return false;
    }

    initValues();
    loadUI();


    return true;
}

void TZhangCardTypeV88::setTagValue(int tag) {
    if (_typeCell.bg) {
        _typeCell.bg->setTag(tag);
    }
}

void TZhangCardTypeV88::setSwallowTouches(bool swallow) {
    if (_typeCell.bg) {
        _typeCell.bg->setSwallowTouches(swallow);
    }
}

void TZhangCardTypeV88::addTouchEventListener(const Widget::ccWidgetTouchCallback &callback) {
    if (_typeCell.bg) {
        _typeCell.bg->addTouchEventListener(callback);
    }
}

//根据类型更新资源
void TZhangCardTypeV88::updateThreeTypes(int idx, BYTE types[3]) {
    if (_typeCell.bg) {
        updateCellBg(idx);
        updateCellContents(types);
    }
}

Size TZhangCardTypeV88::getSize() {
    if (_typeCell.bg) {
        return _typeCell.bg->getContentSize();
    }
    return Size(299, 64);
}

void TZhangCardTypeV88::initValues() {
    memset(types, 0, sizeof(BYTE));
}

void TZhangCardTypeV88::loadUI() {
    auto root = CSLoader::createNode(Cell_Layer_CSB);
    if (root) {
        addChild(root);
        Button* bg = dynamic_cast<Button*>(root->getChildByName("Button"));
        if (bg) {
            _typeCell.bg = bg;
            Sprite* sp0 = dynamic_cast<Sprite*>(bg->getChildByName("Type0"));
            if (sp0) {
                _typeCell.type0 = sp0;
            }
			Sprite* sp1 = dynamic_cast<Sprite*>(bg->getChildByName("Type1"));
            if (sp1) {
                _typeCell.type1 = sp1;
            }
			Sprite* sp2 = dynamic_cast<Sprite*>(bg->getChildByName("Type2"));
            if (sp2) {
                _typeCell.type2 = sp2;
            }

			Sprite* grayImg = dynamic_cast<Sprite*>(bg->getChildByName("Image_2"));
            if (grayImg) {
                grayImg->setName("Type3");
                _typeCell.type3 = grayImg;
                _typeCell.type3->setVisible(false);
            }
        }
    }

}

void TZhangCardTypeV88::updateCellBg(int idx) {
    if (idx < 0 || idx > 2) {
        return;
    }
    char name[100];
    sprintf(name, "Games/12100004/13zhang/13S/cell%d.png", idx);
    if (_typeCell.bg) {
        _typeCell.bg->loadTextures(name, name);
    }
}

void TZhangCardTypeV88::updateCellContents(BYTE types[3]) {
    for (int i = 0; i < 3; i++) {
        if (types[i] < SHAPE_HIGH_CARD || types[i] > SHAPE_STRAIGHT_FLUSH) {
            return;
        }
        //std::pair<std::string, std::string> cardTypeResource = ResourceMgrV88::getCardTypeResource(true, types[i], false);
        std::pair<std::string, std::string> cardTypeResource = ResourceMgrV88::getCardTypeResource(true, types[i]);
        if (i == 0) {
            if (_typeCell.type0) {
                _typeCell.type0->setTexture(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
                //_typeCell.type0->ignoreContentAdaptWithSize(true);
            }
        } else if (i == 1) {
            if (_typeCell.type1) {
                _typeCell.type1->setTexture(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
                //_typeCell.type1->ignoreContentAdaptWithSize(true);
            }
        } else if (i == 2) {
            if (_typeCell.type2) {
                _typeCell.type2->setTexture(ResourceMgrV88::getTypeTextureFileName(cardTypeResource));
                //_typeCell.type2->ignoreContentAdaptWithSize(true);
            }
        }
    }
}

}
