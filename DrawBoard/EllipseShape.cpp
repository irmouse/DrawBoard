#include "EllipseShape.h"

void EllipseShape::Draw(HDC hdc)
{
    Ellipse(hdc, ptFrom.x, ptFrom.y, ptTo.x, ptTo.y);
}

bool EllipseShape::isInside(int x, int y)
{
    float x0 = (ptFrom.x + ptTo.x) / 2.0;
    float y0 = (ptFrom.y + ptTo.y) / 2.0;
    x -= x0;
    y -= y0;
    float a = std::abs(ptTo.x - ptFrom.x) / 2.0;
    float b = std::abs(ptTo.y - ptFrom.y) / 2.0;
    if (a >= b)
    {
        if (std::pow(x / a, 2.0) + std::pow(y / b, 2.0) <= 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (std::pow(y / a, 2.0) + std::pow(x / a, 2.0) <= 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void EllipseShape::changePoints()
{
    const POINT *p = getPoints();
    ptFrom = p[0];
    ptTo = p[1];
}