//
// Created by Anas Guetarni on 22/11/2017.
//

#ifndef TATTOOSHOP_MAIN_FUNCTIONS
#define TATTOOSHOP_MAIN_FUNCTIONS

#define WALK_MIN_T 5
#define WALK_MAX_T 10
#define TATOO_MIN_T 2
#define TATOO_MAX_T 10

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct tatoueurT {
    int id_tatoueur;
}tattoueur_t;

typedef struct clientT {
    int id_client;
}client_t;

typedef struct paramT {
    int id_thread_client;
    int id_thread_tattoueurs;
    int nombre_tatoos;
    int nombre_siege_disponible;
}param_t;

pthread_mutex_t promenadance;
pthread_mutex_t promenadance_end;
pthread_mutex_t tattoueur_reveil;
pthread_mutex_t mut_tattoo_eff;

sem_t sem_fauteuils;
sem_t sem_start_tattoo;
sem_t sem_end_tattoo;
sem_t porte;

int randomWalk(int a, int b);
int randomTatoo(int a, int b);

void *salle_attente(void *params);
void *promenade(void *params);
void *tattoueur (void *params);

#endif
