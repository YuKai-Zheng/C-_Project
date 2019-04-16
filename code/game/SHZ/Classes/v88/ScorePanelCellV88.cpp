//
//  BBLoopSpriteCellV88.cpp
//  MixProject
//
//  Created by 黄百超 on 11/03/2017.
//
//

#include "ScorePanelCellV88.h"
#include "cocostudio/CocoStudio.h"
#include "SHZCommonDef.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "../LibHNUI/Tool/Tools.h"
#include "HNCommon/HNConverCode.h"

namespace SHZ {

    const static char* Basic_Cell_CSB = "Games/31000503/ScorePanel_V88.csb";
    const static char* Cell_Icon_Res = "Games/31000503/shuihuzhuang/kind_%d.png";


	ScorePanelCellV88::ScorePanelCellV88() {

}

	ScorePanelCellV88::~ScorePanelCellV88() {

}

	bool ScorePanelCellV88::init() {
    if (!Layer::init()) {
        return false;
    }

    initDefaultUI();
        return true;
	}

	void ScorePanelCellV88::initDefaultUI() {
		auto root = CSLoader::createNode(Basic_Cell_CSB);
		if (root) {
			_cell.spriteNode = root;
			auto light = dynamic_cast<Sprite*>(root->getChildByName("typeImg"));
			if (light) {
				_cell.icon = light;
			}
			auto textRate = dynamic_cast<Text*>(root->getChildByName("itemRate"));
			if (textRate) {
				_cell.rateText = textRate;
			}
			addChild(root,100);
		}
	}

	void ScorePanelCellV88::setCellValue(int type, int rate, int totalRate) {
		char name[128] = { 0 };
		_cell.rateText->setString("");
		sprintf(name, Cell_Icon_Res, type);
		_cell.icon->setTexture(name);
		char text1[128] = { 0 };
		sprintf(text1, "×%d=%d倍", rate, totalRate);
		_cell.rateText->setString(ForceGBKToUtf8(text1));

	}
}



