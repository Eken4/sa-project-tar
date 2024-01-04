/**
 * TP1 - Exercice 2 - Print reverse
 * Programme qui affiche le contenu d'un fichier en ordre inverse
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define STDOUT 1
#define STDERR 2
#define MAX_PATH_LENGTH 4096

#define USAGE_SYNTAX "[OPTIONS] -i INPUT"
#define USAGE_PARAMS "OPTIONS:\n\
  -i, --input  INPUT_FILE  : input file\n\
***\n\
  -v, --verbose : enable *verbose* mode\n\
  -h, --help    : display this help\n\
"

void print_usage(char *bin_name)
{
    dprintf(1, "USAGE: %s %s\n\n%s\n", bin_name, USAGE_SYNTAX, USAGE_PARAMS);
}

void free_if_needed(void *to_free)
{
    if (to_free != NULL)
        free(to_free);
}

char *dup_optarg_str()
{
    char *str = NULL;

    if (optarg != NULL)
    {
        str = strndup(optarg, MAX_PATH_LENGTH);

        if (str == NULL)
            perror(strerror(errno));
    }

    return str;
}

static struct option binary_opts[] =
    {
        {"help", no_argument, 0, 'h'},
        {"verbose", no_argument, 0, 'v'},
        {"input", required_argument, 0, 'i'},
        {0, 0, 0, 0}};

const char *binary_optstr = "hvi:";

int main(int argc, char **argv)
{
    short int is_verbose_mode = 0;
    char *bin_input_param = NULL;

    int opt = -1;
    int opt_idx = -1;

    while ((opt = getopt_long(argc, argv, binary_optstr, binary_opts, &opt_idx)) != -1)
    {
        switch (opt)
        {
        case 'i':
            if (optarg)
            {
                bin_input_param = dup_optarg_str();
            }
            break;
        case 'v':
            is_verbose_mode = 1;
            break;
        case 'h':
            print_usage(argv[0]);

            free_if_needed(bin_input_param);

            exit(EXIT_SUCCESS);
        default:
            break;
        }
    }

    if (bin_input_param == NULL)
    {
        dprintf(STDERR, "Bad usage! See HELP [--help|-h]\n");

        free_if_needed(bin_input_param);

        exit(EXIT_FAILURE);
    }

    // Open the input file
    int input_fd = open(bin_input_param, O_RDONLY);

    if (input_fd == -1)
    {
        perror("Error opening input file");
        free_if_needed(bin_input_param);
        exit(EXIT_FAILURE);
    }

    // Move the file pointer to the end of the file
    off_t file_size = lseek(input_fd, 0, SEEK_END);

    if (file_size == -1)
    {
        perror("Error seeking to the end of the file");
        close(input_fd);
        free_if_needed(bin_input_param);
        exit(EXIT_FAILURE);
    }

    // Read the file in reverse order and print to STDOUT
    char buffer[1];
    for (off_t i = file_size - 1; i >= 0; i--)
    {
        lseek(input_fd, i, SEEK_SET);
        ssize_t read_status = read(input_fd, buffer, 1);

        if (read_status == -1)
        {
            perror("Error reading file");
            close(input_fd);
            free_if_needed(bin_input_param);
            exit(EXIT_FAILURE);
        }

        // Print the character to STDOUT
        write(STDOUT, buffer, 1);
    }

    // Close the input file
    close(input_fd);

    // Free allocated data
    free_if_needed(bin_input_param);

    return EXIT_SUCCESS;
}
