
#ifndef H_SYMPHONY_COMMANDTABLE
#define H_SYMPHONY_COMMANDTABLE

#include <string>
#include <vector>

class Command;
class Creature;

class CommandTable {
  public:
    // constructors...
    CommandTable(void);
    virtual ~CommandTable(void);

    // public accessors...
    void                          name(const std::string& name)   { _name = name; }
    std::string                   name(void) const                { return _name; }
    std::vector<Command*>&        commands(void)                  { return _commands; }
    const std::vector<Command*>&  commands(void) const            { return _commands; }

    // general methods...
    void                      add(Command* command, bool special = false);
    Command*                  find(std::string& name);
    Command*                  find(const std::string& name);

  private:
    // data
    std::string             _name;
    std::vector<Command*>   _commands;
};

#define DEF_COMMANDTABLE(NAME)          \
class NAME: public CommandTable {       \
  public:                               \
    NAME(void);                       \
    ~NAME(void) { }                   \
    static NAME&  Instance(void) {    \
      static NAME instance;             \
      return instance;                  \
    }                                   \
};

DEF_COMMANDTABLE(Commands)        // Main Commands
DEF_COMMANDTABLE(AeditCommands)   // Area edit Commands
DEF_COMMANDTABLE(MeditCommands)   // Mob edit Commands
DEF_COMMANDTABLE(OeditCommands)   // Object edit Commands
DEF_COMMANDTABLE(PeditCommands)   // Player edit Commands
DEF_COMMANDTABLE(ReditCommands)   // Room edit Commands
DEF_COMMANDTABLE(SeditCommands)   // Social command edit Commands
DEF_COMMANDTABLE(TeditCommands)   // Text edit Commands

// Ability commands are not as trivial

#endif // #ifndef H_SYMPHONY_COMMANDTABLE
