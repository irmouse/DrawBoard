#include <windows.h>
#include <windowsx.h>
// #include <winuser.h>
#include <vector>
#include <list>
#include <cstring>
#include <shobjidl.h>

// °´Å¥ID
#define IDB_ONE 3301
#define IDB_TWO 3302
#define IDB_THREE 3303
#define IDB_FOUR 3304

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_SAVE 3
#define IDM_FILE_QUIT 4

void FileOpenWindow(wchar_t **filePath);
void FileSaveWindow(wchar_t **filePath);

void AddMenus(HWND);
void AddButtons(HWND);