/*
    Header Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>

void printLs();
bool shouldHaveQuotes(char *str);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printLs();
        return 0;
    }

    bool hasI = false;
    bool hasL = false;
    bool hasR = false;

    // validate the arguments
    if (strlen(argv[1]) < 2 || strlen(argv[1]) > 4) {
        printf("Invalidate flags\n");
        return;
    }

    return 0;
}

void printLs() {
    DIR *dir = opendir(".");
    if (!dir) {
        printf("error: something went wrong!\n");
        exit(1);
    }

    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_name[0] != '.') {
            if (shouldHaveQuotes(dp->d_name)) {
                printf("'%s'\n", dp->d_name);
            } else {
                printf("%s\n", dp->d_name);
            }
        }
    }

    // close the directory
    closedir(dir);
}

bool shouldHaveQuotes(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ') {
            return true;
        }
    }
    return false;
}