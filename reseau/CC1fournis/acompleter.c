#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>

/* Ce programme est à compléter et à déposer sur Moodle, sans changer le nom du fichier. N'ajouter aucun autre fichier sauf ceux qui peuvent être demandés pendant l'épreuve.

   Lire attentivement les instructions en commentaires pour compléter correctement ce programme.
 
   Les principales étapes de ce programme sont :
 
   1) contacter un serveur UDP
   
   2) recevoir une instruction à suivre en UDP et l'afficher. L'expéditeur de cette instruction a une adresse différente de celle du serveur que vous avez contacté en 1). 
   Cela sera expliqué dans l'instruction.
 
   3) mettre en place un serveur TCP et échanger avec un client distant.
 
   4) faire une copie du programme et le modifier pour qu'il soit capable de traiter plusieurs clients de manière itérative.
 
   5) faire une copie du programme et le modifier pour qu'il soit capable de traiter plusieurs clients simultanément (en utilisant la fonction fork()).
 
   Attention : vous devez déposer un code qui compile. Exemple : si vous êtes à l'étape 3 qui n'est pas fonctionnelle, mettre cette dernière étape en 
   commentaire pour que toutes les étapes d'avant soient validées.
  
*/

//fonctions
int recvTCP(int sock, void* msg, int sizeMsg) {
    int res;
    int received = 0;

    //boucle de réception
    while(received < sizeMsg) {
        res = recv(sock, msg + received, sizeMsg-received, 0);
        received += res;
        if (res == -1) {
            perror("\trecvTCP : ERROR : problème lors de la reception du message ");
            return -1;
        }
        else if (res == 0) {
            perror("\trecvTCP : ERROR : socket fermée par la couche transport ");
            return 0;
        }
    }

    return received;
}

int sendTCP(int sock, void* msg, int sizeMsg) {
    int res;
    int sent = 0;

    //boucle d'envoi
    while(sent < sizeMsg) {
        res = send(sock, msg + sent, sizeMsg - sent, 0);
        sent += res;
        if (res == -1) {
            perror("\tsendTCP : ERROR : problème lors de l'envoi du message ");
            return -1;
        }
        else if (res == 0) {
            perror("\tsendTCP : ERROR : socket fermée par la couche transport ");
            return 0;
        }
    }
    return sent;
}

int creerSocket() {
    int ds = socket(PF_INET, SOCK_STREAM, 0);
    if (ds == -1){
        perror("creerSocket : ERROR : probleme de creation socket ");
        exit(1); 
    }
    printf("creerSocket : Création de la socket réussie.\n");
    return ds;
}

struct sockaddr_in nommerSocket(int socketServeur, char* port) {
    if (atoi(port) < 1024) {
        printf("nommerSocket : ERROR : le port est inférrieur à 1024.");
        exit(1);
    } 
    
    struct sockaddr_in adresseServeur;
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_addr.s_addr = INADDR_ANY;

    if (atoi(port) == -1) {
        perror("nommerSocket : ERROR : le port est invalide ");
        exit(1);
    }
    adresseServeur.sin_port = htons(atoi(port));

    if (bind(socketServeur, (struct sockaddr*) &adresseServeur, sizeof(adresseServeur)) == -1) {
        perror("nommerSocket : ERROR : probleme le nommage de la socket ");
        exit(1);
    }

    printf("nommerSocket : Socket serveur nommée avec succès.\n");

    return adresseServeur;
}

void ecouterDemande(int socketServeur, char* port) {
    if (listen(socketServeur, 5) == -1) {
        perror("ecouterDemande : ERROR : Formatage de la boite reseau impossible ");
        close(socketServeur);
        exit(1);
    }

    printf("socketServeur : Serveur en écoute sur le port %s.\n", port);
}

void closeSocket(int socket) {
    if (close(socket) == -1) {
        perror("closeSocket : ERROR : probleme avec la fermeture de socket ");
        exit(1); 
    }
}

int main(int argc, char *argv[]) {

  if (argc != 4){
    printf("Utilisation : %s ip_serveurUDP  port_serveurUDP  port_votre_serveur_TCP\n - ip_serveurUDP  et port_serveurUDP : donnés par l'intervenant(e). Il s'agit de l'adresse d'un serveur UDP qui est en attente d'un message que vous allez envoyer.\n - port_votre_serveur_TCP : un numero de votre choix pour un serveur TCP qui sera a implementer\n", argv[0]);
    exit(0);
  }
  
  /* Etape 1 : envoyer un message au serveur UDP (adresse passée en parametres) et recevoir une réponse 
  
     Début :
 
     - envoyer un message de type chaine de caractères. Cette chaine est à saisir au clavier (dites bonjour au serveur UDP).
  */
   
  char premierMsg[200];
  printf("CC : saisir un message a envoyer (moins de 200 caracteres) \n");
  fgets(premierMsg, sizeof(premierMsg), stdin); 
  premierMsg[strlen(premierMsg)-1]  = '\0';                      
   
  int dSUDP = socket(PF_INET, SOCK_DGRAM, 0);

  /* /!\ : Il est indispensable de tester les valeurs de retour de
     toutes les fonctions et agir en fonction des valeurs
     possibles. Voici un exemple */
  if (dSUDP == -1){
    perror("CC : pb creation socket :");
    exit(1); // je choisis ici d'arrêter le programme car le reste
	     // dépend de la réussite de la création de cette socket.
  }
  
  /* Ajouter des traces pour comprendre l'exécution et savoir
     localiser des éventuelles erreurs */
  printf("CC : creation de la socket réussie \n");
 
  // designation de la socket serveur
  printf("CC : Désignation de la socket serveur : \n");
   struct sockaddr_in socketServeur;
   socketServeur.sin_family = AF_INET;
   socketServeur.sin_addr.s_addr = inet_addr(argv[1]);
   socketServeur.sin_port = htons(atoi(argv[2]));
   socklen_t sizeAdr = sizeof(struct sockaddr_in);

   printf("Désignation de la socket serveur terminé !\n");

   // Envois du message au serveur 
   printf("-----Envoyer message------\n"); 

   int resSend = sendto(dSUDP, &premierMsg, strlen(premierMsg)+1, 0, (struct sockaddr *) &socketServeur, sizeAdr) ;
   if (resSend == -1) {
      perror("Client : pb avec le send to :");
      exit(1); // je choisis ici d'arrêter le programme
   }
   printf("\tMessage envoyés : %s\n", premierMsg);
   printf("\tNombre d'octets envoyés : %d\n", resSend);

   printf("-----Fin envoi message------\n"); 

  
  // - recevoir un message de type chaîne de caractères et l'afficher. Il s'agira de la prochaine étape / instruction à réaliser dans votre programme. 
  // Une instruction se terminera par la mention Fin instruction dans le message.
   printf("-----Recevoir instruction 1------\n"); 
    
   char nouvelleInstruction[2024];

   //designation nouvelle socketExpediteur
   struct sockaddr_in sockExpediteur;
   socklen_t lgAdr = sizeof(struct sockaddr_in);

   ssize_t resRecv = recvfrom(dSUDP, &nouvelleInstruction, sizeof(nouvelleInstruction), 0, (struct sockaddr *) &sockExpediteur, &lgAdr);
   if (resRecv == -1) {
      perror("Client : pb avec le recvFrom :");
      exit(1);
   }
   printf("Message reçus : %s\n", nouvelleInstruction);
   printf("Ip de l'expéditeur : %s\n", inet_ntoa(sockExpediteur.sin_addr));
   printf("Port de l'expéditeur : %hu\n\n", htons(sockExpediteur.sin_port));
  
   printf("-----Fin reception instruction 1------\n"); 

  // Fin étape 1    

  //instruction :
  /*
  Super ! Vous avez reussi a me joindre :-).
   L'etape suivante dans votre programme est de :
 - mettre en place une socket d'ecoute en TCP 
 - envoyer l'adresse de cette socket d'ecoute (sous la forme sockaddr_in) à celui qui vous a envoye cette instruction. 
 Bien comprendre l'extrait : a celui qui vous a envoye cette instruction (c'est à dire : une réponse à lexpéditeur) ;
 - terminer les echanges en UDP ; 
 - attendre l'arrivee d'un client TCP ; 
 - recevoir et afficher la prochaine instruction emise par ce client, sous forme de chaine de caracteres 
 (attention a ce que cela implique pour receptionner correctement un tel message. 
 Indice : un entier (int) est à recevoir avant le texte de l'instruction).
 Fin instruction
  */
  


  /* Etape 2 : répondre à la question reçue 
   
     Début */
   
  // --  1 : Création socket seveur  
  int socketEcoute = creerSocket();

  // --  2 : Nommage la socket seveur
   struct sockaddr_in adresseServeur = nommerSocket(socketEcoute, argv[3]);

  // -- 3 : Ecoute des demandes de connections
  ecouterDemande(socketEcoute, argv[3]);

   socklen_t tailleAdresse = sizeof(adresseServeur);

   //-- 4 : envois de cette aacompleterdresse : 
   printf("-----Envoyer adresse------\n"); 
   int resSend2 = sendto(dSUDP, &adresseServeur, sizeof(tailleAdresse), 0, (struct sockaddr *) &sockExpediteur, sizeAdr);
   if (resSend == -1) {
      perror("Client : pb avec le send to :");
      exit(1);
   }
   printf("Nombre d'octets envoyés : %d\n", resSend2);


   //fermeture socket UDP
   printf("-----Fin envois adresse------\n");
   close(dSUDP);

   //attente d'un client :
   printf("-----En attente de connexion d'un client ...\n");

   socklen_t tailleAdresseClient = sizeof(struct sockaddr_in);
   struct sockaddr_in adresseClient;
   int socketClient = accept(socketEcoute,(struct sockaddr *) &adresseClient, &tailleAdresseClient);

   if (socketClient == -1) {
      perror("accepterDemande : ERROR : probleme avec l'acceptation du client ");
      close(socketClient);
      return -1;
   }

   printf("-----Client connecté.\n");

   //Ecoute de la taille du message :
   printf("-----Recevoir nouvelle instruction------\n"); 
   int tailleMessageRecu;
   ssize_t resRecvTaille = recv(socketClient, &tailleMessageRecu, sizeof(tailleMessageRecu),0);
   if (resRecvTaille == -1) {
         perror("  Serveur : pb avec le recv :");
         close(socketClient);
         exit(1);
   }
   else if (resRecvTaille == 0) {
         perror("  Serveur : Connection avec le client perdu :");
         close(socketClient);
         exit(1);
   }
   printf("  Message recus : '%d'\n", tailleMessageRecu);
   printf("  Nombre d'octet recus : '%ld'\n", resRecvTaille);
   
   //Ecoute du message :
   char messageRecu[tailleMessageRecu];
   ssize_t resRecvMsg = recv(socketClient, &messageRecu, tailleMessageRecu,0);
   if (resRecvMsg == -1) {
         perror("  Serveur : pb avec le recv :\n");
         close(socketClient);
         exit(1);
   }
   else if (resRecvMsg == 0) {
         perror("  Serveur : Connection avec le client perdu :\n");
         close(socketClient);
         exit(1);
   }
   printf("  Message recus : '%s'\n", messageRecu);
   printf("  Nombre d'octet recus : '%ld'\n\n", resRecvMsg);

   printf("-----Fin reception nouvelle instruction------\n\n"); 

  // Fin étape 2
  
  //Instruction 2 :
  /*
  Bravo, vous avez mis en place un serveur TCP pret a recevoir des demandes de connexion. La première mission de votre serveur et la suivante :
 - recevoir un tableau de 320000 entiers (int) envoye par le client expediteur de cette instruction. Ne pas afficher ce tableau ! ;
 - renvoyer le tableau recu vers ce client ;
 - recevoir en reponse une chaine de caracteres dont la taille est connue par le client et l'afficher (a vous, encore une fois, de savoir ce qui est necessaire de faire) ;
 - terminer l'echange avec le client
 Fin instruction
  */

  // Etape 3 :

  // Recevoir tableau 320000
   printf("\nReception d'un tableau de 320000 octet :\n");

   int tableau[320000];
   ssize_t resRecvTCPTableau = recvTCP(socketClient, &tableau, sizeof(tableau));
   if (resRecvTCPTableau == 0 || resRecvTCPTableau == -1) {
   printf("\tArret de la boucle.\n");
   exit(1);
   }


   printf("Renvois d'un tableau de 320000 octet\n");

   // Renvoyer le tableau
   int resSendTCPTableau = sendTCP(socketClient, &tableau, sizeof(tableau));
    if (resSendTCPTableau == 0 || resSendTCPTableau == -1) {
      printf("\tArret de la boucle.\n");
      exit(1);
    }

   printf("Fin renvoi d'un tableau de 320000 octet :\n\n");


   printf("----Recevoir insctruction 3-------");
   printf("Recevoir taille chaine de caractere\n");

   //Recevoir taille chaine
    int tailleMessage;
    ssize_t resRecvTCPsize = recvTCP(socketClient, &tailleMessage, sizeof(tailleMessage));
    if (resRecvTCPsize == 0 || resRecvTCPsize == -1) {
      printf("\tArret de la boucle.\n");
      exit(1);
    }
    printf("\tMessage recus : '%d'\n", tailleMessage);
    printf("\tNombre d'octet recus : %ld\n\n", resRecvTCPsize);

   printf("\nRecevoir chaine de caractere\n");

    //Recevoir chaine
   char messageRecu2[tailleMessage];
    ssize_t resRecvTCP = recvTCP(socketClient, messageRecu2, tailleMessage);
    if (resRecvTCP == 0 || resRecvTCP == -1) {
      printf("\tArret de la boucle.\n");
      exit(1);
    }

    printf("\tMessage recus : '%s'\n", messageRecu2);
    printf("\tNombre d'octet recus : '%ld'\n\n", resRecvTCP);

    //Terminer échange
   printf("----Fin reception insctruction 3-------\n");
   closeSocket(socketClient);

    //Instruction 3
    /*
    Bravo, vous avez reussi les echanges avec un client. Avant de vous quitter, voici mon dernier message :
   - verifier la bonne gestion des valeurs de retour des fonctions utilisées ;
   - verifier la bonne gestion des erreurs ; 
   - garder uniquement des traces d'execution significatives ; 
   - verifier la lisibilite de votre code et sa bonne indentation

   Puis, pour aller plus loin :
   - copier le fichier acompleter.c dans un fichier iteratif.c. Dans ce dernier fichier, modifier votre programme pour que le serveur soit iteratif et le tester.
   - ensuite, copier le fichier iteratif.c dans un fichier concurrent.c. Dans ce dernier fichier, modifier votre programme pour que le serveur soit concurrent (en utilisant la fonction fork()) et le tester.
   - le depot de votre travail inclura les trois fichiers acompleter.c, iteratif.c et concurrent.c. Aucun autre fichier n'est a deposer.
   Fin derniere instruction
   */

   printf("Fermeture serveur.\n");
   closeSocket(socketEcoute);
 
}
 
