/*
 * $Id: commandTable-oedit.cpp 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#include "commands-oedit.h"
#include "commandTable.h"
#include "commandTable-default.h"

OeditCommands::OeditCommands( void ) {
  name( "Oedit" );
  add( new CmdExit(), true );
  add( new OCmdAttribute(), true );
  add( new OCmdComposition(), true );
  add( new OCmdDelete(), true );
  add( new OCmdDescription(), true );
  add( new OCmdFlag(), true );
  add( new OCmdFurnitureCapacity(), true );
  add( new OCmdFurnitureLayOn(), true );
  add( new OCmdFurnitureSitAt(), true );
  add( new OCmdFurnitureSitOn(), true );
  add( new OCmdFurnitureStandOn(), true );
  add( new OCmdInformation(), true );
  add( new OCmdKeywords(), true );
  add( new OCmdLevel(), true );
  add( new OCmdLongname(), true );
  add( new OCmdShortname(), true );
  add( new OCmdType(), true );
  add( new OCmdValue(), true );
  add( new OCmdWeaponDamage(), true );
  add( new OCmdWeaponType(), true );
  add( new OCmdWeaponVerb(), true );
  add( new OCmdWearable(), true );
  return;
}
