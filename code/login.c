// login.c - a password check with a buffer overflow bug.
// For learning only. Don't write code like this!

#include <stdio.h>
#include <string.h>

struct account {
    char password[8];   // room for 8 characters
    int authorized;     // 0 = no, anything else = yes
};

int main(void) {
    struct account user;
    user.authorized = 0;

    printf("Password: ");
    scanf("%s", user.password);   // BUG: no limit on how much it reads

    if (strcmp(user.password, "secret") == 0) {
        user.authorized = 1;
    }

    printf("(authorized = %d)\n", user.authorized);

    if (user.authorized) {
        printf("Access granted!\n");
    } else {
        printf("Access denied.\n");
    }
    return 0;
}
