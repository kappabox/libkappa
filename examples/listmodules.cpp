#include <kappa/manager.hpp>
#include <string>
#include <iostream>
#include <locale>
#include <codecvt>

using namespace Kappa;

int main (void) {
    CManager manager;
    try {
        manager = CManager(L"notepad.exe");
    } catch (unsigned long e) {
        return e;
    }

    std::wcout << "[" << manager.GetName() << "]" << " #" << \
        manager.GetPid() << std::endl;

    for (auto e : manager.GetModules()) {
        std::wcout << "\t" << e.first << " @" << e.second << std::endl;
    }

    return 0;
}
