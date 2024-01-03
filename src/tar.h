#ifndef TAR__H
#define TAR__H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "typedef.h"

int tar_read_header(FILE* file, struct tar_header* header);

void tar_extract(FILE* file);

void tar_list(FILE* file);

void print_small_header_info(struct tar_header* header);

void print_full_header_info(struct tar_header* header);

bool header_is_empty(struct  tar_header* header);

bool file_extract(FILE* archive, struct tar_header* header);

bool folder_extract(struct tar_header* header);

void add_file_to_archive(FILE* archive, FILE* file);

struct tar_header fill_header(FILE* archive, char[] filename);

#endif //TAR__H