#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <windowsx.h>
// #include <winuser.h>
#include <vector>
#include <list>
#include <cstring>
#include <shobjidl.h>
#include "Shape.h"
#include "Line.h"
#include "Triangle.h"
#include "EllipseShape.h"
#include "FileAPI.h"
#include "UIwindow.h"

#include <iostream>
using namespace std;

#define WIN_WIDTH 800
#define WIN_HEIGHT 500

enum DrawMode
{
    DRAW,
    MOVE
};

// 添加到原型
Line Line::_line;
Triangle Triangle::_triangle;
EllipseShape EllipseShape::_ellipse;

BOOL fDraw = FALSE;
POINT ptPrevious;
POINT ptCurrent;
ShapeType currentShape = LINE; // 当前绘制的图形
DrawMode currentMode = DRAW;   // 绘制模式/移动模式
Shape *shape = nullptr;        // 当前绘制的图形对象
std::list<Shape *> shapes;     // 当前绘制的列表
wchar_t *filePath = NULL;      // 用于保存文件

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PWSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    WNDCLASSW wc = {0};
    wc.lpszClassName = L"插件式画板";
    wc.hInstance = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc = WndProc;
    wc.hCursor = LoadCursor(0, IDC_ARROW);

    RegisterClassW(&wc);
    HWND m_hwnd = CreateWindowW(wc.lpszClassName, L"插件式画板",
                                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                400, 250, WIN_WIDTH, WIN_HEIGHT, 0, 0, hInstance, 0);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
                         WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    int x = LOWORD(lParam), y = HIWORD(lParam);
    char *a = NULL;

    switch (msg)
    {

    case WM_CREATE:

        AddMenus(hwnd);
        AddButtons(hwnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDM_FILE_NEW:
            FileSaveWindow(&filePath);
            break;
        case IDM_FILE_OPEN:
            FileOpenWindow(&filePath);
            readFromFile(filePath);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case IDM_FILE_SAVE:
            if (filePath == NULL)
                FileSaveWindow(&filePath);
            writeToFile(filePath);
            break;
        case IDB_ONE:
            currentMode = DRAW;
            currentShape = TRIANGLE;
            break;
        case IDB_TWO:
            currentMode = DRAW;
            currentShape = ELLIPSE;
            break;
        case IDB_THREE:
            currentMode = DRAW;
            currentShape = LINE;
            break;
        case IDB_FOUR:
            currentMode = MOVE;
            break;
        case IDM_FILE_QUIT:

            SendMessage(hwnd, WM_CLOSE, 0, 0);
            break;
        }
        break;

    case WM_PAINT:

        hdc = BeginPaint(hwnd, &ps);
        for (auto _shape : shapes)
        {
            _shape->Draw(hdc);
        }
        if (shape)
            shape->Draw(hdc);
        EndPaint(hwnd, &ps);
        break;

    case WM_LBUTTONDOWN:
        switch (currentMode)
        {
        case DRAW:
            if (!shape)
            {
                shape = Shape::CreateShape(currentShape);
                shape->OnLButtonDown(x, y);
            }
            break;
        case MOVE:
            std::list<Shape *>::reverse_iterator iter = shapes.rbegin();
            for (; iter != shapes.rend(); iter++)
            {
                if ((*iter)->isInside(x, y))
                {
                    shape = *iter;
                    shapes.erase((++iter).base());
                    shapes.push_back(shape);
                    shape->MoveBegin(x, y);
                    break;
                }
            }

            break;
        }

        break;

    case WM_LBUTTONUP:
        switch (currentMode)
        {
        case DRAW:
            if (shape)
            {
                InvalidateRect(hwnd, NULL, TRUE);
                shape->OnLButtonMove(x, y);
                shapes.push_back(shape);
                shape = nullptr;
            }
            break;
        case MOVE:
            if (shape)
            {
                InvalidateRect(hwnd, NULL, TRUE);
                shape->MoveEnd(x, y);
                shape = nullptr;
            }
            break;
        }
        break;

    case WM_MOUSEMOVE:
        switch (currentMode)
        {
        case DRAW:
            if (shape)
            {
                InvalidateRect(hwnd, NULL, TRUE);
                shape->OnLButtonMove(x, y);
            }
            break;
        case MOVE:
            if (shape)
            {
                InvalidateRect(hwnd, NULL, TRUE);
                shape->MoveEnd(x, y);
            }
            break;
        }
        break;

    case WM_DESTROY:

        PostQuitMessage(0);
        break;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}
