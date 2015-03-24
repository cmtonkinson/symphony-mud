
#ifndef H_SYMPHONY_COMMANDTABLE
#define H_SYMPHONY_COMMANDTABLE

#include <string>
#include <vector>

class Command;
class Being;

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

#endif // #ifndef H_SYMPHONY_COMMANDTABLE
