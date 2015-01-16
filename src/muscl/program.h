
#ifndef SYMPHONY_PROGRAM
#define SYMPHONY_PROGRAM

#include <string>
#include <vector>
#include "record.h"

using std::string;
using std::vector;

typedef vector<Record> CodeVect;

/*
 * This class handles a few important abstractions:
 *   1) Keeps compiled code organized together with its source
 *   2) Manages program metadata
 *   3) Keeps file backings of all data
 */
class Program {
  public:
    Program(void);
    Program(const string& name);
    ~Program(void);

    string                name(void) const              { return _name; }
    bool                  source(const string& source);
    string                source(void) const            { return _source; }
    CodeVect const* const instructions(void) const      { return &_instructions; }

    bool                  compile(string& feedback);

  private:
    string    _name;
    string    _source;
    CodeVect  _instructions;
};

#endif // #ifndef SYMPHONY_PROGRAM
