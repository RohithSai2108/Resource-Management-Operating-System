# Resource Management in Operating Systems

This repository contains a C++ project that demonstrates resource management techniques used in modern operating systems. The project includes implementations of resource allocation, deadlock handling, and scheduling algorithms for effective management of system resources.

## Overview

Resource management in operating systems ensures the efficient and fair allocation of resources like CPU, memory, and I/O devices. This project explores:
- Resource allocation algorithms.
- Deadlock detection, prevention, and recovery methods.
- CPU scheduling techniques.

## Features

- **Resource Allocation**:
  - Implementation of Banker's Algorithm for safe resource allocation.
  - Simulation of safe and unsafe states in resource requests.
- **Deadlock Management**:
  - Deadlock detection and avoidance algorithms.
  - Methods to prevent or resolve deadlocks.
- **CPU Scheduling**:
  - Implementation of algorithms such as:
    - First Come First Serve (FCFS)
    - Shortest Job First (SJF)
    - Priority Scheduling
    - Round Robin (RR)

## Prerequisites

- A C++ compiler (e.g., GCC, Clang, or MSVC).
- Basic knowledge of operating systems and resource management concepts.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/RohithSai2108/Resource-Management-Operating-System.git
   cd Resource-Management-Operating-System

2. Compile the C++ files:
   ```bash
   g++ -o resource_management resource_management_code.cpp
3. Run the compiled program:
   ```bash
   ./resource_management_code
## Usage
1. Resource Allocation (Banker's Algorithm)
Test resource allocation safety with the Banker's Algorithm.

   Example:
    ```bash
    
    Input:
    Processes: 5
    Resources: 3
    Max Matrix: [...]
    Allocation Matrix: [...]
    Available Resources: [...]

    Output:
    Safe sequence: P1 -> P3 -> P4 -> P0 -> P2

2. Deadlock Management
Simulate deadlock scenarios and apply detection and prevention techniques.

3. CPU Scheduling
Simulate scheduling algorithms and view results for waiting time, turnaround time, and order of execution.

   Example:
   ```bash

   Input:
   Processes: [P1, P2, P3]
   Burst Times: [5, 8, 12]
   Time Quantum (RR): 4

   Output:
   Average Waiting Time: 5 ms
   Average Turnaround Time: 15 ms
   Execution Order: P1 -> P2 -> P3 -> P1 -> P2 -> P3

4. Future Enhancements
- Add memory management techniques (paging, segmentation).
- Extend CPU scheduling to include multilevel queues.
- Add GUI-based simulation for better visualization.




