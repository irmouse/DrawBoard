#include "Triangle.h"

void Triangle::Draw(HDC hdc)
{
    Polygon(hdc, points, 3);
}

bool Triangle::isInside(int x, int y)
{
    float a = (-(x - points[1].x) * (points[2].y - points[1].y) + (y - points[1].y) * (points[2].x - points[1].x)) / (float)(-(points[0].x - points[1].x) * (points[2].y - points[1].y) + (points[0].y - points[1].y) * (points[2].x - points[1].x));
    float b = (-(x - points[2].x) * (points[0].y - points[2].y) + (y - points[2].y) * (points[0].x - points[2].x) / (float)(-(points[1].x - points[2].x) * (points[0].y - points[2].y) + (points[1].y - points[2].y) * (points[0].x - points[2].x)));
    float c = 1 - a - b;
    return a >= 0 && b >= 0 && c >= 0;
}

void Triangle::changePoints()
{
    const POINT *p = getPoints();
    POINT ptTo = p[1], ptFrom = p[0];
    if (ptTo.y > ptFrom.y)
    {
        points[0] = {ptFrom.x, ptTo.y};
        points[1] = {(ptTo.x + ptFrom.x) / 2, ptFrom.y};
        points[2] = {ptTo.x, ptTo.y};
    }
    else
    {
        points[0] = {ptFrom.x, ptFrom.y};
        points[1] = {(ptTo.x + ptFrom.x) / 2, ptTo.y};
        points[2] = {ptTo.x, ptFrom.y};
    }
}