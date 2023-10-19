#pragma once
#include "Shape.h"
class Triangle : public Shape
{
public:
	void Draw(HDC hdc) override;

	bool isInside(int x, int y) override;

	ShapeType returnType() override
	{
		return TRIANGLE;
	}
	void changePoints() override;

	Shape *clone() override
	{
		return new Triangle(1);
	}

protected:
	Triangle(int dummy)
	{
		_id++;
	}

private:
	Triangle()
	{
		addPrototype(this, TRIANGLE);
	}

	static Triangle _triangle;
	POINT points[3];
};
