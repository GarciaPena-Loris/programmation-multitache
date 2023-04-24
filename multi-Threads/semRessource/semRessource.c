#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>

typedef union Semaphores {
    int val;
    struct semid_ds *buf;
    ushort *array;
} Semaphores;

int nbReelRessources = 0;

void afficheTableauSem(int idSem, int nbRessource) {
    Semaphores semaphores;
    semaphores.array = (ushort*)malloc(nbRessource* sizeof(ushort)); 
    if (semctl(idSem, nbRessource, GETALL, semaphores) == -1) {
        perror("❌ Erreur d'initialisation des sémaphores : ");
        exit(1);
    }
    printf("\033[33mTableau de sémaphores [ "); 
    for(int i = 0; i < nbRessource - 1; i++) {
        printf("%d, ", semaphores.array[i]);
    } 
    printf("%d ]\033[0m\n", semaphores.array[nbRessource - 1]);

    free(semaphores.array);
}

void printRessource() {
    printf("1: Piano\n");
    printf("2: Couteaux-Suisse\n");
    printf("3: Casquette\n");
    printf("4: Réacteur nucléaire\n");
    printf("5: Plafond\n");
    printf("6: Serrure\n");
    if (nbReelRessources > 0) {
        printf("q: Quiter\n");
    }
}

void printQuestion() {
    printf("o: oui\n");
    printf("n: non\n");
}

void selection_ressources(int nbRessource, int * tabRessource) {
    for (int i = 0; i < nbRessource; i++) {
        printf("🧰 Selectionnez la ressource souhaitez :\n");
        printRessource();
        char reponse[8];
        fgets(reponse, 8, stdin);
        if (reponse[0] == 'q') {
            break;
        }
        else {
            printf("Ressource souhaité : %d\n\n", atoi(reponse));
            tabRessource[i] = atoi(reponse) - 1;
            tabRessource = realloc(tabRessource, i * sizeof(int) + 1);
            nbReelRessources++;
        }
    }
}

void demande_ressources(int idSem, int * tabRessource) {
    printf("🔎 J'essaye de recuperer les ressources "); 
    for(int i = 0; i < nbReelRessources - 1; i++) {
        printf("%d, ", tabRessource[i]+1);
    } 
    printf("et %d.\n", tabRessource[nbReelRessources-1]+1);

    struct sembuf sops[nbReelRessources];
    for (int i = 0; i < nbReelRessources; i++) {
        sops[i].sem_num = tabRessource[i]; // attente (P)
        sops[i].sem_op = -1;
        sops[i].sem_flg = 0;
    }

    if (semop(idSem, sops, nbReelRessources) == -1) {
        perror("❌ probleme de semop ");
        exit(1);
    }
    printf("✅ Ressources récupérés !\n");
}

void travail_ressource() {
    printf("🔨 Utilisation des ressources en cours... \n");
    while(1) {
        printf("\n❓ Avez vous terminé d'utiliser les ressource ? \n");
        printQuestion();
        char reponse[8];
        fgets(reponse, 8, stdin);
        if (reponse[0] == 'o') {
            break;
        }
    }
    printf("🏁 Utilisation des ressources terminé. \n");
}

void libere_ressource(int idSem, int * tabRessource) {
    struct sembuf sops[nbReelRessources];

    for (int i = 0; i < nbReelRessources; i++) {
        sops[i].sem_num = tabRessource[i]; // incremente (V)
        sops[i].sem_op = 1;
        sops[i].sem_flg = 0;
    }

    printf("🏆 J'essaye de rendre les ressources...\n");
    if (semop(idSem, sops, nbReelRessources) == -1) {
        perror("❌ probleme de semop ");
        exit(1);
    }
    printf("✅ Ressources rendu !\n");
}

int main(int argc, char * argv[]){
    if (argc!=4) {
        printf("Nbre d'args invalide, utilisation :\n");
        printf("%s nombre-ressource fichier-pour-cle-ipc entier-pour-clé-ipc\n", argv[0]);
        exit(0);
    }
        
    int cle = ftok(argv[2], atoi(argv[3]));

    int nbRessource = atoi(argv[1]);

    int idSem = semget(cle, nbRessource, 0);

    if(idSem == -1) {
        perror("❌ erreur semget ");
        exit(-1);
    }

    printf("Sem id : %d \n", idSem);


    while (1) {
        // Init tableau demande
        int * tabRessource = malloc(sizeof(int));
        nbReelRessources = 0;

        afficheTableauSem(idSem, nbRessource); // Affichage
        
        selection_ressources(nbRessource, tabRessource); // (1) permet de choisir les ressources via une saisie
        demande_ressources(idSem, tabRessource); // (2) demande de réservation des ressources sélectionnées
        
        afficheTableauSem(idSem, nbRessource); // Affichage
        
        travail_ressource(); // (3) faire quelque chose
        libere_ressource(idSem, tabRessource); // (4) // libérer les ressources sélectionnées
        
        afficheTableauSem(idSem, nbRessource); // Affichage

        printf("\n---------- fin boucle ----------\n"); 
    }


    return 0;
}
