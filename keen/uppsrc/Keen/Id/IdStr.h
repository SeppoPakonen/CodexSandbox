// C++ port of id_str (string table + tokenizer)
#pragma once


// Keep names and PODs similar to the original for now.

typedef struct STR_Entry {
    const char *str;
    void *ptr;
} STR_Entry;

typedef struct STR_Table {
    std::size_t size;
    #ifdef CK_DEBUG
    std::size_t numElements;
    #endif
    // Implementation detail in C++: use a dynamic array internally (see .cpp)
    // to emulate the original flexible array member.
    // The storage is private to implementation.
} STR_Table;

void STR_AllocTable(STR_Table **tabl, std::size_t size);
bool STR_DoesEntryExist(STR_Table *tabl, const char *str);
void *STR_LookupEntryWithDefault(STR_Table *tabl, const char *str, void *def);
void *STR_LookupEntry(STR_Table *tabl, const char *str);
bool STR_AddEntry(STR_Table *tabl, const char *str, void *value);

void *STR_GetNextEntry(STR_Table *tabl, std::size_t *index);

// Gets the index where "str" either exists, or should be inserted.
std::size_t STR_GetEntryIndex(STR_Table *tabl, const char *str);

#define ID_STR_MAX_TOKEN_LENGTH 1024
#define ID_STR_INVALID_INDEX ((std::size_t)-1)

typedef enum STR_TokenType {
    STR_TOK_EOF,
    STR_TOK_Ident,
    STR_TOK_Number,
    STR_TOK_String
} STR_TokenType;

typedef struct STR_Token {
    STR_TokenType tokenType;
    const char *valuePtr;
    int valueLength;

    int firstIndex;
    int lastIndex;
} STR_Token;

typedef struct STR_ParserState {
    char *data;
    int dataindex;
    int datasize;
    int linecount;
    bool haveBufferedToken;
    STR_Token bufferedToken;
} STR_ParserState;

STR_Token STR_GetToken(STR_ParserState *ps);
STR_Token STR_PeekToken(STR_ParserState *ps);
std::size_t STR_GetStringValue(STR_Token tok, char *tokenBuf, std::size_t bufLen);
std::size_t STR_GetString(STR_ParserState *ps, char *tokenBuf, std::size_t bufLen);
std::size_t STR_GetIdent(STR_ParserState *ps, char *tokenBuf, std::size_t bufLen);
bool STR_IsTokenIdent(STR_Token tok, const char *str);
bool STR_IsTokenIdentCase(STR_Token tok, const char *str);
int STR_GetIntegerValue(STR_Token tok);
int STR_GetInteger(STR_ParserState *ps);
bool STR_ExpectToken(STR_ParserState *ps, const char *str);

// C++ wrapper API (kept alongside C functions for migration)
namespace Str {
struct Token { STR_Token inner; };

class Parser {
public:
    Parser(char* data, int size) { state.data = data; state.datasize = size; state.dataindex = 0; state.linecount = 0; state.haveBufferedToken = false; }
    Token GetToken() { Token t; t.inner = STR_GetToken(&state); return t; }
    Token PeekToken() { Token t; t.inner = STR_PeekToken(&state); return t; }
    size_t GetString(char* buf, size_t len) { return STR_GetString(&state, buf, len); }
    size_t GetIdent(char* buf, size_t len) { return STR_GetIdent(&state, buf, len); }
    int GetInteger() { return STR_GetInteger(&state); }
    bool Expect(const char* s) { return STR_ExpectToken(&state, s); }
    int Line() const { return state.linecount; }
private:
    STR_ParserState state;
};

class Table {
public:
    Table() : t(nullptr) {}
    explicit Table(size_t sz) { STR_AllocTable(&t, sz); }
    bool Exists(const char* s) { return STR_DoesEntryExist(t, s); }
    void* Lookup(const char* s) { return STR_LookupEntry(t, s); }
    bool Add(const char* s, void* v) { return STR_AddEntry(t, s, v); }
    size_t Next(size_t* idx) { void* p = STR_GetNextEntry(t, idx); return p ? *idx : 0; }
    STR_Table* Raw() { return t; }
private:
    STR_Table* t;
};

// Helpers mapping token utilities
inline size_t GetStringValue(const Token& tok, char* buf, size_t len) { return STR_GetStringValue(tok.inner, buf, len); }
inline bool IsTokenIdentCase(const Token& tok, const char* s) { return STR_IsTokenIdentCase(tok.inner, s); }
inline int GetIntegerValue(const Token& tok) { return STR_GetIntegerValue(tok.inner); }
}
