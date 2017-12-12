#ifndef TATTOOSHOP_MAIN_FUNCTIONS
#define TATTOOSHOP_MAIN_FUNCTIONS

#define DEBUG 0

#ifdef DEBUG
#define DEBUG_TEST 1
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_TEST 0
#endif


#define WALK_MIN_T 5
#define WALK_MAX_T 10
#define TATOO_MIN_T 5
#define TATOO_MAX_T 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct paramTTattoueur {
	int id_thread_tattoueurs;
	int nombre_tatoos;
}param_t_tattoo;

typedef struct paramTClient {
	int id_thread_client;
}param_t_client;

pthread_mutex_t promenadance;
pthread_mutex_t promenadance_end;
pthread_mutex_t tattooist_signal;
pthread_mutex_t mut_tattoo_eff;

pthread_mutex_t stats_client_mut;
pthread_mutex_t stats_tattooist_mut;

sem_t sem_seats;
sem_t sem_start_tattoo;
sem_t sem_end_tattoo;

int randomWalk(int a, int b);
int randomTatoo(int a, int b);

void *client(void *params);
void salle_attente(param_t_client *params);
void *tatoueur(void *params);
void tattouage (param_t_tattoo *params);
void stats();

extern int nombre_siege_disponible;
extern int nombre_tattoo_eff;
extern unsigned int seed;

extern int *stats_client;
extern int *stats_tattooist;
#endif
