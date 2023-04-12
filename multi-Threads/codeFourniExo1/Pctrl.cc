#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>

#define MAX_MSG_SIZE 1000

typedef struct demandeAcces {
    long mtype;
    int nproc;
} demandeAcces;

typedef struct sMsg {
    long mtype;
    char message[MAX_MSG_SIZE];
} sMsg;

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Utilisation : %s fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
        exit(-1);
    }
    // Récupération du numéro de processus.
    pid_t pid = getpid();
    printf("Valeur du processus Pctrl : %i\n", pid);
    
    // Création de la Clé
    key_t key = ftok(argv[1], atoi(argv[2]));
    if (key == -1) {
        perror("Erreur lors de la création de la clé ");
        exit(-1);
    }
    printf("Clé créée avec succés.\n");

    // Créer file de message
    int fileId = msgget(key, IPC_CREAT|0666);
    if (fileId == -1) {
        perror("Erreur lors de la création ou l'accès de la file de message ");
        exit(-1);
    }
    printf("File de message créée avec succés, valeur de la file : %d.\n", fileId);


    // Boucle qui attend la demande de la ressource partagée
    while (1) {
        // Attente d'une demande d'accès
        printf("Attente de demande d'accés...\n");
        demandeAcces demandeAcces;
        ssize_t res = msgrcv(fileId, &demandeAcces, sizeof(demandeAcces.nproc), 1, 0);
        if (res == -1) {
            perror("Erreur lors de la demande d'accès à la variable partagée ");
            // Destruction de la file et au revoir.
            if (msgctl(fileId, IPC_RMID, NULL) == -1) {
                perror("Erreur lors de l'exécution du processus ");
            }
            exit(-1);
        }
        printf("Accès donné au processus %i\n", demandeAcces.nproc);


        // Envoie de la donnée au processus
        struct sMsg dataSent;
        dataSent.mtype = demandeAcces.nproc;

        strcpy(dataSent.message, "Ma ressource");

        printf("Envois de la ressource partagée au processus %i\n", demandeAcces.nproc);
        res = msgsnd(fileId, &dataSent, sizeof(dataSent.message), 0);
        if (res == -1) {
            perror("Erreur lors de l'envoie du message dans la file de messages ");
            if (msgctl(fileId, IPC_RMID, NULL) == -1) {
                perror("Erreur lors de l'exécution du processus ");
            }
            exit(-1);;
        }
        printf("Ressource envoyée avec succés.\n");

        // Attente de la réception de la « finition » de la consultation ou modification du message.
        sMsg dataReceived;
        printf("Attente de la validation processus %i ...\n", demandeAcces.nproc);
        res = msgrcv(fileId, &dataReceived, sizeof(dataReceived.message), 2, 0);
        if (res == -1) {
            perror("Erreur lors de la récupération de la variable partagée ");
            // Destruction de la file et au revoir.
            if (msgctl(fileId, IPC_RMID, NULL) == -1) {
                perror("Erreur lors de l'exécution du processus ");
            }
            exit(-1);
        }

        // Copie dans la variable locale de message
        printf("Message recu par le processus %i : '%s'\n", demandeAcces.nproc, dataReceived.message);
        printf("Fin du l'échange avec le processus %i\n", demandeAcces.nproc);
    }

    printf("Fin de l'application.\n");
    return 0;
}