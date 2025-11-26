#include "../include/linked_list.h"        // From Deliverable 2
#include "../include/file_io.h"           // From Deliverable 2

#include "../include/sorting_algorithms.h" // From Deliverable 3
#include "../include/avl_profile.h"        // From Deliverable 3

#include <iostream>
#include <string>

using namespace std;

/*
    This program reads the raw user activity log,
    sorts everything in correct time order,
    builds a fingerprint using an AVL tree,
    and finally saves that fingerprint into a CSV file.
*/

int main(int argc, char* argv[]) {

    string rawLog = "raw_actions.log";
    string outCsv = "fingerprints.csv";

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
    UserAction* arr = list.toArray(count);

    if (!arr || count == 0) {
        cerr << "Failed to convert list to array.\n";
        return 1;
    }

    // Sorting everything by timestamp
    SortingAlgorithms::mergeSort(arr, 0, count - 1);
    cout << "Actions sorted.\n";

    // Building the fingerprint using an AVL tree
    AVLProfile fingerprint;
    for (int i = 0; i < count; i++) {
        fingerprint.insertOrUpdate(arr[i].processName, arr[i].duration);
    }

    cout << "Fingerprint tree created.\n";

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
