#include "sorting_algorithms.h"
#include <iostream>

using namespace std;

/*
    This merge function joins two sorted halves of the array back together.
    We sort the UserAction items by timestamp so the fingerprint is built
    in the correct order of how the user performed actions.
*/
void SortingAlgorithms::merge(UserAction arr[], size_t left, size_t mid, size_t right) {

    size_t leftSize = mid - left + 1;
    size_t rightSize = right - mid;

    // Create temporary arrays (raw arrays instead of vectors)
    UserAction* leftArr = new UserAction[leftSize];
    UserAction* rightArr = new UserAction[rightSize];

    // Copy data into the temporary arrays
    for (size_t i = 0; i < leftSize; i++)
        leftArr[i] = arr[left + i];

    for (size_t j = 0; j < rightSize; j++)
        rightArr[j] = arr[mid + 1 + j];

    size_t i = 0, j = 0, k = left;

    // Merge both halves into the original array
    while (i < leftSize && j < rightSize) {

        if (leftArr[i].timestamp <= rightArr[j].timestamp) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }

        k++;
    }

    // Copy any leftover elements from the left half
    while (i < leftSize) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // Copy any leftover elements from the right half
    while (j < rightSize) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }

    // Free the temporary memory
    delete[] leftArr;
    delete[] rightArr;
}

/*
    Merge Sort splits the array into two halves,
    sorts each half, and then merges them.

    We use Merge Sort to make sure all user actions
    are in correct time order before building the fingerprint.
*/
void SortingAlgorithms::mergeSort(UserAction arr[], size_t left, size_t right) {

    // Base case: single element
    if (left >= right)
        return;

    size_t mid = left + (right - left) / 2;

    // Sort left half
    mergeSort(arr, left, mid);

    // Sort right half
    mergeSort(arr, mid + 1, right);

    // Combine both halves
    merge(arr, left, mid, right);
}
void swap(UserAction& a, UserAction& b) {
    UserAction temp = a;
    a = b;
    b = temp;
}
size_t SortingAlgorithms::partition(UserAction arr[], size_t low, size_t high) {
    // Choose the rightmost element as the pivot (using timestamp for comparison)
    double pivot = arr[high].timestamp;
    size_t i = low - 1; // Index of smaller element

    for (size_t j = low; j < high; j++) {
        // If current element's timestamp is smaller than or equal to the pivot's
        if (arr[j].timestamp <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    // Place the pivot in its correct sorted position
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

/*
    Purpose: Implements the Quick Sort algorithm.
    Working: Uses recursion to divide the array based on a pivot returned by 'partition'.
    DSA Concept: Implementation of Quick Sort (O(n log n) average) and core **Recursion**.
    Use: Alternative to mergeSort in `bio_trainer.cpp` to sort raw log data.
*/
void SortingAlgorithms::quickSort(UserAction arr[], size_t low, size_t high) {
    if (low < high) {
        size_t pi = partition(arr, low, high);

        // Recursively sort the subarrays
        // Need to check bounds when converting size_t to potential negative index (pi - 1)
        if (pi > 0)
            quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


void SortingAlgorithms::heapify(UserAction arr[], size_t n, size_t i) {
    size_t largest = i; // Initialize largest as root
    size_t left = 2 * i + 1; // left child index
    size_t right = 2 * i + 2; // right child index

    // If left child is larger than root
    if (left < n && arr[left].timestamp > arr[largest].timestamp)
        largest = left;

    // If right child is larger than largest so far
    if (right < n && arr[right].timestamp > arr[largest].timestamp)
        largest = right;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);
        // Recursively heapify the affected subtree
        heapify(arr, n, largest);
    }
}
void SortingAlgorithms::heapSort(UserAction arr[], size_t n) {
    // 1. Build Max-Heap (rearrange array)
    // Starts heapify from the last non-leaf node up to the root (index 0)
    // The loop iterates in reverse down to 0, using size_t requires care for the condition i >= 0
    for (size_t i = n / 2 - 1; i != (size_t)-1; i--)
        heapify(arr, n, i);

    // 2. One by one extract elements from the heap
    for (size_t i = n - 1; i > 0; i--) {
        // Move current root (largest) to the end of the array
        swap(arr[0], arr[i]);

        // Call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}