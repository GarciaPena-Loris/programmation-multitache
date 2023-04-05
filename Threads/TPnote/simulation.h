#include <unistd.h>

//fonction qui simule la duree d'un vol (environ sec secondes). 
void vol(int sec);

// fonction qui simule une action qui prends quelques temps. Ici : un embarquement ou débarquement d'un voyageur.
void action();

// affiche une trace, avec : a = 'a' trace de l'avion, sinon d'un passager
void afficher (char a, char * trace, int indice, char * destination, char * couleurTrace);


// fonction à ne pas utiliser dans vos threads. 
void initDefault(int s);
