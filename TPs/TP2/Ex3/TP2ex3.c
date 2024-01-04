/**
 Écrivez un programme C équivalent au script shell suivant : ps eaux | grep "^root " > /dev/null && echo "root est connecté"
 Votre programme devra ainsi prendre en charge les actions suivantes :
    - Réaliser les exécutions de ps et grep en utilisant la primitive execlp dans l’ordre des priorités usuelles.
    - Mettre en place les redirections des entrées/sorties nécessaires grâce à la primitive dup (ou dup2).
    - Réaliser l’affichage final avec la primitive write.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Création du descripteur de pipe
    int pipe_fd[2];
    
    // Création du pipe
    if (pipe(pipe_fd) == -1) {
        perror("Erreur lors de la création du pipe");
        exit(EXIT_FAILURE);
    }

    // Création du processus fils
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    }

    // processus fils
    if (child_pid == 0) { 
        // Fermeture du descripteur d'écriture
        close(pipe_fd[1]);

        // Redirection de la sortie standard vers le pipe
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);

        // Le processus fils lit depuis le pipe
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
            // Écrire "root est connecté" si une ligne correspond à "^root "
            if (write(STDOUT_FILENO, "root est connecté\n", 18) == -1) {
                perror("Erreur lors de l'écriture");
                exit(EXIT_FAILURE);
            }
            break;  // Sortir après la première ligne correspondante
        }

        // En cas d'erreur de lecture
        if (bytesRead == -1) {
            perror("Erreur lors de la lecture du pipe");
            exit(EXIT_FAILURE);
        }

        // Le processus fils se termine
        exit(EXIT_SUCCESS);
    } 

    // processus père
    else {
        // Fermeture du descripteur de lecture
        close(pipe_fd[0]);

        // Redirection de la sortie standard vers le pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);

        // Exécution de la commande ps eaux
        execlp("ps", "ps", "eaux", NULL);

        // En cas d'échec de execlp
        perror("Erreur lors de l'exécution de la commande ps eaux");

        exit(EXIT_FAILURE);
    }
    return 0;
}
