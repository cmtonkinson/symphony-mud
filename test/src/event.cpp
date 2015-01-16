
#include <gtest/gtest.h>
#include "../../src/event.h"

class EventTest: public ::testing::Test {
  protected:


    virtual void SetUp(void) {

      return;
    }

    virtual void TearDown(void) {

      return;
    }
};

TEST_F(EventTest, BasicTest) {
  Event* e = new Event();
  delete e;
}
