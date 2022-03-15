/*
BENCHMARK RESULTS:
Parameters (n, max, s, mult)

(2000, 10000, 10, 100000) = 702 seconds
(2000, 10000, 5000, 100000) = 730 seconds
(2000, 10000, 9000, 100000) =  728 seconds

*/


#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include <time.h>


// functions
// allocatearray(): returns pointer to an array of size n, containing intgerers over [0, max]
int* allocatearray(int n, int max);

// median(): return the median value of an array, given an array and its size
int median(int n, int* array);

// mediansort(): sorts array by asscending order
void  mediansort(int n, int *cycle,int* array, int* sorted);

// search(): search for a given value, i, in a sorted array, array[]
// returns the index of its first occurence where the index of the first value is 0. 
// returns -1 if i is not in the array
int search(int* array, int n, int i);

// benchmark_native(): perform benchmarking, i.e. timing functions given a set of parameters
double benchmark_naive(int n, int max, int s,int mult);


// main function
int main() {
   
   //input n - size of array
   int n;
   printf("Enter the number of elements in the array: ");
   scanf("%d", &n);
   
   // input max - max value of element in the array
   int max;
   printf("Enter the maximum value of an elements in the array: ");
   scanf("%d", &max);
   
   // declare array pointer, array_main, and call allocatearray() assigning it to array_main
   // print each element of the array
   int* array_main; 
   array_main = allocatearray(n, max);
   int i = 0;
  
   printf("\nRandom Array:\n");
   for (i = 0; i< n-1; i++) {
   	printf("%d, ",array_main[i]);
   } 
   printf("%d\n",array_main[n-1]);

   // declaring sorted array
   int*  array_s;
   array_s = (int*) malloc(n * sizeof(int));
   
   // declaring cycle (pointer)
   int cycle = 0;
   int* cycle_s;
   cycle_s = &cycle;
   
   // calling mediansort and printing sorted[]
   mediansort(n, cycle_s, array_main, array_s);
   printf("\nSorted Array:\n");
   for (i = 0; i< n-1; i++) {
       printf("%d, " ,array_s[i]);
   } 
   printf("%d\n\n",array_s[n-1]);
  

   /* Benchmarking commented out
   This was the code I used to generate the figure above
   //benchmarking
   int s=9000;
   int mult = 100000;
   double result;
   result = benchmark_naive(n, max, s, mult);
   printf("\n\n");
   printf("Time elasped = %.0lf seconds\n\n", result);
   */
   return 0;

}

// benchmark_native(): perform benchmarking, i.e. timing functions given a set of parameters
double benchmark_naive(int n, int max, int s,int mult){
 
   // loop parameters
   int i, j;

   // declare time parameters
   time_t time_start, time_end;
  
   // declare pointer to generated array
   int *array_main;
   
   // declare sorted array, cycle and output
   int *array_s;
   array_s = (int*) malloc(n * sizeof(int));
   int cycle_t = 0;
   int *cycle_s_t;
   cycle_s_t = &cycle_t;

   //seach output
   int search_i;
   
   // start timer
   time(&time_start);

   // loop over mult
   for (i = 1 ; i <= mult ; i ++){
   	
	// test to flag every 1000 cycles
	if (i%1000 != 0){
		continue;
	}
 	else{
		// generate array 
		array_main = allocatearray(n, max);	
		
		//sort array
		mediansort(n, cycle_s_t, array_main, array_s);
		
		//search for s		
		search_i = search(array_s, n, s);
		
		// reset median_sort array and cycle pointer
		*cycle_s_t = 0;
		for (j = 0; j< n; j++) {
                	array_s[j]= 0;
                }
	}
   // dont code here
   }

   // stop timer
   time(&time_end);

   // deallocate the array
   free(array_main);
   array_main = NULL;

   // calculate and return the elapsed time in seconds
   time_t elapsed = time_end - time_start;
   return elapsed;
}


//search() search for a given value in a sorted array and returns the index of its first occurence where the index value of the first value is 0.
//returns -1 if it is not in the array
int search(int *array, int n, int i){

  // declare flag for when the value of array equals the searched value and loop index variable, j
  int index_loc =  0;
  int j;  
  
  // loop over every value of array and logical test if its equal to searched value, i 
  // if reached the flag variable is triggered and the loop is broken
  // if array value is greater than i the loop is also broken as the array is ordered
  for (j = 0; j< n; j++) {
	if (array[j] < i){
		continue;	
	}
	else if (array[j] == i){
                index_loc = 1;
		break;
        } 
  	else if (array[j] > i){
		break;
	}
  } 
  
  // if flag is been altered then return index value, otherwise return -1
  if (index_loc == 1){
	return j;
  } else {
	return -1;
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*

MEDIANSORT ALGORITHM DESCRIPTION:

array[] and sorted[] refer to the inputted and sorted arrays.
This method assumes there are less than n values of 0 in array[]  
If array[] contains n '0' values it will fail but given the scope of the assignment this is an negligible scenario
It uses the property that for any inputted array using the given method, the last values of sorted[] to be filled is sorted[n-1]

This section will only be called if sorted[n-1] = 0 i.e. final value in sorted[] = 0
The median of the cycle is calculated ( initial cycle = 0)
The median is placed in the correct loation of sorted[]
The median is removed from the array and the median for the cycle (i.e. minus previous median) is calculated
The location in sorted[] to place each incremental median is based off whether n is odd or even and whether the cycle is odd or even
For n = odd array incremaental medians are placed at n/2, n/2 - 1 , n/2 + 1 , n/2 - 2, n/2 + 2 etc.
For n = even array incremaental medians are placed at (n/2 - 1), (n/2 - 1) + 1 , (n/2 - 1) - 1 , (n/2 - 1) + 2, (n/2 - 1) -  2 etc.
mediansort() is called until the the last value of sorted != 0

*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// mediansort: recursive sorting algorithm using the medium of an array
void  mediansort(int n, int *cycle,int* array,int* sorted) {
   int loc;
   int med;
   int i=0; 
   
   // flag if the array contains an odd or even number of elements
   // used to decide where to place each incremental median in sorted
   int n_even, n_2;
   if (n % 2 == 0){
        n_even = -1;
	n_2 = (n/2 - 1);
   } else {
        n_even = 1;
	n_2 = n/2;
   }  

   // cycle 1 to cycle n-1
   if (sorted[n-1] == 0) {	
	
	//calculate median of the array
	med = median(n- *cycle, array);
	
	// loc = index in sorted[] to place new median
	// follows distinct patterns for odd and even cycles and n
	
	if (*cycle % 2 == 0){
                loc = n_2  + ( (*cycle/2) * n_even);
        } else {
                loc = n_2  -  ( ( (*cycle+1)/2 ) * n_even);
        }

        // store median in sorted[]
        sorted[loc] = med;

	// increment cycle +1
	*cycle = *cycle + 1;
	
	// remove previous median from array[]
	// set median value equal to final value in array[]
	// use realloc to then remove final value from array[]
	for (i = 0; i < n-*cycle; i++){
                if (array[i] == med) {
                        array[i] = array[n-*cycle];
                        break;
        }
	array  = (int*) realloc(array ,(n- 1)*sizeof(int));	 
	}

	// call mediansort() until n-1 index of sorted is filled
	mediansort(n, cycle, array, sorted);
   }
}
// allocatearray(): returns pointer to an array of size n, containing intgerers over [0, max]
int* allocatearray(int n, int max) {
   
   // declare pointer and array it is pointing to
   // dynamically assign memory to create an array of size n (integers)
   int* array;
   array = (int*) malloc(n * sizeof(int));
   
   // set seed for generating random numbers
   // NOTE: commented out for benchmarking keeps generating same array otherwise
   //srand(time(0));
   
   //for loop which assigns array[i] a random value from 0 to max
   int i;
   for (i = 0; i< n; i++) {  
   	array[i] = (rand() % (max + 1));
   	}
   
   //return pointer
   return array;
}


// median(): return the median value of an array, given an array and its size
int median(int n, int* array) {
   if (n == 1){
	return array[0];
   }
   // declare count values and loop indexes
   int count_smaller = 0, count_bigger=0 ,count_equal = 0;
   int i=0, j=0;
   
   // flag that is set to -1 if n is even
   // required to get the lower of two middle values for n-even arrays
   int n_even;
   if (n % 2 == 0){
	n_even = 1;
   } else {
	n_even = 0;
   }
 
   // loop over i -> n for each value of array
   // loop over j -> n for every other value of array (i = j is skipped)
   // +1 is added to appropriate count for each value of j
   // exit j-loop if array[i] is NOT the median - i.e. more than half the values are distinctly smaller or bigger than array[i]
   // return array[i] if it satisfies median conditions i.e. n/2 <= bigger + equal and n/2 <= smaller + equal
   // if not median, reset count values to 0 and begin for next iteration of i 
   for (i=0 ; i<n ; i++){   
	for (j=0; j<n; j++) {
		if (i == j) {
			continue;
		} 
		if (array[i] == array[j]) {
			count_equal ++;
		} else if (array[i] > array[j]) {
			count_smaller ++;
		} else {
			count_bigger ++;
		}
		if (count_smaller > n/2 || count_bigger > n/2) {
			break;
		}	
		if ((count_bigger + count_equal  >=  (n/2)) &&  (((n/2) - n_even) <= count_smaller + count_equal)){
			return array[i];
        	}

	}
	count_bigger = 0;
        count_smaller = 0;
        count_equal = 0;
   }
   
   // return 0 will flag if function is not working except in the unlikely scenario the array is only 0 
   return 0;
}
