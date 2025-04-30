#include <windows.h>
#include <psapi.h>
#include <fstream>
#include <chrono>
#include <string>
#include <iostream>
#include <csignal>

#pragma comment(lib, "psapi.lib")

bool running = true;
const std::wstring LOG_PATH = L"C:\\ProgramData\\applog.txt";

BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        running = false;
    }
    return TRUE;
}

bool IsWindowsApp(const std::wstring& path) {
    return path.find(L"\\Windows\\") != std::wstring::npos;
}

std::wstring GetForegroundExe() {
    HWND hwnd = GetForegroundWindow();
    if (!hwnd) return L"";

    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hProcess) return L"";

    WCHAR path[MAX_PATH];
    DWORD size = MAX_PATH;
    std::wstring result;

    if (QueryFullProcessImageName(hProcess, 0, path, &size)) {
        result = path;
    }

    CloseHandle(hProcess);
    return result;
}

void LogUsage(const std::wstring& appName, int durationSec) {
    SYSTEMTIME st;
    GetLocalTime(&st);

    std::wofstream log(LOG_PATH, std::ios::app);
    if (!log.is_open()) {
        std::wcerr << L"[ERROR] Failed to open log file: " << LOG_PATH << std::endl;
        return;
    }

    int h = durationSec / 3600;
    int m = (durationSec % 3600) / 60;
    int s = durationSec % 60;

    std::wcout << st.wHour << L":" << st.wMinute << L":" << st.wSecond
               << L" - " << appName << L" used for "
               << h << L"h " << m << L"m " << s << L"s" << std::endl;

    log << st.wYear << L"-" << st.wMonth << L"-" << st.wDay << L" "
        << st.wHour << L":" << st.wMinute << L":" << st.wSecond
        << L" - " << appName << L" used for "
        << h << L"h " << m << L"m " << s << L"s\n";
}

int wmain() {
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    std::wcout << L"Monitoring app usage... Press Ctrl+C to exit.\n";

    std::wstring lastApp;
    auto lastTime = std::chrono::system_clock::now();

    while (running) {
        std::wstring current = GetForegroundExe();
        if (!current.empty() && !IsWindowsApp(current)) {
            size_t pos = current.find_last_of(L"\\/");
            std::wstring exe = current.substr(pos + 1);

            if (exe != lastApp) {
                auto now = std::chrono::system_clock::now();
                if (!lastApp.empty()) {
                    int dur = (int)std::chrono::duration_cast<std::chrono::seconds>(now - lastTime).count();
                    LogUsage(lastApp, dur);
                }
                lastApp = exe;
                lastTime = now;
            }
        }
        Sleep(1000);
    }

    // Final log before exit
    auto now = std::chrono::system_clock::now();
    if (!lastApp.empty()) {
        int dur = (int)std::chrono::duration_cast<std::chrono::seconds>(now - lastTime).count();
        LogUsage(lastApp, dur);
    }

    std::wcout << L"Stopped.\n";
    return 0;
}
