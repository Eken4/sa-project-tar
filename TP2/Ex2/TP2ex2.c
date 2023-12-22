/**
 * TP2 - Exercice 2 - Redirection de flux standard
 * Programme qui crée un processus fils, ferme le descripteur de fichier STDOUT, crée un fichier temporaire 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Étape 1
    printf("Message from parent: %s\n", argv[1]);

    // Étape 2
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    // Processus fils
    if (pid == 0) { 
        // Affiche son PID
        printf("Child PID: %d\n", getpid());

        // Ferme le descripteur 1 (STDOUT)
        if (close(1) == -1) {
            perror("close : Error closing STDOUT");
            exit(EXIT_FAILURE);
        }

        // Ouvre en création et écriture le fichier temporaire /tmp/proc-exercise
        int fd = mkstemp("/tmp/proc-exerciseXXXXXX");
        // vérifie si le fichier temporaire a été créé avec succès
        if (fd == -1) {
            perror("mkstemp : Error creating temporary file (/tmp/proc-exercise))");
            exit(EXIT_FAILURE);
        }

        // Affiche le numéro du descripteur du fichier ouvert
        printf("Child File Descriptor: %d\n", fd);

        // Exécute le programme affiché par son père lors de l'étape 1
        execlp(argv[1], argv[1], NULL);

        // In case of failure
        perror("execlp");
        exit(EXIT_FAILURE);

    } 
    // Processus parent
    else {  
        // Affiche son PID
        printf("Parent PID: %d\n", getpid());

        // Attend la fin du fils
        waitpid(pid, NULL, 0);

        // Affiche un message quelconque avant de terminer
        printf("That's All Folks!\n");
    }


    /*
    // Étape 3 (descripteur 2 fermé)
    int fd2 = mkstemp("/tmp/proc-exercise-3");  // Crée et ouvre le fichier temporaire pour la partie 3
    printf("Parent File Descriptor for Step 3: %d\n", fd2);

    // Redirige le descripteur de fichier vers la sortie d'erreur standard (STDERR)
    if (dup2(fd2, 2) == -1) {
        perror("dup2 for Step 3");
        exit(EXIT_FAILURE);
    }

    // Exécute le programme spécifié par le père pour la partie 3
    execlp(argv[1], argv[1], NULL);

    // En cas d'échec de l'exécution
    perror("execlp for Step 3");
    exit(EXIT_FAILURE);
    */

    return 0;
}
