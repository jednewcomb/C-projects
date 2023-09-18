#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <time.h>

#define BUFFER 512
#define CHAR_BUFFER 20000
#define DEFAULT_MODE 0777
#define COPYMODE 0644

char *addSlash(char *string)
{
    char *formatted;
    char *slash = "/";
    formatted = strcat(string, slash);
    return formatted;
}

void copyFiles(char *toBeCopied, char *out)
{
    int src_fd, dst_fd, n, err;
    char *src_path;
    char buf[CHAR_BUFFER];
    char in[CHAR_BUFFER];
    char inOrig[CHAR_BUFFER];
    char outOrig[CHAR_BUFFER];

    if (getcwd(in, sizeof(in)) == NULL)
    {
        fprintf(stderr, "Null");
    }
    strcpy(inOrig, in);
    strcpy(outOrig, out);

    if (strcmp(toBeCopied, "*") > 0)
    {
        addSlash(in);
        strcat(in, toBeCopied);
        src_path = in;
        addSlash(out);
        strcat(out, toBeCopied);

        src_fd = open(src_path, O_RDONLY);
        dst_fd = open(out, O_CREAT | O_WRONLY);
        while (1)
        {
            err = read(src_fd, buf, CHAR_BUFFER);
            if (err == -1)
            {
                printf("Error reading file.\n");
                exit(1);
            }
            n = err;

            if (n == 0)
                break;
            err = write(dst_fd, buf, n);
            if (err == -1)
            {
                printf("Error writing to file.\n");
                exit(1);
            }
        }
        close(src_fd);
        close(dst_fd);
    }

    else
    {

        struct dirent **namelist;
        struct stat stats;
        int dirlen;
        int i = 2;

        dirlen = scandir(".", &namelist, NULL, alphasort);
        if (dirlen == -1)
        {
            fprintf(stderr, "Error");
            exit(1);
        }

        while (i < dirlen)
        {
            char *name = namelist[i]->d_name;
            stat(name, &stats);
            strcpy(in, inOrig);
            strcpy(out, outOrig);
            if (!S_ISDIR(stats.st_mode))
            {
                addSlash(in);
                strcat(in, name);
                src_path = in;
                addSlash(out);
                strcat(out, name);

                src_fd = open(src_path, O_RDONLY);
                dst_fd = open(out, O_CREAT | O_WRONLY);

                while (1)
                {
                    err = read(src_fd, buf, CHAR_BUFFER);
                    if (err == -1)
                    {
                        fprintf(stderr, "Error reading file.\n");
                        exit(1);
                    }
                    n = err;
                    if (n == 0)
                        break;
                    err = write(dst_fd, buf, n);
                    if (err == -1)
                    {
                        fprintf(stderr, "Error writing file.\n");
                        exit(1);
                    }
                }
                close(src_fd);
                close(dst_fd);
            }
            free(namelist[i]);
            i++;
        }
        free(namelist);
    }
}

void showCurrentDirectory(char *direc)
{
    struct dirent **namelist;
    struct stat stats;
    int n;
    int i = 2;

    printf("%s contents:\n", direc);
    printf("--------------------------------------------");
    printf("\nSIZE\t DATE\t  TIME\t        NAME\n");

    n = scandir(direc, &namelist, NULL, alphasort);

    if (n == -1)
    {
        fprintf(stderr, "Error");
        exit(1);
    }

    while (i < n)
    {
        char *name = namelist[i]->d_name;
        stat(name, &stats);
        if (!S_ISDIR(stats.st_mode))
        {
            char *t = ctime(&stats.st_mtime);
            if (t[strlen(t) - 1] == '\n')
                t[strlen(t) - 1] = '\0';
            printf("%-6d %s %s\n", stats.st_size, t, name);
            free(namelist[i]);
        }
        i++;
    }
    free(namelist);
    printf("--------------------------------------------\n");
}

int createDirectory(char *path)
{
    if (mkdir(path, DEFAULT_MODE) == -1)
    {
        printf("Error creating dir\n");
        return 1;
    }
    return 0;
}

int isDirectory(char *path)
{
    struct stat stats;
    stat(path, &stats);

    if (S_ISDIR(stats.st_mode))
    {
        printf("Directory exists: %s\n", path);
        return 1;
    }
    printf("Directory doesn't exist. Typo?");
    return 0;
}

char *getUserName()
{
    struct passwd *password;
    password = getpwuid(getuid());
    return password->pw_name;
}

int main(int argc, char *argv[])
{
    if (argc > 2 || argc < 1)
    {
        printf("Incorrect Argument Length. Typo?\n");
        return (1);
    }

    char submitPath[BUFFER];
    char coursePath[BUFFER];
    char submitPathOrig[BUFFER];

    if (getcwd(submitPath, sizeof(submitPath)) == NULL)
    {
        fprintf(stderr, "Null starting directory");
        exit(1);
    }

    // we'll want a copy of our original
    // directory for printing it later
    strcpy(submitPathOrig, submitPath);
    addSlash(submitPath);
    strcat(submitPath, "submit");
    isDirectory(submitPath);

    printf("Welcome! Please enter course code (lowercase): \n");

    // get info from user, cut out newline character
    // that is so kindly put there by fgets, scanf is
    // is likely better but I've heard that its bad?
    char input[BUFFER];
    fgets(input, sizeof(input), stdin);
    int len = strlen(input);
    if (input[len - 1] == '\n')
        input[len - 1] = 0;

    strcpy(coursePath, submitPath);
    addSlash(coursePath);
    strcat(coursePath, input);

    if (!isDirectory(coursePath))
    {
        createDirectory(coursePath);
    }

    char checkPath[BUFFER];
    strcpy(checkPath, coursePath);
    addSlash(checkPath);
    strcat(checkPath, getUserName());

    createDirectory(checkPath);
    printf("Directory created: %s\n", checkPath);
    printf("What is the name of the assignment?\n");

    char assignment[BUFFER];
    fgets(assignment, sizeof(assignment), stdin);
    len = strlen(assignment);
    if (assignment[len - 1] == '\n')
        assignment[len - 1] = 0;

    // I should have made this a function LOL
    char assignPath[BUFFER];
    strcpy(assignPath, checkPath);
    addSlash(assignPath);
    strcat(assignPath, assignment);

    printf("Directory created: %s\n", assignPath);
    createDirectory(assignPath);
    showCurrentDirectory(submitPathOrig);

    char toSubmit[BUFFER];
    printf("Names of files to submit? * for all\n");
    scanf("%s", toSubmit);

    copyFiles(toSubmit, assignPath);
    showCurrentDirectory(assignPath);
}