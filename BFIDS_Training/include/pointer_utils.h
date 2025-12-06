#ifndef POINTER_UTILS_H
#define POINTER_UTILS_H

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

/*
 * PointerUtils
 * ------------
 * Utilities for memory tracking, allocation, and pointer validation.
 * Provides:
 * - allocate(): safe memory allocation with tracking
 * - deallocate(): safe memory deallocation
 * - isValidPointer(): checks if address is valid
 * - showMemoryUsage(): displays current allocations
 *
 * Struct stores:
 * MemoryBlock { address, size, type }
 */

namespace PointerUtils {

    struct MemoryBlock {
        void* address;
        size_t size;
        string type;
    };

    void* allocate(size_t size, const string& type);
    void deallocate(void* ptr, const string& type);

    bool isValidPointer(void* ptr);
    void showMemoryUsage();
    void showPointerDetails(void* ptr, const string& varName);

    extern MemoryBlock allocations[100];
    extern int allocCount;
}

#endif