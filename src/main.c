#include <stdio.h>
#include <unistd.h>

#include "commands.h"

int main(int argc, char *argv[])
{
    int opt;
    char *archivename;
    char *filenames[argc];
    int nb_files = 0;

    int list_opt = 0, extract_opt = 0, archive_opt = 0;

    while ((opt = getopt(argc, argv, ":e:f:l:hc:")) != -1)
    {
        switch (opt)
        {
        case 'c':
            archive_opt = 1;
            archivename = optarg;
            break;
        case 'l':
            list_opt = 1;
            archivename = optarg;
            break;
        case 'e':
            extract_opt = 1;
            archivename = optarg;
            break;
        default:
            break;
        }
    }

    FILE *archive;

    char *mode = "r+b";

    if (archive_opt)
    {
        mode = "w+b";
    }

    if (!(archive = fopen(archivename, mode)))
    {
        fprintf(stderr, "Error : main() : cannot fopen archive\n");
        fprintf(stderr, strerror(errno));
        return EXIT_FAILURE;
    }

    if (list_opt)
    {
        list(archive);
    }

    if (extract_opt)
    {
        extract(archive);
    }

    if (archive_opt)
    {
        create_archive(archive, filenames, nb_files);
    }

    fclose(archive);

    return EXIT_SUCCESS;
}