#include <iostream>
#include <fstream>
#include <Windows.h>


void error(const wchar_t* errorMessage) {

    std::wcout << errorMessage << "\n";
    std::wcin.get();
    exit(-1);
}

int main()
{
    wchar_t windowTitle[256];
    wchar_t dllName[256];
    wchar_t dllPath[MAX_PATH];
    DWORD processId;
    HANDLE processHandle;
    HANDLE threadHandle;
    void* meme;

    std::wcin.getline(windowTitle, sizeof(windowTitle));
    std::wcin.getline(dllName, sizeof(windowTitle));

    std::ifstream f(dllName);
    if (!f.good()) {
        error(L"No dll");
    }


    if (!GetFullPathName(dllName, MAX_PATH, dllPath, 0)) {
        error(L"Could not locate Dll");
    }

    if (!GetWindowThreadProcessId(FindWindow(0, windowTitle), &processId)) {
        error(L"Could not find process");
    }

    processHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);
    if (!processHandle) {
        error(L"Could not open proces");
    }

    meme = VirtualAllocEx(processHandle, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!meme) {
        error(L"Could not allocate memory");
    }

    if (!WriteProcessMemory(processHandle, meme, dllPath, MAX_PATH, 0)) {
        error(L"Could not write Dll Path");
    }

    threadHandle = CreateRemoteThread(processHandle, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryW"), meme, 0, 0);
    if (!threadHandle) {
        error(L"Could not create a thread");
    }

    CloseHandle(processHandle);
    VirtualFreeEx(processHandle, meme, MAX_PATH, MEM_RELEASE);

    std::wcin.get();
    return 1;
}
