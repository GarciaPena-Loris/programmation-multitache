#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
/*
 création et initialisation d'un tableau de sémaphores pour le
 traitement synchronisé. Le nombre d'éléments correspond au nombre de
 traitements -1 et les valeurs initiales sont é 0 (é la case i, la
 valeur corerspond é la derniére zone traitée par le processus
 P_(i+1))

 création d'un segment de memoire partagé qui sera un tableau d'entier (un élélement correspondra é une zone)
 */

typedef union Semaphores {
  int val;
  struct semid_ds *buf;
  ushort *array;
} Semaphores;

int main(int argc, char * argv[]){
  
  if (argc!=4) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s nombre-zones fichier-pour-cle-ipc entier-pour-clé-ipc\n", argv[0]);
    exit(0);
  }
	  
  int cle = ftok(argv[2], atoi(argv[3]));

  int nbZone = atoi(argv[1]);

  // création et initialisation du segment de mémoire partagé :
  // on réutilise la même clé puisque la numérotation des IPC dépend du type d'objet.
  int laMem = shmget(cle, nbZone * sizeof(int), IPC_CREAT | IPC_EXCL | 0620);
  if (laMem == -1){
    perror("erreur shmget : ");
    exit(-1);
  }

  printf("Creation segment de mémoire ok avec memoire id : %d \n", laMem);
	  
  // attachement au segment pour pouvoir y accéder
  int * p_att = (int *)shmat(laMem, NULL, 0);
  if (p_att== (int *)-1){
    perror("erreur shmat : ");
    exit(-1);
  }

  // j'ai un pointeur sur le segment, j'initialise le tableau 
  for(int i=0; i < nbZone; i++){
    p_att[i] = 0;
  }

  // détachement pour signaler au systéme la fin de l'utilisation du segment
  if (shmdt(p_att) == -1){
    perror("erreur shmdt : ");
    exit(-1);
  }
  
  return 0;
}
