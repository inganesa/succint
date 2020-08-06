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

  /************** Incore TF structure *****************/
  struct TermInfoPerDoc {
    TermFreq count;
    vector<TermLoc> locations; //size is == count
  TermInfoPerDoc() : count(0) {}
    void addInfo(TermLoc loc) {
      count++;
      locations.push_back(loc);
    }
  };

  typedef unordered_map<TermID, TermInfoPerDoc> LocalDS;
  typedef unordered_map<TermID, vector<TermInfo>> GlobalDS;
  
  //public function for test purposes.
  GlobalDS& getGlobalMap() { return _termInfoMap; }

 private:
  /*** private member functions ***/
  void updateLocalMap(TermID id, TermLoc loc);
  void updateGlobalMap(DocID id);
  DocID getDocId(DocID localid, DocID chunkid);
  uint32_t getBucketHdrOffset(uint32_t idx);
  uint32_t getTermHdrOffset(uint32_t idx);

  /**** private data members ****/
  Manifest _manifest;
  LocalDS _termInfoPerDocMap;
  GlobalDS _termInfoMap;
};

#endif //BUILDINDEX_H
