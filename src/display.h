
#ifndef H_SYMPHONY_DISPLAY
#define H_SYMPHONY_DISPLAY

#include <cstdlib>
#include <cstring>
#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "being.h"
#include "regex.h"

class Display {
  private:

  public:
    Display(void);
    ~Display(void);

    static std::string  formatColumns(const std::vector<std::string>& items);
    static std::string  formatAction(const char* format, Being* being, void* arg1, void* arg2, Being* withRespectTo);
    static std::string  formatDamage(unsigned int damage);
};

#endif // #ifndef H_SYMPHONY_DISPLAY
