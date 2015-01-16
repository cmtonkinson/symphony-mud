
#ifndef H_SYMPHONY_NOTE
#define H_SYMPHONY_NOTE

#include <string>
#include "mysql.h"

class Note {
  public:
    // constructors
    Note(const unsigned short& board, const unsigned long& author);
    Note(ROW row);
    ~Note(void);

    // public accessor methods
    void            ID(const unsigned long& ID)         { _ID = ID; }
    unsigned long   ID(void) const                      { return _ID; }
    void            board(const unsigned short& board)  { _board = board; }
    unsigned short  board(void) const                   { return _board; }
    void            author(const unsigned long& author) { _author = author; }
    unsigned long   author(void) const                  { return _author; }
    void            subject(const std::string& subject) { _subject = subject; }
    std::string     subject(void) const                 { return _subject; }
    void            body(const std::string& body)       { _body = body; }
    std::string     body(void) const                    { return _body; }
    std::string*    bodyp(void)                         { return &_body; } // needs to be exposed for the text editor

    // general methods
    bool            save(void);
    bool            destroy(void);

  private:
    unsigned long   _ID;
    unsigned short  _board;
    unsigned long   _author;
    std::string     _subject;
    std::string     _body;
};

#endif // #ifndef H_SYMPHONY_AREA
