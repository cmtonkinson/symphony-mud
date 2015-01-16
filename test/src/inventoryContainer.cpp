
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../../src/container.h"
#include "../../src/estring.h"
#include "../../src/inventoryContainer.h"
#include "../../src/identifiers.h"
#include "../../src/object.h"
#include "../../src/regex.h"

class InventoryContainerTest: public ::testing::Test {
  protected:
    InventoryContainer*       inventory;
    /* for query parsing tests */
    int                       multiplier;
    int                       index;
    std::vector<std::string>  keywords;
    std::vector<std::string>  one_comparison;
    std::vector<std::string>  many_comparison;
    /* for object identification tests */
    std::list<Object*>        objects;
    Object*                   object;

    virtual void SetUp(void) {
      Object* foo = NULL;
      inventory = new InventoryContainer(&Identifiers::shortname);
      one_comparison.push_back("glass");
      many_comparison.push_back("glass");
      many_comparison.push_back("red");
      many_comparison.push_back("wine");
      for (int i=1; i < 11; ++i) {
        foo = new Object();
        foo->identifiers().shortname("short sword");
        foo->identifiers().longname("a short sword is here");
        foo->identifiers().addKeyword("short");
        foo->identifiers().addKeyword("sword");
        foo->identifiers().addKeyword(estring(i));
        foo->vnum(i);
        inventory->add(foo);
      }
      for (int i=11; i < 21; ++i) {
        foo = new Object();
        foo->identifiers().shortname("long sword");
        foo->identifiers().longname("a long sword is here");
        foo->identifiers().addKeyword("long");
        foo->identifiers().addKeyword("sword");
        foo->identifiers().addKeyword(estring(i));
        foo->vnum(i);
        inventory->add(foo);
      }
      return;
    }
};

TEST_F(InventoryContainerTest, SingleKeywordParseQuery) {
  // basic search
  keywords = Container::parseQuery("glass", multiplier, index);
  EXPECT_EQ(multiplier, 1);
  EXPECT_EQ(index, CONTAINER_NONE);
  EXPECT_TRUE(one_comparison == keywords) << "keywords were: \"" << Regex::implode(", ", keywords) << "\"";
  // multiplied search
  keywords = Container::parseQuery("4*glass", multiplier, index);
  EXPECT_EQ(multiplier, 4);
  EXPECT_EQ(index, CONTAINER_NONE);
  EXPECT_TRUE(one_comparison == keywords) << "keywords were: \"" << Regex::implode(", ", keywords) << "\"";
  // indexed search
  keywords = Container::parseQuery("glass#7", multiplier, index);
  EXPECT_EQ(multiplier, 1);
  EXPECT_EQ(index, 7);
  EXPECT_TRUE(one_comparison == keywords) << "keywords were: \"" << Regex::implode(", ", keywords) << "\"";
  // all search
  keywords = Container::parseQuery("all*glass", multiplier, index);
  EXPECT_EQ(multiplier, CONTAINER_ALL_W_KEYWORDS);
  EXPECT_EQ(index, CONTAINER_NONE);
  EXPECT_TRUE(one_comparison == keywords);
}

TEST_F(InventoryContainerTest, MultipleKeywordParseQuery) {
  // basic search
  keywords = Container::parseQuery("'glass red wine'", multiplier, index);
  EXPECT_EQ(multiplier, 1);
  EXPECT_EQ(index, CONTAINER_NONE);
  EXPECT_TRUE(many_comparison == keywords) << "keywords were: \"" << Regex::implode(", ", keywords) << "\"";
  // multiplied search
  keywords = Container::parseQuery("4*'glass red wine'", multiplier, index);
  EXPECT_EQ(multiplier, 4);
  EXPECT_EQ(index, CONTAINER_NONE);
  EXPECT_TRUE(many_comparison == keywords) << "keywords were: \"" << Regex::implode(", ", keywords) << "\"";
  // indexed search
  keywords = Container::parseQuery("'glass red wine'#7", multiplier, index);
  EXPECT_EQ(multiplier, 1);
  EXPECT_EQ(index, 7);
  EXPECT_TRUE(many_comparison == keywords) << "keywords were: \"" << Regex::implode(", ", keywords) << "\"";
  // all search
  keywords = Container::parseQuery("all*'glass red wine'", multiplier, index);
  EXPECT_EQ(multiplier, CONTAINER_ALL_W_KEYWORDS);
  EXPECT_EQ(index, CONTAINER_NONE);
  EXPECT_TRUE(many_comparison == keywords) << "keywords were: \"" << Regex::implode(", ", keywords) << "\"";
}

TEST_F(InventoryContainerTest, ObjectManipulation) {
  // single keyword
  objects = inventory->searchObjects("short");
  EXPECT_EQ(1, objects.size());
  EXPECT_TRUE(objects.front()->identifiers().matchesKeyword("short"));
  // single keyword with index
  objects = inventory->searchObjects("short#3");
  EXPECT_EQ(1, objects.size());
  EXPECT_EQ(3, objects.front()->vnum());
  EXPECT_TRUE(objects.front()->identifiers().matchesKeyword("short"));
  // single keyword with multiplier
  objects = inventory->searchObjects("3*short");
  EXPECT_EQ(3, objects.size());
  EXPECT_TRUE(objects.front()->identifiers().matchesKeyword("short"));
  // multiple keywords
  objects = inventory->searchObjects("'long sword'");
  EXPECT_EQ(1, objects.size());
  EXPECT_TRUE(objects.front()->identifiers().matchesKeyword("long"));
  EXPECT_TRUE(objects.front()->identifiers().matchesKeyword("sword"));
  // multiple keywords with index
  objects = inventory->searchObjects("'long sword'#7");
  EXPECT_EQ(1, objects.size());
  EXPECT_EQ(17, objects.front()->vnum());
  EXPECT_TRUE(objects.front()->identifiers().matchesKeyword("long"));
  EXPECT_TRUE(objects.front()->identifiers().matchesKeyword("sword"));
  // multiple keywords with multiplier
  objects = inventory->searchObjects("5*'long sword'");
  EXPECT_EQ(5, objects.size());
  EXPECT_TRUE(objects.front()->identifiers().matchesKeyword("long"));
  EXPECT_TRUE(objects.front()->identifiers().matchesKeyword("sword"));
}
