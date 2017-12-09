#ifndef TATTOOSHOP_MAIN_FUNCTIONS
#define TATTOOSHOP_MAIN_FUNCTIONS

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
    int nb_tattoo_eff;
}param_t_tattoo;

pthread_mutex_t promenadance;
pthread_mutex_t promenadance_end;
pthread_mutex_t tattooist_signal;
pthread_mutex_t mut_tattoo_eff;

sem_t sem_seats;
sem_t sem_start_tattoo;
sem_t sem_end_tattoo;

int randomWalk(int a, int b);
int randomTatoo(int a, int b);

void *client(void *id_thread);
void salle_attente(int *id_thread);
void *tatoueur(void *params);
void tattouage (param_t_tattoo *params);

extern int nombre_siege_disponible;
extern int nombre_tattoo_eff;
extern unsigned int seed;

#endif
