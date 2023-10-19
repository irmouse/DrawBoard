#include "FileAPI.h"


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