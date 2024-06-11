#include <stdio.h>
#include "options.h"
#include "generate.h"
#include "evaluation.h"

void gantt_chart(char* rec) {
    int labels[QUEUE_SIZE];
    int i = 0;
    int j = 0;
    int previous = 0;
    
    // gantt chart
    printf("| ");
    while (rec[i] != '\n') {
        printf("%c", rec[i]);
        if (rec[i] != rec[i+1]) {
            printf(" | ");
            labels[j] = i + 1;
            j++;
        }
        i++;
    }
    labels[j] = -1;
    printf("\n");
    
    j = 0;
    printf("0");
    while (labels[j] != -1) {
        for (i=previous; i<labels[j]; i++) {
            printf(" ");
        }
        if ((labels[j] > 0) && (labels[j] < 10)) {
            printf(" ");
        }
        printf(" ");
        printf("%d", labels[j]);
        previous = labels[j];
        j++;
    }
    printf("\n");
    
    return;
}

void gantt_chart_RR(char* rec, int quant) {
    int labels[QUEUE_SIZE];
    int i = 0;
    int j = 0;
    int k = 0;
    int previous = 0;
    
    // gantt chart
    printf("| ");
    while (rec[i] != '\n') {
        printf("%c", rec[i]);
        if (rec[i] == rec[i+1]) {
            k++;
            if ((rec[i] != '#') && (k == quant)) {
                printf(" : ");
                labels[j] = i + 1;
                j++;
                k = 0;
            }
        } else {
            printf(" | ");
            labels[j] = i + 1;
            j++;
            k = 0;
        }
        i++;
    }
    labels[j] = -1;
    printf("\n");
    
    j = 0;
    printf("0");
    while (labels[j] != -1) {
        for (i=previous; i<labels[j]; i++) {
            printf(" ");
        }
        if ((labels[j] > 0) && (labels[j] < 10)) {
            printf(" ");
        }
        printf(" ");
        printf("%d", labels[j]);
        previous = labels[j];
        j++;
    }
    printf("\n");
    
    return;
}

void eval_stat(char* rec, int* wait_time, int* turn_time, int n_proc) {
    int i, total_rec;
    int sum_wait, sum_turn, cnt_idle;
    double avg_wait, avg_turn, cpu_util;
    
    sum_wait = 0;
    sum_turn = 0;
    for (i=0; i<n_proc; i++) {
        sum_wait += wait_time[i];
        sum_turn += turn_time[i];
    }

    // average waiting time
    avg_wait = (double) sum_wait/n_proc;

    // average turnaround time
    avg_turn = (double) sum_turn/n_proc;

    // cpu utilization rate
    total_rec = 0;
    cnt_idle = 0;
    i=0;
    while (rec[i] != '\n') {
        if (rec[i] == '#') {
            cnt_idle++;
        }
        total_rec++;
        i++;
    }
    cpu_util = (double) (100 * (total_rec - cnt_idle)/(total_rec));

    // print evaluation statistics
    printf("---+---------+-----------\n");
    printf(" # | waiting | turnaround\n");
    printf("---+---------+-----------\n");
    for (i=0; i<n_proc; i++) {
        printf(" %d |    %02d   |     %02d\n", (i+1), wait_time[i], turn_time[i]);
    }
    printf("---+---------+-----------\n");
    printf("avg|   %04.1f  |    %04.1f\n", avg_wait, avg_turn);
    printf("---+---------+-----------\n");
    printf("CPU utilization:  %.1f%%\n", cpu_util);
    printf("-------------------------\n\n");

    return;
}