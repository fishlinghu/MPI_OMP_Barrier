OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = g++
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

centralized_barrier: barrier_test.cpp centralized_barrier.cpp
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPFLAGS)

centralized_barrier.o: centralized_barrier.cpp
	$(CC) -c -o $@ $(CPPFLAGS) $(CPPFLAGS) $(OMPFLAGS) $^
harness.o: barrier_test.cpp
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPLIBS)

clean:
	rm -f *.o barrier centralized_barrier