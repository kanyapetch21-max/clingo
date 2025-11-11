#include "user.h"
#include "fileio.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void set_defaults(User* u, const char* name) {
    strncpy(u->username, name, sizeof(u->username));
    u->username[sizeof(u->username) - 1] = '\0';
    u->xp = 0;
    u->level = 1;
    u->hearts = 5;
    u->streak = 0;
}

int load_user(const char* name, User* out) {
    FILE* f = fopen_data("users.txt", "r");
    if (!f) {
        set_defaults(out, name);
        return 1;
    }

    char line[256];
    int found = 0;

    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0') continue;

        char* token = strtok(line, "|");
        if (!token) continue;

        if (strcmp(token, name) == 0) {
            set_defaults(out, name);

            token = strtok(NULL, "|"); if (token) out->xp = atoi(token);
            token = strtok(NULL, "|"); if (token) out->level = atoi(token);
            token = strtok(NULL, "|"); if (token) out->hearts = atoi(token);
            token = strtok(NULL, "|"); if (token) out->streak = atoi(token);

            found = 1;
            break;
        }
    }

    fclose(f);

    if (!found)
        set_defaults(out, name);

    return 1;
}

int save_user(const User* u) {
    FILE* f_read = fopen_data("users.txt", "r");
    char lines[256][256];
    int count = 0;

    if (f_read) {
        while (count < 256 && fgets(lines[count], sizeof(lines[count]), f_read)) {
            trim_newline(lines[count]);
            if (lines[count][0] != '\0')
                count++;
        }
        fclose(f_read);
    }

    FILE* f_write = fopen_data("users.txt", "w");
    if (!f_write) {
        printf("Error: cannot write users.txt\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        char tmp[256];
        strncpy(tmp, lines[i], sizeof(tmp));
        tmp[sizeof(tmp) - 1] = '\0';

        char* name = strtok(tmp, "|");
        if (name && strcmp(name, u->username) == 0) {
            continue;
        }
        fprintf(f_write, "%s\n", lines[i]);
    }

    fprintf(f_write, "%s|%d|%d|%d|%d\n",
            u->username, u->xp, u->level, u->hearts, u->streak);

    fclose(f_write);
    return 1;
}

void add_xp(User* u, int add) {
    u->xp += add;
    while (u->xp >= 100) {
        u->xp -= 100;
        u->level++;
        printf("\n🎉 Level Up! You are now level %d!\n", u->level);
    }
}

void lose_heart(User* u) {
    if (u->hearts > 0) {
        u->hearts--;
    }
}
