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
        oss << name << ": 0x" << std::uppercase << std::hex << offset;
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

    auto Workspace = DataModel->FindFirstChild("Workspace");
    if (Workspace) {
        std::cout << "workspace " << Workspace->GetName() << std::endl;
        auto Descendants = Workspace->GetDescendants();
        for (const auto& v : Descendants) {
            std::cout << "instance: " << v->GetName() << std::endl;
        }
    }
    else {
        std::cout << "the hell" << std::endl;
    }

    return 0;
}
