/*
 * $Id: loadRuleObject.h 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_LOADRULE_OBJECT
#define H_SYMPHONY_LOADRULE_OBJECT

#include "loadRule.h"

class LoadRuleObject: public LoadRule {
  public:
    // constructors...
    LoadRuleObject( void );
    LoadRuleObject( ROW row );
    ~LoadRuleObject( void );

    // public accessor methods...
    void            preposition( const unsigned short& preposition )                  { _preposition = preposition; }
    unsigned short  preposition( void ) const                                         { return _preposition; }
    void            indirectObject( const unsigned long& indirectObject )             { _indirectObject = indirectObject; }
    unsigned long   indirectObject( void ) const                                      { return _indirectObject; }
    void            indirectObjectIndex( const unsigned short& indirectObjectIndex )  { _indirectObjectIndex = indirectObjectIndex; }
    unsigned short  indirectObjectIndex( void ) const                                 { return _indirectObjectIndex; }

    // general methods...
    virtual std::string notes( void ) const;
    virtual bool        save( void );
    virtual bool        commit( void );
    virtual bool        execute( std::list<Object*>& new_objects, std::list<Mob*>& new_mobs );

  private:
    unsigned short  _preposition;         // in, on, carry, wear
    unsigned long   _indirectObject;      // vnum of whatever the object will load in or on
    unsigned short  _indirectObjectIndex; // when _preposition is RESET_WEAR, where the object goes on the mob specified by _indirectObject
};

#endif // #ifndef H_SYMPHONY_LOADRULE_OBJECT
