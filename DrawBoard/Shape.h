#pragma once
#include <windows.h>
#include <assert.h>
const int SHAPE_TYPE_NUM = 10;

enum ShapeType
{
	LINE,
	TRIANGLE,
	ELLIPSE,
	NUM
};


class Shape
{
public:
	Shape() {}

	static Shape *CreateShape(ShapeType typeId);
	static Shape *CreateShape(ShapeType typeId, POINT *points);

	// 用户可以自行定义绘制逻辑，所以此处使用了虚函数
	// 由于此处给出了默认实现，所以没有使用纯虚函数
	// 绘制逻辑
	virtual void OnLButtonDown(int x, int y);
	virtual void OnLButtonMove(int x, int y); // up和move逻辑用同一个

	// 移动逻辑
	virtual void MoveBegin(int x, int y);
	virtual void MoveEnd(int x, int y); // 移动过程中和移动结束都调用该函数

	// 存储逻辑
	virtual bool ShapeToBuffer(char *buffer, int size);
	static size_t BufferToShape(char *buffer, Shape *&sp);

	virtual void Draw(HDC hdc) = 0;
	virtual bool isInside(int x, int y) = 0;

	// 返回一个shape占用buffer大小
	int bufferSize()
	{
		return sizeof(POINT) * pointNum() + sizeof(ShapeType) + sizeof(int);
	}

protected:
	virtual ShapeType returnType() = 0;
	virtual Shape *clone() = 0;
	virtual void changePoints() = 0;

	static void addPrototype(Shape *shape, ShapeType type)
	{
		_prototypes[type] = shape;
	}

	const POINT *getPoints()
	{
		return points;
	}

	static size_t _id;
	

private:
	// 从buffer中得到当前shape有几个point
	static int BufferToSize(char *buffer)
	{
		buffer += sizeof(ShapeType);
		int num;
		memcpy(&num, buffer, sizeof(int));
		return num;
	}

	

	int pointNum()
	{
		return 2;
	}

	static Shape *_prototypes[SHAPE_TYPE_NUM];
	// 两个顶点，ptFrom和ptTo
	POINT points[2];
	POINT ptMoveOrigin;
};
