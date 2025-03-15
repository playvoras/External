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
        auto re = TaskScheduler->GetDataModelClient(L"Client 1");
        Logger::info("Result: %llx", re);
    }


    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(100);
    }

    system("pause");
}
