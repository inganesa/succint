#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <tfidf.h>

class Preprocess {

 public:
  Preprocess(ChunkData data) {
    _data  = data;
  }
  
  //TBD: to be implemented.
  bool hasTerm() {}
  void getNextTerm(TermID &id, TermLoc &loc) {}

  ~Preprocess() {}

 private:
  ChunkData _data;
};

#endif //PREPROCESS_H
