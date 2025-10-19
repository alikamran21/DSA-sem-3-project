<div align="center">

# Contributions for: Behavioral Fingerprint-Based Intrusion Detection System (BFIDS)

### Built with Pure C++ | Data Structures & Algorithms

| Role | Name | Registration No. | Status |
|------|------|------------------|--------|
| Primary Contributor | **Fatima Rehman** | **2024177** | **Initial Implementation Phase (Deliverable 2)** |

</div>

---

## Alignment with Problem Statement

The Behavioral Fingerprint-Based Intrusion Detection System (BFIDS) is designed to detect abnormal user behavior by analyzing unique characteristics such as command sequences, timing, and contextual transitions.

During Deliverable 2, Fatima Rehman focused on implementing the core linear data structures and foundational utilities in C++. These components serve as the structural base for the system’s fingerprinting and anomaly detection mechanisms. The work directly supports CLO-1 by applying core data structures and algorithms to design a secure and efficient system architecture.

---

## Phase 1: Foundational Data Structures and Core Utilities  
**Deliverable 2 — Initial Implementation Phase**

This phase focused on the development of essential modules that enable behavioral data capture, temporal sequencing, and efficient memory management.

---

### Core Architecture and Utility Layer

| Module | Files | Contribution and Relevance to BFIDS |
|---------|--------|------------------------------------|
| **User Action Struct** | `include/user_action.h` | Designed the `UserAction` structure that represents the fundamental behavioral unit. Key fields include `action`, `duration`, `timestamp`, and `nextAction`, which collectively form the behavioral fingerprint of each session. |
| **File I/O Utility** | `include/file_io.h`, `src/file_io.cpp` | Implemented the methods for data persistence and report generation. The `saveActionsToFile` function transfers actions from a linked list to a dynamic array, applies `std::sort` for timestamp ordering, and outputs formatted tables for manual and automated analysis. This ensures chronological consistency and data integrity. |

---

### Linear Data Structure Implementations

| Data Structure | Files | Application in BFIDS and Algorithmic Details |
|----------------|--------|----------------------------------------------|
| **Doubly / Circular Linked List** | `include/linked_list.h`, `src/linked_list.cpp` | Developed a doubly linked list to maintain reversible session history, allowing O(1) traversal in both directions. The circular variant supports continuous monitoring, providing a base for sliding window analysis in future behavioral detection. |
| **Stack (LIFO)** | `include/stack_monitor.h`, `src/stack_monitor.cpp` | Built a stack using a linked-list model to monitor nested session actions, such as file openings and closings. The LIFO structure ensures logical sequence validation; inconsistencies in the open-close sequence indicate anomalies. Added deep copy functionality for state checkpointing. |
| **Queue (FIFO)** | `include/queue_monitor.h`, `src/queue_monitor.cpp` | Created a queue structure to maintain the order of events as they occur. This queue acts as the event buffer, feeding ordered data into higher-level fingerprinting and anomaly detection modules. |

---

## Time Complexity Summary

| Component | Primary Operation | Average Time Complexity |
|------------|------------------|-------------------------|
| **Linked List (Insert/Delete)** | Append or remove node | O(1) |
| **Linked List (Traversal/Search)** | Iterate through nodes | O(n) |
| **Stack (Push/Pop)** | Add or remove top element | O(1) |
| **Queue (Enqueue/Dequeue)** | Add or remove front element | O(1) |
| **File I/O (Save/Load)** | File read/write and sorting | O(n log n) |
| **Sorting (std::sort)** | Temporal ordering of actions | O(n log n) |
| **UserAction Operations** | Field access or modification | O(1) |

---

## Deliverable 2 Summary

Fatima Rehman completed the foundational data structure layer for BFIDS, implementing the primary building blocks for user behavior recording and sequential data management. The modules include the `UserAction` struct, `FileIO` utilities, and linear data structures (Linked List, Stack, Queue), each integrated with proper memory handling and modular design.

These implementations collectively enable:

- Structured and reliable capture of user actions  
- Accurate temporal sequencing for event reconstruction  
- Efficient memory management through custom linked structures  
- Support for real-time anomaly detection in future deliverables  

This groundwork provides a stable foundation for the upcoming phase, where non-linear data structures (Hash Table, BST, Trie) will be developed for advanced behavioral fingerprinting and anomaly correlation.

---

## Next Steps (Planned for Later Phases)

| Planned Module | Purpose |
|----------------|----------|
| **Hash Table (`hash_profiles.h/.cpp`)** | Store and retrieve user fingerprint profiles in O(1) time. |
| **Binary Search Tree (`bst_frequency.h/.cpp`)** | Track and analyze action frequency patterns efficiently. |
| **Trie (`trie_behavior.h/.cpp`)** | Model and match behavioral command sequences for pattern recognition. |

---

<div align="center">

**End of Deliverable 2 — Fatima Rehman’s Contributions**

</div>
