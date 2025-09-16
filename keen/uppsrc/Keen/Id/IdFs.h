// Minimal filesystem helpers for the Keen port
#pragma once

#include <cstddef>
#include <cstdio>

#include "IdMm.h"

typedef std::FILE* FS_File;

// C++ wrapper API
namespace FS {
class File {
public:
    File() noexcept;
    explicit File(FS_File f) noexcept;
    File(const File&) = delete;
    File& operator=(const File&) = delete;
    File(File&& other) noexcept;
    File& operator=(File&& other) noexcept;
    ~File();

    static File OpenUser(const char* filename);
    static File CreateUser(const char* filename);

    bool IsValid() const noexcept;
    std::size_t GetSize() const;
    std::size_t Read(void* ptr, std::size_t size, std::size_t nmemb);
    std::size_t Write(const void* ptr, std::size_t size, std::size_t nmemb);
    int PrintF(const char* fmt, ...);
    void Close();

    FS_File GetHandle() const noexcept { return f_; }

private:
    FS_File f_;
};

// Helper: load entire user file into heap buffer via MM_GetPtr.
bool ReadAllUser(const char* filename, mm_ptr_t* out_ptr, int* out_size);

// Binary readers/writers using little-endian serialization
class Reader {
public:
    explicit Reader(File& f) : file(f) {}
    std::size_t Read(void* ptr, std::size_t size, std::size_t nmemb) { return file.Read(ptr, size, nmemb); }
    std::size_t ReadInt8(uint8_t* ptr, std::size_t count);
    std::size_t ReadInt16LE(uint16_t* ptr, std::size_t count);
    std::size_t ReadInt32LE(uint32_t* ptr, std::size_t count);
    std::size_t ReadBool16LE(bool* ptr, std::size_t count);
private:
    File& file;
};

class Writer {
public:
    explicit Writer(File& f) : file(f) {}
    std::size_t Write(const void* ptr, std::size_t size, std::size_t nmemb) { return file.Write(ptr, size, nmemb); }
    std::size_t WriteInt8(const uint8_t* ptr, std::size_t count);
    std::size_t WriteInt16LE(const uint16_t* ptr, std::size_t count);
    std::size_t WriteInt32LE(const uint32_t* ptr, std::size_t count);
    std::size_t WriteBool16LE(const bool* ptr, std::size_t count);
private:
    File& file;
};
}
