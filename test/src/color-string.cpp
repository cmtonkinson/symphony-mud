
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
