// This shit is from chatgpt logger because im too lazy to make one :brokenheart:

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <cstdio>
#include <cstdarg>
#include <string>

#define FOREGROUND_MAGENTA 0x5
#define FOREGROUND_YELLOW 0x6
#define FOREGROUND_WHITE 0x7

class Logger {
private:
    static std::string getCurrentDateTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    static std::string WideCharToMultiByteStr(const wchar_t* wideStr) {
        if (!wideStr) return "";

        int size = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
        if (size <= 0) return "";

        std::string narrowStr(size - 1, 0); // -1 to remove null terminator
        WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &narrowStr[0], size, nullptr, nullptr);

        return narrowStr;
    }

    static void setConsoleColor(const std::string& level) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if (level == "INFO") {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Blue
        }
        else if (level == "SUCCESS") {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Green
        }
        else if (level == "WARNING") {
            SetConsoleTextAttribute(hConsole, FOREGROUND_YELLOW | FOREGROUND_INTENSITY); // Yellow
        }
        else if (level == "ERROR") {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); // Red
        }
        else if (level == "DEBUG") {
            SetConsoleTextAttribute(hConsole, FOREGROUND_MAGENTA | FOREGROUND_INTENSITY); // Magenta
        }
        else {
            SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY); // Default White
        }
    }

    // LogMessage for char* strings
    static void logMessage(const char* level, const char* format, va_list args) {
        printf("[%s] ", getCurrentDateTime().c_str()); 
        setConsoleColor(level);
        printf("[%s] ", level);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_WHITE); // Reset to white for the message
        vprintf(format, args); 
        printf("\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_WHITE); // Reset to white after printing
    }

    // LogMessage for wchar_t* strings
    static void logMessage(const char* level, const wchar_t* format, va_list args) {
        printf("[%s] ", getCurrentDateTime().c_str()); // Date and time first
        setConsoleColor(level); // Set color based on log level
        printf("[%s] ", level); // Log type
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_WHITE); // Reset to white for the message
        std::string convertedFormat = WideCharToMultiByteStr(format); // Convert format
        vprintf(convertedFormat.c_str(), args); // The actual message
        printf("\n");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_WHITE); // Reset to white after printing
    }

public:
    template <typename T>
    static void info(const T* format, ...) {
        va_list args;
        va_start(args, format);
        logMessage("INFO", format, args);
        va_end(args);
    }

    template <typename T>
    static void success(const T* format, ...) {
        va_list args;
        va_start(args, format);
        logMessage("SUCCESS", format, args);
        va_end(args);
    }

    template <typename T>
    static void warning(const T* format, ...) {
        va_list args;
        va_start(args, format);
        logMessage("WARNING", format, args);
        va_end(args);
    }

    template <typename T>
    static void error(const T* format, ...) {
        va_list args;
        va_start(args, format);
        logMessage("ERROR", format, args);
        va_end(args);
    }

    template <typename T>
    static void debug(const T* format, ...) {
        va_list args;
        va_start(args, format);
        logMessage("DEBUG", format, args);
        va_end(args);
    }
};
