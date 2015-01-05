/*
 * $Id: schedule.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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
#include <iostream>
#include "../../src/schedule.h"

class Thing {
  public:
    int n;
    bool actOne( Job* j ) {
      n = 1;
      return true;
    }
    bool actTwo( Job* j ) {
      n *= 2;
      return true;
    }
    bool actThree( Job* j ) {
      n *= 3;
      return true;
    }
};

class ScheduleTest: public ::testing::Test {
  protected:
    Schedule* s;
    Thing* thing;

    virtual void SetUp( void ) {
      thing = new Thing();
      thing->n = 0;
      s = new Schedule();
      s->schedule( time( NULL ) - 1, thing, &Thing::actOne );
      s->schedule( time( NULL ) + 2, thing, &Thing::actTwo );
      s->schedule( time( NULL ) + 2, thing, &Thing::actThree );
      return;
    }

    virtual void TearDown( void ) {
      delete thing;
      delete s;
      return;
    }
};

TEST_F( ScheduleTest, TestFire ) {
  EXPECT_EQ( 3, s->size() );
  EXPECT_TRUE( s->fire() );
  EXPECT_FALSE( s->fire() );
  EXPECT_EQ( 2, s->size() );
  sleep( 3 );
  EXPECT_TRUE( s->fire() );
  EXPECT_TRUE( s->fire() );
  EXPECT_FALSE( s->fire() );
  EXPECT_EQ( 0, s->size() );
}
