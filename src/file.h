//
// file.h
// File handling functionality.
//

#ifndef _FILE_H_
#define _FILE_H_

#include <string>

namespace pg {

std::string GetExecutableDirectory();
std::string AppendPath(const std::string& path1, const std::string& path2);

}

#endif
