#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "options.h"
#include "main.h"
#include "generate.h"
#include "algorithm.h"
#include "evaluation.h"

int main(int argc, char* argv[]) {
    Process processes[MAX_PROCESS];
    Config set = {0};
    int n_process;
    char input = '0';
    char cf = '0';
    char alg = '0';

    n_process = atoi(argv[1]);

    if ((n_process > MAX_PROCESS) || (n_process < MIN_PROCESS)) {
        printf("Invalid arguments!\n\n");
        return -1;
    } 

    // main function
    system("clear");
    srand(time(NULL));
    set.rr_quant = DEFAULT_TIME_QUANT;
    
    do {
        // main interface
        main_int();
        printf("Select: ");
        scanf("%c", &input);
        while ((getchar()) != '\n');

        printf("\n");
        if (input == '1') {
            // generate processes
            generate_processes(&(Process*){processes}, n_process, set.exp_a, set.exp_cpu, set.perc);
            printf("Random processes generated!\n");
            sleep(1);
            system("clear");

            // select algorithm
            do {
                sel_alg_int();
                printf("Select: ");
                scanf("%c", &alg);
                while ((getchar()) != '\n');

                printf("\n");
                switch (alg) {
                case '1':
                    // FCFS
                    FCFS(processes, n_process);
                    printf("Press [Enter] to go back... ");
                    while ((getchar()) != '\n');
                    break;
                case '2':
                    // Non-preemptive SJF
                    nonpreemptive_SJF(processes, n_process);
                    printf("Press [Enter] to go back... ");
                    while ((getchar()) != '\n');
                    break;
                case '3':
                    // Preemptive SJF
                    preemptive_SJF(processes, n_process);
                    printf("Press [Enter] to go back... ");
                    while ((getchar()) != '\n');
                    break;
                case '4':
                    // Non-preemptive priority
                    nonpreemptive_priority(processes, n_process);
                    printf("Press [Enter] to go back... ");
                    while ((getchar()) != '\n');
                    break;
                case '5':
                    // Preemptive priority
                    preemptive_priority(processes, n_process);
                    printf("Press [Enter] to go back... ");
                    while ((getchar()) != '\n');
                    break;
                case '6':
                    // Round Robin
                    /* code implementation */
                    RR(processes, n_process, set.rr_quant);
                    printf("Press [Enter] to go back... ");
                    while ((getchar()) != '\n');
                    break;
                case '7':
                    // see generated processes
                    printf(" # of processes: %d\n\n", n_process);
                    show_processes(processes, n_process);
                    printf("Press [Enter] to go back... ");
                    while ((getchar()) != '\n');
                    break;
                case '8':
                    break;
                default:
                    printf("Wrong input!\n\n");
                    sleep(1);
                    break;
                }
                system("clear");
            } while (alg != '8');
            
        } else if (input == '2') {
            // go to config
            system("clear");

            do {
                set.temp = -1;
                config_int();
                printf("Select: ");
                scanf("%c", &cf);
                while ((getchar()) != '\n');

                printf("\n");
                switch (cf) {
                case '1':
                    // distribution of arrival time
                    printf("Which distribution to use for arrival time? (current: %d)\n\n", set.exp_a);
                    printf(" 0. Uniform (default)\n 1. Exponential (λ=0.1)\n");
                    printf(" 2. Exponential (λ=0.3)\n 3. Exponential (λ=0.5)\n");
                    printf(" 4. All zeros\n\n");
                    printf("Select: ");
                    scanf("%d", &set.temp);
                    while ((getchar()) != '\n');
                    if ((set.temp >= 0) && (set.temp <= 4)) {
                        set.exp_a = set.temp;
                        printf("Option %d selected!\n", set.exp_a);
                    } else {
                        printf("Wrong input! Go back to Configuration...\n\n");
                    }
                    break;
                case '2':
                    // distribution of CPU burst time
                    printf("Which distribution to use for cpu burst time? (current: %d)\n\n", set.exp_cpu);
                    printf(" 0. Uniform (default)\n 1. Exponential (λ=0.1)\n");
                    printf(" 2. Exponential (λ=0.3)\n 3. Exponential (λ=0.5)\n\n");
                    printf("Select: ");
                    scanf("%d", &set.temp);
                    while ((getchar()) != '\n');
                    if ((set.temp >= 0) && (set.temp <= 3)) {
                        set.exp_cpu = set.temp;
                        printf("Option %d selected!\n", set.exp_cpu);
                    } else {
                        printf("Wrong input! Go back to Configuration...\n\n");
                    }
                    break;
                case '3':
                    // probability of having IO burst
                    printf("Probability of having IO burst in percentage (current: %d%%)\n\n", set.perc);
                    printf("Enter in percentage [0-100]: ");
                    scanf("%d", &set.temp);
                    while ((getchar()) != '\n');
                    if ((set.temp >= 0) && (set.temp <= 100)) {
                        set.perc = set.temp;
                        printf("Percentage changed to %d%%!\n", set.perc);
                    } else {
                        printf("Wrong input! Go back to Configuration...\n\n");
                    }
                    break;
                case '4':
                    // Round Robin time quantum
                    // need to be used in RR!!!
                    printf("Time quantum in Round Robin algorithm (current: %d)\n\n", set.rr_quant);
                    printf("Enter the time quantum [1-%d]: ", MAX_TIME_QUANT);
                    scanf("%d", &set.temp);
                    while ((getchar()) != '\n');
                    if ((set.temp >= 1) && (set.temp <= MAX_TIME_QUANT)) {
                        set.rr_quant = set.temp;
                        printf("Time quantum changed to %d!\n", set.rr_quant);
                    } else {
                        printf("Wrong input! Go back to Configuration...\n\n");
                    }
                    break;
                case '5':
                    break;
                default:
                    printf("Wrong input!\n\n");
                    break;
                }
                if (cf != '5') {
                    sleep(1);
                }
                system("clear");
            } while (cf != '5');

        } else if (input != '3') {
            printf("Wrong input!\n\n");
            sleep(1);
        }
        
        if (input != '3') {
            system("clear");
        }
    } while (input != '3');
    return 0;
}

void main_int() {
    printf("======================================\n\n");
    printf("\tCPU scheduler simulator\n\n");
    printf("======================================\n\n");
    printf(" 1. Start Simulation\n");
    printf(" 2. Configuration\n");
    printf(" 3. Quit\n\n");
    return;
}

void sel_alg_int() {
    printf("======================================\n\n");
    printf("     Select scheduling algorithm\n\n");
    printf("======================================\n\n");
    printf(" 1. FCFS\n");
    printf(" 2. Non-preemptive SJF\n");
    printf(" 3. Preemptive SJF\n");
    printf(" 4. Non-preemptive priority\n");
    printf(" 5. Preemptive priority\n");
    printf(" 6. Round Robin\n\n");
    printf(" 7. See generated processes\n");
    printf(" 8. Go back to menu\n\n");
    return;
}

void config_int() {
    printf("======================================\n\n");
    printf("\t    Configuration\n\n");
    printf("======================================\n\n");
    printf(" 1. Distribution of arrival time\n");
    printf(" 2. Distribution of CPU burst time\n");
    printf(" 3. Probability of having IO burst\n");
    printf(" 4. Round Robin time quantum\n\n");
    printf(" 5. Go back to menu\n\n");
    return;
}