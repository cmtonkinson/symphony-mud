
#ifndef H_SYMPHONY_AVATAR
#define H_SYMPHONY_AVATAR

#include <string>
#include "color-string.h"
#include "creature.h"
#include "enumTable.h"
#include "flagTable.h"
#include "socket.h"

class Area;
class Object;
class ObjContainer;
class IOHandler;
class Note;

class Avatar: public Creature {
  public:
    // constructors...
    Avatar(Socket* socket_ = nullptr);
    virtual ~Avatar(void);

    // general methods...
    virtual bool      isAvatar(void) const        { return true; }

    Status&           status(void)                { return _status; }
    const Status&     status(void) const          { return _status; }
    bool              isConnecting(void) const    { return status().number() == CONNECTING; }
    bool              isCreating(void) const      { return status().number() == CREATING; }
    bool              isConnected(void) const     { return status().number() == CONNECTED; }
    Composing&        composing(void)             { return _composing; }
    const Composing&  composing(void) const       { return _composing; }

    bool            create(void);
    virtual bool    save(void);
    virtual bool    load(void);
    virtual bool    destroy(void);
    void            changeName(std::string name);
    bool            markForDeletion(const unsigned short& value);
    virtual void    send(std::string message);
    virtual void    send(const char* format, ...);
    time_t          secondsLoggedOn(void)         { return time(NULL) - _loggedOn; }
    std::string     stringLoggedOn(void);
    bool            hasInput(void);
    std::string     getInput(void);
    bool            hasOutput(void);
    void            flushOutput(void);
    bool            checkPassword(std::string password);
    std::string     listWhoFlags(void);
    void            restoreRoom(void);
    void            adjustStartingStats(void);

    // public accessors...
    void            socket(Socket* socket)                        { _socket = socket; }
    Socket*         socket(void)                                  { return _socket; }
    void            output(std::string output)                    { _output = output; }
    ColorString     output(void) const                            { return _output; }
    virtual void    disconnected(const bool& disconnected)        { _disconnected = disconnected; }
    virtual bool    disconnected(void) const                      { return _disconnected; }
    void            deleteMe(const bool& deleteMe)                { _deleteMe = deleteMe; }
    bool            deleteMe(void) const                          { return _deleteMe; }
    void            password(std::string password)                { _password = password; }
    std::string     password(void)                                { return _password; }
    void            roomNumber(unsigned long roomNumber)          { _roomNumber = roomNumber; }
    unsigned long   roomNumber(void) const                        { return _roomNumber; }

    FlagBank&       adminFlags(void)                              { return _adminFlags; }
    FlagBank&       channelFlags(void)                            { return _channelFlags; }
    FlagBank&       whoFlags(void)                                { return _whoFlags; }
    void            gechoColor(char gechoColor)                   { _gechoColor = gechoColor; }
    char            gechoColor(void) const                        { return _gechoColor; }
    void            replyTo(std::string replyTo)                  { _replyTo = replyTo; }
    std::string     replyTo(void)                                 { return _replyTo; }
    virtual void    room(Room* room);
    virtual Room*   room(void) const                              { return Creature::room(); }
    Mode&           mode(void)                                    { return _mode; }
    const Mode&     mode(void) const                              { return _mode; }
    void            pedit(Avatar* pedit)                          { _pedit = pedit; }
    Avatar*         pedit(void)                                   { return _pedit; }
    void            aedit(Area* aedit)                            { _aedit = aedit; }
    Area*           aedit(void)                                   { return _aedit; }
    void            oedit(Object* oedit)                          { _oedit = oedit; }
    Object*         oedit(void)                                   { return _oedit; }
    void            medit(Mob* medit)                             { _medit = medit; }
    Mob*            medit(void)                                   { return _medit; }
    void            sedit(SocialCommand* sedit)                   { _sedit = sedit; }
    SocialCommand*  sedit(void)                                   { return _sedit; }
    void            note(Note* note)                              { _note = note; }
    Note*           note(void) const                              { return _note; }
    void            title(std::string title);
    const char*     title(void) const                             { return _title.c_str(); }
    void            poofin(std::string poofin)                    { _poofin = poofin; }
    const char*     poofin(void) const                            { return _poofin.c_str(); }
    void            poofout(std::string poofout)                  { _poofout = poofout; }
    const char*     poofout(void) const                           { return _poofout.c_str(); }
    void            board(unsigned short board)                   { _board = board; }
    unsigned short  board(void) const                             { return _board; }
    void            age(unsigned short age)                       { _age = age; }
    unsigned short  age(void) const                               { return _age; }
    void            bankGold(unsigned long bankGold)              { _bankGold = bankGold; }
    unsigned long   bankGold(void) const                          { return _bankGold; }
    void            bankSilver(unsigned long bankSilver)          { _bankSilver = bankSilver; }
    unsigned long   bankSilver(void) const                        { return _bankSilver; }

    // Combat...
    virtual void    whatHappensWhenIDie(void);

  private:
    // Low level/System members...
    Socket*                 _socket;
    ColorString             _output;
    Status                  _status;
    time_t                  _loggedOn;
    Composing               _composing;
    bool                    _disconnected;
    bool                    _deleteMe;
    std::string             _password;    // only used during creation
    unsigned long           _roomNumber;  // only used during login & reboot

    // Game-related members...
    FlagBank                _adminFlags;
    FlagBank                _channelFlags;
    FlagBank                _whoFlags;
    char                    _gechoColor;
    std::string             _replyTo;
    Mode                    _mode;
    Avatar*                 _pedit;
    Area*                   _aedit;
    Object*                 _oedit;
    Mob*                    _medit;
    SocialCommand*          _sedit;
    Note*                   _note;
    std::string             _title;
    std::string             _poofin;
    std::string             _poofout;
    unsigned short          _age;
    unsigned long           _bankGold;
    unsigned long           _bankSilver;
    unsigned short          _board;

    // Avatar-specific private methods...
    void                    processOutput(std::string text);
};

#endif // #ifndef H_SYMPHONY_AVATAR
