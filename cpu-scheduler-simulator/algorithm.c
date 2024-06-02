#include <stdio.h>
#include <string.h>
#include "options.h"
#include "generate.h"
#include "evaluation.h"
#include "algorithm.h"

void init_bubble_sort(Process* p_org, int** pqueue, char e, int n_proc) {
    int i, j;
    Process temp;
    Process p[MAX_PROCESS];
    memcpy(p, p_org, sizeof(Process)*MAX_PROCESS);

    // bubble sort
    switch (e) {
    case 'a':
        // by arrival time [min ... max]
        for (i=(n_proc-1); i>0; i--) {
            for (j=0; j<i; j++) {
                if (p[j].arrival > p[j+1].arrival) {
                    temp = p[j];
                    p[j] = p[j+1];
                    p[j+1] = temp;
                }
            }
        }
        break;
    case 'c':
        // first sort by arrival time [min ... max]
        for (i=(n_proc-1); i>0; i--) {
            for (j=0; j<i; j++) {
                if (p[j].arrival > p[j+1].arrival) {
                    temp = p[j];
                    p[j] = p[j+1];
                    p[j+1] = temp;
                    } else if (p[j].arrival == p[j+1].arrival) {
                    // then by cpu burst [min ... max]
                    if (p[j].cpu_burst > p[j+1].cpu_burst) {
                        temp = p[j];
                        p[j] = p[j+1];
                        p[j+1] = temp;
                    }
                }
            }
        }
        break;
    case 'p':
        // first sort by arrival time [min ... max]
        for (i=(n_proc-1); i>0; i--) {
            for (j=0; j<i; j++) {
                if (p[j].arrival > p[j+1].arrival) {
                    temp = p[j];
                    p[j] = p[j+1];
                    p[j+1] = temp;
                    } else if (p[j].arrival == p[j+1].arrival) {
                    // then by priority [max ... min] (larger is more important)
                    if (p[j].priority < p[j+1].priority) {
                        temp = p[j];
                        p[j] = p[j+1];
                        p[j+1] = temp;
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    // set queue
    for (i=0; i<n_proc; i++) {
        (*pqueue)[i] = p[i].pid;
    }

    return;
}

void queue_bubble_sort(Process* p_org, int** pqueue, char e, int t, int front, int rear) {
    int i, j;
    int a, b, c, d;
    int temp;
    Process p[MAX_PROCESS];
    memcpy(p, p_org, sizeof(Process)*MAX_PROCESS);

    // bubble sort
    switch (e) {
    case 'a':
        // by arrival time [min ... max]
        for (i=(rear-1); i>front; i--) {
            for (j=front; j<i; j++) {
                a = (p[(*pqueue)[j]].arrival-t > 0)? (p[(*pqueue)[j]].arrival-t) : 0;
                b = (p[(*pqueue)[j+1]].arrival-t > 0)? (p[(*pqueue)[j+1]].arrival-t) : 0;
                if (a > b) {
                    temp = (*pqueue)[j];
                    (*pqueue)[j] = (*pqueue)[j+1];
                    (*pqueue)[j+1] = temp;
                    
                }
            }
        }
        break;
    case 'c':
        // first sort by arrival time [min ... max]
        for (i=(rear-1); i>front; i--) {
            for (j=front; j<i; j++) {
                a = (p[(*pqueue)[j]].arrival-t > 0)? (p[(*pqueue)[j]].arrival-t) : 0;
                b = (p[(*pqueue)[j+1]].arrival-t > 0)? (p[(*pqueue)[j+1]].arrival-t) : 0;
                if (a > b) {
                    temp = (*pqueue)[j];
                    (*pqueue)[j] = (*pqueue)[j+1];
                    (*pqueue)[j+1] = temp;
                } else if (a == b) {
                    // then by cpu burst [min ... max]
                    c = p[(*pqueue)[j]].cpu_burst;
                    d = p[(*pqueue)[j+1]].cpu_burst;
                    if (c > d) {
                        temp = (*pqueue)[j];
                        (*pqueue)[j] = (*pqueue)[j+1];
                        (*pqueue)[j+1] = temp;
                    }
                }
            }
        }
        break;
    case 'p':
        // first sort by arrival time [min ... max]
        for (i=(rear-1); i>front; i--) {
            for (j=front; j<i; j++) {
                a = (p[(*pqueue)[j]].arrival-t > 0)? (p[(*pqueue)[j]].arrival-t) : 0;
                b = (p[(*pqueue)[j+1]].arrival-t > 0)? (p[(*pqueue)[j+1]].arrival-t) : 0;
                if (a > b) {
                    temp = (*pqueue)[j];
                    (*pqueue)[j] = (*pqueue)[j+1];
                    (*pqueue)[j+1] = temp;
                } else if (a == b) {
                    // by priority [max ... min] (larger is more important)
                    c = p[(*pqueue)[j]].priority;
                    d = p[(*pqueue)[j+1]].priority;
                    if (c < d) {
                        temp = (*pqueue)[j];
                        (*pqueue)[j] = (*pqueue)[j+1];
                        (*pqueue)[j+1] = temp;
                    }
                }
            }
        }
        break;
    default:
        break;
    }

    return;
}

void increase_waiting_time(int** wait_time, int* ready, int front, int rear) {
    int i;
    for (i=front; i<rear; i++) {
        (*wait_time)[ready[i]]++;
    }
    return;
}

int io_operation(Process** p, int** waitp, int** readyp, int* fp, int* rp, int* fp2, int* rp2) {
    int i;
    if ((*fp) != (*rp)) {
        (*p)[(*waitp)[(*fp)]].io_burst--;
        if ((*p)[(*waitp)[(*fp)]].io_burst == 0) {
            (*readyp)[(*rp2)] = (*waitp)[(*fp)];
            (*rp2)++;
            (*fp)++;
            return 1;
        }
    }
    return 0;
}

int all_process_fin(int* finished, int n_proc) {
    int i;
    for (i=0; i<n_proc; i++) {
        if (finished[i] == 0) {
            return -1;
        }
    }
    return 0;
}

void FCFS(Process* procs, int n_proc) {
    char record[MAX_CHART_SIZE];

    int ready[QUEUE_SIZE];
    int waiting[QUEUE_SIZE];

    int wait_time[MAX_PROCESS] = {0};
    int turn_time[MAX_PROCESS] = {0};
    int finished[MAX_PROCESS] = {0};
    
    int c_f, c_r, i_f, i_r;
    int t = 0;

    Process processes[MAX_PROCESS];
    memcpy(processes, procs, sizeof(Process)*MAX_PROCESS);

    c_f = 0;
    c_r = n_proc;
    i_f = 0;
    i_r = 0;

    printf(" < First Come First Served >\n");

    // initial sort (ready queue) by arrival time
    init_bubble_sort(processes, &(int*){ready}, 'a', n_proc);

    while (1) {
        if ((c_f == c_r) || (processes[ready[c_f]].arrival > t)) {
            // record: no process
            record[t] = '#';

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, c_f, c_r);

            /* decrease io burst time for the process in io device */
            if (io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r) == 1) {
                // if io -> ready, sort ready queue again
                queue_bubble_sort(processes, &(int*){ready}, 'a', t, c_f, c_r);
            }
            t++;
            continue;
        }
        

        while (processes[ready[c_f]].cpu_burst > 0) { // run process
            // record process
            record[t] = ready[c_f] + '0' + 1;        
            processes[ready[c_f]].cpu_burst--;

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, (c_f+1), c_r);

            /* decrease io burst time for all processes in waiting queue */
            if (io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r) == 1) {
                // if io -> ready, sort ready queue again (except current executing process)
                queue_bubble_sort(processes, &(int*){ready}, 'a', t, (c_f+1), c_r);
            }
            
            t++;
            
            // io burst occurs in a fixed manner (after 1ms in cpu)
            if (processes[ready[c_f]].io_burst > 0) { 
                // put current process to waiting queue
                waiting[i_r++] = ready[c_f];
                break;
            }
            
        }

        if ((processes[ready[c_f]].cpu_burst == 0)) {
            finished[ready[c_f]] = 1;
        }

        if (all_process_fin(finished, n_proc) == 0) {
                break;
        }

        c_f++;
    }

    record[t] = '\n';

    // waiting / turnaround time for each process
    for (int i=0; i<n_proc; i++) {
        wait_time[i] -= procs[i].arrival;
        turn_time[i] = wait_time[i] + procs[i].cpu_burst;
    }

    // in wait_time, need to subtract arrival time (waiting time = wait_time - arrival)
    // after that, add cpu burst to get turnaround time (turnaround time = waiting + cpu burst)

    gantt_chart(record);
    eval_stat(record, wait_time, turn_time, n_proc);

    return;
}

void nonpreemptive_SJF(Process* procs, int n_proc) {
    char record[MAX_CHART_SIZE];

    int ready[QUEUE_SIZE];
    int waiting[QUEUE_SIZE];

    int wait_time[MAX_PROCESS] = {0};
    int turn_time[MAX_PROCESS] = {0};
    int finished[MAX_PROCESS] = {0};
    
    int c_f, c_r, i_f, i_r;
    int t = 0;

    Process processes[MAX_PROCESS];
    memcpy(processes, procs, sizeof(Process)*MAX_PROCESS);

    c_f = 0;
    c_r = n_proc;
    i_f = 0;
    i_r = 0;

    printf(" < Non-preemptive Shortest Job First >\n");

    // initial sort (ready queue) by arrival time
    init_bubble_sort(processes, &(int*){ready}, 'c', n_proc);

    while (1) {
        if ((c_f == c_r) || (processes[ready[c_f]].arrival > t)) {
            // record: no process
            record[t] = '#';

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, c_f, c_r);

            /* decrease io burst time for the process in io device */
            io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r);
            
            t++;

            // sort after idle time is over
            queue_bubble_sort(processes, &(int*){ready}, 'c', t, c_f, c_r);
            continue;
        }
        

        while (processes[ready[c_f]].cpu_burst > 0) { // run process
            // record process
            record[t] = ready[c_f] + '0' + 1;        
            processes[ready[c_f]].cpu_burst--;

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, (c_f+1), c_r);

            /* decrease io burst time for all processes in waiting queue */
            io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r);

            t++;
            
            // io burst occurs in a fixed manner (after 1ms in cpu)
            if (processes[ready[c_f]].io_burst > 0) { 
                // put current process to waiting queue
                waiting[i_r++] = ready[c_f];
                break;
            }
            
        }

        if ((processes[ready[c_f]].cpu_burst == 0)) {
            finished[ready[c_f]] = 1;
        }

        if (all_process_fin(finished, n_proc) == 0) {
                break;
        }

        // sort after a process is terminated
        c_f++;
        queue_bubble_sort(processes, &(int*){ready}, 'c', t, c_f, c_r);
    }

    record[t] = '\n';

    // waiting / turnaround time for each process
    for (int i=0; i<n_proc; i++) {
        wait_time[i] -= procs[i].arrival;
        turn_time[i] = wait_time[i] + procs[i].cpu_burst;
    }

    // in wait_time, need to subtract arrival time (waiting time = wait_time - arrival)
    // after that, add cpu burst to get turnaround time (turnaround time = waiting + cpu burst)

    gantt_chart(record);
    eval_stat(record, wait_time, turn_time, n_proc);

    return;
}

void preemptive_SJF(Process* procs, int n_proc) {
    char record[MAX_CHART_SIZE];

    int ready[QUEUE_SIZE];
    int waiting[QUEUE_SIZE];

    int wait_time[MAX_PROCESS] = {0};
    int turn_time[MAX_PROCESS] = {0};
    int finished[MAX_PROCESS] = {0};
    
    int c_f, c_r, i_f, i_r;
    int t = 0;

    Process processes[MAX_PROCESS];
    memcpy(processes, procs, sizeof(Process)*MAX_PROCESS);

    c_f = 0;
    c_r = n_proc;
    i_f = 0;
    i_r = 0;

    printf(" < Preemptive Shortest Job First >\n");

    // initial sort (ready queue) by arrival time
    init_bubble_sort(processes, &(int*){ready}, 'c', n_proc);

    while (1) {
        if ((c_f == c_r) || (processes[ready[c_f]].arrival > t)) {
            // record: no process
            record[t] = '#';

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, c_f, c_r);

            /* decrease io burst time for the process in io device */
            io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r);
            
            t++;

            // sort after idle process is over
            queue_bubble_sort(processes, &(int*){ready}, 'c', t, c_f, c_r);
            continue;
        }
        

        while (processes[ready[c_f]].cpu_burst > 0) { // run process
            // sort processes for each time
            queue_bubble_sort(processes, &(int*){ready}, 'c', t, c_f, c_r);

            // record process
            record[t] = ready[c_f] + '0' + 1;        
            processes[ready[c_f]].cpu_burst--;

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, (c_f+1), c_r);

            /* decrease io burst time for all processes in waiting queue */
            io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r);

            t++;
            
            // io burst occurs in a fixed manner (after 1ms in cpu)
            if (processes[ready[c_f]].io_burst > 0) { 
                // put current process to waiting queue
                waiting[i_r++] = ready[c_f];
                break;
            }
        }

        if ((processes[ready[c_f]].cpu_burst == 0)) {
            finished[ready[c_f]] = 1;
        }

        if (all_process_fin(finished, n_proc) == 0) {
                break;
        }

        // sort after a process is terminated
        c_f++;
        queue_bubble_sort(processes, &(int*){ready}, 'c', t, c_f, c_r);
    }

    record[t] = '\n';

    // waiting / turnaround time for each process
    for (int i=0; i<n_proc; i++) {
        wait_time[i] -= procs[i].arrival;
        turn_time[i] = wait_time[i] + procs[i].cpu_burst;
    }

    // in wait_time, need to subtract arrival time (waiting time = wait_time - arrival)
    // after that, add cpu burst to get turnaround time (turnaround time = waiting + cpu burst)

    gantt_chart(record);
    eval_stat(record, wait_time, turn_time, n_proc);

    return;
}

void nonpreemptive_priority(Process* procs, int n_proc) {
    char record[MAX_CHART_SIZE];

    int ready[QUEUE_SIZE];
    int waiting[QUEUE_SIZE];

    int wait_time[MAX_PROCESS] = {0};
    int turn_time[MAX_PROCESS] = {0};
    int finished[MAX_PROCESS] = {0};
    
    int c_f, c_r, i_f, i_r;
    int t = 0;

    Process processes[MAX_PROCESS];
    memcpy(processes, procs, sizeof(Process)*MAX_PROCESS);

    c_f = 0;
    c_r = n_proc;
    i_f = 0;
    i_r = 0;

    printf(" < Non-preemptive Priority >\n");

    // initial sort (ready queue) by arrival time
    init_bubble_sort(processes, &(int*){ready}, 'p', n_proc);

    while (1) {
        if ((c_f == c_r) || (processes[ready[c_f]].arrival > t)) {
            // record: no process
            record[t] = '#';

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, c_f, c_r);

            /* decrease io burst time for the process in io device */
            io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r);
            
            t++;

            // sort after idle time is over
            queue_bubble_sort(processes, &(int*){ready}, 'p', t, c_f, c_r);
            continue;
        }
        

        while (processes[ready[c_f]].cpu_burst > 0) { // run process
            // record process
            record[t] = ready[c_f] + '0' + 1;        
            processes[ready[c_f]].cpu_burst--;

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, (c_f+1), c_r);

            /* decrease io burst time for all processes in waiting queue */
            io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r);

            t++;
            
            // io burst occurs in a fixed manner (after 1ms in cpu)
            if (processes[ready[c_f]].io_burst > 0) { 
                // put current process to waiting queue
                waiting[i_r++] = ready[c_f];
                break;
            }
            
        }

        if ((processes[ready[c_f]].cpu_burst == 0)) {
            finished[ready[c_f]] = 1;
        }

        if (all_process_fin(finished, n_proc) == 0) {
                break;
        }

        // sort after a process is terminated
        c_f++;
        queue_bubble_sort(processes, &(int*){ready}, 'p', t, c_f, c_r);
    }

    record[t] = '\n';

    // waiting / turnaround time for each process
    for (int i=0; i<n_proc; i++) {
        wait_time[i] -= procs[i].arrival;
        turn_time[i] = wait_time[i] + procs[i].cpu_burst;
    }

    // in wait_time, need to subtract arrival time (waiting time = wait_time - arrival)
    // after that, add cpu burst to get turnaround time (turnaround time = waiting + cpu burst)

    gantt_chart(record);
    eval_stat(record, wait_time, turn_time, n_proc);

    return;
}

void preemptive_priority(Process* procs, int n_proc) {
    char record[MAX_CHART_SIZE];

    int ready[QUEUE_SIZE];
    int waiting[QUEUE_SIZE];

    int wait_time[MAX_PROCESS] = {0};
    int turn_time[MAX_PROCESS] = {0};
    int finished[MAX_PROCESS] = {0};
    
    int c_f, c_r, i_f, i_r;
    int t = 0;

    Process processes[MAX_PROCESS];
    memcpy(processes, procs, sizeof(Process)*MAX_PROCESS);

    c_f = 0;
    c_r = n_proc;
    i_f = 0;
    i_r = 0;

    printf(" < Preemptive Priority >\n");

    // initial sort (ready queue) by arrival time
    init_bubble_sort(processes, &(int*){ready}, 'p', n_proc);

    while (1) {
        if ((c_f == c_r) || (processes[ready[c_f]].arrival > t)) {
            // record: no process
            record[t] = '#';

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, c_f, c_r);

            /* decrease io burst time for the process in io device */
            io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r);
            
            t++;

            // sort after idle process is over
            queue_bubble_sort(processes, &(int*){ready}, 'p', t, c_f, c_r);
            continue;
        }
        

        while (processes[ready[c_f]].cpu_burst > 0) { // run process
            // sort processes for each time
            queue_bubble_sort(processes, &(int*){ready}, 'p', t, c_f, c_r);
            
            // record process
            record[t] = ready[c_f] + '0' + 1;        
            processes[ready[c_f]].cpu_burst--;

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, (c_f+1), c_r);

            /* decrease io burst time for all processes in waiting queue */
            io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r);

            t++;
            
            // io burst occurs in a fixed manner (after 1ms in cpu)
            if (processes[ready[c_f]].io_burst > 0) { 
                // put current process to waiting queue
                waiting[i_r++] = ready[c_f];
                break;
            }
        }

        if ((processes[ready[c_f]].cpu_burst == 0)) {
            finished[ready[c_f]] = 1;
        }

        if (all_process_fin(finished, n_proc) == 0) {
                break;
        }

        // sort after a process is terminated
        c_f++;
        queue_bubble_sort(processes, &(int*){ready}, 'p', t, c_f, c_r);
    }

    record[t] = '\n';

    // waiting / turnaround time for each process
    for (int i=0; i<n_proc; i++) {
        wait_time[i] -= procs[i].arrival;
        turn_time[i] = wait_time[i] + procs[i].cpu_burst;
    }

    // in wait_time, need to subtract arrival time (waiting time = wait_time - arrival)
    // after that, add cpu burst to get turnaround time (turnaround time = waiting + cpu burst)

    gantt_chart(record);
    eval_stat(record, wait_time, turn_time, n_proc);

    return;
}

void RR(Process* procs, int n_proc, int quant) {
    char record[MAX_CHART_SIZE];

    int ready[QUEUE_SIZE];
    int waiting[QUEUE_SIZE];

    int wait_time[MAX_PROCESS] = {0};
    int turn_time[MAX_PROCESS] = {0};
    int finished[MAX_PROCESS] = {0};
    
    int c_f, c_r, i_f, i_r;
    int t = 0;
    int i;

    Process processes[MAX_PROCESS];
    memcpy(processes, procs, sizeof(Process)*MAX_PROCESS);

    c_f = 0;
    c_r = n_proc;
    i_f = 0;
    i_r = 0;

    printf(" < Round Robin: time quantum = %d >\n", quant);

    // initial sort (ready queue) by arrival time
    init_bubble_sort(processes, &(int*){ready}, 'a', n_proc);

    while (1) {
        if ((c_f == c_r) || (processes[ready[c_f]].arrival > t)) {
            // record: no process
            record[t] = '#';

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, c_f, c_r);

            /* decrease io burst time for the process in io device */
            if (io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r) == 1) {
                // if io -> ready, sort ready queue again
                queue_bubble_sort(processes, &(int*){ready}, 'a', t, c_f, c_r);
            }
            t++;
            continue;
        }

        for (i=0; i<quant; i++) { // run process
            // cpu burst < time quant
            if (processes[ready[c_f]].cpu_burst == 0) {
                break;
            }

            // record process
            record[t] = ready[c_f] + '0' + 1;        
            processes[ready[c_f]].cpu_burst--;

            /* increase waiting time for all processes in ready queue */
            increase_waiting_time(&(int*){wait_time}, ready, (c_f+1), c_r);

            /* decrease io burst time for all processes in waiting queue */
            /* decrease io burst time for all processes in waiting queue */
            if (io_operation(&(Process*){processes}, &(int*){waiting}, &(int*){ready}, &i_f, &i_r, &c_f, &c_r) == 1) {
                // if io -> ready, sort ready queue again (except current executing process)
                queue_bubble_sort(processes, &(int*){ready}, 'a', t, (c_f+1), c_r);
            }
            
            t++;
            
            // io burst occurs in a fixed manner (after 1ms in cpu)
            if (processes[ready[c_f]].io_burst > 0) { 
                // put current process to waiting queue
                waiting[i_r++] = ready[c_f];
                break;
            }
        }
        
        if ((processes[ready[c_f]].cpu_burst == 0)) {
            finished[ready[c_f]] = 1;
        } else if (i == quant) {
            // process sent to ready queue since time quantum is expired
            // -> located at the end of the queue
            ready[c_r++] = ready[c_f];
            queue_bubble_sort(processes, &(int*){ready}, 'a', t, c_f, c_r);
        }

        if (all_process_fin(finished, n_proc) == 0) {
                break;
        }

        c_f++;
    }

    record[t] = '\n';

    // waiting / turnaround time for each process
    for (int i=0; i<n_proc; i++) {
        wait_time[i] -= procs[i].arrival;
        turn_time[i] = wait_time[i] + procs[i].cpu_burst;
    }

    // in wait_time, need to subtract arrival time (waiting time = wait_time - arrival)
    // after that, add cpu burst to get turnaround time (turnaround time = waiting + cpu burst)

    gantt_chart_RR(record, quant);
    eval_stat(record, wait_time, turn_time, n_proc);

    return;
}