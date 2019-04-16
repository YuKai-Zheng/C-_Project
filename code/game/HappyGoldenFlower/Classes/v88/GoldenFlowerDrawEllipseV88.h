#ifndef __GoldenFlowerDrawEllipse_V88_h__
#define __GoldenFlowerDrawEllipse_V88_h__

#include "cocos2d.h"
USING_NS_CC;

namespace goldenflower
{
	class DrawEllipseV88
	{
	public:
		DrawEllipseV88(Vec2 oValue, float xValue, float yValue);
		bool setEllipse(Vec2 oValue, float xValue, float yValue);

		bool isOnEllipse(Vec2 point);
		bool isInEllipse(Vec2 point);
		bool isOutEllipse(Vec2 point);

		float checkEllipse(Vec2 point);

		Vec2 getEllipseOrigin();
		float getEllipseX();
		float getEllipseY();

		DrawEllipseV88();
		~DrawEllipseV88();


	private:
		// the radius in x-axis
		float x;

		// the radius in y-axis
		float y;

		// the origin of the ellipse
		Vec2 o;
	};

}

#endif // __GoldenFlowerDrawEllipseV88_h__
