#ifndef A88BAFA1_AB72_4A2D_812B_D477C32DA685
#define A88BAFA1_AB72_4A2D_812B_D477C32DA685

#include <cstddef>
#include "../../Deliverable 2/include/user_action.h"


class SortingAlgorithms {
public:
    // Sort array of UserAction by timestamp (ascending)
    static void mergeSort(UserAction arr[], size_t left, size_t right);

private:
    static void merge(UserAction arr[], size_t left, size_t mid, size_t right);
};

#endif /* A88BAFA1_AB72_4A2D_812B_D477C32DA685 */

