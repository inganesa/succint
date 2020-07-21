#ifndef BUILDINDEX_H
#define BUILDINDEX_H

#include <tfidf.h>
#include <manifest.h>

class Index {
 public:

  Index() {}
  // Supported operations on Index
  bool init(string manifestFilePath);
  bool build();
  bool purge(); // TODO to s3 in serialized fashion.  
  void tearDown();
  ~Index () {}

 private:
  /*** private member functions ***/
  void updateLocalMap(TermID id, TermLoc loc);
  void updateGlobalMap(DocID id);
  TermID getTermId(Term term);
  DocID getDocId(DocID localid, DocID chunkid);

  /**** private data members ****/
  Manifest _manifest;
  LocalDS _termInfoPerDocMap;
  GlobalDS _termInfoMap;
};

#endif //BUILDINDEX_H
