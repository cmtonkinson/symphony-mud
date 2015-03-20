
#include "area.h"
#include "avatar.h"
#include "color-string.h"
#include "commands-tedit.h"
#include "commandTable-default.h"
#include "commandTable.h"
#include "display.h"
#include "io-handler.h"
#include "room.h"
#include "world.h"

TCmdAppend::TCmdAppend(void) {
  name("append");
  addSyntax( 0, "            // move down to the next line");
  addSyntax(-1, "<string>    // append the given string");
  brief("Appends the string to the last line of text, word-wrapping if needed.");
  return;
}

bool TCmdAppend::execute(Being* being, const std::vector<std::string>& args) {
  std::string src = Regex::trim(args[0]);
  std::string* current_line = NULL;
  std::vector<ColorString>* v = (std::vector<ColorString>*)avatar()->IOhandler()->getState()["vector"];
  size_t space_left = 0;
  size_t last_char = 0;

  if (args[0].empty()) {
    v->push_back(std::string());
    avatar()->send("You've appended a newline to the text.");
    return true;
  }
  // make sure we've got at least one line to start with...
  if (v->empty()) {
    v->push_back(std::string());
  }

  while (!src.empty()) {
    src = Regex::trim(src);
    current_line = &(v->back());
    space_left = 79 - current_line->length();
    // is this line already full?
    if (space_left < 2) {
      v->push_back(std::string());
      continue;
    }
    // can we fit the whole text on the current line?
    if (src.length() < space_left) {
      if (!current_line->empty()) {
        current_line->append(1, ' ');
      }
      current_line->append(src);
      src.clear();
      continue;
    }
    // can any of the new text fit onto this line?
    if ((last_char = src.rfind(' ', space_left)) == std::string::npos) {
      v->push_back(std::string());
      continue;
    }
    // move whatever we can from src into the current line
    if (!current_line->empty()) {
      current_line->append(1, ' ');
    }
    current_line->append(src, 0, last_char);
    src.erase(0, last_char);
  }

  avatar()->send("Text appended successfully.\n");
  return true;
}

TCmdCancel::TCmdCancel(void) {
  name("cancel");
  addSyntax(0, "");
  brief("Discards the current editor session.");
  return;
}

bool TCmdCancel::execute(Being* being, const std::vector<std::string>& args) {
  switch (avatar()->composing().number()) {
    case COMP_NOTE:
      avatar()->replaceIOHandler(new NoteCancelIOHandler(being));
      break;
    default:
      avatar()->send("You're not doing anything that can be 'cancelled.'");
      return false;
      break;
  }
  return true;
}

TCmdDelete::TCmdDelete(void) {
  name("delete");
  shortcut("-");
  addSyntax(1, "<number>");
  brief("Erases a line of text.");
  return;
}

bool TCmdDelete::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<ColorString>* l = (std::vector<ColorString>*)avatar()->IOhandler()->getState()["vector"];
  std::vector<ColorString> copy = *l;
  unsigned int line = estring(args[0]);
  unsigned int i = 0;
  bool deleted = false;

  if (line < 1 || line > 99) {
    avatar()->send("Invalid line number.");
    return false;
  }

  // clear the vector, and copy back into it all but the given line (in order, of course)
  l->clear();
  for (std::vector<ColorString>::iterator it = copy.begin(); it != copy.end(); ++it, ++i) {
    if (i == line-1) {
      deleted = true;
    } else {
      l->push_back(*it);
    }
  }

  if (deleted) {
    avatar()->send("Deleted line %u\n", line);
  } else {
    avatar()->send("Nothing was deleted.\n");
  }
  return true;
}

TCmdClear::TCmdClear(void) {
  name("clear");
  addSyntax(0, "");
  brief("Erases the entire text.");
  return;
}

bool TCmdClear::execute(Being* being, const std::vector<std::string>& args) {
  ((std::vector<ColorString>*)avatar()->IOhandler()->getState()["vector"])->clear();
  avatar()->send("Text cleared.");
  return true;
}

TCmdInsert::TCmdInsert(void) {
  name("insert");
  addSyntax( 1, "<line>                // insert a blank line");
  addSyntax(-2, "<line> <string>       // insert the given text");
  brief("Inserts a blank line or the given text (truncated to 79 characters).");
  addOptions("line", "a line number (between 1 and 99)");
  return;
}

bool TCmdInsert::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<ColorString>* l = (std::vector<ColorString>*)avatar()->IOhandler()->getState()["vector"];
  unsigned int line = 0;
  unsigned int i = 0;
  bool blank = args.size() == 2 ? false : true;

  line = estring(args[0]);

  if (line < 1 || line > 99) {
    avatar()->send("Invalid line number.");
    return false;
  }

  for (std::vector<ColorString>::iterator it = l->begin(); it != l->end(); ++it, ++i) {
    if (i == line-1) {
      l->insert(it, blank ? std::string() : ColorString(args[1]).effectiveSubstr(0, 79));
      avatar()->send("Inserted the %s at line %u\n", blank ? "blank line" : "text", line);
      return true;
    }
  }

  l->push_back(blank ? std::string() : args[1]);
  avatar()->send("Inserted the %s on the last line.\n", blank ? "blank line" : "text");
  return true;
}

TCmdReplace::TCmdReplace(void) {
  name("replace");
  addSyntax( 1, "<number>");
  addSyntax(-2, "<number> <string>");
  brief("Replaces the line of text given by <number> with <string>.");
  return;
}

bool TCmdReplace::execute(Being* being, const std::vector<std::string>& args) {
  TCmdDelete del;
  std::vector<std::string> del_args;
  TCmdInsert ins;
  std::vector<std::string> ins_args;

  del_args.push_back(args[0]);
  del.avatar(avatar());
  del.execute(being, del_args);
  ins_args.push_back(args[0]);
  if (args.size() == 2) {
    ins_args.push_back(args[1]);
  } else {
    ins_args.push_back(std::string());
  }
  ins.avatar(avatar());
  ins.execute(being, ins_args);
  return true;
}

TCmdSend::TCmdSend(void) {
  name("send");
  addSyntax(0, "");
  brief("Finalizes a note and posts it.");
  return;
}

bool TCmdSend::execute(Being* being, const std::vector<std::string>& args) {
  switch (avatar()->composing().number()) {
    case COMP_NOTE:
      avatar()->replaceIOHandler(new NoteSendIOHandler(being));
      break;
    default:
      avatar()->send("You're not doing anything that can be 'sent.'");
      return false;
      break;
  }
  return true;
}

TCmdShow::TCmdShow(void) {
  name("show");
  addSyntax(0, "");
  brief("Displays the entire text at once.");
  return;
}

bool TCmdShow::execute(Being* being, const std::vector<std::string>& args) {
  std::vector<ColorString>* l = (std::vector<ColorString>*)avatar()->IOhandler()->getState()["vector"];
  char buffer[4096];
  std::string output;
  unsigned int u = 0;
  unsigned int len = 0;

  if (l->empty()) {
    avatar()->send("There is no text in the buffer at the moment.");
    return true;
  }

  output.append("+--+").append(81, '-').append("+\n");
  for (std::vector<ColorString>::iterator it = l->begin(); it != l->end(); ++it) {
    try {
      len = it->effectiveLength();
      sprintf(buffer, "|{y%02u{x| ", ++u);
      output.append(buffer).append((*it).c_str()).append(79-len, ' ').append(" {x|\n");
    } catch (std::exception e) {
      fprintf(stderr, "TCmdShow::execute(): \"%s\" buffer was \"%s\" len was %u", e.what(), buffer, len);
    }
  }
  output.append("+--+").append(81, '-').append("+\n");

  avatar()->send(output);
  return true;
}
