#include "fileio.h"
#include <stdio.h>

FILE* fopen_data(const char* name, const char* mode) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", DATA_DIR, name);

    FILE* f = fopen(path, mode);
    if (!f) {
        return NULL;
    }
    return f;
}
