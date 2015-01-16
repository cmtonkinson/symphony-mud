/*
 * $Id: color-string.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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
#include "../../src/color-string.h"

class ColorStringTest: public ::testing::Test {
  protected:
    std::string _plain;
    std::string _formatted;
    std::string _mangled;
    ColorString _special;

    virtual void SetUp(void) {
      _plain = "Hello {there}";
      _formatted = "Hell{Go{x {{there}";
      _mangled = "Hell\033[0;32;1mo\033[0;0;0m {there}";
      _special = _formatted;
      return;
    }

    virtual void TearDown(void) {
      return;
    }
};

TEST_F(ColorStringTest, Assignment) {
  EXPECT_EQ(_formatted, _special);
}

TEST_F(ColorStringTest, Length) {
  EXPECT_EQ(_formatted.length(), _special.length());
  EXPECT_EQ(_plain.length(), _special.effectiveLength());
}

TEST_F(ColorStringTest, StripColor) {
  EXPECT_EQ(_plain, _special.stripColor());
  EXPECT_EQ(_plain.length(), _special.stripColor().length());
}

TEST_F(ColorStringTest, Substring) {
  EXPECT_EQ(std::string("l{Go{x {{th"), _special.effectiveSubstr(3, 6));
  EXPECT_EQ(std::string("l{Go{x {{there}"), _special.effectiveSubstr(3));
}
