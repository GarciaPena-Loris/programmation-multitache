#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <stdio.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>

#include "fonctionTPC.h"

/* Programme client */

void traitementClassique(int socketClient) {
  // --- Etape 1 : Ecriture du message
  printf("Veuillez saisir le message à envoyer au serveur (30000 caractère max) : \n");

  char messageClient[30000];
  fgets(messageClient, sizeof(messageClient), stdin);
  messageClient[strlen(messageClient) - 1] = '\0';

  printf("Message rentré : '%s'\n", messageClient);
  printf("Taille Message : %ld\n", strlen(messageClient));

  // --- Etape 2 : Envois de la taille du message
  printf("-----Envoyer messages-----\n");
  printf("  --Envois de la taille--\n");
  int tailleMessage = strlen(messageClient) + 1;
  ssize_t resSendTCPsize = sendTCP(socketClient, &tailleMessage, sizeof(tailleMessage));

  printf("\tMessage envoyé : %d\n", tailleMessage);
  printf("\tNombre d'octets envoyés : %zd\n", resSendTCPsize);

  // --- Etape 3 : Envois du message
  printf("  --Envois du message--\n");
  ssize_t resSendTCP = sendTCP(socketClient, messageClient, strlen(messageClient) + 1);

  printf("\tMessage envoyé : %s\n", messageClient);
  printf("\tNombre d'octets envoyés : %zd\n", resSendTCP);

  printf("-----Fin envoie message-----\n");

  // --- Etape 4 : Reception du message
  printf("-----Recevoir message (nombre octets recu par le serveur)-----\n");
  
  int octetsRecus;
  ssize_t resRecvTCP = recvTCP(socketClient, &octetsRecus, sizeof(octetsRecus));

  printf("\tMessage recus : '%d'\n", octetsRecus);
  printf("\tNombre d'octet recus : '%ld'\n", resRecvTCP);

  printf("-----Fin reception message-----\n");

  // --- Etape 5 : Comparaison des messages
  printf("\n--------\n");
  printf("Comparaison du message recu avec le message envoyé :\n");
  printf("\tNombre octets envoyés : %ld\n", resSendTCP);
  printf("\tTaille du message recu par le serveur : %d\n", octetsRecus);
  if (resSendTCP != octetsRecus) {
    printf("Analyse : WRONG : la taille des messages ne correspond pas.\n");        
  }
  else {
    printf("Analyse : CORRECT : la taille des messages correspond bien.\n");
  }
  printf("--------\n");

  return;
} 

void traitementBoucle(int socketClient, int nbAppel) {
  // --- Etape 0 : Initialisation varaibles
  int nbTotalOctetsEnvoye = 0;
  int nbTotalOctetsSuppose = 0;
  int nbAppelSend = 0;

  // --- Etape 1 : Ecriture du message
  printf("Envoi d'un talbeau de 100000 octets : \n");

    char messageClient[300000];
   for (int i=0; i<300000; i++) {
      messageClient[i] = 'a';
   }

  //printf("Message rentré : '%s'\n", messageClient);
  printf("Taille Message : %ld\n", strlen(messageClient));

  nbTotalOctetsSuppose = (strlen(messageClient) + 1) * nbAppel;
  nbTotalOctetsSuppose += (sizeof(int)) * nbAppel;

  printf("************* Début des %d boucles *************\n", nbAppel);

  for (int i = 0; i < nbAppel; i++) {
    printf("************* Boucle n°%d *************\n", i+1);

    // --- Etape 2 : Envois de la taille du message
    printf("-----Envoyer messages-----\n");
    printf("  --Envois de la taille--\n");

    int tailleMessage = strlen(messageClient) + 1;
    ssize_t resSendTCPsize = sendTCP(socketClient, &tailleMessage, sizeof(tailleMessage));
    if (resSendTCPsize == 0 || resSendTCPsize == -1) {
      printf("\tArret de la boucle.\n");
      break;
    }

    printf("\tMessage envoyé : '%d'\n", tailleMessage);
    printf("\tNombre d'octets envoyés : %zd\n", resSendTCPsize);

    // --- Etape 3 : Incrémentation des variables
    nbTotalOctetsEnvoye += resSendTCPsize;
    nbAppelSend++;

    // --- Etape 4 : Envois du message
    printf("  --Envois du message--\n");
    ssize_t resSendTCP = sendTCP(socketClient, messageClient, strlen(messageClient) + 1);
    if (resSendTCP == 0 || resSendTCP == -1) {
      printf("\tArret de la boucle.\n");
      break;
    }

    //printf("\tMessage envoyé : '%s'\n", messageClient);
    printf("\tNombre d'octets envoyés : %zd\n", resSendTCP);

    printf("-----Fin envoie message------\n\n");

    // --- Etape 5 : Incrémentation des variables
    nbTotalOctetsEnvoye += resSendTCP;
    nbAppelSend++;

    // --- Etape 6 : Reception du message
    printf("-----Recevoir message (nombre octets recu par le serveur)-----\n");
    
    int octetsRecus;
    ssize_t resRecvTCP = recvTCP(socketClient, &octetsRecus, sizeof(octetsRecus));
    if (resRecvTCP == 0 || resRecvTCP == -1) {
      printf("\tArret de la boucle.\n");
      break;
    }
    
    printf("\tMessage recus : '%d'\n", octetsRecus);
    printf("\tNombre d'octet recus : %ld\n", resRecvTCP);

    printf("-----Fin reception message-----\n");

    // --- Etape 7 : Comparaison des messages
    printf("\n--------\n");
    printf("Comparaison du message recu avec le message envoyé :\n");
    printf("\tNombre octets envoyés : %ld\n", resSendTCP);
    printf("\tTaille du message recu par le serveur : %d\n", octetsRecus);
    if (resSendTCP != octetsRecus) {
      printf("Analyse : WRONG : la taille des messages ne correspond pas.\n");        
    }
    else {
      printf("Analyse : CORRECT : la taille des messages correspond bien.\n");
    }
    printf("--------\n");

    printf("\n-----Fin reception message-----\n");

    printf("************* Fin boucle n°%d *************\n", i+1);
  }

  printf("************* Fin des boucles *************\n");

  // --- Etape 8 : Affichage comparaison appels
  printf("\n---- Résumé ----\n");
  printf("Nombre d'octet Supposément envoyé : %d\n", nbTotalOctetsSuppose);
  printf("Nombre d'octet réellement envoyé : %d\n", nbTotalOctetsEnvoye);
  printf("Nombre d'appel à send supposé: %d\n", nbAppel * 2);
  printf("Nombre d'appel à send : %d\n", nbAppelSend);
  printf("--------\n");

  return;
} 

void traitementDialogue(int socketClient) {
}

int main(int argc, char *argv[]) {

  /* Je passe en paramètre l'ip et le numéro de port qui sera donné à la socket créée plus loin.*/
  if (argc != 3){
    printf("utilisation : %s ip_serveur port_serveur \n", argv[0]);
    exit(1);
  }
  char* adresseIPServeur = argv[1];
  char* portServeur = argv[2];

  // -- Etape 1 : Création socket client  
  int socketClient = creerSocket();

  // -- Etape 2 : Connection a la socket serveur
  connectionServeur(socketClient, adresseIPServeur, portServeur);

  // -- Etape 3 : Traitement
  printf("Quel type de traitement souhaitez vous effecuter ?\n");
  printf("\t 1 : traitement classique (1 envois)\n");
  printf("\t 2 : traitement en boucle (envois d'un meme message)\n");
  printf("\t 3 : traitement en dialogue (chat classique)\n");
  printf("\t Quel est votre choix ? :\n");

  char choix[10];
  fgets(choix, sizeof(choix), stdin);

  switch (choix[0]) {
      case '1':
      printf("*************Traitement classique*************\n");
      traitementClassique(socketClient);
      break;
    case '2':
      printf("*************Traitement en boucle*************\n");
      printf("Nombre d'itération souahités (max 10000) :\n");

      char nbIterations[10000];
      fgets(nbIterations, 10, stdin);

      traitementBoucle(socketClient, atoi(nbIterations));
      break;
    case '3':
      printf("*************Traitement dialogue*************\n");
      traitementDialogue(socketClient);
    break;
    default:
      printf("problème dans le choix.\n");
  }

  // -- Etape 4 : Fermeture de la socket client
  closeSocket(socketClient);

  printf("Client : terminé.\n");

  return 0;
}
