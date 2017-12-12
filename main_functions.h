/*
 * @author Group12 - Guetarni Anas - Gay Melvin - Keraim Marwan
 * @file : main_functions.h
 * @Define component uses in main_fonction.c .
 *
 * permit to define global variables like interval or text color, but also structure and fonctions
 * Define mutex and semaphore
 */
#ifndef TATTOOSHOP_MAIN_FUNCTIONS
#define TATTOOSHOP_MAIN_FUNCTIONS

#define DEBUG 0

#ifdef DEBUG
#define DEBUG_TEST 1
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_TEST 0
#endif

/*
 * Creating walk time intervals and tattoo time intervals in global variable with min values and max values
 */
#define WALK_MIN_T 5
#define WALK_MAX_T 10
#define TATOO_MIN_T 5
#define TATOO_MAX_T 10

/*
 * Creation of different global variables "color" to increase visibility
 */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/*
 * Definition of the "paramTTattoueur" structure
 * This structure is used to transmit several parameters when creating tattoist threads
 *
 * "id_thread_tattoueurs" corresponds to the identity of the tattoist thread
 * "number_tatoos" is the number of tattoos that we want to do (passed in parameters)
 * "nb_tattoo_eff" the number of tattoos that have been done
 * "number_tattoos_per_tattoo" the number of tattoos that will be performed by each tattoist
 */
typedef struct paramTTattoueur {
	int id_thread_tattoueurs;
	int nombre_tatoos;
}param_t_tattoo;


/*
 * Definition of the "paramTClient" structure
 * This structure is used to transmit several parameters when creating client threads
 *
 * "id_thread_client" corresponds to the identity of the client thread
 * "nb_tattouage" number of tatoos received by the client
 */
typedef struct paramTClient {
	int id_thread_client;
}param_t_client;


/*
 * Declaration of mutex
 */
pthread_mutex_t promenadance;
pthread_mutex_t promenadance_end;
pthread_mutex_t tattooist_signal;
pthread_mutex_t mut_tattoo_eff;

pthread_mutex_t stats_client_mut;
pthread_mutex_t stats_tattooist_mut;


/*
 * Declaration of semaphores
 */
sem_t sem_seats;
sem_t sem_start_tattoo;
sem_t sem_end_tattoo;


/*
 * Define fonctions "randomWalk" and "randomTatoo"
 * These fonctions are used to get time for the walking and making tattoos at random with rand_r
 */
int randomWalk(int a, int b);
int randomTatoo(int a, int b);

/*
 * Define functions link to client threads
 */
void *client(void *params);
void salle_attente(param_t_client *params);

/*
 * Define functions link to tattoist threads
 */
void *tatoueur(void *params);
void tattouage (param_t_tattoo *params);

/*
 * Define statistics function
 */
void stats();

/*
 * Definition of global variable used and modified during program execution
 *
 * "nombre_siege_disponible" permit to know how many seats are available in the waiting room
 * "nombre_tattoo_eff" as same like "nb_tattoo_eff" in tattoist structure
 * "seed" was used in rand_r function
 * *stats_client and *stats_tattouist for make statistics
 */
extern int nombre_siege_disponible;
extern int nombre_tattoo_eff;
extern unsigned int seed;

extern int *stats_client;
extern int *stats_tattooist;
#endif
