#ifndef ALGORITHM_H
#define ALGORITHM_H

// ready queue, waiting queue, finished
// FCFS -> arrival
// Non-preemptive SJF -> (arrival) cpu burst
// Preemptive SJF -> arrival, cpu burst
// Non-preemptive priority -> (arrival) priority
// Preemptive priority -> arrival, priority
// Round Robin -> (arrival) time quantum, priority

/*
typedef struct eval_time {
    int pid;
    int wait;
    int turn;
} ETime;
*/

void init_bubble_sort(Process* p_org, int** pqueue, char e, int n_proc);
void queue_bubble_sort(Process* p_org, int** pqueue, char e, int t, int front, int rear);
void increase_waiting_time(int** wait_time, int* ready, int front, int rear);
int io_operation(Process** p, int** waitp, int** readyp, int* fp, int* rp, int* fp2, int* rp2);
int all_process_fin(int* finished, int n_proc);
void FCFS(Process* processes, int n_proc);
void nonpreemptive_SJF(Process* procs, int n_proc);
void preemptive_SJF(Process* procs, int n_proc);
void nonpreemptive_priority(Process* procs, int n_proc);
void preemptive_priority(Process* procs, int n_proc);
void RR(Process* procs, int n_proc, int quant);

#endif