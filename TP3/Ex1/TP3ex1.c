/**
 * TP3 - Exercice 1
 * Programme qui crée un tableau de 1e8 entiers, initialise chaque case avec une valeur aléatoire,
 * crée n threads, chaque thread calcule le min et le max d'une partie du tableau, et le résultat est affiché par le processus principal
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE (int)1e8
#define THREAD_COUNT 8 //2 //4 

int tab[SIZE];
pthread_mutex_t mutex;
int min_result, max_result;

// Function executed by each thread
// Each thread finds the min and max of a part of the array
void *find_min_max(void *arg) {
    int thread_id = *(int *)arg;
    int chunk_size = SIZE / THREAD_COUNT; // Number of elements to process by each thread
    int start = thread_id * chunk_size; 
    int end = (thread_id == THREAD_COUNT - 1) ? SIZE : (thread_id + 1) * chunk_size; // Last thread processes the remaining elements

    int local_min = tab[start];
    int local_max = tab[start];

    // Find local min and max
    for (int i = start + 1; i < end; ++i) {
        if (tab[i] < local_min) {
            local_min = tab[i];
        }
        if (tab[i] > local_max) {
            local_max = tab[i];
        }
    }

    // Update global min and max using mutex
    pthread_mutex_lock(&mutex);
    if (local_min < min_result) {
        min_result = local_min;
    }
    if (local_max > max_result) {
        max_result = local_max;
    }
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int thread_ids[THREAD_COUNT];
    srand(time(NULL));

    // Initialize the array with random values
    for (int i = 0; i < SIZE; ++i) {
        tab[i] = rand();
    }

    // Initialize variables
    pthread_mutex_init(&mutex, NULL);
    min_result = tab[0];
    max_result = tab[0];
    struct timeval start_time, end_time;

    
    gettimeofday(&start_time, NULL); // -- Start measuring time --

    // Create threads
    for (int i = 0; i < THREAD_COUNT; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, find_min_max, (void *)&thread_ids[i]);
    }

    // Join threads
    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end_time, NULL); // -- Stop measuring time --

    // Calculate elapsed time in seconds
    long elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000000 + end_time.tv_usec - start_time.tv_usec;

    // Print results
    printf("Min: %d\n", min_result);
    printf("Max: %d\n", max_result);
    printf("Time: %ld microseconds\n", elapsed_time);
    printf("Size of array: %d\n", SIZE);
    printf("Number of threads: %d\n", THREAD_COUNT);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
