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
#include <pwd.h>
#include <grp.h>
#include <time.h>


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
void printLs(char *path);

// print long ls
void printLongLs(char *path);

// print recursively
void printRec(char *path, bool eptFirstLine, bool lng);

// print with inode number
void printInode(char *path);

// print long ls with inode value
void printLongInode(char *path, bool inode);

// print with all flags
void printAllFlags(char *path, bool eptFirstLine);

// print recurcively with inode values
void printRecInode(char *path, bool eptFirstLine);



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


/*
    Main Function
*/
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
                    printf("Invalid flag Usage\n");
                    exit(INCOR_USAGE);
                }
            }

            char* path = ".";
            dir = opendir(path); // no validation required for 'this' directory            

            // print with flags

            // singular flags
            if (hasL && !hasR && !hasI) {
                printLongLs(path);
            } else if (hasR && !hasL && !hasI) {
                // PRINT RECURSIVELY
                printRec(path, false, false);
            } else if (hasI && !hasL && !hasR) {
                printInode(path);
            }

            // double combinations
            if (hasL && hasR && !hasI) { // -lR and permutations
                printRec(path, false, true);
            } else if (!hasL && hasR && hasI) { // -Ri and permutations
                printRecInode(path, false);
            } else if (hasL && !hasR && hasI) { // -li and permutations
                printLongInode(path, true);
            }

            // all flags
            if (hasL && hasR && hasI) {
                printAllFlags(path, false);
            }
        }

        break;

    case 3:
        // this checks for the flags (arg2)
        // and for the directory (arg3)

        // if second arg is not a flag
        // or if the third arg is a flag
        if ((argv[1])[0] != '-' || (argv[2])[0] == '-') {
            printf("Incorrect usage\n");
            printf("Usage: ./UnixLs <flags> <dir/path>\n");
            exit(INCOR_USAGE);
        }

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
                printf("Invalid flag Usage\n");
                exit(INCOR_USAGE);
            }
        }

        char *openPath = argv[2];
        if (!validateDir(openPath)) {
            printf("%s is not a directory\n", openPath);
            exit(INCOR_USAGE);
        }

        dir = opendir(openPath);

        // print with flags

        // singular flags
        if (hasL && !hasR && !hasI) {
            printLongInode(openPath, false);
            // printLongLs(openPath);
        } else if (hasR && !hasL && !hasI) {
            // PRINT RECURSIVELY
            printRec(openPath, false, false);
        } else if (hasI && !hasL && !hasR) {
            printInode(openPath);
        }

        // double combinations
        if (hasL && hasR && !hasI) { // -lR and permutations
            printRec(openPath, false, true);
        } else if (!hasL && hasR && hasI) { // -Ri and permutations
            printRecInode(openPath, false);
        } else if (hasL && !hasR && hasI) { // -li and permutations
            printLongInode(openPath, true);
        }

        // all flags
        if (hasL && hasR && hasI) {
            printAllFlags(openPath, false);
        }

        break;

    
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
            // skip file if hidden
            if ((dp->d_name)[0] == '.') continue;

            char *idk = malloc(strlen(path) + strlen(dp->d_name) + 2);
            strcpy(idk, path);
            strcat(idk, "/");
            strcat(idk, dp->d_name);
            
            DIR *optionalDir = opendir(idk);
            struct stat optionalBuf;

            if (optionalDir) {
                // if is a directory
                shouldHaveQuotes(dp->d_name) ? printf("'%s'/  ", dp->d_name) : printf("%s/  ", dp->d_name);
            } else if ((stat(idk, &optionalBuf) >= 0) && (optionalBuf.st_mode > 0) && (S_IXUSR & optionalBuf.st_mode)) {
                // if is an executable file
                shouldHaveQuotes(dp->d_name) ? printf("'%s'*  ", dp->d_name) : printf("%s*  ", dp->d_name);
            } else {
                // else is a normal file
                shouldHaveQuotes(dp->d_name) ? printf("'%s'  ", dp->d_name) : printf("%s  ", dp->d_name);
            }
            
            free(idk);
    }

    printf("\n");
}

// prints long ls
void printLongLs(char *path) {
    if (!dir) {
        printf("Something went wrong!\n");
        exit(UNEXP_ERR);
    }

    while ((dp = readdir(dir)) != NULL) {
            // skip file if hidden
            if ((dp->d_name)[0] == '.') continue;

            stat(dp->d_name, &buf);

            // (column 1) print file permissions
            printf((S_ISDIR(buf.st_mode)) ? "d" : "-");
            printf((buf.st_mode & S_IRUSR) ? "r" : "-");
            printf((buf.st_mode & S_IWUSR) ? "w" : "-");
            printf((buf.st_mode & S_IXUSR) ? "x" : "-");
            printf((buf.st_mode & S_IRGRP) ? "r" : "-");
            printf((buf.st_mode & S_IWGRP) ? "w" : "-");
            printf((buf.st_mode & S_IXGRP) ? "x" : "-");
            printf((buf.st_mode & S_IROTH) ? "r" : "-");
            printf((buf.st_mode & S_IWOTH) ? "w" : "-");
            printf((buf.st_mode & S_IXOTH) ? "x" : "-");
            printf("  ");

            // (column 2) print number of hard links to the file
            printf("%lu  ", buf.st_nlink);

            // (column 3) print the owner of the file
            printf("%s  ", getpwuid(buf.st_uid)->pw_name);

            // (column 4) print the group of the file
            printf("%s  ", getgrgid(buf.st_gid)->gr_name);

            // (column 5) print size of the file in characters
            printf("%10ld  ", buf.st_size);

            // (column 6) print the date
            // format: mmm dd yyyy hh:mm
            char *dateStr = malloc(sizeof(char) * 20);
            time_t *time = &buf.st_mtime;
            strftime(dateStr, 22, "%b %e %Y %R", localtime(time));
            printf("%s  ", dateStr);
            free(dateStr);

            // (column 7) print file name
            char *idk = malloc(strlen(path) + strlen(dp->d_name) + 2);
            strcpy(idk, path);
            strcat(idk, "/");
            strcat(idk, dp->d_name);

            DIR *optionalDir = opendir(idk);
            struct stat optionalBuf;

            if (optionalDir) {
                // if is a directory
                shouldHaveQuotes(dp->d_name) ? printf("'%s'/\n", dp->d_name) : printf("%s/\n", dp->d_name);
            } else if ((stat(idk, &optionalBuf) >= 0) && (optionalBuf.st_mode > 0) && (S_IXUSR & optionalBuf.st_mode)) {
                // if is an executable file
                shouldHaveQuotes(dp->d_name) ? printf("'%s'*\n", dp->d_name) : printf("%s*\n", dp->d_name);
            } else {
                // else is a normal file
                shouldHaveQuotes(dp->d_name) ? printf("'%s'\n", dp->d_name) : printf("%s\n", dp->d_name);
            }

            free(idk);
    }
}

// print recursively
void printRec(char *path, bool eptFirstLine, bool lng) {
    if (eptFirstLine) {
        printf("\n%s:\n", path);
    } else {
        printf("%s:\n", path);
    }


    dir = opendir(path); // no validation required for 'this' directory      

    if (lng) {
        printLongInode(path, false);
    } else {
        printLs(path);
    }

    struct dirent *dir2;
    DIR *dp2 = opendir(path);

    if (dp2) {
        // printf("%s is a directory\n", path);

        while ((dir2 = readdir(dp2)) != NULL) {
            if ((dir2->d_name)[0] == '.' || strcmp(dir2->d_name, ".") == 0 || strcmp(dir2->d_name, "..") == 0) continue;

            if (dir2->d_type == 4) { // is a directory
                char *newPath = malloc(strlen(path) + 1 + strlen(dir2->d_name) + 1);
                strcpy(newPath, path);
                strcat(newPath, "/");
                strcat(newPath, dir2->d_name);

                printRec(newPath, true, lng);
            
                free(newPath);
            }
        }
    } 

    closedir(dp2);
}

// print with inode number
void printInode(char *path) {
    if (!dir) {
        printf("Something went wrong!\n");
        exit(UNEXP_ERR);
    }

    while ((dp = readdir(dir)) != NULL) {
        // skip file if hidden
        if ((dp->d_name)[0] == '.') continue;

        // (column 7) print file name
        char *idk = malloc(strlen(path) + strlen(dp->d_name) + 2);
        strcpy(idk, path);
        strcat(idk, "/");
        strcat(idk, dp->d_name);

        stat(idk, &buf);
    
        printf("%lu ", buf.st_ino);

        DIR *optionalDir = opendir(idk);
        struct stat optionalBuf;

        if (optionalDir) {
            // if is a directory
            shouldHaveQuotes(dp->d_name) ? printf("'%s'/  ", dp->d_name) : printf("%s/  ", dp->d_name);
        } else if ((stat(idk, &optionalBuf) >= 0) && (optionalBuf.st_mode > 0) && (S_IXUSR & optionalBuf.st_mode)) {
            // if is an executable file
            shouldHaveQuotes(dp->d_name) ? printf("'%s'*  ", dp->d_name) : printf("%s*  ", dp->d_name);
        } else {
            // else is a normal file
            shouldHaveQuotes(dp->d_name) ? printf("'%s'  ", dp->d_name) : printf("%s  ", dp->d_name);
        }

        free(idk);
    }

    printf("\n");
}

// print long ls with inode values
void printLongInode(char *path, bool inode) {
    if (!dir) {
        printf("Something went wrong!\n");
        exit(UNEXP_ERR);
    }

    while ((dp = readdir(dir)) != NULL) {
            // skip file if hidden
            if ((dp->d_name)[0] == '.') continue;

            char *idk = malloc(strlen(path) + strlen(dp->d_name) + 2);
            strcpy(idk, path);
            strcat(idk, "/");
            strcat(idk, dp->d_name);

            DIR *optionalDir = opendir(idk);
            struct stat optionalBuf;

            stat(idk, &buf);

            if (inode) {
                // column 0 - print inode number
                printf("%lu  ", buf.st_ino);
            }

            // (column 1) print file permissions
            printf((S_ISDIR(buf.st_mode)) ? "d" : "-");
            printf((buf.st_mode & S_IRUSR) ? "r" : "-");
            printf((buf.st_mode & S_IWUSR) ? "w" : "-");
            printf((buf.st_mode & S_IXUSR) ? "x" : "-");
            printf((buf.st_mode & S_IRGRP) ? "r" : "-");
            printf((buf.st_mode & S_IWGRP) ? "w" : "-");
            printf((buf.st_mode & S_IXGRP) ? "x" : "-");
            printf((buf.st_mode & S_IROTH) ? "r" : "-");
            printf((buf.st_mode & S_IWOTH) ? "w" : "-");
            printf((buf.st_mode & S_IXOTH) ? "x" : "-");
            printf("  ");

            // (column 2) print number of hard links to the file
            printf("%lu  ", buf.st_nlink);

            // (column 3) print the owner of the file
            printf("%s  ", getpwuid(buf.st_uid)->pw_name);

            // (column 4) print the group of the file
            printf("%s  ", getgrgid(buf.st_gid)->gr_name);

            // (column 5) print size of the file in characters
            printf("%10ld  ", buf.st_size);

            // (column 6) print the date
            // format: mmm dd yyyy hh:mm
            char *dateStr = malloc(sizeof(char) * 20);
            time_t *time = &buf.st_mtime;
            strftime(dateStr, 22, "%b %e %Y %R", localtime(time));
            printf("%s  ", dateStr);
            free(dateStr);

            // (column 7) print file name
            if (optionalDir) {
                // if is a directory
                shouldHaveQuotes(dp->d_name) ? printf("'%s'/\n", dp->d_name) : printf("%s/\n", dp->d_name);
            } else if ((stat(idk, &optionalBuf) >= 0) && (optionalBuf.st_mode > 0) && (S_IXUSR & optionalBuf.st_mode)) {
                // if is an executable file
                shouldHaveQuotes(dp->d_name) ? printf("'%s'*\n", dp->d_name) : printf("%s*\n", dp->d_name);
            } else {
                // else is a normal file
                shouldHaveQuotes(dp->d_name) ? printf("'%s'\n", dp->d_name) : printf("%s\n", dp->d_name);
            }

            free(idk);
    }
}

// print with all the flags
void printAllFlags(char *path, bool eptFirstLine) {
    if (eptFirstLine) {
        printf("\n%s:\n", path);
    } else {
        printf("%s:\n", path);
    }

    dir = opendir(path); // no validation required for 'this' directory            
    printLongInode(path, true);

    struct dirent *dir2;
    DIR *dp2 = opendir(path);

    if (dp2) {
        // printf("%s is a directory\n", path);

        while ((dir2 = readdir(dp2)) != NULL) {
            if ((dir2->d_name)[0] == '.' || strcmp(dir2->d_name, ".") == 0 || strcmp(dir2->d_name, "..") == 0) continue;

            if (dir2->d_type == 4) { // is a directory
                char *newPath = malloc(strlen(path) + 1 + strlen(dir2->d_name) + 1);
                strcpy(newPath, path);
                strcat(newPath, "/");
                strcat(newPath, dir2->d_name);

                printAllFlags(newPath, true);
            
                free(newPath);
            }
        }
    } 

    closedir(dp2);
}

// print recurcively with inode values
void printRecInode(char *path, bool eptFirstLine) {
    if (eptFirstLine) {
        printf("\n%s:\n", path);
    } else {
        printf("%s:\n", path);
    }


    dir = opendir(path); // no validation required for 'this' directory      
    printInode(path);

    struct dirent *dir2;
    DIR *dp2 = opendir(path);

    if (dp2) {
        // printf("%s is a directory\n", path);

        while ((dir2 = readdir(dp2)) != NULL) {
            if ((dir2->d_name)[0] == '.' || strcmp(dir2->d_name, ".") == 0 || strcmp(dir2->d_name, "..") == 0) continue;

            if (dir2->d_type == 4) { // is a directory
                char *newPath = malloc(strlen(path) + 1 + strlen(dir2->d_name) + 1);
                strcpy(newPath, path);
                strcat(newPath, "/");
                strcat(newPath, dir2->d_name);

                printRecInode(newPath, true);
            
                free(newPath);
            }
        }
    } 

    closedir(dp2);   
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

