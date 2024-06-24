#include <windows.h>
#include <stdio.h>

#define IDC_LISTBOX 101
#define IDC_EDITBOX 102
#define IDC_BTN_RUN 103
#define IDC_BTN_ADD 104
#define IDC_BTN_RUN_NOTEPAD 105
#define INI_FILE "progman.ini"
//gcc -o progman progman.c -mwindows
// Função para carregar o conteúdo do arquivo ini na listbox
void LoadIniFile(HWND hwndList) {
    FILE *file = fopen(INI_FILE, "r");
    if (file) {
        char line[256];
        SendMessage(hwndList, LB_RESETCONTENT, 0, 0); // Limpa a listbox
        while (fgets(line, sizeof(line), file)) {
            // Remove nova linha
            line[strcspn(line, "\r\n")] = 0;
            SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)line);
        }
        fclose(file);
    }
}

// Função para adicionar uma nova linha ao arquivo ini
void AddLineToIniFile(const char *line) {
    FILE *file = fopen(INI_FILE, "a");
    if (file) {
        fprintf(file, "%s\n", line);
        fclose(file);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndList, hwndEdit, hwndBtnRun, hwndBtnAdd, hwndBtnRunNotepad;
    switch (uMsg) {
        case WM_CREATE: {
            hwndList = CreateWindowW(L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_VSCROLL, 
                                     10, 10, 360, 200, hwnd, (HMENU)IDC_LISTBOX, NULL, NULL);
            hwndEdit = CreateWindowW(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 
                                     10, 220, 360, 25, hwnd, (HMENU)IDC_EDITBOX, NULL, NULL);
            hwndBtnRun = CreateWindowW(L"BUTTON", L"Run", WS_CHILD | WS_VISIBLE, 
                                       380, 10, 80, 30, hwnd, (HMENU)IDC_BTN_RUN, NULL, NULL);
            hwndBtnAdd = CreateWindowW(L"BUTTON", L"Add", WS_CHILD | WS_VISIBLE, 
                                       380, 50, 80, 30, hwnd, (HMENU)IDC_BTN_ADD, NULL, NULL);
            hwndBtnRunNotepad = CreateWindowW(L"BUTTON", L"Run Notepad", WS_CHILD | WS_VISIBLE, 
                                              380, 90, 100, 30, hwnd, (HMENU)IDC_BTN_RUN_NOTEPAD, NULL, NULL);
            // Verifica se o arquivo ini existe, se não, cria com uma linha inicial
            FILE *file = fopen(INI_FILE, "r");
            if (!file) {
                file = fopen(INI_FILE, "w");
                if (file) {
                    fprintf(file, "notepad progman.ini\n");
                    fclose(file);
                }
            } else {
                fclose(file);
            }
            LoadIniFile(hwndList);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDC_BTN_RUN_NOTEPAD) {
                system("notepad progman.ini");
            } else if (LOWORD(wParam) == IDC_BTN_RUN) {
                char command[256];
                GetWindowText(hwndEdit, command, sizeof(command));
                system(command);
            } else if (LOWORD(wParam) == IDC_BTN_ADD) {
                char command[256];
                GetWindowText(hwndEdit, command, sizeof(command));
                AddLineToIniFile(command);
                LoadIniFile(hwndList);
            } else if (LOWORD(wParam) == IDC_LISTBOX && HIWORD(wParam) == LBN_DBLCLK) {
                int index = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                if (index != LB_ERR) {
                    char command[256];
                    SendMessage(hwndList, LB_GETTEXT, index, (LPARAM)command);
                    system(command);
                }
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MyWindowClass";
    wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 0)); // Fundo amarelo
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "Progman", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                               CW_USEDEFAULT, CW_USEDEFAULT, 500, 300, NULL, NULL, hInstance, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
