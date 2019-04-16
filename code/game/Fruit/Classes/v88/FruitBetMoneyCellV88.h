//
//  FruitBetMoneyCellV88_h.h
//  MixProject
//
//  Created by 黄百超 on 27/03/2017.
//
//

#ifndef FruitBetMoneyCellV88_h
#define FruitBetMoneyCellV88_h

#include "UI/base/HNLayer.h"
#include "cocos2d.h"
#include "FruitCommonDef.h"

USING_NS_CC;

namespace Fruit{

	class FruitBetMoneyCellV88 : public Layer {
	public:
		FruitBetMoneyCellV88();
		~FruitBetMoneyCellV88();

		CREATE_FUNC(FruitBetMoneyCellV88);
		virtual bool init() override;



		struct Cell {
			Node* cellNode;
			Text* moneyLb;
			Cell() {
				cellNode = nullptr;
				moneyLb = nullptr;
			}
		} _cell;


	private:
		void initCell();
	public:
		void addBetMonye(int money);

	public :
		Node cell;
};

}

#endif /* FruitBetMoneyCellV88_h */
