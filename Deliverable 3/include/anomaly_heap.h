#ifndef F0B538EE_8E44_4955_A72B_01287974D5A4
#define F0B538EE_8E44_4955_A72B_01287974D5A4
#ifndef ANOMALY_HEAP_H
#define ANOMALY_HEAP_H

#include <stddef.h> // For size_t
#include "../../Deliverable 2/include/user_action.h" // Assumed defined

/*
 * AnomalyNode: Stores an action and its calculated severity score.
 */
struct AnomalyNode {
    UserAction action;
    double anomalyScore; // Higher score means higher priority

    AnomalyNode() : anomalyScore(0.0) {}
    AnomalyNode(const UserAction& a, double score) : action(a), anomalyScore(score) {}
};

/*
 * AnomalyHeap (Max-Heap)
 * Implemented using a dynamically allocated C-style array.
 */
class AnomalyHeap {
private:
    AnomalyNode* heap; // **Raw Array** (Heap storage)
    size_t size;       // Current number of elements
    size_t capacity;   // Max elements currently allocated

    // Dynamic **Array** resizing (required since vectors are forbidden)
    void resize(size_t newCapacity);

    void heapifyDown(size_t index);
    void heapifyUp(size_t index);
    void swapAnomalyNode(AnomalyNode& a, AnomalyNode& b);

public:
    AnomalyHeap(size_t initialCapacity = 50);
    ~AnomalyHeap();

    // Insert new anomaly (O(log n))
    void insert(const UserAction& action, double score);

    // Retrieves and removes the max anomaly (O(log n))
    AnomalyNode extractMax();

    // Returns the max anomaly without removal (O(1))
    const AnomalyNode& peekMax() const;

    bool isEmpty() const { return size == 0; }
};


#endif /* F0B538EE_8E44_4955_A72B_01287974D5A4 */
