//
// file.cpp
//

#include "common.h"
#include "file.h"

#define PATH_SEP "/"

#if defined __APPLE__
#include <mach-o/dyld.h>
#elif defined UNIX
#include <unistd.h>
#elif defined WIN32
#include <windows.h>
#undef PATH_SEP
#define PATH_SEP "\\"
#endif

namespace pg {

//
//
string GetExecutableDirectory()
{
    char dir[512];
#if defined __APPLE__
    uint32_t bufsize = 512;
    _NSGetExecutablePath(dir, &bufsize);
#elif defined WIN32
    HMODULE module = GetModuleHandleA(NULL);
    GetModuleFileNameA(module, dir, 512);
#else
#error "GetExecutableDirectory not implemented on this platform"
#endif

    int idx = strlen(dir);
    while (idx >= 0) {
        if (dir[idx] == '/' || dir[idx] == '\\') {
            dir[idx] = 0;
            break;
        }
        idx--;
    }

    return string(dir);
}

//
//
string AppendPath(const string& path1, const string& path2)
{
    string path = path1;
    char ch = path[path.length() - 1];
    if (ch != '/' || ch != '\\') {
        path += PATH_SEP;
    }
    path += path2;
    return path;
}

}
