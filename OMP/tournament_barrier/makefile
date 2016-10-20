OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CC = g++
CPPFLAGS = -g -Wall
LDFLAGS = -g -Wall
LDLIBS = $(OMPLIBS)

tournament_barrier: tounament.cpp
	$(CC) -o $@ $(LDFLAGS) $^ $(LDLIBS) $(OMPFLAGS)

tournament_barrier.o: tounament.cpp
	$(CC) -c -o $@ $(CPPFLAGS) $(CPPFLAGS) $(OMPFLAGS) $^

clean:
	rm -f *.o tournament_barrier