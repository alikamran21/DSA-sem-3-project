#ifndef ANOMALY_HEAP_H
#define ANOMALY_HEAP_H

#include <stddef.h> // For size_t
#include "user_action.h" // Assumed defined

/*
 * AnomalyHeap
 * -----------
 * Max-Heap implementation for prioritizing anomaly events.
 * Provides:
 * - insert(): adds a new anomaly with a score (O(log n))
 * - extractMax(): removes and returns the highest priority anomaly
 * - peekMax(): returns the highest priority anomaly without removal
 *
 * Node stores:
 * AnomalyNode { UserAction, anomalyScore }
 */

struct AnomalyNode {
    UserAction action;
    double anomalyScore; // Higher score means higher priority

    AnomalyNode() : anomalyScore(0.0) {}
    AnomalyNode(const UserAction& a, double score) : action(a), anomalyScore(score) {}
};

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


#endif