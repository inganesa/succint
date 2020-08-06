#include <buildIndex.cc>
#include <gtest/gtest.h>


TEST(DISABLED_buildIndexTest, build)
{
  Index index;
  string mpath = "../buildIndexTest.xml";
  EXPECT_EQ(true, index.init(mpath));
  EXPECT_EQ(true, index.build());
  GlobalDS &gmap = index.getGlobalMap();
  EXPECT_EQ(3, gmap.size());
  int i = 0;
  for (auto kv : gmap) {
    EXPECT_EQ(i++, kv.first);
    vector<TermInfo> &info = kv.second;
    for (int i = 0; i < info.size(); ++i) {
      TermInfo ti = info[i];
      EXPECT_EQ(0, ti.docId);
      EXPECT_EQ(2, ti.count);
      //TODO: verify location
    }
  }
}


TEST(buildIndexTest, purge)
{
  Index index;
  //  string mpath = "../buildIndexTest.xml";
  // EXPECT_EQ(true, index.init(mpath));
  GlobalDS &termInfoMap = index.getGlobalMap();

  TermFreq f = 5;
  vector<TermLoc> loc = {0,1,2};
  TermInfo v1(0, 5, loc);
  TermInfo v2(1, 10, loc);
  vector<TermInfo> info= {v1,v2}; 
  for (int i = 0; i = 10; ++i) {
    termInfoMap[i] = info;
  }
  
  EXPECT_EQ(true, index.purge());

  fstream fs("serialized_index.txt");
  EXPECT_EQ(true, fs.is_open());
  BucketHeader bhdr;
  fs.read(reinterpret_cast<char*>(&bhdr), sizeof(BucketHeader));
  fs.seekg(sizeof(BucketHeader), ios::cur);
  EXPECT_EQ(10, bhdr.hdr.count);

}
int main(int argc, char **argv) 
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
