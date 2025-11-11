#include "ui.h"
#include <stdio.h>
#include <stdlib.h>

void pause_enter(void) {
    int c;
    printf("\nPress Enter to continue...");
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
