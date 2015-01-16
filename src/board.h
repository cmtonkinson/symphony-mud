
#ifndef H_SYMPHONY_BOARD
#define H_SYMPHONY_BOARD

#include <map>
#include "note.h"


class Board {
  public:

    // board numbers
    static const unsigned GENERAL  = 1;
    static const unsigned CHANGES  = 2;
    static const unsigned ADMIN    = 3;
    static const unsigned NEWS     = 4;
    static const unsigned IMMORTAL = 5;
    static const unsigned BUILDING = 6;

    // constructors
    Board(const unsigned short& number);
    ~Board(void);

    // public accessors...
    void                                  number(const unsigned short& number)    { _number = number; }
    unsigned short                        number(void) const                      { return _number; }
    std::map<unsigned long,Note*>&        notes(void)                             { return _notes; }
    const std::map<unsigned long,Note*>&  notes(void) const                       { return _notes; }

    // general methods...
    const char*                           name(void) const;
    bool                                  load(void);
    void                                  save(void);

  private:
    // data...
    unsigned short                        _number;
    std::map<unsigned long,Note*>         _notes;
};

#endif // #ifndef H_SYMPHONY_AREA
