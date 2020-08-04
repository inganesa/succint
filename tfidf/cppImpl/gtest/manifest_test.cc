#include <memory>
#include <manifest.cc>
#include <gtest/gtest.h>
 
TEST(ManifestTest, PositiveKeys) 
{
  Manifest manifest;
  std::string path("../foo_valid.xml");
  EXPECT_NO_THROW(manifest.init(path));
  ASSERT_EQ(true, manifest.hasNext());
  ASSERT_EQ(1, manifest.getNumDocs());
  DataKey key = manifest.getNext();
  ASSERT_EQ(DataKeyType::file, key.type);
  ASSERT_EQ("foo.txt", key.val);
}
 
TEST(ManifestTest, NegativeKeys) 
{
  Manifest manifest;
  std::string path("../foo_invalid.xml");
  try {
    manifest.init(path);
    FAIL() << "Expected: No such node (document.Keys)";
  } catch (const boost::property_tree::ptree_error &e) {
    EXPECT_EQ(e.what(),std::string("No such node (document.Keys)"));
  }
}

TEST(DataContainerTest, PositiveKey)
{
  DataKey key(DataKeyType::file, "../foo.txt");
  unique_ptr<DataContainer> dc = DataContainer::Create(key);
  EXPECT_EQ(true, dc->init());
  EXPECT_EQ(true, dc->hasData());
  ChunkData data = dc->getNextChunk();
  EXPECT_EQ("this is an example.\n", std::string(data.data));
  EXPECT_EQ(0, data.id);
  EXPECT_EQ(20, data.size);
  EXPECT_EQ(false, dc->hasData());
  dc->tearDown();
}
 
int main(int argc, char **argv) 
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
