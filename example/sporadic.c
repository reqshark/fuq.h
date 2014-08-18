/**
 * Compiled using: -g -Wall -pthread -o sporadic -O3 sporadic.c
 */

#include "../fuq.h"
#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#define ITER 1e7

fuq_queue queue;
pthread_t thread;


static void* task_runner(void* arg) {
  for (size_t i = 1; i < ITER; i++) {
    fuq_push(&queue, (void*) i);
  }
  return NULL;
}


int main(void) {
  uint64_t sum = 0;
  uint64_t check = 0;
  void* tmp;

  fuq_init(&queue);
  assert(pthread_create(&thread, NULL, task_runner, NULL) == 0);

  for (size_t i = 1; i < ITER; i++) {
    check += i;
    if (NULL != (tmp = fuq_shift(&queue)))
      sum += (uint64_t) tmp;
  }

  fprintf(stderr, "check: %zi\n", check);
  fprintf(stderr, "sum:   %zi\n", sum);

  assert(pthread_join(thread, NULL) == 0);

  while (NULL != (tmp = fuq_shift(&queue)))
    sum += (uint64_t) tmp;

  fprintf(stderr, "sum:   %zi\n", sum);
  assert(sum == check);

  fuq_dispose(&queue);
}