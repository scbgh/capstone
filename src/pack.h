//
// pack.h
// Handles the game data in a packaged archive.
//

#include <string>
#include <map>

namespace pg {

//
// struct PackEntry
// A representation of a single file in a Pack.
//
struct PackEntry {
    int length;
    char *data;
};

//
// class Pack
// A representation of an archive file in memory.
//
class Pack {
public:
    Pack(const std::string& filename);
    ~Pack();

    PackEntry operator[](const std::string& key);
    bool contains(const std::string& key);

private:
    int length;
    char *data;
    std::map<std::string, PackEntry> entries;

    // Load a pack from a file
    void LoadFromFile(const std::string& filename);

    // Non-copyable
    Pack(const Pack&);
    Pack& operator=(const Pack&);
};


}