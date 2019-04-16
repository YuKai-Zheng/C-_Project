

#include "FruitBetMoneyCellV88.h"
#include "cocostudio/ActionTimeline/CSLoader.h"
#include "Tool/Tools.h"
#include "HNLobby/v66/PlatformResV66.h"

namespace Fruit {

	const static char* Cell_CSB = "Games/31000502/Node_Bet_Money_V66.csb";

	const static char* BETFONT_V88 = "Games/31000502/shuiguoji/23.png";

	FruitBetMoneyCellV88::FruitBetMoneyCellV88() {

	}

	FruitBetMoneyCellV88::~FruitBetMoneyCellV88() {

	}

	bool FruitBetMoneyCellV88::init() {
		if (!Layer::init()) {
			return false;
		}
		initCell();
		return true;  
	}


	void FruitBetMoneyCellV88::addBetMonye(int money)
	{
		string addMoney = StringUtils::format("%.2d", money);
		if (_cell.moneyLb)
		{
			_cell.moneyLb->setString(addMoney);
		}
	}


	void FruitBetMoneyCellV88::initCell() {
		auto node = CSLoader::createNode(Cell_CSB);
		if (node) {
			_cell.cellNode = node;
			addChild(node);
			auto _tLabel = Text::create("", FONT_TTF_FZCY, 26);
			_tLabel->setName("moneyLb");
			_tLabel->setAnchorPoint(Vec2(0.5, 0.5));
			_tLabel->setPosition(Vec2(0, 2));
			_cell.moneyLb = _tLabel;
			addChild(_tLabel);
		}
	}
	

}
