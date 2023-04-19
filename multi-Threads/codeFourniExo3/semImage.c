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

void traitementImage(int numProc, int nbZone, int idSem, int nbSem, int * tabMem) {
    // Initialisation du random du wait
    srand(getpid()); 
    
    if (numProc == 0) { // Je suis le premier processus
        printf("\033[%im [%i] Je suis le premiere processus 🍼 \033[0m \n\n", numProc + 32, numProc+1);
    }
    else if (numProc != nbSem) {
        printf("\033[%im [%i] Je suis le %deme processus 🍼 \033[0m \n\n", numProc + 32, numProc+1, numProc+1); 
    }
    else {
        printf("\033[%im [%i] Je suis le dernier processus 🍼 \033[0m \n\n", numProc + 32, numProc+1); 
    }

    for (int i = 0; i < nbZone; i++) {
        struct sembuf sops[3];
        sops[0].sem_num = numProc;
        sops[0].sem_op = (i+1);           
        sops[0].sem_flg = 0;

        sops[1].sem_num = numProc;   
        sops[1].sem_op = -(i+1);         
        sops[1].sem_flg = 0;

        sops[2].sem_num = numProc-1;   
        sops[2].sem_op = 0;         
        sops[2].sem_flg = 0;
        
        if (numProc == 0) { // Je suis le premier processus
            printf("\n\033[%im [%i] Début de traitement de la zone %d 🖼️ \033[0m \n", numProc + 32, numProc+1, i+1);
            // Modifie 
            if (semop(idSem, &sops[0], 1) == -1) {
                perror(" probleme de semop ");
                exit(1);
            }

            // Récupération de la valeur de la sémaphore actuelle pour affichage
            int semValue = semctl(idSem, numProc, GETVAL);
            if (semValue == -1) {
                perror(" probleme de semctl ");
                exit(1);
            }
                
            printf("\033[%im [%i] J'ai mis à jour le planning de zones, valeur actuelle de la sémaphore : %i \033[0m \n", numProc + 32, numProc+1, semValue);

            // Traitement d'un temps aléatoire
            int wait = (rand() % 6) + 1;
            printf("\033[%im [%i] Je modifie la zone %d, ça va me prendre %d secondes ⏱️ \033[0m \n", numProc + 32, numProc+1, i+1, wait);
            sleep(wait);

            // Modifie la zone partagee
            tabMem[i] += wait;

            printf("\033[%im [%i] Traitement terminé, mise à jour du planning de zones ✅\033[0m \n", numProc + 32, numProc+1);

            if (semop(idSem, &sops[1], 1) == -1) {
                perror(" probleme de semop ");
                exit(1);
            }

            // Récupération de la valeur de la sémaphore actuelle pour affichage
            semValue = semctl(idSem, numProc, GETVAL, 0);
            if (semValue == -1) {
                perror(" probleme de semctl ");
                exit(1);
            }
                
            printf("\033[%im [%i] J'ai mis à jour le planning de zones, valeur actuelle de la sémaphore : %i \033[0m \n", numProc + 32, numProc+1, semValue);

            if (i == nbZone - 1) {
                printf("\033[%im [%i] J'ai finis de mettre a jour toutes les zones 🏆\033[0m \n", numProc + 32, numProc+1);
            }
        }

        else if (numProc != nbSem) { // Processus autre que le premier et le dernier
            printf("\033[%im [%i] Début de traitement de la zone %d 🖼️ \033[0m \n", numProc + 32, numProc+1, i+1);

            // Modifie 
            printf("\033[%im [%i] J'attends que ce soit mon tour de modifier 😴 \033[0m \n", numProc + 32, numProc+1);
            if (semop(idSem, &sops[2], 1) == -1) {
                perror(" probleme de semop ");
                exit(1);
            }

            printf("\033[%im [%i] C'est bon, c'est a moi ! \033[0m \n", numProc + 32, numProc+1);
            if (semop(idSem, &sops[0], 1) == -1) {
                perror(" probleme de semop ");
                exit(1);
            }

            // Récupération de la valeur de la sémaphore actuelle pour affichage
            int semValue = semctl(idSem, numProc, GETVAL, 0);
            if (semValue == -1) {
                perror(" probleme de semctl ");
                exit(1);
            }
                
            printf("\033[%im [%i] J'ai mis à jour le planning de zones, valeur actuelle de la sémaphore : %i \033[0m \n", numProc + 32, numProc+1, semValue);

            // Traitement d'un temps aléatoire
            int wait = (rand() % 6) + 5;
            printf("\033[%im [%i] Je modifie la zone %d, ça va me prendre %d secondes ⏱️ \033[0m \n", numProc + 32, numProc+1, i+1, wait);
            sleep(wait);

            // Modifie la zone partagee
            tabMem[i] += wait;
            
            printf("\033[%im [%i] Traitement terminé ✅, mise à jour du planning de zones \033[0m \n", numProc + 32, numProc+1);

            if (semop(idSem, &sops[1], 1) == -1) {
                perror(" probleme de semop ");
                exit(1);
            }

            // Récupération de la valeur de la sémaphore actuelle pour affichage
            semValue = semctl(idSem, numProc, GETVAL, 0);
            if (semValue == -1) {
                perror(" probleme de semctl ");
                exit(1);
            }
                
            printf("\033[%im [%i] J'ai met à jour le planning de zones, valeur actuelle de la sémaphore : %i \033[0m \n", numProc + 32, numProc+1, semValue);

            if (i == nbZone - 1) {
                printf("\033[%im [%i] J'ai finis de mettre a jour toutes les zones 🏆 \033[0m \n", numProc + 32, numProc+1);
            }
        }
        else {
            printf("\033[%im [%i] Début de traitement de la zone %d 🖼️ \033[0m \n", numProc + 32, numProc+1, i+1);

            // Modifie 
            printf("\033[%im [%i] J'attends que ce soit mon tour de modifier 😴 \033[0m \n", numProc + 32, numProc+1);
            if (semop(idSem, &sops[2], 1) == -1) {
                perror(" probleme de semop ");
                exit(1);
            }

            printf("\033[%im [%i] C'est bon, c'est a moi ! \033[0m \n", numProc + 32, numProc+1);

            // Traitement d'un temps aléatoire
            int wait = (rand() % 6) + 5;
            printf("\033[%im [%i] Je modifie la zone %d, ça va me prendre %d secondes ⏱️ \033[0m \n", numProc + 32, numProc+1, i+1, wait);
            sleep(wait);

            // Modifie la zone partagee
            tabMem[i] += wait;
            
            printf("\033[%im [%i] Traitement terminé ✅ \033[0m \n", numProc + 32, numProc+1);

            if (i == nbZone - 1) {
                printf("\033[%im [%i] 🎉 J'ai finis de mettre a jour toutes les zones, traitement terminé 🎉\033[0m \n", numProc + 32, numProc+1);
            }
        }    

    }

    exit(0);
}

int main(int argc, char * argv[]){
    if (argc!=6) {
        printf("Nbre d'args invalide, utilisation :\n");
        printf("%s nombre-traitements nombre-zones fichier-pour-cle-ipc entier-pour-clé-ipc numero-processus\n", argv[0]);
        exit(0);
    }
        
    int cle = ftok(argv[3], atoi(argv[4]));

    int nbSem = atoi(argv[1]) -1;
    int nbZone = atoi(argv[2]);
    int numProc = atoi(argv[5]);

    int idSem = semget(cle, nbSem, 0);

    if(idSem == -1) {
        perror("erreur semget : ");
        exit(-1);
    }

    printf("Sem id : %d \n", idSem);



    /* test affichage des valeurs des sémaphores du tableau */
    Semaphores semaphores;
    semaphores.array = (ushort*)malloc(nbSem * sizeof(ushort));

    if (semctl(idSem, nbSem, GETALL, semaphores) == -1){
        perror("erreur initialisation sem : ");
        exit(1);
    } 

    printf("Valeurs actuelle des sempahores [ "); 
    for(int i=0; i < nbSem-1; i++){
        printf("%d, ", semaphores.array[i]);
    }
    printf("%d ] \n", semaphores.array[nbSem-1]);

    free(semaphores.array);


    // création et initialisation du segment de mémoire partagé :

    // on réutilise la même clé puisque la numérotation des IPC dépend du type d'objet.
    int laMem = shmget(cle, 0, 0);
    if (laMem == -1){
        perror("erreur shmget : ");
        exit(-1);
    }

    printf("Récupération du segment de mémoire, id : %d \n", laMem);

    // attachement au segment pour pouvoir y accéder
    int * p_att = (int *)shmat(laMem, NULL, 0);
    if (p_att== (int *)-1){
        perror("erreur shmat : ");
        exit(-1);
    }

    // j'ai un pointeur sur le segment, je lance le traitement
    printf("Début du traitement \n");
    traitementImage(numProc-1, nbZone, idSem, nbSem, p_att);
    printf("Fin du traitement \n");

    // détachement pour signaler au systéme la fin de l'utilisation du segment
    if (shmdt(p_att) == -1){
        perror("erreur shmdt : ");
        exit(-1);
    }

    return 0;
}
