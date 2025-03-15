#pragma once
#include <Windows.h>
#include <thread>
#include <iostream>
#include <TlHelp32.h>
#include <unordered_map>
#include <map>
#include <psapi.h>


class TTaskScheduler
{
private:
	std::unordered_map<uintptr_t, std::wstring> TaskSchedulerClients;
public:
	auto GetTaskSchedulerClient(const std::wstring& ClientName) -> uintptr_t;
	auto GetJobClient(const std::wstring& ClientName, std::string) -> uintptr_t;
	auto GetDataModelClient(const std::wstring& ClientName) -> uintptr_t;
	auto GetScriptContextClient(const std::wstring& ClientName) -> uintptr_t;
};

inline auto TaskScheduler = std::make_unique<TTaskScheduler>();