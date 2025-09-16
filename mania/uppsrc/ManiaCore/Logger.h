#pragma once

// Minimal logger facade; expand later as needed.
#undef ERROR
namespace ManiaLogLevel { enum Level { INFO, WARN, ERROR, DEBUG }; }

void Log(ManiaLogLevel::Level lvl, const String& msg);
inline void LogInfo(const String& s)  { Log(ManiaLogLevel::INFO, s); }
inline void LogWarn(const String& s)  { Log(ManiaLogLevel::WARN, s); }
inline void LogError(const String& s) { Log(ManiaLogLevel::ERROR, s); }
inline void LogDebug(const String& s) { Log(ManiaLogLevel::DEBUG, s); }

