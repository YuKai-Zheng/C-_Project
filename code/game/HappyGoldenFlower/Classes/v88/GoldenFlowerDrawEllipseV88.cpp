#include "GoldenFlowerDrawEllipseV88.h"

namespace goldenflower
{
	DrawEllipseV88::DrawEllipseV88()
	{
		o = Vec2(0, 0);
		x = 2;
		y = 1;
	}


	DrawEllipseV88::~DrawEllipseV88()
	{
	}


	DrawEllipseV88::DrawEllipseV88(Vec2 oValue, float xValue, float yValue)
	{
		o = oValue;
		x = xValue;
		y = yValue;
	}

	bool DrawEllipseV88::setEllipse(Vec2 oValue, float xValue, float yValue)
	{
		if (xValue < 0 || yValue < 0)
		{
			return false;
		}

		o = oValue;
		x = xValue;
		y = yValue;

		return true;
	}

	bool DrawEllipseV88::isOnEllipse(Vec2 point)
	{
		return checkEllipse(point) == 0 ? true : false;
	}
	bool DrawEllipseV88::isInEllipse(Vec2 point)
	{
		return checkEllipse(point) < 0 ? true : false;
	}
	bool DrawEllipseV88::isOutEllipse(Vec2 point)
	{
		return checkEllipse(point) > 0 ? true : false;
	}

	float DrawEllipseV88::checkEllipse(Vec2 point)
	{
		float a = point.x - o.x;
		float b = point.y - o.y;

		return (a*a) / (x*x) + (b*b) / (y*y) - 1;
	}

	Vec2 DrawEllipseV88::getEllipseOrigin()
	{
		return o;
	}
	float DrawEllipseV88::getEllipseX()
	{
		return x;
	}
	float DrawEllipseV88::getEllipseY()
	{
		return y;
	}
}

