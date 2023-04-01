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
#include <sys/stat.h>

#define MAX_BUFFER_SIZE 146980


/* Programme serveur */

void traitementClassique(int socketClient) {
  // --- Etape 1 : Reception taille message
  printf("-----Recevoir message-----\n");
  printf("  --Recevoir la taille du message--\n");

  int tailleMessage;
  ssize_t resRecvTCPsize = recvTCP(socketClient, &tailleMessage, sizeof(tailleMessage));

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

void traitementClassiqueNumerote(int socketClient, int numeroClient) {
  // --- Etape 1 : Reception taille message
  printf("-----Recevoir message du client n°%d-----\n", numeroClient);
  printf("  --Recevoir la taille du message--\n");

  int tailleMessage;
  ssize_t resRecvTCPsize = recvTCP(socketClient, &tailleMessage, sizeof(tailleMessage));

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
    ssize_t resRecvTCPsize = recvTCP(socketClient, &tailleMessage, sizeof(tailleMessage));
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

    char messageRecu[30000];
    ssize_t resRecvTCP = recvTCP(socketClient, messageRecu, tailleMessage);
    if (resRecvTCP == 0 || resRecvTCP == -1) {
      printf("\tArret de la boucle.\n");
      break;
    }

    printf("\tMessage recus : '%s'\n", messageRecu);
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

void traitementBoucleNumerote(int socketClient, int numeroClient) {
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
    ssize_t resRecvTCPsize = recvTCP(socketClient, &tailleMessage, sizeof(tailleMessage));
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

void traitementFTP(int socketClient) {
  int nbTotalOctetRecu = 0; // variable pour compter le nombre total d'octet effectivement recu.
  int nbAppelRcv = 0; // variable pour compter le nombre total d'appel a rcvTCP

  printf("-----Recevoir fichier-----\n");

  // --- Etape 1 : Recuperer taille nom fichier
  printf("  --Recois taille nom fichier--\n");
  int tailleNomFichier;

  ssize_t resRecvsizeName = recvTCP(socketClient, &tailleNomFichier, sizeof(tailleNomFichier));
  if (resRecvsizeName == 0 || resRecvsizeName == -1) {
    printf("\tArret de la reception.\n");
    exit(1);
  }
  nbTotalOctetRecu += resRecvsizeName;
  nbAppelRcv++;
  
  printf("\tMessage recus : '%d'\n", tailleNomFichier);
  printf("\tNombre d'octet recus : %ld\n", resRecvsizeName);

  // --- Etape 2 : Recuperer nom fichier
  printf("  --Recois nom fichier--\n");
  char nomFichier[tailleNomFichier];
  
  ssize_t resRecvName = recvTCP(socketClient, nomFichier, tailleNomFichier);
  if (resRecvName == 0 || resRecvName == -1) {
    printf("\tArret de la reception.\n");
    exit(1);
  }
  nbTotalOctetRecu += resRecvName;
  nbAppelRcv++;
  
  printf("\tMessage recus : '%s'\n", nomFichier);
  printf("\tNombre d'octet recus : %ld\n", resRecvName);


  // --- Etape 3 : Creation du fichier
  char* cheminFichier = malloc(tailleNomFichier + 16);
  cheminFichier[0] = '\0';
  strcat(cheminFichier,"../reception/");
  strcat(cheminFichier, nomFichier);

  printf("Chemin vers le fichier : '%s'\n", cheminFichier);

  FILE* fichier = fopen(cheminFichier, "wb");
  if(fichier == NULL){
    perror("fopen : ERROR : erreur ouverture fichier ");
    exit(1);  
  }

  // --- Etape 4 : Recuperation de la taille du fichier
  int tailleFichier;
  ssize_t resRcvTCPSize = recvTCP(socketClient, &tailleFichier, sizeof(tailleFichier));
  if (resRcvTCPSize == 0 || resRcvTCPSize == -1) {
    printf("\tArret de la reception.\n");
    exit(1);
  }
  nbTotalOctetRecu += resRcvTCPSize;
  nbAppelRcv++;

  printf("\tMessage recu : '%d'\n", tailleFichier);
  printf("\tNombre d'octets recus : %zd\n", resRcvTCPSize);
  

  // --- Etape 5 : Recuperation du fichier partie par partie
  printf("  --Reception du message partie par partie--\n");
  int nbOctetTotalEcrit = 0;
  int nbPartie = 1;
  while (nbOctetTotalEcrit < tailleFichier) {
    // --- Etape 6 : Recuperation de la partie
    char partie[MAX_BUFFER_SIZE];

    ssize_t resRcvpartie = recv(socketClient, &partie, MAX_BUFFER_SIZE, 0);
    if (resRcvpartie == 0 || resRcvpartie == -1) {
      printf("\tArret de la boucle de reception.\n");
      exit(1);
    }
    nbTotalOctetRecu += resRcvpartie;
    nbAppelRcv++;

    printf("\tReception n°%d, nombre d'octets recus : %ld\n",nbPartie, resRcvpartie);

    // --- Etape 7 : Ecriture de la partie dans le fichier
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

  printf("\n--------\n");
  printf("Nombre d'octets recu par le recv : %d\n", nbTotalOctetRecu);
  printf("Nombre d'octets ecrit dans le fichier : %d\n", nbOctetTotalEcrit);
  printf("Nombre d'appel à recv : %d\n", nbAppelRcv);
  printf("--------\n");

  // --- Etape 8 : Fermeture du fichier
  fclose(fichier);

}

void traitementFTPNumerote(int socketClient, int numeroClient) {
  long unsigned int nbTotalOctetRecu = 0; // variable pour compter le nombre total d'octet effectivement recu.
  long unsigned int nbAppelRcv = 0; // variable pour compter le nombre total d'appel a rcvTCP

  printf("-----Recevoir fichier du client n°%d-----\n", numeroClient);

  // --- Etape 1 : Recuperer taille nom fichier
  printf("  --Recois taille nom fichier du client n°%d--\n", numeroClient);

  //ATTENTION : Ce mettre d'accords avec le Client : 
  // int tailleNomFichier;
  long unsigned int tailleNomFichier;

  ssize_t resRecvsizeName = recvTCP(socketClient, &tailleNomFichier, sizeof(tailleNomFichier));
  if (resRecvsizeName == 0 || resRecvsizeName == -1) {
    printf("\tArret de la reception du client n°%d.\n", numeroClient);
    exit(1);
  }
  nbTotalOctetRecu += resRecvsizeName;
  nbAppelRcv++;
  
  printf("\tMessage recus client du n°%d : '%lu'\n", numeroClient, tailleNomFichier);
  printf("\tNombre d'octet recus du client n°%d : %ld\n", numeroClient, resRecvsizeName);

  // --- Etape 2 : Recuperer nom fichier
  printf("  --Recois nom fichier du client n°%d--\n", numeroClient);
  char nomFichier[tailleNomFichier];
  
  ssize_t resRecvName = recvTCP(socketClient, nomFichier, tailleNomFichier);
  if (resRecvName == 0 || resRecvName == -1) {
    printf("\tArret de la reception du client n°%d.\n", numeroClient);
    exit(1);
  }
  nbTotalOctetRecu += resRecvName;
  nbAppelRcv++;
  
  printf("\tMessage recus du client n°%d : '%s'\n", numeroClient, nomFichier);
  printf("\tNombre d'octet recus du client n°%d : %ld\n", numeroClient, resRecvName);


  // --- Etape 3 : Creation du fichier et sous fichier pour chaque client (max 9999)
  char* cheminFichier = malloc(tailleNomFichier + 25);
  cheminFichier[0] = '\0';

  struct stat attribus;
  char numClientStr[1000];
  sprintf(numClientStr, "%d", numeroClient);

  strcat(cheminFichier,"../reception/client");
  strcat(cheminFichier, numClientStr);

  printf("Chemin vers le fichier : '%s'\n", cheminFichier);

  if (stat(cheminFichier, &attribus) == -1) {
      mkdir(cheminFichier, 0700);
  }

  strcat(cheminFichier,"/");
  strcat(cheminFichier, nomFichier);

  FILE* fichier = fopen(cheminFichier, "wb");
  if(fichier == NULL){
    perror("fopen : ERROR : erreur ouverture fichier ");
    exit(1);  
  }

  // --- Etape 4 : Recuperation de la taille du fichier
  printf("  --Recois taille du fichier du client n°%d--\n", numeroClient);
  long unsigned int tailleFichier;
  ssize_t resRcvTCPSize = recvTCP(socketClient, &tailleFichier, sizeof(tailleFichier));
  if (resRcvTCPSize == 0 || resRcvTCPSize == -1) {
    printf("\tArret de la reception du client n°%d.\n", numeroClient);
    exit(1);
  }
  nbTotalOctetRecu += resRcvTCPSize;
  nbAppelRcv++;

  printf("\tMessage recu du client n°%d : '%lu'\n", numeroClient, tailleFichier);
  printf("\tNombre d'octets recus du client n°%d : %zd\n", numeroClient, resRcvTCPSize);
  

  // --- Etape 5 : Recuperation du fichier partie par partie
  printf("  --Reception du message partie par partie du client n°%d--\n", numeroClient);
  long unsigned int nbOctetTotalEcrit = 0;
  long unsigned int nbPartie = 1;

  while (nbOctetTotalEcrit < tailleFichier) {
    // --- Etape 6 : Recuperation de la partie
    char partie[MAX_BUFFER_SIZE];

    ssize_t resRcvpartie = recv(socketClient, &partie, MAX_BUFFER_SIZE, 0);
    if (resRcvpartie == 0 || resRcvpartie == -1) {
      printf("\tArret de la boucle de reception du client n°%d.\n", numeroClient);
      exit(1);
    }
    nbTotalOctetRecu += resRcvpartie;
    nbAppelRcv++;

    printf("\tReception n°%lu du client n°%d, nombre d'octets recus : %ld\n",nbPartie, numeroClient, resRcvpartie);

    // --- Etape 7 : Ecriture de la partie dans le fichier
    size_t nombreOctetsEcrit = 0;
    while(nombreOctetsEcrit < resRcvpartie) {
      size_t written = fwrite(partie, sizeof(char), resRcvpartie, fichier);
      nombreOctetsEcrit = written;
    }

    nbOctetTotalEcrit += nombreOctetsEcrit;
    nbPartie++;
  }

  printf("-----Fin reception du fichier du client n°%d-----\n", numeroClient);

  printf("Ecriture dans fichier reussie pour le client n°%d.\n", numeroClient);

  printf("\n--------\n");
  printf("Nombre d'octets recu par le recv du client n°%d : %lu\n", numeroClient, nbTotalOctetRecu);
  printf("Nombre d'octets ecrit dans le fichier du client n°%d : %lu\n", numeroClient, nbOctetTotalEcrit);
  printf("Nombre d'appel à recv du client n°%d : %lu\n", numeroClient, nbAppelRcv);
  printf("--------\n");

  // --- Etape 8 : Fermeture du fichier
  fclose(fichier);

}

void traitementFTPNumeroteInverse(int socketClient, int numeroClient) {
  long unsigned int nbTotalOctetEnvoyes = 0; // variable pour compter le nombre total d'octet effectivement envoye.
  long unsigned int nbAppelSent = 0; // variable pour compter le nombre total d'appel a sendTCP
  long unsigned int nbTotalOctetRecu = 0; // variable pour compter le nombre total d'octet effectivement recu.
  long unsigned int nbAppelRecv = 0; // variable pour compter le nombre total d'appel a recv


  // --- Etape 1 : Recuperer taille nom fichier a envoyer
  printf("-----Recevoir nom demandé par le client n°%d-----\n", numeroClient);
  printf("  --Recois taille nom fichier demandé par le client n°%d--\n", numeroClient);

  //ATTENTION : Ce mettre d'accords avec le Client : 
  // int tailleNomFichier;
  long unsigned int tailleNomFichier;

  ssize_t resRecvsizeName = recvTCP(socketClient, &tailleNomFichier, sizeof(tailleNomFichier));
  if (resRecvsizeName == 0 || resRecvsizeName == -1) {
    printf("\tArret de la reception du client n°%d.\n", numeroClient);
    exit(1);
  }
  nbTotalOctetRecu += resRecvsizeName;
  nbAppelRecv++;
  
  printf("\tMessage recus client du n°%d : '%lu'\n", numeroClient, tailleNomFichier);
  printf("\tNombre d'octet recus du client n°%d : %ld\n", numeroClient, resRecvsizeName);

  // --- Etape 2 : Recuperer nom fichier a envoyer
  printf("  --Recois nom fichier demandé par le client n°%d--\n", numeroClient);
  char nomFichier[tailleNomFichier];
  
  ssize_t resRecvName = recvTCP(socketClient, nomFichier, tailleNomFichier);
  if (resRecvName == 0 || resRecvName == -1) {
    printf("\tArret de la reception du client n°%d.\n", numeroClient);
    exit(1);
  }
  nbTotalOctetRecu += resRecvName;
  nbAppelRecv++;
  
  printf("\tMessage recus du client n°%d : '%s'\n", numeroClient, nomFichier);
  printf("\tNombre d'octet recus du client n°%d : %ld\n", numeroClient, resRecvName);

  printf("-----Fin reception par le client n°%d-----\n\n", numeroClient);

  printf("-----Envois du fichier au client n°%d-----\n", numeroClient);

  // --- Etape 3 : Verification que le fichier est bien présent
  char* cheminFichier = malloc(tailleNomFichier + 16);
  cheminFichier[0] = '\0'; 

  strcat(cheminFichier,"../emission/");
  strcat(cheminFichier, nomFichier);

  printf("Chemin vers le fichier : '%s'\n", cheminFichier);


  // -- Etape 4 : Notification au client de la présence du fichier
  printf("  --Envois de la notification au client n°%d--\n", numeroClient);
  FILE* fichier = fopen(cheminFichier, "rb");
  if(fichier == NULL){
    perror("fopen : ERROR : erreur ouverture fichier ");

    // fichier non trouvé : envois -1
    int notification = -1;
    ssize_t resSendTCPnotification = sendTCP(socketClient, &notification, sizeof(notification));
    if (resSendTCPnotification == 0 || resSendTCPnotification == -1) {
        printf("\tArret de l'envois.\n");
        exit(1);
    }
    nbTotalOctetEnvoyes += resSendTCPnotification;
    nbAppelSent++;

    printf("\tMessage envoyé au client n°%d : '%d'\n", numeroClient, notification);
    printf("\tNombre d'octet envoyé au client n°%d : %ld\n", numeroClient, resSendTCPnotification);
    exit(1);
  }

  // fichier trouvé : envois 0
  int notification = 0;
  ssize_t resSendTCPnotification = sendTCP(socketClient, &notification, sizeof(notification));
  if (resSendTCPnotification == 0 || resSendTCPnotification == -1) {
      printf("\tArret de l'envois.\n");
      exit(1);
  }
  nbTotalOctetEnvoyes += resSendTCPnotification;
  nbAppelSent++;

  printf("\tMessage envoyé au client n°%d : '%d'\n", numeroClient, notification);
  printf("\tNombre d'octet envoyé au client n°%d : %ld\n", numeroClient, resSendTCPnotification);

  // --- Etape 5 :  Obtention de la taille du fichier
  struct stat attribus;
  if(stat(cheminFichier, &attribus) == -1) {
    perror("stat : ERROR : probleme avec l'obtention de la taille du fichier ");
    free(cheminFichier);
    exit(1);
  }

  //ATTENTION : Ce mettre d'accords avec le Client : 
  //int tailleFichier = attribus.st_size;
  long unsigned int tailleFichier = attribus.st_size;
  
  printf("\tLa taille du fichier est : %lu octets\n", tailleFichier);

  // --- Etape 6 :  Envois de la taille du fichier
  printf("  --Envois de la taille au client n°%d--\n", numeroClient);
  ssize_t resSendTCPsize = sendTCP(socketClient, &tailleFichier, sizeof(tailleFichier));
  if (resSendTCPsize == 0 || resSendTCPsize == -1) {
      printf("\tArret de l'envois.\n");
      exit(1);
  }
  nbTotalOctetEnvoyes += resSendTCPsize;
  nbAppelSent++;

  printf("\tMessage envoyé : '%lu'\n", tailleFichier);
  printf("\tNombre d'octets envoyés : %zd\n", resSendTCPsize);

  // --- Etape 7 : Envois du fichier partie par partie
  printf("  --Envois du message partie par partie au client n°%d--\n", numeroClient);
  long unsigned int nbPartie = 1;
  long unsigned int nbOctetTotalLu = 0;
  char bufferLectureFichier[MAX_BUFFER_SIZE];
  size_t nombreOctetsLu = 0;
  long unsigned int nombreOctetsMaxALire = 0;

  while(nbOctetTotalLu < tailleFichier){
    // --- Etape 8 : Lecture de la partie du fichier
    nombreOctetsLu = 0;
    if (MAX_BUFFER_SIZE > (tailleFichier - nbOctetTotalLu))
      nombreOctetsMaxALire = tailleFichier - nbOctetTotalLu;
    else
      nombreOctetsMaxALire = MAX_BUFFER_SIZE;
      
    if(nombreOctetsMaxALire == 0){
      printf("\tFin de la lecture du fichier.\n"); // fin du fichier
      break;
    }

    while(nombreOctetsLu < nombreOctetsMaxALire) {
      size_t read = fread(bufferLectureFichier, sizeof(char), MAX_BUFFER_SIZE, fichier);
      if(ferror(fichier) != 0) {
        perror("\tfread : ERROR : erreur lors de la lecture du fichier ");
        fclose(fichier);
        exit(1);
      }
      nombreOctetsLu = read;
    }

    // --- Etape 7 : Envois d'une partie du message
    ssize_t resSendTCP = sendTCP(socketClient, bufferLectureFichier, nombreOctetsLu);
    if (resSendTCP == 0 || resSendTCP == -1) {
      printf("\tArret de la boucle d'envois.\n");
      break;
    }
    
    printf("\tEnvoi n°%lu du client n°%d, nombre d'octets envoyés : %zd\n", nbPartie, numeroClient, resSendTCP);
    nbTotalOctetEnvoyes += resSendTCP;
    nbAppelSent++;
    nbPartie++;

    nbOctetTotalLu += nombreOctetsLu;
  }

  printf("-----Fin envois fichier au client n°%d-----\n", numeroClient);

  printf("\n--------\n");
  printf("Nombre d'octets recu par le recv du client n°%d : %lu\n", numeroClient, nbTotalOctetRecu);
  printf("Nombre d'appel à recv du client n°%d : %lu\n", numeroClient, nbAppelRecv);
  printf("Nombre d'octets lu depuis le fichier : %lu\n", nbOctetTotalLu);
  printf("Nombre d'octets envoyé par le sentTCP du client n°%d : %lu\n", numeroClient, nbTotalOctetEnvoyes);
  printf("Nombre d'appel à sentTCP du client n°%d : %lu\n", numeroClient, nbAppelSent);
  printf("--------\n");

  // --- Etape 8 : Fermeture du fichier
  fclose(fichier);
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

  int numeroClient = 1;
  switch (choixMode[0]) {
    case '1':
      printf("**************Serveur itératif*************\n");
      printf("En attente de connexion d'un client...\n");
      
      // -- Etape 5 : Acceptation en boucle de clients
      while(1) {
        // -- Etape 6 : Acceptation d'un client
        struct sockaddr_in adresseClient;
        int socketClient = accepterDemande(socketServeur, &adresseClient);
        if (socketClient == -1) {
          printf("Annulation traitement du client n°%d\n", numeroClient);
        }
        else {
          printf("-----Debut de traiement avec le client %s, %d-----\n\n", inet_ntoa(adresseClient.sin_addr), ntohs(adresseClient.sin_port));
          
          // -- Etape 7 : Traitement
          printf("Quel type de traitement souhaitez vous effecuter ?\n");
          printf("\t 1 : traitement classique (1 reception)\n");
          printf("\t 2 : traitement en boucle (repetition d'un meme message)\n");
          printf("\t 3 : traitement FTP (reception de fichier)\n");
          printf("\t 4 : traitement FTP (envoi de fichier)\n");
          printf("\t Quel est votre choix ? :\n");

          char choixTratement[10] = "";
          fgets(choixTratement, sizeof(choixTratement), stdin);

          switch (choixTratement[0]) {
            case '1':
              printf("*************Traitement classique*************\n");
              traitementClassiqueNumerote(socketClient, numeroClient);
              break;
            case '2':
              printf("*************Traitement en boucle*************\n");
              traitementBoucleNumerote(socketClient, numeroClient);
              break;
            case '3':
              printf("*************Traitement FTP (reception)*************\n");
              traitementFTPNumerote(socketClient, numeroClient);
              break;
            case '4':
              printf("*************Traitement FTP (envoi)*************\n");
              traitementFTPNumeroteInverse(socketClient, numeroClient);
              break;
            default:
              printf("Problème dans le choix.\n");
          }

          printf("-----Fin de traiement avec le client %s, %d-----\n\n", inet_ntoa(adresseClient.sin_addr), ntohs(adresseClient.sin_port));
        }
        numeroClient++;
      }
      break;
    case '2':
      printf("*************Serveur concurent*************\n");
      printf("En attente de connexion des clients...\n");

      // -- Etape 5 : Acceptation en boucle de clients
      while(1) {
        // -- Etape 6 : Acceptation d'un client
        struct sockaddr_in adresseClient;
        int socketClient = accepterDemande(socketServeur, &adresseClient);
        if (socketClient == -1) {
          printf("Annulation traitement du client n°%d\n", numeroClient);
        }
        else {
          // -- Etape 7 : Creation fils pour le client
          switch (fork()) {
              case -1 :
                  fprintf(stderr, "fork : Erreur de fork\n");
                  closeSocket(socketClient);
                  exit(EXIT_FAILURE);
              case 0 :
                  closeSocket(socketServeur);
                  
                  // -- Etape 8 : Traitement
                  printf("-----Debut de traitement avec le client n°%d, d'ip : %s et de port : %d--\n\n",numeroClient, inet_ntoa(adresseClient.sin_addr), ntohs(adresseClient.sin_port));
          
                  printf("Quel type de traitement souhaitez vous effecuter ?\n");
                  printf("\t 1 : traitement classique (1 reception)\n");
                  printf("\t 2 : traitement en boucle (repetition d'un meme message)\n");
                  printf("\t 3 : traitement FTP (reception de fichier)\n");
                  printf("\t 4 : traitement FTP (envoi de fichier)\n");
                  printf("\t Quel est votre choix ? :\n");

                  char choixTratement[10] = "";
                  fgets(choixTratement, sizeof(choixTratement), stdin);

                  switch (choixTratement[0]) {
                    case '1':
                      printf("*************Traitement classique*************\n");
                      traitementClassiqueNumerote(socketClient, numeroClient);
                      break;
                    case '2':
                      printf("*************Traitement en boucle*************\n");
                      traitementBoucleNumerote(socketClient, numeroClient);
                      break;
                    case '3':
                      printf("*************Traitement FTP (reception)*************\n");
                      traitementFTPNumerote(socketClient, numeroClient);
                      break;
                    case '4':
                      printf("*************Traitement FTP (envoi)*************\n");
                      traitementFTPNumeroteInverse(socketClient, numeroClient);
                      break;
                    default:
                      printf("Problème dans le choix.\n");
                  }

                  printf("--Fin de traiement avec le client n°%d, d'ip : %s et de port : %d--\n\n",numeroClient, inet_ntoa(adresseClient.sin_addr), ntohs(adresseClient.sin_port));
        
                  exit(EXIT_SUCCESS);
              default :
                  closeSocket(socketClient);
                  printf("\nEn attente du client suivant...\n");
          }
        }
        numeroClient++;
      }
      break;
    default:
      printf("Problème dans le choix.\n");
  }

  // -- Etape 9 : Fermeture de la socket serveur
  closeSocket(socketServeur);

  printf("Serveur : terminé.\n");

  return 0;
}
