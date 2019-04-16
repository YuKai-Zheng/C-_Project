//
//  TigerScorePanelCellV88.cpp
//  MixProject
//
//  Created by 黄百超 on 11/03/2017.
//
//

#include "HonorScorePanelCellV88.h"
#include "cocostudio/CocoStudio.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "../LibHNUI/Tool/Tools.h"
#include "HNCommon/HNConverCode.h"
#include "HonorCommonDef.h"

namespace Honor {

    const static char* Basic_Cell_CSB = "csb/Games/CSLL_V66/ScorePanel_V66.csb";
    const static char* Cell_Icon_Res = "Games/31000509/type/%02d.png";//这个已经没用了

	HonorScorePanelCellV88::HonorScorePanelCellV88() {
    }

	HonorScorePanelCellV88::~HonorScorePanelCellV88() {
    }

	bool HonorScorePanelCellV88::init() {
        if (!Layer::init()) {
            return false;
        }

        initDefaultUI();
        return true;
	}

	void HonorScorePanelCellV88::initDefaultUI() {
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
			addChild(root);
		}
	}

	void HonorScorePanelCellV88::setCellValue(int type, int rate, int totalRate, int textType) {
		char name[128] = { 0 };
		_cell.rateText->setString("");
		sprintf(name, Cell_Icon_Res, type);
		_cell.icon->setTexture(name);
        char str[128] = {0};
        if (textType == PT_MONEY) {
            sprintf(str, GetText("NL_PT_MONEY"), rate, totalRate);
        }
        else if (textType == PT_FREE) {
            sprintf(str, GetText("NL_PT_MONEY"), rate, totalRate);
        }
        else if (textType == PT_JACKPOT) {
            sprintf(str, GetText("NL_PT_JACKPOT"), rate, totalRate);
        }
        _cell.rateText->setString(str);
	}
	
}



