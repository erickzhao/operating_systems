#include <stdio.h>    // for printf and scanf
#include <stdlib.h>   // for malloc
#include <pthread.h>  // for threads
#include <limits.h>   // for intmax
#include <string.h>   // for memcpy
#include <unistd.h>   // for sleep
#include <time.h>     // for rand

int *avail, **max, **need, **hold, numProcesses, numResources;
pthread_mutex_t lock;
pthread_mutex_t lock2;

int isAvail(int pr_id, int* request_vector) {
  int j;
  for (j=0; j<numResources; j++) {
    if (request_vector[j] > avail[j]) {
      return 0;
    }
  }

  return 1;
}

int isSafe() {
  int *work, *finish;
  int i,j;

  // 1. initialize temporary work, finish arrays

  // work set to avail
  work = malloc(sizeof(avail));
  memcpy(work, avail, sizeof(&avail));
  // finish set to all false
  finish = malloc(numProcesses*sizeof(int));
  for (i=0;i<numProcesses;i++) {
    finish[i] = 0;
  }

  while(1) {
    // no selected index at first
    int selectedIndex = INT_MIN;

    // 2. find a process index i such that finish[i] is not true
    // AND all need[i] values are smaller than their work counterparts
    for (i=0;i<numProcesses;i++) {
      int isSelected = 1; // set selected boolean to true at first
      if (!finish[i]) {
        for (j=0;j<numResources;j++) {
          if (need[i][j] > work[j]) {
            isSelected = 0; // if need exceeds work, can't select
            break;
          }
        }
      } else {
        isSelected = 0; // if finished, we can't select it
      }

      // if we find process to select, break loop
      if (isSelected) {
        selectedIndex = i;
        break;
      }
    }

    // if no process found, skip to step 4
    if (selectedIndex == INT_MIN) {
      break;
    }

    // 3. update work and finish, then repeat loop
    for (j=0;j<numResources;j++) {
      work[j] = work[j] + hold[i][j];
    }
    finish[i] = 1;
  }

  // 4. evaluate if safe or not
  // if all processes are marked as finish, then safe, otherwise not
  for (i=0; i<numProcesses;i++) {
    if (!finish[i]) {
      return 0;
    }
  }
  return 1;
}

int bankers_algorithm(int pr_id, int* request_vector) {
  int safe = 0;
  int j;
  for (j=0; j < numResources; j++) {
    if (request_vector[j] > need[pr_id][j]) {
      return 0;
    }
  }

  if (!isAvail(pr_id, request_vector)) {
    return 0;
  }

  pthread_mutex_lock(&lock);
  for (j=0;j<numResources;j++) {
    avail[j] = avail[j] - request_vector[j];
    hold[pr_id][j] = hold[pr_id][j] + request_vector[j];
    need[pr_id][j] = need[pr_id][j] - request_vector[j];
  }
  printf("[%d] Checking if allocation is safe...\n", pr_id);
  safe = isSafe();
  if (!safe) {
    printf("[%d] Allocation is not safe! Cancelling.\n", pr_id);
    for (j=0;j<numResources;j++) {
      avail[j] = avail[j] + request_vector[j];
      hold[pr_id][j] = hold[pr_id][j] - request_vector[j];
      need[pr_id][j] = need[pr_id][j] + request_vector[j];
    }
  } else {
    printf("[%d] System is safe! Allocating.\n", pr_id);
  }
  pthread_mutex_unlock(&lock);
  
  return safe;
}

void* process_simulator(void* pr_id) {
  // 1. process begins
  int id = (int) pr_id;
  printf("Initializing process %d...\n", id);
  int j;
  while (1) {
    // 2. randomly generate request vector
    int *req = malloc(numResources*sizeof(int));
    // populate request vector with random number between 0 and need[j]
    pthread_mutex_lock(&lock2);
    printf("[%d] The resource vector requested array is: ", id);
    for (j=0; j<numResources;j++) {
      int rnd = rand()%(need[id][j] + 1);
      req[j] = rnd;
      printf("%d ", req[j]);
    }
    printf("\n");
    pthread_mutex_unlock(&lock2);

    // 3. run bankers
    // 4. inside banker's alg, resources are acquired
    // 6. repeat continuously until safe allocation is found
    while(!bankers_algorithm(id, req)) {
      sleep(1);
    };

    // 5. check if any requests remaining
    // if so, sleep for 3s otherwise exit and free resources
    int isDone = 1;
    for(j=0;j<numResources;j++) {
      if (need[id][j] > 0) {
        isDone = 0;
        break;
      }
    }

    if (isDone) {
      printf("Allocation done! Freeing resources and exiting process %d.\n", id);
      pthread_mutex_lock(&lock);
      for(j=0; j<numResources;j++) {
        avail[j] = avail[j] + hold[id][j];
        hold[id][j] = 0;
      }
      pthread_mutex_unlock(&lock);
      break;
    } else {
      sleep(3);
    }
  }

  return 0;
}

int main() {
  srand(time(NULL));
  int i, j;

  printf("Enter number of processes: ");
  scanf("%d", &numProcesses);

  printf("Enter number of resources: ");
  scanf("%d", &numResources);

  avail = (int *) malloc(numResources*sizeof(int));

  max = (int **) malloc(numProcesses*sizeof(int *));
  for (i=0; i<numProcesses; i++) {
    max[i] = (int *) malloc(numResources*sizeof(int));
  }

  need = (int **) malloc(numProcesses*sizeof(int *));
  for (i=0; i<numProcesses; i++) {
    need[i] = (int *) malloc(numResources*sizeof(int));
  }

  hold = (int **) malloc(numProcesses*sizeof(int *));
  for (i=0; i<numProcesses; i++) {
    hold[i] = (int *) malloc(numResources*sizeof(int));
  }

  for (i=0; i<numProcesses; i++) {
    for (j=0; j<numResources; j++) {
      hold[i][j] = 0;
    }
  }

  printf("Enter available quantity for each resource (space-separated): ");
  for (i=0; i<numResources; i++) {
    scanf("%d", &avail[i]);
  }

  printf("Enter maximum resources each process can claim: \n");
  for (i=0; i<numProcesses; i++) {
    for (j=0; j<numResources; j++) {
      scanf("%d", &max[i][j]);
      need[i][j] = max[i][j];
    }
  }

  printf("----------------\n");

  printf("The number of each resource in the system is: ");
  for (i=0; i< numResources;i++) {
    printf("%d ", avail[i]);
  }
  printf("\n");

  printf("The allocated resources table is:\n");
  for (i=0; i<numProcesses; i++) {
    for (j=0; j<numResources; j++) {
      printf("%d ", hold[i][j]);
    }
    printf("\n");
  }

  printf("The maximum claim table is:\n");
  for (i=0; i<numProcesses; i++) {
    for (j=0; j<numResources; j++) {
      printf("%d ", max[i][j]);
    }
    printf("\n");
  }

  printf("The needed resource table is:\n");
  for (i=0; i<numProcesses; i++) {
    for (j=0; j<numResources; j++) {
      printf("%d ", need[i][j]);
    }
    printf("\n");
  }

  printf("----------------\n");

  printf("Initializing %d processes...\n", numProcesses);
  pthread_t threads[numProcesses];

  for (i=0; i<numProcesses; i++) {
    pthread_create((void*) &threads[i], NULL, &process_simulator, (void *)(intptr_t)i);
  }
  
  pthread_exit(NULL);
}