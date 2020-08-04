#ifndef UTIL_H
#define UTIL_H

#include <tfidf.h>
#include <fstream>

using namespace std;

namespace util {
  uint32_t getBucketHdrOffset(uint32_t idx);
  uint32_t getTermHdrOffset(uint32_t idx);
  void updateTermHdrInfo(fstream &fs, uint32_t idx, uint32_t offset, uint32_t length);
  void updateBucketHdrInfo(fstream &fs, uint32_t idx, uint32_t offset, uint32_t length);
  void updateBucketHdr(fstream &fs, BucketHeader &hdr);
  void copyTermInfo(ifstream &ifs, fstream &fs, uint32_t length);
} // << util
#endif //UTIL_H
