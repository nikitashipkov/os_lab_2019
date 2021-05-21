CC=gcc


all : parallel_min_max parallel_sum process_memory

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c 

parallel_sum : parallel_sum.o
	$(CC) -o parallel_sum parallel_sum.o utils.o  -lpthread -lm

process_memory : process_memory.o
	$(CC) -o process_memory process_memory.o

utils.o : utils.h
	$(CC) utils.c -c 

find_min_max.o : utils.h find_min_max.h
	$(CC) find_min_max.c -c 

parallel_sum.o : parallel_sum.c
	$(CC) parallel_sum.c -c 
process_memory.o : process_memory.c
	$(CC) process_memory.c -c 

clean :
	rm utils.o find_min_max.o process_memory.o parallel_sum.o parallel_min_max process_memory parallel_sum