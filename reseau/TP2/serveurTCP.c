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

/* Programme serveur */
void traitement(int dsClient) {
  /* Etape 5 : Traitement avec le client*/
    int i = 0;
    int nbTotalOctetTotalRecu = 0;
    int nbRecv = 0;

    printf("************* Début de la boucle *************\n\n");
    while(1) {
      printf("************* Boucle n°%d *************\n", i+1);
  
      printf("---Recevoir message---\n\n");
      printf("  En attente de la taille du message...\n");

      /* Etape 6 prebis: Reception de la taille du taille*/
      long tailleMessageRecu;
      ssize_t resRecvTaille = recv(dsClient, &tailleMessageRecu, sizeof(tailleMessageRecu),0);
      nbRecv++;
      if (resRecvTaille == -1) {
          perror("  Serveur : pb avec le recv :");
          close(dsClient);
          break;
      }
      else if (resRecvTaille == 0) {
          perror("  Serveur : Connection avec le client perdu :");
          close(dsClient);
          break;
      }
      printf("  Message recus : '%ld'\n", tailleMessageRecu);
      printf("  Nombre d'octet recus : '%ld'\n\n", resRecvTaille);
      nbTotalOctetTotalRecu += resRecvTaille;

      printf("  -------\n");
      printf("  Nombre total de recv depuis le début : %d\n", nbRecv);
      printf("  Nombre d'octet total reçus depuis de ce client : %d\n", nbTotalOctetTotalRecu);
      printf("  -------\n");

      printf("\n  En attente de message (de taille %ld)...\n", tailleMessageRecu);

      /* Etape 6 : Reception d'un message de taille*/
      char messageRecu[tailleMessageRecu];
      ssize_t resRecv = recv(dsClient, &messageRecu, tailleMessageRecu,0);
      nbRecv++;
      if (resRecv == -1) {
          perror("  Serveur : pb avec le recv :\n");
          close(dsClient);
          break;
      }
      else if (resRecv == 0) {
          perror("  Serveur : Connection avec le client perdu :\n");
          close(dsClient);
          break;
      }
      printf("  Message recus : '%s'\n", messageRecu);
      printf("  Nombre d'octet recus : '%ld'\n\n", resRecv);
      nbTotalOctetTotalRecu += resRecv;
      printf("  -------\n");
      printf("  Nombre total de recv depuis le début : %d\n", nbRecv);
      printf("  Nombre d'octet total reçus depuis de ce client : %d\n", nbTotalOctetTotalRecu);
      printf("  -------\n");

      printf("-----Fin reception message-----\n");


      /* Etape 7 : renvois de la taille du message au client*/
      printf("\n-----Envoyer message------\n\n");

      int resSend = send(dsClient, &resRecv, sizeof(int), 0);
      if (resSend == -1) {
          perror("  Serveur : pb avec le send :");
          close(dsClient);
          break;
      }
      else if (resSend == 0) {
          perror("Serveur : Connection avec le client perdu :");
          close(dsClient);
          break; 
      }
      printf("  Message envoyé : '%ld'\n", resRecv);    
      printf("  Nombre d'octets envoyés : %d\n", resSend);

      printf("\n-----Fin envoie message------\n\n");

      printf("************* Fin de la boucle n°%d *************\n", i+1);
      i++;
    }
    /* Etape 8 : fermeture de la socket client*/
    printf("************* Fin des boucles *************\n");
    close(dsClient);
    return;
} 

int main(int argc, char *argv[]) {

  /* Je passe en paramètre le numéro de port qui sera donné à la socket créée plus loin.*/
  if (argc != 2){
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */   
  int ds = socket(PF_INET, SOCK_STREAM, 0);

  if (ds == -1){
    perror("Serveur : pb creation socket :");
    exit(1); 
  }
    printf("Serveur : creation de la socket réussie \n");
  
  
  /* Etape 2 : Nommer la socket du seveur */
   struct sockaddr_in ss;
   ss.sin_family = AF_INET;
   ss.sin_addr.s_addr = INADDR_ANY;
   ss.sin_port = htons(atoi(argv[1]));

   if (bind(ds, (struct sockaddr*)&ss, sizeof(ss)) == -1){
      perror("Serveur : pb du bind :");
      exit(1); 
   }
   printf("Nommage de la socket réussi ! \n");

   /*Etape 2.5 affichage de l'ip */
      struct ifaddrs *ifap, *ifa;
      struct sockaddr_in *sa;
      char *addr;

      getifaddrs(&ifap);
      for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
         if (ifa->ifa_addr && ifa->ifa_addr->sa_family==AF_INET) {
               sa = (struct sockaddr_in *) ifa->ifa_addr;
               addr = inet_ntoa(sa->sin_addr);
               printf("Address IP : %s\n", addr);
         }
      }
      freeifaddrs(ifap);

   /*Etape 3  écoute les demandes de connections*/
    if (listen(ds, 5) == -1) {
        perror("Serveur : Formatage de la boite reseau impossible :");
        close(ds);
        exit(0);
    }

    printf("Serveur OK, en écouté sur le port %s...\n\n", argv[1]);


    while(1) {
        /* Etape 4 : attente de connection d'un client*/
        struct sockaddr_in adr_client;
        socklen_t taille_adresse_client = sizeof(struct sockaddr_in);
        int dsClient = accept(ds,(struct sockaddr *) &adr_client, &taille_adresse_client);
    
        if (dsClient == -1) {
            perror("Serveur : Probleme avec l'acceptation du client : ");
            close(dsClient);
            exit(0);
        }

        printf("-----Debut de traiement avec le client %s, %d-----\n\n", inet_ntoa(adr_client.sin_addr), ntohs(adr_client.sin_port));
        traitement(dsClient);
        printf("Fin de traiement avec le client %s, %d\n", inet_ntoa(adr_client.sin_addr), ntohs(adr_client.sin_port));

        /*
        if (strcmp(messageRecu,"stop") == 0) {
              // Etape 9 : fermer la socket serveur
              if (close(ds) == -1) {
                  perror("Serveur : pb avec le close :");
                  exit(1);
              }
              printf("Serveur : 'stop' detecte, fermeture de la socket réussi !\n");
              break;
        } */
    }
  printf("Serveur : terminé\n");
  return 0;
}
