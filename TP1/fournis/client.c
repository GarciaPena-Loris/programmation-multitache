   #include <stdio.h> 
   #include <unistd.h>
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <netdb.h>
   #include <stdlib.h>
   #include<arpa/inet.h>
   #include<string.h>

   /* Programme client */

   int main(int argc, char *argv[]) {
      /* je passe en paramètre l'adresse de la socket du serveur (IP et
         numéro de port) et un numéro de port à donner à la socket créée plus loin.*/

      /* Je teste le passage de parametres. Le nombre et la nature des
         paramètres sont à adapter en fonction des besoins. Sans ces
         paramètres, l'exécution doit être arrétée, autrement, elle
         aboutira à des erreurs.*/
      if (argc != 4){
         printf("utilisation : %s ip_serveur port_serveur port_client\n", argv[0]);
         exit(1);
      }

   /* Etape 1 : créer une socket */   
      int ds = socket(PF_INET, SOCK_DGRAM, 0);

      /* /!\ : Il est indispensable de tester les valeurs de retour de
         toutes les fonctions et agir en fonction des valeurs
         possibles. Voici un exemple */
      if (ds == -1){
         perror("Client : pb creation socket :");
         exit(1); // je choisis ici d'arrêter le programme car le reste
            // dépendent de la réussite de la création de la socket.
      }
   
   /* J'ajoute des traces pour comprendre l'exécution et savoir
      localiser des éventuelles erreurs */
      printf("Client : creation de la socket réussie \n");
      
      // Je peux tester l'exécution de cette étape avant de passer à la
      // suite. Faire de même pour la suite : n'attendez pas de tout faire
      // avant de tester.
   
   /* Etape 2 : Nommer la socket du client */
      struct sockaddr_in BRC;
      BRC.sin_family = AF_INET;
      BRC.sin_addr.s_addr = INADDR_ANY;
      BRC.sin_port = htons((short)0) ;

      int res = bind(ds, (struct sockaddr*)&BRC, sizeof(BRC));

      if (res == -1){
         perror("Client : pb du bind :");
         exit(1); // je choisis ici d'arrêter le programme car le reste dépendent de la réussite de la création de la socket.
      }
      printf("Nommage de la socket réussi ! \n");

   /* Etape 3 : Désigner la socket du serveur */
      struct sockaddr_in sockServ;
      sockServ.sin_family = AF_INET;
      sockServ.sin_addr.s_addr = inet_addr(argv[1]);
      sockServ.sin_port = htons(atoi(argv[2]));

      printf("Désignation de la socket serveur terminé !\n");
   /* Etape 4 : envoyer un message au serveur  (voir sujet pour plus de détails)*/
      printf("-----Envoyer message------\n");

      printf("Veuillez saisir le message à envoyer au serveur (100 caractère max) : ");

      char messageClient[100];
      scanf("%s", messageClient);

      /*
      printf("Envois d'un tableau de 100.000 octet : \n");
      double messageClient[100000];
      for (int i=0; i<100000; i++) {
         messageClient[i] = i;
      }
      trop long*/

      socklen_t sizeAdr = sizeof(struct sockaddr_in);
      int resSend = sendto(ds, &messageClient, strlen(messageClient)+1, 0, (struct sockaddr *) &sockServ, sizeAdr) ;
      //int resSend = sendto(ds, &messageClient, sizeof(messageClient), 0, (struct sockaddr *) &sockServ, sizeAdr) ;
      if (resSend == -1) {
         perror("Client : pb avec le send to :");
         exit(1); // je choisis ici d'arrêter le programme
      }
      printf("Nombre d'octets envoyés : %d\n", resSend);
      printf("-----Recevoir message------\n");
      // il sera primordial de controler les valeurs de retour de toutes les fonctions utilisées.
   
   /* Etape 5 : recevoir un message du serveur (voir sujet pour plus de détails)*/
      struct sockaddr_in sockExpediteur ;
      socklen_t lgAdr = sizeof(struct sockaddr_in);
      int messageServeur;
      ssize_t resRecv = recvfrom(ds, &messageServeur, sizeof(messageServeur), 0, (struct sockaddr *) &sockExpediteur, &lgAdr);
      if (resRecv == -1) {
         perror("Client : pb avec le recvFrom :");
         exit(1); // je choisis ici d'arrêter le programme
      }
      printf("Message reçus : %d\n", messageServeur);
      //affichage des informations de l'expéditeur :
      printf("Ip de l'expéditeur : %s\n", inet_ntoa(sockExpediteur.sin_addr));
      printf("Port de l'expéditeur : %hu\n", htons(sockExpediteur.sin_port));
      
   /* Etape 6 : fermer la socket (lorsqu'elle n'est plus utilisée)*/
      int cls = close(ds);
      if (cls == -1) {
         perror("Client : pb avec le close :");
         exit(1); // je choisis ici d'arrêter le programme
      }
      printf("Close de la socket réussi !\n");
   
      printf("\nClient : je terminé...\n");
      return 0;
   }
