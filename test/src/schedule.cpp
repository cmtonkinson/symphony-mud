
#include <gtest/gtest.h>
#include <iostream>
#include "../../src/schedule.h"

class Thing {
  public:
    int n;
    bool actOne(Job* j) {
      n = 1;
      return true;
    }
    bool actTwo(Job* j) {
      n *= 2;
      return true;
    }
    bool actThree(Job* j) {
      n *= 3;
      return true;
    }
};

class ScheduleTest: public ::testing::Test {
  protected:
    Schedule* s;
    Thing* thing;

    virtual void SetUp(void) {
      thing = new Thing();
      thing->n = 0;
      s = new Schedule();
      s->schedule(time(NULL) - 1, thing, &Thing::actOne);
      s->schedule(time(NULL) + 2, thing, &Thing::actTwo);
      s->schedule(time(NULL) + 2, thing, &Thing::actThree);
      return;
    }

    virtual void TearDown(void) {
      delete thing;
      delete s;
      return;
    }
};

TEST_F(ScheduleTest, TestFire) {
  EXPECT_EQ(3, s->size());
  EXPECT_TRUE(s->fire());
  EXPECT_FALSE(s->fire());
  EXPECT_EQ(2, s->size());
  sleep(3);
  EXPECT_TRUE(s->fire());
  EXPECT_TRUE(s->fire());
  EXPECT_FALSE(s->fire());
  EXPECT_EQ(0, s->size());
}
