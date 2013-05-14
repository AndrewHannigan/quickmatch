using namespace std;

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <limits.h>

#define swap(x, y, type) \
  {						\
  type t = x;					\
  x = y;					\
  y = t;					\
  }

void init_array(int *A, int n) {
  for (int i = n, j = 1; i > 0; i--, j++)
    A[i] = j;
}

void dump_array(string msg, int *A, int n) {
#ifdef DUMP
  cout << msg << endl;
  for (int i = 1; i <= n; i++)
    cout << A[i] << " ";
  cout << endl;
#endif
}

void selection_sort(int *A, int n) {
  for (int i = 1; i < n; i++) {
    int smallest = i;

    for (int j = i+1; j <= n; j++) {
      if (A[j] < A[smallest])
	smallest = j;
    }

    swap(A[i], A[smallest], int);
  }
}

void insertion_sort(int *A, int n) {
  for (int i = 2; i <= n; i++) {
    int key = A[i];
    int j = i-1;

    while (j > 0 && A[j] > key) {
      A[j+1] = A[j];
      j--;
    }

    A[j+1] = key;
  }
}

// int c;

void merge(int *A, int p, int q, int r) {
//   cout << c << ": Merge(A, " << p << ", " << q << ", " << r << ")" << endl;
//   c++;
  int m = q - p + 1;
  int n = r - q;
  int *L = new int [m+2];
  int *R = new int [n+2];
  int i, j, k;

  memcpy(L+1, A+p, m * sizeof(int));
  memcpy(R+1, A+(q+1), n * sizeof(int));
  L[m+1] = R[n+1] = INT_MAX;

  for (i = 1, j = 1, k = p; k <= r; ) {
    if (L[i] < R[j])
      A[k++] = L[i++];
    else
      A[k++] = R[j++];
  }

  delete [] L;
  delete [] R;
}

void merge_sort(int *A, int p, int r) {
//   cout <<  c << ": Merge-Sort(A, " << p << ", " << r << ")" << endl;
//   c++;
  if (p < r) {
    int q = (p + r) / 2;
    merge_sort(A, p, q);
    merge_sort(A, q+1, r);
    merge(A, p, q, r);
  }
}

int rand_partition(int *A, int p, int r) {
  int s = p + (random() % (r - p + 1));
  swap(A[r], A[s], int);
  int pivot = A[r];
  int i = p - 1;

  for (int j = p; j < r; j++) {
    if (A[j] <= pivot) {
      i++;
      swap(A[i], A[j], int);
    }
  }

  swap(A[i+1], A[r], int);

  return i+1;
}

void rand_quicksort(int *A, int p, int r) {
  if (p < r) {
    int q = rand_partition(A, p, r);
    rand_quicksort(A, p, q-1);
    rand_quicksort(A, q+1, r);
  }
}

int threshhold;

void insertion_sort(int *A, int p, int r) {
  for (int i = p+1; i <= r; i++) {
    int key = A[i];
    int j = i-1;

    while (j >= p && A[j] > key) {
      A[j+1] = A[j];
      j--;
    }

    A[j+1] = key;
  }
}

void hybrid_quicksort(int *A, int p, int r) {
  if (p < r) {
    if (r - p + 1 <= threshhold)
      insertion_sort(A, p, r);
    else {
      int q = rand_partition(A, p, r);
      hybrid_quicksort(A, p, q-1);
      hybrid_quicksort(A, q+1, r);
    }
  }
}

int det_partition(int *A, int p, int r) {
//   int s = p + (random() % (r - p + 1));
//   swap(A[r], A[s], int);
  int pivot = A[r];
  int i = p - 1;

  for (int j = p; j < r; j++) {
    if (A[j] <= pivot) {
      i++;
      swap(A[i], A[j], int);
    }
  }

  swap(A[i+1], A[r], int);

  return i+1;
}

void det_quicksort(int *A, int p, int r) {
  if (p < r) {
    int q = det_partition(A, p, r);
    det_quicksort(A, p, q-1);
    det_quicksort(A, q+1, r);
  }
}

bool check_result(int *A, int n) {
  bool OK = true;

  for (int i = 1; i <= n; i++) {
    if (A[i] != i) {
      OK = false;
      fprintf(stderr, "Error: A[%d] = %d\n", i, A[i]);
      return OK;
    }
  }

  return OK;
}

#define START_TIMER \
  cycles_high1 = 0; cycles_low1 = 0; cycles_high2 = 0; cycles_low2 = 0; \
  temp_cycles1 = 0, temp_cycles2 = 0, total_cycles = 0; \
  __asm__ __volatile__ ("rdtsc" : "=a"(cycles_low1), "=d"(cycles_high1));

#define STOP_TIMER \
  __asm__ __volatile__ ("rdtsc" : "=a"(cycles_low2), "=d"(cycles_high2)); \
  temp_cycles1 = ((unsigned long long) cycles_high1 << 32) | cycles_low1; \
  temp_cycles2 = ((unsigned long long) cycles_high2 << 32) | cycles_low2; \
  total_cycles = temp_cycles2 - temp_cycles1; \
  seconds = double (total_cycles) / double (MHz*1000000);

int main(int argc, char **argv) {
  int *sizes;
  int trials;
  int count;

  if (argc > 2 && strcmp(argv[1], "-l") == 0) {
    trials = atoi(argv[2]);
    count = argc - 3;

    // Get size list from command line.
    sizes = new int [argc-3];
    for (int i = 0; i < count; i++)
      sizes[i] = atoi(argv[i+3]);
  }
  else if (argc == 5) {
    // Get trials, start, increment, limit.
    trials = atoi(argv[1]);
    int start = atoi(argv[2]);
    int increment = atoi(argv[3]);
    int limit = atoi(argv[4]);
    count = (limit - start) / increment + 1;
    sizes = new int [count];
    for (int i = 0; i < count; i++)
      sizes[i] = start + (i * increment);
  }
  else {
    fprintf(stderr, "Usage error\n");
    exit(1);
  }

  printf("Element size is %ld\n\n", sizeof(int));

  int *A;
  int n;

  unsigned cycles_high1, cycles_low1, cycles_high2, cycles_low2;
  unsigned long long temp_cycles1, temp_cycles2;
  unsigned long long total_cycles;
  double seconds;
  unsigned MHz = 2400; 	   // My Mac is a 2.4 GHz machine, so 2400 MHz
  double cumulative_seconds;

  printf("n,");
  for (int i = 0; i < count; i++) {
    n = sizes[i];
    printf("%d", n);
    if (i < count-1)
      printf(",");
  }
  printf("\n");

  printf("Selection sort,");
  for (int i = 0; i < count; i++) {
    n = sizes[i];
    cumulative_seconds = 0.0;
    A = new int [n+1];
    init_array(A, n);
    selection_sort(A, n);
    check_result(A, n);
    for (int trial = 0; trial < trials; trial++) {
      init_array(A, n);
      START_TIMER;
      selection_sort(A, n);
      STOP_TIMER;
      check_result(A, n);
      cumulative_seconds += seconds;
    }

    double average_seconds = cumulative_seconds / trials;
    printf("%.8f", average_seconds);
    if (i < count-1)
      printf(",");
    fflush(NULL);
    delete [] A;
  }
  printf("\n");
  fflush(NULL);

  printf("Insertion sort,");
  for (int i = 0; i < count; i++) {
    n = sizes[i];
    cumulative_seconds = 0.0;
    A = new int [n+1];
    init_array(A, n);
    insertion_sort(A, n);
    check_result(A, n);
    for (int trial = 0; trial < trials; trial++) {
      init_array(A, n);
      START_TIMER;
      insertion_sort(A, n);
      STOP_TIMER;
      check_result(A, n);
      cumulative_seconds += seconds;
    }

    double average_seconds = cumulative_seconds / trials;
    printf("%.8f", average_seconds);
    if (i < count-1)
      printf(",");
    fflush(NULL);
    delete [] A;
  }
  printf("\n");
  fflush(NULL);
  
  printf("Merge sort,");
  for (int i = 0; i < count; i++) {
    n = sizes[i];
    cumulative_seconds = 0.0;
    A = new int [n+1];
    init_array(A, n);
    merge_sort(A, 1, n);
    check_result(A, n);
    for (int trial = 0; trial < trials; trial++) {
      init_array(A, n);
      START_TIMER;
      merge_sort(A, 1, n);
      STOP_TIMER;
      check_result(A, n);
      cumulative_seconds += seconds;
    }

    double average_seconds = cumulative_seconds / trials;
    printf("%.8f", average_seconds);
    if (i < count-1)
      printf(",");
    fflush(NULL);
    delete [] A;
  }
  printf("\n");
  fflush(NULL);
  
  printf("Deterministic quicksort,");
  for (int i = 0; i < count; i++) {
    n = sizes[i];
    cumulative_seconds = 0.0;
    A = new int [n+1];
    init_array(A, n);
    det_quicksort(A, 1, n);
    check_result(A, n);
    for (int trial = 0; trial < trials; trial++) {
      init_array(A, n);
      START_TIMER;
      det_quicksort(A, 1, n);
      STOP_TIMER;
      check_result(A, n);
      cumulative_seconds += seconds;
    }

    double average_seconds = cumulative_seconds / trials;
    printf("%.8f", average_seconds);
    if (i < count-1)
      printf(",");
    fflush(NULL);
    delete [] A;
  }
  printf("\n");
  fflush(NULL);
  
  printf("Randomized quicksort,");
  for (int i = 0; i < count; i++) {
    n = sizes[i];
    cumulative_seconds = 0.0;
    A = new int [n+1];
    init_array(A, n);
    rand_quicksort(A, 1, n);
    check_result(A, n);
    for (int trial = 0; trial < trials; trial++) {
      init_array(A, n);
      START_TIMER;
      rand_quicksort(A, 1, n);
      STOP_TIMER;
      check_result(A, n);
      cumulative_seconds += seconds;
    }

    double average_seconds = cumulative_seconds / trials;
    printf("%.8f", average_seconds);
    if (i < count-1)
      printf(",");
    fflush(NULL);
    delete [] A;
  }
  printf("\n");
  fflush(NULL);

  printf("Hybrid quicksort,");
  //  for (threshhold = 2; threshhold <= 500; threshhold++) {
  //    cout << threshhold << ",";
  threshhold = 22;
    for (int i = 0; i < count; i++) {
      n = sizes[i];
      cumulative_seconds = 0.0;
      A = new int [n+1];
      init_array(A, n);
      hybrid_quicksort(A, 1, n);
      check_result(A, n);
      for (int trial = 0; trial < trials; trial++) {
	init_array(A, n);
	START_TIMER;
	hybrid_quicksort(A, 1, n);
	STOP_TIMER;
	check_result(A, n);
	cumulative_seconds += seconds;
      }

      double average_seconds = cumulative_seconds / trials;
      printf("%.8f", average_seconds);
      if (i < count-1)
	printf(",");
      fflush(NULL);
      delete [] A;
    }
    printf("\n");
    fflush(NULL);
    //  }

  return 0;
}

