#include <iostream>
#include "Memory.h"



int main()
{
    Memory->Init(L"RobloxPlayerBeta.exe");
    Memory->Watcher();
    if (Memory->ClientsRunning()) {
        Logger::warning("Clients aren't running so you can't read.");
    }
    else 
    {
        auto result = Memory->read_on_module<uintptr_t>(Memory->GetClient(L"Client 1"), 0x61E5E38);
        Logger::info("Result: %llx", result);
    }


    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(100);
    }

    system("pause");
}
