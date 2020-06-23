CC=g++
CFLAGS=-I. -std=c++11 
DEPS = index.h tfidf.h
OBJS = test.o index.o

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm *.o test
