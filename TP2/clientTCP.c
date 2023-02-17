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

/* Programme client */
void traitement(int ds, int nbAppel) {
  int nbTotalOctetsEnvoye = 0;
    int nbTotalOctetsSuppose = 0;
    int nbAppelSend = 0;

   /*Etape 3.5  Attente du message */
    printf("Veuillez saisir le message à envoyer au serveur (300 caractère max) : ");

    char messageClient[300];
    scanf("%[^\n]", messageClient);
    printf("Texte rentré : '%s'\n", messageClient);
    printf("Taille message : %ld\n", strlen(messageClient));

    nbTotalOctetsSuppose = (strlen(messageClient) + 1) * nbAppel;
    nbTotalOctetsSuppose += (sizeof(strlen(messageClient) + 1)) * nbAppel;

    printf("************* Début de la boucle *************\n");
    for (int i = 0; i < nbAppel; i++) {
      printf("************* Boucle n°%d *************\n", i+1);

      /* Etape 4 : Envois d'un message */
      printf("-----Envoyer message------\n\n");
      /*printf("Veuillez saisir le message à envoyer au serveur (300 caractère max) : ");

      char messageClient[300];
      scanf("%[^\n]", messageClient);
      printf("Texte rentré : '%s'\n", messageClient);
      printf("Taille message : %ld\n", strlen(messageClient));
      */

      /* Etape 4 1: Envois taille message */
      printf("  Envois de la taille du message... \n");
      long tailleMessage = strlen(messageClient) + 1;
      ssize_t resSendTaille = send(ds, &tailleMessage, sizeof(tailleMessage), 0);
      nbAppelSend++;
      if (resSendTaille == -1) {
          perror("Client : pb avec le send:");
          close(ds);
          exit(1); 
      }
      else if (resSendTaille == 0) {
          perror("Client : Connection avec le serveur perdu :");
          close(ds);
          exit(1); 
      }
      printf("  Message envoyés lors de l'envois de la taille: '%ld'\n", tailleMessage);
      printf("  Nombre d'octets envoyés lors de l'envois de la taille: %zd\n\n", resSendTaille);
      nbTotalOctetsEnvoye += resSendTaille;

      /* Etape 4 2: Envois taille message */
      printf("  Envois du message... \n");
      ssize_t resSend = send(ds, messageClient, strlen(messageClient) + 1, 0);
      nbAppelSend++;
      if (resSend == -1) {
          perror("Client : pb avec le send:");
          close(ds);
          exit(1); 
      } 
      else if (resSend == 0) {
          perror("Client : Connection avec le serveur perdu :");
          close(ds);
          exit(1); 
      }
      printf("  Message envoyés : '%s'\n", messageClient);
      printf("  Nombre d'octets envoyés : %zd\n", resSend);
      nbTotalOctetsEnvoye += resSend;

      printf("-----Fin envoie message------\n\n");

      /* Etape 5 : Reception d'un message de la taille du message lu*/
      printf("\n-----Recevoir message (nombre octet envoyés)-----\n\n");
      printf("  En attente de message...\n");

      int messageRecu;
      ssize_t resRecv = recv(ds, &messageRecu, sizeof(messageRecu),0);
      if (resRecv == -1) {
          perror("  Client : pb avec le recv :");
          close(ds);
          exit(1); 
      }
      else if (resRecv == 0) {
          perror("  Client : Connection avec le serveur perdu :\n");
          close(ds);
          exit(1); 
      }
      printf("  Message recus : '%d'\n", messageRecu);
      printf("  Nombre d'octet recus : '%ld'\n", resRecv);

      printf("\n-----Fin reception message-----\n");

      /* Etape 6 : Comparaison taille message*/
      printf("\n--------\n");
      printf("Comparaison du message recu avec le message envoyé :\n");
      printf("    Taille message envoyé : %lu\n", strlen(messageClient) + 1);
      printf("    Taille message recu : %d\n", messageRecu);
      if (messageRecu != strlen(messageClient) + 1) {
          printf("Analyse : la taille des messages ne correspond pas\n");        
      }
      else {
        printf("Analyse : la taille des messages correspond\n");
      }
      printf("--------\n");


      /*if (strcmp(messageClient,"stop") == 0) {
            // Etape 9 : fermer la socket serveur
            if (close(ds) == -1) {
                perror("Client : pb avec le close :");
                exit(1);
            }
            printf("Client : 'stop' detecte, fermeture de la socket réussi !\n");
            break;
        }
      printf("************* Fin boucle n°%d *************\n", i+1);
    }*/

      printf("************* Fin de la boucle n°%d *************\n", i+1);
    /* Etape 7 : Fermeture de la socket*/
    }
    printf("************* Fin des boucles *************\n");

    /* Etape 8 : Affichage */
    printf("\n---- Résumé ----\n");
    printf("Nombre d'octet Supposément envoyé : %d\n", nbTotalOctetsSuppose);
    printf("Nombre d'octet réellement envoyé : %d\n", nbTotalOctetsEnvoye);
    printf("Nombre d'appel à send supposé: %d\n", nbAppel * 2);
    printf("Nombre d'appel à send : %d\n", nbAppelSend);
    printf("--------\n");
    return;
} 

int main(int argc, char *argv[]) {

  /* Je passe en paramètre le numéro de port qui sera donné à la socket créée plus loin.*/
  if (argc != 5){
    printf("utilisation : %s ip_serveur port_serveur port_client nb_iteration\n", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */   
  int ds = socket(PF_INET, SOCK_STREAM, 0);

  if (ds == -1){
    perror("Client : pb creation socket :");
    exit(1); 
  }
  printf("Client : creation de la socket réussie \n");
  
  
  /* Etape 2 : Connection a notre serveur */
   struct sockaddr_in ss;
   ss.sin_family = AF_INET;
   ss.sin_addr.s_addr = inet_addr(argv[1]);
   ss.sin_port = htons(atoi(argv[2]));

   /*Etape 3  Demandes de connection au serveur*/
    if (connect(ds, (struct sockaddr *) &ss, sizeof(ss)) == -1) {
        perror("Client : probleme avec le connect :");
        close(ds);
        exit(0);
    }
    printf("Client : connection réussi\n\n");

    traitement(ds, atoi(argv[4]));

    /*int j;
    scanf("%d", &j);*/
    close(ds);
    printf("Client : terminé :)\n");
    return 0;
}
