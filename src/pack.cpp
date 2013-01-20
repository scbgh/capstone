//
// pack.cpp
//

#include <fstream>
#include <sstream>
#include "pack.h"

namespace pg {

//
//
Pack::Pack(const std::string& filename)
{
    LoadFromFile(filename);
}

//
//
Pack::~Pack()
{
    delete data;
}

//
//
PackEntry Pack::operator[](const std::string& key)
{
    return entries[key];
}

//
//
bool Pack::contains(const std::string& key)
{
    return entries.find(key) != entries.end();
}

//
//
void Pack::LoadFromFile(const std::string& filename)
{
    char name[256];

    std::ifstream file(filename.c_str());
    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);

    data = new char[length];
    file.read(data, length);
    file.close();

    std::stringstream stream(data);
    
    while (!stream.eof()) {
        PackEntry entry;

        stream.read(name, 256);
        stream.read((char *)&entry.length, 4);
        entry.data = data + stream.tellg();
        stream.seekg(entry.length, std::ios::cur);

        entries[name] = entry;
    }
}

}