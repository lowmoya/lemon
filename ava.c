#include <stddef.h>
#include <stdlib.h>

int
comp_func(int * a, int * b)
{
	return *a - *b;
}

void
sort_array(size_t n, int arr[n])
{
	int * nums = calloc(n, sizeof(int)), count = 0;
	for (size_t i = 0; i < n; i++)
		if (arr[i] & 1)
			nums[count++] = arr[i];
	qsort(nums, count, sizeof(int), (int(*)(void *, void *))comp_func);
	count = 0;
	for (size_t i = 0; i < n; i++)
		if (arr[i] & 1)
			arr[i] = nums[count++];
	free(nums);
}
