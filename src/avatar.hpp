
#ifndef H_SYMPHONY_AVATAR
#define H_SYMPHONY_AVATAR

#include <string>
#include "being.hpp"
#include "color-string.hpp"
#include "currency.hpp"
#include "enum-table.hpp"
#include "flag-table.hpp"
#include "socket.hpp"

class Zone;
class Item;
class ItemContainer;
class IOHandler;
class Note;

class Avatar: public Being {
  public:

    // Deletion status...
    static const unsigned DO_NOT_DELETE   = 0;
    static const unsigned DELETE_ON_LOGIN = 1;
    static const unsigned DESTROY_NOW     = 2;

    // constructors...
    Avatar(Socket* socket_ = nullptr);
    virtual ~Avatar(void);

    // general methods...
    virtual void      _animate(void);
    virtual bool      isAvatar(void) const        { return true; }

    Status&           status(void)                { return _status; }
    const Status&     status(void) const          { return _status; }
    bool              isConnecting(void) const    { return status().number() == CONNECTING; }
    bool              isCreating(void) const      { return status().number() == CREATING; }
    bool              isConnected(void) const     { return status().number() == CONNECTED; }
    Composing&        composing(void)             { return _composing; }
    const Composing&  composing(void) const       { return _composing; }

    virtual std::string   printInformation(void) const;


    // Persistence...
    void            save(void);
    bool            load(void);
    void            changeName(std::string name);
    bool            rename(std::string new_name);
    std::string     hashPassword(std::string source);
    void            setPassword(std::string str);
    bool            checkPassword(std::string attempt);
    void            destroy(void);
    bool            shouldDelete(void) const;
    bool            shouldDestroy(void) const;

    virtual void    send(std::string message);
    virtual void    send(const char* format, ...);
    virtual void    indentedSend(unsigned spaces, std::string message);
    virtual void    indentedSend(unsigned spaces, const char* format, ...);

    time_t          secondsLoggedOn(void) const         { return time(NULL) - _loggedOn; }
    std::string     stringLoggedOn(void) const;
    bool            hasInput(void);
    std::string     getInput(void);
    bool            hasOutput(void);
    void            flushOutput(void);
    std::string     listWhoFlags(void) const;
    void            restoreRoom(void);
    void            adjustStartingStats(void);

    // Shortcuts...
    std::string           ip(void) const;
    virtual Avatar*       toAvatar(void)          { return this; }
    virtual const Avatar* toAvatar(void) const    { return this; }

    // public accessors...
    void            socket(Socket* socket)                        { _socket = socket; }
    Socket*         socket(void)                                  { return _socket; }
    const Socket*   socket(void) const                            { return _socket; }
    void            output(std::string output)                    { _output = output; }
    ColorString     output(void) const                            { return _output; }
    virtual void    disconnected(const bool& disconnected)        { _disconnected = disconnected; }
    void            deletionStatus(unsigned deletionStatus)       { _deletionStatus = deletionStatus; }
    unsigned        deletionStatus(void) const                    { return _deletionStatus; }
    virtual bool    disconnected(void) const                      { return _disconnected; }
    void            password(std::string password)                { _password = password; }
    std::string     password(void)                                { return _password; }
    void            roomNumber(unsigned long roomNumber)          { _roomNumber = roomNumber; }
    unsigned long   roomNumber(void) const                        { return _roomNumber; }
    void            bigBrother(unsigned bigBrother)               { _bigBrother = bigBrother; }
    unsigned        bigBrother(void) const                        { return _bigBrother; }
    FlagBank&       adminFlags(void)                              { return _adminFlags; }
    FlagBank&       channelFlags(void)                            { return _channelFlags; }
    FlagBank&       whoFlags(void)                                { return _whoFlags; }
    const FlagBank& whoFlags(void) const                          { return _whoFlags; }
    void            gechoColor(char gechoColor)                   { _gechoColor = gechoColor; }
    char            gechoColor(void) const                        { return _gechoColor; }
    void            replyTo(std::string replyTo)                  { _replyTo = replyTo; }
    std::string     replyTo(void)                                 { return _replyTo; }
    virtual void    room(Room* room);
    virtual Room*   room(void) const                              { return Being::room(); }
    Mode&           mode(void)                                    { return _mode; }
    const Mode&     mode(void) const                              { return _mode; }
    void            pedit(Avatar* pedit)                          { _pedit = pedit; }
    Avatar*         pedit(void)                                   { return _pedit; }
    void            zedit(Zone* zedit)                            { _zedit = zedit; }
    Zone*           zedit(void)                                   { return _zedit; }
    void            iedit(Item* iedit)                            { _iedit = iedit; }
    Item*           iedit(void)                                   { return _iedit; }
    void            nedit(Npc* nedit)                             { _nedit = nedit; }
    Npc*            nedit(void)                                   { return _nedit; }
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
    void            bankMoney(unsigned value)                     { _bankMoney.value(value); }
    Currency&       bankMoney(void)                               { return _bankMoney; }
    const Currency& bankMoney(void) const                         { return _bankMoney; }

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
    unsigned                _deletionStatus;
    std::string             _password;    // only used during creation
    unsigned long           _roomNumber;  // only used during login & reboot

    // Game-related members...
    unsigned                _bigBrother;
    FlagBank                _adminFlags;
    FlagBank                _channelFlags;
    FlagBank                _whoFlags;
    char                    _gechoColor;
    std::string             _replyTo;
    Mode                    _mode;
    Avatar*                 _pedit;
    Zone*                   _zedit;
    Item*                   _iedit;
    Npc*                    _nedit;
    SocialCommand*          _sedit;
    Note*                   _note;
    std::string             _title;
    std::string             _poofin;
    std::string             _poofout;
    unsigned short          _age;
    Currency                _bankMoney;
    unsigned short          _board;

    // Avatar-specific private methods...
    void      changeIdentifiers(std::string name);
    void      processOutput(std::string text);
};

#endif // #ifndef H_SYMPHONY_AVATAR
