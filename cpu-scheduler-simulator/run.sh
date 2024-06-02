#!/bin/sh

gcc -c algorithm.c
gcc -c evaluation.c
gcc -c generate.c
gcc -c main.c

gcc main.o generate.o evaluation.o algorithm.o -o sim -lm