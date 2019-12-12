#include <pthread.h>
#include <stdio.h>   // printf etc.
#include <stdlib.h>
#include <stdint.h>

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
    int32_t counter;
} shared_counter_t;

shared_counter_t * shared_counter_new(void);
void shared_counter_drop(shared_counter_t sc);
shared_counter_increment(shared_counter_t * sc);
int32_t shared_counter_value(const shared_counter_t * sc);

typedef struct _thread_data_t {
  int32_t thread_id;
  shared_counter_t * shared_counter;
} thread_data_t;

/**
 * Create and allocate in heap a counter designed to be shared across threads.
 * @return pointer to heap allocated shared_counter_t.
 */
shared_counter_t * shared_counter_new(void) {
    shared_counter_t * sc = malloc(sizeof(shared_counter_t *));
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

void shared_counter_increment(shared_counter_t * sc) {
    // TODO: Might need something like a ???.
    sc->counter += 1;
}

int shared_counter_value(const shared_counter_t * sc) {
    // TODO: Read need something or not?
    return sc->counter;
}
///////////////////////////////////////////////////////////////////////////////

// Main program.

#define MAX_THREADS 3

void * run(void * args) {
    thread_data_t * data = (thread_data_t *) args;
    printf("Bonjour depuis le thread: %i.\n", data->thread_id);
    // Let's work thread! Increment counter 10000 times.
    for (int32_t i = 0; i < 10000; i++) {
        shared_counter_increment(data->shared_counter);
    }
    // End our thread it worked well, can gain some rest.
    pthread_exit(NULL);
}

int main(void) {
    printf("The main thread is started\n");

    // Create an array for thread struct
    pthread_t threads[MAX_THREADS];
    // Array for data passed to threads
    thread_data_t thr_data[MAX_THREADS];

    // Create shared ressource: a shared counter
    // Let's discovers joy and pitfalls of concurrent programming ;)
    shared_counter_t * shared_counter;
    shared_counter_new();

    // Create `MAX_THREADS` threads
    for (int32_t i = 0; i < MAX_THREADS; i += 1) {
        thr_data[i].thread_id = i;
        // Take a pointer to shared counter
        thr_data[i].shared_counter = &shared_counter;

        printf("Création du thread %i\n", i);
        int32_t failure_reason = pthread_create(&threads[i], NULL, run, &thr_data[i]);
        if (failure_reason) {
            fprintf(stderr, "Error: pthread_create, rc: %d\n", failure_reason);
            return EXIT_FAILURE;
        }
    }

    // Joins threads, block until they finished their work.
    for (int32_t i = 0; i < MAX_THREADS; i += 1) {
        pthread_join(threads[i], NULL);
    }

    // We expect the counter to be equals to MAX_THREAD * 10000.
    // How can we correct? :) Indice: pthread_mutex_init, pthread_cond_init
    printf("Résultat du compteur partagé: %i\n", shared_counter_value(shared_counter));

    shared_counter_drop(shared_counter);
    printf("Les threads ont fini leur travail et son terminées le thread main peut maintenant se terminer\n");
}