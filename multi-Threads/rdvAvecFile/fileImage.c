#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/shm.h>
#include <unistd.h>

typedef struct accesZone {
    long etiquette;
    int msg;
} accesZone;


void afficheTableauPartage(int nbZone, int numProc, int * tabMem) {
    printf("\033[%im [%i] Tableau partagee :\033[0m [ ", numProc + 31, numProc+1); 
    for(int i = 0; i < nbZone - 1; i++) {
        printf("%d, ", tabMem[i]);
    } 
    printf("%d ] \n", tabMem[nbZone - 1]);
}

void traitementImage(int fileId, int numProc, int nbZone, int * tabMem) {
    if (numProc == 0) { // Je suis le premier processus
        printf("\033[%im [%i] 🍼 Je suis le premiere processus \033[0m \n\n", numProc + 31, numProc+1);
    }
    else {
        printf("\033[%im [%i] 🍼 Je suis le %deme processus \033[0m \n\n", numProc + 31, numProc+1, numProc+1); 
    }

    for (int i = 0; i < nbZone; i++) {        
        printf("\n\033[%im [%i] 🖼️  Début de traitement de la zone %d \033[0m \n", numProc + 31, numProc+1, i+1);
        if (numProc == 0) { // Premier processus
            printf("\033[%im [%i] J'attend personne pour modifier 🥇 \033[0m \n", numProc + 31, numProc+1);
        }
        else { // Vérification que ce soit mon tour
            printf("\033[%im [%i] J'attends que ce soit mon tour 😴 \033[0m \n", numProc + 31, numProc+1);
            // Demande de message
            accesZone verif;
            int res = msgrcv(fileId, &verif, 0, numProc, 0);
            if (res == -1) {
                perror("❌ Erreur lors de la réception de la variable partagée ");
                exit(1);
            }
            printf("\033[%im [%i] 📩 Message récupéré avec succès ! \033[0m \n", numProc + 31, numProc+1);

        }

        // Traitement d'un temps aléatoire
        int wait = (rand() % 2) + 1;
        printf("\033[%im [%i] Je modifie la zone %d, ça va me prendre %d secondes ⏱️ \033[0m \n", numProc + 31, numProc+1, i+1, wait);
        // Modifie la zone partagee
        printf("\033[%im ... \033[0m \n", numProc + 31);

        sleep(wait);
        tabMem[i] += 1;
        // Affiche le tableau
        afficheTableauPartage(nbZone, numProc, tabMem);

        printf("\033[%im [%i] ✅ Traitement terminé, envois du message dans la file \033[0m \n", numProc + 31, numProc+1);

         // Envois du message dans la file messages
        printf("\033[%im [%i] 📤 Envois de la demande d'accés a la ressource \033[0m\n", numProc + 31, numProc+1);
        accesZone envois;
        envois.etiquette = numProc+1;
        envois.msg = 0;
        ssize_t res = msgsnd(fileId, &envois, 0, 0);
        if (res == -1) {
            perror("❌ Erreur lors de la demande d'accès de la variable partagée ");
            exit(1);
        }
        printf("\033[%im [%i] 📬 Demande de la ressource envoyé avec succés !\033[0m \n", numProc + 31, numProc+1);

        if (i == nbZone - 1) {
            printf("\n\033[%im [%i] J'ai finis toutes les zones 🏆\033[0m \n", numProc + 31, numProc+1);
        }
    }

    exit(0);

}

int main(int argc, char **argv) {
    if (argc != 5) {
        printf("Utilisation : %s nombre-zones fichier-pour-cle-ipc entier-pour-cle-ipc numero-processus\n", argv[0]);
        exit(-1);
    }

    int nbZone = atoi(argv[1]);
    int numProc = atoi(argv[4]);

    // Création de la Clé
    key_t key = ftok(argv[2], atoi(argv[3]));
    if (key == -1) {
        perror("❌ Erreur lors de la création de la clé ");
        exit(-1);
    }
    printf("Clé créée avec succés.\n");

    // Recuperer File de message
    int fileId = msgget(key, IPC_CREAT|0666);
    if (fileId == -1) {
        perror("❌ Erreur lors de l'accès à la file de message ");
        exit(-1);
    }
    printf("ID de la file de message : %i\n", fileId);

    // création et initialisation du segment de mémoire partagé :

    // on réutilise la même clé puisque la numérotation des IPC dépend du type d'objet.
    int laMem = shmget(key, 0, 0);
    if (laMem == -1){
        perror("❌ erreur shmget ");
        exit(-1);
    }

    printf("Récupération du segment de mémoire, id : %d \n", laMem);

    // attachement au segment pour pouvoir y accéder
    int * p_att = (int *)shmat(laMem, NULL, 0);
    if (p_att== (int *)-1){
        perror("❌ erreur shmat ");
        exit(-1);
    }

    // j'ai un pointeur sur le segment, je lance le traitement
    printf("Début du traitement \n");
    traitementImage(fileId, numProc-1, nbZone, p_att);
    printf("Fin du traitement \n");

    // détachement pour signaler au systéme la fin de l'utilisation du segment
    if (shmdt(p_att) == -1){
        perror("❌ erreur shmdt ");
        exit(-1);
    }
    
    return 0;
}