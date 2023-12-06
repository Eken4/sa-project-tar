#include <stdio.h>
#include <unistd.h>

#include "tar.h"

int tar_read_header(FILE* file, struct tar_header* header){
    if(file == NULL){
        printf("Bad file\n");
        return -1;
    }

    if(fread(header, sizeof(struct tar_header), 1, file)){
        return 1;
    }
    else{
        return 0;
    }

}

void tar_list(FILE* file){
    long int archivesize=0;
    long int readbytes=0;

    struct tar_header headerstruct;

    fseek(file, 0, SEEK_END);
    archivesize=ftell(file);
    fseek(file, -archivesize, SEEK_END);

    while(readbytes<archivesize){
        tar_read_header(file, &headerstruct);
        if(header_is_empty(&headerstruct)){
            printf("y a pu\ndist");
            return;
        }
        print_small_header_info(&headerstruct);
        long int filesize = strtol(headerstruct.size, NULL, 8);
        long int step_size = ((filesize + 511)/512)*512;
        fseek(file, step_size, SEEK_CUR);
        readbytes+=sizeof(headerstruct)+step_size;
    }
}

void print_small_header_info(struct tar_header* header){
    printf("Nom du fichier : %s\n", header->name);
    printf("Taille du fichier : %lu octets\n", strtol(header->size, NULL, 8));
}

void print_full_header_info(struct tar_header* header) 
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

bool header_is_empty(struct  tar_header* header)
{
    if (header->name[0]=='\0' && header->size[0]=='\0' && header->type=='\0') return true;
    else return false;
}
