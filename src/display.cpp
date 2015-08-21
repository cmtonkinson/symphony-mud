
#include <math.h>
#include "being.hpp"
#include "display.hpp"
#include "exit.hpp"
#include "item.hpp"

Display::Display(void) {
  return;
}

Display::~Display(void) {
  return;
}

std::string Display::formatColumns(const std::vector<std::string>& items) {
  unsigned long cols = 6;
  unsigned long column_width = 13;
  unsigned long rows = items.size()/cols+1;
  unsigned long offset = 0;
  unsigned long size = items.size();
  char* buffer = (char*)calloc(column_width, sizeof(char));
  std::string foo;
  std::string dest;

  // Loop through rows and columns and print each item in the list...
  for (unsigned long row = 0; row < rows; ++row) {
    for (unsigned long column = 0; column < cols; ++column) {
      // Calculate our list offset for the current row/column...
      offset = column*rows+row;
      if (offset < size) {
        // The offset is a valid list element...
        foo = items[offset];
      } else {
        // The offset is out of bounds (only happens in rightmost column)...
        foo.clear();
      }
      // Copy the data...
      sprintf(buffer, "%-13s{x", foo.substr(0, column_width).c_str());
      dest.append(buffer);
    }
    // Newline after every row but the last...
    if (row < rows-1) {
      dest.append(1, '\n');
    }
  }

  free(buffer);
  return dest;
}

std::string Display::formatAction(const char* format, Being* actor, const void* arg1, const void* arg2, Being* withRespectTo) {
  std::string input(format);
  std::string message;

  for (std::string::iterator it = input.begin(); it != input.end(); ++it) {
    if (*it == '$') {
      switch (*(it+1)) {
        // arbitrary strings
        case 's': message.append(arg1 ? (const char*)arg1 : "[s]");                                         break;
        case 'S': message.append(arg2 ? (const char*)arg2 : "[S]");                                         break;
        // the 'actor'
        case 'a': message.append(withRespectTo ? withRespectTo->seeName(actor) : "[a]");                    break;
        // arbitrary actors
        case 'c': message.append(withRespectTo ? withRespectTo->seeName((Being*)arg1) : "[c]");             break;
        case 'C': message.append(withRespectTo ? withRespectTo->seeName((Being*)arg2) : "[C]");             break;
        // reflexive pronouns
        case 'f': message.append(withRespectTo ? withRespectTo->seeReflexivePronoun((Being*)arg1) : "[f]"); break;
        case 'F': message.append(withRespectTo ? withRespectTo->seeReflexivePronoun((Being*)arg2) : "[F]"); break;
        // item pronouns
        case 'b': message.append(withRespectTo ? withRespectTo->seeItemPronoun((Being*)arg1) : "[b]");      break;
        case 'B': message.append(withRespectTo ? withRespectTo->seeItemPronoun((Being*)arg2) : "[B]");      break;
        // possessive pronouns
        case 'r': message.append(withRespectTo ? withRespectTo->seePosessivePronoun((Being*)arg1) : "[r]"); break;
        case 'R': message.append(withRespectTo ? withRespectTo->seePosessivePronoun((Being*)arg2) : "[R]"); break;
        // items
        case 'o': message.append(arg1 ? ((Item*)arg1)->identifiers().shortname() : "[o]");                  break;
        case 'O': message.append(arg2 ? ((Item*)arg2)->identifiers().shortname() : "[O]");                  break;
        // exits
        case 'e': message.append(arg1 ? Exit::name(((Exit*)arg1)->direction().number()) : "[e]");           break;
        case 'E': message.append(arg2 ? Exit::name(((Exit*)arg2)->direction().number()) : "[E]");           break;
        // exit inverses
        case 'n': message.append(arg1 ? Exit::inverseName(((Exit*)arg1)->direction().number()) : "[n]");    break;
        case 'N': message.append(arg2 ? Exit::inverseName(((Exit*)arg2)->direction().number()) : "[N]");    break;
        // huh?
        default:  message.append("[$]"); break;
      }
      message.append("{x");
      ++it;
    } else {
      message.append(1, *it);
    }
  }
  message[0] = toupper(message[0]);       // proper mechanics
  Regex::replace("s's", "s'", message); // fix for posessives ending in 's'
  return message;
}

std::string Display::formatDamage(unsigned int damage) {
  std::string output;
  if (damage > 1000) {
    output = "does {?H{?E{?L{?L{?I{?S{?H{x things to";
  } else if (damage > 250) {
    output = "{WRUINS{x";
  } else if (damage > 200) {
    output = "{w--{W=={r{{{R{{{WANNIHILATES{R}{r}{W=={w--{x";
  } else if (damage > 180) {
    output = "{b=={c**{CDEVASTATES{c**{b=={x";
  } else if (damage > 160) {
    output = "{w<{x<{W<{RDESOLATES{W>{x>{w>{x";
  } else if (damage > 140) {
    output = "{g({G({YSHATTERS{G){g){x";
  } else if (damage > 120) {
    output = "{w[{BBREAKS{w]{x";
  } else if (damage > 100) {
    output = "{YDISFIGURES{x";
  } else if (damage > 80) {
    output = "{BDESTROYS{x";
  } else if (damage > 70) {
    output = "{RRAVAGES{x";
  } else if (damage > 60) {
    output = "{Gdecimates{x";
  } else if (damage > 50) {
    output = "{Ymutilates{x";
  } else if (damage > 40) {
    output = "{Bmangles{x";
  } else if (damage > 35) {
    output = "{Mbatters{x";
  } else if (damage > 30) {
    output = "{Cdamages{x";
  } else if (damage > 25) {
    output = "{winjures{x";
  } else if (damage > 20) {
    output = "{charms{x";
  } else if (damage > 16) {
    output = "{yhurts{x";
  } else if (damage > 12) {
    output = "{gwounds{x";
  } else if (damage > 8) {
    output = "{bbruises{x";
  } else if (damage > 4) {
    output = "{rscratches{x";
  } else if (damage > 0) {
    output = "{mgrazes{x";
  } else {
    output = "does nothing";
  }
  output.append(" [").append(estring(damage)).append("]");
  return output;
}
