#include <buildIndex.h>
#include <iostream>

using namespace std;


void indexBuilderHandler(string manifestFilePath) {
  Index index;

  if (index.init(manifestFilePath)) {
    cout << "Index init failed" << endl;
    return;
  }

  if (!index.build()) {
    cout << "Index build failed" << endl;
    return;
  }

  if (index.purge()) {
    cout << "Index purge failed" << endl;
    return;
  }
  index.tearDown();
}  

int  main() {
  indexBuilderHandler("foo.xml");
  return 0;
}
