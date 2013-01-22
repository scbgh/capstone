//
// file.cpp
//

#include "common.h"
#include "file.h"

#define PATH_SEP "/"

#ifdef __APPLE__
#include <mach-o/dyld.h>
#elif UNIX
#include <unistd.h>
#elif WIN32
#include <windows.h>
#define PATH_SEP "\\"
#endif

namespace pg {

//
//
string GetExecutableDirectory()
{
    char dir[512];
#ifdef __APPLE__
    uint32_t bufsize = 512;
    _NSGetExecutablePath(dir, &bufsize);
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
