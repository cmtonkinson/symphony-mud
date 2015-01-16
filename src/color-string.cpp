/*
 * $Id: color-string.cpp 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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

#include "color-string.h"

ColorString::ColorString(void) {
  return;
}

ColorString::ColorString(const std::string& ref):
    std::string(ref) {
  return;
}

ColorString::ColorString(const ColorString& ref):
    std::string(ref) {
  return;
}

ColorString::~ColorString(void) {
  return;
}

std::string& ColorString::operator = (const std::string& ref) {
  this->std::string::operator = (ref);
  return *this;
}

size_t ColorString::effectiveLength(void) const {
  size_t total = 0;

  for (std::string::const_iterator it = begin(); it != end(); ++it) {
    if (*it == '{') {
      ++it;
      if (*it == '{') {
        ++total;
      }
      continue;
    }
    ++total;
  }

  return total;
}

std::string ColorString::effectiveSubstr(size_t pos, size_t n) const {
  std::string dest;
  size_t start = pos;
  size_t current_pos = start;
  size_t current_len = 0;

  while (current_len < n && current_pos < size()) {
    if (at(current_pos) == '{') {
      ++current_pos;
      dest.append(1, '{');
      if (at(current_pos) == '{') {
        ++current_len;
      }
      dest.append(1, at(current_pos));
    } else {
      dest.append(1, at(current_pos));
      ++current_len;
    }
    ++current_pos;
  }

  return dest;
}

std::string ColorString::stripColor(void) const {
  std::string dest;

  for (std::string::const_iterator it = begin(); it != end(); ++it) {
    if (*it == '{') {
      if (*(it+1) == '{') {
        dest.append(1, '{');
      }
      ++it;
    } else {
      dest.append(1, *it);
    }
  }

  return dest;
}

std::string ColorString::interpretColor(void) const {
  std::string dest;

  // Parse colors...
  for (std::string::const_iterator it = begin(); it != end(); ++it) {
    if (*it == '{') {
      dest.append(getColorCode(*(it+1)));
      ++it;
    } else {
      dest.append(1, *it);
    }
  }
  dest.append(getColorCode('x'));

  return dest;
}

const char* ColorString::getColorCode(const char& c) {
  switch (c) {
    case 'r': return "\033[0;31m";    // dark red
    case 'R': return "\033[0;31;1m";  // bright red
    case 'g': return "\033[0;32m";    // dark green
    case 'G': return "\033[0;32;1m";  // bright green
    case 'y': return "\033[0;33m";    // dark yellow
    case 'Y': return "\033[0;33;1m";  // bright yellow
    case 'b': return "\033[0;34m";    // dark blue
    case 'B': return "\033[0;34;1m";  // bright blue
    case 'm': return "\033[0;35m";    // dark magenta
    case 'M': return "\033[0;35;1m";  // bright magenta
    case 'c': return "\033[0;36m";    // dark cyan
    case 'C': return "\033[0;36;1m";  // bright cyan
    case 'w': return "\033[1;30m";    // dark gray
    case 'W': return "\033[0;37;1m";  // pure white
    case '{': return "{";             // bracket
    case '?':
      switch (rand() % 14) {
        case  0: return getColorCode('r');
        case  1: return getColorCode('R');
        case  2: return getColorCode('g');
        case  3: return getColorCode('G');
        case  4: return getColorCode('y');
        case  5: return getColorCode('Y');
        case  6: return getColorCode('b');
        case  7: return getColorCode('B');
        case  8: return getColorCode('m');
        case  9: return getColorCode('M');
        case 10: return getColorCode('c');
        case 11: return getColorCode('C');
        case 12: return getColorCode('w');
        case 13: return getColorCode('W');
        default: return getColorCode('x');
      }
    default:  return "\033[0;0;0m";   // default
  }
}
