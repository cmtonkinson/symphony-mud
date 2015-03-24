
#ifndef H_SYMPHONY_NOTE
#define H_SYMPHONY_NOTE

#include <string>

class Note {
  public:
    // constructors
    Note(void);
    Note(unsigned board, std::string author);
    ~Note(void);

    // public accessor methods
    void          ID(unsigned ID)               { _ID = ID; }
    unsigned      ID(void) const                { return _ID; }
    void          board(unsigned board)         { _board = board; }
    unsigned      board(void) const             { return _board; }
    void          author(std::string author)    { _author = author; }
    std::string   author(void) const            { return _author; }
    void          subject(std::string subject)  { _subject = subject; }
    std::string   subject(void) const           { return _subject; }
    void          body(std::string body)        { _body = body; }
    std::string   body(void) const              { return _body; }

    std::string*  bodyp(void) { return &_body; } // needs to be exposed for the text editor

    // general methods
    void save(void);
    void destroy(void);

  private:
    unsigned      _ID;
    unsigned      _board;
    std::string   _author;
    std::string   _subject;
    std::string   _body;
};

#endif // #ifndef H_SYMPHONY_ZONE
