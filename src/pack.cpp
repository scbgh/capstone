//
// pack.cpp
//

#include <fstream>
#include <sstream>
#include "common.h"
#include "pack.h"

namespace pg {

//
//
Pack::Pack(const std::string& filename)
{
    Debug("Loading pack file '%s'...", filename.c_str());
    LoadFromFile(filename);
    Debug("Pack file loaded");
}

//
//
PackEntry Pack::operator[](const std::string& key)
{
    return entries_[key];
}

//
//
PackEntry Pack::get(const std::string& key)
{
    return operator[](key);
}

//
//
bool Pack::contains(const std::string& key) const
{
    return entries_.find(key) != entries_.end();
}

//
//
int Pack::count() const
{
    return entries_.size();
}

//
//
void Pack::LoadFromFile(const std::string& filename)
{
    char name[256];

    std::ifstream file(filename.c_str(), std::ios::binary | std::ios::in);
    if (file.fail()) {
        Die("Failed to open pack file '%s'", filename.c_str());
    }

    file.seekg(0, std::ios::end);
    length_ = file.tellg();
    file.seekg(0, std::ios::beg);

    data_ = unique_ptr<char>(new char[length_]);
    file.read(data_.get(), length_);
    file.close();

    std::istringstream stream(string(data_.get(), length_));

    while (stream.good() && stream.tellg() < length_) {
        PackEntry entry;

        stream.read(name, 256);
        stream.read((char *)&entry.length, 4);
        entry.data = data_.get() + stream.tellg();
        stream.seekg(entry.length, std::ios::cur);

        entries_[name] = entry;
    }
}

}