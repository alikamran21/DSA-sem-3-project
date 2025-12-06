#include "anomaly_heap.h"
#include <stdexcept>
#include <iostream>

using namespace std;

/*
    anomaly_heap.cpp
    ----------------
    Implements a Priority Queue using a Max-Heap.
    Used to prioritize anomalies based on their severity score.
    
    Key DSA Concepts:
        - Dynamic Array Resizing
        - Heapify Up/Down (Tree traversal on array)
*/

// Helper function to swap two nodes
void AnomalyHeap::swapAnomalyNode(AnomalyNode& a, AnomalyNode& b) {
    AnomalyNode temp = a;
    a = b;
    b = temp;
}

// Constructor
AnomalyHeap::AnomalyHeap(size_t initialCapacity) : size(0), capacity(initialCapacity) {
    // Dynamic **Array** allocation
    heap = new AnomalyNode[capacity];
}

// Destructor
AnomalyHeap::~AnomalyHeap() {
    delete[] heap;
}

/*
    resize
    ------
    Manually resizes the dynamic array when capacity is reached.
    Copies existing elements to a larger memory block.
*/
void AnomalyHeap::resize(size_t newCapacity) {
    if (newCapacity <= capacity) return;

    AnomalyNode* newHeap = new AnomalyNode[newCapacity];

    // Copy existing data using a simple loop
    for (size_t i = 0; i < size; ++i) {
        newHeap[i] = heap[i];
    }

    delete[] heap; // Free old memory
    heap = newHeap;
    capacity = newCapacity;
}

/*
    heapifyDown
    -----------
    Restores the Max-Heap property starting from the given index downwards.
    Used after extracting the root element.
*/
void AnomalyHeap::heapifyDown(size_t index) {
    size_t largest = index;
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;

    if (left < size && heap[left].anomalyScore > heap[largest].anomalyScore)
        largest = left;

    if (right < size && heap[right].anomalyScore > heap[largest].anomalyScore)
        largest = right;

    if (largest != index) {
        swapAnomalyNode(heap[index], heap[largest]);
        heapifyDown(largest); // **Recursion**
    }
}

/*
    heapifyUp
    ---------
    Restores the Max-Heap property starting from the given index upwards.
    Used after inserting a new element at the bottom.
*/
void AnomalyHeap::heapifyUp(size_t index) {
    if (index > 0) {
        size_t parent = (index - 1) / 2;
        if (heap[index].anomalyScore > heap[parent].anomalyScore) {
            swapAnomalyNode(heap[index], heap[parent]);
            heapifyUp(parent); // **Recursion**
        }
    }
}

/*
    insert
    ------
    Adds a new anomaly event to the heap.
    Triggers resize if full, then floats the new element up.
*/
void AnomalyHeap::insert(const UserAction& action, double score) {
    if (score <= 0) return;

    if (size == capacity) {
        resize(capacity * 2);
    }

    heap[size] = AnomalyNode(action, score);
    heapifyUp(size);
    size++;
}

/*
    extractMax
    ----------
    Removes and returns the anomaly with the highest score (root).
    Replaces root with the last element and sinks it down.
*/
AnomalyNode AnomalyHeap::extractMax() {
    if (isEmpty()) {
        throw runtime_error("Attempted to extract from an empty anomaly heap.");
    }

    AnomalyNode maxNode = heap[0];

    heap[0] = heap[size - 1];
    size--;

    heapifyDown(0);

    return maxNode;
}

/*
    peekMax
    -------
    Returns the highest priority anomaly without removing it.
*/
const AnomalyNode& AnomalyHeap::peekMax() const {
    if (isEmpty()) {
        throw runtime_error("Attempted to peek at an empty anomaly heap.");
    }
    return heap[0];
}