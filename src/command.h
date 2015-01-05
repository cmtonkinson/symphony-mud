/*
 * $Id: command.h 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_COMMAND
#define H_SYMPHONY_COMMAND

#include <set>
#include <string>
#include <vector>
#include "avatar.h"
#include "flagBank.h"
#include "mysql.h"

typedef std::map<std::string,std::string> OptionMap;

class CommandTable;
class Creature;

class Command {
  public:
    // constructors...
    Command( void );
    virtual ~Command( void );

    // public accessors...
    void            enabled( const bool& enabled )                          { _enabled = enabled; }
    bool            enabled( void ) const                                   { return _enabled; }
    void            playerOnly( const bool& playerOnly )                    { _playerOnly = playerOnly; }
    bool            playerOnly( void ) const                                { return _playerOnly; }
    void            special( const bool& special )                          { _special = special; }
    bool            special( void ) const                                   { return _special; }
    void            social( const bool& social )                            { _social = social; }
    bool            social( void ) const                                    { return _social; }
    void            level( const unsigned short& level );
    unsigned short  level( void ) const                                     { return _level; }
    void            allowedPositions( const unsigned long& positions )      { _allowedPositions = positions; }
    unsigned long   allowedPositions( void )                                { return _allowedPositions; }
    void            name( const std::string& name )                         { _name = name; }
    std::string     name( void ) const                                      { return _name; }
    void            shortcut( const std::string& shortcut )                 { _shortcut = shortcut; }
    std::string     shortcut( void ) const                                  { return _shortcut; }
    std::set<short>&        arguments( void )                               { return _arguments; }
    const std::set<short>&  arguments( void ) const                         { return _arguments; }
    std::vector<std::string>&       syntax( void )                          { return _syntax; }
    const std::vector<std::string>& syntax( void ) const                    { return _syntax; }
    void            brief( const std::string& brief )                       { _brief = brief; }
    std::string     brief( void ) const                                     { return _brief; }
    OptionMap&        options( void )                                       { return _options; }
    const OptionMap&  options( void ) const                                 { return _options; }
    void            commandTable( CommandTable* commandTable )              { _commandTable = commandTable; }
    CommandTable*   commandTable( void )                                    { return _commandTable; }
    void            avatar( Avatar* avatar )                                { _avatar = avatar; }
    Avatar*         avatar( void ) const                                    { return _avatar; }
    FlagBank&       flags( void )                                           { return _flags; }
    const FlagBank& flags( void ) const                                     { return _flags; }

    // general methods...
    void            addSyntax( const short& num_args, const std::string& usage );
    const char*     printSyntax( void ) const;
    void            allowedWhile( const unsigned long& positions )          { allowedPositions( positions ); }
    void            notAllowedWhile( const unsigned long& positions )       { allowedPositions( ~positions ); }
    void            addOptions( const std::string& argument, const std::string& option_list );
    virtual bool    execute( Creature* creature, const std::vector<std::string>& args ) { return false; }

    bool            isEnabled( void ) const     { return flags().test( COM_ENABLED ); }
    bool            isPlayerOnly( void ) const  { return flags().test( COM_PLAYERONLY ); }
    bool            isSpecial( void ) const     { return flags().test( COM_SPECIAL ); }
    bool            isSocial( void ) const      { return flags().test( COM_SOCIAL ); }
    bool            isAudible( void ) const     { return flags().test( COM_AUDIBLE ); }
    bool            isTactile( void ) const     { return flags().test( COM_TACTILE ); }

  private:
    // data...
    bool                      _enabled;           // can this Command be used?
    bool                      _playerOnly;        // is this command available for mobs? (automatically set when level > HERO)
    bool                      _special;           // is this a command for a special mode (like Medit)?
    bool                      _social;            // is this a social Command?
    unsigned short            _level;             // what is the minimum level required to access this Command?
    std::set<short>           _arguments;         // number of arguments this Command expects?
    unsigned long             _allowedPositions;  // from what positions (sitting, standing, etc) can this Command be executed?
    std::string               _name;              // name of the Command
    std::string               _shortcut;          // does it have a shortcut?
    std::vector<std::string>  _syntax;            // how this Command is used.
    std::string               _brief;             // terse explanation of the command.
    OptionMap                 _options;           // lists any finite and enumerable options for the help view
    CommandTable*             _commandTable;      // in which CommandTable does this Command live?
    Avatar*                   _avatar;            // automatically assigned from the Creature* parameter when _playerOnly is set
    FlagBank                  _flags;
};

/* NEED TO RECTIFY Command::_shortcut std::string vs. char debate! */

#define DEF_COMMAND(NAME)                                                 \
class NAME: public Command {                                              \
  public:                                                                 \
    NAME( void );                                                         \
    virtual ~NAME( void ) { }                                             \
                                                                          \
    virtual bool execute( Creature* creature, const std::vector<std::string>& args );  \
};                                                                        \

class SocialCommand: public Command {
  public:
    // constructors...
    SocialCommand( void );
    SocialCommand( const std::string& name, const unsigned long& avatarID );
    SocialCommand( ROW row );
    virtual ~SocialCommand( void );

    // public accessors...
    void          ID( const unsigned long& ID )                         { _ID = ID; }
    unsigned long ID( void ) const                                      { return _ID; }
    void          creator( const unsigned long& creator )               { _creator = creator; }
    unsigned long creator( void ) const                                 { return _creator; }

    void          targetNone( const bool& targetNone )                  { _targetNone = targetNone; }
    bool          targetNone( void ) const                              { return _targetNone; }
    void          noneActor( const std::string& noneActor )             { _noneActor = noneActor; }
    std::string   noneActor( void ) const                               { return _noneActor; }
    void          noneRoom( const std::string& noneRoom )               { _noneRoom = noneRoom; }
    std::string   noneRoom( void ) const                                { return _noneRoom; }

    void          targetSelf( const bool& targetSelf )                  { _targetSelf = targetSelf; }
    bool          targetSelf( void ) const                              { return _targetSelf; }
    void          selfActor( const std::string& selfActor )             { _selfActor = selfActor; }
    std::string   selfActor( void ) const                               { return _selfActor; }
    void          selfRoom( const std::string& selfRoom )               { _selfRoom = selfRoom; }
    std::string   selfRoom( void ) const                                { return _selfRoom; }

    void          targetVictim( const bool& targetVictim )              { _targetVictim = targetVictim; }
    bool          targetVictim( void ) const                            { return _targetVictim; }
    void          victimActor( const std::string& victimActor )         { _victimActor = victimActor; }
    std::string   victimActor( void ) const                             { return _victimActor; }
    void          victimVictim( const std::string& victimVictim )       { _victimVictim = victimVictim; }
    std::string   victimVictim( void ) const                            { return _victimVictim; }
    void          victimRoom( const std::string& victimRoom )           { _victimRoom = victimRoom; }
    std::string   victimRoom( void ) const                              { return _victimRoom; }

    // general methods...
    void          save( void );
    bool          destroy( void );
    virtual bool  execute( Creature* creature, const std::vector<std::string>& args );

  private:
    // data...
    unsigned long _ID;
    unsigned long _creator;
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
