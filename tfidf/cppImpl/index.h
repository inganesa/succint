#include <tfidf.h>
#include <vector>
#include <float.h>

class Index {
  //TODO build auxillary structure.
 public:

  Index(uint32_t seed) {
    _seed = seed;
    _tid = 0;
    _did = 0;
  }

  // Supported operations on Index
  void build(string baseFolderName);
  void purge(); // TODO to s3? //in succint format?
  void load(); // TODO from s3?
  void merge(Index &idx2); //TODO merge incore
  vector<Doc> query(string query, int k); //Query Incore: just for testing, should be later from succint structure.
  
  //getters
  DocMetadata& getDocMetaText() {
    return _docMetaText;
  }

  DocMetadata& getDocMetaTitle() {
    return _docMetaTitle;
  }

  PostingList& getPostlingList() {
    return _postingList;
  }

  DocIdx& getDocIdx() {
    return _docIndex;
  }

  TermIdx& getTermIdx() {
    return _termIndex;
  }

  ~Index () {}

 private:
  void updateTFIDFIndex(string data, bool isTitle);
  TermCountMap getTermsCount(vector<string> preprocessedText);
  Dataset getDataset(string baseFolderName);
  DocID getNewDocId();
  TermID getNewTermId();
  uint32_t docFreq(TermID termId);
  uint32_t termFreq(DocID docId, TermID termId, bool isTitle);
  float tfidfVal(DocID docId, TermID termId);

  TermID _tid;
  DocID _did;
  uint32_t _seed;
  DocIdx _docIndex; //index of the vector gives doc id
  TermIdx _termIndex;
  PostingList _postingList;
  DocMetadata _docMetaText; //TODO can pair be used as key in google densehashmap 
  DocMetadata _docMetaTitle; //TODO can pair be used in google densehashmap 
};
