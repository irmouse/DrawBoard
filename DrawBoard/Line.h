#pragma once
#include <windows.h>
#include <cmath>
#include "Shape.h"
class Line : public Shape
{
public:
	void Draw(HDC hdc) override;

	bool isInside(int x, int y) override;

	void changePoints() override;

	ShapeType returnType() override
	{
		return LINE;
	}

	Shape *clone() override
	{
		return new Line(1);
	}

protected:
	// dummyռλʹ�ã�Ŀ����Ϊ�˺��޲ι�������
	Line(int dummy)
	{
		_id++;
	}

private:
	Line()
	{
		addPrototype(this, LINE);
	}

	static Line _line;

	POINT ptFrom;
	POINT ptTo;
};
