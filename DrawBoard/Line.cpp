#include "Line.h"

void Line::Draw(HDC hdc)
{
    MoveToEx(hdc, ptFrom.x, ptFrom.y, NULL);
    LineTo(hdc, ptTo.x, ptTo.y);
}


bool Line::isInside(int x, int y) {
    int x1 = ptFrom.x, y1 = ptFrom.y, x2 = ptTo.x, y2 = ptTo.y;
    double y0 = (y1 - y2) * (x - x1) / (double)(x1 - x2) + y1;
    double dy = y0 - y;
    if (std::abs(dy) < 2) return true;
    return false;
}

void Line::changePoints() {
    const POINT* points = getPoints();
    ptFrom = points[0];
    ptTo = points[1];
}