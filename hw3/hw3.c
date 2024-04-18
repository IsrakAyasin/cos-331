#include <stdlib.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <semaphore.h> 
#include <sys/stat.h> 
#include <stdio.h>
#include <pthread.h> 
#include <time.h>

int random_array[1024][1024];
int total_random[256] = {0};
int num_threads;
char sem_names[256][10];
sem_t *My_Sems[256];
sem_t mutex;

// Thread function for 0 semaphore solution
void *thread_function_0_semaphore(void *param) {
    int thread_id = *(int *)param;
    int start_row = (1024 / num_threads) * thread_id;
    int end_row = (1024 / num_threads) * (thread_id + 1);
    int My_Array[256] = {0}; // Local array for counting

    // Count the occurrences of each value in the assigned rows
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < 1024; j++) {
            My_Array[random_array[i][j]]++;
        }
    }

    // Add counts to the global array without protection (no semaphore)
    for (int i = 0; i < 256; i++) {
        total_random[i] += My_Array[i];
    }

    pthread_exit(NULL);
}

// Thread function for 1 semaphore solution
void *thread_function_1_semaphore(void *param) {
    int thread_id = *(int *)param;
    int start_row = (1024 / num_threads) * thread_id;
    int end_row = (1024 / num_threads) * (thread_id + 1);
    int My_Array[256] = {0}; // Local array for counting

    // Count the occurrences of each value in the assigned rows
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < 1024; j++) {
            My_Array[random_array[i][j]]++;
        }
    }

    // Protect the critical section using a single semaphore
    sem_wait(&mutex);
    for (int i = 0; i < 256; i++) {
        total_random[i] += My_Array[i];
    }
    sem_post(&mutex);

    pthread_exit(NULL);
}

// Thread function for 256 semaphore solution
void *thread_function_256_semaphore(void *param) {
    int thread_id = *(int *)param;
    int start_row = (1024 / num_threads) * thread_id;
    int end_row = (1024 / num_threads) * (thread_id + 1);
    int My_Array[256] = {0}; // Local array for counting

    // Count the occurrences of each value in the assigned rows
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < 1024; j++) {
            My_Array[random_array[i][j]]++;
        }
    }
    // Protect the critical section using semaphores
    for (int i = 0; i < 256; i++) {
        sem_wait(My_Sems[i]);
        total_random[i] += My_Array[i];
        sem_post(My_Sems[i]);
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    long seed = 8;
    srand((unsigned) seed);
    clock_t start_time, end_time;
    double time_taken1, time_taken2, time_taken3;

    // Populate the random_array
    for (int i = 0; i < 1024; i++) {
        for (int j = 0; j < 1024; j++) {
            random_array[i][j] = rand() % 256;
        }
    }

    num_threads = atoi(argv[1]);
    
    // Initialize semaphores
    for (int i = 0; i < 256; i++) {
        sprintf(sem_names[i], "S%d", i);
        My_Sems[i] = sem_open(sem_names[i], O_CREAT, 0777, 1);
    }

    // Initialize the single semaphore for 1 semaphore solution
    sem_init(&mutex, 0, 1);

    // Create threads
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    // Measure time for 0 semaphore solution
    start_time = clock();
    // Create threads for 0 semaphore solution
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function_0_semaphore, &thread_ids[i]);
    }
    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    end_time = clock();
    time_taken1 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    // Printing all the results for 0 semaphore
    printf("\nTotal counts for 0 semaphore:\n");
    for (int i = 0; i < 256; i++) {
        printf("total_random[%d] = %d\n", i, total_random[i]);
    }

    // Measure time for 1 semaphore solution
    start_time = clock();
    // Create threads for 1 semaphore solution
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function_1_semaphore, &thread_ids[i]);
    }
    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    end_time = clock();
    time_taken2 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    // Printing all the results for 1 semaphore
    printf("\nTotal counts for 1 semaphore:\n");
    for (int i = 0; i < 256; i++) {
        printf("total_random[%d] = %d\n", i, total_random[i]);
    }

    // Measure time for 256 semaphore solution
    start_time = clock();
    // Create threads for 256 semaphore solution
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function_256_semaphore, &thread_ids[i]);
    }
    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    end_time = clock();
    time_taken3 = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    // Printing all the results for 256 semaphore
    printf("\nTotal counts for 256 semaphore:\n");
    for (int i = 0; i < 256; i++) {
        printf("total_random[%d] = %d\n", i, total_random[i]);
    }

    
    printf("0 semaphore solution time: %f seconds\n", time_taken1);
    printf("1 semaphore solution time: %f seconds\n", time_taken2);
    printf("256 semaphore solution time: %f seconds\n", time_taken3);

    // Close and unlink semaphores
    for (int i = 0; i < 256; i++) {
        sem_close(My_Sems[i]);
        sem_unlink(sem_names[i]);
    }

    return 0;
}
