#ifndef MAIN_H
#define MAIN_H

typedef struct config {
    int temp;
    int exp_a;
    int exp_cpu;
    int rr_quant;
    int perc;
} Config;

void main_int();
void sel_alg_int();
void config_int();
#endif