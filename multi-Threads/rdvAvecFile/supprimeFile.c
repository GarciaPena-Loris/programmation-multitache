#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h> 
#include <sys/shm.h>

// pour supprimer la file ayant la clé passée en parametre.

// une autre solution est d'utiliser la commande ipcrm -q <id_file>
// après avoir obtenue l'id de la file via la commande ipcs.

int main(int argc, char * argv[]) {
  if (argc!=3) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s fichier-pour-cle-ipc entier-pour-clé-ipc\n", argv[0]);
    exit(0);
  }
	  
  key_t cle=ftok(argv[1], atoi(argv[2]));

  if (cle==-1) {
    perror("Erreur ftok : ");
    exit(2);
  }

  printf("ftok ok" );
    
  int msgid = msgget(cle, 0);
  if(msgid==-1) {
    perror("erreur msgget : ");
    exit(2);
  }
  
  printf("msgget ok" );

 
  if (msgctl(msgid, IPC_RMID, NULL) == -1)
    perror("erreur suppression file de message :");

  printf("suppression file ok");
  
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
