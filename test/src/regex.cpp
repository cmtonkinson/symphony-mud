/*
 * $Id: regex.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2009 Chris Tonkinson <cmtonkinson@gmail.com>
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
#include "../../src/regex.h"

class RegexTest: public ::testing::Test {
  protected:
    std::string one;
    std::string two;
    std::string three;
    std::string four;
    std::string five;
    std::vector<std::string>  v_one;
    std::vector<std::string>  v_two;
    std::vector<std::string>  v_three;
    std::vector<std::string>  v_four;
    std::vector<std::string>  v_five;
    std::vector<std::string>  v_six;
    std::vector<std::string>  test;

    virtual void SetUp( void ) {
      one = "get sword backpack";
      v_one.push_back( "get" );
      v_one.push_back( "sword" );
      v_one.push_back( "backpack" );
      two = "get 7*sword#3 chest"; // nonsensical, but it compounds both tests into one
      v_two.push_back( "get" );
      v_two.push_back( "7*sword#3" );
      v_two.push_back( "chest" );
      three = "get 'short sword' backpack";
      v_three.push_back( "get" );
      v_three.push_back( "'short sword'" );
      v_three.push_back( "backpack" );
      four = "get 'bronze dirk' 'leather sheath'";
      v_four.push_back( "get" );
      v_four.push_back( "'bronze dirk'" );
      v_four.push_back( "'leather sheath'" );
      five = "cast 'magic missle' 'seedy crook'";
      v_five.push_back( "cast" );
      v_five.push_back( "'magic missle'" );
      v_five.push_back( "'seedy crook'" );
      v_six.push_back( "cast 'magic missle' 'seedy crook'" );
      return;
    }

    virtual void TearDown( void ) {
      return;
    }
};

TEST_F( RegexTest, Split ) {
  test = Regex::split( one, 3 );
  EXPECT_TRUE( v_one == test );
  test = Regex::split( two, 3 );
  EXPECT_TRUE( v_two == test );
  test = Regex::split( three, 3 );
  EXPECT_TRUE( v_three == test );
  test = Regex::split( five, 0 );
  EXPECT_TRUE( v_six == test );
  test = Regex::split( "", 0 );
  ASSERT_EQ( 1, test.size() );
  EXPECT_TRUE( test[0] == "" );
}
