#include "utils.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <assert.h>

namespace Utils {

ProcessUtils *ProcessUtils::get_instance()
{
    static ProcessUtils p;
    return &p;
}

int ProcessUtils::GetProcessId(std::wstring process_name){
    int result = 0;
    HANDLE snapshot_handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    assert(snapshot_handle != INVALID_HANDLE_VALUE);

    PROCESSENTRY32 process_entry = { .dwSize = sizeof(PROCESSENTRY32) };
    boolean success = Process32First(snapshot_handle, &process_entry);
    while (success){
        if (_wcsicmp(process_entry.szExeFile, process_name.c_str()) == 0){
            if(this->m_collection.size() <= 0){
                result = process_entry.th32ProcessID;
                this->m_collection.push_back(process_entry.th32ProcessID);
                break;
            }

            for(int id : this->m_collection){
                if(process_entry.th32ProcessID != id){
                    this->m_collection.push_back(process_entry.th32ProcessID);
                    result = process_entry.th32ProcessID;
                    break;
                }
            }
        }
        success = Process32Next(snapshot_handle, &process_entry);
    }
    CloseHandle(snapshot_handle);
    return result;
}

void ProcessUtils::RemoveProcessId(int pid){
    std::erase(this->m_collection, pid);
}
}
