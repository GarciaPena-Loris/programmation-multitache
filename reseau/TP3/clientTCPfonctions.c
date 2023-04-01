#include <stdio.h> //perror
#include <unistd.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>

#include <sys/stat.h>

#include "fonctionTPC.h"
#define MAX_BUFFER_SIZE 146980


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
  printf("Veuillez saisir le message à envoyer au serveur (30000 caractère max) : \n");

  char messageClient[30000];
  fgets(messageClient, sizeof(messageClient), stdin);
  messageClient[strlen(messageClient) - 1] = '\0';

  printf("Message rentré : '%s'\n", messageClient);
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

    printf("\tMessage envoyé : '%s'\n", messageClient);
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

void traitementFTP(int socketClient, char* nomFichier) {
  long unsigned int nbTotalOctetEnvoyes = 0; // variable pour compter le nombre total d'octet effectivement envoyés au serveur du début à la fin des échanges.
  long unsigned int nbAppelSentTCP = 0; // variable pour compter le nombre total d'appel a sendTCP

  // --- Etape 0 : Construction du nom du chemin vers le fichier
  char* cheminFichier = malloc(strlen(nomFichier) + 16); // ./emission/+nom fichier +\0
  cheminFichier[0] = '\0';
  strcat(cheminFichier, "../emission/");
  strcat(cheminFichier, nomFichier);

  printf("Chemin vers le fichier : '%s'\n", cheminFichier);

  // --- Etape 1 : Envois de la taille du nom du fichier
  printf("-----Envoyer fichier-----\n");
  printf("  --Envois de la taille du nom du fichier--\n");
  //ATTENTION : Ce mettre d'accords avec le Serveur : 
  long unsigned int tailleNomFichier = strlen(nomFichier) + 1;

  ssize_t resSendTCPsizeName = sendTCP(socketClient, &tailleNomFichier, sizeof(tailleNomFichier));
  if (resSendTCPsizeName == 0 || resSendTCPsizeName == -1) {
      printf("\tArret de l'envois.\n");
      exit(1);
  }
  nbTotalOctetEnvoyes += resSendTCPsizeName;
  nbAppelSentTCP++;

  printf("\tMessage envoyé : '%lu'\n", tailleNomFichier);
  printf("\tNombre d'octets envoyés : %zd\n", resSendTCPsizeName);

  // --- Etape 2 :  Envois du nom du fichier
  printf("  --Envois du nom du fichier--\n");
  ssize_t resSendTCPname = sendTCP(socketClient, nomFichier, strlen(nomFichier) +1);
  if (resSendTCPname == 0 || resSendTCPname == -1) {
      printf("\tArret de l'envois.\n");
      exit(1);
  }
  nbTotalOctetEnvoyes += resSendTCPname;
  nbAppelSentTCP++;

  printf("\tMessage envoyé : '%s'\n", nomFichier);
  printf("\tNombre d'octets envoyés : %zd\n", resSendTCPname);

  // --- Etape 2 :  Obtention de la taille du fichier
  struct stat attribus;
  if(stat(cheminFichier, &attribus) == -1) {
    perror("stat : ERROR : probleme avec l'obtention de la taille du fichier ");
    free(cheminFichier);
    exit(1);
  }

  //ATTENTION : Ce mettre d'accords avec le Serveur : 
  //int tailleFichier = attribus.st_size;
  long unsigned int tailleFichier = attribus.st_size;
  
  printf("La taille du fichier est : %lu octets\n", tailleFichier);
 
  // --- Etape 3 :  Envois de la taille du fichier
  printf("  --Envois de la taille--\n");
  ssize_t resSendTCPsize = sendTCP(socketClient, &tailleFichier, sizeof(tailleFichier));
  if (resSendTCPsize == 0 || resSendTCPsize == -1) {
      printf("\tArret de l'envois.\n");
      exit(1);
  }
  nbTotalOctetEnvoyes += resSendTCPsize;
  nbAppelSentTCP++;

  printf("\tMessage envoyé : '%lu'\n", tailleFichier);
  printf("\tNombre d'octets envoyés : %zd\n", resSendTCPsize);
  
  // --- Etape 4 : Lecture du contenu d'un fichier
  FILE* fichier = fopen(cheminFichier, "rb");
  if(fichier == NULL){
    perror("fopen : ERROR : erreur ouverture fichier ");
    free(cheminFichier);
    exit(1);   
  }
  free(cheminFichier);

  //int nbOctetTotalLu = 0;
  long unsigned int nbOctetTotalLu = 0;

  char bufferLectureFichier[MAX_BUFFER_SIZE];

  // --- Etape 5 : Boucle de lecture du fichier
  printf("  --Envois du message partie par partie--\n");
  long unsigned int nbPartie = 1;
  while(nbOctetTotalLu < tailleFichier){
    
    // --- Etape 6 : Lecture de la partie du fichier
    size_t read = fread(bufferLectureFichier, sizeof(char), MAX_BUFFER_SIZE, fichier);
    
    if(read == 0){
      if(ferror(fichier) != 0) {
        perror("\tfread : ERROR : erreur lors de la lecture du fichier ");
        fclose(fichier);
        exit(1);
      }
      else {
        printf("\tFin de la lecture du fichier.\n"); // fin du fichier
	      break;
      }
    }
    // --- Etape 7 : Envois d'une partie du message
    ssize_t resSendTCP = sendTCP(socketClient, bufferLectureFichier, read);
    if (resSendTCP == 0 || resSendTCP == -1) {
      printf("\tArret de la boucle d'envois.\n");
      break;
    }
    //printf("\tMessage envoyé : %s\n", bufferLectureFichier);
    printf("\tEnvoi n°%lu, nombre d'octets envoyés : %zd\n", nbPartie, resSendTCP);
    nbTotalOctetEnvoyes += resSendTCP;
    nbAppelSentTCP++;
    nbPartie++;

    nbOctetTotalLu += read;
  }

  // --- Etape 8 : Fermeture du fichier
  fclose(fichier); 
  
  printf("-----Fin envoie du fichier-----\n");

  printf("\n--------\n");
  printf("Nombre d'octets envoyé par le sentTCP : %lu\n", nbTotalOctetEnvoyes);
  printf("Nombre d'octets lu depuis le fichier : %lu\n", nbOctetTotalLu);
  printf("Nombre d'appel à sentTCP : %lu\n", nbAppelSentTCP);
  printf("--------\n");

  return;
}

void traitementFTPinverse(int socketClient, char* nomFichier) {
  long unsigned int nbTotalOctetEnvoyes = 0; // variable pour compter le nombre total d'octet effectivement envoye.
  long unsigned int nbAppelSent = 0; // variable pour compter le nombre total d'appel a sendTCP
  long unsigned int nbTotalOctetRecu = 0; // variable pour compter le nombre total d'octet effectivement recu.
  long unsigned int nbAppelRecv = 0; // variable pour compter le nombre total d'appel a recv

  // --- Etape 1 : Envois de la taille du nom du fichier demander au serveur
  printf("-----Envoyer fichier demandé-----\n");
  printf("  --Envois de la taille du nom du fichier--\n");
  long unsigned int tailleNomFichier = strlen(nomFichier) + 1;

  ssize_t resSendTCPsizeName = sendTCP(socketClient, &tailleNomFichier, sizeof(tailleNomFichier));
  if (resSendTCPsizeName == 0 || resSendTCPsizeName == -1) {
      printf("\tArret de l'envois.\n");
      exit(1);
  }
  nbTotalOctetEnvoyes += resSendTCPsizeName;
  nbAppelSent++;

  printf("\tMessage envoyé : '%lu'\n", tailleNomFichier);
  printf("\tNombre d'octets envoyés : %zd\n", resSendTCPsizeName);

  // --- Etape 2 :  Envois du nom du fichier demander au serveur
  printf("  --Envois du nom du fichier--\n");
  ssize_t resSendTCPname = sendTCP(socketClient, nomFichier, strlen(nomFichier) +1);
  if (resSendTCPname == 0 || resSendTCPname == -1) {
      printf("\tArret de l'envois.\n");
      exit(1);
  }
  nbTotalOctetEnvoyes += resSendTCPname;
  nbAppelSent++;

  printf("\tMessage envoyé : '%s'\n", nomFichier);
  printf("\tNombre d'octets envoyés : %zd\n", resSendTCPname);

  printf("-----Fin envois fichier demandé-----\n\n");

  printf("-----Reception du fichier-----\n");
  printf("  --Reception de la notification de la présence du fichier--\n");

  // -- Etape 3 : Reception de la notification du fichier
  int notification;
  ssize_t resRecvTCPnotification = recvTCP(socketClient, &notification, sizeof(notification));
  if (resRecvTCPnotification == 0 || resRecvTCPnotification == -1) {
      printf("\tArret de la reception.\n");
      exit(1);
  }
  nbTotalOctetRecu += resRecvTCPnotification;
  nbAppelRecv++;
  printf("\tMessage recu : '%d'\n", notification);
  printf("\tNombre d'octets recus : %zd\n", resRecvTCPnotification);

  if (notification != 0) {
      printf("\tnotification : PROBLEM : Le serveur ne possede pas le fichier demandé.\n");
      printf("\tArret du client.\n");
      exit(1);
  }

  // --- Etape 4 : Creation du fichier
  char* cheminFichier = malloc(strlen(nomFichier) + 16);
  cheminFichier[0] = '\0';
  strcat(cheminFichier, "../reception/");
  strcat(cheminFichier, nomFichier);

  printf("Chemin vers le fichier : '%s'\n", cheminFichier);

  FILE* fichier = fopen(cheminFichier, "wb");
  if(fichier == NULL){
    perror("fopen : ERROR : erreur ouverture fichier ");
    exit(1);  
  }

  // --- Etape 5 : Reception de la taille du fichier
  printf("  --Reception de la taille du fichier--\n");

  //ATTENTION : Ce mettre d'accords avec le Serveur : 
  long unsigned int tailleFichier;
  ssize_t resRcvTCPSize = recvTCP(socketClient, &tailleFichier, sizeof(tailleFichier));
  if (resRcvTCPSize == 0 || resRcvTCPSize == -1) {
    printf("\tArret de la reception.\n");
    exit(1);
  }
  nbTotalOctetRecu += resRcvTCPSize;
  nbAppelRecv++;

  printf("\tMessage recu : '%lu'\n", tailleFichier);
  printf("\tNombre d'octets recus : %zd\n", resRcvTCPSize);
  

  // --- Etape 6 : Reception des parties du fichier
  printf("  --Reception du message partie par partie--\n");
  long unsigned int nbOctetTotalEcrit = 0;
  long unsigned int nbPartie = 1;

  while (nbOctetTotalEcrit < tailleFichier) {
    // --- Etape 7 : Recuperation de la partie
    char partie[MAX_BUFFER_SIZE];

    ssize_t resRcvpartie = recv(socketClient, &partie, MAX_BUFFER_SIZE, 0);
    if (resRcvpartie == 0 || resRcvpartie == -1) {
      printf("\tArret de la boucle de reception.\n");
      exit(1);
    }
    nbTotalOctetRecu += resRcvpartie;
    nbAppelRecv++;

    printf("\tReception n°%lu, nombre d'octets recus : %ld\n",nbPartie, resRcvpartie);

    // --- Etape 8 : Ecriture de la partie dans le fichier
    size_t nombreOctetsEcrit = 0;
    while(nombreOctetsEcrit < resRcvpartie) {
      size_t written = fwrite(partie, sizeof(char), resRcvpartie, fichier);
      nombreOctetsEcrit = written;
    }

    nbOctetTotalEcrit += nombreOctetsEcrit;
    nbPartie++;
  }

  printf("-----Fin reception du fichier-----\n");

  printf("Ecriture dans fichier reussie.\n");

  // --- Etape 9 : Affichage des valeurs
  printf("\n--------\n");
  printf("Nombre d'octets recu par le recv par le serveur : %lu\n", nbTotalOctetRecu);
  printf("Nombre d'appel à recv : %lu\n", nbAppelRecv);
  printf("Nombre d'octets lu depuis le fichier : %lu\n", nbOctetTotalEcrit);
  printf("Nombre d'octets envoyé par le sentTCP : %lu\n", nbTotalOctetEnvoyes);
  printf("Nombre d'appel à sentTCP : %lu\n", nbAppelSent);;
  printf("--------\n");

  // --- Etape 10 : Fermeture du fichier
  fclose(fichier); 

  return;
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
  printf("\t 3 : traitement FTP (envoi de fichier)\n");
  printf("\t 4 : traitement FTP (reception de fichier)\n");
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
      printf("Nombre d'itération souhaités (max 10000) :\n");

      char nbIterations[10000];
      fgets(nbIterations, 10, stdin);

      traitementBoucle(socketClient, atoi(nbIterations));
      break;
    case '3':
      printf("*************Traitement FTP (envois)*************\n");
      printf("Nom du fichier à envoyer (dans le dossier emission, max 1000) :\n");

      char nomFichier[1000];
      fgets(nomFichier, 1000, stdin);
      nomFichier[strlen(nomFichier) - 1] = '\0';

      traitementFTP(socketClient, nomFichier);
      break;
    case '4':
      printf("*************Traitement FTP (reception)*************\n");
      printf("Nom du fichier à télécharger dans le dossier reception (max 1000) :\n");

      fgets(nomFichier, 1000, stdin);
      nomFichier[strlen(nomFichier) - 1] = '\0';

      traitementFTPinverse(socketClient, nomFichier);
      break;
    default:
      printf("Problème dans le choix.\n");
  }

  // -- Etape 4 : Fermeture de la socket client
  closeSocket(socketClient);

  printf("Client : terminé.\n");

  return 0;
}
