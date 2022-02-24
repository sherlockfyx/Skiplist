CC=g++  
CXXFLAGS = --std=c++11 -O0 -Wall -g -D_PTHREADS
CCLINK=-lpthread
skiplist: main.o 
	$(CC) $(CXXFLAGS) -o ./bin/main main.o $(CCLINK)
	rm -f ./*.o


clean: 
	rm -f ./*.o