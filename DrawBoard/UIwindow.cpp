#include "UIwindow.h"
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
        L"BUTTON",                                                // Predefined class; Unicode assumed
        L"三角形",                                                // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,    // Styles
        0,                                                        // x position
        0,                                                        // y position
        60,                                                       // Button width
        30,                                                       // Button height
        hwnd,                                                     // Parent window
        (HMENU)IDB_ONE,                                           // 按钮没有菜单，但是可以用作按钮的id
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL); // Pointer not needed.

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

void FileOpenWindow(wchar_t **filePath)
{
    // CoInitializeEx初始化COM库
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
                                          COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr))
    {
        IFileOpenDialog *pFileOpen = NULL;

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