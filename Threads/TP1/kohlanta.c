#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// structure qui regroupe les variables partagées entre les threads.
struct varPartageesTeam {

	int nbTeam;
	int nbPlayer;
	int teamReady;
	int teamWinner;

	pthread_mutex_t *start_mutex;
	pthread_cond_t *start_cond;

	pthread_mutex_t *winner_mutex;
	pthread_cond_t *winner_cond;

};


// structure qui regroupe les paramètres d'un thread
struct paramsTeam {

	int idThreadTeam; // cet entier correspond au numéro de traitement associé à un thread

	struct varPartageesTeam *vPartage;

};


struct varPartageesPlayerTeam {
	int bucket;
	int nbReady;

	pthread_mutex_t *bucket_mutex;
	pthread_cond_t *team_conds;

	pthread_mutex_t *nbReady_mutex;
	pthread_cond_t *nbReady_cond;

	struct paramsTeam * vParamTeam;

};


struct paramsPlayer {
	int idThreadPlayer; // cet entier correspond au numéro de traitement associé à un thread
	
	struct varPartageesPlayerTeam * vPartage;
};



void * traitementPlayer (void * p) {
    struct paramsPlayer * args = (struct paramsPlayer *) p;
	struct varPartageesPlayerTeam * vPartage = args -> vPartage;
	struct paramsTeam * ParamTeam = vPartage -> vParamTeam;
	struct varPartageesTeam * ParamPartageTeam = ParamTeam -> vPartage;

	int idDuThread = args->idThreadPlayer;
	int idTeam = (ParamTeam->idThreadTeam);
	int nbrPlayer = (ParamPartageTeam->nbPlayer);

	//Créer n Threads Joueur de cette team!
	printf("[team %d] : [player %d] 💤 : Je suis pret, j'attends le top départ.\n",idTeam+1, idDuThread+1);

	pthread_mutex_lock(vPartage->nbReady_mutex);

	(vPartage->nbReady)++;

	if( vPartage->nbReady == nbrPlayer ){
		pthread_cond_signal(vPartage->nbReady_cond);
	}

	pthread_mutex_unlock(vPartage->nbReady_mutex);

	//Depart de la course!

	pthread_mutex_lock(vPartage->bucket_mutex);
	
	pthread_cond_wait(&(vPartage->team_conds[idDuThread]), vPartage->bucket_mutex);

	pthread_mutex_lock(ParamPartageTeam->winner_mutex);

	while(vPartage->bucket < 100 && ParamPartageTeam->teamWinner==-1 ){

		pthread_mutex_unlock(ParamPartageTeam->winner_mutex);
		sleep(rand()%2+1);
		int water = rand()%10 + 1;
		vPartage->bucket = vPartage->bucket + water;
		printf("[team %d - player %d] : Je rajoute %ddl 💧, il y a maintenant %ddl dans la team %d 🪣\n",idTeam+1, idDuThread+1, water, vPartage->bucket, idTeam+1);
		pthread_cond_signal(&(vPartage->team_conds[(idDuThread + 1) % nbrPlayer]));
		
		//printf("[ team %d ] : [ player %d ] : J'attendre\n",idTeam+1, idDuThread+1);
		pthread_cond_wait(&(vPartage->team_conds[idDuThread]), vPartage->bucket_mutex);

		pthread_mutex_lock(ParamPartageTeam->winner_mutex);

	}

	pthread_mutex_unlock(vPartage->bucket_mutex);

	if(ParamPartageTeam->teamWinner == -1){
		ParamPartageTeam->teamWinner = idTeam;
	} 

	pthread_mutex_unlock(ParamPartageTeam->winner_mutex);

	pthread_cond_signal(&(vPartage->team_conds[(idDuThread + 1) % nbrPlayer]));


	//printf("[ team %d ] : [ player %d ] : Je Termine!\n",idTeam+1, idDuThread+1);

	pthread_exit(EXIT_SUCCESS);
}


void * traitementTeam (void * p) {

	struct paramsTeam * args = (struct paramsTeam *) p;
	struct  varPartageesTeam * vPartage = args -> vPartage;

	int idDuThread = args->idThreadTeam;
	int nbPlayers = vPartage->nbPlayer;

	//Créer n Threads Joueur de cette team!
	printf("[team %d] 🍼 : Je suis crée, prete à jouer 🏊‍ ! \n", idDuThread+1);

	pthread_t threadsPlayer[nbPlayers];
	struct paramsPlayer tabParamsPlayer[nbPlayers];

	pthread_cond_t team_conds[nbPlayers];
	for(int i = 0; i < nbPlayers; i++){
		pthread_cond_init(&team_conds[i], NULL);
	}

	pthread_mutex_t bucket_mutex;
	pthread_mutex_init(&bucket_mutex, NULL);

	pthread_mutex_t nbReady_mutex;
	pthread_mutex_init(&nbReady_mutex, NULL);

	pthread_cond_t nbReady_cond;
	pthread_cond_init(&nbReady_cond, NULL);

	struct varPartageesPlayerTeam vPartagePlayer;

	vPartagePlayer.team_conds = team_conds;
	vPartagePlayer.bucket_mutex = &bucket_mutex;
	vPartagePlayer.vParamTeam = args;
	vPartagePlayer.nbReady = 0;
	vPartagePlayer.nbReady_mutex = &nbReady_mutex;
	vPartagePlayer.nbReady_cond = &nbReady_cond;

	for (int i = 0; i < nbPlayers; i++){

		tabParamsPlayer[i].idThreadPlayer = i;
		tabParamsPlayer[i].vPartage = &vPartagePlayer;

		if (pthread_create(&threadsPlayer[i], NULL, traitementPlayer, &tabParamsPlayer[i]) != 0){
			perror("erreur creation thread");
			exit(1);
		}

	}

	//On prepare le top départ!
	pthread_mutex_lock(vPartagePlayer.nbReady_mutex);

	if(vPartagePlayer.nbReady != nbPlayers ){
		pthread_cond_wait(vPartagePlayer.nbReady_cond, vPartagePlayer.nbReady_mutex);
	} 

	pthread_mutex_unlock(vPartagePlayer.nbReady_mutex);

	printf("[team %d] 💪 : Tous les joueurs sont prêts !\n", idDuThread+1);

	pthread_mutex_lock(vPartage->start_mutex);

	(vPartage->teamReady)++;

	if(vPartage->teamReady!=vPartage->nbTeam){ 
		if ((vPartage->teamReady) == 1) {
			printf("[team %d] ⌛ : Je suis la %dere team prête. J'attends.\n", idDuThread+1, (vPartage->teamReady));
		}
		else {
			printf("[team %d] ⌛ : Je suis la %deme team prête. J'attends.\n", idDuThread+1, (vPartage->teamReady));
		}
		pthread_cond_wait(vPartage->start_cond, vPartage->start_mutex);
	}else{
		printf("[team %d] ⌛ : Je suis la derniere team prête ! Je donne le départ 📢\n", idDuThread+1);
		pthread_cond_broadcast(vPartage->start_cond);
	}

	pthread_mutex_unlock(vPartage->start_mutex);

	pthread_cond_signal(&(vPartagePlayer.team_conds[0]));

	for (int i = 0; i < nbPlayers; i++){
		pthread_join(threadsPlayer[i], NULL);
	}

    //Quand le sceau est plein la team à finit!
	//printf("thread %d : Je termine !\n", idDuThread+1);
	pthread_exit(EXIT_SUCCESS);
}


int main(int argc, char * argv[]){

	if (argc != 3 ){
		printf("Utilisation 🤖: %s nombre_equipes nombre_joueurs \n", argv[0]);
		return 1;
	}

	int nbThreadsTeam = atoi(argv[1]);

	pthread_t threadsTeam[nbThreadsTeam];
	struct paramsTeam tabParamsTeam[nbThreadsTeam];

	int nbThreadsPlayer = atoi(argv[2]);

	pthread_mutex_t start_mutex;
	pthread_mutex_init(&start_mutex, NULL);

	pthread_cond_t start_cond;
	pthread_cond_init(&start_cond, NULL);

	pthread_mutex_t winner_mutex;
	pthread_mutex_init(&winner_mutex, NULL);

	pthread_cond_t winner_cond;
	pthread_cond_init(&winner_cond, NULL);

	struct varPartageesTeam vPartageTeam;

	vPartageTeam.nbTeam = nbThreadsTeam;
	vPartageTeam.nbPlayer = nbThreadsPlayer;
	vPartageTeam.teamWinner = -1;
	vPartageTeam.teamReady = 0;
	vPartageTeam.start_mutex = &start_mutex;
	vPartageTeam.start_cond = &start_cond;
	vPartageTeam.winner_mutex = &winner_mutex;
	vPartageTeam.winner_cond = &winner_cond;

	// création des threards 
	for (int i = 0; i < nbThreadsTeam; i++){

		tabParamsTeam[i].idThreadTeam = i;
		tabParamsTeam[i].vPartage = &vPartageTeam;

		if (pthread_create(&threadsTeam[i], NULL, traitementTeam, &tabParamsTeam[i]) != 0){
			perror("erreur creation thread");
			exit(1);
		}

	}

	// attente de la fin des threards
	for (int i = 0; i < nbThreadsTeam; i++){
		pthread_join(threadsTeam[i], NULL);
	}
	
	printf("\n[Thread principal] 🏆 : Team Vainqueur : %d \n\n", (vPartageTeam.teamWinner) + 1);

	printf("[Thread principal] : ✨ Fin de tous les threads secondaires ✨.\n");

	return 0;
}