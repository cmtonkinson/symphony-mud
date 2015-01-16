
#ifndef H_SYMPHONY_BOARD
#define H_SYMPHONY_BOARD

#include <map>
#include "note.h"

// board numbers
#define BOARD_GENERAL   1
#define BOARD_CHANGES   2
#define BOARD_ADMIN     3
#define BOARD_NEWS      4
#define BOARD_IMMORTAL  5
#define BOARD_BUILDING  6

class Board {
  public:
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
