#pragma once
#include "Shape.h"
#include <cmath>
class EllipseShape : public Shape
{
public:
	void Draw(HDC hdc) override;

	bool isInside(int x, int y) override;

	void changePoints() override;

	ShapeType returnType() override
	{
		return ELLIPSE;
	}

	Shape *clone() override
	{
		return new EllipseShape(1);
	}

protected:
	EllipseShape(int dummy)
	{
		_id++;
	}
	void setPoints(POINT *points)
	{
		ptFrom = points[0];
		ptTo = points[1];
	}

private:
	EllipseShape()
	{
		addPrototype(this, ELLIPSE);
	}

	static EllipseShape _ellipse;

	POINT ptFrom;
	POINT ptTo;
};
