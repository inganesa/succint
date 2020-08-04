#include "util.h"

/**************** util functions for serailized ds ******************/
uint32_t
util::getBucketHdrOffset(uint32_t idx) {
  return sizeof(BucketHeaderInfo) * idx;
}

uint32_t
util::getTermHdrOffset(uint32_t idx) {
  return BUCKET_HDR_SIZE + sizeof(TermHeaderInfo) * idx;
}

void
util::updateTermHdrInfo(fstream &fs, uint32_t idx, uint32_t offset, uint32_t length) {
  TermHeaderInfo thdr = {};
  fs.seekg(util::getTermHdrOffset(idx), ios::beg);
  fs.read(reinterpret_cast<char*>(&thdr), sizeof(thdr));
  thdr.offset = offset;
  thdr.length = length;
  fs.write(reinterpret_cast<char*>(&thdr), sizeof(thdr));
  fs.seekp(fs.end);
 
}

void
util::updateBucketHdrInfo(fstream &fs, uint32_t idx, uint32_t offset, uint32_t length) {
    BucketHeaderInfo bhdr;
    fs.seekg(util::getBucketHdrOffset(idx), ios::beg);
    fs.read(reinterpret_cast<char*>(&bhdr), sizeof(bhdr));
    bhdr.offset = offset;
    bhdr.length = length;
    fs.write(reinterpret_cast<char*>(&bhdr), sizeof(bhdr));
    fs.seekp(0, fs.end);
}

void
util::updateBucketHdr(fstream &fs, BucketHeader &hdr) {
  fs.seekp(0, ios::beg);
  fs.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));
  fs.seekp(0, ios::end);
}

void
util::copyTermInfo(ifstream &ifs, fstream &fs, uint32_t length) {
  char buffer[length];
  ifs.read(buffer, (int)length);
  ifs.seekg(length, ios::cur);
  fs.write(buffer, (int)length);
  fs.seekp(0,ios::end);
}
