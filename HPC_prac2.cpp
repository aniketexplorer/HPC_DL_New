#include <omp.h>
#include <iostream>
#include <string>
#include <chrono>

using namespace std::chrono;
using namespace std;

void displayArray(string message, int nums[], int length)
{
    cout << "\t" << message << ": [";
    for (int i = 0; i < length; i++)
    {
        cout << nums[i];
        if (i != length - 1)
            cout << ", ";
    }
    cout << "]" << endl;
}

void merge(int nums[], int leftStart, int leftEnd, int rightStart, int rightEnd)
{
    int n = (rightEnd - leftStart) + 1;
    int tempArray[n];

    int t = 0;
    int l = leftStart;
    int r = rightStart;

    while (l <= leftEnd && r <= rightEnd)
    {
        if (nums[l] <= nums[r])
            tempArray[t++] = nums[l++];
        else
            tempArray[t++] = nums[r++];
    }

    while (l <= leftEnd)
        tempArray[t++] = nums[l++];

    while (r <= rightEnd)
        tempArray[t++] = nums[r++];

    for (int i = 0; i < n; i++)
        nums[leftStart + i] = tempArray[i];
}

void mergeSort(int nums[], int start, int end)
{
    if (start < end)
    {
        int mid = (start + end) / 2;

#pragma omp parallel sections num_threads(2)
        {
#pragma omp section
            mergeSort(nums, start, mid);

#pragma omp section
            mergeSort(nums, mid + 1, end);
        }

        merge(nums, start, mid, mid + 1, end);
    }
}

void bubbleSort(int nums[], int length)
{
    for (int i = 0; i < length; i++)
    {
        int start = i % 2;

#pragma omp parallel for
        for (int j = start; j < length - 1; j += 2)
        {
            if (nums[j] > nums[j + 1])
            {
                int temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }
        }
    }
}

int main()
{
    // Bubble Sort Input
    int length1;

    cout << "Enter number of elements for Bubble Sort: ";
    cin >> length1;

    int nums1[length1];

    cout << "Enter elements: ";
    for (int i = 0; i < length1; i++)
    {
        cin >> nums1[i];
    }

    cout << "\nBubble Sort:" << endl;
    displayArray("Before", nums1, length1);

    auto start_bubble = high_resolution_clock::now();
    bubbleSort(nums1, length1);
    auto end_bubble = high_resolution_clock::now();

    displayArray("After", nums1, length1);

    auto duration_bubble = duration_cast<microseconds>(end_bubble - start_bubble);

    cout << "\nExecution time for Bubble Sort: "
         << duration_bubble.count()
         << " microseconds" << endl;

    // Merge Sort Input
    int length2;

    cout << "\nEnter number of elements for Merge Sort: ";
    cin >> length2;

    int nums2[length2];

    cout << "Enter elements: ";
    for (int i = 0; i < length2; i++)
    {
        cin >> nums2[i];
    }

    cout << "\nMerge Sort:" << endl;
    displayArray("Before", nums2, length2);

    auto start_merge = high_resolution_clock::now();
    mergeSort(nums2, 0, length2 - 1);
    auto end_merge = high_resolution_clock::now();

    displayArray("After", nums2, length2);

    auto duration_merge = duration_cast<microseconds>(end_merge - start_merge);

    cout << "\nExecution time for Merge Sort: "
         << duration_merge.count()
         << " microseconds" << endl;

    return 0;
}