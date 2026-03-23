#if !defined QUEUE_H
#define QUEUE_H 1

struct elem {
  int presses;
  int val;
};
typedef struct elem elem;

void alloc_queue(int);
void free_queue(void);
void enqueue(elem);
elem dequeue(void);
bool empty(void);

#endif
