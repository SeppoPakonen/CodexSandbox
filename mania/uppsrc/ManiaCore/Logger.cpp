#include "ManiaCore.h"

NAMESPACE_UPP

void Log(ManiaLogLevel::Level lvl, const String& msg)
{
    switch (lvl) {
    case ManiaLogLevel::INFO:  Cout() << "[INFO]  " << msg << '\n'; break;
    case ManiaLogLevel::WARN:  Cout() << "[WARN]  " << msg << '\n'; break;
    case ManiaLogLevel::ERROR: Cout() << "[ERROR] " << msg << '\n'; break;
    case ManiaLogLevel::DEBUG: Cout() << "[DEBUG] " << msg << '\n'; break;
    }
}

END_UPP_NAMESPACE
