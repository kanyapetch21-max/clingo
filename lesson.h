#ifndef LESSON_H
#define LESSON_H

typedef struct {
    int id;
    char unit_name[64];
} Unit;

typedef struct {
    int id;
    int unit_id;
    int type;           // 0 = multiple choice, 1 = typing
    char prompt[128];
    char answer[64];
    char choiceA[64];
    char choiceB[64];
    char choiceC[64];
    char choiceD[64];
    int mistakes;       // how many times answered wrong
} Item;

int load_units(Unit units[], int max_units);
int load_items(Item items[], int max_items);

void load_progress(Item items[], int item_count);
void save_progress(const Item items[], int item_count);

int ask_exercise(const Item* it);

void flashcard_review_mistakes(const Item items[], int item_count);

#endif
