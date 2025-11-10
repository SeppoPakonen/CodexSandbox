#ifndef UPP_KEEN_CROSS_H
#define UPP_KEEN_CROSS_H

#include "Game.h"

NAMESPACE_UPP

// Add printf format-string warnings on compilers which support them
#ifdef __GNUC__
#define CK_PRINTF_FORMAT(fmtstridx, firstarg) __attribute__ ((format (printf, fmtstridx, firstarg)))
#else
#define CK_PRINTF_FORMAT(fmtstridx, firstarg)
#endif

typedef enum CK_Log_Message_Class_T
{
	CK_LOG_MSG_NORMAL,
	CK_LOG_MSG_WARNING,
	CK_LOG_MSG_ERROR,
	CK_LOG_MSG_NONE
} CK_Log_Message_Class_T;

extern const char *ck_cross_logLevel_strings[];

// Log level.
extern CK_Log_Message_Class_T ck_cross_logLevel;

// Used for debugging
void CK_PRINTF_FORMAT(2, 3) CK_Cross_LogMessage(CK_Log_Message_Class_T msgClass, const char *format, ...);

// Emulates the functionality of the "puts" function in text mode
void CK_Cross_puts(const char *str);

// More standard C functions emulated,
// taking English locale into account (and more, but NOT all)
int CK_Cross_toupper(int c);
// A bit less standard, but still done assuming English locale
int CK_Cross_strcasecmp(const char *s1, const char *s2);
int CK_Cross_strncasecmp(const char *s1, const char *s2, size_t n);

// The C standard library doesn't have an implementation of min/max, which is sad.
#define CK_Cross_max(x, y) ((x) < (y) ? (y) : (x))
#define CK_Cross_min(x, y) ((x) > (y) ? (y) : (x))

// Let's have our own definiton of ABS() as well
#define CK_Cross_abs(x) ((x) > 0 ? (x) : (-(x)))

// And a function to clamp the result.
#define CK_Cross_clamp(x, a, b) CK_Cross_max(CK_Cross_min((x), (b)), (a))

// Safe strcpy variant which Quit()s if the buffer is too small.
size_t CK_Cross_strscpy(char* dst, const char* src, size_t bufsiz);

END_UPP_NAMESPACE

#endif