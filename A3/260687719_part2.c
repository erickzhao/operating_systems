#include <stdio.h>  //for printf and scanf
#include <stdlib.h> //for malloc
#include <limits.h> //for intmax

#define LOW 0
#define HIGH 199
#define START 53

//compare function for qsort
//you might have to sort the request array
//use the qsort function
// an argument to qsort function is a function that compares 2 quantities
//use this there.
int cmpfunc(const void *a, const void *b)
{
  return (*(int *)a - *(int *)b);
}

//function to swap 2 integers
void swap(int *a, int *b)
{
  if (*a != *b)
  {
    *a = (*a ^ *b);
    *b = (*a ^ *b);
    *a = (*a ^ *b);
    return;
  }
}

//Prints the sequence and the performance metric
void printSeqNPerformance(int *request, int numRequest)
{
  int i, last, acc = 0;
  last = START;
  printf("\n");
  printf("%d", START);
  for (i = 0; i < numRequest; i++)
  {
    printf(" -> %d", request[i]);
    acc += abs(last - request[i]);
    last = request[i];
  }
  printf("\nPerformance : %d\n", acc);
  return;
}

//access the disk location in FCFS
void accessFCFS(int *request, int numRequest)
{
  //simplest part of assignment
  printf("\n----------------\n");
  printf("FCFS :");
  printSeqNPerformance(request, numRequest);
  printf("----------------\n");
  return;
}

//access the disk location in SSTF
void accessSSTF(int *request, int numRequest)
{

  // create array to keep track of which indexes are unused
  // initialize all indexes to 0
  int *isAccessed = malloc(sizeof(request));
  int i;
  for (i = 0; i < numRequest; i++) {
    isAccessed[i] = 0;
  }

  // create array to insert access locations in new SSTF order
  // set current location at predefined START
  int *requestSSTF = malloc(sizeof(request));
  int current = START;
  int numSSTF = 0;

  // find shortest time first until we've gotten the number of requests
  while (numSSTF < numRequest)
  {
    int minDistance = INT_MAX;
    int minIndex;
    int i;
    for (i = 0; i < numRequest; i++) {
      // only consider locations if they haven't been accessed yet
      if (!isAccessed[i])
      {
        // if shorter distance is found, set location as min distance
        int distance = abs(request[i] - current);
        if (distance < minDistance) {
          minIndex = i;
          minDistance = distance;
        }
      }
    }
    // set current location as min distance location, mark as accessed,
    // and push to new array
    current = request[minIndex];
    isAccessed[minIndex] = 1;
    requestSSTF[numSSTF] = current;
    numSSTF++;
  }

  printf("\n----------------\n");
  printf("SSTF :");
  printSeqNPerformance(requestSSTF, numSSTF);
  printf("----------------\n");
  return;
}

//access the disk location in SCAN
void accessSCAN(int *request, int numRequest)
{
  int numSCAN = 0;
  int isRight = START - 100 >= 0;
  int *requestSCAN = malloc(sizeof(request) + sizeof(int));

  // keep index of first element to the right of element
  // default value: no elements to right
  int firstRightIndex = numRequest;

  // sort all elements in increasing order
  qsort(request, numRequest, sizeof(int), cmpfunc);

  // find index of first element to the right of start position
  int i = 0;
  while (i < numRequest) {
    if (request[i] >= START) {
      firstRightIndex = i;
      break;
    }
    i++;
  }

  // start towards whichever side is closer
  if (isRight) {
    // scan through all elements on the right
    for (i = firstRightIndex; i < numRequest; i++) {
      requestSCAN[numSCAN] = request[i];
      numSCAN++;
    }

    // if we need to go in inverse direction,
    // note that we reached the max location
    if (firstRightIndex > 0) {
      requestSCAN[numSCAN] = 199;
      numSCAN++;
    }

    // scan backwards from the centre to the leftmost item
    for (i = firstRightIndex - 1; i >= 0; i--) {
      requestSCAN[numSCAN] = request[i];
      numSCAN++;
    }
  } else {
    // scan backwards from the centre to the leftmost item
    for (i = firstRightIndex - 1; i >= 0; i--) {
      requestSCAN[numSCAN] = request[i];
      numSCAN++;
    }

    // if we need to go in inverse direction,
    // (i.e. there exists a valid first right index)
    // note that we reached the min location
    if (firstRightIndex < numRequest) {
      requestSCAN[numSCAN] = 0;
      numSCAN++;
    }

    // scan through all elements on the right
    for (i = firstRightIndex; i < numRequest; i++) {
      requestSCAN[numSCAN] = request[i];
      numSCAN++;
    }
  }

  printf("\n----------------\n");
  printf("SCAN :");
  printSeqNPerformance(requestSCAN, numSCAN);
  printf("----------------\n");
  return;
}

//access the disk location in CSCAN
void accessCSCAN(int *request, int numRequest)
{
  int numCSCAN = 0;
  int isRight = START - 100 >= 0;
  // allocate 2 more int slots in case we need to jump (record 199 and 0)
  int *requestCSCAN = malloc(sizeof(request) + 2*sizeof(int));

  // keep index of first element to the right of element
  // default value: no elements to right
  int firstRightIndex = numRequest;
  
  int hasJump = 0;

  // sort all elements in increasing order
  qsort(request, numRequest, sizeof(int), cmpfunc);

  // find index of first element to the right of start position
  int i = 0;
  while (i < numRequest) {
    if (request[i] >= START) {
      firstRightIndex = i;
      // if starting right and we have items on left of start
      // or if starting left and we have items on right of start
      // indicate that we have to make a jump
      if ((isRight && firstRightIndex > 0) || (!isRight && firstRightIndex < numRequest)) {
        hasJump = 1;
      }
      break;
    }
    i++;
  }

  if (isRight) {
    // scan through all elements from centre to right
    for (i = firstRightIndex; i < numRequest; i++) {
      requestCSCAN[numCSCAN] = request[i];
      numCSCAN++;
    }

    // if jump, we need to record the jump from extremities
    if (hasJump) {
      requestCSCAN[numCSCAN] = 199;
      numCSCAN++;
      requestCSCAN[numCSCAN] = 0;
      numCSCAN++;
    }

    // scan through all elements from left to centre
    for (i = 0; i < firstRightIndex; i++) {
      requestCSCAN[numCSCAN] = request[i];
      numCSCAN++;
    }
  } else {
    // scan through all elements from centre to left
    for (i = firstRightIndex - 1; i >= 0; i--) {
      requestCSCAN[numCSCAN] = request[i];
      numCSCAN++;
    }

    // if jump, we need to record the jump from extremities
    if (hasJump) {
      requestCSCAN[numCSCAN] = 0;
      numCSCAN++;
      requestCSCAN[numCSCAN] = 199;
      numCSCAN++;
    }

    // scan through all elements from right to start
    for (i = numRequest - 1; i >= firstRightIndex; i--) {
      requestCSCAN[numCSCAN] = request[i];
      numCSCAN++;
    }
  }

  printf("\n----------------\n");
  printf("CSCAN :");
  printSeqNPerformance(requestCSCAN, numCSCAN);
  printf("----------------\n");
  return;
}

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest)
{
  int numLOOK = 0;
  int isRight = START - 100 >= 0;
  int *requestLOOK = malloc(sizeof(request) + sizeof(int));

  // keep index of first element to the right of element
  // default value: no elements to right
  int firstRightIndex = numRequest;

  // sort all elements in increasing order
  qsort(request, numRequest, sizeof(int), cmpfunc);

  // find index of first element to the right of start position
  int i = 0;
  while (i < numRequest) {
    if (request[i] >= START) {
      firstRightIndex = i;
      break;
    }
    i++;
  }

  // start towards whichever side is closer
  if (isRight) {
    // scan through all elements on the right
    for (i = firstRightIndex; i < numRequest; i++) {
      requestLOOK[numLOOK] = request[i];
      numLOOK++;
    }

    // if we need to go in inverse direction,
    // note that we reached the max location
    if (firstRightIndex > 0) {
      requestLOOK[numLOOK] = 199;
      numLOOK++;
    }

    // scan backwards from the centre to the leftmost item
    for (i = firstRightIndex - 1; i >= 0; i--) {
      requestLOOK[numLOOK] = request[i];
      numLOOK++;
    }
  } else {
    // scan backwards from the centre to the leftmost item
    for (i = firstRightIndex - 1; i >= 0; i--) {
      requestLOOK[numLOOK] = request[i];
      numLOOK++;
    }

    // scan through all elements on the right
    for (i = firstRightIndex; i < numRequest; i++) {
      requestLOOK[numLOOK] = request[i];
      numLOOK++;
    }
  }
  printf("\n----------------\n");
  printf("LOOK :");
  printSeqNPerformance(requestLOOK, numLOOK);
  printf("----------------\n");
  return;
}

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest)
{
  int numCLOOK = 0;
  int isRight = START - 100 >= 0;
  // allocate 2 more int slots in case we need to jump (record 199 and 0)
  int *requestCLOOK = malloc(sizeof(request) + 2*sizeof(int));

  // keep index of first element to the right of element
  // default value: no elements to right
  int firstRightIndex = numRequest;
  
  int hasJump = 0;

  // sort all elements in increasing order
  qsort(request, numRequest, sizeof(int), cmpfunc);

  // find index of first element to the right of start position
  int i = 0;
  while (i < numRequest) {
    if (request[i] >= START) {
      firstRightIndex = i;
      // if starting right and we have items on left of start
      // or if starting left and we have items on right of start
      // indicate that we have to make a jump
      if ((isRight && firstRightIndex > 0) || (!isRight && firstRightIndex < numRequest)) {
        hasJump = 1;
      }
      break;
    }
    i++;
  }

  if (isRight) {
    // scan through all elements from centre to right
    for (i = firstRightIndex; i < numRequest; i++) {
      requestCLOOK[numCLOOK] = request[i];
      numCLOOK++;
    }

    // if jump, we need to record the jump from extremities
    if (hasJump) {
      requestCLOOK[numCLOOK] = 0;
      numCLOOK++;
    }

    // scan through all elements from left to centre
    for (i = 0; i < firstRightIndex; i++) {
      requestCLOOK[numCLOOK] = request[i];
      numCLOOK++;
    }
  } else {
    // scan through all elements from centre to left
    for (i = firstRightIndex - 1; i >= 0; i--) {
      requestCLOOK[numCLOOK] = request[i];
      numCLOOK++;
    }

    // if jump, we need to record the jump from extremities
    if (hasJump) {
      requestCLOOK[numCLOOK] = 199;
      numCLOOK++;
    }

    // scan through all elements from right to start
    for (i = numRequest - 1; i >= firstRightIndex; i--) {
      requestCLOOK[numCLOOK] = request[i];
      numCLOOK++;
    }
  }

  printf("\n----------------\n");
  printf("CLOOK :");
  printSeqNPerformance(requestCLOOK, numCLOOK);
  printf("----------------\n");
  return;
}

int main()
{
  int *request, numRequest, i, ans;

  //allocate memory to store requests
  printf("Enter the number of disk access requests : ");
  scanf("%d", &numRequest);
  request = malloc(numRequest * sizeof(int));

  printf("Enter the requests ranging between %d and %d\n", LOW, HIGH);
  for (i = 0; i < numRequest; i++)
  {
    scanf("%d", &request[i]);
  }

  printf("\nSelect the policy : \n");
  printf("----------------\n");
  printf("1\t FCFS\n");
  printf("2\t SSTF\n");
  printf("3\t SCAN\n");
  printf("4\t CSCAN\n");
  printf("5\t LOOK\n");
  printf("6\t CLOOK\n");
  printf("----------------\n");
  scanf("%d", &ans);

  switch (ans)
  {
  //access the disk location in FCFS
  case 1:
    accessFCFS(request, numRequest);
    break;

  //access the disk location in SSTF
  case 2:
    accessSSTF(request, numRequest);
    break;

    //access the disk location in SCAN
  case 3:
    accessSCAN(request, numRequest);
    break;

    //access the disk location in CSCAN
  case 4:
    accessCSCAN(request, numRequest);
    break;

  //access the disk location in LOOK
  case 5:
    accessLOOK(request, numRequest);
    break;

  //access the disk location in CLOOK
  case 6:
    accessCLOOK(request, numRequest);
    break;

  default:
    break;
  }
  return 0;
}