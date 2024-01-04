#ifndef UTILS__H
#define UTILS__H

/**
 * \file utils.h
 * \brief Regroupe les fonctions utilitaires pour les opérations sur les archives
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#include "typedef.h"

/**
 * \def taille d'un bloc pour la création d'archive
*/
#define BLOCKSIZE 512

int read_header(FILE *file, struct tar_header *header);

void print_small_header_info(struct tar_header *header);

void print_full_header_info(struct tar_header *header);

bool header_is_empty(struct tar_header *header);

void add_end_of_archive_marker(FILE *archive);

unsigned int calculate_checksum_header(char *header_string);

struct tar_header fill_header(FILE *file, char *filename);

#endif // UTILS__H