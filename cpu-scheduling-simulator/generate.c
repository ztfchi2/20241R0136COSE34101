#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "options.h"
#include "generate.h"

int rand_range(int min, int max) {
    return (rand() % (max - min + 1) + min);
}

// exponential dist random here
int exp_rand(int min, int max, int ind) {
    float num, exp_out, lambda;
    int out;
    
    // set rate parameter lambda
    if (ind == 1) {
        lambda = 0.1;
    } else if (ind == 2) {
        lambda = 0.3;
    } else {
        lambda = 0.5;
    }
    
    // sample from exponential distribution
    // by inverse cdf method
    // kind of truncated exponential
    do {
        num = (float)rand()/(float)(RAND_MAX);
        if (num == 1) {
            continue;
        }
        exp_out = (float)(-log(1-num)/lambda);
        out = (int)exp_out;
    } while ((out < min) || (out > max));

    return out;
}

void generate_processes(Process** pprocesses, int n_proc, int exp_a, int exp_cpu, int perc) {
    // default: uniform distribution
    for (int i=0; i<n_proc; i++) {
        int choose = rand_range(1, 100);

        // randomly generate processes
        (*pprocesses)[i].pid = i;

        if (exp_a == 0) {
            (*pprocesses)[i].arrival = rand_range(MIN_ARRIVAL, MAX_ARRIVAL);
        } else if (exp_a == 4) {
            (*pprocesses)[i].arrival = 0;
        } else {
            (*pprocesses)[i].arrival = exp_rand(MIN_ARRIVAL, MAX_ARRIVAL, exp_a);
        }

        if (exp_cpu == 0) {
            (*pprocesses)[i].cpu_burst = rand_range(MIN_CPU_BURST, MAX_CPU_BURST);
        } else {
            (*pprocesses)[i].cpu_burst = exp_rand(MIN_CPU_BURST, MAX_CPU_BURST, exp_cpu);
        }

        (*pprocesses)[i].io_burst = rand_range(MIN_IO_BURST, MAX_IO_BURST);
        if (choose > perc) {
            (*pprocesses)[i].io_burst = 0;
        }
        
        (*pprocesses)[i].priority = rand_range(1, n_proc);
    }

}

void show_processes(Process* processes, int n_proc) {
    // print processes
    printf("---+---------+-----------+----------+---------\n");
    printf(" # | arrival | cpu burst | io burst | priority\n");
    printf("---+---------+-----------+----------+---------\n");
    for (int i=0; i<n_proc; i++) {
        printf(" %d |", (processes[i].pid+1));
        printf("   %02d    |", processes[i].arrival);
        printf("    %02d     |", processes[i].cpu_burst);
        printf("    %02d    |", processes[i].io_burst);
        printf("    %d    \n", processes[i].priority);
    }
    printf("---+---------+-----------+----------+---------\n\n");
    return;
}