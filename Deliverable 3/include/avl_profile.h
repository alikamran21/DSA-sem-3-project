#ifndef A24A5966_2E58_4AB3_BF14_A5239420D819
#define A24A5966_2E58_4AB3_BF14_A5239420D819

#include <string>
#include "UserActionProfile.h"

/*
 * AVLProfile
 * ----------
 * Stores the statistical fingerprint in an AVL Tree.
 * Provides:
 *   - insertOrUpdate(): used during training (increment frequency + avgDuration)
 *   - search(): find profile for live detection comparison
 *   - exportToCSV(): save final fingerprint to disk
 *   - importFromCSV(): load fingerprint for live detection mode
 *
 * Node stores:
 *   UserActionProfile { processName, frequency, avgDuration }
 */
class AVLProfile {
public:
    AVLProfile();
    ~AVLProfile();

    // TRAINING: Insert new process or update avgDuration + frequency
    void insertOrUpdate(const std::string& processName, double duration);

    // IMPORT: Direct insert of a full profile (frequency, avgDuration)
    void insertProfileDirect(const UserActionProfile& profile);

    // SEARCH: Returns true if process exists in fingerprint
    bool search(const std::string& processName, UserActionProfile& outProfile) const;

    // PERSISTENCE
    bool exportToCSV(const std::string& filename) const;
    bool importFromCSV(const std::string& filename);

private:
    struct Node {
        UserActionProfile profile;
        Node* left;
        Node* right;
        int height;

        Node(const UserActionProfile& p)
            : profile(p), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    // AVL Helpers
    int height(Node* n) const;
    int getBalance(Node* n) const;

    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);

    Node* insertOrUpdateNode(Node* node, const std::string& processName, double duration);
    Node* insertProfileNode(Node* node, const UserActionProfile& profile);

    Node* findNode(Node* node, const std::string& processName) const;

    void inorderExport(Node* node, const std::function<void(const UserActionProfile&)>& fn) const;

    void destroy(Node* node);
};

#endif /* A24A5966_2E58_4AB3_BF14_A5239420D819 */

