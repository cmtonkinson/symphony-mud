
#include <gtest/gtest.h>
#include "../../src/dice.h"

class DiceTest: public ::testing::Test {
  protected:
    Dice* one;
    Dice* two;
    Dice* three;

    virtual void SetUp(void) {
      one = new Dice(3, 6);
      two = new Dice(1, 20);
      three = new Dice(5, 5);
      return;
    }

    virtual void TearDown(void) {
      delete one;
      delete two;
      delete three;
      return;
    }
};

TEST_F(DiceTest, MinMax) {
  ASSERT_EQ(one->number(), one->min());
  ASSERT_EQ(one->number() * one->faces(), one->max());
}

TEST_F(DiceTest, Roll) {
  // heuristic to feel good that our rolls won't go out of bounds
  for (int i=0; i < 1000; ++i) {
    EXPECT_LE(one->min(), one->roll());
    EXPECT_GE(one->max(), one->roll());
    EXPECT_LE(two->min(), two->roll());
    EXPECT_GE(two->max(), two->roll());
    EXPECT_LE(three->min(), three->roll());
    EXPECT_GE(three->max(), three->roll());
  }
}

TEST_F(DiceTest, Average) {
  EXPECT_EQ(10, one->average());
  EXPECT_EQ(10, two->average());
  EXPECT_EQ(15, three->average());
}

TEST_F(DiceTest, Print) {
  EXPECT_EQ(0, strcmp("3d6", one->toString()));
  EXPECT_EQ(0, strcmp("1d20", two->toString()));
  EXPECT_EQ(0, strcmp("5d5", three->toString()));
}
