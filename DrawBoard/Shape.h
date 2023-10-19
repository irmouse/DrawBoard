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

typedef struct ShapeRep
{
	ShapeType type;
	int pointNum;
	POINT points[1]; // ���賤��Ϊ1����������
} ShapeRep;

class Shape
{
public:
	Shape() {}
	Shape(const POINT *points);

	static Shape *CreateShape(ShapeType typeId);
	static Shape *CreateShape(ShapeType typeId, POINT *points);

	// �û��������ж�������߼������Դ˴�ʹ�����麯��
	// ���ڴ˴�������Ĭ��ʵ�֣�����û��ʹ�ô��麯��
	// �����߼�
	virtual void OnLButtonDown(int x, int y);
	virtual void OnLButtonMove(int x, int y); // up��move�߼���ͬһ��

	// �ƶ��߼�
	virtual void MoveBegin(int x, int y);
	virtual void MoveEnd(int x, int y); // �ƶ������к��ƶ����������øú���

	// �洢�߼�
	virtual bool ShapeToBuffer(char *buffer, int size);
	static size_t BufferToShape(char *buffer, Shape *&sp);

	virtual void Draw(HDC hdc) = 0;
	virtual bool isInside(int x, int y) = 0;

protected:
	virtual ShapeType returnType() = 0;
	virtual Shape *clone() = 0;
	virtual void changePoints() = 0;
	int pointNum()
	{
		return 2;
	}

	static void addPrototype(Shape *shape, ShapeType type)
	{
		_prototypes[type] = shape;
	}

	POINT getFromPoint()
	{
		return points[0];
	}

	POINT getToPoint()
	{
		return points[1];
	}

	const POINT *getPoints()
	{
		return points;
	}

	static size_t _id;
	// �������㣬ptFrom��ptTo
	POINT points[2];
	POINT ptMoveOrigin;

private:
	// ��buffer�еõ���ǰshape�м���point
	static int BufferToSize(char *buffer)
	{
		buffer += sizeof(ShapeType);
		int num;
		memcpy(&num, buffer, sizeof(int));
		return num;
	}

	// ����һ��shapeռ��buffer��С
	int bufferSize()
	{
		return sizeof(POINT) * pointNum() + sizeof(ShapeType) + sizeof(int);
	}

	static Shape *_prototypes[SHAPE_TYPE_NUM];
};
