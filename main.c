#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"
#include "lesson.h"
#include "ui.h"
#include "util.h"

#define MAX_UNITS 64
#define MAX_ITEMS 1024

void draw_header(void) {
    printf("______________________________________\n");
    printf("|                                    |\n");
    printf("|            C-LINGO v1.0            |\n");
    printf("|____________________________________|\n");
}

void draw_menu(void) {
    printf("| 1.) Start Learning                 |\n");
    printf("| 2.) Profile                        |\n");
    printf("| 3.) Flashcards (Mistakes Only)     |\n");
    printf("| 0.) Exit                           |\n");
    printf("|____________________________________|\n");
}

static void show_profile(const User* u) {
    draw_header();
    printf("|             PROFILE                |\n");
    printf("|____________________________________|\n");
    printf("| User:   %-25s|\n", u->username);
    printf("| Level:  %-25d|\n", u->level);
    printf("| XP:     %-25d|\n", u->xp);
    printf("| Hearts: %-25d|\n", u->hearts);
    printf("| Streak: %-25d|\n", u->streak);
    printf("|____________________________________|\n");
    pause_enter();
}

static void learn_by_unit(User* u,
                          Unit units[], int unit_count,
                          Item items[], int item_count) {
    clear_screen();
    draw_header();
    printf("|             LESSONS                |\n");
    printf("|____________________________________|\n");
    for (int i = 0; i < unit_count; i++) {
        printf("| %2d.) %-28s|\n", units[i].id, units[i].unit_name);
    }
    printf("|  0.) Back                         |\n");
    printf("|____________________________________|\n");
    printf("Select unit id: ");

    int id;
    if (scanf("%d", &id) != 1) return;
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
    if (id == 0) return;

    clear_screen();
    draw_header();
    printf("|         PRACTICE UNIT %d           |\n", id);
    printf("|____________________________________|\n");

    int correct = 0, total = 0;

    for (int i = 0; i < item_count; i++) {
        if (items[i].unit_id == id) {
            total++;
            int ok = ask_exercise(&items[i]);
            if (ok) {
                add_xp(u, 10);
                correct++;
            } else {
                lose_heart(u);
                items[i].mistakes++;
                printf("Hearts left: %d\n", u->hearts);
                if (u->hearts <= 0) {
                    printf("No hearts left! Come back later.\n");
                    break;
                }
            }
        }
    }

    save_progress(items, item_count);

    printf("\nYou got %d / %d correct!\n", correct, total);
    pause_enter();
}

int main(void) {
    Unit units[MAX_UNITS];
    Item items[MAX_ITEMS];

    int unit_count = load_units(units, MAX_UNITS);
    int item_count = load_items(items, MAX_ITEMS);

    if (unit_count <= 0 || item_count <= 0) {
        printf("Error: cannot load data files.\n");
        return 1;
    }

    load_progress(items, item_count);

    char name[32];
    clear_screen();
    draw_header();
    printf("Enter your username: ");
    if (!fgets(name, sizeof(name), stdin)) return 1;
    trim_newline(name);

    User user;
    load_user(name, &user);

    int choice;
    while (1) {
        clear_screen();
        draw_header();
        draw_menu();
        printf("Select: ");
        if (scanf("%d", &choice) != 1) break;
        int c; while ((c = getchar()) != '\n' && c != EOF) {}

        switch (choice) {
            case 1:
                learn_by_unit(&user, units, unit_count, items, item_count);
                break;
            case 2:
                clear_screen();
                show_profile(&user);
                break;
            case 3:
                clear_screen();
                flashcard_review_mistakes(items, item_count);
                break;
            case 0:
                save_user(&user);
                save_progress(items, item_count);
                clear_screen();
                draw_header();
                printf("|          THANK YOU FOR PLAYING     |\n");
                printf("|____________________________________|\n");
                printf("|  See you again, %s!%-11s|\n", user.username, "");
                printf("|____________________________________|\n");
                return 0;
            default:
                printf("Invalid choice.\n");
                pause_enter();
        }
    }

    return 0;
}
