#include <iostream>
#include "Memory.h"
#include "Roblox/TaskScheduler/TaskScheduler.h"
#include <sstream>
#pragma comment(lib, "User32.lib")
#include <windows.h>

static void print_address(const std::string& name, uintptr_t offset) {
    if (name == "PID") {
        std::cout << name << ": " << offset << std::endl;
    }
    else {
        std::ostringstream oss;
        oss << name << ": 0x" << std::uppercase << std::hex << offset;  // Convert to uppercase hex
        std::cout << oss.str() << std::endl;
    }
}

int main()
{
    process->setup();

    auto re = TaskScheduler->GetDataModelClient();
    auto sc = TaskScheduler->GetScriptContextClient();
    print_address("PID", process->get_pid());
    print_address("DataModel", re);
    print_address("ScriptContext", sc);
    return 0;
}