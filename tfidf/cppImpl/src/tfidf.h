#ifndef TFIDF_H
#define TFIDF_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility> //for std::pair
#include <stdint.h> //for uint32_t
#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include <ostream>

using namespace std;

// TODO: Replace all maps with google dense hash map.
// TODO: all strings need to replaced with UT8 characters of different length.

/* TF-IDF Data structures:
1. Incore TF structure: <term-id (uint32) > => <term count (uint32), array of term location(uint16?)> // This is a local incore DS stored per docID(global docID + chunkID) to be stored.
   After process one doc chunk this needs to be merged with TF Structured to be serialized.
2. TF structure to be serialized:This large DS to be store in dense hash map(?) will be serialized and stored in dense hash map.
2. Terms index:  <term (utf-8 256 characters)> => <term-id (uint32)> // This is served as a global dictionary in succint form TBD 
3. Documents index: <doc-id (uint32)> => <doc name (utf-8 1024 characters)> // this is stored in a object store by the lambda function that schedules index build.
4 Location of Terms in a document <doc-id (uint32):  term-id (uint32)> => < (uint32)s fashioned as a string > 
*/

typedef uint32_t TermID;
typedef uint32_t DocID;
typedef uint32_t ChunkID;
typedef uint32_t TermFreq;
typedef string Term;
typedef string Doc;
typedef uint16_t TermLoc;


/****************** TF data structure to be serialized *******************/

#define  MAX_BUCKETS_PER_OBJECT 10
#define BUCKET_HDR_SIZE sizeof(BucketHeaderInfo) * MAX_BUCKETS_PER_OBJECT

struct __attribute__((packed)) ObjectHeaderInfo {
  uint32_t count:16; // no of buckets
  // uint32_t length; // length of header
};

inline ostream& operator<< (ostream& os, const ObjectHeaderInfo& rhs) {
  os  << rhs.count;
  return os;
}

struct __attribute__((packed)) BucketHeaderInfo {
  uint32_t id;     // bucket id
  uint32_t count; // no of terms
  uint32_t offset; // offset to term Header
  uint32_t length; // length of term Header infos; // is this needed?
};

inline ostream& operator << (ostream& os, const BucketHeaderInfo& rhs) {
  os << rhs.id;
  os << rhs.count;
  os << rhs.offset;
  os << rhs.length;
  return os;
}

struct __attribute__((packed)) BucketHeader {
  ObjectHeaderInfo hdr;
  BucketHeaderInfo bslots[MAX_BUCKETS_PER_OBJECT];
};

inline ostream& operator << (ostream& os, const BucketHeader& rhs) {
  os << rhs.hdr;
  for (auto v : rhs.bslots) {
    os << v;
  }
}

struct __attribute__((packed)) TermHeaderInfo {
  uint32_t id; // term id
  uint32_t count; // no of documents.
  uint32_t offset; // offset to term info
  uint32_t length; // length of term info
};

inline ostream& operator << (ostream& os, const TermHeaderInfo& rhs) {
  os << rhs.id;
  os << rhs.count;
  os << rhs.offset;
  os << rhs.length;
  return os;
}

// each bucket will point to one or more TermHeader infos.
struct __attribute__((packed)) TermHeader {
  TermHeaderInfo tslots[0];
};

struct TermInfo {
  DocID docId; //global doc id + chunk id
  TermFreq count;
  vector<TermLoc> locations;
TermInfo(DocID id, TermFreq f, vector<TermLoc> &loc) : docId(id), count(f), locations(loc) {}
TermInfo() : docId(0), count(0) {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar & docId;
    ar & count;
    ar & locations;
  }
};

struct TermData {
  TermInfo tdata[0];
};

#endif //TFIDF_H
