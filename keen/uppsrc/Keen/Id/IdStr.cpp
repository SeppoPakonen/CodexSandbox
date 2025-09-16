#include "Id.h"


NAMESPACE_UPP

namespace {
    struct STR_Table_Impl : STR_Table {
        std::vector<STR_Entry> arr;
    };

    static unsigned int STR_HashString(const char *str) {
        unsigned int hash = 5381;
        for (; *str; ++str)
            hash = ((hash << 5) + hash) ^ (unsigned int)(unsigned char)(*str);
        return hash * 0x9E3779B1u;
    }

    static inline STR_Table_Impl* Impl(STR_Table* t) {
        return static_cast<STR_Table_Impl*>(t);
    }

    static inline const STR_Table_Impl* Impl(const STR_Table* t) {
        return static_cast<const STR_Table_Impl*>(t);
    }

    static void KeenPanic(const char* msg) {
        // Keep simple for now; can be wired to U++ logging later.
        throw std::runtime_error(msg);
    }

    static int StrNCaseCompare(const char* a, const char* b, size_t n) {
        for(size_t i = 0; i < n; ++i) {
            unsigned char ca = (unsigned char)a[i];
            unsigned char cb = (unsigned char)b[i];
            if (ca == 0 || cb == 0) return (int)ca - (int)cb;
            int da = std::tolower(ca);
            int db = std::tolower(cb);
            if (da != db) return da - db;
        }
        return 0;
    }
}

void STR_AllocTable(STR_Table **tabl, size_t size) {
    auto* t = new STR_Table_Impl();
    t->size = size;
    #ifdef CK_DEBUG
    t->numElements = 0;
    #endif
    t->arr.resize(size);
    for (size_t i = 0; i < size; ++i) {
        t->arr[i].str = nullptr;
        t->arr[i].ptr = nullptr;
    }
    *tabl = t;
}

size_t STR_GetEntryIndex(STR_Table *tabl, const char *str) {
    if (!tabl || !str || Impl(tabl)->arr.empty())
        return ID_STR_INVALID_INDEX;
    size_t size = Impl(tabl)->arr.size();
    size_t hash = STR_HashString(str) % size;
    size_t lastHash = (size_t)-1;
    for (size_t i = hash; i != lastHash; i = (i + 1) % size) {
        if (Impl(tabl)->arr[i].str == nullptr)
            return i;
        else if (!std::strcmp(Impl(tabl)->arr[i].str, str))
            return i;
        lastHash = hash;
    }
    return ID_STR_INVALID_INDEX;
}

bool STR_DoesEntryExist(STR_Table *tabl, const char *str) {
    size_t index = STR_GetEntryIndex(tabl, str);
    if (index == (size_t)-1)
        return false;
    if (Impl(tabl)->arr[index].str)
        return true;
    return false;
}

void *STR_LookupEntryWithDefault(STR_Table *tabl, const char *str, void *def) {
    size_t i = STR_GetEntryIndex(tabl, str);
    if (i == ID_STR_INVALID_INDEX || Impl(tabl)->arr[i].str == nullptr)
        return def;
    return Impl(tabl)->arr[i].ptr;
}

void *STR_LookupEntry(STR_Table *tabl, const char *str) {
    return STR_LookupEntryWithDefault(tabl, str, (void*)(0));
}

bool STR_AddEntry(STR_Table *tabl, const char *str, void *value) {
    size_t i = STR_GetEntryIndex(tabl, str);
    if (i == ID_STR_INVALID_INDEX) {
        return false;
    } else {
        #ifdef CK_DEBUG
        if (Impl(tabl)->arr[i].str != nullptr) {
            std::fprintf(stderr, "[STR] Overwriting entry (\"%s\", %p) with (\"%s\", %p)\n",
                         str, value, Impl(tabl)->arr[i].str, Impl(tabl)->arr[i].ptr);
        }
        #endif
        Impl(tabl)->arr[i].str = str;
        Impl(tabl)->arr[i].ptr = value;
        #ifdef CK_DEBUG
        Impl(tabl)->numElements++;
        if (Impl(tabl)->numElements == Impl(tabl)->size) {
            KeenPanic("Tried to over-fill a hashtable!");
        }
        #endif
        return true;
    }
}

void *STR_GetNextEntry(STR_Table *tabl, size_t *index) {
    if (!tabl || !index) return (void*)0;
    for (size_t i = *index;; ++i) {
        if (i >= Impl(tabl)->arr.size()) {
            *index = 0;
            return (void*)0;
        }
        if (Impl(tabl)->arr[i].str != nullptr) {
            *index = i + 1;
            return Impl(tabl)->arr[i].ptr;
        }
    }
}

static char STR_PeekCharacter(STR_ParserState *ps) {
    if (ps->dataindex >= ps->datasize)
        return '\0';
    return ps->data[ps->dataindex];
}

static char STR_GetCharacter(STR_ParserState *ps) {
    if (ps->dataindex >= ps->datasize)
        return '\0';
    char c = ps->data[ps->dataindex++];
    if (c == '\n')
        ps->linecount++;
    return c;
}

static void STR_SkipWhitespace(STR_ParserState *ps) {
    char c;
    do {
        c = STR_PeekCharacter(ps);
        if (c == '#') {
            while (STR_PeekCharacter(ps) != '\n') {
                c = STR_GetCharacter(ps);
            }
            c = '\n';
        }
        else if (std::isspace((unsigned char)c))
            STR_GetCharacter(ps);
    } while (c && std::isspace((unsigned char)c));
}

STR_Token STR_GetToken(STR_ParserState *ps) {
    if (ps->haveBufferedToken) {
        ps->haveBufferedToken = false;
        return ps->bufferedToken;
    }
    char tokenbuf[ID_STR_MAX_TOKEN_LENGTH];
    int i = 0;
    STR_SkipWhitespace(ps);
    STR_Token tok;
    tok.tokenType = STR_TOK_EOF;
    tok.firstIndex = ps->dataindex;
    if (STR_PeekCharacter(ps) && STR_PeekCharacter(ps) == '"') {
        tok.tokenType = STR_TOK_String;
        STR_GetCharacter(ps);
        while (STR_PeekCharacter(ps) != '"') {
            char c = STR_GetCharacter(ps);
            if (c == '\\') {
                c = STR_GetCharacter(ps);
                switch (c) {
                case 'n': c = '\n'; break;
                default: break;
                }
            }
            tokenbuf[i++] = c;
            if (i == ID_STR_MAX_TOKEN_LENGTH)
                KeenPanic("Token exceeded max length!");
        }
        STR_GetCharacter(ps);
    }
    else if (STR_PeekCharacter(ps)) {
        tok.tokenType = STR_TOK_Ident;
        do {
            tokenbuf[i++] = STR_GetCharacter(ps);
            if (i == ID_STR_MAX_TOKEN_LENGTH)
                KeenPanic("Token exceeded max length!");
        } while (STR_PeekCharacter(ps) && !std::isspace((unsigned char)STR_PeekCharacter(ps)) && !(STR_PeekCharacter(ps) == ','));
    }
    tok.lastIndex = ps->dataindex;
    tokenbuf[i] = '\0';

    tok.valuePtr = &ps->data[tok.firstIndex];
    tok.valueLength = tok.lastIndex - tok.firstIndex;
    return tok;
}

STR_Token STR_PeekToken(STR_ParserState *ps) {
    STR_Token tok = STR_GetToken(ps);
    ps->haveBufferedToken = true;
    ps->bufferedToken = tok;
    return tok;
}

size_t STR_GetStringValue(STR_Token tok, char *tokenBuf, size_t bufLength) {
    int i = 0;
    if (tok.tokenType == STR_TOK_EOF)
        return 0;
    if (tok.tokenType == STR_TOK_String) {
        tok.valuePtr++;
        while (*(tok.valuePtr) != '"') {
            char c = *(tok.valuePtr++);
            if (c == '\\') {
                c = *(tok.valuePtr++);
                switch (c) {
                case 'n': c = '\n'; break;
                default: break;
                }
            }
            tokenBuf[i++] = c;
            if ((size_t)i == bufLength)
                KeenPanic("Token exceeded max length!");
        }
    } else {
        if ((size_t)tok.valueLength >= bufLength)
            KeenPanic("Token exceeded max length!");
        std::memcpy(tokenBuf, tok.valuePtr, tok.valueLength);
        i = tok.valueLength;
    }
    tokenBuf[i] = '\0';
    return i;
}

size_t STR_GetString(STR_ParserState *ps, char *tokenBuf, size_t bufLength) {
    STR_Token tok = STR_GetToken(ps);
    return STR_GetStringValue(tok, tokenBuf, bufLength);
}

size_t STR_GetIdent(STR_ParserState *ps, char *tokenBuf, size_t bufLength) {
    STR_Token tok = STR_GetToken(ps);
    return STR_GetStringValue(tok, tokenBuf, bufLength);
}

bool STR_IsTokenIdent(STR_Token tok, const char *str) {
    size_t len = std::strlen(str);
    if ((int)len != tok.valueLength)
        return false;
    return !std::strncmp(tok.valuePtr, str, len);
}

bool STR_IsTokenIdentCase(STR_Token tok, const char *str) {
    size_t len = std::strlen(str);
    if ((int)len != tok.valueLength)
        return false;
    return !StrNCaseCompare(tok.valuePtr, str, len);
}

int STR_GetIntegerValue(STR_Token token) {
    int result = 0;
    if (token.tokenType != STR_TOK_Ident && token.tokenType != STR_TOK_Number)
        return 0;
    if (token.valuePtr[0] == '$')
        result = std::strtol(token.valuePtr + 1, nullptr, 16);
    else
        result = std::strtol(token.valuePtr, nullptr, 0);
    return result;
}

int STR_GetInteger(STR_ParserState *ps) {
    STR_Token token = STR_GetToken(ps);
    return STR_GetIntegerValue(token);
}

bool STR_ExpectToken(STR_ParserState *ps, const char *str) {
    STR_Token tok = STR_GetToken(ps);
    if (tok.tokenType == STR_TOK_EOF)
        return false;
    bool result = !std::strncmp(tok.valuePtr, str, tok.valueLength);
    if (!result)
        std::fprintf(stderr, "[STR] ExpectToken, got \"%.*s\" expected \"%s\" on line %d\n",
                     tok.valueLength, tok.valuePtr, str, ps->linecount);
    return result;
}

END_UPP_NAMESPACE

