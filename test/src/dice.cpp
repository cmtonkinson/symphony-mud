/*
 * $Id: dice.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
