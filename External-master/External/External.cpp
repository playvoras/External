#include <iostream>
#include "Memory.h"
#include "Roblox/TaskScheduler/TaskScheduler.h"
#include "Roblox/instance/instance.h"
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

    if (!process->IsRunning()) {
        std::cout << "[!] RobloxPlayerBeta isn't running." << std::endl;
        return 1;
    }

    auto re = TaskScheduler->GetDataModelClient();
    auto DataModel = Instance(TaskScheduler->GetDataModelClient());
    auto sc = TaskScheduler->GetScriptContextClient();
    print_address("[?] DataModel", re);
    std::cout << DataModel->GetName() << std::endl;


    return 0;
}