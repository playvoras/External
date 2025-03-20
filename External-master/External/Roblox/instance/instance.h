#pragma once
#include <Windows.h>
#include <memory>  // Include this for std::make_unique
#include <string>
#include <vector>


class tInstance
{
private:
    uintptr_t InstanceAddress = NULL;
public:
    tInstance() : InstanceAddress(NULL) {}
    tInstance(uintptr_t Addresses) : InstanceAddress(Addresses) {}
    auto GetAddress() -> uintptr_t { return this->InstanceAddress; };
    auto GetName() -> std::string;
    auto GetRobloxClassName() -> std::string;
    auto Parent() -> uintptr_t;
    auto GetKids() -> std::vector<tInstance*>;
    auto FindFirstChild(std::string ChildName) -> tInstance*;
    auto WaitForChild(std::string ChildName, int timeout = 5) -> tInstance*;
    auto operator()(uintptr_t Addresses) -> tInstance* {
        return new tInstance(Addresses);
    }
};

inline tInstance Instance;
