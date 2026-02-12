#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <comdef.h>

// C++17 filesystem desteği
#include <filesystem>
#if defined(_WIN32) && defined(_MSC_VER)
// Visual Studio 2017+ için
namespace fs = std::filesystem;
#else
// Diğer compiler'lar için
namespace fs = std::filesystem;
#endif

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
// Resource IDs
#define IDI_ICON1           1

// Window control IDs
#define IDC_PATH_EDIT       1001
#define IDC_BROWSE_BUTTON   1002
#define IDC_STATUS_TEXT     1003
#define IDC_DROP_AREA       1004

// Global variables
HWND g_hWnd = nullptr;
HWND g_hPathEdit = nullptr;
HWND g_hBrowseButton = nullptr;
HWND g_hStatusText = nullptr;
HWND g_hDropArea = nullptr;
HINSTANCE g_hInstance = nullptr;
std::wstring g_projectPath;

// Fonksiyon bildirimleri
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateControls(HWND hwnd);
void OnDropFiles(HDROP hDrop);
bool ExtractZip(const std::wstring& zipPath, const std::wstring& extractPath);
void DistributeFiles(const std::wstring& extractPath, const std::wstring& projectPath);
std::wstring BrowseForFolder(HWND hwnd);
void UpdateStatus(const std::wstring& message);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_hInstance = hInstance;
    
    // Register window class
    const wchar_t CLASS_NAME[] = L"KiCadFileOrganizer";
    
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    
    RegisterClassW(&wc);
    
    // Create window - larger size for better UI
    g_hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"KiCad Component Organizer",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 550, 380,
        nullptr, nullptr, hInstance, nullptr
    );
    
    if (g_hWnd == nullptr) {
        return 0;
    }
    
    ShowWindow(g_hWnd, nCmdShow);
    
    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            CreateControls(hwnd);
            // Enable drag and drop support
            DragAcceptFiles(hwnd, TRUE);
            return 0;
            
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            
        case WM_DROPFILES:
            OnDropFiles((HDROP)wParam);
            return 0;
            
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_BROWSE_BUTTON) {
                std::wstring folder = BrowseForFolder(hwnd);
                if (!folder.empty()) {
                    g_projectPath = folder;
                    SetWindowTextW(g_hPathEdit, folder.c_str());
                    UpdateStatus(L"Project path selected: " + folder);
                }
            }
            return 0;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Background for drag and drop area with light styling
            if (g_hDropArea) {
                RECT rect;
                GetClientRect(g_hDropArea, &rect);
                MapWindowPoints(g_hDropArea, hwnd, (LPPOINT)&rect, 2);

                // Light blue background
                HBRUSH hBrush = CreateSolidBrush(RGB(240, 248, 255));
                FillRect(hdc, &rect, hBrush);
                DeleteObject(hBrush);

                // Border
                HPEN hPen = CreatePen(PS_SOLID, 2, RGB(100, 149, 237));
                HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
                Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
                SelectObject(hdc, hOldPen);
                SelectObject(hdc, hOldBrush);
                DeleteObject(hPen);
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CreateControls(HWND hwnd) {
    // Title label
    HFONT hTitleFont = CreateFontW(
        18, 0, 0, 0, FW_BOLD,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI"
    );
    
    HWND hTitle = CreateWindowW(L"STATIC", L"KiCad Component Organizer",
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        100, 25, 420, 25,
        hwnd, nullptr, nullptr, nullptr);
    SendMessageW(hTitle, WM_SETFONT, (WPARAM)hTitleFont, TRUE);
    
    // Subtitle
    HWND hSubtitle = CreateWindowW(L"STATIC", L"Organize your KiCad components easily",
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        100, 50, 420, 20,
        hwnd, nullptr, nullptr, nullptr);
    
    // Project path label
    CreateWindowW(L"STATIC", L"Project Path:",
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        20, 100, 100, 20,
        hwnd, nullptr, nullptr, nullptr);
    
    // Project path edit box
    g_hPathEdit = CreateWindowW(L"EDIT", L"",
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
        20, 120, 400, 28,
        hwnd, (HMENU)IDC_PATH_EDIT, nullptr, nullptr);
    
    // Browse button
    g_hBrowseButton = CreateWindowW(L"BUTTON", L"Browse...",
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
        430, 120, 90, 28,
        hwnd, (HMENU)IDC_BROWSE_BUTTON, nullptr, nullptr);
    
    // Drag and drop area with better styling (ASCII-only text)
    g_hDropArea = CreateWindowW(L"STATIC", L"Drag and drop ZIP file here\r\n\r\nSupported files:\r\n- .kicad_mod (Footprints)\r\n- .kicad_sym (Symbols)\r\n- .step / .stp (3D Models)",
        WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
        20, 165, 500, 140,
        hwnd, (HMENU)IDC_DROP_AREA, nullptr, nullptr);
    
    // Status text with better styling
    g_hStatusText = CreateWindowW(L"STATIC", L"Ready - Select a project path to begin",
        WS_VISIBLE | WS_CHILD | SS_LEFT,
        20, 320, 500, 25,
        hwnd, (HMENU)IDC_STATUS_TEXT, nullptr, nullptr);
}

void OnDropFiles(HDROP hDrop) {
    UINT fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
    
    if (fileCount == 0) {
        DragFinish(hDrop);
        return;
    }
    
    wchar_t szFile[MAX_PATH];
    DragQueryFileW(hDrop, 0, szFile, MAX_PATH);
    
    std::wstring zipPath = szFile;
    std::wstring ext = fs::path(zipPath).extension().wstring();
    
    // Only accept zip files
    std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);
    if (ext != L".zip") {
        UpdateStatus(L"Error: Please select a ZIP file!");
        DragFinish(hDrop);
        return;
    }
    
    if (g_projectPath.empty()) {
        UpdateStatus(L"Error: Please select a project path first!");
        DragFinish(hDrop);
        return;
    }
    
    UpdateStatus(L"Extracting ZIP file...");
    
    // Extract to temporary folder
    std::wstring tempDir = fs::temp_directory_path().wstring() + L"\\kicad_extract_" + 
                           std::to_wstring(GetTickCount());
    fs::create_directories(tempDir);
    
    if (ExtractZip(zipPath, tempDir)) {
        UpdateStatus(L"Distributing files...");
        DistributeFiles(tempDir, g_projectPath);
        
        // Clean up temporary folder
        fs::remove_all(tempDir);
        UpdateStatus(L"Process completed!");
    } else {
        UpdateStatus(L"Error: Could not extract ZIP file!");
        fs::remove_all(tempDir);
    }
    
    DragFinish(hDrop);
}

bool ExtractZip(const std::wstring& zipPath, const std::wstring& extractPath) {
    // Extract zip using PowerShell (Windows built-in support)
    // Properly escape quotes
    std::wstring escapedZipPath = zipPath;
    std::wstring escapedExtractPath = extractPath;
    
    // Double backslashes
    size_t pos = 0;
    while ((pos = escapedZipPath.find(L"\\", pos)) != std::wstring::npos) {
        escapedZipPath.replace(pos, 1, L"\\\\");
        pos += 2;
    }
    pos = 0;
    while ((pos = escapedExtractPath.find(L"\\", pos)) != std::wstring::npos) {
        escapedExtractPath.replace(pos, 1, L"\\\\");
        pos += 2;
    }
    
    std::wstring psCommand = L"powershell.exe -Command \"Expand-Archive -Path \\\"" + 
                             escapedZipPath + L"\\\" -DestinationPath \\\"" + 
                             escapedExtractPath + L"\\\" -Force\"";
    
    STARTUPINFO si = {};
    PROCESS_INFORMATION pi = {};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    
    // Create command line
    std::vector<wchar_t> cmdLine(psCommand.begin(), psCommand.end());
    cmdLine.push_back(L'\0');
    
    BOOL success = CreateProcessW(
        nullptr,
        cmdLine.data(),
        nullptr,
        nullptr,
        FALSE,
        0,
        nullptr,
        nullptr,
        &si,
        &pi
    );
    
    if (!success) {
        return false;
    }
    
    // Wait for process to complete (max 30 seconds)
    DWORD waitResult = WaitForSingleObject(pi.hProcess, 30000);
    
    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    return (waitResult == WAIT_OBJECT_0 && exitCode == 0);
}

void DistributeFiles(const std::wstring& extractPath, const std::wstring& projectPath) {
    try {
        // Extract project name from path (last directory name)
        fs::path projectPathObj(projectPath);
        std::wstring projectName = projectPathObj.filename().wstring();
        
        // Create target folders with project name prefix
        std::wstring footprintsDir = projectPath + L"\\" + projectName + L".pretty";
        std::wstring symbolsDir = projectPath + L"\\" + projectName + L"_Symbols";
        std::wstring modelsDir = projectPath + L"\\" + projectName + L"_Models";
        
        fs::create_directories(footprintsDir);
        fs::create_directories(symbolsDir);
        fs::create_directories(modelsDir);
        
        int footprintCount = 0;
        int symbolCount = 0;
        int modelCount = 0;
        
        // Scan all files and distribute by extension
        for (const auto& entry : fs::recursive_directory_iterator(extractPath)) {
            if (entry.is_regular_file()) {
                std::wstring filePath = entry.path().wstring();
                std::wstring fileName = entry.path().filename().wstring();
                std::wstring ext = entry.path().extension().wstring();
                
                std::transform(ext.begin(), ext.end(), ext.begin(), ::towlower);
                
                std::wstring destPath;
                bool shouldMove = false;
                
                if (ext == L".kicad_mod") {
                    destPath = footprintsDir + L"\\" + fileName;
                    shouldMove = true;
                    footprintCount++;
                } else if (ext == L".kicad_sym") {
                    destPath = symbolsDir + L"\\" + fileName;
                    shouldMove = true;
                    symbolCount++;
                } else if (ext == L".step" || ext == L".stp") {
                    destPath = modelsDir + L"\\" + fileName;
                    shouldMove = true;
                    modelCount++;
                }
                
                if (shouldMove) {
                    try {
                        // Overwrite if file already exists
                        if (fs::exists(destPath)) {
                            fs::remove(destPath);
                        }
                        fs::copy_file(filePath, destPath, fs::copy_options::overwrite_existing);
                    } catch (const std::exception& e) {
                        // Continue on error
                    }
                }
            }
        }
        
        std::wstringstream status;
        status << L"Completed! " << footprintCount << L" footprints, " 
               << symbolCount << L" symbols, " << modelCount << L" model files copied.";
        UpdateStatus(status.str());
        
    } catch (const std::exception& e) {
        UpdateStatus(L"Error: An error occurred while distributing files!");
    }
}

std::wstring BrowseForFolder(HWND hwnd) {
    BROWSEINFO bi = {};
    wchar_t szPath[MAX_PATH];
    
    bi.hwndOwner = hwnd;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = L"Select project folder";
    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    
    if (pidl != nullptr) {
        if (SHGetPathFromIDList(pidl, szPath)) {
            CoTaskMemFree(pidl);
            return std::wstring(szPath);
        }
        CoTaskMemFree(pidl);
    }
    
    return L"";
}

void UpdateStatus(const std::wstring& message) {
    if (g_hStatusText) {
        SetWindowTextW(g_hStatusText, message.c_str());
    }
}
