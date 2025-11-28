#include "sorting_algorithms.h"
#include "user_action.h"
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
