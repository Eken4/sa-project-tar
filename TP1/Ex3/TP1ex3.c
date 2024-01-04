/**
 * TP1 - Exercice 3 - ls
 * Programme qui affiche les métadonnées des fichiers d'un répertoire
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>     
#include <pwd.h>        
#include <grp.h>       
#include <time.h>       
#include <errno.h> 
#include <libgen.h>     // basename()

#define MAX_PATH_LENGTH 4096

// Function to display file metadata
void display_file_metadata(const char *filename) {
    struct stat file_stat;
    struct passwd *pwd;
    struct group *grp;
    struct tm *timeinfo;

    if (lstat(filename, &file_stat) == -1) {
        perror("Error reading file metadata");
        return;
    }

    // Get owner's username
    pwd = getpwuid(file_stat.st_uid);
    if (pwd == NULL) {
        perror("Error getting owner's username");
        return;
    }

    // Get group name
    grp = getgrgid(file_stat.st_gid);
    if (grp == NULL) {
        perror("Error getting group name");
        return;
    }

    // Get last modification time
    timeinfo = localtime(&file_stat.st_mtime);
    if (timeinfo == NULL) {
        perror("Error getting modification time");
        return;
    }

    // Display file metadata
    printf("%s - ", basename(strdup(filename)));

    // Display permissions as rwx
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

    // Display owner, group, size, and modification time
    printf(" %s : %s - %lld - %02d%02d%02d @ %02d:%02d\n",
           pwd->pw_name,
           grp->gr_name,
           (long long)file_stat.st_size,
           timeinfo->tm_year + 1900,
           timeinfo->tm_mon + 1,
           timeinfo->tm_mday,
           timeinfo->tm_hour,
           timeinfo->tm_min);
}


// Function to list files in a directory
void list_files(const char *dirname) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char path[MAX_PATH_LENGTH];
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        // Skip "." and ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            display_file_metadata(path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    // Check for the correct number of arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // List files in the specified directory
    list_files(argv[1]);

    return EXIT_SUCCESS;
}
