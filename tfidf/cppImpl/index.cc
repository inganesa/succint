#include <index.h>
#include <preprocess.h>
#include <math.h>
#include <iostream> //std::cout
#include <algorithm> //std::sort
#include <fstream> //std::ifstream

/* Private Methods */
Dataset
Index::getDataset(string baseFolderName) {
  
}

TermCountMap
Index::getTermsCount(vector<string> preprocessedText) {
  TermCountMap termCount;
  for (auto w  : preprocessedText) {
    ++termCount[w];
  }
}

TermID
Index::getNewTermId() {
  // TODO : use seed to generate uinque term id. Need to generate key based on term.
  return ++_tid;
}

DocID
Index::getNewDocId() {
  // TODO: use seed to generate uinque doc id.
  return ++_did;
}

void
Index::updateTFIDFIndex(string data, bool isTitle) {
  TermCountMap termsMap = getTermsCount(preprocess(data));
  for (auto t : termsMap) {
    Term term = t.first;
    uint32_t termFreq = t.second;
    // new term needs an update in termIndex and InverseTermIndex
    if (_termIndex.find(term) == _termIndex.end()) {
      _termIndex[term] = getNewTermId();
    }
    TermID termId = _termIndex[term];
    // add doc id to the current term's postling list
    _postingList[termId].insert(_did);
    // update term frequency for current doc;
    if (isTitle) {
      _docMetaTitle[make_pair(_did, termId)] += termFreq; 
    } else {
      _docMetaText[make_pair(_did, termId)] += termFreq;
    }
  }
}

// TODO: this should be updated to query from succint DS
uint32_t
Index::docFreq(TermID termId) {
  return _postingList[termId].size();
}

// TODO: this should be updated to query from succint DS
uint32_t
Index::termFreq(DocID docId, TermID termId, bool isTitle) {
  if (isTitle) {
    return _docMetaTitle[make_pair(docId, termId)];
  }
  return _docMetaText[make_pair(docId, termId)];
}

float
Index::tfidfVal(DocID docId, TermID termId) {
  float alpha = 0.3;
  uint32_t N = _docIndex.size();
  uint32_t df;
  float tf;
  float idf;
  tf = termFreq(docId, termId,  true) + alpha * termFreq(docId, termId, false);
  df = docFreq(termId);
  // TODO confirm if it is log2.
  idf = log2((N + 1) / (df + 1));
  return tf * idf;
}


/* Public Methods */
void
Index::build(string baseFolderName)
{
  Dataset dataset = getDataset(baseFolderName);
  string line;
  for (auto files : dataset) {
    Filepath filepath = files.first;
    Title title = files.second;
    ifstream myfile(filepath, ifstream::in);
    _docIndex[getNewDocId()] = filepath;
    if (myfile.is_open()) {
      //1. Update TF_IDF structures for title.
      updateTFIDFIndex(title, true);
      //2. Update TD_IDF structure for text.
      while ( getline (myfile,line) ) {
	updateTFIDFIndex(line, false);
      }
      myfile.close();
    }
    else cout << "Unable to open file: " << files.first;
  }
}

void
Index::merge(Index &idx2)
{
  DocMetadata &DMText = idx2.getDocMetaText();
  DocMetadata &DMTitle = idx2.getDocMetaTitle();
  DocIdx &docIdx = idx2.getDocIdx();
  TermIdx &termIdx = idx2.getTermIdx();

  _docMetaText.insert(DMText.begin(), DMText.end());
  _docMetaTitle.insert(DMTitle.begin(), DMTitle.end());
  _docIndex.insert(docIdx.begin(), docIdx.end());
  _termIndex.insert(termIdx.begin(), termIdx.end());

  PostingList &PL = idx2.getPostlingList();
  for (auto df : PL) {
    TermID tid = df.first;
    unordered_set<DocID>& docs = df.second;
    _postingList[tid].insert(docs.begin(), docs.end());

  }
}

void
Index::purge()
{
  //TODO  and Write to s3?
}

void
Index::load()
{
  //TODO Load Index fro S3
}

vector<Doc>
Index::query(string query, int k)
{
  vector<Term> queryTokens = preprocess(query);
  unordered_map<DocID, float> queryWeights;
  vector<Doc> topDocs;
  for (auto t : queryTokens) {
    if (_termIndex.find(t) == _termIndex.end()) continue;
    TermID tid = _termIndex[t];
    for (auto d : _postingList[tid]) {
      queryWeights[d] += tfidfVal(d, tid);
    }
  }
  typedef std::pair<DocID, float> qwPair;

  vector<qwPair> sortedqw;
  for (auto qw :queryWeights) {
    sortedqw.push_back(make_pair(qw.first, qw.second));
  }
  auto qwsort = [] (qwPair p1, qwPair p2) {
    return p1.second > p2.second;
  };
  sort(sortedqw.begin(), sortedqw.end(), qwsort);
  
  for (int i = 0; i < k; ++i) {
    topDocs.push_back(_docIndex[sortedqw[i].first]);
  }
  return topDocs;
}
