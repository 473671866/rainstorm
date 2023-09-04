#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>

#define PROCESS_NAME L"vmware-vmx.exe"

namespace Utils {

class ProcessUtils{
public:
    static ProcessUtils* get_instance();
    int GetProcessId(std::wstring process_name);
    void RemoveProcessId(int pid);
private:
    std::vector<int>m_collection;
};


}

#endif // UTILS_H
