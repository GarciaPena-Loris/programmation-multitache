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

int main(int argc, char *argv[]) {

    switch (fork()) {
        case -1 :
            fprintf(stderr, "fork : Erreur de fork\n");
            exit(EXIT_FAILURE);
        case 0 :
            int nbOctetTotalLu = 0;
            exit(EXIT_SUCCESS);
        default :
            printf("\nEn attente du client suivant...\n");
    }
}








