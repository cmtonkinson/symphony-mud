/*
 * $Id: typeinfo.h 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#ifndef H_TYPEINFO
#define H_TYPEINFO

#include <typeinfo>

using std::type_info;

class Typeinfo {
  public:
    explicit          Typeinfo( const type_info& ref ): _info(ref)  { return; }
    bool              operator < ( const Typeinfo& ref ) const      { return _info.before( ref.info() ); }
    const type_info&  info( void ) const                            { return _info; }
  private:
    const type_info& _info;
};

#endif // #ifndef H_EVENT
