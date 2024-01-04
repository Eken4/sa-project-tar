/**
 * TP2 - Exercice 1 - fork
 * Programme qui crée un processus fils et affiche les PID
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    // Création d'un processus fils
    pid_t pid = fork();

    if (pid == -1) {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Code du processus fils
        printf("Je suis le processus fils avec PID %d et mon père a PID %d\n", getpid(), getppid());

        // Sortie avec le dernier chiffre du PID comme code de retour
        exit(getpid() % 10);
    } else {  // Code du processus père
        printf("Je suis le processus père avec PID %d, mon fils a PID %d\n", getpid(), pid);

        // Attente de la terminaison du fils
        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            // Affichage du code de retour du fils
            printf("Mon fils s'est terminé avec le code %d\n", WEXITSTATUS(status));
        } else {
            perror("Le processus fils ne s'est pas terminé correctement");
        }
    }

    return 0;
}
