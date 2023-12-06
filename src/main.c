#include <stdio.h>
#include <unistd.h>

#include "tar.h"

int main(int argc, char *argv[]){
    int opt;
    char* filename = "./archive.tar";

    int list=0;

    while((opt=getopt(argc, argv, ":lf:")) != -1){

    }

    FILE* file;

    if(!(file = fopen(filename, "rb"))){
        printf("File not found");
        return EXIT_FAILURE;
    }

    if(list){
        printf("File not ok");
        // tar_list(file);
    }

    tar_list(file);

    printf("a l'affut\ndist\n");

    fclose(file);


    return EXIT_SUCCESS;
}

//Lire les headers
// - Lire un header
//   - Ouvrir le fichier
//   --> fopen
//   - Lire le header
//   --> fread sur header & sizeof header