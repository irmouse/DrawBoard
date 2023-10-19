#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <list>
#include "Shape.h"


#define FILE_SUFFIX "PTC"
extern Shape *shape;
extern std::list<Shape *> shapes;
void writeToFile(const wchar_t *filename);
void readFromFile(const wchar_t *filename);