#ifndef MERGE_INDEX_H
#define MERGE_INDEX_H

#include <fstream>

using namespace std;

class Merger {
 public:
  Merger() {}
  bool init(string filepath1, string filepath2, string filepath3);
  void merge();
  void tearDown();
 private:
  ifstream _ifs1;
  ifstream _ifs2;
  fstream _fs;
};


#endif //MERGE_INDEX_H
