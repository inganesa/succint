#include "mergeIndex.h"

#include <tfidf.h>
#include <util.h>

bool
Merger::init(string filepath1, string filepath2, string filepath3) {
  _ifs1.open(filepath1, fstream::in);
  if (!_ifs1.is_open()) {
    cout << "Could not open file" << filepath1 << endl;
  }
  _ifs2.open(filepath2, fstream::in);
  if(!_ifs2.is_open()) {
    cout << "Could not open file" << filepath2 << endl;
  }
  _fs.open(filepath3, fstream::in);
  if (!_fs.is_open()){ 
    cout << "Could not open file" << filepath3 << endl;
  }
}

void
Merger::merge() {

  BucketHeader bhdr1, bhdr2, bhdrm = {};

  _ifs1.read(reinterpret_cast<char*>(&bhdr1), sizeof(BucketHeader));
  _ifs2.read(reinterpret_cast<char*>(&bhdr2), sizeof(BucketHeader)); 
  _ifs1.seekg(sizeof(bhdr1), ios::cur);
  _ifs2.seekg(sizeof(bhdr2), ios::cur);

  _fs.write(reinterpret_cast<char*>(&bhdrm), sizeof(BucketHeader));
  _fs.seekp(sizeof(bhdrm), ios::cur);

  // write header info
  for (int b = 0; b < MAX_BUCKETS_PER_OBJECT; ++b) {
    uint32_t tc1 = bhdr1.bslots[b].count;
    uint32_t tc2 = bhdr2.bslots[b].count;

    vector<TermHeaderInfo> thdrm = {};
    vector<TermHeaderInfo> thdr1 = {};
    vector<TermHeaderInfo> thdr2 = {};

    _ifs1.read(reinterpret_cast<char*>(&thdr1), sizeof(TermHeaderInfo) * tc1);
    _ifs2.read(reinterpret_cast<char*>(&thdr2), sizeof(TermHeaderInfo) * tc2); 
    _ifs1.seekg(sizeof(thdr1),ios::cur);
    _ifs2.seekg(sizeof(thdr2),ios::cur);

    int i = 0, j = 0;
    while (i < tc1 && j < tc2) {
      if (thdr1[i].id < thdr2[j].id) {
	thdrm.push_back(thdr1[i++]);
      } else if (thdr1[i].id > thdr2[j].id) {
	thdrm.push_back(thdr2[j++]);
      } else {
	TermHeaderInfo hdr = {};
	hdr.id = thdr1[i].id;
	hdr.count = thdr1[i].count + thdr2[j].count;
	thdrm.push_back(hdr);
	i++; j++;
      }
    }
    while (i < tc1) {
      thdrm.push_back(thdr1[i++]);
    }
    while (j < tc2) {
      thdrm.push_back(thdr2[j++]);
    }

    bhdrm.bslots[b].id = b;
    bhdrm.bslots[b].count = thdrm.size();
    uint32_t offset = _fs.tellp();
    bhdrm.bslots[b].offset = offset;
    _fs.write(reinterpret_cast<char*>(&thdrm), sizeof(TermHeaderInfo) * thdrm.size());
    _fs.seekp(sizeof(thdrm) * thdrm.size(), ios::cur);
    bhdrm.bslots[b].length = (uint32_t)_fs.tellp() -  offset;
  }
  util::updateBucketHdr(_fs, bhdrm);
  
  // merge terms
  for (int b = 0; b < MAX_BUCKETS_PER_OBJECT; ++b) {
    uint32_t tc1 = bhdr1.bslots[b].count;
    uint32_t tc2 = bhdr2.bslots[b].count;
    int i = 0, j = 0, k = 0;
    uint32_t length, offset;

    vector<TermHeaderInfo> thdr1 = {};
    vector<TermHeaderInfo> thdr2 = {};

    _ifs1.read(reinterpret_cast<char*>(&thdr1), sizeof(TermHeaderInfo) * tc1);
    _ifs2.read(reinterpret_cast<char*>(&thdr2), sizeof(TermHeaderInfo) * tc2); 
    _ifs1.seekg(sizeof(thdr1),ios::cur);
    _ifs2.seekg(sizeof(thdr2),ios::cur);

    while (i < tc1 && j < tc2) {
      offset = _fs.tellp();
      if (thdr1[i].id < thdr2[j].id) {
	length = thdr1[i].length;
	util::copyTermInfo(_ifs1, _fs, length);
	i++;
      } else if (thdr1[i].id > thdr2[j].id) {
	length = thdr2[j].length;
	util::copyTermInfo(_ifs2, _fs, length);
	j++;
      } else {
	uint32_t length1 = thdr1[i].length;
	util::copyTermInfo(_ifs1, _fs, length1);
	uint32_t length2 = thdr2[i].length;
	util::copyTermInfo(_ifs2, _fs, length2);
	i++; j++;
	length = length1 + length2;
      }
      //update term header
      util::updateTermHdrInfo(_fs, k, offset, length);
      k++;
    }
    while (i < tc1) {
      offset = _fs.tellp();
      length = thdr1[i].length;
      util::copyTermInfo(_ifs1, _fs, length);
      i++;
      //update term header
      util::updateTermHdrInfo(_fs, k, offset, length);
      k++;
    }
    while (j < tc2) {
      offset = _fs.tellp();
      length = thdr2[j].length;
      util::copyTermInfo(_ifs2, _fs, length);
      j++;
      //update term header
      util::updateTermHdrInfo(_fs, k, offset, length);
      k++;      
    }
  } //end of merge

}

void
Merger::tearDown() {
  _ifs1.close();
  _ifs2.close();
  _fs.close();
}

