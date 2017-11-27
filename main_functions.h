//
// Created by Anas Guetarni on 22/11/2017.
//

#ifndef TATTOOSHOP_MAIN_FUNCTIONS
#define TATTOOSHOP_MAIN_FUNCTIONS

#include <semaphore.h>

#define WALK_MIN_T 30
#define WALK_MAX_T 50
#define TATOO_MIN_T 2
#define TATOO_MAX_T 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct promenadeT {
    int walk_min_t;
    int walk_max_t;
} promenade_t;

typedef struct tattouageT {
    int tatoo_min_t;
    int tatoo_max_t;
} tattouage_t;

typedef struct tatoueurT {
    int id_tatoueur;
    int number_seats;
    int time_tatoo;
}tattoueur_t;

typedef struct clientT {
    int id_client;
    int time_promenade;
}client_t;

typedef struct {
    int count;
    pthread_mutex_t *mutex;
    pthread_mutex_t *block;
} barrier_t;

pthread_mutex_t mutex_salle_attente;
sem_t semaphore_balade;
barrier_t barrier;

int randomWalk(promenade_t unePromenade);
int randomTatoo(tattouage_t unTatoo);
void barrier_init(barrier_t *b, int count);
void barrier_wait(barrier_t *b);
void barrier_destroy(barrier_t *b);

#endif TATTOOSHOP_MAIN_FUNCTIONS
