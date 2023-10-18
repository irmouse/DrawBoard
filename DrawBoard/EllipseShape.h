#pragma once
#include "Shape.h"
#include <cmath>
class EllipseShape : public Shape {
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
		
		Ellipse(hdc, ptFrom.x, ptFrom.y, ptTo.x, ptTo.y);
	}

	bool isInside(int x, int y) {
		float x0 = (ptFrom.x + ptTo.x) / 2.0;
		float y0 = (ptFrom.y + ptTo.y) / 2.0;
		x -= x0; // 将椭圆重心坐标移动到坐标原点
		y -= y0; 
		float a = std::abs(ptTo.x - ptFrom.x) / 2.0;
		float b = std::abs(ptTo.y - ptFrom.y) / 2.0;
		if (a >= b) {
			if (std::pow(x / a, 2.0) + std::pow(y / b, 2.0) <= 1) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (std::pow(y / a, 2.0) + std::pow(x / a, 2.0) <= 1) {
				return true;
			}
			else {
				return false;
			}
		}
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

	ShapeType returnType() {
		return ELLIPSE;
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

	Shape* clone() {
		return new EllipseShape(1);
	}

protected:
	EllipseShape(int dummy) {
		_id++;
	}
	void setPoints(POINT* points) {
		ptFrom = points[0];
		ptTo = points[1];
	}
private:
	EllipseShape() {
		addPrototype(this, ELLIPSE);
	}

	int pointNum() {
		return 2;
	}

	

	static EllipseShape _ellipse;

	POINT ptFrom;
	POINT ptTo;
	POINT ptMoveOrign;
};

