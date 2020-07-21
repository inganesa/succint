#include "buildIndex.h"

#include <preprocess.h>

#include <math.h>
#include <iostream> //std::cout
#include <algorithm> //std::sort
#include <fstream> //std::ifstream
#include <boost/property_tree/exceptions.hpp>

/* Private Methods */
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
 
TermID
Index::getTermId(Term term) {
  //TBD lookup global map tbd.
}

DocID
Index::getDocId(DocID localid, DocID chunkid) {
  DocID id = localid << 16 + chunkid;
  return id;
}

/* Public Methods */
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
	Term term;
	TermID id;
	TermLoc loc;

	p.getNextTerm(term, loc);
	id = getTermId(term);

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
  // TODO: Serialize 
  for ( unsigned i = 0; i < _termInfoMap.bucket_count(); ++i) {
    for ( auto it = _termInfoMap.begin(i); it!= _termInfoMap.end(i); ++it )
      //serialize here into K objects
      cout << it->first << endl;
    //cout << i << endl;
  }
  return true;
}

void
Index::tearDown()
{
  _termInfoMap.clear();
  _termInfoPerDocMap.clear();
}
