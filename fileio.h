#ifndef FILEIO_H
#define FILEIO_H

#include <stdio.h>

#define DATA_DIR "data"

FILE* fopen_data(const char* name, const char* mode);

#endif
