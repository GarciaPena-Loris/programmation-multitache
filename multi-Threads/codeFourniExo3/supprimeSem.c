/*
  Description : 
  Le programme dertruit un tableau de s�maphores s'il existe.
  
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(int argc, char * argv[]){

  if (argc!=3) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	  
  int cle=ftok(argv[1], atoi(argv[2]));

  int idSem = semget(cle, 1, 0600);

  // j'utilise semget de sorte a s'assurer que le tableau a d�truire existe.
  if (idSem==-1){
    perror("erreur  semget");
    exit(-1);
  }

  printf("sem suppr id : %d \n", idSem);

  // destruction :
  semctl(idSem, 0, IPC_RMID, NULL);

  int laMem = shmget(cle, 0, 0);
  if (laMem == -1){
    perror("erreur shmget : ");
    exit(-1);
  }

  printf("mem suppr id : %d \n", laMem);

  // destruction :
  shmctl(laMem, IPC_RMID, NULL);

  return 0;
}

