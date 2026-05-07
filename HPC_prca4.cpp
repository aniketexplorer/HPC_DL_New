// Write a program to implement performance enhancement of
// parallel Quicksort Algorithm using OpenMP.

#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Function to swap two elements
void swapElements(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}

// Partition function
int partition(int arr[], int low, int high)
{
    int pivot = arr[high]; // Last element as pivot
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swapElements(arr[i], arr[j]);
        }
    }

    swapElements(arr[i + 1], arr[high]);

    return (i + 1);
}

// Sequential QuickSort
void quickSortSequential(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        quickSortSequential(arr, low, pi - 1);
        quickSortSequential(arr, pi + 1, high);
    }
}

// Parallel QuickSort using OpenMP
void quickSortParallel(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                quickSortParallel(arr, low, pi - 1);
            }

            #pragma omp section
            {
                quickSortParallel(arr, pi + 1, high);
            }
        }
    }
}

// Function to print array
void printArray(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int main()
{
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    int arr1[n], arr2[n];

    cout << "Enter array elements:\n";

    for (int i = 0; i < n; i++)
    {
        cin >> arr1[i];
        arr2[i] = arr1[i];
    }

    // Sequential QuickSort Timing
    auto start1 = high_resolution_clock::now();

    quickSortSequential(arr1, 0, n - 1);

    auto stop1 = high_resolution_clock::now();

    auto duration1 =
        duration_cast<microseconds>(stop1 - start1);

    // Parallel QuickSort Timing
    auto start2 = high_resolution_clock::now();

    quickSortParallel(arr2, 0, n - 1);

    auto stop2 = high_resolution_clock::now();

    auto duration2 =
        duration_cast<microseconds>(stop2 - start2);

    // Output
    cout << "\nSorted Array using Sequential QuickSort:\n";
    printArray(arr1, n);

    cout << "\nSorted Array using Parallel QuickSort:\n";
    printArray(arr2, n);

    cout << "\nSequential QuickSort Time: "
         << duration1.count() << " microseconds" << endl;

    cout << "Parallel QuickSort Time: "
         << duration2.count() << " microseconds" << endl;

    return 0;
}