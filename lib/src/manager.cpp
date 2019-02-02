#include "manager.hpp"

#include <windows.h>
#include <TlHelp32.h>
#include <memory>
#include <map>
#include <cstdio>

namespace Kappa {
CManager::CManager() {
    this->name = std::wstring {0};
    this->handle = HANDLE {0};
    this->pid = DWORD {0};
}

CManager::CManager(std::wstring name) {
    this->name = name;
    this->handle = HANDLE {0};
    this->pid = DWORD {0};

    PROCESSENTRY32W proc;
    proc.dwSize = sizeof(proc);
    HANDLE psnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (psnap == INVALID_HANDLE_VALUE) throw GetLastError();
    if (!Process32FirstW(psnap, &proc)) throw GetLastError();
    do if (name == proc.szExeFile) {
        this->pid = proc.th32ProcessID;
        this->handle = OpenProcess(CManager::rights, FALSE, this->pid);
        if (!handle) throw GetLastError();
    } while (Process32NextW(psnap, &proc));
    if (!this->pid) throw GetLastError();
    CloseHandle(psnap);

    MODULEENTRY32W mod;
    mod.dwSize = sizeof(mod);
    HANDLE msnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->pid);
    if (msnap == INVALID_HANDLE_VALUE) throw GetLastError();
    if (!Module32FirstW(msnap, &mod)) throw GetLastError();
    do this->modules.emplace(mod.szModule, mod.modBaseAddr); while (Module32NextW(msnap, &mod));
    CloseHandle(msnap);
}

CManager::~CManager() {
    CloseHandle(this->handle);
}
}
