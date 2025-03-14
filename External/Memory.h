#pragma once
#include <Windows.h>
#include <thread>
#include <iostream>
#include <TlHelp32.h>
#include <unordered_map>
#include "log.h"
#include <map>
#include <psapi.h>

class TMemory
{
private:
	std::unordered_map<DWORD, HANDLE> processHandles;
	DWORD StartProcessId = NULL;
	const wchar_t* procName = NULL;
	std::unordered_map<DWORD, std::wstring> clients;
public:


	template<typename T>
	auto read(DWORD ClientId, uintptr_t address) -> T {
		T buff;
		SIZE_T size;

#if _DEBUG
		Logger::debug("OpenProcess started.");
#endif 


		HANDLE hClient = OpenProcess(PROCESS_ALL_ACCESS, false, ClientId);
		if (hClient == INVALID_HANDLE_VALUE) {
			Logger::error("Couldn't open process, may not running. (STATUS_CODE=%d)", GetLastError());
			return 0;
		}

#if _DEBUG
		for (const auto& client : clients) {
			if (client.first == ClientId) {
				Logger::debug("Passed, Read memory at (%llx) on (%ls)", address, client.second.c_str());
			}
		}
#endif 

		if (!ReadProcessMemory(hClient, address, buff, sizeof(buff), size)) {

#if _DEBUG
			for (const auto& client : clients) {
				if (client.first == ClientId) {
					Logger::warning("Failed to Read memory at (%llx) on (%ls)", address, client.second.c_str());
				}
			}
#else
	Logger::error("Failed to read memory. (STATUS_CODE=%d)", GetLastError());
#endif 

			return 0;
		}



		return buff;
	}

	template<typename T>
	auto read_on_module(DWORD ClientId, uintptr_t address) -> T {
		T buff;
		DWORD size;
		SIZE_T Size;

		if (clients.empty()) {
			Logger::error("Couldn't read because none of client is running (BRUH!)");
			return 0;
		}


#if _DEBUG
		Logger::debug("OpenProcess started.");
#endif 

		HANDLE hClient = OpenProcess(PROCESS_ALL_ACCESS, false, ClientId);
		if (hClient == INVALID_HANDLE_VALUE) {
			Logger::error("Couldn't open process, may not running. (STATUS_CODE=%d)", GetLastError());
			return 0; 
		}

		MODULEINFO moduleInfo = {};
		HMODULE hModule = nullptr;
		if (EnumProcessModules(hClient, &hModule, sizeof(hModule), &size)) {
			if (GetModuleInformation(hClient, hModule, &moduleInfo, sizeof(moduleInfo))) {
				uintptr_t base = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);

#if _DEBUG
				for (const auto& client : clients) {
					if (client.first == ClientId) {
						Logger::debug("(%ls) Base address: (%llx)", client.second.c_str(), base);
					}
				}
#endif 

				uintptr_t Address = base + address;

#if _DEBUG
				Logger::debug("Reading memory at (%llx)", Address);
#endif

				if (!ReadProcessMemory(hClient, reinterpret_cast<LPCVOID>(Address), &buff, sizeof(buff), &Size)) {
#if _DEBUG
					for (const auto& client : clients) {
						if (client.first == ClientId) {
							Logger::warning("Failed to Read memory at (%llx) on (%ls)", Address, client.second.c_str());
						}
					}
#else
					Logger::error("Failed to read memory. (STATUS_CODE=%d)", GetLastError());
#endif 
					return 0; 
				}
			}
			else {
				Logger::error("Failed to get module. (STATUS_CODE=%d)", GetLastError());
			}
		}
		else {
			Logger::error("Failed to get module. (STATUS_CODE=%d)", GetLastError());
		}

		CloseHandle(hClient);

		return buff;
	}

	auto Init(const wchar_t* ProcessName) -> void {
		Logger::info("Memory initialize Starting Now.");
		procName = ProcessName;

#if _DEBUG
		Logger::debug("Trying to Detect {%ls}", ProcessName);

#endif 
		if (!StartProcessId) {
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, false);
			if (hSnap != INVALID_HANDLE_VALUE) {
				PROCESSENTRY32 Entry;
				Entry.dwSize = sizeof(Entry); 

				do {
					if (wcscmp(Entry.szExeFile, ProcessName) == NULL) {
						StartProcessId = Entry.th32ProcessID;
						clients[StartProcessId] = L"Client 1";

#if _DEBUG
						Logger::success("Detected Client, Got ProcessId {%d}", StartProcessId);
#endif
					}
				} while (Process32Next(hSnap, &Entry));

				CloseHandle(hSnap);
			}


			if (clients.empty()) {
				Logger::warning("Didn't Detected Process (%ls)", ProcessName);
				return;
			}

		}


		Logger::success("Memory successfuly initialize!");
	}

	auto test(const std::wstring& ClientName) -> void {
		Sleep(5000);
		auto result = this->read_on_module<uintptr_t>(this->GetClient(ClientName), 0x61E5E38);
		Logger::info("result on (%ls) : %llx", ClientName.c_str(), result);
	}

	auto ClientsRunning() -> bool {
		if (clients.empty()) {
			return true;
		}
		else {
			return false;
		}
	}

	auto GetClient(const std::wstring& Name) -> DWORD {
		for (const auto& client : clients) {
			if (client.second == Name) {
				return client.first;
			}
		}

		Logger::error(L"Client not found: %ls", Name.c_str());
		return 0;
	}

	auto RefreshProcessList() -> void {
		std::unordered_map<DWORD, std::wstring> newMap;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hSnap != INVALID_HANDLE_VALUE) {
			PROCESSENTRY32 Entry;
			Entry.dwSize = sizeof(PROCESSENTRY32);

			DWORD clientIndex = 1;
			do {
				if (wcscmp(Entry.szExeFile, procName) == 0) {
					std::wstring name = L"Client " + std::to_wstring(clientIndex++);
					newMap[Entry.th32ProcessID] = name;
				}
			} while (Process32Next(hSnap, &Entry));

			CloseHandle(hSnap);
		}


		clients.swap(newMap);
	}

	auto Watcher() -> void {
		Logger::success("Watcher Started.");
		bool AllClientGone = false;


		std::thread([&]() -> void {
			while (!GetAsyncKeyState(VK_END))
			{
				std::this_thread::sleep_for(std::chrono::microseconds(100));

				auto previousClients = clients;
				RefreshProcessList();

				for (const auto& [id, name] : previousClients) {
					if (!clients.count(id)) {
						Logger::warning(L"Process exited: %ls (%d)", name.c_str(), id);
					}
				}

				if (clients.empty()) {
					if (!AllClientGone) { // must keep running watcher while all client exit (FLAG)
						Logger::warning("No Client detected, Watcher will wait for new process");
						AllClientGone = true;
					}
				}
				else {
					AllClientGone = false;
				}

				for (const auto& [id, name] : clients) {
					if (!previousClients.count(id)) {
						Logger::warning("Detected new process running: (%ls) {%d}", name.c_str(), id);
						test(name);
					}
				}

			}
		}).detach();
	};


};

inline auto Memory = std::make_unique<TMemory>();