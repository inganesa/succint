#ifndef TFIDF_H
#define TFIDF_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility> //for std::pair
#include <stdint.h> //for uint32_t
#include <vector>

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


/*struct pair_hash
{
  template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const
  {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};

typedef string Filepath;
typedef string Title;
typedef unordered_map<Filepath, Title> Dataset;
typedef unordered_map<Term, TermFreq> TermCountMap;
typedef unordered_map<DocID, Doc> DocIdx;
typedef unordered_map<Term, TermID> TermIdx;
typedef unordered_map<std::pair<DocID,TermID>, TermFreq, pair_hash> DocMetadata;
typedef unordered_map<TermID, unordered_set<DocID>> PostingList;
*/


typedef uint32_t TermID;
typedef uint32_t DocID;
typedef uint32_t ChunkID;
typedef uint32_t TermFreq;
typedef string Term;
typedef string Doc;
typedef uint16_t TermLoc;


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


/****************** TF data structure to be serialized *******************/
struct TermInfo {
  DocID docId; //global doc id + chunk id
  TermFreq count;
  vector<TermLoc> locations;
};

typedef unordered_map<TermID, vector<TermInfo>> GlobalDS;


/************ Manifest File DS **************/
enum DataKeyType {
  file     = 0,
  object   = 1,
  c2cRepo  = 2
};

struct DataKey {
  DataKeyType type;
  string val;
DataKey() : type(DataKeyType::file), val("") {}
DataKey(DataKeyType type, string val) : type(type), val(val) {}
};
  
#define CHUNKSIZE 8096

struct ChunkData {
  ChunkID id;
  uint32_t size;
  char data[CHUNKSIZE];
};

#endif //TFIDF_H
