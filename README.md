<div align="center">

# 🧠 Behavioral Fingerprint-Based Intrusion Detection System (BFIDS)

### Built with Pure C++ | Data Structures & Algorithms | Cybersecurity Project  

[![C++](https://img.shields.io/badge/Language-C++-00599C?style=flat&logo=cplusplus)]()
[![DSA](https://img.shields.io/badge/Core-Data%20Structures%20%26%20Algorithms-blueviolet)]()
[![Cybersecurity](https://img.shields.io/badge/Focus-Cybersecurity-red)]()
[![Status](https://img.shields.io/badge/Status-In%20Development-yellow)]()

</div>

---

# DSA-sem-3-project  
**Title:** Behavioral Fingerprint-Based Intrusion Detection System (BFIDS)  
**Team:** (Fatima Rahman, Ali Kamran)

---

## 🧩 Problem Statement

Modern cybersecurity systems often rely on static signatures or heuristic methods that fail to identify new or insider threats.  
Every authenticated user exhibits unique behavioral characteristics—such as command sequences, typing intervals, and action transitions—that can be analyzed to detect abnormal activity.

This project aims to implement a **Behavioral Fingerprint-Based Anomaly Detection System** using only **C++ and core Data Structures and Algorithms**.  
The system will learn normal user behavior, generate behavioral fingerprints, and detect anomalies by comparing new sessions against stored profiles.  
It will demonstrate how foundational DSA concepts can be applied to a real-world cybersecurity problem without relying on external libraries or machine learning frameworks.

---

## 📘 List of Data Structures and Algorithms

| **Data Structure / Algorithm** | **Application in Project** |
|--------------------------------|-----------------------------|
| Array / Dynamic Array | Store time-series samples, latency, and raw behavioral data. |
| Singly & Doubly Linked Lists | Maintain session logs and reversible action histories. |
| Stack (LIFO) | Track nested actions such as file open/close and process calls. |
| Queue (FIFO) | Manage ordered event logs for temporal sequence validation. |
| Circular Queue | Implement sliding windows for recent session buffering. |
| Priority Queue / Heap (Min/Max) | Rank anomaly scores and generate priority alerts. |
| Hash Table | Map user IDs to fingerprints for constant-time retrieval. |
| Binary Search Tree (BST) | Store sorted frequency metrics of user actions. |
| AVL Tree | Maintain balanced user profiles for efficient lookup. |
| Trie (Prefix Tree) | Represent sequences of commands or key transitions. |
| Graph (Adjacency List) | Model user action transitions and dependencies. |
| Disjoint Set (Union–Find) | Cluster correlated behaviors into groups. |
| Sorting Algorithms | Bubble, Selection, Insertion, Merge, Quick, Heap Sort — used for session ranking and report generation. |
| Searching Algorithms | Linear, Binary, Interpolation — used for command or profile lookup. |
| Divide & Conquer | Implemented through Merge Sort and Quick Sort. |
| Greedy Algorithms | Used in threshold optimization and alert prioritization. |
| Dynamic Programming | Compute behavioral similarity using edit distance. |
| Graph Traversal | DFS and BFS for exploring action transition graphs. |
| Shortest Path (Dijkstra) | Determine deviation cost between behavior paths. |
| Backtracking | Used for reconstructing possible valid action sequences. |
| Recursion | Utilized in tree traversals, sorting, and matching routines. |
| File I/O Operations | Reading, writing, and updating fingerprints, logs, and anomaly reports. |

This ensures comprehensive coverage of almost every data structure and algorithm from the **CS221** course.

---

## 🎯 Expected Outcomes

| **Outcome** | **Description** |
|--------------|----------------|
| 1. Modular C++ Implementation | A fully functional system demonstrating behavioral anomaly detection using classic DSA. |
| 2. Behavioral Fingerprints | Generation and comparison of fingerprints for multiple simulated users. |
| 3. Real-Time Detection | Identification of abnormal sessions with ranked severity using heaps. |
| 4. Algorithmic Analysis | Time and space complexity evaluation for core operations. |
| 5. Performance Reporting | Documentation of detection accuracy and runtime performance. |
| 6. Scalability & Extensibility | Architecture ready for integration with larger cybersecurity systems. |

---

## 🗂️ Data Repository

All behavioral data will be stored as plain text logs and fingerprint records in the `/data` directory.

### Example Data Format:

| Timestamp | UserID | Action | Process | Duration | Next_Action | Status |
|------------|---------|---------|----------|-----------|--------------|---------|
| 2025-10-06 22:45:31 | U101 | Open_File | explorer.exe | 120ms | Edit_Doc | Normal |
| 2025-10-06 22:46:11 | U101 | Execute_Command | cmd.exe | 80ms | Network_Call | Deviated |

The repository will maintain:

- **user_logs.txt** → Raw session data  
- **fingerprints.txt** → Stored behavioral profiles  
- **anomalies.txt** → Detected anomalies with scores  

---

## 📁 Proposed File Structure

```bash
BehavioralFingerprintSystem/
│
├── main.cpp
│
├── include/
│   ├── user_action.h
│   ├── stack_monitor.h
│   ├── queue_logger.h
│   ├── linked_list.h
│   ├── trie_behavior.h
│   ├── graph_transition.h
│   ├── bst_frequency.h
│   ├── avl_profile.h
│   ├── hash_profiles.h
│   ├── heap_alerts.h
│   ├── disjoint_cluster.h
│   ├── sorting_algorithms.h
│   ├── searching_algorithms.h
│   ├── dynamic_edit.h
│   ├── traversal_algorithms.h
│   ├── fingerprint_manager.h
│   ├── anomaly_detection.h
│   ├── utils.h
│   └── file_io.h
│
├── src/
│
├── data/
│   ├── user_logs.txt
│   ├── fingerprints.txt
│   └── anomalies.txt
│
├── tests/
│   ├── test_stack.cpp
│   ├── test_queue.cpp
│   ├── test_trie.cpp
│   ├── test_graph.cpp
│   ├── test_bst.cpp
│   └── test_manager.cpp
│
└── README.md

--- 

## 🗓️ Week-by-Week Plan

| **Week** | **Focus Area** | **Modules / Deliverables** |
|-----------|----------------|-----------------------------|
| Week 1 | Core I/O and session structure | Implement `user_action`, `stack_monitor`, `queue_logger`, and `file_io` |
| Week 2 | Behavioral modeling | Develop `linked_list`, `trie_behavior`, and `graph_transition` |
| Week 3 | Fingerprint management | Implement `hash_profiles`, `bst_frequency`, and `avl_profile` |
| Week 4 | Detection & analysis | Integrate `heap_alerts`, `dynamic_edit`, and `sorting/searching` modules |
| Week 5 | Testing & reporting | Conduct complexity evaluation, optimize performance, and prepare final report |

---

## 🧾 Conclusion

This project integrates almost the complete spectrum of **Data Structures and Algorithms** to develop a **behavioral anomaly detection system in C++**.  
It is designed to be completed within **five weeks** while maintaining **academic rigor and practical feasibility**.  

The approach ensures that each DSA concept from the **CS221** course is applied in a **real-world cybersecurity context**, demonstrating both **technical depth** and **implementation capability**.

---

