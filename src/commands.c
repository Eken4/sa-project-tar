#include "commands.h"
/**
 * \file commands.c
 * \brief Regroupe les fonctionnalités sur les archives
*/


/**
 * \fn list
 * \brief Liste les informations des fichiers contenus dans une archive
 * \param file l'archive à lire
 * \return
 */
void list(FILE *file)
{
    long int archivesize = 0;
    long int readbytes = 0;

    struct tar_header headerstruct;

    fseek(file, 0, SEEK_END);
    archivesize = ftell(file);
    fseek(file, -archivesize, SEEK_END);

    while (readbytes < archivesize)
    {
        read_header(file, &headerstruct);
        if (header_is_empty(&headerstruct))
        {
            printf("y a pu\ndist");
            return;
        }
        print_small_header_info(&headerstruct);
        long int filesize = strtol(headerstruct.size, NULL, 8);
        long int step_size = ((filesize + 511) / 512) * 512;
        fseek(file, step_size, SEEK_CUR);
        readbytes += sizeof(headerstruct) + step_size;
    }
}

/**
 * \fn extract
 * \brief Extrait le contenu d'une archive
 * \param file l'archive
 * \return
 */
void extract(FILE *file)
{
    long int archivesize = 0;
    long int readbytes = 0;

    struct tar_header headerstruct;

    fseek(file, 0, SEEK_END);
    archivesize = ftell(file);
    fseek(file, -archivesize, SEEK_END);

    while (true)
    {
        read_header(file, &headerstruct);
        if (header_is_empty(&headerstruct))
        {
            printf("y a pu\ndist");
            return;
        }
        if (strtol(headerstruct.size, NULL, 8) == 0)
        {
            folder_extract(&headerstruct);
        }
        else
            file_extract(file, &headerstruct);
    }
}

/**
 * \fn create_archive
 * \brief génère une archive
 * \param archive l'archive à créer
 * \param filenames les noms des fichiers à extraire
 * \param nb_files le nombre de fichiers à extraire
 * \return
 */
void create_archive(FILE *archive, char *filenames[], int nb_files)
{
    for (int i = 0; i < nb_files; i++)
    {
        FILE *file;

        if (!(file = fopen(filenames[i], "r")))
        {
            fprintf(stderr, "Error : tar_generate_archive() : cannot open file %s\n", filenames[i]);

            return;
        }

        add_file_to_archive(archive, file, filenames[i]);

        fclose(file);
    }

    add_end_of_archive_marker(archive);
}

/**
 * \fn file_extract
 * \brief extrait un unique fichier d'une archive
 * \param archive l'archive dont le fichier doit être extrait
 * \param header la structure contenant les informations sur le fichier à extraire
 * \return un booléen selon la réussite de l'opération
 */
bool file_extract(FILE *archive, struct tar_header *header)
{
    FILE *newfile = fopen(header->name, "w");

    long int headersize = strtol(header->size, NULL, 8);

    if (!newfile)
        return false;

    int *temp = malloc(512);

    for (long int i = 0; i < headersize; i += 512)
    {
        if ((headersize)-i > 511)
        {
            fread(temp, 512, 1, archive);
            fwrite(temp, 512, 1, newfile);
        }
        else
        {
            fread(temp, headersize - i, 1, archive);
            fwrite(temp, headersize - i, 1, newfile);
            fseek(archive, i + 512 - headersize, SEEK_CUR);
            break;
        }
    }

    free(temp);

    fclose(newfile);
    return true;
}

/**
 * \fn folder_extract
 * \brief extrait un unique répertoire d'une archive
 * \param header
 * \return un booléen selon la réussite de l'opération
 */
bool folder_extract(struct tar_header *header)
{
    if (strtol(header->size, NULL, 8) > 0)
    {
        return false;
    }

    return mkdir(header->name, strtol(header->mode, NULL, 8));
}

/**
 * \fn add_file_to_archive
 * \brief ajoute un unique fichier à une archive 
 * \param archive l'archive où ajouter le fichier
 * \param file le fichier à ajouter
 * \param filename le nom du fichier
 * \return un booléen selon la réussite de l'opération
 */
bool add_file_to_archive(FILE *archive, FILE *file, char *filename)
{
    struct tar_header header = fill_header(file, filename);

    if (header_is_empty(&header))
    {
        return false;
    }

    int *temp = malloc(BLOCKSIZE);

    long int file_size = strtol(header.size, NULL, 8);

    fwrite(&header, BLOCKSIZE, 1, archive);

    for (long int i = 0; i < file_size; i += BLOCKSIZE)
    {
        fread(temp, BLOCKSIZE, 1, file);
        fwrite(temp, BLOCKSIZE, 1, archive);
    }

    add_end_of_archive_marker(archive);

    free(temp);

    return true;
}