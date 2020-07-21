CC=g++
CFLAGS=-I. -std=c++11 
DEPS = tfidf.h buildIndex.h manifest.h preprocess.h merge.h
OBJS = test.o manifest.o buildIndex.o

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm *.o test
