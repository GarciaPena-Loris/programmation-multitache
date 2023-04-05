/*
 Programme d'un vol aller-retour d'un avion avec un thread avion et des threads passagers.
  
 Les parties à compléter sont indiquées en commentaires.
 
 Les traces fournies sont suffisantes.
 
*/

#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"

#define COND_EMBARQUEMENT 0
#define COND_DEBARQUEMENT 1

struct varPartagees {
 // regroupe toute les variables partagées entre threads. Ces variables ne sont pas forcément toutes utilisées par un thread particulier.
 
 // vous pouvez choisir de faire autrement, à partir du moment où il n'y a pas de variables globales ! 

  //... à compléter
  /*
  int nbPlacesRestantesAller;
  int nbPlacesRestantesRetour;
  */
  int embarquementEnCours;
  char * voyage_en_cour;

  pthread_mutex_t verrou;
  pthread_cond_t conds[2];

  pthread_cond_t condEmbarquementAller;
  pthread_cond_t condEmbarquementRetour;
};


struct paramsAvion {
  // structure pour regrouper les paramètres du thread avion. 
  char * dest_aller;
  char * dest_retour;
  int nbPassagers_Aller; // nombre de passagers à l'aller
  int nbPassagers_Retour; // nombre de passagers au retour
  
  // ... à compléter
  struct varPartagees * varPartagees;
};

struct paramsPassager {
  // structure qui regroupe les paramètres d'un thread passager. 
  int idPassager; // un identifiant de passager 1, 2, 3, etc.)
  char * destination; // la destination du passager (aller ou retour) donnée en paramètre du programme.
  char * couleurTrace; // permet de gérer les couleurs des traces.
  
  // ... à compléter. Remarque : en aucun cas un passager n'a besoin de connaitre le nombre total de passagers.
  struct varPartagees * varPartagees;

};


void * avion (void * p) {

  struct paramsAvion * args = (struct paramsAvion *) p;
  struct varPartagees *  varPartagee = args->varPartagees;
  
  char * couleurTrace = "\x1b[38;5;39m";
  
  // implémenter le  vol aller 
  afficher('a', "embarquement immediat !", 0, args -> dest_aller, couleurTrace);
   
   
   /* ... à compléter pour :
    
    1) annoncer le début de l'embarquement aux passagers de l'aller
    2) attendre que tous les passagers de l'aller aient embarqué. Cette partie doit inclure la ligne suivante au bon moment :
    
    afficher('a', "attente fin embarquement", 0, args -> dest_aller, couleurTrace);
    
    */
  int nbPassagerActuel = 0;

  pthread_mutex_lock(&varPartagee->verrou);

  varPartagee->embarquementEnCours = 1;

  pthread_cond_broadcast(&varPartagee->condEmbarquementAller);

  afficher('a', "attente fin embarquement", 0, args -> dest_aller, couleurTrace);

  // Attente que l'avion soit plein.
  while (nbPassagerActuel != args->nbPassagers_Aller) {
    pthread_cond_wait(&varPartagee->conds[COND_EMBARQUEMENT], &varPartagee->verrou);
    nbPassagerActuel++;
    printf("Quelqu'un monte, il y a maintenant %d passager\n", nbPassagerActuel);
  }
  varPartagee->embarquementEnCours = 0;

  pthread_mutex_unlock(&varPartagee->verrou);
  afficher('a',"avion complet, le voyage commence !", 0, args -> dest_aller, couleurTrace); 
      
  

  afficher('a',"décollage imminent! Durée du vol estimée à quelques secondes", 0, args -> dest_aller, couleurTrace); 
 
  vol(4); // simulation d'une durée de vol (vous pouvez tester avec différentes valeurs. Le code rendu doit toutefois remettre vol(4); 
  
  afficher('a',"arrivé à destination", 0, args -> dest_aller, couleurTrace);
      
  /* ... à compléter pour :
  
  1) annoncer l'arrivée à destination et donc le début du débarquement
  2) attendre que tous les passagers soient descendus de l'avion. Cette partie doit inclure la ligne suivante au bon moment :
  
    afficher('a', "attente que tout le monde soit descendu", 0, args -> dest_aller, couleurTrace); 
  
 */
  pthread_mutex_lock(&varPartagee->verrou);
  pthread_cond_broadcast(&varPartagee->conds[COND_DEBARQUEMENT]);

  // Attente que tout le monde descennde.
  while (nbPassagerActuel > 0) {
    afficher('a', "attente que tout le monde soit descendu", 0, args -> dest_aller, couleurTrace); 
    pthread_cond_wait(&varPartagee->conds[COND_DEBARQUEMENT], &varPartagee->verrou);
    nbPassagerActuel--;
  }
  
  afficher('a', "vol aller terminé", 0, args -> dest_aller, couleurTrace);
  
  // impélmenter le vol retour : 
  
   afficher('a', "embarquement immédiat !", 0, args -> dest_retour, couleurTrace);
   
   /* ... à compléter. 
   
   Remarques :
   - ne faire cette partie qu'une fois l'implémentation du vol aller terminée et testée !
   - les threads passager du vol retour ne doivent pas être réveillés inutilement tout au long de l'étape implémentant l'aller.
   
   */
  afficher('a', "vol retour terminé", 0, args -> dest_retour, couleurTrace);
  
  pthread_exit(NULL); 
}


void * passager (void * p) {

  struct paramsPassager * args = (struct paramsPassager *) p;
  struct varPartagees * varPartagees = args->varPartagees;
    
  afficher('p', "Yoopi, je vais prendre l'avion !", args -> idPassager, args -> destination, args ->couleurTrace);
  
  
  /* ... à compléter pour attendre l'annonce de l'embarquement. Cette partie doit inclure la ligne suivante au bon moment :
  
  afficher('p', "je patiente en salle d'embarquement", args -> idPassager, args -> destination, args ->couleurTrace);
  
  */
  pthread_mutex_lock(&varPartagees->verrou);

  if (varPartagees->embarquementEnCours == 0 && args->destination == varPartagees->voyage_en_cour) {
    afficher('p', "je patiente en salle d'embarquement", args -> idPassager, args -> destination, args ->couleurTrace);
    pthread_cond_wait(&varPartagees->condEmbarquementAller, &varPartagees->verrou);
  }
  pthread_cond_broadcast(&varPartagees->conds[COND_EMBARQUEMENT]);
  
  pthread_mutex_unlock(&varPartagees->verrou);

  
  afficher('p', "j'embarque ...", args -> idPassager, args -> destination, args ->couleurTrace);
   
   // ... ici, en fonction de votre solution, il est possible d'ajouter du code
    
  action(); // simulation de l'action embarquer.
  
   // ... ici, en fonction de votre solution, il est possible d'ajouter du code
   
   
  // ici se produit la simulation du vol (géré par l'avion). il n'y a rien à faire
   
  /* ... à compléter pour attendre l'arrivée à destination et donc attendre de pouvoir débarquer. Cette partie doit inclure la ligne suivante au bon moment :
  
    afficher('p', "maintenant, je patiente jusqu'à destination", args -> idThread, args -> destination, args ->couleurTrace);
    
    */

  pthread_mutex_lock(&varPartagees->verrou);
  afficher('p', "maintenant, je patiente jusqu'à destination", args -> idPassager, args -> destination, args ->couleurTrace);
  pthread_cond_wait(&varPartagees->conds[COND_DEBARQUEMENT], &varPartagees->verrou);
  pthread_mutex_unlock(&varPartagees->verrou);
   
  afficher('p', "je descends de l'avion", args -> idPassager, args -> destination, args ->couleurTrace);
  
   // ... ici, en fonction de votre solution, il est possible d'ajouter du code
    
  action(); // simulation de l'action débarquer.
  
   // ... ici, en fonction de votre solution, il est possible d'ajouter du code
   

  afficher('p', "je quitte l'aéroport !", args -> idPassager, args -> destination, args ->couleurTrace); 
  pthread_exit(NULL); 
}


int main(int argc, char * argv[]){
  
  if (argc!=5) {
    printf(" argument requis \n %s destination_aller nombre_passager_aller destination_retour nombre_passager_retour\n", argv[0]);
    exit(1);
  }

  initDefault(atoi(argv[2])); // ne pas modifier cet appel ni le déplacer.
 
 
 
  // initialisations 
  pthread_t threads[1+atoi(argv[2])+atoi(argv[4])]; // pour garder les identifiants de tous les threads dans le même tableau. Vous pouvez faire autrement
  
  // le code fourni dans le reste de ce programme est pour vous aider. Vous pouvez le modifier si vous le souhaitez, à condition que tous les threads passagers (aller et retour) soient créés avant l'avion. 
  
  struct varPartagees varP;
  
  // ... à compléter pour initialiser varP

  varP.embarquementEnCours = 0;
  varP.voyage_en_cour = argv[1];

  
  if (( pthread_mutex_init(&varP.verrou, NULL) != 0)) {
      perror("Erreur lors de l'initialisation du mutex: %s\n");
      exit(EXIT_FAILURE);
  }
  if (( pthread_cond_init(&varP.condEmbarquementAller, NULL) != 0)) {
      perror("Erreur lors de la création de la condition: %s\n");
      exit(EXIT_FAILURE);
  }
  for (size_t i = COND_EMBARQUEMENT; i <= COND_DEBARQUEMENT; ++i) {
      if (pthread_cond_init(&varP.conds[i], NULL) != 0) {
          perror("Erreur lors de la création de la condition");
          exit(EXIT_FAILURE);
      }
  }


  struct paramsAvion paramAvion; 
   
  // initialisations pour l'avion
  paramAvion.dest_aller = argv[1];
  paramAvion.dest_retour = argv[3];
  paramAvion.nbPassagers_Aller = atoi(argv[2]);
  paramAvion.nbPassagers_Retour = atoi(argv[4]);
  
  // ... à compléter
  paramAvion.varPartagees = &varP;
  
  
  // initialisations et création des threads pour les passagers 
  
  struct paramsPassager tabParamsAller[atoi(argv[2])];
   
  struct paramsPassager tabParamsRetour[atoi(argv[4])]; 
 
  // création des threads

  // Passagers aller 
  char * couleurTraceA = "\x1b[38;5;226m";
  
  for (int i = 0; i < atoi(argv[2]); i++){
    tabParamsAller[i].idPassager = i+1;
    tabParamsAller[i].destination = argv[1];
    tabParamsAller[i].couleurTrace = couleurTraceA;
    
    // ... à compléter
    tabParamsAller[i].varPartagees = &varP;
    
    if (pthread_create(&threads[1+i], NULL, passager, &(tabParamsAller[i])) != 0){
      perror("erreur creation thread passager aller");
      exit(1);
    }
  }
  
  
  // Passagers retour
  char * couleurTraceR = "\x1b[38;5;46m";
  for (int i = 0; i < atoi(argv[4]); i++){
  
  /* ... à compléter. 
   l'appel pthread_create aura comme premier paramètre &threads[1+atoi(argv[2])+i]
    */
    tabParamsRetour[i].idPassager = 1+atoi(argv[2])+i;
    tabParamsRetour[i].destination = argv[3];
    tabParamsRetour[i].couleurTrace = couleurTraceR;
    
    // ... à compléter
    tabParamsRetour[i].varPartagees = &varP;
    
    if (pthread_create(&threads[1+atoi(argv[2])+i], NULL, passager, &(tabParamsRetour[i])) != 0){
      perror("erreur creation thread passager aller");
      exit(1);
    }
  }
  
  // Avion
  if (pthread_create(&(threads[0]), NULL, avion, &paramAvion) != 0){
    perror("erreur creation thread avion");
    exit(1);
   } 
    
    // attente de la fin des  threads. 
    for (int i = 0; i < 1+atoi(argv[2])+atoi(argv[4]); i++){
        if (pthread_join(threads[i], NULL) != 0) {
          perror("erreur attente thread");
          exit(1);
        }
    }
    
    // ... à compléter pour une terminaison "propre" (destruction des variables de type mutex, variables conditionnelles, espaces alloués dynamiquement...
    // Destruction du mutex et des conditions
    if (pthread_mutex_destroy(&varP.verrou) != 0) {
        perror("Erreur lors de la destruction du mutex:");
        exit(EXIT_FAILURE);
    }
    for (size_t i = COND_EMBARQUEMENT; i <= COND_DEBARQUEMENT; ++i) {
        if (pthread_cond_destroy(&varP.conds[i]) != 0) {
            perror("Erreur lors de la destruction de la condition");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
 
