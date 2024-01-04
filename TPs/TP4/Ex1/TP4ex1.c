#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// Déclaration du compteur
int compteur = 0;

// Fonction de gestion du signal SIGINT
void gestionnaire_SIGINT(int signal) {
    compteur++;
    printf("Nombre de signaux SIGINT reçus : %d\n", compteur);
}

// Fonction de gestion du signal SIGTERM
void gestionnaire_SIGTERM(int signal) {
    printf("Programme terminé. Nombre total de signaux SIGINT reçus : %d\n", compteur);
    _exit(0);
}

int main() {

    // Affichage des informations
    printf("\nPour envoyer un signal SIGINT, tapez ctrl+C ici ou utiliser la commande kill -2 %d dans un autre terminal\n", getpid() );
    printf("Pour envoyer un signal SIGTERM, utiliser la commande kill -15 %d dans un autre terminal\n", getpid());

    if (signal(SIGINT, gestionnaire_SIGINT) == SIG_ERR) {
        perror("Erreur lors de la configuration du gestionnaire SIGINT");
        return 1;
    }
    if (signal(SIGTERM, gestionnaire_SIGTERM) == SIG_ERR) {
        perror("Erreur lors de la configuration du gestionnaire SIGTERM");
        return 1;
    }

    // Boucle
    while (1) {
        pause();
    }

    return 0;
}
