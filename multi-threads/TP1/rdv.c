#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "calcul.h"

struct predicatRdv {

  int compteur;
  pthread_cond_t * condRDV; // condition pour le RDV
  pthread_mutex_t * verrou; // mutex 
};

struct params {
  // structure pour regrouper les parametres d'un thread. 
  int idThread; // un identifiant de thread, de 1 a N (N le nombre total de theads secondaires
  int N;
  struct predicatRdv * varPartagee;

};

// fonction associie a chaque thread participant au RdV.

void * participant (void * p){ 

  struct params * args = (struct params *) p;
  int idThread = args->idThread;
  int nbThread = args->N;
  struct predicatRdv * predicat = args -> varPartagee;


  // simulation d'un long calcul pour le travail avant RdV
  //int temps = idThread + rand() % 10;
  int temps = rand() % 5 + 1;
  //int temps = 0;

  // RdV 
  printf("[Thread d°%d] 🍼 : Je suis né 🥺.\n", idThread);
  //calcul
  printf("[Thread d°%d] 🤖 : Je commence mon PREMIER calcul en cours pendant %d sec ⏱️.\n", idThread ,temps);
  calcul(temps);
  printf("[Thread d°%d] 🤯 : J'ai finis mon PREMIER calcul, je demande un verrou 🔐.\n", idThread);

  // lock
  pthread_mutex_lock(predicat->verrou);
  printf("[Thread d°%d] 🔒 : J'ai vérouillé.\n", idThread);

  // affichage compteur
  predicat->compteur++;
  if (predicat->compteur == 1)
    printf("[Thread d°%d] 🧮 : Je suis arrivé au RDV en %der.\n", idThread, predicat->compteur);
  else
    printf("[Thread d°%d] 🧮 : Je suis arrivé au RDV en %dème.\n", idThread, predicat->compteur);

  if (predicat->compteur == nbThread) {
    // libere tous les threads bloqué
    printf("[Thread d°%d] ⏰ : Je suis arrivé en dernier ! Je libere les verrous 🔓 et je réveille tout le monde 🔔.\n", idThread);
    printf("[Thread d°%d] 📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢-📢\n", idThread);
    sleep(1);
    pthread_cond_broadcast(predicat->condRDV);
  }
  else {
    while (predicat->compteur < nbThread) {
      // bloque l’appelant et rend le verrou de facon atomique
      printf("[Thread d°%d] 💤 : Tout le monde n'a pas finis... Je me met en attente 😴.\n", idThread);
      pthread_cond_wait(predicat->condRDV, predicat->verrou);
      printf("[Thread d°%d] 🥱 : Je me réveille.\n", idThread);
    }
  }

  // tous les threads sont arrivés, on peut débloquer le RdV
  pthread_mutex_unlock(predicat->verrou);
  printf("[Thread d°%d] 🔑 : Je suis libre !\n", idThread);

  // suite de l'execution
  //int temps2 = idThread + rand() % 10;
  int temps2 =  rand() % 5;
  //int temps2 = 0;

  printf("[Thread d°%d] 🤖 : Je commence mon DEUXIEME calcul en cours pendant %d sec ⏱️.\n", idThread ,temps2);
  calcul(temps2); // reprise et poursuite de l'execution.
  printf("[Thread d°%d] 🤯 : J'ai finis mon DEUXIEME calcul.\n", idThread);
  
  pthread_exit(NULL); // fortement recommande.
}


int main(int argc, char * argv[]){
  
  if (argc!=2) {
    printf("utilisation: %s nombre_threads \n", argv[0]);
    exit(1);
  }
 
  // initialisations 
  pthread_t threads[atoi(argv[1])];
  struct params tabParams[atoi(argv[1])];

  srand(atoi(argv[1]));  // initialisation de rand pour la simulation de longs calculs
 
  struct predicatRdv predicat;
  predicat.compteur = 0;

  // Création du verrou et de la variable conditionnelle
  pthread_mutex_t verrou;
  pthread_cond_t cond;
  pthread_mutex_init(&verrou, NULL);
  pthread_cond_init(&cond, NULL);

   // Création des threads 
    for (int i = 0; i < atoi(argv[1]); i++){
        tabParams[i].idThread = i + 1;
        tabParams[i].N = atoi(argv[1]);
        tabParams[i].varPartagee = &predicat;

        predicat.verrou = &verrou;
        predicat.condRDV = &cond;


        if (pthread_create(&threads[i], NULL, 
        &participant, &tabParams[i]) != 0){
            perror("erreur creation thread");
            exit(1);
        }
    }

    // Attente de la fin des threads. Partie obligatoire 
    for (int i = 0; i < atoi(argv[1]); i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Thread principal : ✨ Fin de tous les threads secondaires ✨.\n");
    
    // Terminer proprement
    pthread_mutex_destroy(&verrou);
    pthread_cond_destroy(&cond);
 
}
 
