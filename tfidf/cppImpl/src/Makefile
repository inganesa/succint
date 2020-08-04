CXX = g++
#Include path to boost to be changed based on where you downaload and install it.
CXXFLAGS = -I. -std=c++11 -I/root/boost_1_55_0
LDFLAGS = -L~/boost/stage/lib/
LIBS = -lboost_serialization
DEPS = tfidf.h buildIndex.h manifest.h preprocess.h mergeIndex.h util.h
OBJS = util.o test.o manifest.o buildIndex.o mergeIndex.o

%.o: %.cc $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(LDFLAGS) $(LIBS)

test: $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS) $(LIBS)

clean: 
	rm *.o test
