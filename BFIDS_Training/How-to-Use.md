#  BFIDS Training & Monitoring Module

The **BFIDS Training Module** is the learning engine of the intrusion detection system. It analyzes raw data—both system logs and real-time physical interactions—to construct a "normal" behavioral profile (Fingerprint) for the user.

##  Module Overview

This folder contains the source code for the training subsystems and the live biometric monitor:

| Subsystem | Source File | Description | Output |
|-----------|------------|-------------|--------|
| **Log Trainer** | `src/train_system.cpp` | Parses historical simulation logs, sorts them by time, and calculates statistical averages for process execution. | `fingerprints.csv` |
| **Biometric Trainer** | `src/bio_trainer.cpp` | Captures live **Keystroke Dynamics** (typing latency) and **Mouse Dynamics** (movement magnitude) to learn your behavior. | `bio_fingerprints.csv` |
| **Biometric Monitor** | `src/bio_monitor.cpp` | Compares live input against the learned profile (`bio_fingerprints.csv`) to detect anomalies. | `Console Alerts` |

---

##  Setup & Compilation

### Prerequisites
* **OS:** Linux (Required for `bio_trainer` and `bio_monitor` due to `<linux/input.h>`).
* **Permissions:** `sudo` access is required to read hardware events from `/dev/input/`.

### 1. Compile the Log Trainer (System Activity)
This system utilizes the core DSA libraries (Linked List, Merge Sort, AVL Tree).

```bash
g++ src/train_system.cpp \
    src/file_io.cpp \
    src/linked_list.cpp \
    src/avl_profile.cpp \
    src/sorting_algorithms.cpp \
    -o train_system
````

### 2. Compile the Biometric Tools (Hardware Activity)

This part compiles the biometric trainer and monitor, which interact directly with hardware interfaces and require the AVL profile logic.

#### Compile Trainer

```bash
g++ src/bio_trainer.cpp src/avl_profile.cpp -o bio_trainer
```

#### Compile Monitor

```bash
g++ src/bio_monitor.cpp src/avl_profile.cpp -o bio_monitor
```

---

## 🚀 Usage Guide

### Phase 1: Training Biometrics

Before the system can detect intruders, it must learn your specific typing and mouse movement patterns.

Run the trainer with `sudo`:

```bash
sudo ./bio_trainer
```

**Interact:**

* **Type:** Type normally on the keyboard (e.g., write a sentence).
* **Move:** Move the mouse around the screen.
* **Stop:** Press `ESC` to finish training.

**Result:** A `bio_fingerprints.csv` file is generated containing your behavioral averages.

### Phase 2: Live Monitoring (Testing)

Once `bio_fingerprints.csv` exists, you can run the monitor to detect anomalies.

Run the monitor with `sudo`:

```bash
sudo ./bio_monitor
```

**Test Scenarios:**

* **Normal Behavior:** Type and move normally. You should see green `[OK]` messages.
* **Simulate Attack:** Mash keys very fast or type extremely slowly (e.g., 1 key per second).

**Result:** The anomaly score will rise. If it exceeds the threshold, the system triggers a **SHUTDOWN/LOCKOUT** alert.
