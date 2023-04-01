#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <stdio.h>

#include "fonctionTPC.h"

/* Programme serveur */
void traitementClassique(int socketClient) {
  // --- Etape 1 : Reception taille message
  printf("-----Recevoir message-----\n");
  printf("  --Recevoir la taille du message--\n");

  int tailleMessage;
  ssize_t resRecvTCPsize = recvTCP(socketClient, &tailleMessage, sizeof(int));

  printf("\tMessage recus : '%d'\n", tailleMessage);
  printf("\tNombre d'octet recus : '%ld'\n\n", resRecvTCPsize);

  // --- Etape 2 : Reception du message
  printf("  --Recevoir le message de taille %d--\n", tailleMessage);

  char messageRecu[30000];
  ssize_t resRecvTCP = recvTCP(socketClient, messageRecu, tailleMessage);

  printf("\tMessage recus : '%s'\n", messageRecu);
  printf("\tNombre d'octet recus : '%ld'\n\n", resRecvTCP);

  printf("-----Fin reception message-----\n");

  // --- Etape 3 : Envois taille message recu
  printf("-----Envoyer message------\n");

  int resSendTCP = sendTCP(socketClient, &resRecvTCP, sizeof(int));
     
  printf("\tMessage envoyé : %ld\n", resRecvTCP);
  printf("\tNombre d'octets envoyés : %d\n", resSendTCP);
   
 
  printf("-----Fin envoie message------\n");

  // --- Etape 4 : Fermeture socketClient
  closeSocket(socketClient);
  return;
}

void traitementClassiqueConcurrent(int socketClient, int numeroClient) {
  // --- Etape 1 : Reception taille message
  printf("-----Recevoir message du client n°%d-----\n", numeroClient);
  printf("  --Recevoir la taille du message--\n");

  int tailleMessage;
  ssize_t resRecvTCPsize = recvTCP(socketClient, &tailleMessage, sizeof(int));

  printf("\tMessage recus du client n°%d : '%d'\n",numeroClient, tailleMessage);
  printf("\tNombre d'octet recus du client n°%d : '%ld'\n\n",numeroClient, resRecvTCPsize);

  // --- Etape 2 : Reception du message
  printf("  --Recevoir le message de taille %d--\n", tailleMessage);

  char messageRecu[30000];
  ssize_t resRecvTCP = recvTCP(socketClient, messageRecu, tailleMessage);

  printf("\tMessage recus du client n°%d : '%s'\n",numeroClient, messageRecu);
  printf("\tNombre d'octet recus du client n°%d : '%ld'\n\n",numeroClient, resRecvTCP);

  printf("-----Fin reception message-----\n");

  // --- Etape 3 : Envois taille message recu
  printf("-----Envoyer message du client n°%d------\n", numeroClient);

  int resSendTCP = sendTCP(socketClient, &resRecvTCP, sizeof(int));
     
  printf("\tMessage envoyé du client n°%d : %ld\n",numeroClient, resRecvTCP);
  printf("\tNombre d'octets envoyés du client n°%d : %d\n",numeroClient, resSendTCP);
   
 
  printf("-----Fin envoie message du client n°%d------\n", numeroClient);

  // --- Etape 4 : Fermeture socketClient
  closeSocket(socketClient);
  return;
}

void traitementBoucle(int socketClient) {
  // --- Etape 0 : Initialisation varaibles
  int nbTotalOctetTotalRecu = 0;
  int nbAppelRecv = 0;
  int i = 1;

  printf("************* Début de la boucle infinie*************\n");

  while(1) {
    printf("************* Boucle n°%d *************\n", i);

    // --- Etape 1 : Reception taille message
    printf("-----Recevoir message-----\n");
    printf("  --Recevoir la taille du message--\n");

    int tailleMessage;
    ssize_t resRecvTCPsize = recvTCP(socketClient, &tailleMessage, sizeof(int));
    if (resRecvTCPsize == 0 || resRecvTCPsize == -1) {
      printf("\tArret de la boucle.\n");
      break;
    }
    printf("\tMessage recus : '%d'\n", tailleMessage);
    printf("\tNombre d'octet recus : %ld\n\n", resRecvTCPsize);

    // --- Etape 2 : Affichage nombre appel et nombre Octet
    nbTotalOctetTotalRecu += resRecvTCPsize;
    nbAppelRecv++;
    printf("\t-------\n");
    printf("\tNombre total de recv depuis le début : %d\n", nbAppelRecv);
    printf("\tNombre d'octet total reçus depuis de ce client : %d\n", nbTotalOctetTotalRecu);
    printf("\t-------\n");

    // --- Etape 3 : Reception du message
    printf("  --Recevoir le message de taille %d--\n", tailleMessage);

    char messageRecu[300000];
    ssize_t resRecvTCP = recvTCP(socketClient, messageRecu, tailleMessage);
    if (resRecvTCP == 0 || resRecvTCP == -1) {
      printf("\tArret de la boucle.\n");
      break;
    }

    //printf("\tMessage recus : '%s'\n", messageRecu);
    printf("\tNombre d'octet recus : '%ld'\n\n", resRecvTCP);

    // --- Etape 4 : Affichage nombre appel et nombre Octet
    nbTotalOctetTotalRecu += resRecvTCP;
    nbAppelRecv++;
    printf("\t-------\n");
    printf("\tNombre total de recv depuis le début : %d\n", nbAppelRecv);
    printf("\tNombre d'octet total reçus depuis de ce client : %d\n", nbTotalOctetTotalRecu);
    printf("\t-------\n");


    printf("-----Fin reception message-----\n");


    // --- Etape 5 : Envois taille message recu
    printf("-----Envoyer message------\n");

    int resSendTCP = sendTCP(socketClient, &resRecvTCP, sizeof(int));
    if (resSendTCP == 0 || resSendTCP == -1) {
      printf("\tArret de la boucle.\n");
      break;
    }

    printf("\tMessage envoyé : '%ld'\n", resRecvTCP);
    printf("\tNombre d'octets envoyés : %d\n", resSendTCP);


    printf("-----Fin envoie message------\n");

    printf("************* Fin boucle n°%d *************\n", i);
    i++;
  }
  printf("************* Fin de la boucles infinie *************\n");

  // --- Etape 6 : Fermeture socketClient
  close(socketClient);
  return;
}

void traitementBoucleConcurrent(int socketClient, int numeroClient) {
  // --- Etape 0 : Initialisation varaibles
  int nbTotalOctetTotalRecu = 0;
  int nbAppelRecv = 0;
  int i = 1;

  printf("************* Début de la boucle infinie du client n°%d*************\n", numeroClient);

  while(1) {
    printf("************* Boucle n°%d du client n°%d*************\n", i, numeroClient);

    // --- Etape 1 : Reception taille message
    printf("-----Recevoir message du client n°%d-----\n", numeroClient);
    printf("  --Recevoir la taille du message--\n");

    int tailleMessage;
    ssize_t resRecvTCPsize = recvTCP(socketClient, &tailleMessage, sizeof(int));
    if (resRecvTCPsize == 0 || resRecvTCPsize == -1) {
      printf("\tArret de la boucle du client n°%d.\n", numeroClient);
      break;
    }
    printf("\tMessage recus du client n°%d : '%d'\n", numeroClient, tailleMessage);
    printf("\tNombre d'octet recus du client n°%d : %ld\n\n", numeroClient, resRecvTCPsize);

    // --- Etape 2 : Affichage nombre appel et nombre Octet
    nbTotalOctetTotalRecu += resRecvTCPsize;
    nbAppelRecv++;
    printf("\t-------\n");
    printf("\tNombre total de recv depuis le début du client n°%d : %d\n", numeroClient, nbAppelRecv);
    printf("\tNombre d'octet total reçus du client n°%d : %d\n", numeroClient, nbTotalOctetTotalRecu);
    printf("\t-------\n");

    // --- Etape 3 : Reception du message
    printf("  --Recevoir le message de taille %d du client n°%d--\n", tailleMessage, numeroClient);

    char messageRecu[30000];
    ssize_t resRecvTCP = recvTCP(socketClient, messageRecu, tailleMessage);
    if (resRecvTCP == 0 || resRecvTCP == -1) {
      printf("\tArret de la boucle du client n°%d.\n", numeroClient);
      break;
    }

    printf("\tMessage recus du client n°%d : '%s'\n",numeroClient, messageRecu);
    printf("\tNombre d'octet recus du client n°%d : '%ld'\n\n",numeroClient, resRecvTCP);

    // --- Etape 4 : Affichage nombre appel et nombre Octet
    nbTotalOctetTotalRecu += resRecvTCP;
    nbAppelRecv++;
    printf("\t-------\n");
    printf("\tNombre total de recv depuis le début du client n°%d : %d\n",numeroClient, nbAppelRecv);
    printf("\tNombre d'octet total reçus du client n°%d : %d\n",numeroClient, nbTotalOctetTotalRecu);
    printf("\t-------\n");


    printf("-----Fin reception message du client n°%d-----\n", numeroClient);


    // --- Etape 5 : Envois taille message recu
    printf("-----Envoyer message du client n°%d------\n", numeroClient);

    int resSendTCP = sendTCP(socketClient, &resRecvTCP, sizeof(int));
    if (resSendTCP == 0 || resSendTCP == -1) {
      printf("\tArret de la boucle du client n°%d.\n", numeroClient);
      break;
    }

    printf("\tMessage envoyé du client n°%d : '%ld'\n",numeroClient, resRecvTCP);
    printf("\tNombre d'octets envoyés du client n°%d : %d\n",numeroClient, resSendTCP);


    printf("-----Fin envoie message du client n°%d------\n", numeroClient);

    printf("************* Fin boucle n°%d du client n°%d*************\n", i, numeroClient);
    i++;
  }
  printf("************* Fin de la boucles infinie du client n°%d*************\n", numeroClient);

  // --- Etape 6 : Fermeture socketClient
  close(socketClient);
  return;
}

void traitementDialogue(int socketclient) {
  printf("Pas encore fait x)\n");
}

int main(int argc, char *argv[]) {
  /* Je passe en paramètre le numéro de port qui sera donné à la socket créée plus loin.*/
  if (argc != 2){
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }
  char* numeroPort = argv[1];

  // -- Etape 1 : Création socket seveur  
  int socketServeur = creerSocket();

  // -- Etape 2 : Nommage la socket seveur
  nommerSocket(socketServeur, numeroPort);

  // -- Etape intermediaire : affichage de l'ip
  afficherIPMachine();

  // -- Etape 3 : Ecoute des demandes de connections
  ecouterDemande(socketServeur, numeroPort);

  // -- Etape 4 : Choix du mode du serveur
  printf("Quel type de serveur souhaitez vous lancer ?\n");
  printf("\t 1 : serveur itératif (traitement a la suite)\n");
  printf("\t 2 : serveur concurent (traitement en parallele)\n");
  printf("\t Quel est votre choix ? :\n");

  char choixMode[10] = "";
  fgets(choixMode, sizeof(choixMode), stdin);

  switch (choixMode[0]) {
    case '1':
      printf("**************Serveur itératif*************\n");
      printf("En attente de connexion d'un client...\n");
      
      // -- Etape 5 : Acceptation en boucle de clients
      while(1) {
        // -- Etape 6 : Acceptation d'un client
        struct sockaddr_in adresseClient;
        int socketClient = accepterDemande(socketServeur, &adresseClient);

        printf("-----Debut de traiement avec le client %s, %d-----\n\n", inet_ntoa(adresseClient.sin_addr), ntohs(adresseClient.sin_port));
        
        // -- Etape 7 : Traitement
        printf("Quel type de traitement souhaitez vous effecuter ?\n");
        printf("\t 1 : traitement classique (1 reception)\n");
        printf("\t 2 : traitement en boucle (repetition d'un meme message)\n");
        printf("\t 3 : traitement en dialogue (chat classique)\n");
        printf("\t Quel est votre choix ? :\n");

        char choixTratement[10] = "";
        fgets(choixTratement, sizeof(choixTratement), stdin);

        switch (choixTratement[0]) {
          case '1':
            printf("*************Traitement classique*************\n");
            traitementClassique(socketClient);
            break;
          case '2':
            printf("*************Traitement en boucle*************\n");
            traitementBoucle(socketClient);
            break;
          case '3':
            printf("*************Traitement dialogue*************\n");
            traitementDialogue(socketClient);
            break;
          default:
            printf("problème dans le choix.\n");
        }

        printf("-----Fin de traiement avec le client %s, %d-----\n\n", inet_ntoa(adresseClient.sin_addr), ntohs(adresseClient.sin_port));
      }
      break;
    case '2':
      printf("*************Serveur concurent*************\n");
      printf("En attente de connexion d'un client...\n");
      // -- Etape 5 : Acceptation en boucle de clients
      int numeroClient = 1;
      while(1) {
        // -- Etape 6 : Acceptation d'un client
        struct sockaddr_in adresseClient;
        int socketClient = accepterDemande(socketServeur, &adresseClient);

        // -- Etape 7 : Creation fils pour le client
        switch (fork()) {
            case -1 :
                fprintf(stderr, "fork : Erreur de fork\n");
                closeSocket(socketClient);
                break;
            case 0 :
                closeSocket(socketServeur);
                
                // -- Etape 8 : Traitement
                printf("-----Debut de traitement avec le client n°%d, d'ip : %s et de port : %d--\n\n",numeroClient, inet_ntoa(adresseClient.sin_addr), ntohs(adresseClient.sin_port));
        
                printf("Quel type de traitement souhaitez vous effecuter ?\n");
                printf("\t 1 : traitement classique (1 reception)\n");
                printf("\t 2 : traitement en boucle (repetition d'un meme message)\n");
                printf("\t Quel est votre choix ? :\n");

                char choixTratement[10] = "";
                fgets(choixTratement, sizeof(choixTratement), stdin);

                switch (choixTratement[0]) {
                  case '1':
                    printf("*************Traitement classique*************\n");
                    traitementClassiqueConcurrent(socketClient, numeroClient);
                    break;
                  case '2':
                    printf("*************Traitement en boucle*************\n");
                    traitementBoucleConcurrent(socketClient, numeroClient);
                    break;
                  default:
                    printf("problème dans le choix.\n");
                }

                printf("--Fin de traiement avec le client n°%d, d'ip : %s et de port : %d--\n\n",numeroClient, inet_ntoa(adresseClient.sin_addr), ntohs(adresseClient.sin_port));
      
                exit(EXIT_SUCCESS);
            default :
                closeSocket(socketClient);
                printf("\nEn attente du client suivant...\n");
        }
        numeroClient++;
      }
      break;
    default:
      printf("problème dans le choix.\n");
  }

  // -- Etape 9 : Fermeture de la socket serveur
  closeSocket(socketServeur);

  printf("Serveur : terminé.\n");

  return 0;
}
