CXX = g++
CXXFLAGS = -I. -std=c++11 
DEPS = tfidf.h buildIndex.h manifest.h preprocess.h merge.h
OBJS = test.o manifest.o buildIndex.o

%.o: %.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

test: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean: 
	rm *.o test
