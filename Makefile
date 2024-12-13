CC = gcc                 
CFLAGS = -Wall -g        
OBJ = mem_alloc.o  test.o  mem_alloc_2.0.o perf.o perf_2.0.o perf.exe perf_2.0.exe test.exe
//TARGET = test_program   

all : mem_alloc.o mem_alloc_2.0.o test.o perf.o perf_2.0.o

test : test.o
	gcc test.o mem_alloc.o -o test.exe
	./test.exe

perf : perf.o
	gcc perf.o mem_alloc.o -o perf.exe
	./perf.exe

perf_2.0 : perf_2.0.o
	gcc perf_2.0.o mem_alloc_2.0.o -o perf_2.0.exe
	./perf_2.0.exe

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(OBJ)

mem_alloc.o : mem_alloc.c mem_alloc.h
	$(CC) $(CFLAGS) -c mem_alloc.c

mem_alloc_2.0.o : mem_alloc_2.0.c mem_alloc_2.0.h
	$(CC) $(CFLAGS) -c mem_alloc_2.0.c

test.o : mem_alloc.c test.c mem_alloc.h
	$(CC) $(CFLAGS) -c test.c

perf.o : mem_alloc.c mem_alloc.h
	$(CC) $(CFLAGS) -c perf.c

perf_2.0.o : mem_alloc_2.0.c mem_alloc_2.0.h
	$(CC) $(CFLAGS) -c perf_2.0.c

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean
