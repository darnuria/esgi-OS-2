#include <pthread.h>
#include <stdio.h>   // printf etc.
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// Objectif du TP - découvrir les bases de la programmation concurrente et
// et parallelles
//
// On va réaliser un compteur partagé
//
// Bonne référence sur le sujet:
// https://randu.org/tutorials/threads/
//
// Exercices:
// - Corriger les erreurs de compilation (pourquoi? Pour vous faire lire le fichier)
// - Lancer plusieurs fois ce code observer les différences entre 8 lancements
// - Commenter les fonctions dans le style des docstrings (resemble bcp au jsdoc).
// - 
///////////////////////////////////////////////////////////////////////////////

// Shared_counter functions and type definitions.
typedef struct _shared_counter_t {
    int counter;
} shared_counter_t;

shared_counter_t * shared_counter_new(void);
void shared_counter_drop(shared_counter_t * sc);
void shared_counter_increment(shared_counter_t * shared_counter);
int shared_counter_value(const shared_counter_t * shared_counter);

typedef struct _thread_data_t {
  int thread_id;
  shared_counter_t * shared_counter;
} thread_data_t;

/**
 * Create and allocate in heap a counter designed to be shared across threads.
 * @return pointer to heap allocated shared_counter_t.
 */
shared_counter_t * shared_counter_new(void) {
    shared_counter_t * sc = malloc(sizeof(shared_counter_t));
    sc->counter = 0;
    return sc;
}

/**
 * Handle destruction of a heap allocated shared counter.
 * ⚠ Beaware that threads might have an acces to this counter
 * after deletion!
 * @param sc Shared counter to delete.
 */
void shared_counter_drop(shared_counter_t * sc) {
    // TODO: Might need some change to work really in the way we attend ;)
    free(sc);
}

void shared_counter_increment(shared_counter_t * shared_counter) {
    // TODO: Might need something like a ???.
    shared_counter->counter += 1;
}

int shared_counter_value(const shared_counter_t * shared_counter) {
    // TODO: Read need something or not?
    return shared_counter->counter;
}
///////////////////////////////////////////////////////////////////////////////

// Main program.

#define MAX_THREADS 3
void * run(void * args) {
    thread_data_t * data = (thread_data_t *) args;
    printf("Bonjour depuis le thread: %i.\n", data->thread_id);
    // Let's work thread! Increment counter 10000 times.
    for (int i = 0; i < 10000; i++) {
        shared_counter_increment(data->shared_counter);
    }
    // End our thread it worked well, can gain some rest.
    pthread_exit(NULL);
}

int main(void) {
    printf("The main thread is started\n");

    // On crée un tableau pour les structures pthreads.
    pthread_t threads[MAX_THREADS];
    // on crée un tableau pour les data passées au thread.
    thread_data_t thr_data[MAX_THREADS];

    // On crée une ressource partagée ici un compteur partagé
    // Et on va découvrir les problèmes de partager sans politique de partage ;)
    shared_counter_t * shared_counter = shared_counter_new();

    // On va créer `MAX_THREADS` threads
    for (int i = 0; i < MAX_THREADS; i += 1) {
        thr_data[i].thread_id = i;
        // On passe un pointeur sur shared_counter
        thr_data[i].shared_counter = shared_counter;
        // Petit bug à la ligne suivante checkez vos erreurs de compilation
        int failure_reason = pthread_create(&threads[i], NULL, run, &thr_data[i]);
        if (failure_reason) {
            fprintf(stderr, "Error: pthread_create, rc: %d\n", failure_reason);
            return EXIT_FAILURE;
        }
        printf("Création du thread %i\n", i);
    }

    // Joins threads, block until they finished their work.
    
    // We create `MAX_THREADS` threads
    for (int i = 0; i < MAX_THREADS; i += 1) {
        pthread_join(threads[i], NULL);
    }

    // We expect the counter to be equals to MAX_THREAD * 10000.
    // How can we correct? :) Indice: pthread_mutex_init, pthread_cond_init
    printf("Résultat du compteur partagé: %i\n", shared_counter_value(shared_counter));

    printf("Les threads ont fini leur travail et son terminées le thread main peut maintenant se terminer\n");
}