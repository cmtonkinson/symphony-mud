/*
 * $Id: compiler.h 494 2012-02-21 21:03:17Z cmtonkinson@gmail.com $
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

#ifndef SYMPHONY_COMPILER
#define SYMPHONY_COMPILER

/*
 * I'm not even pretending that this class is OOP-y. This basically
 * just provides a wrapper object around yyparse() so that the other
 * language processing components can continue on with their decoupled
 * lives, believing my well-crafted delusion that the rest of their
 * family adheres to proper paradigm.
 *
 * To that end, it would be unwise to attempt maintaining more than
 * one of these at a time; I haven't found reason to worry about
 * trying to coerce flex & bison into producing re-entrant code.
 */
class Compiler {
  public:
    Compiler(void);
    ~Compiler(void);
};

#endif // #ifndef SYMPHONY_COMPILER
