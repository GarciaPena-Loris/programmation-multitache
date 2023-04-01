#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "calcul.h"

// Données partagées entre les threads
struct varPartagees
{
    int nbZones;
    int *di;              // Tableau des zones actuellement traitées
    pthread_cond_t *cond; // Tableau des conditions deux à deux
    pthread_mutex_t mutex;
};

// Paramètres d'un thread
struct params
{
    int idThread;
    struct varPartagees *vPartage;
};

// Fonction de traitement d'une zone
void traitementZone(int idThread, int idZone, int temps)
{
    printf("[Thread n°\033[1m%d\033[0m] 🖌️  : Traitement de la zone \033[1m%d\033[0m\n", idThread, idZone);
    calcul(temps); // Simulation d'un traitement long
}

// Fonction associée à chaque thread secondaire
void *traitement(void *p)
{
    struct params *args = (struct params *)p;
    struct varPartagees *vPartage = args->vPartage;

    printf("[Thread n°\033[1m%d\033[0m] 🍼 : Je suis né 🥺, je vais traiter %i zones 🖼️.\n", args->idThread, vPartage->nbZones);

    for (int i = 1; i <= vPartage->nbZones; i++)
    {
        pthread_mutex_lock(&vPartage->mutex);

        if (args->idThread > 1)
        { // Le premier traitement doit forcément finir sa zone pour que les suivants la traite.
            if (vPartage->di[args->idThread - 1] <= vPartage->nbZones && vPartage->di[args->idThread - 1] < i + 1)
            {
                printf("[Thread n°\033[1m%d\033[0m] 💤 : Le Threads d'avant travaille encore 😴, je me met en attente de la zone \033[1m%d\033[0m.\n", args->idThread, i);
                pthread_cond_wait(&vPartage->cond[args->idThread - 2], &vPartage->mutex);
            }
        }
        pthread_mutex_unlock(&vPartage->mutex);

        // calcul du temps
        // int temps = args->idThread + rand() % 3;
        // int temps = 1 + rand() % 2;
        int temps = 0;

        printf("[Thread n°\033[1m%d\033[0m] 🎨 : Début de traitement sur la zone %i. Temps estimé: \033[1m %is \033[0m ⏱️\n", args->idThread, i, temps * 3);
        traitementZone(args->idThread, i, temps);
        printf("[Thread n°\033[1m%d\033[0m] 🎨 : Fin de traitement sur la zone %i.\n", args->idThread, i);

        pthread_mutex_lock(&vPartage->mutex);

        // On incrémente la valeur de di
        vPartage->di[args->idThread] = i + 1;

        // On signale que le thread traite la i-ème zone :
        printf("[Thread n°\033[1m%d\033[0m] ⏰ : J'ai finis mon traitement de la zone %d ! Je libere le verrou 🔓 et je réveille le Thread suivant 🔔.\n", i, args->idThread);
        pthread_cond_signal(&vPartage->cond[args->idThread - 1]);
        pthread_mutex_unlock(&vPartage->mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("🤖 Utilisation: %s nombre_threads nombre_zones\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialisations
    int nbThreads = atoi(argv[1]);
    pthread_t threads[nbThreads];
    struct params tabParams[nbThreads];
    struct varPartagees vPartage;
    vPartage.nbZones = atoi(argv[2]);
    vPartage.di = (int *)calloc(nbThreads, sizeof(int));
    vPartage.cond = (pthread_cond_t *)malloc(nbThreads * sizeof(pthread_cond_t));

    if (pthread_mutex_init(&vPartage.mutex, NULL) != 0)
    {
        perror("💀 erreur creation mutex");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbThreads; i++)
    {
        if (pthread_cond_init(&vPartage.cond[i], NULL) != 0)
        {
            perror("💀 erreur creation cond");
            exit(EXIT_FAILURE);
        }
    }

    // Pour la simulation des temps de calcul
    srand(atoi(argv[1]));

    printf("[Thread principal] : \033[1m ✨ Debut de la modification de %d zones par %d Threads ! ✨ \033[0m \n\n", atoi(argv[2]), atoi(argv[1]));

    // Création des threads
    for (int i = 0; i < atoi(argv[1]); i++)
    {
        tabParams[i].idThread = i + 1;
        tabParams[i].vPartage = &vPartage;
        if (pthread_create(&threads[i], NULL, traitement, (void *)&tabParams[i]) != 0)
        {
            perror("💀 Erreur lors de la création du thread ");
            exit(1);
        }
    }

    // Attente de la fin des threads.
    for (int i = 0; i < atoi(argv[1]); i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("\n[Thread principal] : \033[1m ✨ Fin de tous les traitements d'images, mon image est maintenant terminée ! ✨ \033[0m \n");

    for (int i = 0; i < nbThreads; i++)
    {
        pthread_cond_destroy(&vPartage.cond[i]);
    }

    // Terminer proprement
    pthread_mutex_destroy(&vPartage.mutex);
    free(vPartage.di);

    return 0;
}