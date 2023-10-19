#include "Shape.h"

Shape *Shape::_prototypes[SHAPE_TYPE_NUM];
size_t Shape::_id;



Shape *Shape::CreateShape(ShapeType typeId)
{
    if (typeId < ShapeType::NUM)
    {
        return _prototypes[typeId]->clone();
    }
    else
    {
        return nullptr;
    }
}

Shape *Shape::CreateShape(ShapeType typeId, POINT *points)
{
    if (typeId < ShapeType::NUM)
    {
        Shape *sp = _prototypes[typeId]->clone();
        for (int i = 0; i < 2; i++)
        {
            sp->points[i].x = points[i].x;
            sp->points[i].y = points[i].y;
        }
        sp->changePoints();
        return sp;
    }
    else
    {
        return nullptr;
    }
}

void Shape::OnLButtonDown(int x, int y)
{
    points[0].x = x;
    points[0].y = y;
}

void Shape::OnLButtonMove(int x, int y)
{
    points[1].x = x;
    points[1].y = y;
}

void Shape::MoveBegin(int x, int y)
{
    ptMoveOrigin.x = x;
    ptMoveOrigin.y = y;
}

void Shape::MoveEnd(int x, int y)
{
    int dx = x - ptMoveOrigin.x;
    int dy = y - ptMoveOrigin.y;
    for (int i = 0; i < 2; i++)
    {
        points[i].x += dx;
        points[i].y += dy;
    }
    ptMoveOrigin.x = x;
    ptMoveOrigin.y = y;
}

size_t Shape::BufferToShape(char *buffer, Shape *&sp)
{
    char *p = buffer;
    ShapeType shapeType = *((ShapeType *)p);
    p += sizeof(ShapeType);
    int pointN = *((int *)p);
    p += sizeof(int);
    POINT *points = new POINT[pointN];
    memcpy(points, p, sizeof(POINT) * pointN);
    p += sizeof(POINT) * pointN;
    sp = CreateShape(shapeType, points);
    delete[] points;
    return p - buffer;
}

bool Shape::ShapeToBuffer(char *buffer, int size)
{
    assert(size >= bufferSize());
    size_t skip = sizeof(ShapeType);
    ShapeType shapeType = returnType();
    memcpy(buffer, &shapeType, skip);
    buffer += skip;

    int pointN = 2;
    skip = sizeof(int);
    memcpy(buffer, &pointN, skip);
    buffer += skip;

    skip = sizeof(POINT);
    for (int i = 0; i < pointN; i++)
    {
        memcpy(buffer, &points[i], skip);
        buffer += skip;
    }
    return true;
}