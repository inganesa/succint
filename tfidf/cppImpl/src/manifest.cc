#include "manifest.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <set>
#include <exception>
#include <iostream>
#include <unordered_map>

namespace pt = boost::property_tree;
static std::unordered_map<string, DataKeyType> KeyTable = { {"file", DataKeyType::file},
							    {"object", DataKeyType::object},
							    {"c2cRepo", DataKeyType::c2cRepo}};

/********************* Manifest Class ***********************/
void
Manifest::init(string path) {
    // Create empty property tree object
    pt::ptree tree;

    // Parse the XML into the property tree.
    pt::read_xml(path, tree);

    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("document.Keys")) {
      // The data function is used to access the data stored in a node.
      string keyname = v.first.data();
      string keyval = v.second.data();
      DataKeyType keytype = KeyTable[keyname];

      DataKey key(keytype, keyval);
      _fPaths.push_back(key);
    }

    _count = tree.get("document.Count", 0);
    _startId = tree.get("document.StartID", 0);
}

bool
Manifest::hasNext() {
  return i < _count;
}

DataKey
Manifest::getNext() {
  return _fPaths[i++];
}

DocID
Manifest::getNextDocId() {
  DocID id =  _startId + _docCount;
  ++_docCount;
  return id;
}


/************************* DataContainer Class functions **************************/

/*static*/unique_ptr<DataContainer>
DataContainer::Create(DataKey key) {
  if (key.type == DataKeyType::file) {
    return unique_ptr<DataContainer>(new FileData(key));
  } else {
    // TODO Implement others
  }
}


/**************************** FileData Class ****************************/

bool
FileData::init() {
  string filePath = _key.val;
  _fs.open(filePath, fstream::in);
  if(!_fs.is_open()) {
    cout << "Could not open file: "<< filePath << endl;
    return false;
  }
  return true;
}

bool
FileData::hasData() {
  return !_fs.eof();
}

void
FileData::tearDown(){ 
  _fs.close();
}

ChunkData
FileData::getNextChunk() {
    ChunkData data = {};
    data.id = getNextChunkID();
    _fs.read(data.data, _chunkSize);
    data.size = _fs.gcount();
    return data;
}

/******************************* End of FileData Class ******************************/
