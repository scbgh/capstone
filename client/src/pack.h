//
// pack.h
// Handles the game data in a packaged archive.
//

#ifndef _PACK_H_
#define _PACK_H_

#include <string>
#include <map>
#include <memory>

namespace pg {

//
// A representation of a single file in a Pack.
struct PackEntry {
    int length;
    char *data;

    std::string ToString() { return std::string(data, length); }
};

//
// A representation of an archive file in memory.
class Pack {
public:
    Pack(const std::string& filename);

    PackEntry operator[](const std::string& key);
    PackEntry get(const std::string& key);
    bool contains(const std::string& key) const;
    int count() const;

private:
    int length_;
    std::unique_ptr<char[]> data_;
    std::map<std::string, PackEntry> entries_;

    // Load a pack from a file
    void LoadFromFile(const std::string& filename);

    // Non-copyable
    Pack(const Pack&);
    Pack& operator=(const Pack&);
};


}

#endif