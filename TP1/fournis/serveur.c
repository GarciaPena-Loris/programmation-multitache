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

int main(int argc, char *argv[]) {

  /* Je passe en paramètre le numéro de port qui sera donné à la socket créée plus loin.*/

  /* Je teste le passage de parametres. Le nombre et la nature des
     paramètres sont à adapter en fonction des besoins. Sans ces
     paramètres, l'exécution doit être arrétée, autrement, elle
     aboutira à des erreurs.*/
  if (argc != 2){
    printf("utilisation : %s port_serveur\n", argv[0]);
    exit(1);
  }

  /* Etape 1 : créer une socket */   
  int ds = socket(PF_INET, SOCK_DGRAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (ds == -1){
    perror("Serveur : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
	     // dépendent de la réussite de la création de la socket.
  }
  
  /* J'ajoute des traces pour comprendre l'exécution et savoir
     localiser des éventuelles erreurs */
  printf("Serveur : creation de la socket réussie \n");
  
  // Je peux tester l'exécution de cette étape avant de passer à la
  // suite. Faire de même pour la suite : n'attendez pas de tout faire
  // avant de tester.
  
  /* Etape 2 : Nommer la socket du seveur */
   struct sockaddr_in ss;
   ss.sin_family = AF_INET;
   ss.sin_addr.s_addr = INADDR_ANY;
   ss.sin_port = htons(atoi(argv[1]));

   int res = bind(ds, (struct sockaddr*)&ss, sizeof(ss));

   if (res == -1){
      perror("Serveur : pb du bind :");
      exit(1); // je choisis ici d'arrêter le programme car le reste dépendent de la réussite de la création de la socket.
   }
   printf("Nommage de la socket réussi ! \n");

   /*Etape 3 bis affichage de l'ip */
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

  /* Etape 4 : recevoir un message du client (voir sujet pour plus de détails)*/
   printf("-----Recevoir message------\n");
   printf("En attente de message...\n");

   struct sockaddr_in sockExpediteur ;
   socklen_t lgAdr = sizeof(struct sockaddr_in);
   char messageRecu[300];

   ssize_t resRecv = recvfrom(ds, &messageRecu, sizeof(messageRecu), 0, (struct sockaddr *) &sockExpediteur, &lgAdr);
   if (resRecv == -1) {
      perror("Client : pb avec le recvFrom :");
      exit(1); // je choisis ici d'arrêter le programme
   }
   printf("Message reçus : %s\n", messageRecu);

   //affichage des informations de l'expéditeur :
   printf("Ip de l'expéditeur : %s\n", inet_ntoa(sockExpediteur.sin_addr));
   printf("Port de l'expéditeur : %hu\n", htons(sockExpediteur.sin_port));

  /* Etape 5 : envoyer un message au serveur (voir sujet pour plus de détails)*/
   printf("-----Envoyer message------\n");
   int messageRetour = strlen(messageRecu)+1;

   socklen_t sizeAdr = sizeof(struct sockaddr_in);
   int resSend = sendto(ds, &messageRetour, sizeof(messageRetour), 0, (struct sockaddr *) &sockExpediteur, sizeAdr);

   if (resSend == -1) {
      perror("Client : pb avec le send to :");
      exit(1); // je choisis ici d'arrêter le programme
   }
   printf("Nombre d'octets envoyés : %d\n", resSend);
   // il sera primordial de controler les valeurs de retour de toutes les fonctions utilisées.

  /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
  int cls = close(ds);
      if (cls == -1) {
         perror("Client : pb avec le close :");
         exit(1); // je choisis ici d'arrêter le programme
      }
      printf("Close de la socket réussi !\n");
  
  printf("Serveur : je termine\n");
  return 0;
}
