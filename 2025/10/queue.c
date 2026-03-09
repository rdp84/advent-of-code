#include "day_10.h"
#include "queue.h"

static int   max, front, rear;
static elem *queue;
static bool *visited;

void alloc_queue(int nlights)
{
  int i;
  
  front = rear = -1;
  max = (int) pow(2, nlights);
  queue = malloc(max * sizeof(elem));
  visited = malloc(max * sizeof(bool));

  for (i = 0; i < max; i++)
    *(visited + i) = false;
}

void free_queue(void)
{
  free(queue);
  free(visited);
  queue = NULL;
  visited = NULL;
}

void enqueue(elem e)
{
  if (!visited[e.val]) {
    if (front == -1)
      front = 0;

    rear++;
    queue[rear] = e;
    visited[e.val] = true;
  }
}

elem dequeue(void)
{
  elem retVal;

  if (!empty()) {
    retVal = queue[front];
    front++;
    if (front > rear)
      front = rear = -1;
  }
  else
    retVal.presses = retVal.val = -1;

  return retVal;
}

bool empty(void)
{
  return front == -1;
}
