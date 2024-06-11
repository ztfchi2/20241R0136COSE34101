#ifndef EVALUATION_H
#define EVALUATION_H

void gantt_chart(char* rec);
void gantt_chart_RR(char* rec, int quant);
void eval_stat(char* rec, int* wait_time, int* turn_time, int n_proc);

#endif