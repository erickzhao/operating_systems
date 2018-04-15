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
  printSeqNPerformance(request, numRequest);
  printf("----------------\n");
  return;
}

//access the disk location in SCAN
void accessSCAN(int *request, int numRequest)
{

  //write your logic here
  printf("\n----------------\n");
  printf("SCAN :");
  // printSeqNPerformance(newRequest, newCnt);
  printf("----------------\n");
  return;
}

//access the disk location in CSCAN
void accessCSCAN(int *request, int numRequest)
{
  //write your logic here
  printf("\n----------------\n");
  printf("CSCAN :");
  // printSeqNPerformance(newRequest, newCnt);
  printf("----------------\n");
  return;
}

//access the disk location in LOOK
void accessLOOK(int *request, int numRequest)
{
  //write your logic here
  printf("\n----------------\n");
  printf("LOOK :");
  // printSeqNPerformance(newRequest, newCnt);
  printf("----------------\n");
  return;
}

//access the disk location in CLOOK
void accessCLOOK(int *request, int numRequest)
{
  //write your logic here
  printf("\n----------------\n");
  printf("CLOOK :");
  // printSeqNPerformance(newRequest,newCnt);
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