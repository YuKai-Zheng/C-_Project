#ifndef __BJLResultCardV88_h__
#define __BJLResultCardV88_h__

#include "HNNetExport.h"


#include "BJLMessageHead.h"
#include "BJLGameLogic.h"
#include "BJLPokerCardV88.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace cocostudio;
using namespace ui;
using namespace std;

namespace BJL
{
	class GameTableUIV88;

	class ResultCardV88
	{
	public:
		ResultCardV88(GameTableUIV88* gameTableUi);
		~ResultCardV88();

	public:
		bool dispatchCard();
		bool dispatchCardWithoutAnimation();
		void showCardBorad();
		void showCardBoradWithOutAnimation();
		//计算游戏牌型点数
		void calculatePoint(bool bPlay);
		//播结果
		void playResult();
		void hideCardBorad();

		// 获取数据
		void initCard(BYTE card[2][3], BYTE dealerCardNum, BYTE leisureCardNum,int dealerCardtype[5],int leisureCardtype[5] );
		void initNewGame();

		void playAnimation(int index, bool loop = false);
		bool isAnimationPlaying();

	public:
		BYTE getCardFileValue(const WORD& card);


	private:
		GameTableUIV88* _gameTableUi;
		Node* _csLoader;
		Node* _panel;
		PokerCardV88* _spriteCard[2][3];
		ImageView* background;
		//获取坐标点
		TextAtlas*	leisurePoint;
		TextAtlas*	dealerPoint;
		//胜负平image
		ImageView* leisure_win;
		ImageView* dealer_win;
		ImageView* dealer_leisure;

		//牌堆图片
		ImageView* cardsss;

		ImageView* cardTest[2];		//牌图片 ，用于播放动画时确定位子  手动没调好，  0.0

		cocostudio::timeline::ActionTimeline* _antAction;
	private:
		BYTE _cards[2][3];				   //牌值
		Vec2 destinationPositon[2][3];     //目标位置
		int _dealerCardtype[5];			   //庄家牌型
		int _leisureCardtype[5];           //闲家牌型
		BYTE    _bSendCardIndex;            //发牌索引
		BYTE _dealerCardNum;
		BYTE _leisureCardNum;
		LabelAtlas* spArr[2];
	private:
		void cardFlipCallback(Node* pNode);
		void dispatchCardAnimation(PokerCardV88* pokerCard , BYTE state);
		BYTE countTheAnswer();
	};
}

#endif // __BJLResultCardV88_h__
