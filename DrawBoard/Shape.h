#pragma once
#include <windows.h>
#include <assert.h>
const int SHAPE_TYPE_NUM = 10;

enum ShapeType{
	LINE, TRIANGLE, ELLIPSE, NUM
};

typedef struct ShapeRep {
	ShapeType type;
	int pointNum;
	POINT points[1];  // 假设长度为1，后面会添加
}ShapeRep;

class Shape
{
public:
	static Shape* CreateShape(ShapeType typeId) {
		if (typeId < ShapeType::NUM) {
			return _prototypes[typeId]->clone();
		}
		else {
			return nullptr;
		}
	}

	static Shape* CreateShape(ShapeType typeId, POINT* points) {
		if (typeId < ShapeType::NUM) {
			Shape* sp = _prototypes[typeId]->clone();
			sp->setPoints(points);
			return sp;
		}
		else {
			return nullptr;
		}
	}

	virtual void OnLButtonDown(int x, int y) = 0;
	virtual void OnLButtonUp(int x, int y) = 0;
	virtual void OnLButtonMove(int x, int y) = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual bool isInside(int x, int y) = 0;
	virtual void MoveBegin(int x, int y) = 0;
	virtual void MoveEnd(int x, int y) = 0;
	// virtual bool ShapeToBuffer(char* buffer, int size) = 0;
	// virtual ShapeRep* ShapeToRep() = 0;
	ShapeRep* ShapeToRep(ShapeType type, int point_num, POINT* points) {
		ShapeRep* rep = (ShapeRep*)malloc(sizeof(ShapeRep) + sizeof(POINT) * (point_num - 1));
		rep->pointNum = point_num;
		rep->type = type;
		for (int i = 0; i < point_num; i++) {
			rep->points[i] = points[i];
		}

		return rep;
	}

	
	// 将rep写入到buffer中，buffer大小为size
	bool ShapeToBuffer(char* buffer, int size, ShapeRep* rep) {
		if (size < bufferSize()) return false;
		assert(rep);

		size_t skip = sizeof(ShapeType);
		ShapeType shapeType = rep->type;
		memcpy(buffer, &shapeType, skip);
		buffer += skip;

		int pointN = rep->pointNum;
		skip = sizeof(int);
		memcpy(buffer, &pointN, skip);
		buffer += skip;

		skip = sizeof(POINT);
		for (int i = 0; i < pointN; i++) {
			memcpy(buffer, &rep->points[i], skip);
			buffer += skip;
		}
		return true;
	}
	// 从buffer中得到当前shape有几个point
	static int BufferToSize(char* buffer) {
		buffer += sizeof(ShapeType);
		int num;
		memcpy(&num, buffer, sizeof(int));
		return num;
	}
	// 返回一个shape占用buffer大小
	static size_t BufferToShape(char* buffer, Shape* & sp) {
		char* p = buffer;
		ShapeType shapeType = *((ShapeType*)p);
		p += sizeof(ShapeType);
		int pointN = *((int*)p);
		p += sizeof(int);
		POINT* points = new POINT[pointN];
		memcpy(points, p, sizeof(POINT) * pointN);
		p += sizeof(POINT) * pointN;
		sp = CreateShape(shapeType, points);
		delete[] points;
		return p - buffer;

	}
	int bufferSize() {
		return sizeof(POINT) * pointNum() + sizeof(ShapeType) + sizeof(int);
	}


protected:
	virtual ShapeType returnType() = 0;
	virtual Shape* clone() = 0;
	virtual int pointNum() = 0;
	virtual void setPoints(POINT* points) = 0;
	
	static void addPrototype(Shape* shape, ShapeType type) {
		_prototypes[type] = shape;
	}

	static size_t _id;

private:

	static Shape* _prototypes[SHAPE_TYPE_NUM];
};


