#include <buildIndex.h>
#include <mergeIndex.h>
#include <iostream>

using namespace std;


void indexBuildHandler(string manifestFilePath) {
  Index index;

  if (!index.init(manifestFilePath)) {
    cout << "Index init failed" << endl;
    return;
  }
  cout << "Index inited" << endl;

  if (!index.build()) {
    cout << "Index build failed" << endl;
    return;
  }
  cout << "Index built" << endl;
  if (!index.purge()) {
    cout << "Index purge failed" << endl;
    return;
  }
  cout << "Index purged" << endl;
  index.tearDown();
}

void indexMergeHandler(string filepath1, string filepath2, string filepath3) {
  Merger merger;
  if (merger.init(filepath1, filepath2, filepath3)) {
    cout << "Merger init failed" << endl;
  }
  merger.merge();
  merger.tearDown();
}

int  main() {
  indexBuildHandler("foo.xml");
  indexBuildHandler("bar.xml");
  indexMergeHandler("fooIndex.txt", "barIndex.txt", "foobarIndex.txt");
  return 0;
}
