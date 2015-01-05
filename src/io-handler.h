/*
 * $Id: io-handler.h 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef H_SYMPHONY_IO_HANDLER
#define H_SYMPHONY_IO_HANDLER

#include <map>
#include <string>

class CommandTable;
class Creature;

class IOHandler {
  private:
    Creature*                           _creature;
    Avatar*                             _avatar;
    CommandTable*                       _commandTable;
    std::map<std::string,void*>   	    _state;
    std::string                         _lastInput;

  public:
    // Constructors...
    IOHandler( Creature* creature = NULL );
    virtual ~IOHandler( void )                                                        { }

    // Public accessor methods...
    void                                creature( Creature* creature )              { _creature = creature; }
    Creature*                           creature( void )                            { return _creature; }
    void                                avatar( Avatar* avatar )                    { _avatar = avatar; }
    Avatar*                             avatar( void )                              { return _avatar; }
    void                                commandTable( CommandTable* commandTable )  { _commandTable = commandTable; }
    CommandTable*                       commandTable( void )                        { return _commandTable; }
    std::map<std::string,void*>&  	    getState( void )                            { return _state; }
    const std::map<std::string,void*>&  getState( void ) const                      { return _state; }
    void                                lastInput( const std::string& lastInput )   { _lastInput = lastInput; }
    std::string                         lastInput( void ) const                     { return _lastInput; }

    // Public methods...
    virtual void                        activate( void )                            = 0;
    virtual void                        deactivate( void )                          = 0;
    //virtual bool                        handle( Creature* creature, const std::string& input ) { return false; }
    virtual bool                        handle( const std::string& input );
    virtual bool                        handle( void );
    virtual std::string                 prompt( void )                              = 0;
};

#define DEF_TRIVIAL_IOHANDLER(NAME)                                       \
class NAME: public IOHandler {                                            \
  public:                                                                 \
    NAME( Creature* creature = NULL ) {                                   \
      this->creature( creature );                                         \
      this->avatar( (Avatar*)creature );                                  \
      return;                                                             \
    }                                                                     \
    virtual ~NAME( void ) { }                                             \
    virtual void        activate( void );                                 \
    virtual void        deactivate( void );                               \
    virtual bool        handle( void );                                   \
    virtual std::string prompt( void );                                   \
}                                                                         \

#define DEF_FULL_IOHANDLER(NAME)                                          \
class NAME: public IOHandler {                                            \
  public:                                                                 \
    NAME( Creature* creature = NULL ) {                                   \
      this->creature( creature );                                         \
      this->avatar( (Avatar*)creature );                                  \
      return;                                                             \
    }                                                                     \
    virtual ~NAME( void ) { }                                             \
    virtual void        activate( void );                                 \
    virtual void        deactivate( void );                               \
    virtual std::string prompt( void );                                   \
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
DEF_FULL_IOHANDLER(OeditIOHandler); // objects
DEF_FULL_IOHANDLER(PeditIOHandler); // players
DEF_FULL_IOHANDLER(ReditIOHandler); // rooms
DEF_FULL_IOHANDLER(SeditIOHandler); // social commands
DEF_FULL_IOHANDLER(TeditIOHandler); // text editor

#endif // #ifndef H_SYMPHONY_IO_HANDLER
