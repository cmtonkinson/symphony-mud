
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
    Dice(unsigned number, unsigned faces);
    ~Dice(void);

    // public accessor methods
    void      number(unsigned number)   { _number = number; }
    unsigned  number(void) const        { return _number; }
    void      faces(unsigned faces)     { _faces = faces; }
    unsigned  faces(void) const         { return _faces; }

    // general methods
    unsigned    roll(void) const;
    unsigned    average(void) const;
    unsigned    min(void) const         { return number(); }
    unsigned    max(void) const         { return number() * faces(); }
    const char* toString(void) const;

    std::string serialize(void) const;
    void        unserialize(std::string ser);

  private:
    unsigned  _number;
    unsigned  _faces;
};

#endif // #ifndef H_SYMPHONY_DICE
