#include <index.h>
#include <iostream>

using namespace std;

int  main() {
  Index index(0);
  string baseFolderName = "stories";
  index.build(baseFolderName);
  string qs = "Polymetals are alloys of metals that have polymer composites";
  vector<Doc> docs = index.query(qs, 3);
  for (auto d : docs) {
    cout << "Document: " << d << endl;
  }
  return 0;
}
