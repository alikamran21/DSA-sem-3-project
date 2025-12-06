#include "pointer_utils.h"

/*
    pointer_utils.cpp
    -----------------
    Provides custom memory management wrappers.
    Tracks allocations to help detect memory leaks and invalid accesses.
    Simulates a basic garbage collector or memory profiler.
*/

namespace PointerUtils
{

    // A simple array to keep track of all active memory allocations
    MemoryBlock allocations[100]; // can store up to 100 allocation records
    int allocCount = 0;            // how many allocations are currently active

    /*
        allocate
        --------
        Custom wrapper for malloc().
        Logs the allocation size, type, and address.
    */
    void *allocate(size_t size, const string &type)
    {
        // Try to allocate memory
        void *ptr = malloc(size);

        // Check if allocation failed
        if (!ptr)
        {
            cerr << "Memory allocation failed for type: " << type << endl;
            return nullptr;
        }

        // Store info about this allocation in our tracker
        allocations[allocCount++] = {ptr, size, type};

        // Log what happened for debugging / visualization
        cout << "Allocated " << size << " bytes for " << type
             << " at address " << ptr << endl;

        return ptr;
    }

    /*
        deallocate
        ----------
        Custom wrapper for free().
        Removes the record from the tracker and frees memory.
        Warns if freeing a pointer that wasn't tracked.
    */
    void deallocate(void *ptr, const string &type)
    {
        // Handle case where user passes a null pointer
        if (!ptr)
        {
            cerr << "Attempted to deallocate a null pointer (" << type << ")." << endl;
            return;
        }

        bool found = false;

        // Look through all known allocations to find this pointer
        for (int i = 0; i < allocCount; ++i)
        {
            if (allocations[i].address == ptr)
            {
                // Found it — print some info before freeing
                cout << "Freed " << allocations[i].size << " bytes for "
                     << allocations[i].type << " at address " << ptr << endl;

                // Shift the array left to remove this record
                for (int j = i; j < allocCount - 1; ++j)
                    allocations[j] = allocations[j + 1];

                allocCount--;
                found = true;
                break;
            }
        }

        // If we didn’t find it, warn the user
        if (!found)
            cerr << "Pointer not found in allocation records: " << ptr << endl;

        // Actually free the memory
        free(ptr);
    }

    /*
        isValidPointer
        --------------
        Checks if a pointer exists in the allocation record.
        Returns true if the memory is currently allocated.
    */
    bool isValidPointer(void *ptr)
    {
        if (!ptr)
            return false;

        // Look through active allocations
        for (int i = 0; i < allocCount; ++i)
        {
            if (allocations[i].address == ptr)
                return true;
        }
        return false;
    }

    /*
        showMemoryUsage
        ---------------
        Prints a report of all currently allocated memory blocks.
    */
    void showMemoryUsage()
    {
        cout << "\nMemory Tracker — Current Allocations (" << allocCount << "):\n";

        // If no memory is currently allocated
        if (allocCount == 0)
        {
            cout << "   No active allocations.\n";
            return;
        }

        // Display table headers
        cout << left << setw(15) << "Address"
             << setw(10) << "Size"
             << setw(15) << "Type" << endl;

        // Draw a separator line
        cout << string(40, '-') << endl;

        // Loop through all active allocations and print details
        for (int i = 0; i < allocCount; ++i)
        {
            cout << left << setw(15) << allocations[i].address
                 << setw(10) << allocations[i].size
                 << setw(15) << allocations[i].type << endl;
        }
        cout << endl;
    }

    /*
        showPointerDetails
        ------------------
        Inspects a specific pointer and prints its status.
    */
    void showPointerDetails(void *ptr, const string &varName)
    {
        cout << "\nPointer Inspection — " << varName << endl;
        cout << "   Address: " << ptr << endl;

        // Tell the user if this pointer is still valid
        if (isValidPointer(ptr))
            cout << "   This pointer is currently allocated and valid.\n";
        else
            cout << "   This pointer is invalid or already freed.\n";
    }
}