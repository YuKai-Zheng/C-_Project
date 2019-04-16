
#ifndef  __BRXJPOCKERV88_H__
#define __BRXJPOCKERV88_H__

#include "HNNetExport.h"
using namespace ui;

namespace BRXJ
{
#define POKER_CARD_NUM			54
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

	class PockerV88 : public ImageView
	{
	public:

		static PockerV88* create(Vec2 position, float scal, Vec2 targetPos, float angle, BYTE value);
	private:

		PockerV88();
		~PockerV88();

		bool init(Vec2 position, float scal, Vec2 targetPo,float angle,BYTE value);
		std::string Transformation(int num);
	public:

		void setCardValue(BYTE valu);
		void setTargetPos(Vec2 pos);
		void moveTagertPos();

	private:

		BYTE		_value;
		Vec2		_pos;
		Vec2		_targetPos;
		float		_angle;
		float		_scal;
	};
}

#endif //__BRXJPOCKERV88_H__