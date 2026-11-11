// login_fixed.c - same program, but reading input safely.

#include <stdio.h>
#include <string.h>

struct account {
    char password[8];
    int authorized;
};

int main(void) {
    struct account user;
    user.authorized = 0;

    printf("Password: ");
    // FIX: fgets never writes more than sizeof(user.password) bytes
    fgets(user.password, sizeof(user.password), stdin);
    user.password[strcspn(user.password, "\n")] = '\0';  // drop the newline

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
