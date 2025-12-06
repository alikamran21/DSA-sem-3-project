#ifndef A88BAFA1_AB72_4A2D_812B_D477C32DA685
#define A88BAFA1_AB72_4A2D_812B_D477C32DA685

#include <cstddef>
#include "user_action.h"

/*
 * SortingAlgorithms
 * -----------------
 * Collection of sorting algorithms for UserAction arrays.
 * Provides:
 * - mergeSort(): O(n log n) divide and conquer sort
 * - quickSort(): O(n log n) average case partition sort
 * - heapSort(): O(n log n) heap-based sort
 */

class SortingAlgorithms {
public:
    // Sort array of UserAction by timestamp (ascending)
    static void mergeSort(UserAction arr[], size_t left, size_t right);
    static void quickSort(UserAction arr[], size_t low, size_t high);
    static void heapSort(UserAction arr[], size_t n);
private:
    static void merge(UserAction arr[], size_t left, size_t mid, size_t right);
    static size_t partition(UserAction arr[], size_t low, size_t high);
    static void heapify(UserAction arr[], size_t n, size_t i);
};

#endif