/*
 * $Id: commandTable.h 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_COMMANDTABLE
#define H_SYMPHONY_COMMANDTABLE

#include <string>
#include <vector>

class Command;
class Creature;

class CommandTable {
  public:
    // constructors...
    CommandTable( void );
    virtual ~CommandTable( void );

    // public accessors...
    void                          name( const std::string& name )   { _name = name; }
    std::string                   name( void ) const                { return _name; }
    std::vector<Command*>&        commands( void )                  { return _commands; }
    const std::vector<Command*>&  commands( void ) const            { return _commands; }

    // general methods...
    void                      add( Command* command, bool special = false );
    Command*                  find( std::string& name );
    Command*                  find( const std::string& name );

  private:
    // data
    std::string             _name;
    std::vector<Command*>   _commands;
};

#define DEF_COMMANDTABLE(NAME)          \
class NAME: public CommandTable {       \
  public:                               \
    NAME( void );                       \
    ~NAME( void ) { }                   \
    static NAME&  Instance( void ) {    \
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

#endif // #ifndef H_SYMPHONY_COMMANDTABLE
