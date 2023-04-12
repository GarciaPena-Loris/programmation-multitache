#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

typedef union Semaphores {
    int val;
    struct semid_ds *buf;
    ushort *array;
} Semaphores;

void rdv(int nProc, int idSem, int nbRdv) {
    // Initialisation du random du wait
    srand(getpid()); 

    for (int i = 0; i < nbRdv; i++) {
        // Calcul d'un temps aléatoire
        int wait = (rand() % 6) + 1;
        printf("\033[%im [%i] Calcul d'environ %i secondes ⏱️ \033[0m \n", nProc + 31, nProc, wait);
        sleep(wait);
        printf("\033[%im [%i] Calcul terminé, mise à jour du point de rendez-vous ✅ \033[0m \n", nProc + 31, nProc);
        
        // On retire 1 du tableau de sémaphores car ce processus est arrivé au RDV
        struct sembuf op[] = {  { i, -1, 0 },
                                { i, 0, 0 } };
        if (semop(idSem, op, 1) == -1)
            exit(1);

        // Récupération de la valeur de la sémaphore actuelle pour affichage
        int semValue = semctl(idSem, i, GETVAL, 0);
        if (semValue == -1) 
            exit(1);
            
        printf("\033[%im [%i] Valeur actuelle de la sémaphore : %i \033[0m \n", nProc + 31, nProc, semValue);
        if (semValue == 0) {
            if (i == nbRdv-1) {
                printf("\033[%im [%i] Je suis la derniere sémaphore, c'était le dernier calcul 👑\033[0m \n", nProc + 31, nProc);
            }
            else {
                printf("\033[%im [%i] Je suis la derniere sémaphore, il reste %d calcul, je lance le calcul suivant 🏁\033[0m \n", nProc + 31, nProc, nbRdv-i-1);
            }
        }  

        // Affiche le tableau actuel
        Semaphores semaphores;
        semaphores.array = (ushort*)malloc(nbRdv * sizeof(ushort)); 
        if (semctl(idSem, nbRdv, GETALL, semaphores) == -1) {
            perror("Erreur d'initialisation des sémaphores ");
            exit(1);
        }
        printf("\033[%im [%i] Valeurs des actuelle de tableau de sémaphores :\033[0m [ ", nProc + 31, nProc); 
        for(int i = 0; i < nbRdv - 1; i++) {
            printf("%d, ", semaphores.array[i]);
        } 
        printf("%d ] \n", semaphores.array[nbRdv - 1]);


        // Tant que la sémaphore actuelle n'est pas à 0, on attend.
        // Synchroniser les processus pour qu'ils attendent tous que les autres aient terminé avant de passer à l'étape suivante
        if (semop(idSem, op + 1, 1) == -1) {
            exit(1);
        }
    }

    exit(0);
}

int main(int argc, char * argv[]){
    if (argc != 5) {
        printf("Utilisation: %s nombreRdv nombreProcessus fichierCle entierCle\n", argv[0]);
        exit(1);
    }

    // Création de la sémaphore.
    key_t clesem = ftok(argv[3], atoi(argv[4]));
    int nbRdv = atoi(argv[1]);
    int nbProc = atoi(argv[2]);
    int idSem = semget(clesem, nbRdv, IPC_CREAT|IPC_EXCL|0600);
    
    if(idSem == -1){
        perror("Erreur lors du semget ");
        exit(1);
    }

    printf("Sémaphore crée avec l'ID : %d\n", idSem);

    // Initialisation des sémaphores 
    ushort tabinit[nbRdv];
    for (int i = 0; i < nbRdv; i++) {
        tabinit[i] = nbProc;
    }

    Semaphores semaphores;
    semaphores.array = tabinit;

    // Set
    if (semctl(idSem, nbRdv, SETALL, semaphores) == -1) {
        perror("Erreur lors de l'initialisation des sémaphores ");
        // Supprime la sémaphore
        if (semctl(idSem, 0, IPC_RMID) == -1) {
            perror("Erreur fatale lors de l'exécution du processus ");
        }
        exit(1);
    }

    semaphores.array = (ushort*)malloc(nbRdv * sizeof(ushort)); 

    // Get
    if (semctl(idSem, nbRdv, GETALL, semaphores) == -1) {
        perror("Erreur d'initialisation des sémaphores ");
        if (semctl(idSem, 0, IPC_RMID) == -1) {
            perror("Erreur fatale lors de l'exécution du processus ");
        }
        exit(1);
    }
    
    printf("Valeurs des sempahores apres initialisation [ "); 
    for(int i = 0; i < nbRdv - 1; i++) {
        printf("%d, ", semaphores.array[i]);
    } 
    printf("%d ] \n", semaphores.array[nbRdv - 1]);

    for (int i = 0; i < nbProc; i++) {
        if (fork() == 0) {
            rdv(i, idSem, nbRdv);
        }
    }

    // Attente des enfants 
    while (wait(NULL) > 0);
    printf("Fin du rendez-vous !\n"); 

    free(semaphores.array);
    if (semctl(idSem, 0, IPC_RMID) == -1) {
        perror("Erreur lors de la fin du processus ");
    }
    return 0;
}
