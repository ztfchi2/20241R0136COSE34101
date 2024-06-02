#ifndef GENERATE_H
#define GENERATE_H

typedef struct process {
    int pid;
    int arrival;
    int cpu_burst;
    int io_burst;
    int priority;
} Process;

int rand_range(int min, int max);
int exp_rand(int min, int max, int ind);
void generate_processes(Process** pprocesses, int n_proc, int exp_a, int exp_cpu, int perc);
void show_processes(Process* processes, int n_proc);
#endif