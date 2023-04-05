#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

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
    printf("Valeur du processus Pi : %i\n", pid);

    if (pid < 0) {
        printf("Erreur : le processus ne produit pas une étiquette unique.\n");
        exit(-1);
    }

    // Création de la Clé
    key_t key = ftok(argv[1], atoi(argv[2]));
    if (key == -1) {
        perror("Erreur lors de la création de la clé ");
        exit(-1);
    }
    printf("Clé créée avec succés.\n");

    // File de message
    int fileId = msgget(key, IPC_CREAT|0666);
    if (fileId == -1) {
        perror("Erreur lors de l'accès à la file de message ");
        exit(-1);
    }
    printf("ID de la file de message : %i\n", fileId);

    // Tant que l'utilisateur veut envoyer des messages, il peut les saisir.
    while (1) {
        printf("Entrez un message svp : ");
        char message[MAX_MSG_SIZE];
        fgets(message, MAX_MSG_SIZE, stdin);
        message[strlen(message) - 1] = '\0';

        // Envois demande d'accès à la variable de messages
        printf("Envois de la demande d'accés a la ressource.\n");
        struct demandeAcces demande;
        demande.mtype = 1;
        demande.nproc = pid;
        ssize_t res = msgsnd(fileId, &demande, sizeof(demande.nproc), 0);
        if (res == -1) {
            perror("Erreur lors de la demande d'accès de la variable partagée ");
            exit(EXIT_FAILURE);
        }
        printf("Demande de la ressource envoyé avec succés.\n");

        // Réception de la variable partagée
        sMsg data;
        res = msgrcv(fileId, &data, sizeof(data.message), pid, 0);
        if (res == -1) {
            perror("Erreur lors de la réception de la variable partagée ");
            exit(EXIT_FAILURE);
        }
        printf("Variable partagée recu avec succés : '%s'\n", data.message);

        // Ajoute le message dans la variable partagée
        strcpy(data.message, message);
        data.mtype = 2;

        // Renvois la variable
        printf("Envois du message '%s' via la variable partagée.\n", message);
        res = msgsnd(fileId, (const void *)&data, strlen(data.message), 0);
        if (res == -1) {
            perror("Erreur lors de la modification de la variable partagée ");
            exit(EXIT_FAILURE);
        }
        printf("Variable partagée envoyé avec succés.!\n");


    }
    
    printf("Fin de l'application.\n");
    return 0;
}