/*
    Header Files
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>


/*
    Values
*/
#define INCOR_USAGE 1001
#define UNEXP_ERR 1002


/*
    Function Signatures
*/

    /*
        Printing functions
    */
// prints the basic ls
void printLs();


    /* 
        Helper Functions
    */
// returns true if a file name should have quotes,
// returns false otherwise
bool shouldHaveQuotes(char *str);

// Validates if a directory exists or not
// returns true if exists,
// false otherwise
bool validateDir(char *name);


/*
    Variables
*/

DIR *dir;
struct dirent *dp;
struct stat buf;

int main(int argc, char *argv[]) {
    if (argc > 3) {
        printf("Incorrect Usage\n");
        printf("Usage: ./UnixLs -flag(s) dir\n");
        exit(INCOR_USAGE);
    }

    bool hasL = false;
    bool hasR = false;
    bool hasI = false;

    switch (argc)
    {
    case 1: ;
        char* path = ".";
        dir = opendir(path); // no validation required for 'this' directory
        printLs();
        
        break;

    case 2:
        // if dir name is passed as the second argument
        if ((argv[1])[0] != '-') { // not a flag arg
            // open the specified directory
            char *path = argv[1];

            if (!validateDir(path)) {
                printf("Not a valid directory\n");
                exit(INCOR_USAGE);
            }

            // open the directory
            dir = opendir(path);

            printLs();
        } else if ((argv[1])[0] == '-') { // flags
            for (int i = 1; i < strlen(argv[0]); i++) { // start after '-'
                if ((argv[0])[i] == 'l') {
                    if (hasL) {
                        printf("Flags cannot be repeated\n");
                        exit(INCOR_USAGE);
                    } else {
                        hasL = true;
                    }
                }

                if ((argv[0])[i] == 'R') {
                    if (hasR) {
                        printf("Flags cannot be repeated\n");
                        exit(INCOR_USAGE);
                    } else {
                        hasR = true;
                    }
                }

                if ((argv[0])[i] == 'i') {
                    if (hasI) {
                        printf("Flags cannot be repeated\n");
                        exit(INCOR_USAGE);
                    } else {
                        hasI = true;
                    }
                }
            }

            // print with flags
        }

        break;

    case 3:
        // this checks for the flags (arg2)
        // and for the directory (arg3)

    
    default:
        printf("Incorrect usage or,\n");
        printf("Something went wrong!\n");
        exit(INCOR_USAGE);
    }

    // validate the arguments
    /*
    if (strlen(argv[1]) < 2 || strlen(argv[1]) > 4) {
        printf("Invalidate flags\n");
        return 1;
    }
    */

    closedir(dir);

    return 0;
}


/*
    Function Implimentations
*/

// prints the basic ls
void printLs() {
    if (!dir) {
        printf("Something went wrong!\n");
        exit(UNEXP_ERR);
    }

    while ((dp = readdir(dir)) != NULL) {
        if (dp->d_name[0] != '.') {
            if (shouldHaveQuotes(dp->d_name)) {
                printf("'%s'  ", dp->d_name);
            } else {
                printf("%s  ", dp->d_name);
            }
        }
    }

    printf("\n");
}

// returns true if a file name should have quotes,
// returns false otherwise
bool shouldHaveQuotes(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ') {
            return true;
        }
    }
    return false;
}

// Validates if a directory exists or not
// returns true if exists,
// false otherwise
bool validateDir(char *name) {
    stat(name, &buf);
    if (S_ISDIR(buf.st_mode)) {
        return true;
    }
    return false;
}

