/*
----------------- COMP 310/ECSE 427 Winter 2018 -----------------
Dimitri Gallos
Assignment 2 Skeleton

-----------------------------------------------------------------
I declare that the awesomeness below is a genuine piece of work
and falls under the McGill code of conduct, to the best of my knowledge.
-----------------------------------------------------------------
 */

//Please enter your name and McGill ID below
//Name: Erick Zhao
//McGill ID: 260687719

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <semaphore.h>

int BUFFER_SIZE = 100; //size of queue

sem_t empty_count;
sem_t full_count;
sem_t mutex;

// A structure to represent a queue
struct Queue
{
    int front, rear, size;
    unsigned capacity;
    int* array;
};

// function to create a queue of given capacity. 
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // This is important, see the enqueue
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes equal to the capacity 
int isFull(struct Queue* queue)
{
    return ((queue->size ) >= queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.  
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue. 
// It changes front and size
int dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

void print(struct Queue* queue){
    if (queue->size == 0){
        return;
    }
    
    for (int i = queue->front; i < queue->front + queue->size; i++){
        printf(" Element at position %d is %d \n ", i % (queue->capacity), queue->array[i % (queue->capacity)]);
    }
}

struct Queue* queue;

/*Producer Function: Simulates an Airplane arriving and dumping 5-10 passengers to the taxi platform */
void *FnAirplane(void* cl_id)
{
    int plane_id = (int) cl_id;
    while(1) {
        // generate random number between 5 and 10
        int num_people = (rand() % 6) + 5;
        printf("Airplane %d arrives with %d passengers\n", plane_id, num_people);
        // loop for each passenger
        for (int i = 0; i < num_people; i++) {
            // if full, other passengers go away
            // otherwise, approach critical section to go to platform
            if (isFull(queue)) {
                printf("Platform is full: Rest of passengers of plane %d take the bus\n", plane_id);
                break;
            } else {
                sem_wait(&empty_count);
                sem_wait(&mutex);
                int passenger_id = 1000000+plane_id*1000+i;
                printf("Passenger %d arrives to platform\n",passenger_id);
                enqueue(queue, passenger_id);
                sem_post(&mutex);
                sem_post(&full_count);
            }
        }
        sleep(1);
    }
}

/* Consumer Function: simulates a taxi that takes n time to take a passenger home and come back to the airport */
void *FnTaxi(void* pr_id)
{
    int taxi_id = (int) pr_id;
    while(1) {
        printf("Taxi driver %d arrives\n", taxi_id);
        // continuously check if empty queue
        // then when queue is not empty, attempt to approach critical section
        if (isEmpty(queue)) {
            printf("Taxi driver %d waits for passengers to enter the platform \n", taxi_id);
            while(isEmpty(queue));
        }
        sem_wait(&full_count);
        sem_wait(&mutex);
        int passenger_id = dequeue(queue);
        printf("Taxi driver %d picks up client %d from the platform\n", taxi_id, passenger_id);
        sem_post(&mutex);
        sem_post(&empty_count);
        int taxi_time = ((float)(rand() % 21 + 10)/60.0)*1000000;
        usleep(taxi_time);
    }
}

int main(int argc, char *argv[])
{
    // initialize random number generator
    srand(time(NULL));

    int num_airplanes;
    int num_taxis;

    if (!argv[1] || !argv[2]) {
        printf("You didn't enter correct arguments!\n");
        return 1;
    }

    num_airplanes=atoi(argv[1]);
    num_taxis=atoi(argv[2]);

    printf("You entered: %d airplanes per hour\n",num_airplanes);
    printf("You entered: %d taxis\n", num_taxis);

    //initialize queue
    queue = createQueue(BUFFER_SIZE);

    //declare arrays of threads and initialize semaphore(s)
    pthread_t taxi_threads[num_taxis];
    pthread_t airplane_threads[num_airplanes];

    sem_init(&empty_count, 0, BUFFER_SIZE);
    sem_init(&full_count, 0, 0);
    sem_init(&mutex, 0, 1);

    //create threads for airplanes
    for (int i=0; i<num_airplanes;i++) {
        printf("Creating airplane thread %d\n", i);
        pthread_create((void*) &airplane_threads[i], NULL, &FnAirplane, (void *)(intptr_t)i);
    }

    //create threads for taxis
    for (int j=0; j<num_taxis; j++) {
        pthread_create((void*) &taxi_threads[j], NULL, &FnTaxi, (void *)(intptr_t)j);
    }

    // join all threads
    for (int m=0;m<num_airplanes;m++) {
        pthread_join(airplane_threads[m],NULL);
    }
    for (int n=0;n<num_taxis;n++) {
        pthread_join(taxi_threads[n],NULL);
    }

    pthread_exit(NULL);
}
