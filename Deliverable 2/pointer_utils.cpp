#include "pointer_utils.h"

namespace PointerUtils
{

    MemoryBlock allocations[100];
    int allocCount = 0;

    void *allocate(size_t size, const string &type)
    {
        void *ptr = malloc(size);
        if (!ptr)
        {
            cerr << "Memory allocation failed for type: " << type << endl;
            return nullptr;
        }

        allocations[allocCount++] = {ptr, size, type};
        cout << "Allocated " << size << " bytes for " << type
             << " at address " << ptr << endl;

        return ptr;
    }

    void deallocate(void *ptr, const string &type)
    {
        if (!ptr)
        {
            cerr << "Attempted to deallocate a null pointer (" << type << ")." << endl;
            return;
        }

        bool found = false;
        for (int i = 0; i < allocCount; ++i)
        {
            if (allocations[i].address == ptr)
            {
                cout << "Freed " << allocations[i].size << " bytes for "
                     << allocations[i].type << " at address " << ptr << endl;
                for (int j = i; j < allocCount - 1; ++j)
                    allocations[j] = allocations[j + 1];
                allocCount--;
                found = true;
                break;
            }
        }

        if (!found)
            cerr << "Pointer not found in allocation records: " << ptr << endl;

        free(ptr);
    }

    bool isValidPointer(void *ptr)
    {
        if (!ptr)
            return false;
        for (int i = 0; i < allocCount; ++i)
        {
            if (allocations[i].address == ptr)
                return true;
        }
        return false;
    }

    void showMemoryUsage()
    {
        cout << "\nMemory Tracker — Current Allocations (" << allocCount << "):\n";
        if (allocCount == 0)
        {
            cout << "   No active allocations.\n";
            return;
        }
        cout << left << setw(15) << "Address"
             << setw(10) << "Size"
             << setw(15) << "Type" << endl;
        cout << string(40, '-') << endl;
        for (int i = 0; i < allocCount; ++i)
        {
            cout << left << setw(15) << allocations[i].address
                 << setw(10) << allocations[i].size
                 << setw(15) << allocations[i].type << endl;
        }
        cout << endl;
    }

    void showPointerDetails(void *ptr, const string &varName)
    {
        cout << "\nPointer Inspection — " << varName << endl;
        cout << "   Address: " << ptr << endl;
        if (isValidPointer(ptr))
            cout << "   This pointer is currently allocated and valid.\n";
        else
            cout << "   This pointer is invalid or already freed.\n";
    }
}
