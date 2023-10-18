#pragma once
#include "Shape.h"
class Triangle : public Shape {
public:
	void OnLButtonDown(int x, int y) {
		ptFrom.x = x;
		ptFrom.y = y;
	}
	void OnLButtonUp(int x, 
		int y) {
		ptTo.x = x;
		ptTo.y = y;
		setPoints();
	}
	void OnLButtonMove(int x, int y) {
		ptTo.x = x;
		ptTo.y = y;
		setPoints();
	}
	void Draw(HDC hdc) {
		Polygon(hdc, points, 3);
	}

	bool isInside(int x, int y) {
		float a = (-(x - points[1].x)*(points[2].y - points[1].y) + (y - points[1].y)*(points[2].x - points[1].x)) 
			/ (float)(-(points[0].x - points[1].x) * (points[2].y - points[1].y) + (points[0].y - points[1].y) * (points[2].x - points[1].x));
		float b = (-(x - points[2].x) * (points[0].y - points[2].y) + (y - points[2].y) * (points[0].x - points[2].x)
			/ (float)(-(points[1].x - points[2].x) * (points[0].y - points[2].y) + (points[1].y - points[2].y) * (points[0].x - points[2].x)));
		float c = 1 - a - b;
		return a >= 0 && b >= 0 && c >= 0;
	}

	ShapeType returnType() {
		return TRIANGLE;
	}

	void MoveBegin(int x, int y) {
		ptMoveOrign.x = x;
		ptMoveOrign.y = y;
	}

	void MoveEnd(int x, int y) {
		int dx = x - ptMoveOrign.x;
		int dy = y - ptMoveOrign.y;
		for (int i = 0; i < 3; i++) {
			points[i].x += dx;
			points[i].y += dy;
		}
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
		for (int i = 0; i < 3; i++) {
			memcpy(buffer, &points[i], skip);
			buffer += skip;
		}
		return true;
	}

	Shape* clone() {
		return new Triangle(1);
	}

protected:
	Triangle(int dummy) {
		_id++;
	}
private:
	Triangle() {
		addPrototype(this, TRIANGLE);
	}

	int pointNum() {
		return 3;
	}


	void setPoints() {
		if (ptTo.y > ptFrom.y) {
			points[0] = { ptFrom.x, ptTo.y };
			points[1] = { (ptTo.x + ptFrom.x) / 2, ptFrom.y };
			points[2] = { ptTo.x, ptTo.y };
		}
		else {
			points[0] = { ptFrom.x, ptFrom.y };
			points[1] = { (ptTo.x + ptFrom.x) / 2, ptTo.y };
			points[2] = { ptTo.x, ptFrom.y };
		}
	}

	void setPoints(POINT* points) {
		for (int i = 0; i < 3; i++) {
			this->points[i] = points[i];
		}
	}

	static Triangle _triangle;

	POINT ptFrom;
	POINT ptTo;
	POINT points[3];
	POINT ptMoveOrign;
};

