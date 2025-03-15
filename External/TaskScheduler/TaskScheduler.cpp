#include "TaskScheduler.h"
#include "../Memory.h"
#include "../log.h"
#include "../TypeDefinitions.h"


auto TTaskScheduler::GetTaskSchedulerClient(const std::wstring& ClientName) -> uintptr_t
{
    auto result = Memory->read_on_module<uintptr_t>(Memory->GetClient(ClientName), Addresses::RawScheduler);
    if (result == 0) {
#if _DEBUG
        Logger::warning("Failed getting Task Scheduler on (%ls)", ClientName.c_str());
#else
        Logger::error("Couldn't Find Task Scheduler on (%ls), please update address / offsets.", ClientName.c_str());

#endif // _DEBUG
    }

    return result;
}

auto TTaskScheduler::GetJobClient(const std::wstring& ClientName, std::string JobName) -> uintptr_t
{
    uintptr_t Task = this->GetTaskSchedulerClient(ClientName);
    uintptr_t Start = Memory->read<uintptr_t>(Memory->GetClient(ClientName), (Task + Offsets::JobStart));
    uintptr_t End = Memory->read<uintptr_t>(Memory->GetClient(ClientName), (Task + Offsets::JobEnd));

    if (!Start || !End || Start >= End) return 0;

    for (uintptr_t job = Start; job < End; job += 0x10) {
        uintptr_t Address = Memory->read<uintptr_t>(Memory->GetClient(ClientName), job);
        if (!Address || Address == -1) continue;

        uintptr_t pointer = Memory->read<uintptr_t>(Memory->GetClient(ClientName), (Address + Offsets::JobName));
        std::string jobName = Memory->read_string_unknown(Memory->GetClient(ClientName), pointer);

        if (jobName == JobName) {
            return Address;
        }
    }

    /*
    * 
    * 
    * Check if found string or nah uh
    * 
    * 
    uintptr_t WaitingHybridScriptsJob = Memory->read<uintptr_t>(Memory->GetClient(ClientName), (Start + 0xE0));
    uintptr_t pointer = Memory->read<uintptr_t>(Memory->GetClient(ClientName), (WaitingHybridScriptsJob + 0x90));


    auto name = Memory->read_string_unknown(Memory->GetClient(ClientName), pointer);
    Logger::info("Result: %llx", string_pointer);
    Logger::info("Result: %s", name.c_str());
    */
    



    return 0;
}

auto TTaskScheduler::GetDataModelClient(const std::wstring& ClientName) -> uintptr_t
{
    uintptr_t DataModelOrUgc = Memory->read<uintptr_t>(Memory->GetClient(ClientName), (this->GetScriptContextClient(ClientName) + Offsets::Parent));

    if (!DataModelOrUgc) return 0;

    return DataModelOrUgc;
}

auto TTaskScheduler::GetScriptContextClient(const std::wstring& ClientName) -> uintptr_t
{
    uintptr_t ScriptContext = Memory->read<uintptr_t>(Memory->GetClient(ClientName), (this->GetJobClient(ClientName, "WaitingHybridScriptsJob") + Offsets::ScriptContextPoint));

    if (!ScriptContext) return 0;

    return ScriptContext;
}



