#include "Id.h"


NAMESPACE_UPP

// --------- C++ wrapper (namespace FS) ----------

namespace FS {

File::File() noexcept : f_(nullptr) {}

File::File(FS_File f) noexcept : f_(f) {}

File::File(File&& other) noexcept : f_(other.f_) {
    other.f_ = nullptr;
}

File& File::operator=(File&& other) noexcept {
    if (this != &other) {
        Close();
        f_ = other.f_;
        other.f_ = nullptr;
    }
    return *this;
}

File::~File() {
    Close();
}

File File::OpenUser(const char* filename) { return File(std::fopen(filename, "rb")); }

File File::CreateUser(const char* filename) { return File(std::fopen(filename, "wb")); }

bool File::IsValid() const noexcept { return f_ != nullptr; }

std::size_t File::GetSize() const {
    if (!f_) return 0;
    long pos = std::ftell(f_);
    if (pos < 0) return 0;
    std::fseek(f_, 0, SEEK_END);
    long size = std::ftell(f_);
    std::fseek(f_, pos, SEEK_SET);
    if (size < 0) return 0;
    return (std::size_t)size;
}

std::size_t File::Read(void* ptr, std::size_t size, std::size_t nmemb) {
    if (!f_) return 0;
    return std::fread(ptr, size, nmemb, f_);
}

std::size_t File::Write(const void* ptr, std::size_t size, std::size_t nmemb) {
    if (!f_) return 0;
    return std::fwrite(ptr, size, nmemb, f_);
}

int File::PrintF(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = f_ ? std::vfprintf(f_, fmt, args) : 0;
    va_end(args);
    return ret;
}

void File::Close() {
    if (f_) {
        std::fclose(f_);
        f_ = nullptr;
    }
}

bool ReadAllUser(const char* filename, mm_ptr_t* out_ptr, int* out_size) {
    if (!out_ptr || !out_size) return false;
    File f = File::OpenUser(filename);
    if (!f.IsValid()) {
        *out_ptr = nullptr;
        *out_size = 0;
        return false;
    }
    std::size_t length = f.GetSize();
    MM_GetPtr(out_ptr, (int)length);
    if (!*out_ptr) {
        f.Close();
        *out_size = 0;
        return false;
    }
    *out_size = (int)length;
    std::size_t amountRead = f.Read(*out_ptr, 1, length);
    f.Close();
    if (amountRead != length) {
        MM_FreePtr(out_ptr);
        return false;
    }
    return true;
}

static inline uint16_t MakeLE16(uint8_t b0, uint8_t b1) {
    return (uint16_t)((uint16_t)b0 | ((uint16_t)b1 << 8));
}
static inline uint32_t MakeLE32(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
    return (uint32_t)((uint32_t)b0 | ((uint32_t)b1 << 8) | ((uint32_t)b2 << 16) | ((uint32_t)b3 << 24));
}

std::size_t Reader::ReadInt8(uint8_t* ptr, std::size_t count) {
    return file.Read(ptr, 1, count);
}

std::size_t Reader::ReadInt16LE(uint16_t* ptr, std::size_t count) {
    std::size_t done = 0;
    for (std::size_t i = 0; i < count; ++i) {
        uint8_t b[2];
        if (file.Read(b, 1, 2) != 2) break;
        ptr[i] = MakeLE16(b[0], b[1]);
        ++done;
    }
    return done;
}

std::size_t Reader::ReadInt32LE(uint32_t* ptr, std::size_t count) {
    std::size_t done = 0;
    for (std::size_t i = 0; i < count; ++i) {
        uint8_t b[4];
        if (file.Read(b, 1, 4) != 4) break;
        ptr[i] = MakeLE32(b[0], b[1], b[2], b[3]);
        ++done;
    }
    return done;
}

std::size_t Reader::ReadBool16LE(bool* ptr, std::size_t count) {
    std::size_t done = 0;
    for (std::size_t i = 0; i < count; ++i) {
        uint8_t b[2];
        if (file.Read(b, 1, 2) != 2) break;
        uint16_t v = MakeLE16(b[0], b[1]);
        ptr[i] = (v != 0);
        ++done;
    }
    return done;
}

std::size_t Writer::WriteInt8(const uint8_t* ptr, std::size_t count) {
    return file.Write(ptr, 1, count);
}

std::size_t Writer::WriteInt16LE(const uint16_t* ptr, std::size_t count) {
    std::size_t done = 0;
    for (std::size_t i = 0; i < count; ++i) {
        uint8_t b[2];
        uint16_t v = ptr[i];
        b[0] = (uint8_t)(v & 0xFF);
        b[1] = (uint8_t)((v >> 8) & 0xFF);
        if (file.Write(b, 1, 2) != 2) break;
        ++done;
    }
    return done;
}

std::size_t Writer::WriteInt32LE(const uint32_t* ptr, std::size_t count) {
    std::size_t done = 0;
    for (std::size_t i = 0; i < count; ++i) {
        uint8_t b[4];
        uint32_t v = ptr[i];
        b[0] = (uint8_t)(v & 0xFF);
        b[1] = (uint8_t)((v >> 8) & 0xFF);
        b[2] = (uint8_t)((v >> 16) & 0xFF);
        b[3] = (uint8_t)((v >> 24) & 0xFF);
        if (file.Write(b, 1, 4) != 4) break;
        ++done;
    }
    return done;
}

std::size_t Writer::WriteBool16LE(const bool* ptr, std::size_t count) {
    std::size_t done = 0;
    for (std::size_t i = 0; i < count; ++i) {
        uint16_t v = ptr[i] ? 1u : 0u;
        uint8_t b[2] = { (uint8_t)(v & 0xFF), (uint8_t)((v >> 8) & 0xFF) };
        if (file.Write(b, 1, 2) != 2) break;
        ++done;
    }
    return done;
}

} // namespace FS

END_UPP_NAMESPACE
