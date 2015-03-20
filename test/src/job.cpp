
#include <gtest/gtest.h>
#include <iostream>
#include "../../src/event-handler-method.h"
#include "../../src/job.h"

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

class JobTest: public ::testing::Test {
  protected:
    Job* j1;
    Job* j2;
    Job* j3;
    Thing* thing;

    template <class T,class EventT>
    Job* createJob(time_t when, T* item, bool (T::*method)(EventT*)) {
      return new Job(when, new EventHandlerMethod<T,EventT>(item, method));
    }

    virtual void SetUp(void) {
      thing = new Thing();
      thing->n = 0;
      j1 = createJob(time(NULL)-2, thing, &Thing::actOne);
      j2 = createJob(time(NULL),   thing, &Thing::actTwo);
      j3 = createJob(time(NULL)+2, thing, &Thing::actThree);
      return;
    }

    virtual void TearDown(void) {
      delete thing;
      delete j1;
      delete j2;
      delete j3;
      return;
    }
};

TEST_F(JobTest, TestReadyMethod) {
  EXPECT_TRUE(j1->ready());
  EXPECT_TRUE(j2->ready());
  EXPECT_FALSE(j3->ready());
}

TEST_F(JobTest, TestFireMethod) {
  j1->fire();
  EXPECT_EQ(1, thing->n);
  j2->fire();
  EXPECT_EQ(2, thing->n);
  j3->fire();
  EXPECT_EQ(6, thing->n);
}
