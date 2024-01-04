#include "utils.h"
/**
 * \file utils.c
 * \brief Regroupe les fonctions utilitaires pour les opérations sur les archives
*/


/**
 * \fn
 * \brief
 * \param
 * \return
 */
int read_header(FILE *file, struct tar_header *header)
{
    if (file == NULL)
    {
        printf("Mauvais fichier en entrée\n");
        return -1;
    }

    if (fread(header, sizeof(struct tar_header), 1, file))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * \fn
 * \brief
 * \param
 * \return
 */
void print_small_header_info(struct tar_header *header)
{
    printf("Nom du fichier : %s\n", header->name);
    printf("Taille du fichier : %lu octets\n", strtol(header->size, NULL, 8));
}

/**
 * \fn
 * \brief
 * \param
 * \return
 */
void print_full_header_info(struct tar_header *header)
{
    printf("Nom du fichier : %s\n", header->name);
    printf("Mode d'accès : %s\n", header->mode);
    printf("ID du propriétaire : %s\n", header->owner);
    printf("ID du groupe : %s\n", header->group);
    printf("Taille du fichier : %lu octets\n", strtol(header->size, NULL, 8));

    time_t mtime = strtol(header->mtime, NULL, 8);
    struct tm *tm_info = localtime(&mtime);
    char mtime_str[20];
    // strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%d %H:%M:%S", tm_info);
    strftime(mtime_str, sizeof(mtime_str), "%d-%m-%Y %H:%M:%S", tm_info);

    printf("Date de modification : %s\n", mtime_str);
    printf("Somme de contrôle : %s\n", header->checksum);
    printf("Type de fichier : %c\n", header->type);
    printf("Nom du lien : %s\n", header->linkname);

    printf("\n");
}

/**
 * \fn
 * \brief
 * \param
 * \return
 */
bool header_is_empty(struct tar_header *header)
{
    if (header->name[0] == '\0' && header->size[0] == '\0' && header->type == '\0')
        return true;
    else
        return false;
}

/**
 * \fn
 * \brief
 * \param
 * \return
 */
void add_end_of_archive_marker(FILE *archive)
{
    char data[BLOCKSIZE];

    memset(data, 0, BLOCKSIZE);

    for (int i = 0; i < 3; i++)
    {
        fwrite(data, BLOCKSIZE, 1, archive);
    }
}

/**
 * \fn
 * \brief
 * \param
 * \return
 */
unsigned int calculate_checksum_header(char *header_string)
{
    unsigned int sum = 0;
    unsigned int size = BLOCKSIZE;

    for (int i = 0; i < size; i++)
    {
        sum += (unsigned int)header_string[i];
    }

    return sum;
}

/**
 * \fn
 * \brief
 * \param
 * \return
 */
struct tar_header fill_header(FILE *file, char *filename)
{
    struct tar_header header;

    int fd = fileno(file);
    struct stat file_stat;

    if (fstat(fd, &file_stat) != 0)
    {
        printf("Mauvaises métadonnées\n");
        return header;
    }

    strncpy(header.name, filename, 100);
    header.name[sizeof(header.name) - 1] = '\0';

    // copier les métadonnées obligatoires
    sprintf(header.mode, "%o", file_stat.st_mode);
    sprintf(header.owner, "%o", file_stat.st_uid);
    sprintf(header.group, "%o", file_stat.st_gid);
    sprintf(header.size, "%lo", file_stat.st_size);
    sprintf(header.mtime, "%lo", file_stat.st_mtime);

    memset(header.checksum, ' ', sizeof(header.checksum));

    header.type = '0'; // si lien archive dossier etc

    unsigned int checksum = calculate_checksum_header((char *)&header);

    sprintf(header.checksum, "%06o\0 ", checksum);

    return header;
}