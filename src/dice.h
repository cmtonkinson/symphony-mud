
#ifndef H_SYMPHONY_DICE
#define H_SYMPHONY_DICE

#include <math.h>
#include <stdlib.h>
#include "estring.h"

class Dice {
  public:
    // constructors
    Dice(void);
    Dice(const Dice& ref);
    Dice(const unsigned short& number, const unsigned short& faces);
    ~Dice(void);

    // public accessor methods
    void            number(const unsigned short& number)  { _number = number; }
    unsigned short  number(void) const                    { return _number; }
    void            faces(const unsigned short& faces)    { _faces = faces; }
    unsigned short  faces(void) const                     { return _faces; }

    // general methods
    unsigned    roll(void) const;
    unsigned    average(void) const;
    unsigned    min(void) const                           { return number(); }
    unsigned    max(void) const                           { return number() * faces(); }
    const char* toString(void) const;

  private:
    unsigned short  _number;
    unsigned short  _faces;
};

#endif // #ifndef H_SYMPHONY_DICE
