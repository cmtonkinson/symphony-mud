
#ifndef H_SYMPHONY_COMMAND
#define H_SYMPHONY_COMMAND

#include <set>
#include <string>
#include <vector>
#include "avatar.h"
#include "flagBank.h"

typedef std::map<std::string,std::string> OptionMap;

class Avatar;
class CommandTable;
class Being;

///////////////////////////////////////////// BASE CLASS /////////////////////////////////////////////
class Command {
  public:

    static const char STRING_SEPARATOR = ';';

    // constructors...
    Command(void);
    virtual ~Command(void);

    // public accessors...
    void            enabled(bool enabled)                             { _enabled = enabled; }
    bool            enabled(void) const                               { return _enabled; }
    void            playerOnly(bool playerOnly)                       { _playerOnly = playerOnly; }
    bool            playerOnly(void) const                            { return _playerOnly; }
    void            special(bool special)                             { _special = special; }
    bool            special(void) const                               { return _special; }
    void            social(bool social)                               { _social = social; }
    bool            social(void) const                                { return _social; }
    void            level(const unsigned short& level);
    unsigned short  level(void) const                                 { return _level; }
    void            allowedPositions(const unsigned long& positions)  { _allowedPositions = positions; }
    unsigned long   allowedPositions(void)                            { return _allowedPositions; }
    void            name(std::string name)                            { _name = name; }
    std::string     name(void) const                                  { return _name; }
    void            shortcut(std::string shortcut)                    { _shortcut = shortcut; }
    std::string     shortcut(void) const                              { return _shortcut; }
    std::set<short>&        arguments(void)                           { return _arguments; }
    const std::set<short>&  arguments(void) const                     { return _arguments; }
    std::vector<std::string>&       syntax(void)                      { return _syntax; }
    const std::vector<std::string>& syntax(void) const                { return _syntax; }
    void            brief(std::string brief)                          { _brief = brief; }
    std::string     brief(void) const                                 { return _brief; }
    OptionMap&        options(void)                                   { return _options; }
    const OptionMap&  options(void) const                             { return _options; }
    void            commandTable(CommandTable* commandTable)          { _commandTable = commandTable; }
    CommandTable*   commandTable(void)                                { return _commandTable; }
    void            avatar(Avatar* avatar)                            { _avatar = avatar; }
    Avatar*         avatar(void) const                                { return _avatar; }
    FlagBank&       flags(void)                                       { return _flags; }
    const FlagBank& flags(void) const                                 { return _flags; }

    // general methods...
    void            addSyntax(const short& num_args, std::string usage);
    const char*     printSyntax(void) const;
    void            allowedWhile(const unsigned long& positions)          { allowedPositions(positions); }
    void            notAllowedWhile(const unsigned long& positions)       { allowedPositions(~positions); }
    void            addOptions(std::string argument, std::string option_list);
    void            seeAlso(std::string command);
    virtual bool    execute(Being* being, const std::vector<std::string>& args) { return false; }

    bool            isEnabled(void) const     { return flags().test(COM_ENABLED); }
    bool            isPlayerOnly(void) const  { return flags().test(COM_PLAYERONLY); }
    bool            isSpecial(void) const     { return flags().test(COM_SPECIAL); }
    bool            isSocial(void) const      { return flags().test(COM_SOCIAL); }
    bool            isAudible(void) const     { return flags().test(COM_AUDIBLE); }
    bool            isTactile(void) const     { return flags().test(COM_TACTILE); }

  private:
    // data...
    bool                      _enabled;           // can this Command be used?
    bool                      _playerOnly;        // is this command available for npcs? (automatically set when level > HERO)
    bool                      _special;           // is this a command for a special mode (like Nedit)?
    bool                      _social;            // is this a social Command?
    unsigned short            _level;             // what is the minimum level required to access this Command?
    std::set<short>           _arguments;         // number of arguments this Command expects?
    unsigned long             _allowedPositions;  // from what positions (sitting, standing, etc) can this Command be executed?
    std::string               _name;              // name of the Command
    std::string               _shortcut;          // does it have a shortcut?
    std::vector<std::string>  _syntax;            // how this Command is used.
    std::string               _brief;             // terse explanation of the command.
    std::set<std::string>     _seeAlso;           // set of other commands to reference while trawling the documentation.
    OptionMap                 _options;           // lists any finite and enumerable options for the help view
    CommandTable*             _commandTable;      // in which CommandTable does this Command live?
    Avatar*                   _avatar;            // automatically assigned from the Being* parameter when _playerOnly is set
    FlagBank                  _flags;
};

/* NEED TO RECTIFY Command::_shortcut std::string vs. char debate! */

#define DEF_COMMAND(NAME)                                                           \
class NAME: public Command {                                                        \
  public:                                                                           \
    NAME(void);                                                                     \
    virtual ~NAME(void) { }                                                         \
                                                                                    \
    virtual bool execute(Being* being, const std::vector<std::string>& args); \
};                                                                                  \

///////////////////////////////////////////// SOCIALS /////////////////////////////////////////////
class SocialCommand: public Command {
  public:
    // constructors...
    SocialCommand(void);
    SocialCommand(std::string name_, std::string creator_);
    virtual ~SocialCommand(void);

    // public accessors...
    void          creator(std::string creator)            { _creator = creator; }
    std::string   creator(void) const                     { return _creator; }

    void          targetNone(bool targetNone)             { _targetNone = targetNone; }
    bool          targetNone(void) const                  { return _targetNone; }
    void          noneActor(std::string noneActor)        { _noneActor = noneActor; }
    std::string   noneActor(void) const                   { return _noneActor; }
    void          noneRoom(std::string noneRoom)          { _noneRoom = noneRoom; }
    std::string   noneRoom(void) const                    { return _noneRoom; }

    void          targetSelf(bool targetSelf)             { _targetSelf = targetSelf; }
    bool          targetSelf(void) const                  { return _targetSelf; }
    void          selfActor(std::string selfActor)        { _selfActor = selfActor; }
    std::string   selfActor(void) const                   { return _selfActor; }
    void          selfRoom(std::string selfRoom)          { _selfRoom = selfRoom; }
    std::string   selfRoom(void) const                    { return _selfRoom; }

    void          targetVictim(bool targetVictim)         { _targetVictim = targetVictim; }
    bool          targetVictim(void) const                { return _targetVictim; }
    void          victimActor(std::string victimActor)    { _victimActor = victimActor; }
    std::string   victimActor(void) const                 { return _victimActor; }
    void          victimVictim(std::string victimVictim)  { _victimVictim = victimVictim; }
    std::string   victimVictim(void) const                { return _victimVictim; }
    void          victimRoom(std::string victimRoom)      { _victimRoom = victimRoom; }
    std::string   victimRoom(void) const                  { return _victimRoom; }

    // general methods...
    void                  initialize(void);
    void                  destroy(void);
    void                  save(void);
    static SocialCommand* load(std::string filename);
    virtual bool          execute(Being* being, const std::vector<std::string>& args);

  private:
    std::string   _creator;
    /* Can the social be entered without any targets?  (example: "blink") */
    bool          _targetNone;
    std::string   _noneActor;
    std::string   _noneRoom;
    /* Can the social be directed at oneself?  (example: "point self") */
    bool          _targetSelf;
    std::string   _selfActor;
    std::string   _selfRoom;
    /* Can the social be directed at a victim?  (example: "slap john") */
    bool          _targetVictim;
    std::string   _victimActor;
    std::string   _victimVictim;
    std::string   _victimRoom;
};

#endif // #ifndef H_SYMPHONY_COMMAND
