#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_SEATS 5  // Maximum waiting students

pthread_mutex_t mutex;
pthread_cond_t ta_sleep;
sem_t seats;
int waiting_students = 0;
int student_count;  // Number of students yet to be processed

void *ta_function(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex);

        // Exit condition: no students left and all have been processed
        if (student_count == 0 && waiting_students == 0) {
            printf("TA exits as all students are done.\n");
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }

        while (waiting_students == 0) {
            printf("TA is sleeping...\n");
            pthread_cond_wait(&ta_sleep, &mutex);
        }

        pthread_mutex_unlock(&mutex);

        printf("TA is helping a student...\n");
        sleep(rand() % 3 + 1); // Simulate help time

        pthread_mutex_lock(&mutex);
        waiting_students--;
        sem_post(&seats); // Free a seat
        pthread_mutex_unlock(&mutex);
    }
}

void *student_function(void *arg) {
    int id = *(int*)arg;

    pthread_mutex_lock(&mutex);
    if (waiting_students < MAX_SEATS) {
        waiting_students++;
        printf("Student %d is waiting...\n", id);
        pthread_mutex_unlock(&mutex); // Unlock before sem_wait to prevent deadlock
        sem_wait(&seats); // Occupy a seat (may block but mutex is released)
        pthread_cond_signal(&ta_sleep); // Wake TA if needed
    } else {
        printf("Student %d found no seat and leaves.\n", id);
        pthread_mutex_unlock(&mutex);
    }

    // Decrement student_count safely
    pthread_mutex_lock(&mutex);
    student_count--;
    pthread_mutex_unlock(&mutex);

    free(arg);
    return NULL;
}

int main() {
    pthread_t ta;
    pthread_t *students;

    printf("Enter the number of students: ");
    scanf("%d", &student_count);
    int initial_student_count = student_count; // Store initial count

    students = malloc(student_count * sizeof(pthread_t));

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&ta_sleep, NULL);
    sem_init(&seats, 0, MAX_SEATS);

    pthread_create(&ta, NULL, ta_function, NULL);

    for (int i = 0; i < initial_student_count; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&students[i], NULL, student_function, id);
        sleep(rand() % 2); // Stagger student arrivals
    }

    // Wait for all students to finish
    for (int i = 0; i < initial_student_count; i++) {
        pthread_join(students[i], NULL);
    }

    // Ensure TA exits after all students are done
    pthread_join(ta, NULL);

    free(students);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&ta_sleep);
    sem_destroy(&seats);

    return 0;
}
//compile with  gcc -pthread ta_simulation.c -o ta_simulation./ta_simulation