#ifndef COMMANDS__H
#define COMMANDS__H

/**
 * \file commands.h
 * \brief Regroupe les fonctionnalités sur les archives
 */

#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "utils.h"

void list(FILE *file);

void extract(FILE *file);

void create_archive(FILE *archive, char *filenames[], int nb_files);

bool file_extract(FILE *archive, struct tar_header *header);

bool folder_extract(struct tar_header *header);

bool add_file_to_archive(FILE *archive, FILE *file, char *filename);

#endif // COMMANDS__H