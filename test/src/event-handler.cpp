
#include <gtest/gtest.h>
#include <iostream>
#include "../../src/event-handler.h"

class Action: public Event {};

class OtherAction: public Event {
  public:
    OtherAction(void): s(0) {};
    short s;
};

class UniqueAction: public Event {
  public:
    UniqueAction(void): s(0) {};
    short s;
};

class TestHandler: public EventHandler {
  public:
    short n;

    bool doSomething(Action* event) {
      n |= 1;
      return true;
    }
    bool doSomethingElse(OtherAction* event) {
      n |= 2;
      return true;
    }
    bool doSomethingExtra(OtherAction* event) {
      n |= 4;
      return true;
    }
};

bool doSomethingUnique(UniqueAction* event) {
  event->s = 32;
  return true;
}

bool doSomethingUnique2(OtherAction* event) {
  event->s = 64;
  return true;
}

class EventHandlerTest: public ::testing::Test {
  protected:
    TestHandler* reactor;

    virtual void SetUp(void) {
      reactor = new TestHandler();
      reactor->n = 0;
      return;
    }

    virtual void TearDown(void) {
      delete reactor;
      return;
    }
};

TEST_F(EventHandlerTest, OneFunction) {
  UniqueAction* ua = new UniqueAction();
  EXPECT_EQ(0, ua->s);
  reactor->registerHandler(&doSomethingUnique);
  reactor->handle(ua);
  EXPECT_EQ(32, ua->s);
}

TEST_F(EventHandlerTest, OneMethod) {
  reactor->registerHandler(reactor, &TestHandler::doSomething);
  reactor->handle(new Action());
  EXPECT_EQ(1, reactor->n & 1);
}

TEST_F(EventHandlerTest, TwoMethods) {
  reactor->registerHandler(reactor, &TestHandler::doSomething);
  reactor->registerHandler(reactor, &TestHandler::doSomethingElse);
  reactor->handle(new Action());
  reactor->handle(new OtherAction());
  EXPECT_EQ(1, reactor->n & 1);
  EXPECT_EQ(2, reactor->n & 2);
}

TEST_F(EventHandlerTest, MultipleStuff) {
  OtherAction* oa = new OtherAction();
  reactor->registerHandler(reactor, &TestHandler::doSomethingElse);
  reactor->registerHandler(reactor, &TestHandler::doSomethingExtra);
  reactor->registerHandler(&doSomethingUnique2);
  reactor->handle(oa);
  EXPECT_EQ(2, reactor->n & 2);
  EXPECT_EQ(4, reactor->n & 4);
  EXPECT_EQ(64, oa->s);
}
