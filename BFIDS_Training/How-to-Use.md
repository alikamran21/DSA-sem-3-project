# BFIDS Training & Monitoring Module

The **BFIDS Training Module** is the learning engine of the intrusion detection system. It analyzes raw data—both system logs and real-time physical interactions—to construct a "normal" behavioral profile (Fingerprint) for the user.

## Module Overview

This folder contains the source code for the training subsystems and the live biometric monitor:

| Subsystem | Source File | Description | Output |
|-----------|------------|-------------|--------|
| **Log Trainer** | `src/train_system.cpp` | Parses historical simulation logs, sorts them by time, and calculates statistical averages for process execution. | `fingerprints.csv` |
| **Biometric Trainer** | `src/bio_trainer.cpp` | Captures live **Keystroke Dynamics** (typing latency) and **Mouse Dynamics** (movement magnitude) to learn your behavior. | `bio_fingerprints.csv` |
| **Biometric Monitor** | `src/bio_monitor.cpp` | Compares live input against the learned profile (`bio_fingerprints.csv`) to detect anomalies. | `Console Alerts` |

---

## Setup & Compilation

### Prerequisites
* **OS:** Linux (Required for `bio_trainer` and `bio_monitor` due to `<termios.h>`).
* **Permissions:** `sudo` access is required to read hardware events from `/dev/input/`.

### 1. Identify Input Devices
You may need to know your keyboard and mouse event handlers.

```bash
cat /proc/bus/input/devices
````

  * **Keyboard:** Look for `Handlers=sysrq kbd event<X>` (e.g., `event1`).
  * **Mouse:** Look for `Handlers=mouse0 event<Y>` (e.g., `event2`).
  * **Note:** Remember these event numbers.

### 2\. Permissions to Make the script executable.

```bash
chmod +x compile.sh
```

### 3\. Running the compliation

```bash
./compile.sh
```
## Usage Guide

### Phase 1: Training Biometrics

Before the system can detect intruders, it must learn your specific typing and mouse movement patterns.

Run the trainer with `sudo`:

```bash
sudo ./bio_trainer
```

**Instructions:**

1.  **Type:** Type normally for **30 seconds**.
2.  **Move:** Move your mouse around in **circles**.
3.  **Stop:** Press `ESC` to finish training.

**Result:** A `bio_fingerprints.csv` file is generated containing your behavioral averages.

### Phase 2: Live Monitoring (Testing)

Once `bio_fingerprints.csv` exists, you can run the monitor to detect anomalies.

Run the monitor with `sudo`:

```bash
sudo ./bio_monitor
```

**Test Scenarios:**

  * **Normal Behavior:**

      * Type normally.
      * **Result:** You should see `[OK] Key match` messages.

  * **Simulate an Intruder:**

      * **Mash Keys:** Start mashing keys incredibly fast (like a script).
      * **Slow Typing:** Type painfully slow (like someone who doesn't know the layout).
      * **Violent Mouse:** Yank the mouse violently.

**Result:** The **Threat Level** will rise. If it hits **20**, the system triggers a **VM SHUTDOWN**.

```
```