#ifndef _ALGORITHMS_H
#define _ALGORITHMS_H

#include <stddef.h>
bool bubbleSort(int numbers[], int array_size);
void *bubbleThread(void *arg);

bool insertionSort(int v[], int n);
void *insertionThread(void *arg);

bool selectionSort(int num[], int tam);
void *selectionThread(void *arg);

bool mergeSort(int *input, int size);
void *mergeThread(void *arg);

bool heapSort(int numbers[], int array_size);
void *heapThread(void *arg);

bool quickSort(int *data, int N);
void *quickThread(void *arg);

//void radixSort_uint(unsigned int *a, size_t size, int bits);
void radixSort(unsigned int *a, size_t size);
void *radixThread(void *arg);

void bucketSort(int array[], int n);
void *bucketThread(void *arg);
#endif
