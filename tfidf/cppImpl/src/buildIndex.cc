#include "buildIndex.h"
#include <preprocess.h>
#include <util.h>

#include <math.h>
#include <iostream> //std::cout
#include <algorithm> //std::sort
#include <fstream> //std::ifstream
#include <boost/property_tree/exceptions.hpp>
#include <boost/archive/text_oarchive.hpp>

/******************** Private Methods ********************/
void
Index::updateLocalMap(TermID id, TermLoc loc) {

  if (_termInfoPerDocMap.find(id) == _termInfoPerDocMap.end()) {
    TermInfoPerDoc tinfo;
    _termInfoPerDocMap[id] = tinfo;
  }
  TermInfoPerDoc &tinfo = _termInfoPerDocMap[id];
  tinfo.addInfo(loc);
}

void
Index::updateGlobalMap(DocID id) {
  for (auto t : _termInfoPerDocMap) {
    TermInfo tinfo;
    tinfo.docId = id; //doc id
    tinfo.count = t.second.count; //term frequency
    tinfo.locations = t.second.locations; // term locations.
    _termInfoMap[t.first].push_back(tinfo);
  }
  _termInfoPerDocMap.clear();
}

DocID
Index::getDocId(DocID localid, DocID chunkid) {
  DocID id = localid << 16 + chunkid;
  return id;
}


/*********** Public Methods **************/
bool
Index::init(string manifestFilepath) {
  try
    {
      _manifest.init(manifestFilepath);
    }
  catch(const boost::property_tree::ptree_error &e)
    {
      cout << e.what() << endl;
      return false;
    }
  return true;
}

bool
Index::build()
{

  while (_manifest.hasNext()) {
    DataKey key = _manifest.getNext();
    unique_ptr<DataContainer> dc = DataContainer::Create(key);

    if (!dc->init()) {
      cout << "init failed" << endl;
      return false;
    }

    while(dc->hasData()) {
      ChunkData data = dc->getNextChunk();

      //preprocess
      Preprocess p(data);

      while (p.hasTerm()) {
	TermID id;
	TermLoc loc;

	p.getNextTerm(id, loc);
	updateLocalMap(id, loc);

      } // Finished processing all terms in a chunk.

      DocID localDocId = _manifest.getNextDocId();
      DocID globalDocId = getDocId(localDocId, data.id);

      //For all the terms seen in this document chunk, update map that will be serialized.
      updateGlobalMap(globalDocId);

    }// Finished processing all chunks in a document.

    //cleanup dc
    dc->tearDown();

  } // Finished processing all documents.

  return true;
} //End of buildIndex


bool
Index::purge()
{

  std::fstream fs("serialized_index.txt");
  uint32_t length, offset;

  // write bucket headers
  BucketHeader bhdr;
  // split into multiple objects.
  bhdr.hdr.count = _termInfoMap.bucket_count();
  for ( unsigned i = 0; i < _termInfoMap.bucket_count(); ++i) {
    bhdr.bslots[i].id = i;
    bhdr.bslots[i].count = _termInfoMap.bucket_size(i);
    // bucketheaderinfo offset and length  will be filled when term header is written.
  }
  fs << bhdr;

  // write terms header.
  for ( unsigned i = 0; i < _termInfoMap.bucket_count(); ++i) {

    vector<TermID> keys;
    for (auto it = _termInfoMap.begin(i); it != _termInfoMap.end(i); ++it) {
       keys.push_back(it->first);
    }
    sort(keys.begin(), keys.end());

    for (auto it = keys.begin(); it != keys.end(); ++it) {
      TermHeaderInfo thdr = {};
      TermID id = *it;
      thdr.id = id;
      thdr.count = _termInfoMap[id].size();
      // TermHeaderInfo offset and length will be filled while writing terms data.
      fs << thdr;
    }

    length = (uint32_t)fs.tellp() - offset;
    util::updateBucketHdrInfo(fs, i, offset, length);

    // write terms data
    unsigned j = 0;
    for (auto it = keys.begin(); it != keys.end(); ++it) {
      offset = fs.tellp();
      TermID id = *it;
  
      // create and open a character archive for output
      boost::archive::text_oarchive oa(fs);
      oa << _termInfoMap[id];
      length = (uint32_t)fs.tellp() - offset;
      util::updateTermHdrInfo(fs, j, offset, length);
    }

  }
  return true;
}

void
Index::tearDown()
{
  _termInfoMap.clear();
  _termInfoPerDocMap.clear();
}
