#include "lesson.h"
#include "fileio.h"
#include "util.h"
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void draw_header(void);

// Load units
int load_units(Unit units[], int max_units) {
    FILE* f = fopen_data("units.txt", "r");
    if (!f) {
        printf("Error: cannot open units.txt\n");
        return 0;
    }

    char line[256];
    int count = 0;

    while (count < max_units && fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0') continue;

        char* token = strtok(line, "|");
        if (!token) continue;
        units[count].id = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(units[count].unit_name, token, sizeof(units[count].unit_name));
        units[count].unit_name[sizeof(units[count].unit_name) - 1] = '\0';

        count++;
    }

    fclose(f);
    return count;
}

// Load items
int load_items(Item items[], int max_items) {
    FILE* f = fopen_data("items.txt", "r");
    if (!f) {
        printf("Error: cannot open items.txt\n");
        return 0;
    }

    char line[512];
    int count = 0;

    while (count < max_items && fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (line[0] == '\0') continue;

        char* token = strtok(line, "|");
        if (!token) continue;
        items[count].id = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        items[count].unit_id = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        items[count].type = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(items[count].prompt, token, sizeof(items[count].prompt));
        items[count].prompt[sizeof(items[count].prompt) - 1] = '\0';

        token = strtok(NULL, "|");
        if (!token) continue;
        strncpy(items[count].answer, token, sizeof(items[count].answer));
        items[count].answer[sizeof(items[count].answer) - 1] = '\0';

        token = strtok(NULL, "|");
        strncpy(items[count].choiceA, token ? token : "", sizeof(items[count].choiceA));
        items[count].choiceA[sizeof(items[count].choiceA) - 1] = '\0';

        token = strtok(NULL, "|");
        strncpy(items[count].choiceB, token ? token : "", sizeof(items[count].choiceB));
        items[count].choiceB[sizeof(items[count].choiceB) - 1] = '\0';

        token = strtok(NULL, "|");
        strncpy(items[count].choiceC, token ? token : "", sizeof(items[count].choiceC));
        items[count].choiceC[sizeof(items[count].choiceC) - 1] = '\0';

        token = strtok(NULL, "|");
        strncpy(items[count].choiceD, token ? token : "", sizeof(items[count].choiceD));
        items[count].choiceD[sizeof(items[count].choiceD) - 1] = '\0';

        items[count].mistakes = 0;

        count++;
    }

    fclose(f);
    return count;
}

// Progress load/save
void load_progress(Item items[], int item_count) {
    FILE* f = fopen_data("progress.txt", "r");
    if (!f) return;

    int id, m;
    while (fscanf(f, "%d %d", &id, &m) == 2) {
        for (int i = 0; i < item_count; i++) {
            if (items[i].id == id) {
                items[i].mistakes = m;
                break;
            }
        }
    }

    fclose(f);
}

void save_progress(const Item items[], int item_count) {
    FILE* f = fopen_data("progress.txt", "w");
    if (!f) {
        printf("Warning: cannot write progress.txt\n");
        return;
    }

    for (int i = 0; i < item_count; i++) {
        if (items[i].mistakes > 0) {
            fprintf(f, "%d %d\n", items[i].id, items[i].mistakes);
        }
    }

    fclose(f);
}

// Exercises
static int ask_mcq(const Item* it) {
    printf("\n%s\n", it->prompt);
    printf("A) %s\n", it->choiceA);
    printf("B) %s\n", it->choiceB);
    printf("C) %s\n", it->choiceC);
    printf("D) %s\n", it->choiceD);
    printf("Select (A-D): ");

    char choice;
    if (scanf(" %c", &choice) != 1) return 0;
    int c; while ((c = getchar()) != '\n' && c != EOF) {}

    const char* selected = NULL;
    switch (choice) {
        case 'A': case 'a': selected = it->choiceA; break;
        case 'B': case 'b': selected = it->choiceB; break;
        case 'C': case 'c': selected = it->choiceC; break;
        case 'D': case 'd': selected = it->choiceD; break;
        default:
            printf("Invalid option.\n");
            return 0;
    }

    if (stricmp_ci(selected, it->answer) == 0) {
        printf("✅ Correct!\n");
        return 1;
    } else {
        printf("❌ Wrong. Answer: %s\n", it->answer);
        return 0;
    }
}

static int ask_typing(const Item* it) {
    printf("\nTranslate: %s -> ", it->prompt);
    char buf[128];
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    trim_newline(buf);

    if (stricmp_ci(buf, it->answer) == 0) {
        printf("✅ Correct!\n");
        return 1;
    } else {
        printf("❌ Wrong. Answer: %s\n", it->answer);
        return 0;
    }
}

int ask_exercise(const Item* it) {
    if (it->type == 0)
        return ask_mcq(it);
    else
        return ask_typing(it);
}

// Flashcards for frequent mistakes
void flashcard_review_mistakes(const Item items[], int item_count) {
    draw_header();
    printf("|     FLASHCARDS: FREQUENT MISTAKES  |\n");
    printf("|____________________________________|\n");

    int shown = 0;
    int c;

    for (int i = 0; i < item_count; i++) {
        if (items[i].mistakes >= 1) {
            shown++;
            printf("\nWord: %s\n", items[i].prompt);
            printf("(Press Enter to see the answer)");
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("→ Meaning: %s\n", items[i].answer);
            printf("--------------------------------------\n");
        }
    }

    if (shown == 0) {
        printf("\nNo mistakes yet. Keep practicing!\n");
    }

    pause_enter();
}
