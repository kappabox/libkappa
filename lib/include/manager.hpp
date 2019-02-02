#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <memory>
#include <map>
#include <string>

namespace Kappa {
class CManager {
private:
    static const DWORD rights = PROCESS_VM_OPERATION | PROCESS_VM_READ |
        PROCESS_VM_WRITE;

    std::wstring name;
    HANDLE handle;
    DWORD pid;
    std::map<std::wstring, BYTE*> modules;

    /* TODO: ZwRead/ZwWrite */

public:
    CManager();
    CManager(std::wstring);
    ~CManager();

    std::wstring GetName() { return this->name; }
    HANDLE GetHandle() { return this->handle; }
    DWORD GetPid() { return this->pid; }
    auto GetModules() { return this->modules; }
    BYTE* GetModule(std::wstring module) { return this->modules[module]; }
};
}
