#ifndef _ALGORITHMS_H
#define _ALGORITHMS_H

#include <stdbool.h>
#include <stddef.h>
bool bubbleSort_int(int numbers[], int array_size);
void *bubbleThread_int(void *arg);

bool insertionSort_int(int v[], int n);
void *insertionThread_int(void *arg);

bool selectionSort_int(int num[], int tam);
void *selectionThread_int(void *arg);

bool mergeSort_int(int *input, int size);
void *mergeThread_int(void *arg);

bool heapSort_int(int numbers[], int array_size);
void *heapThread_int(void *arg);

bool quickSort_int(int *data, int N);
void *quickThread_int(void *arg);

//void radixSort_uint(unsigned int *a, size_t size, int bits);
bool radixSort_int(unsigned int *a, size_t size);
void *radixThread_int(void *arg);

bool bucketSort_int(int array[], int n);
void *bucketThread_int(void *arg);
#endif
