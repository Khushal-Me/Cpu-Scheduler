// Author: Khushal Mehta
// CPU Scheduler

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROCESSES 100

typedef struct {
    int id;
    int burst_time;
    int remaining_burst;
    int arrival_time;
    int wait_time;
    int turnaround_time;
    int completion_time;
    int in_queue;  // Flag to track if process is in ready queue
} Process;

// Circular queue for Round Robin
typedef struct {
    Process* items[MAX_PROCESSES];
    int front;
    int rear;
    int size;
} Queue;

// Global variables
Process processes[MAX_PROCESSES];
int num_processes = 0;
Queue ready_queue;

// Function prototypes to fix implicit declaration warnings

// Reads input data from a file and fills the processes array
void read_input_file(const char* filename);

// Implements the First Come First Served (FCFS) scheduling
void fcfs();

// Implements the Shortest Job First (SJF) scheduling
void sjf();

// Implements Round Robin scheduling with a given time quantum
void round_robin(int quantum);

// Prints final statistics (wait time, turnaround time) for processes
void print_final_stats();

// Checks if all processes are completed
int all_processes_complete();

// Gets the next process that has arrived based on current time
Process* get_next_arrived_process(int current_time);

// Gets the process with the shortest remaining burst time
Process* get_shortest_job(int current_time);

// Updates wait time and turnaround time for all processes
void update_wait_times(int current_time, Process* current_process);

// Checks for newly arrived processes and adds them to the ready queue
void check_arrivals(int current_time);

// Resets processes' state for a new scheduling run
void reset_processes();

// Queue operations
void init_queue() {
    ready_queue.front = 0;
    ready_queue.rear = -1;
    ready_queue.size = 0;
}

void enqueue(Process* process) {
    if (!process->in_queue && process->remaining_burst > 0) {
        ready_queue.rear = (ready_queue.rear + 1) % MAX_PROCESSES;
        ready_queue.items[ready_queue.rear] = process;
        ready_queue.size++;
        process->in_queue = 1;
    }
}

Process* dequeue() {
    if (ready_queue.size <= 0) return NULL;

    Process* process = ready_queue.items[ready_queue.front];
    ready_queue.front = (ready_queue.front + 1) % MAX_PROCESSES;
    ready_queue.size--;
    process->in_queue = 0;
    return process;
}

// Check for newly arrived processes
void check_arrivals(int current_time) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time == current_time) {
            enqueue(&processes[i]);
        }
    }
}

// Read input file and populate processes array
void read_input_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        exit(1);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ",");
        if (token && token[0] == 'P') {
            processes[num_processes].id = atoi(token + 1);
            processes[num_processes].arrival_time = processes[num_processes].id;
            processes[num_processes].in_queue = 0;

            token = strtok(NULL, ",");
            if (token) {
                processes[num_processes].burst_time = atoi(token);
                processes[num_processes].remaining_burst = processes[num_processes].burst_time;
                num_processes++;
            }
        }
    }
    fclose(file);
}

// Reset process states for different algorithms
void reset_processes() {
    for (int i = 0; i < num_processes; i++) {
        processes[i].remaining_burst = processes[i].burst_time;
        processes[i].wait_time = 0;
        processes[i].turnaround_time = 0;
        processes[i].completion_time = 0;
        processes[i].in_queue = 0;
    }
}

// Check if all processes are complete
int all_processes_complete() {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].remaining_burst > 0) {
            return 0;
        }
    }
    return 1;
}

// Get next process for FCFS
Process* get_next_arrived_process(int current_time) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time <= current_time &&
            processes[i].remaining_burst > 0) {
            return &processes[i];
        }
    }
    return NULL;
}

// Get shortest remaining job
Process* get_shortest_job(int current_time) {
    Process* shortest = NULL;
    int shortest_burst = -1;

    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time <= current_time &&
            processes[i].remaining_burst > 0) {
            if (shortest == NULL ||
                processes[i].remaining_burst < shortest_burst) {
                shortest = &processes[i];
                shortest_burst = processes[i].remaining_burst;
            }
        }
    }
    return shortest;
}

// Update wait times and turnaround times for all processes
void update_wait_times(int current_time, Process* current_process) {
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].arrival_time <= current_time &&
            processes[i].remaining_burst > 0) {
            if (&processes[i] != current_process) {
                processes[i].wait_time++;
            }
            processes[i].turnaround_time++;
        }
    }
}

// First Come First Served (FCFS) scheduling algorithm
void fcfs() {
    printf("First Come First Served\n");
    int current_time = 0;  // Initialize the current time to 0

    // Continue until all processes are complete
    while (!all_processes_complete()) {
        // Get the next process that has arrived at the current time
        Process* current = get_next_arrived_process(current_time);

        // If a process has arrived, execute it
        if (current != NULL) {
            // Print process information: current time, process ID, remaining burst time, wait time, and turnaround time
            printf("T%d : P%d - Burst left %d, Wait time %d, Turnaround time %d\n",
                   current_time, current->id, current->remaining_burst,
                   current->wait_time, current->turnaround_time);

            // Decrease the remaining burst time by 1 (process execution step)
            current->remaining_burst--;

            // Update the wait times for all processes based on the current time
            update_wait_times(current_time, current);
        }
        current_time++;  // Increment time for the next cycle
    }
}

// Shortest Job First (SJF) scheduling algorithm
void sjf() {
    printf("Shortest Job First\n");
    int current_time = 0;  // Initialize the current time to 0

    // Continue until all processes are complete
    while (!all_processes_complete()) {
        // Get the process with the shortest remaining burst time at the current time
        Process* shortest = get_shortest_job(current_time);

        // If a process is found, execute it
        if (shortest != NULL) {
            // Print process information: current time, process ID, remaining burst time, wait time, and turnaround time
            printf("T%d : P%d - Burst left %d, Wait time %d, Turnaround time %d\n",
                   current_time, shortest->id, shortest->remaining_burst,
                   shortest->wait_time, shortest->turnaround_time);

            // Decrease the remaining burst time by 1 (process execution step)
            shortest->remaining_burst--;

            // Update the wait times for all processes based on the current time
            update_wait_times(current_time, shortest);
        }
        current_time++;  // Increment time for the next cycle
    }
}


// Round Robin algorithm
void round_robin(int quantum) {
    printf("Round Robin with Quantum %d\n", quantum);
    int current_time = 0;
    Process* current = NULL;
    int time_in_quantum = 0;

    init_queue();

    while (!all_processes_complete()) {
        // Check for new arrivals
        check_arrivals(current_time);

        // If no current process or quantum expired or process finished
        if (current == NULL || time_in_quantum == quantum ||
            current->remaining_burst == 0) {

            // If current process still has burst time, put it back in queue
            if (current != NULL && current->remaining_burst > 0) {
                enqueue(current);
            }

            // Get next process from queue
            current = dequeue();
            time_in_quantum = 0;
        }

        if (current != NULL) {
            printf("T%d : P%d - Burst left %d, Wait time %d, Turnaround time %d\n",
                   current_time, current->id, current->remaining_burst,
                   current->wait_time, current->turnaround_time);

            current->remaining_burst--;
            time_in_quantum++;
            update_wait_times(current_time, current);

            // If process just finished, set current to NULL to force next dequeue
            if (current->remaining_burst == 0) {
                current = NULL;
            }
        }

        current_time++;
    }
}

// Print final statistics
void print_final_stats() {
    float total_wait = 0;
    float total_turnaround = 0;

    for (int i = 0; i < num_processes; i++) {
        printf("\nP%d\n", i);
        printf("\tWaiting time:\t\t%d\n", processes[i].wait_time);
        printf("\tTurnaround time:\t\t%d\n",
               processes[i].wait_time + processes[i].burst_time);

        total_wait += processes[i].wait_time;
        total_turnaround += processes[i].wait_time + processes[i].burst_time;
    }

    printf("\nTotal average waiting time:\t%.1f\n", total_wait / num_processes);
    printf("Total average turnaround time:\t%.1f\n",
           total_turnaround / num_processes);
}

// Main function to handle user input, execute scheduling algorithms, and print results
int main(int argc, char* argv[]) {
    // Check if the correct number of arguments is provided
    if (argc < 3) {
        printf("Usage: %s [-f|-s|-r <quantum>] <input_file>\n", argv[0]);
        return 1;  // Return an error if arguments are insufficient
    }

    // Parse the algorithm choice and file name from command-line arguments
    char* algorithm = argv[1];
    char* filename;
    int quantum = 0;

    // Handle Round Robin scheduling with a time quantum
    if (strcmp(algorithm, "-r") == 0) {
        if (argc < 4) {
            printf("Error: Round Robin requires a time quantum\n");
            return 1;  // Return error if quantum is missing
        }
        quantum = atoi(argv[2]);  // Convert the quantum value from string to integer
        if (quantum <= 0) {
            printf("Error: Invalid time quantum\n");
            return 1;  // Return error if the quantum value is invalid
        }
        filename = argv[3];
    } else {
        filename = argv[2];  // Default file name for other algorithms
    }

    // Read input file to load processes
    read_input_file(filename);

    // Choose the appropriate scheduling algorithm based on user input
    if (strcmp(algorithm, "-f") == 0) {
        fcfs();  // First Come First Served scheduling
    } else if (strcmp(algorithm, "-s") == 0) {
        sjf();  // Shortest Job First scheduling
    } else if (strcmp(algorithm, "-r") == 0) {
        round_robin(quantum);  // Round Robin scheduling with the given quantum
    } else {
        printf("Error: Invalid algorithm option\n");
        return 1;  // Return error if the algorithm option is invalid
    }

    // Print final statistics after scheduling
    print_final_stats();
    return 0;  // Successful program termination
}
