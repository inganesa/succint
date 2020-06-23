#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility> //for std::pair
#include <stdint.h> //for uint32_t

using namespace std;
// TODO: Replace all maps with google dense hash map.
// TODO: all strings need to replaced with UT8 characters of different length.
// TODO: Implement getDatasetAPI. which dataset to be used?
// TODO: Merge with preproces API.
// TODO: Build auxillary datastructure.

/* TF-IDF Data structures:
1.TF/IDF structure : <doc-id (uint32) : term-id (uint32) > => <term count (uint32) > // The denominator N needed for the logN will be  kept elsewhere?
2. Terms index: <term-id (uint32)> => <term (utf-8 256 characters)>
3. Documents index: <doc-id (uint32)> => <doc name (utf-8 1024 characters)>
4 <TO_DO>Location of Terms in a document <doc-id (uint32):  term-id (uint32)> => < (uint32)s fashioned as a string > 
*/


struct pair_hash
{
  template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const
  {
    return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
  }
};

typedef uint32_t TermID;
typedef uint32_t DocID;
typedef uint32_t TermFreq;
typedef string Term;
typedef string Doc;
typedef string Filepath;
typedef string Title;
typedef unordered_map<Filepath, Title> Dataset;
typedef unordered_map<Term, TermFreq> TermCountMap;
typedef unordered_map<DocID, Doc> DocIdx;
typedef unordered_map<Term, TermID> TermIdx;
typedef unordered_map<std::pair<DocID,TermID>, TermFreq, pair_hash> DocMetadata;
typedef unordered_map<TermID, unordered_set<DocID>> PostingList;
