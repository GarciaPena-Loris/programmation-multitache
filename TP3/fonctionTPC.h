#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#include <ifaddrs.h>

int creerSocket() {
    int ds = socket(PF_INET, SOCK_STREAM, 0);
    if (ds == -1){
        perror("creerSocket : ERROR : probleme de creation socket ");
        exit(1); 
    }
    printf("creerSocket : Création de la socket réussie.\n");
    return ds;
}

void connectionServeur(int socketClient, char* IP, char* port) {
    struct sockaddr_in socketServeur;
    socketServeur.sin_family = AF_INET;
    socketServeur.sin_addr.s_addr = inet_addr(IP);
    socketServeur.sin_port = htons(atoi(port));

    /*Etape 3  Demandes de connection au serveur*/
    if (connect(socketClient, (struct sockaddr *) &socketServeur, sizeof(socketServeur)) == -1) {
        perror("connectionServeur : ERROR : probleme avec le connect ");
        close(socketClient);
        exit(1);
    }
    printf("connectionServeur : Connection réussi.\n"); 
}

void closeSocket(int socket) {
    if (close(socket) == -1) {
        perror("closeSocket : ERROR : probleme avec la fermeture de socket ");
        exit(1); 
    }
}

void nommerSocket(int socketServeur, char* port) {
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
    
    // -- Afficher ip et port du serveur
    socklen_t tailleAdresse = sizeof(adresseServeur);
    if (getsockname(socketServeur, (struct sockaddr*) &adresseServeur, &tailleAdresse) == -1) {
        perror("nommerSocket : ERROR : probleme lors de la récupération de l'ip ");
        exit(1);
    } 
    printf("nommerSocket : Serveur est nommé sur l'ip %s et le port %i.\n", inet_ntoa(adresseServeur.sin_addr), ntohs(adresseServeur.sin_port));
}

void ecouterDemande(int socketServeur, char* port) {
    if (listen(socketServeur, 5) == -1) {
        perror("ecouterDemande : ERROR : Formatage de la boite reseau impossible ");
        close(socketServeur);
        exit(1);
    }

    printf("socketServeur : Serveur en écoute sur le port %s.\n", port);
}

int accepterDemande(int socketServeur, struct sockaddr_in *adresseClient) {
    socklen_t tailleAdresseClient = sizeof(struct sockaddr_in);

    int socketClient = accept(socketServeur,(struct sockaddr *) adresseClient, &tailleAdresseClient);

    if (socketClient == -1) {
        perror("accepterDemande : ERROR : probleme avec l'acceptation du client ");
        close(socketClient);
        return -1;
    }

    return socketClient;
}

void afficherIPMachine() {
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