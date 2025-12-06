#!/bin/bash

echo "--- Compiling BFIDS System for Ubuntu (SSH) ---"

# 1. Compile the Log Trainer (Historical Data)
echo "[1/3] Compiling Log Trainer..."
g++ -I include src/train_system.cpp \
    src/file_io.cpp \
    src/linked_list.cpp \
    src/avl_profile.cpp \
    src/sorting_algorithms.cpp \
    src/utils.cpp \
    src/btree_index.cpp \
    -o train_system

# 2. Compile the Biometric Trainer (Live Training)
echo "[2/3] Compiling Biometric Trainer..."
g++ -I include src/bio_trainer.cpp \
    src/avl_profile.cpp \
    src/queue_monitor.cpp \
    src/stack_monitor.cpp \
    src/utils.cpp \
    src/btree_index.cpp \
    -o bio_trainer

# 3. Compile the Biometric Monitor (Live Security)
echo "[3/3] Compiling Biometric Monitor..."
g++ -I include src/bio_monitor.cpp \
    src/avl_profile.cpp \
    src/Array_handler.cpp \
    src/pointer_utils.cpp \
    src/utils.cpp \
    src/hash_profiles.cpp \
    src/graph_transition.cpp \
    src/anomaly_heap.cpp \
    src/process_trie.cpp \
    src/btree_index.cpp \
    -o bio_monitor

echo "--- Compilation Complete! ---"
echo "Run ./bio_trainer to train and ./bio_monitor to secure."