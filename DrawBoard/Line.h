#pragma once
#include<windows.h>
#include<cmath>
#include "Shape.h"
class Line : public Shape
{
public:
	void OnLButtonDown(int x, int y) {
		ptFrom.x = x;
		ptFrom.y = y;
	}
	void OnLButtonUp(int x, int y) {
		ptTo.x = x;
		ptTo.y = y;
	}
	void OnLButtonMove(int x, int y) {
		ptTo.x = x;
		ptTo.y = y;
	}
	void Draw(HDC hdc) {
		MoveToEx(hdc, ptFrom.x, ptFrom.y, NULL);
		LineTo(hdc, ptTo.x, ptTo.y);
	}

	bool isInside(int x, int y) {
		int x1 = ptFrom.x, y1 = ptFrom.y, x2 = ptTo.x, y2 = ptTo.y;
		double y0 = (y1 - y2) * (x - x1) / (double)(x1 - x2) + y1;
		double dy = y0 - y;
		if (std::abs(dy) < 2) return true;
		return false;
	}

	void MoveBegin(int x, int y) {
		
		ptMoveOrign.x = x;
		ptMoveOrign.y = y;
	}


	void MoveEnd(int x, int y) {
		int dx = x - ptMoveOrign.x;
		int dy = y - ptMoveOrign.y;
		ptFrom.x += dx;
		ptFrom.y += dy;
		ptTo.x += dx;
		ptTo.y += dy;
		ptMoveOrign.x = x;
		ptMoveOrign.y = y;
	}

	bool ShapeToBuffer(char* buffer, int size) {
		if (size < bufferSize()) return false;
		size_t skip = sizeof(ShapeType);
		ShapeType shapeType = returnType();
		memcpy(buffer, &shapeType, skip);
		buffer += skip;

		int pointN = pointNum();
		skip = sizeof(int);
		memcpy(buffer, &pointN, skip);
		buffer += skip;

		skip = sizeof(POINT);
		memcpy(buffer, &ptFrom, skip);
		buffer += skip;
		memcpy(buffer, &ptTo, skip);
		return true;
	}

	ShapeType returnType() {
		return LINE;
	}

	

	Shape* clone() {
		return new Line(1);
	}
protected:
	// dummy占位使用，目的是为了和无参构造区分
	Line(int dummy) {
		_id++;
	}

private:
	Line() {
		addPrototype(this, LINE);
	}

	int pointNum() {
		return 2;
	}

	void setPoints(POINT* points) {
		ptFrom = points[0];
		ptTo = points[1];
	}

	static Line _line;

	POINT ptFrom;
	POINT ptTo;
	POINT ptMoveOrign;
};



