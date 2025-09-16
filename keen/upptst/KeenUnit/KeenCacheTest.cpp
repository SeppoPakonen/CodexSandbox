#include "KeenUnit.h"

NAMESPACE_UPP

void RunKeenCacheTests() {
    // Create a small file
    const char* fname = "keen_unittest_chunk.bin";
    {
        FS::File f = FS::File::CreateUser(fname);
        ASSERT(f.IsValid());
        const char* msg = "HELLO";
        f.Write(msg, 1, 5);
        f.Close();
    }

    CA::Cache cache;
    ASSERT(cache.LoadUser("greet", fname));
    auto c = cache.Get("greet");
    ASSERT(c && c->data.size() == 5);
    ASSERT(std::memcmp(c->data.data(), "HELLO", 5) == 0);
}

END_UPP_NAMESPACE

