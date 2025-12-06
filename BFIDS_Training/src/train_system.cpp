#include "linked_list.h"
#include "file_io.h"

#include "sorting_algorithms.h"
#include "avl_profile.h"

#include <iostream>
#include <string>
#include <cstdlib> // Added for system()

using namespace std;

/*
    train_system.cpp
    ----------------
    Batch Training Module.
    Processes raw log files to create a behavioral fingerprint.
    
    Process Flow:
    1. ETL (Extract): Load raw actions from a log file into a Linked List.
    2. Sort: Convert List to Array and Sort by Timestamp (Merge Sort).
    3. Train: Insert sorted actions into an AVL Tree to calculate averages/frequencies.
    4. Export: Save the final fingerprint to CSV.
*/

int main(int argc, char* argv[]) {

    string rawLog = "raw_actions.log";
    // --- CHANGE: Default path updated ---
    string outCsv = "fingerprints/fingerprints.csv";

    // Allow custom input and output file names
    if (argc >= 2) rawLog = argv[1];
    if (argc >= 3) outCsv = argv[2];

    cout << "\nTraining started...\n";

    // Loading the raw actions into our linked list
    LinkedList list;
    bool ok = FileIO::loadActionsFromFile(rawLog, list);

    if (!ok || list.getSize() == 0) {
        cerr << "Could not load raw logs. Nothing to train.\n";
        return 1;
    }

    cout << "Loaded " << list.getSize() << " actions.\n";

    // Converting list into a plain array for sorting
    int count = 0;
    UserAction* arr = new UserAction[count];
     list.copyToArray(arr);


    if (!arr || count == 0) {
        cerr << "Failed to convert list to array.\n";
        return 1;
    }

    // Sorting everything by timestamp
    // Uses Merge Sort O(n log n)
    SortingAlgorithms::mergeSort(arr, 0, count - 1);
    cout << "Actions sorted.\n";

    // Building the fingerprint using an AVL tree
    // Aggregates frequency and calculates running average duration
    AVLProfile fingerprint;
    for (int i = 0; i < count; i++) {
        fingerprint.insertOrUpdate(arr[i].processName, arr[i].duration);
    }

    cout << "Fingerprint tree created.\n";

    // --- CHANGE: Ensure directory exists ---
    system("mkdir -p fingerprints");

    // Saving the fingerprint to a CSV file
    bool saved = fingerprint.exportToCSV(outCsv);

    if (!saved) {
        cerr << "Could not save fingerprint CSV.\n";
        delete[] arr;
        return 1;
    }

    cout << "Fingerprint saved to " << outCsv << endl;

    delete[] arr;

    cout << "Training finished.\n";
    return 0;
}