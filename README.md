# CPU Scheduler

A C implementation of common CPU scheduling algorithms including First Come First Served (FCFS), Shortest Job First (SJF), and Round Robin scheduling.

## 📜 Description

This program implements three fundamental CPU scheduling algorithms:

1. **First Come First Served (FCFS)**: A non-preemptive scheduling algorithm that executes processes in the order they arrive.
2. **Shortest Job First (SJF)**: A non-preemptive scheduling algorithm that executes the process with the shortest burst time first.
3. **Round Robin (RR)**: A preemptive scheduling algorithm that uses a fixed time quantum to execute processes in a circular queue.

## 🚀 Features

- Implementation of three different scheduling algorithms
- Process management with arrival time and burst time
- Calculation of waiting time and turnaround time for each process
- Computation of average waiting time and average turnaround time
- Support for reading process data from input files
- Detailed execution timeline output
- Final statistics summary

## ⚙️ Getting Started

### Prerequisites

- GCC compiler

### Compilation

```bash
gcc -o scheduler cpu_scheduler.c
```

### Usage

The program accepts command-line arguments in the following format:

```natural
./scheduler [-f|-s|-r <quantum>] <input_file>
```

Where:
- `-f` : Run First Come First Served algorithm
- `-s` : Run Shortest Job First algorithm
- `-r <quantum>` : Run Round Robin algorithm with specified time quantum
- `<input_file>` : Path to the input file containing process data

### Input File Format

The input file should contain process information in the following format:
```
P1,6
P2,3
P3,8
```
Each line represents a process with:
- Process ID (Px where x is the process number)
- Burst time (comma-separated integer value)

## Example Usage

1. For FCFS scheduling:
```bash
./scheduler -f input.txt
```

2. For SJF scheduling:
```bash
./scheduler -s input.txt
```

3. For Round Robin scheduling with quantum = 2:
```bash
./scheduler -r 2 input.txt
```

## Output

The program provides detailed output including:
- Step-by-step execution timeline
- Process state at each time unit
- Final statistics for each process including:
  - Waiting time
  - Turnaround time
- Overall average waiting time and turnaround time

## Implementation Details

The scheduler uses the following key data structures:
- `Process` struct to store process information
- Circular queue implementation for Round Robin scheduling
- Arrays for process management and scheduling
