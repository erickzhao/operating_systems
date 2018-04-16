#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc
#include <limits.h> //for intmax

int main() {
  int *avail, **max, **hold, numProcesses, numResources, i, j;

  printf("Enter number of processes: ");
  scanf("%d", &numProcesses);

  printf("Enter number of resources: ");
  scanf("%d", &numResources);

  avail = (int *) malloc(numResources*sizeof(int));

  max = (int **) malloc(numProcesses*sizeof(int *));
  for (i=0; i<numProcesses; i++) {
    max[i] = (int *) malloc(numResources*sizeof(int));
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

  printf("----------------\n");

  return 1;
}

void* process_simulator(void* pr_id) {
  return 0;
}

int bankers_algorithm(int pr_id, int* request_vector) {
    return 0;
}

int isSafe() {

    int isSafe = 0;
    return isSafe;

}