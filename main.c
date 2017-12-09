#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>
#include <time.h>
#include "main_functions.h"

int nombre_siege_disponible;
int nombre_tattoo_eff;
unsigned int seed;

int main(int argc, char *argv[]) {

	if (argc != 5) {
        fprintf(stderr, "Not enough arguments \n");
        fprintf(stderr, "Usage: <number_tattoos> <number_clients> <number_tattooist> <number_waiting_room_seats> \n");
        exit(EXIT_FAILURE);
    }

	int number_tattoos = atoi(argv[1]);
	int number_clients = atoi(argv[2]);
	int number_tattooist = atoi(argv[3]);
	int number_waiting_room_seats = atoi(argv[4]);

	sem_init(&sem_seats, 0,number_tattooist);
	sem_init(&sem_start_tattoo,0,0);
	sem_init(&sem_end_tattoo,0,0);

	pthread_mutex_init(&promenadance,NULL);
	pthread_mutex_init(&promenadance_end,NULL);
	pthread_mutex_init(&tattooist_signal,NULL);
	pthread_mutex_init(&mut_tattoo_eff,NULL);

	printf(ANSI_COLOR_GREEN "Number of tattoos: %d" ANSI_COLOR_RESET "\n", number_tattoos);
	printf(ANSI_COLOR_GREEN "Number of clients: %d" ANSI_COLOR_RESET "\n", number_clients);
	printf(ANSI_COLOR_GREEN "Number of tattoueurs: %d" ANSI_COLOR_RESET "\n", number_tattooist);
	printf(ANSI_COLOR_GREEN "Number of sieges: %d" ANSI_COLOR_RESET "\n", number_waiting_room_seats);
	
	// First seed
	seed = time(NULL);

	assert(randomWalk(WALK_MIN_T,WALK_MAX_T) <= WALK_MAX_T && randomWalk(WALK_MIN_T,WALK_MAX_T) >= WALK_MIN_T);
	assert(randomTatoo(TATOO_MIN_T, TATOO_MAX_T) <= TATOO_MAX_T && randomTatoo(TATOO_MIN_T, TATOO_MAX_T) >= TATOO_MIN_T);

	pthread_t threads_clients[number_clients];
	pthread_t threads_tattoo[number_tattooist];

	param_t_client params_client[number_clients];
	param_t_tattoo params_tattoueurs[number_tattooist];

	nombre_tattoo_eff = 0;
	nombre_siege_disponible = number_waiting_room_seats;

	for (int i = 0; i < number_clients; i++)
	{
		params_client[i].id_thread_client = i;
		params_client[i].nb_tattouage = 0;
		int code = pthread_create(&threads_clients[i], NULL, client, &params_client[i]);
		assert(code == 0);
	}

	for (int i = 0; i < number_tattooist; i++)
	{
		params_tattoueurs[i].id_thread_tattoueurs = i;
		params_tattoueurs[i].nombre_tatoos = number_tattoos;
		params_tattoueurs[i].nb_tattoo_eff = 0;
		int code = pthread_create(&threads_tattoo[i], NULL, tatoueur, &params_tattoueurs[i]);
		assert(code == 0);
	}

	for (int k = 0; k < number_clients; k++){
	  int codeJoin = pthread_join(threads_clients[k], NULL) != 0;
	  assert(codeJoin == 0);
	}

	for (int l = 0; l < number_tattooist; l++){
	  int codeJoin = pthread_join(threads_tattoo[l], NULL) != 0;
	  assert(codeJoin == 0);
	}

	sem_destroy(&sem_seats);
	sem_destroy(&sem_start_tattoo);
	sem_destroy(&sem_end_tattoo);

	pthread_mutex_destroy(&promenadance);
	pthread_mutex_destroy(&promenadance_end);
	pthread_mutex_destroy(&tattooist_signal);
	pthread_mutex_destroy(&mut_tattoo_eff);

	return EXIT_SUCCESS;
}
