#pragma once

#include <sstream>

enum class LogLevel {
    Info,
    Warning,
    Error
};

void LogOpen(fs::path& folder);
void LogClose();
void LogPrint(LogLevel level, const CharString& message);

template <typename... Ts>
void LogPrint(LogLevel level, Ts... args) {
    std::stringstream s;
    int dummy[sizeof...(Ts)] = { (s << args, 0)... };
    LogPrint(level, s.str());
}
