#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>

typedef union Semaphores {
  int val;
  struct semid_ds *buf;
  ushort *array;
} Semaphores;

int main(int argc, char * argv[]){
  
  if (argc!=4) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s nombre-ressource fichier-pour-cle-ipc entier-pour-clé-ipc\n", argv[0]);
    exit(0);
  }
	  
  int cle = ftok(argv[2], atoi(argv[3]));

  int nbSem = atoi(argv[1]);

  int idSem = semget(cle, nbSem, IPC_CREAT | IPC_EXCL | 0600);
  
  if(idSem == -1) {
    perror("erreur semget : ");
    exit(-1);
  }

  printf("sem id : %d \n", idSem);


  
  // initialisation des sémaphores à 0
 
  ushort tabinit[nbSem];
  for (int i = 0; i < nbSem; i++) tabinit[i] = 1;
 

  Semaphores valinit;
  
  valinit.array = tabinit;

  if (semctl(idSem, nbSem, SETALL, valinit) == -1){
    perror("erreur initialisation sem : ");
    exit(1);
  }

  /* test affichage des valeurs des sémaphores du tableau */
  valinit.array = (ushort*)malloc(nbSem * sizeof(ushort));

  if (semctl(idSem, nbSem, GETALL, valinit) == -1){
    perror("erreur initialisation sem : ");
    exit(1);
  } 
   
  printf("Valeurs des sempahores apres initialisation [ "); 
  for(int i=0; i < nbSem-1; i++){
    printf("%d, ", valinit.array[i]);
  }
  printf("%d ] \n", valinit.array[nbSem-1]);

  free(valinit.array);  
  return 0;
}
