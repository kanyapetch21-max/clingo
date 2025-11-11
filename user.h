#ifndef USER_H
#define USER_H

typedef struct {
    char username[32];
    int xp;
    int level;
    int hearts;
    int streak;
} User;

int load_user(const char* name, User* out);
int save_user(const User* u);
void add_xp(User* u, int add);
void lose_heart(User* u);

#endif
