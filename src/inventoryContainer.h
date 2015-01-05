/*
 * $Id: inventoryContainer.h 408 2010-06-16 00:40:36Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_CONTAINER_LIST
#define H_SYMPHONY_CONTAINER_LIST

#include <list>
#include <string>
#include "container.h"
#include "identifiers.h"

class Object;

class InventoryContainer: public Container {
  public:
    // constructors...
    InventoryContainer( std::string (Identifiers::*getName)( void ) const );
    InventoryContainer( const InventoryContainer& ref );
    virtual ~InventoryContainer( void );

    // Hack-job because I can't get getName() to return the method pointer
    // from within the copy constructor
    std::string (Identifiers::*_getName)( void ) const;

    // virtual methods....
    virtual void                add( Object* object );
    virtual void                add( const std::list<Object*>& objects );
    virtual void                remove( Object* object );
    virtual void                remove( const std::list<Object*>& objects );
    virtual void                purgeObjects( void );
    virtual std::list<Object*>  searchObjects( const std::string& q );
    virtual Object*             searchSingleObject( const std::string& q );

    // general methods...
    std::list<Object*>  searchObjects( const unsigned long& vnum );
    unsigned            howManyObjects( const unsigned long& vnum ) const;
    std::string         listObjects( bool compact = true ) const;

    // public accessor methods...
    std::list<Object*>&                                   objectList( void )        { return _objectList; }
    const std::list<Object*>&                             objectList( void ) const  { return _objectList; }
    std::string (Identifiers::*getName(void))(void) const                           { return _getName; }

  private:
    std::list<Object*> _objectList;
};

#endif // H_SYMPHONY_CONTAINER_LIST
