#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include <pthread.h>
#include "threadUtils.h"
#include "algorithms.h"

#include <stdio.h>

#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)

//
//		oooooooooo.               .o8        .o8       oooo             .oooooo..o                        .
//		`888'   `Y8b             "888       "888       `888            d8P'    `Y8                      .o8
//		 888     888 oooo  oooo   888oooo.   888oooo.   888   .ooooo.  Y88bo.       .ooooo.  oooo d8b .o888oo
//		 888oooo888' `888  `888   d88' `88b  d88' `88b  888  d88' `88b  `"Y8888o.  d88' `88b `888""8P   888
//		 888    `88b  888   888   888   888  888   888  888  888ooo888      `"Y88b 888   888  888       888
//		 888    .88P  888   888   888   888  888   888  888  888    .o oo     .d8P 888   888  888       888 .
//		o888bood8P'   `V88V"V8P'  `Y8bod8P'  `Y8bod8P' o888o `Y8bod8P' 8""88888P'  `Y8bod8P' d888b      "888"
//

//http://www.algorithmist.com/index.php/Bubble_sort.c
bool bubbleSort_int(int numbers[], int array_size) {
	int i, j, temp;
	bool success;
	if(numbers != NULL && array_size > 0) {
		for (i = (array_size - 1); i > 0; i--) {
			for (j = 1; j <= i; j++) {
				if (numbers[j-1] > numbers[j]) {
					temp = numbers[j-1];
					numbers[j-1] = numbers[j];
					numbers[j] = temp;
				}
			}
		}
		success = true;
	} else {
		success = false;
	}
	return success;
}

void *bubbleThread_int(void *arg) {
	/* */ SortingInfo info = (SortingInfo)arg;
	/* */ uint32_t start, end;
	/* */ start = *(getSortingClock(info));
	//Inicio
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(bubbleSort_int(*(getSortingDataArray(info)), getSortingSize(info)) == false) {
		return ((void *) EXIT_FAILURE);
	}
	//Fim
	/* */ end = *(getSortingClock(info));
	/* */ *(getSortingTime(info)) = end - start;
	return ((void *) EXIT_SUCCESS);
}

//
//		ooooo                                             .    o8o                         .oooooo..o                        .
//		`888'                                           .o8    `"'                        d8P'    `Y8                      .o8
//		 888  ooo. .oo.    .oooo.o  .ooooo.  oooo d8b .o888oo oooo   .ooooo.  ooo. .oo.   Y88bo.       .ooooo.  oooo d8b .o888oo
//		 888  `888P"Y88b  d88   "8 d88' `88b `888""8P   888   `888  d88' `88b `888P"Y88b   `"Y8888o.  d88' `88b `888""8P   888
//		 888   888   888  `"Y88b.  888ooo888  888       888    888  888   888  888   888       `"Y88b 888   888  888       888
//		 888   888   888  o.   88b 888    .o  888       888 .  888  888   888  888   888  oo     .d8P 888   888  888       888 .
//		o888o o888o o888o 8""888P' `Y8bod8P' d888b      "888" o888o `Y8bod8P' o888o o888o 8""88888P'  `Y8bod8P' d888b      "888"
//

//http://pt.wikipedia.org/wiki/Insertion_sort#C
bool insertionSort_int(int v[], int n) {
	int i, j, chave;
	bool success;
	if(v != NULL && n > 0) {
		for(j=1; j<n; j++) {
			chave = v[j];
			i = j-1;
			while(i >= 0 && v[i] > chave) {
				v[i+1] = v[i];
				i--;
			}
			v[i+1] = chave;
		}
		success = true;
	} else {
		success = false;
	}
	return success;
}

void *insertionThread_int(void *arg) {
	/* */ SortingInfo info = (SortingInfo)arg;
	/* */ uint32_t start, end;
	/* */ start = *(getSortingClock(info));
	//Inicio
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(insertionSort_int(*(getSortingDataArray(info)), getSortingSize(info)) == false) {
		return ((void *) EXIT_FAILURE);
	}
	//Fim
	/* */ end = *(getSortingClock(info));
	/* */ *(getSortingTime(info)) = end - start;
	return ((void *) EXIT_SUCCESS);
}

//
//		 .oooooo..o           oooo                          .    o8o                         .oooooo..o                        .
//		d8P'    `Y8           `888                        .o8    `"'                        d8P'    `Y8                      .o8
//		Y88bo.       .ooooo.   888   .ooooo.   .ooooo.  .o888oo oooo   .ooooo.  ooo. .oo.   Y88bo.       .ooooo.  oooo d8b .o888oo
//		 `"Y8888o.  d88' `88b  888  d88' `88b d88' `"Y8   888   `888  d88' `88b `888P"Y88b   `"Y8888o.  d88' `88b `888""8P   888
//		     `"Y88b 888ooo888  888  888ooo888 888         888    888  888   888  888   888       `"Y88b 888   888  888       888
//		oo     .d8P 888    .o  888  888    .o 888   .o8   888 .  888  888   888  888   888  oo     .d8P 888   888  888       888 .
//		8""88888P'  `Y8bod8P' o888o `Y8bod8P' `Y8bod8P'   "888" o888o `Y8bod8P' o888o o888o 8""88888P'  `Y8bod8P' d888b      "888"
//

//http://pt.wikipedia.org/wiki/Selection_sort#C
bool selectionSort_int(int num[], int tam) {
	int i, j, min;
	bool success;
	if(num != NULL && tam > 0) {
		for(i = 0; i < (tam-1); i++) {
			min = i;
			for(j = (i+1); j < tam; j++) {
				if(num[j] < num[min]) {
					min = j;
				}
			}
			if (i != min) {
				int swap = num[i];
				num[i] = num[min];
				num[min] = swap;
			}
		}
		success = true;
	} else {
		success = false;
	}
	return success;
}

void *selectionThread_int(void *arg) {
	/* */ SortingInfo info = (SortingInfo)arg;
	/* */ uint32_t start, end;
	/* */ start = *(getSortingClock(info));
	//Inicio
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(selectionSort_int(*(getSortingDataArray(info)), getSortingSize(info)) == false) {
		return ((void *) EXIT_FAILURE);
	}
	//Fim
	/* */ end = *(getSortingClock(info));
	/* */ *(getSortingTime(info)) = end - start;
	return ((void *) EXIT_SUCCESS);
}

//
//		ooo        ooooo                                          .oooooo..o                        .
//		`88.       .888'                                         d8P'    `Y8                      .o8
//		 888b     d'888   .ooooo.  oooo d8b  .oooooooo  .ooooo.  Y88bo.       .ooooo.  oooo d8b .o888oo
//		 8 Y88. .P  888  d88' `88b `888""8P 888' `88b  d88' `88b  `"Y8888o.  d88' `88b `888""8P   888
//		 8  `888'   888  888ooo888  888     888   888  888ooo888      `"Y88b 888   888  888       888
//		 8    Y     888  888    .o  888     `88bod8P'  888    .o oo     .d8P 888   888  888       888 .
//		o8o        o888o `Y8bod8P' d888b    `8oooooo.  `Y8bod8P' 8""88888P'  `Y8bod8P' d888b      "888"
//		                                    d"     YD
//		                                    "Y88888P'
//

//http://www.cprogramming.com/tutorial/computersciencetheory/merge.html
/* Helper function for finding the min of two numbers */
inline int min_int(int a, int b)
{
  return (a < b) ? a : b;
}

/* left is the index of the leftmost element of the subarray; right is one
 * past the index of the rightmost element */
void mergeHelper_int(int *input, int left, int right, int *scratch)
{
    /* base case: one element */
    if(right == left + 1) {
        return;
    } else {
        int i = 0;
        int length = right - left;
        int midpoint_distance = length/2;
        /* l and r are to the positions in the left and right subarrays */
        int l = left, r = left + midpoint_distance;

        /* sort each subarray */
        mergeHelper_int(input, left, left + midpoint_distance, scratch);
        mergeHelper_int(input, left + midpoint_distance, right, scratch);

        /* merge the arrays together using scratch for temporary storage */ 
        for(i = 0; i < length; i++)
        {
            /* Check to see if any elements remain in the left array; if so,
             * we check if there are any elements left in the right array; if
             * so, we compare them.  Otherwise, we know that the merge must
             * use take the element from the left array */
            if(l < left + midpoint_distance && 
                    (r == right || min_int(input[l], input[r]) == input[l]))
            {
                scratch[i] = input[l];
                l++;
            }
            else
            {
                scratch[i] = input[r];
                r++;
            }
        }
        /* Copy the sorted subarray back to the input */
        for(i = left; i < right; i++)
        {
            input[i] = scratch[i - left];
        }
    }
}

/* mergesort returns true on success.  Note that in C++, you could also
 * replace malloc with new and if memory allocation fails, an exception will
 * be thrown.  If we don't allocate a scratch array here, what happens? 
 *
 * Elements are sorted in reverse order -- greatest to least */

bool mergeSort_int(int *input, int size) {
	bool success;
    int *scratch = (int *)malloc(size * sizeof(int));
    if(scratch != NULL && size > 0) {
        mergeHelper_int(input, 0, size, scratch);
        free(scratch);
        success = true;
    } else {
    	success = false;
    }
    return success;
}

void *mergeThread_int(void *arg) {
	/* */ SortingInfo info = (SortingInfo)arg;
	/* */ uint32_t start, end;
	/* */ start = *(getSortingClock(info));
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	//Inicio
	if(mergeSort_int(*(getSortingDataArray(info)), getSortingSize(info)) == false) {
		return ((void *) EXIT_FAILURE);
	}
	//Fim
	/* */ end = *(getSortingClock(info));
	/* */ *(getSortingTime(info)) = end - start;
	return ((void *) EXIT_SUCCESS);
}

//
//		ooooo   ooooo                                 .oooooo..o                        .
//		`888'   `888'                                d8P'    `Y8                      .o8
//		 888     888   .ooooo.   .oooo.   oo.ooooo.  Y88bo.       .ooooo.  oooo d8b .o888oo
//		 888ooooo888  d88' `88b `P   88b   888' `88b  `"Y8888o.  d88' `88b `888""8P   888
//		 888     888  888ooo888  .oP"888   888   888      `"Y88b 888   888  888       888
//		 888     888  888    .o d8   888   888   888 oo     .d8P 888   888  888       888 .
//		o888o   o888o `Y8bod8P' `Y888""8o  888bod8P' 8""88888P'  `Y8bod8P' d888b      "888"
//		                                   888
//		                                  o888o
//

//http://www.algorithmist.com/index.php/Heap_sort.c
void siftDown_int(int numbers[], int root, int bottom) {
	int done, maxChild, temp;

	done = 0;
	while((root*2 <= bottom) && (!done)) {
		if(root*2 == bottom)
			maxChild = root * 2;
		else if(numbers[root * 2] > numbers[root * 2 + 1])
			maxChild = root * 2;
		else
			maxChild = root * 2 + 1;

		if (numbers[root] < numbers[maxChild]) {
			temp = numbers[root];
			numbers[root] = numbers[maxChild];
			numbers[maxChild] = temp;
			root = maxChild;
		}
		else
		done = 1;
	}
}

bool heapSort_int(int numbers[], int array_size) {
	int i, temp;
	bool success;
	if(numbers != NULL && array_size > 0) {
		for(i = (array_size / 2); i >= 0; i--)
			siftDown_int(numbers, i, array_size - 1);

		for(i = array_size-1; i >= 1; i--) {
			temp = numbers[0];
			numbers[0] = numbers[i];
			numbers[i] = temp;
			siftDown_int(numbers, 0, i-1);
		}
		success = true;
	} else {
		success = false;
	}
	return success;
}

void *heapThread_int(void *arg) {
	/* */ SortingInfo info = (SortingInfo)arg;
	/* */ uint32_t start, end;
	/* */ start = *(getSortingClock(info));
	//Inicio
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(heapSort_int(*(getSortingDataArray(info)), getSortingSize(info)) == false) {
		return ((void *) EXIT_FAILURE);
	}
	//Fim
	/* */ end = *(getSortingClock(info));
	/* */ *(getSortingTime(info)) = end - start;
	return ((void *) EXIT_SUCCESS);
}

//
//		  .oooooo.                 o8o            oooo         .oooooo..o                        .
//		 d8P'  `Y8b                `"'            `888        d8P'    `Y8                      .o8
//		888      888  oooo  oooo  oooo   .ooooo.   888  oooo  Y88bo.       .ooooo.  oooo d8b .o888oo
//		888      q88  `888  `888  `888  d88' `"Y8  888 .8P'    `"Y8888o.  d88' `88b `888""8P   888
//		888   d8  8b   888   888   888  888        888888.         `"Y88b 888   888  888       888
//		`88b   d888b   888   888   888  888   .o8  888 `88b.  oo     .d8P 888   888  888       888 .
//		 `Y8bood888'   `V88V"V8P' o888o `Y8bod8P' o888o o888o 8""88888P'  `Y8bod8P' d888b      "888"
//		         88b
//

//http://www.algorithmist.com/index.php/Quicksort.c 
void quickSortWorker_int(int *data, int N) {
	int i, j;
	int v, t;

	if(N <= 1)
		return;

	// Partition elements
	v = data[0];
	i = 0;
	j = N;
	for(;;) {
		while(data[++i] < v && i < N) { }
		while(data[--j] > v) { }
		if( i >= j )
		break;
		t = data[i];
		data[i] = data[j];
		data[j] = t;
	}
	t = data[i-1];
	data[i-1] = data[0];
	data[0] = t;
	quickSortWorker_int(data, i-1);
	quickSortWorker_int(data+i, N-i);
}

bool quickSort_int(int *data, int n) {
	bool success;
	if(data != NULL && n > 0) {
		quickSortWorker_int(data, n);
		success = true;
	} else {
		success = false;
	}
	return success;
}

void *quickThread_int(void *arg) {
	/* */ SortingInfo info = (SortingInfo)arg;
	/* */ uint32_t start, end;
	/* */ start = *(getSortingClock(info));
	//Inicio
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if(quickSort_int(*(getSortingDataArray(info)), getSortingSize(info)) == false) {
		return ((void *) EXIT_FAILURE);
	}
	//Fim
	/* */ end = *(getSortingClock(info));
	/* */ *(getSortingTime(info)) = end - start;
	return ((void *) EXIT_SUCCESS);
}

//
//		ooooooooo.                   .o8   o8o               .oooooo..o                        .
//		`888   `Y88.                "888   `"'              d8P'    `Y8                      .o8
//		 888   .d88'  .oooo.    .oooo888  oooo  oooo    ooo Y88bo.       .ooooo.  oooo d8b .o888oo
//		 888ooo88P'  `P   88b  d88' `888  `888   `88b..8P'   `"Y8888o.  d88' `88b `888""8P   888
//		 888`88b.     .oP"888  888   888   888     Y888'         `"Y88b 888   888  888       888
//		 888  `88b.  d8   888  888   888   888   .o8"'88b   oo     .d8P 888   888  888       888 .
//		o888o  o888o `Y888""8o `Y8bod88P" o888o o88'   888o 8""88888P'  `Y8bod8P' d888b      "888"
//

//http://en.literateprograms.org/Special:Downloadcode/Radix_sort_%28C%29
//void radixSort_int_uint(unsigned int *a, size_t size, int bits) {
bool radixSort_int(unsigned int *a, size_t size) {
	int bits = 4;
	unsigned int mask;
	unsigned int rshift=0u;
	unsigned int *p, *b, *b_orig;
	unsigned int i;
	unsigned int key;
	int cntsize;
	int *cntarray;
	bool success;
	if(a != NULL && size > 0) {
		b=b_orig=malloc(size*sizeof(unsigned int));
		cntsize=1u<<bits;
		printf("cntsize: %d\n", cntsize);
		cntarray=calloc(cntsize, sizeof(int));
		assert(cntarray);
		assert(b_orig);
		for(mask=~(UINT_MAX<<bits); mask; mask<<=bits, rshift+=bits) {
			for(p=a; p<a+size; ++p) {
				key=(*p & mask)>>rshift;
				++cntarray[key];
			}
			for(i=1; i<cntsize; ++i) cntarray[i]+=cntarray[i-1];
			for(p=a+size-1; p>=a; --p) {
				key=(*p & mask)>>rshift;
				b[cntarray[key]-1]=*p;
				--cntarray[key];
			}
			p=b; b=a; a=p;
			bzero(cntarray, cntsize * sizeof(int));
		}
		if(a==b_orig) memcpy(b, a, size*sizeof(unsigned int));
		free(b_orig);
		free(cntarray);
		success = true;
	} else {
		success = false;
	}
	return success;
}

void *radixThread_int(void *arg) {
	/* */ SortingInfo info = (SortingInfo)arg;
	/* */ uint32_t start, end;
	/* */ start = *(getSortingClock(info));
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	//Inicio
	if(radixSort_int(*((unsigned int **)getSortingDataArray(info)), (size_t)(getSortingSize(info))) == false) {
		return ((void *) EXIT_FAILURE);
	}
	//Fim
	/* */ end = *(getSortingClock(info));
	/* */ *(getSortingTime(info)) = end - start;
	return ((void *) EXIT_SUCCESS);
}

//
//		oooooooooo.                        oooo                      .    .oooooo..o                        .
//		`888'   `Y8b                       `888                    .o8   d8P'    `Y8                      .o8
//		 888     888 oooo  oooo   .ooooo.   888  oooo   .ooooo.  .o888oo Y88bo.       .ooooo.  oooo d8b .o888oo
//		 888oooo888' `888  `888  d88' `"Y8  888 .8P'   d88' `88b   888    `"Y8888o.  d88' `88b `888""8P   888
//		 888    `88b  888   888  888        888888.    888ooo888   888        `"Y88b 888   888  888       888
//		 888    .88P  888   888  888   .o8  888 `88b.  888    .o   888 . oo     .d8P 888   888  888       888 .
//		o888bood8P'   `V88V"V8P' `Y8bod8P' o888o o888o `Y8bod8P'   "888" 8""88888P'  `Y8bod8P' d888b      "888"
//

//http://en.literateprograms.org/Special:Downloadcode/Bucket_sort_%28C%29
bool bucketSort_int(int array[], int n) {
	int i, j;
	bool success;
	if(array != NULL && n > 0) {
		//int count[n]; //Gerava falha de segmentação para valores muito altos
		int *count = (int *) malloc(n * sizeof(int));
		for(i=0; i < n; i++) {
			count[i] = 0;
		}

		for(i=0; i < n; i++) {
			(count[array[i]])++;
		}

		for(i=0,j=0; i < n; i++) {
			for(; count[i]>0; (count[i])--) {
				array[j++] = i;
			}
		}
		success = true;
	} else {
		success = false;
	}
	return success;
}

void *bucketThread_int(void *arg) {
	/* */ SortingInfo info = (SortingInfo)arg;
	/* */ uint32_t start, end;
	/* */ start = *(getSortingClock(info));
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	//Inicio
	if(bucketSort_int(*(getSortingDataArray(info)), getSortingSize(info)) == false) {
		return ((void *) EXIT_FAILURE);
	}
	//Fim
	/* */ end = *(getSortingClock(info));
	/* */ *(getSortingTime(info)) = end - start;
	return ((void *) EXIT_SUCCESS);
}
