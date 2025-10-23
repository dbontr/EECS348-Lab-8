# Makefile for EECS 348 Lab 8
CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean

all: score temp

score: score.c
	$(CC) $(CFLAGS) -o $@ $<

temp: temp.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f score temp *.o
