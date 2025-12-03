#include "../include/anomaly_heap.h"
#include <stdexcept>
#include <iostream>

using namespace std;

// Helper function to swap two nodes
void AnomalyHeap::swapAnomalyNode(AnomalyNode& a, AnomalyNode& b) {
    AnomalyNode temp = a;
    a = b;
    b = temp;
}

AnomalyHeap::AnomalyHeap(size_t initialCapacity) : size(0), capacity(initialCapacity) {
    // Dynamic **Array** allocation
    heap = new AnomalyNode[capacity];
}

AnomalyHeap::~AnomalyHeap() {
    delete[] heap;
}

// Implements dynamic **Array** resizing
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

void AnomalyHeap::heapifyUp(size_t index) {
    if (index > 0) {
        size_t parent = (index - 1) / 2;
        if (heap[index].anomalyScore > heap[parent].anomalyScore) {
            swapAnomalyNode(heap[index], heap[parent]);
            heapifyUp(parent); // **Recursion**
        }
    }
}

void AnomalyHeap::insert(const UserAction& action, double score) {
    if (score <= 0) return;

    if (size == capacity) {
        resize(capacity * 2);
    }

    heap[size] = AnomalyNode(action, score);
    heapifyUp(size);
    size++;
}

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

const AnomalyNode& AnomalyHeap::peekMax() const {
    if (isEmpty()) {
        throw runtime_error("Attempted to peek at an empty anomaly heap.");
    }
    return heap[0];
}
