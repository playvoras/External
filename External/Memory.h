#pragma once
#include <Windows.h>
#include <thread>
#include <iostream>
#include <TlHelp32.h>
#include <unordered_map>
#include "log.h"
#include <map>


class TMemory
{
private:
	std::unordered_map<DWORD, HANDLE> processHandles;
	DWORD StartProcessId = NULL;
	const wchar_t* procName = NULL;
	std::unordered_map<DWORD, std::wstring> clients;
public:


	template<typename T>
	auto read(uintptr_t address) -> T {
		T buff;
		SIZE_T size;


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
					}
				}

			}
		}).detach();
	};


};

inline auto Memory = std::make_unique<TMemory>();