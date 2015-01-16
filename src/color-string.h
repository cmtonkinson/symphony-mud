
#ifndef H_SYMPHONY_COLOR_STRING
#define H_SYMPHONY_COLOR_STRING

#include <stdio.h>
#include <stdlib.h>
#include <string>

class ColorString: public std::string {
  public:
    // constructors
    ColorString(void);
    ColorString(const std::string& ref);
    ColorString(const ColorString& ref);
    ~ColorString(void);

    // operators
    std::string& operator = (const std::string& ref);

    /* return the length of the string ignoring color codes */
    size_t      effectiveLength(void) const;
    /* return a substring ignoring color codes */
    std::string effectiveSubstr(size_t pos = 0, size_t n = npos) const;
    /* return a string sans color codes */
    std::string stripColor(void) const;
    /* return a string where color codes have been cast to the `\033[` format */
    std::string interpretColor(void) const;

    // static methods
    static const char*  getColorCode(const char& c);
};

#endif // !H_SYMPHONY_COLOR_STRING
