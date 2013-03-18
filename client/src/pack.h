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
//! A representation of a single file in a Pack
struct PackEntry {
    //! The length in bytes of this entry
    int length;
    //! The raw bytes of the data contained in this entry
    char *data;

    //! Returns a string representation of this entry
    std::string ToString() { return std::string(data, length); }
};

//
//! A representation of an archive file in memory
class Pack {
public:
    //! Create and load a pack file from disk
    //! \param filename The filename of the pack file to load
    explicit Pack(const std::string& filename);

    //! Access an entry in this pack
    //! \param key The name of the entry to access
    PackEntry operator[](const std::string& key);
    //! Access an entry in this pack
    //! \param key The name of the entry to access
    PackEntry get(const std::string& key);
    //! Returns true if the specified entry exists in the pack
    //! \param key The name of the entry to look up
    bool contains(const std::string& key) const;
    //! The number of entries in the pack
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
