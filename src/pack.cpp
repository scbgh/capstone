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
    delete data_;
}

//
//
PackEntry Pack::operator[](const std::string& key)
{
    return entries_[key];
}

//
//
bool Pack::contains(const std::string& key)
{
    return entries_.find(key) != entries_.end();
}

//
//
void Pack::LoadFromFile(const std::string& filename)
{
    char name[256];

    std::ifstream file(filename.c_str());
    file.seekg(0, std::ios::end);
    length_ = file.tellg();
    file.seekg(0, std::ios::beg);

    data_ = new char[length_];
    file.read(data_, length_);
    file.close();

    std::stringstream stream(data_);
    
    while (!stream.eof()) {
        PackEntry entry;

        stream.read(name, 256);
        stream.read((char *)&entry.length, 4);
        entry.data = data_ + stream.tellg();
        stream.seekg(entry.length, std::ios::cur);

        entries_[name] = entry;
    }
}

}