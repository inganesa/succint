#ifndef MANIFEST_H
#define MANIFEST_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <tfidf.h>


using namespace std;

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


/*
 * Manifest file layout example:
 * <document>
 *   <Keys>
 *     <file> foo.txt </file>
 *     <object> object id </object>
 *     <c2cRepo> c2c repo file </c2cRepo>
 *   </Keys>
 *   <Count> *no of docs* </Count>
 *   <StartID> *Doc Id of starting document </StartID>
 * </document>
 */
class Manifest {
 public:
 Manifest():i(0) {}

  void init(string path);
  bool hasNext();
  DataKey getNext();
  DocID getNextDocId();

  int getNumDocs() {
    return _count;
  }

  ~Manifest() {}

 private:
  vector<DataKey> _fPaths; // List of key value pairs, where key represent the type of file path.
                           // value represent the file path/objectID.
  int _count;
  DocID _startId;
  DocID _docCount;
  int i;

};


class DataContainer {
 public:
  virtual bool init() = 0;
  virtual bool hasData() = 0;
  virtual ChunkData getNextChunk() = 0;
  virtual void tearDown() = 0;
  static unique_ptr<DataContainer>Create(DataKey key);
  virtual ~DataContainer() {}
  DataContainer() {}
 DataContainer(DataKey key) : _key(key), _chunkId(0) {}

 protected:
  ChunkID getNextChunkID () {
    return _chunkId++;
  }
  uint32_t _chunkSize;
  DataKey _key;

 private:
  ChunkID _chunkId;
};



class FileData : public DataContainer {

 public:
  virtual bool init ();
  virtual bool hasData();
  virtual ChunkData getNextChunk();
  virtual void tearDown();

 FileData(DataKey key): DataContainer(key) {}
  ~FileData() {}

 private:
  ifstream _fs;
};

#endif //MANIFEST_H
