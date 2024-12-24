#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PROCESSES 6
#define MAX_MEMORY 1024
#define MAX_RESOURCES 10
#define NUM_FEEDBACK_QUEUES 3

// Process structure
typedef struct {
    int id;
    int arrival_time;
    int priority;
    int memory;
    int cpu_time;
    int resources;
    int state; // 0: pending, 1: running, 2: suspended, 3: terminated
    int remaining_time;
} Process;

// Queue structure
typedef struct {
    Process* processes[MAX_PROCESSES];
    int front, rear, count;
} Queue;

// Global variables
Process processes[MAX_PROCESSES];
Queue input_queue, real_time_queue, user_job_queue, feedback_queues[NUM_FEEDBACK_QUEUES];
int available_memory = MAX_MEMORY;
int available_resources = MAX_RESOURCES;
int dispatcher_time = 0;
int running_process_id = -1;

// Function prototypes
void initialize_queue(Queue* queue);
bool is_queue_empty(const Queue* queue);
bool is_queue_full(const Queue* queue);
void enqueue(Queue* queue, Process* process);
Process* dequeue(Queue* queue);
void initialize_processes();
bool can_allocate_resources(int memory, int resources);
void allocate_resources(Process* process);
void release_resources(Process* process);
void manage_dispatch();

void initialize_queue(Queue* queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
}

bool is_queue_empty(const Queue* queue) {
    return queue->count == 0;
}

bool is_queue_full(const Queue* queue) {
    return queue->count == MAX_PROCESSES;
}

void enqueue(Queue* queue, Process* process) {
    if (is_queue_full(queue)) return;
    queue->processes[queue->rear] = process;
    queue->rear = (queue->rear + 1) % MAX_PROCESSES;
    queue->count++;
}

Process* dequeue(Queue* queue) {
    if (is_queue_empty(queue)) return NULL;
    Process* process = queue->processes[queue->front];
    queue->front = (queue->front + 1) % MAX_PROCESSES;
    queue->count--;
    return process;
}

void initialize_processes() {
    // Direct initialization of process attributes
    int arrivals[MAX_PROCESSES] = {0, 1, 2, 3, 4, 5};
    int priorities[MAX_PROCESSES] = {0, 1, 1, 2, 2, 3};
    int memories[MAX_PROCESSES] = {100, 200, 150, 300, 250, 400};
    int cpu_times[MAX_PROCESSES] = {5, 3, 6, 2, 4, 7};
    int resources[MAX_PROCESSES] = {2, 3, 1, 2, 1, 3};

    for (int i = 0; i < MAX_PROCESSES; i++) {
        processes[i].id = i;
        processes[i].arrival_time = arrivals[i];
        processes[i].priority = priorities[i];
        processes[i].memory = memories[i];
        processes[i].cpu_time = cpu_times[i];
        processes[i].resources = resources[i];
        processes[i].state = 0; // Pending
        processes[i].remaining_time = processes[i].cpu_time;
        enqueue(&input_queue, &processes[i]);
    }
}

bool can_allocate_resources(int memory, int resources) {
    return memory <= available_memory && resources <= available_resources;
}

void allocate_resources(Process* process) {
    available_memory -= process->memory;
    available_resources -= process->resources;
}

void release_resources(Process* process) {
    available_memory += process->memory;
    available_resources += process->resources;
}

void manage_dispatch() {
    while (true) {
        bool all_terminated = true;

        // Move pending processes to appropriate queues
        while (!is_queue_empty(&input_queue)) {
            Process* process = input_queue.processes[input_queue.front];
            if (process->arrival_time > dispatcher_time) break;
            dequeue(&input_queue);
            if (process->priority == 0) {
                enqueue(&real_time_queue, process);
            } else {
                enqueue(&user_job_queue, process);
            }
        }

        // Process user job queue for allocation
        while (!is_queue_empty(&user_job_queue)) {
            Process* process = user_job_queue.processes[user_job_queue.front];
            if (!can_allocate_resources(process->memory, process->resources)) break;
            dequeue(&user_job_queue);
            allocate_resources(process);
            enqueue(&feedback_queues[0], process);
        }

        // Manage the running process
        if (running_process_id != -1) {
            Process* running_process = &processes[running_process_id];
            running_process->remaining_time--;

            if (running_process->remaining_time == 0) {
                running_process->state = 3; // Terminated
                release_resources(running_process);
                running_process_id = -1;
            } else if (running_process->priority > 0 && !is_queue_empty(&real_time_queue)) {
                running_process->state = 2; // Suspended
                enqueue(&feedback_queues[running_process->priority - 1], running_process);
                running_process_id = -1;
            }
        }

        // Assign a new process to run if none is running
        if (running_process_id == -1) {
            Process* next_process = NULL;

            if (!is_queue_empty(&real_time_queue)) {
                next_process = dequeue(&real_time_queue);
            } else {
                for (int i = 0; i < NUM_FEEDBACK_QUEUES; i++) {
                    if (!is_queue_empty(&feedback_queues[i])) {
                        next_process = dequeue(&feedback_queues[i]);
                        break;
                    }
                }
            }

            if (next_process) {
                next_process->state = 1; // Running
                running_process_id = next_process->id;
            }
        }

        // Check termination status
        for (int i = 0; i < MAX_PROCESSES; i++) {
            if (processes[i].state != 3) {
                all_terminated = false;
                break;
            }
        }

        // Display system state
        printf("Time = %d: ", dispatcher_time);
        for (int i = 0; i < MAX_PROCESSES; i++) {
            printf("P%d:", i);
            switch (processes[i].state) {
                case 0: printf(" Pending "); break;
                case 1: printf(" Running "); break;
                case 2: printf(" Suspended "); break;
                case 3: printf(" Terminated "); break;
            }
        }
        printf("\n");

        if (all_terminated) break;
        dispatcher_time++;
    }
}

int main() {
    initialize_queue(&input_queue);
    initialize_queue(&real_time_queue);
    initialize_queue(&user_job_queue);
    for (int i = 0; i < NUM_FEEDBACK_QUEUES; i++) {
        initialize_queue(&feedback_queues[i]);
    }

    initialize_processes();
    manage_dispatch();

    return 0;
}