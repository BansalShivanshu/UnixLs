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

// print long ls
void printLongLs();


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
        printLs(path);
        
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

            printLs(path);
        } else if ((argv[1])[0] == '-') { // flags
            for (int i = 1; i < strlen(argv[1]); i++) { // start after '-'
                switch ((argv[1])[i])
                {
                case 'l':
                    if (hasL) {
                        printf("Flags cannot be repeated\n");
                        exit(INCOR_USAGE);
                    } else {
                        hasL = true;
                    }
                    break;

                case 'R':
                    if (hasR) {
                        printf("Flags cannot be repeated\n");
                        exit(INCOR_USAGE);
                    } else {
                        hasR = true;
                    }
                    break;

                case 'i':
                    if (hasI) {
                        printf("Flags cannot be repeated\n");
                        exit(INCOR_USAGE);
                    } else {
                        hasI = true;
                    }
                    break;
                
                default:
                    printf("the fuck!\n");
                    break;
                }
            }

            char* path = ".";
            dir = opendir(path); // no validation required for 'this' directory            

            // print with flags
            if (hasL && !hasR && !hasI) {
                printf("printing long ls\n");
                printLongLs();
            } else if (hasR && !hasL && !hasI) {
                printf("printing recursively\n");
            } else if (hasI && !hasL && !hasR) {
                printf("printing with i (not sure what 'i' is but okay?\n");
            }
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
void printLs(char *path) {
    if (!dir) {
        printf("Something went wrong!\n");
        exit(UNEXP_ERR);
    }

    while ((dp = readdir(dir)) != NULL) {
            // ((stat(dp->d_name, &buf) >= 0) && (buf.st_mode > 0) && (S_IXUSR & buf.st_mode));

                // char *idk = malloc(strlen(path) + strlen(dp->d_name) + 2);
                // strcpy(idk, path);
                // strcat(idk, "/");
                // strcat(idk, dp->d_name);

                // stat(idk, &buf);
                        // if (S_ISDIR(buf.st_mode)) printf("is a dir %s\n", idk);

                        // if ((dp->d_name)[0] != '.') {
                        //     if ((stat(dp->d_name, &buf) >= 0) && (buf.st_mode > 0) && (S_IXUSR & buf.st_mode)) { // is an executable
                        //         printf("%s is an executable!\n", dp->d_name);
                        //     } else if (stat(dp->d_name, &buf) && S_ISDIR(buf.st_mode)) { // is a dir
                        //         printf("%s is a dir!\n", dp->d_name);
                        //     } else {
                        //         printf("%s is a regular file\n", dp->d_name);
                        //     }
                        // }

            char *idk = malloc(strlen(path) + strlen(dp->d_name) + 2);
            strcpy(idk, path);
            strcat(idk, "/");
            strcat(idk, dp->d_name);
            DIR *optionalDir = opendir(idk);
            struct stat optionalBuf;
            if (optionalDir) {
                printf("%s/\n", dp->d_name);
            } else if ((stat(idk, &optionalBuf) >= 0) && (optionalBuf.st_mode > 0) && (S_IXUSR & optionalBuf.st_mode)) {
                printf("%s*\n", dp->d_name);
            } else {
                printf("%s\n", dp->d_name);
            }
            free(idk);


                // free(idk);   


            // if (shouldHaveQuotes(dp->d_name)) {
            //     if (stat(dp->d_name, &buf) == 0 && buf.st_mode && S_IXUSR){ // is an executable file
            //         printf("'%s'*  ", dp->d_name);
            //     } else if (stat(dp->d_name, &buf) && S_ISDIR(buf.st_mode)) { // is a dir
            //         printf("'%s'/  ", dp->d_name);
            //     } else {
            //         printf("'%s'  ", dp->d_name);
            //     }
            // } else {
            //     if (stat(dp->d_name, &buf) == 0 && buf.st_mode && S_IXUSR){ // is an executable file
            //         printf("%s*  ", dp->d_name);
            //     } else if (stat(dp->d_name, &buf) && S_ISDIR(buf.st_mode)) { // is a dir
            //         printf("%s/  ", dp->d_name);
            //     } else {
            //         printf("%s  ", dp->d_name);
            //     }
            // }
    }

    printf("\n");
}

// prints long ls
void printLongLs() {

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

