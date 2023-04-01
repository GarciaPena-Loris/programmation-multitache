#include <string.h>
#include <stdio.h>//perror
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "calcul.h" //calcul


struct paramsFonctionThread {
  int idThread;
  int temps;
  // si d'autres parametres, les ajouter ici.
};


void* fonctionThread(void* params){

  struct paramsFonctionThread* args = (struct paramsFonctionThread *) params;

  int id = args->idThread;
  int temps = 1;

  pthread_t moi = pthread_self();
  printf("Debut du thread d'id %d, numero %lu, proc. %d\n",id, moi, getpid());
  printf("Debut du cacul de %ds du thread %d\n", temps, id);
  calcul(temps);
  printf("Fin du thread d'id %d\n",id);
  pthread_exit(NULL);
}


int main(int argc, char * argv[]){

  if (argc < 2 ){
    printf("utilisation: %s  nombre_threads  \n", argv[0]);
    return 1;
  }     

  pthread_t threads[atoi(argv[1])];
  
  // creation des threards 
  for (int i = 0; i < atoi(argv[1]); i++){

    // Le passage de parametre est fortement conseille (eviter les variables globles).
    struct paramsFonctionThread *args = malloc(sizeof(struct paramsFonctionThread));
    args->idThread=i;
    // completer pour initialiser les parametres
    if (pthread_create(&threads[i], NULL, 
      fonctionThread, args) != 0){
      perror("erreur creation thread");
      exit(1);
    }
    
    printf("Sortie de la boucle.");
  }

  for (int i = 0; i < atoi(argv[1]); i++) {
    pthread_join(threads[i], NULL);
  }

  // garder cette saisie et modifier le code en temps venu.
  char c; 
  printf("saisir un caractere \n");
  fgets(&c, 1, stdin);

  //... completer

  return 0;
 
}
 
