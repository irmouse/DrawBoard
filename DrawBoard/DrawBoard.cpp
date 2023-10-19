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

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_SAVE 3
#define IDM_FILE_QUIT 4
#define WIN_WIDTH 800
#define WIN_HEIGHT 500

// 按钮ID
#define IDB_ONE 3301
#define IDB_TWO 3302
#define IDB_THREE 3303
#define IDB_FOUR 3304

#define FILE_SUFFIX "PTC"

enum DrawMode
{
    DRAW,
    MOVE
};

Line Line::_line;
Triangle Triangle::_triangle;
EllipseShape EllipseShape::_ellipse;

BOOL fDraw = FALSE;
POINT ptPrevious;
POINT ptCurrent;
ShapeType currentShape = LINE;
DrawMode currentMode = DRAW;
Shape *shape = nullptr;
// std::vector<Shape*> shapes;
std::list<Shape *> shapes;
wchar_t *filePath = NULL; // for save file

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void FileOpenWindow(wchar_t **filePath);
void FileSaveWindow(wchar_t **filePath);
void AddMenus(HWND);
void AddButtons(HWND);

void writeToFile(const wchar_t *filename);
void readFromFile(const wchar_t *filename);

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
            writeToFile(filePath); // ppd
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
                shape->OnLButtonMove(LOWORD(lParam), HIWORD(lParam));
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

void AddMenus(HWND hwnd)
{
    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_SAVE, L"&Save");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL); // 分隔线
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
    SetMenu(hwnd, hMenubar);
}

void AddButtons(HWND hwnd)
{
    CreateWindow(
        L"BUTTON",                                             // Predefined class; Unicode assumed
        L"三角形",                                             // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        0,                                                     // x position
        0,                                                     // y position
        60,                                                    // Button width
        30,                                                    // Button height
        hwnd,                                                  // Parent window
        (HMENU)IDB_ONE,                                        // 按钮没有菜单，但是可以用作按钮的id
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

    CreateWindow(
        L"BUTTON",                                             // Predefined class; Unicode assumed
        L"椭圆形",                                             // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        60,                                                    // x position
        0,                                                     // y position
        60,                                                    // Button width
        30,                                                    // Button height
        hwnd,                                                  // Parent window
        (HMENU)IDB_TWO,                                        // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

    CreateWindow(
        L"BUTTON",                                             // Predefined class; Unicode assumed
        L"直线",                                               // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        120,                                                   // x position
        0,                                                     // y position
        60,                                                    // Button width
        30,                                                    // Button height
        hwnd,                                                  // Parent window
        (HMENU)IDB_THREE,                                      // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.
    CreateWindow(
        L"BUTTON",                                             // Predefined class; Unicode assumed
        L"选择",                                               // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        180,                                                   // x position
        0,                                                     // y position
        60,                                                    // Button width
        30,                                                    // Button height
        hwnd,                                                  // Parent window
        (HMENU)IDB_FOUR,                                       // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.
}

size_t shapeTotalSize()
{
    size_t size = 0;
    for (Shape *sp : shapes)
    {
        size += sp->bufferSize();
    }
    return size;
}

void writeToFile(const wchar_t *filename)
{
    char buffer[512];
    char *bfptr = buffer;
    size_t buffersize = 512;
    for (Shape *sp : shapes)
    {
        size_t skip = sp->bufferSize();
        if (!sp->ShapeToBuffer(bfptr, buffersize))
            break;
        bfptr += skip;
        buffersize -= skip;
    }
    FILE *fp = _wfopen(filename, L"w");
    fwrite(FILE_SUFFIX, 4, 1, fp);
    size_t shapeSize = shapes.size();
    fwrite(&shapeSize, sizeof(size_t), 1, fp);
    fwrite(buffer, shapeTotalSize(), 1, fp);
    fclose(fp);
}

void readFromFile(const wchar_t *filename)
{
    if (!shapes.empty())
    {
        for (auto sp : shapes)
        {
            delete sp;
        }
        shapes.clear();
    }

    FILE *fp = _wfopen(filename, L"r");
    char buffer[512];
    fread(buffer, 4, 1, fp);
    if (strcmp(FILE_SUFFIX, buffer) != 0)
    {
        fclose(fp);
        return;
    }
    size_t shapeNum = 0;
    if (fread(&shapeNum, sizeof(size_t), 1, fp))
    {
        fread(buffer, 512, 1, fp);
        char *p = buffer;
        for (int i = 0; i < shapeNum; i++)
        {
            Shape *sp;
            size_t skip = Shape::BufferToShape(p, sp);
            p += skip;
            shapes.push_back(sp);
        }
    }

    fclose(fp);
}

void FileOpenWindow(wchar_t **filePath)
{
    // CoInitializeEx初始化COM库
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                                          COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;

        // Create the FileOpenDialog object.
        // 创建Common Item Dialog对象，并获取对象IFileOpenDialog接口的指针
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                              IID_IFileOpenDialog, reinterpret_cast<void **>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            // Show the Open dialog box.
            // 向用户显示对话框
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                // 调用GetResult函数，获得指向shell item object的指针，它实现了IShellItem接口，代表了用户选择的文件
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    // 获取文件路径
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, filePath);
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        // 可以取消初始化COM库
        CoUninitialize();
    }
}

void FileSaveWindow(wchar_t **filePath)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                                          COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen;
        IFileSaveDialog *pFileSave;

        IShellItem *ist = NULL;
        hr = SHCreateItemFromParsingName(L"D:\\Data\\QQ data\\743544510\\FileRecv", NULL, IID_PPV_ARGS(&ist));
        if (!SUCCEEDED(hr))
            return;

        // Create the FileOpenDialog object.
        hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                              IID_IFileSaveDialog, reinterpret_cast<void **>(&pFileSave));

        if (SUCCEEDED(hr))
        {
            COMDLG_FILTERSPEC rgSpec[] =
                {
                    {L"ptc", L"*.ptc"},
                    {L"all", L"*.*"},
                };
            hr = pFileSave->SetFileTypes(2, rgSpec);
            if (!SUCCEEDED(hr))
                return;

            hr = pFileSave->SetDefaultFolder(ist);
            if (!SUCCEEDED(hr))
                return;

            hr = pFileSave->SetDefaultExtension(L"ptc"); // 默认添加扩展名
            if (!SUCCEEDED(hr))
                return;

            // Show the Open dialog box.
            hr = pFileSave->Show(NULL);

            // Get the file name from the dialog box.
            if (SUCCEEDED(hr))
            {
                IShellItem *pItem;
                hr = pFileSave->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, filePath);
                    pItem->Release();
                }
            }
            pFileSave->Release();
        }
        CoUninitialize();
    }
}