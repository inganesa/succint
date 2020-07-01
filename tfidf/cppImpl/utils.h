#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include <unordered_map>
namespace pt = boost::property_tree;
using namespace std;

enum DataKey {
  file     = 0,
  object   = 1,
  c2cRepo  = 2
};

typedef string DataPath;

static std::unordered_map<string, DataKey> const KeyTable = { {"file",dataKey::file}, {"object",dataKey::object}, {"c2cRepo", dataKey::c2cRepo}};
typedef pair<DataKey, DataPath> DataFile;

Class Manifest {
 public:
  Manifest(string mPath) {
    _path = mPath;
  }

  void init() {
    // Create empty property tree object
    pt::ptree tree;

    // Parse the XML into the property tree.
    pt::read_xml(filename, tree);

    BOOST_FOREACH(pt::ptree::value_type &v, tree.get_child("Keys")) {
      // The data function is used to access the data stored in a node.
      dataKey key = getKey(v.first.data());
      string path = v.second.data();
      _fPaths.push_back(make_pair(key, path));
    }
  }

  
    
  ~Manifest() {}
 private:
  string _mPath; // Manifest file path
  vector<pair<dataKey,string> _fPaths; // List of key value pairs, where key represent the type of file path.
                                      // value represent the file path.
};
