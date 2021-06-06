#include <string.h>
#include <malloc.h> 
#include <memory.h>
#include <iostream>

int main() {
	int arr1[2];
	int arr2[4];

	int x = 10;
	scanf("%i", &x);
	void *(*funcPtr)(void *__restrict__ __dest, const void *__restrict__ __src, size_t __n) = memcpy;
	(*funcPtr)(arr1, arr2, x);
	if (x >= 0 && x < 8) {
		//memcpy(arr1, arr2, x);
	}

	scanf("%i", &x);
	memcpy(arr1, arr2, x);
	
    return 0;
}
