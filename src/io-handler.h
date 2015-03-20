
#ifndef H_SYMPHONY_IO_HANDLER
#define H_SYMPHONY_IO_HANDLER

#include <map>
#include <string>

class CommandTable;
class Being;

class IOHandler {
  private:
    Being*                           _being;
    Avatar*                             _avatar;
    CommandTable*                       _commandTable;
    std::map<std::string,void*>   	    _state;
    std::string                         _lastInput;

  public:
    // Constructors...
    IOHandler(Being* being = NULL);
    virtual ~IOHandler(void)                                                        { }

    // Public accessor methods...
    void                                being(Being* being)              { _being = being; }
    Being*                           being(void)                            { return _being; }
    void                                avatar(Avatar* avatar)                    { _avatar = avatar; }
    Avatar*                             avatar(void)                              { return _avatar; }
    void                                commandTable(CommandTable* commandTable)  { _commandTable = commandTable; }
    CommandTable*                       commandTable(void)                        { return _commandTable; }
    std::map<std::string,void*>&  	    getState(void)                            { return _state; }
    const std::map<std::string,void*>&  getState(void) const                      { return _state; }
    void                                lastInput(const std::string& lastInput)   { _lastInput = lastInput; }
    std::string                         lastInput(void) const                     { return _lastInput; }

    // Public methods...
    virtual void                        activate(void)                            = 0;
    virtual void                        deactivate(void)                          = 0;
    //virtual bool                        handle(Being* being, const std::string& input) { return false; }
    virtual bool                        handle(const std::string& input);
    virtual bool                        handle(void);
    virtual std::string                 prompt(void)                              = 0;
};

#define DEF_TRIVIAL_IOHANDLER(NAME)                                       \
class NAME: public IOHandler {                                            \
  public:                                                                 \
    NAME(Being* being = NULL) {                                   \
      this->being(being);                                         \
      this->avatar((Avatar*)being);                                  \
      return;                                                             \
    }                                                                     \
    virtual ~NAME(void) { }                                             \
    virtual void        activate(void);                                 \
    virtual void        deactivate(void);                               \
    virtual bool        handle(void);                                   \
    virtual std::string prompt(void);                                   \
}                                                                         \

#define DEF_FULL_IOHANDLER(NAME)                                          \
class NAME: public IOHandler {                                            \
  public:                                                                 \
    NAME(Being* being = NULL) {                                   \
      this->being(being);                                         \
      this->avatar((Avatar*)being);                                  \
      return;                                                             \
    }                                                                     \
    virtual ~NAME(void) { }                                             \
    virtual void        activate(void);                                 \
    virtual void        deactivate(void);                               \
    virtual std::string prompt(void);                                   \
}                                                                         \

// Login...
DEF_TRIVIAL_IOHANDLER(LoginBannerIOHandler);
DEF_TRIVIAL_IOHANDLER(LoginNameIOHandler);
DEF_TRIVIAL_IOHANDLER(LoginPasswordIOHandler);
DEF_TRIVIAL_IOHANDLER(LoginDeleteIOHandler);

// Creation...
DEF_TRIVIAL_IOHANDLER(CreationNameIOHandler);
DEF_TRIVIAL_IOHANDLER(CreationPasswordIOHandler);
DEF_TRIVIAL_IOHANDLER(CreationPasswordConfirmIOHandler);
DEF_TRIVIAL_IOHANDLER(CreationGenderIOHandler);
DEF_TRIVIAL_IOHANDLER(CreationRaceIOHandler);
DEF_TRIVIAL_IOHANDLER(CreationClassIOHandler);
//DEF_TRIVIAL_IOHANDLER(CreationTotemIOHandler);
DEF_TRIVIAL_IOHANDLER(CreationSummaryIOHandler);

// Notes...
DEF_TRIVIAL_IOHANDLER(NoteGetSubjectIOHandler);
DEF_TRIVIAL_IOHANDLER(NoteWriteIOHandler);
DEF_TRIVIAL_IOHANDLER(NoteExitIOHandler);
DEF_TRIVIAL_IOHANDLER(NoteSendIOHandler);
DEF_TRIVIAL_IOHANDLER(NoteCancelIOHandler);

// Main IOHandler...
DEF_FULL_IOHANDLER(InputIOHandler);
// Editors...
DEF_FULL_IOHANDLER(AeditIOHandler); // areas
DEF_FULL_IOHANDLER(MeditIOHandler); // mobs
DEF_FULL_IOHANDLER(IeditIOHandler); // items
DEF_FULL_IOHANDLER(PeditIOHandler); // players
DEF_FULL_IOHANDLER(ReditIOHandler); // rooms
DEF_FULL_IOHANDLER(SeditIOHandler); // social commands
DEF_FULL_IOHANDLER(TeditIOHandler); // text editor

#endif // #ifndef H_SYMPHONY_IO_HANDLER
