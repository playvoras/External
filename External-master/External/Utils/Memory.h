#pragma once
#include <Windows.h>
#include <thread>
#include <TlHelp32.h>
#include <psapi.h>
#include <string>
#include "ntdll.h"
#include <vector>



class TMemory {
private:
    DWORD process_id = NULL;
    HANDLE process_handle = NULL;
    const wchar_t* procName = L"RobloxPlayerBeta.exe";
    HWND rbx_window = NULL;
    DWORD rbx_thread_id = NULL;
public:
    auto setup() -> void {
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnap != INVALID_HANDLE_VALUE) {
            PROCESSENTRY32 Entry;
            Entry.dwSize = sizeof(PROCESSENTRY32);
            do {
                if (wcscmp(Entry.szExeFile, procName) == 0) {
                    process_id = Entry.th32ProcessID;
                    process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
                    rbx_window = FindWindowA(NULL, "Roblox");
                    rbx_thread_id = GetWindowThreadProcessId(
                        rbx_window, &process_id
                    );
                    HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");

                    NTDLL_INIT_FCNS(ntdll);
                    break;
                }
            } while (Process32Next(hSnap, &Entry));
            CloseHandle(hSnap);
        }
    }

    template<typename T>
    auto read(uintptr_t address) -> T {
        T buff{};
        if (!process_handle) return 0;
        SIZE_T bytesRead;
        NtReadVirtualMemory(process_handle, reinterpret_cast<LPCVOID>(address), &buff, sizeof(T), &bytesRead);
        return buff;
    }

    bool read_bool(uintptr_t address) { return read<bool>(address); }
    int read_int(uintptr_t address) { return read<int>(address); }

    auto allocate(size_t size, DWORD protection = PAGE_EXECUTE_READWRITE) -> uintptr_t {
        if (!process_handle) return 0;

        return reinterpret_cast<uintptr_t>(
            VirtualAllocEx(process_handle, nullptr, size, MEM_COMMIT | MEM_RESERVE, protection)
            );
    }

    auto write_bytes(uintptr_t address, const std::vector<uint8_t>& data) -> bool {
        if (!process_handle) return false;

        SIZE_T bytesWritten;
        return WriteProcessMemory(process_handle, reinterpret_cast<LPVOID>(address), data.data(), data.size(), &bytesWritten)
            && bytesWritten == data.size();
    }



    auto read_bytes(uintptr_t address, size_t size) -> std::vector<uint8_t> {
        if (!process_handle || size == 0) return {};
        std::vector<uint8_t> buffer(size);
        SIZE_T bytesRead;
        if (NtReadVirtualMemory(process_handle, reinterpret_cast<LPCVOID>(address), buffer.data(), size, &bytesRead) == 0 && bytesRead == size) {
            return buffer;
        }
        return {};
    }


    template<typename T>
    auto read_on_module(uintptr_t address) -> T {
        T buff{};
        DWORD size;
        if (!process_handle) return 0;
        MODULEINFO moduleInfo = {};
        HMODULE hModule = nullptr;
        if (EnumProcessModules(process_handle, &hModule, sizeof(hModule), &size)) {
            if (GetModuleInformation(process_handle, hModule, &moduleInfo, sizeof(moduleInfo))) {
                uintptr_t base = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
                uintptr_t Address = base + address;
                NtReadVirtualMemory(process_handle, reinterpret_cast<LPCVOID>(Address), &buff, sizeof(T), nullptr);
            }
        }
        return buff;
    }

    template<typename T>
    bool write_memory(uintptr_t address, T value) {
        if (!process_handle) return false;
        SIZE_T bytesWritten;
        return NtWriteVirtualMemory(process_handle, reinterpret_cast<PVOID>(address), &value, sizeof(T), &bytesWritten) == 0 && bytesWritten == sizeof(T);
    }

    bool write_bool(uintptr_t address, bool value) { return write_memory<bool>(address, value); }
    bool write_int(uintptr_t address, int value) { return write_memory<int>(address, value); }

    bool write_string(uintptr_t address, const char* value) {
        if (!process_handle) return false;
        SIZE_T bytesWritten;
        return NtWriteVirtualMemory(process_handle, reinterpret_cast<PVOID>(address), (PVOID)value, strlen(value) + 1, &bytesWritten) == 0 && bytesWritten == strlen(value) + 1;
    }

    auto read_string_unknown(uintptr_t address) -> std::string {
        std::string result;
        char character = 0;
        int offset = 0;
        result.reserve(204);
        while (offset < 200) {
            character = read<char>(address + offset);
            if (character == 0) break;
            offset += sizeof(character);
            result.push_back(character);
        }
        return result;
    }

    auto read_long(uintptr_t address) -> long {
        return read<long>(address);
    }

    auto read_longlong(uintptr_t address) -> long long {
        return read<long long>(address);
    }

    auto write_long(uintptr_t address, long value) -> bool {
        return write_memory<long>(address, value);
    }

    template <typename T>
    auto write_longlong(uintptr_t address, T value) -> bool {
        if constexpr (std::is_pointer_v<T>) {
            if (!value) return false;
            return write_memory<long long>(address, *value);  // Dereference pointer
        }
        else {
            return write_memory<long long>(address, value);  // Direct value
        }
    }



    auto read_string(uintptr_t address) -> std::string {
        const auto length = read<int>(address + 0x10);
        if (length >= 16u) {
            const auto name = read<uintptr_t>(address);
            return read_string_unknown(name);
        }
        return read_string_unknown(address);
    }

    auto get_pid() -> DWORD { return process_id; }
    auto get_handle() -> HANDLE { return process_handle; }
    auto get_window() -> HWND { return rbx_window;  }
    auto get_thread() -> DWORD { return rbx_thread_id;  }

    auto pattern_scan(const std::string& pattern) -> uintptr_t {
        if (!process_handle) return 0;

        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        uintptr_t start = (uintptr_t)sysInfo.lpMinimumApplicationAddress;
        uintptr_t end = (uintptr_t)sysInfo.lpMaximumApplicationAddress;

        MEMORY_BASIC_INFORMATION mbi;
        std::vector<uint8_t> bytePattern(pattern.begin(), pattern.end());

        constexpr SIZE_T bufferSize = 4096;
        uint8_t buffer[bufferSize];

        for (uintptr_t addr = start; addr < end; addr += mbi.RegionSize) {
            if (!VirtualQueryEx(process_handle, (LPCVOID)addr, &mbi, sizeof(mbi)))
                continue;

            if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS)
                continue;
            SIZE_T bytesRead;
            if (!ReadProcessMemory(process_handle, (LPCVOID)addr, buffer, sizeof(buffer), &bytesRead))
                continue;
            for (SIZE_T i = 0; i < bytesRead - bytePattern.size(); i++) {
                if (memcmp(buffer + i, bytePattern.data(), bytePattern.size()) == 0) {
                    return addr + i;
                }
            }
        }
        return 0;
    }

    auto get_first_module() -> MODULEENTRY32 {
        MODULEENTRY32 moduleEntry = { 0 };
        moduleEntry.dwSize = sizeof(MODULEENTRY32);

        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
        if (hSnap == INVALID_HANDLE_VALUE) {
            return moduleEntry;
        }

        if (Module32First(hSnap, &moduleEntry)) {
            return moduleEntry;
        }
        return moduleEntry;
    }

    auto get_hyperion() -> uintptr_t {
        uintptr_t modBaseAddr = 0;
        HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, process_id);
        if (hSnap != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 modEntry;
            modEntry.dwSize = sizeof(modEntry);
            if (Module32First(hSnap, &modEntry)) {
                do {
                    if (!_wcsicmp(modEntry.szModule, L"RobloxPlayerBeta.dll")) {
                        modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                        break;
                    }
                } while (Module32Next(hSnap, &modEntry));
            }
        }
        CloseHandle(hSnap);
        return modBaseAddr;
    }

    ~TMemory() {
        if (process_handle) {
            CloseHandle(process_handle);
        }
    }
};

inline auto process = std::make_unique<TMemory>();
