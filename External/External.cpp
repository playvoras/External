#include <iostream>
#include "Memory.h"



int main()
{
    Memory->Init(L"RobloxPlayerBeta.exe");
    Memory->Watcher();


    while (!GetAsyncKeyState(VK_END))
    {
        Sleep(100);
    }

    system("pause");
}
