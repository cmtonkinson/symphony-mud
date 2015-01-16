
#include <vector>
#include "avatar.h"
#include "color-string.h"
#include "commandTable-default.h"
#include "commandTable.h"
#include "display.h"
#include "io-handler.h"
#include "world.h"

/************************************** BASE CLASS HANDLE METHOD **************************************/
IOHandler::IOHandler(Creature* creature) {
  this->creature(creature);
  this->avatar((Avatar*)creature);
  this->commandTable(&(Commands::Instance()));
  return;
}

bool IOHandler::handle(const std::string& text) {
  Command* command = NULL;
  std::string input = text;
  std::vector<std::string> args;
  short max_num_args = 0;
  short negative_args = 0;
  bool status = false;
  unsigned long position = creature()->position().number();

  /* '!' is a shortcut for 'execute the previous command again.' */
  if (input == "!") {
    input = lastInput();
  } else {
    lastInput(input);
  }

  /* Search the CommandTable for a match of the first token in `input`. */
  if ((command = commandTable()->find(input)) == NULL) {
    creature()->send("Huh?");
    return false;
  }

  /* Make sure that the Creature is allowed to execute the
   * specified Command.  Assert:
   *   1) That the Creature has a high enough level
   *   2) That the Command is enabled
   *   3) Kirby Wuz Here
   *   4) That a "player only" command can't be executed by a Mob
   */
  if (creature()->level() < command->level() || !command->enabled()) {
    creature()->send("Huh?");
    return false;
  }
  if (command->playerOnly()) {
    if (creature()->isAvatar()) {
      command->avatar((Avatar*)creature());
    } else {
      creature()->send("Huh?");
      return false;
    }
  }

  /* We need to make sure that the Creature is in one of the
   * allowed positions (sleeping, sitting, standing, etc) to
   * execute the Command.
   */
  if ((position & command->allowedPositions()) != position) {
    creature()->send("You can't do that while %s.", creature()->position().string().c_str());
    return false;
  }

  /* A Command has a std::set<short> called _arguments.  If this set contains positive
   * numbers (this is considered the general case) then we Regex::split() on the largest
   * of those numbers and then perform a check to ensure that the actual number of
   * arguments captured is valid.  There are special cases:
   *
   * 1) When the first argument to split() is empty, then no arguments were provided for
   *    the Command.  In this case, we assert that 0 is a valid number of arguments for the
   *    Command.  And example of this is the `look` Command:
   *        `look`         (zero arguments)
   *        `look chris`   (one argument)
   *    In the first case here, `look` is executed with no arguments, so we make a check
   *    that 0 is in Command::_arguments.
   *
   * 2) When we find a negative number (call it N) in _arguments, then the given Command
   *    expects a certain number of exact arguments followed by an arbitrary string.  In
   *    this case, the number of exact arguments is -(N+1) and the remaining text is
   *    part of the arbitraty string.  Examples are:
   *        `say hello there`                         (-1 arguments)
   *        `tell chris Hello there.`                 (-2 arguments)
   *        `restring sword shortname a neat sword`   (-3 arguments)
   *    So `say` expects an arbitrary string.  `tell` expects one argument (the name of the
   *    target) and THEN an arbitrary string.  And so on.
   *
   * 3) When a Command has multiple use cases, where one takes some positive finite number
   *    of arguments, and another requires and arbitrary string (as in case #2 above), we
   *    have to treat this differently as well.  The nature of accepting arbitrary strings
   *    requires that we check for such strings before analyzing positive and finite use
   *    cases.  Therefore, we check for this only after a failed attemp to identify a case
   *    such as #2.  An example would be the `insert` command from within the Text Editor:
   *        `insert 4`                    (will insert a blank at line 4)
   *        `insert 4 a string of text`   (will insert 'a string of text' at line 4)
   */

  // Get the largest number of accpted arguments and split()
  max_num_args = *command->arguments().rbegin();
  args = Regex::split(input, max_num_args);
  // This is special case #1 from above...
  if (args[0].empty()) {
    if (command->arguments().find(0) == command->arguments().end()) {
      creature()->send(command->printSyntax());
      return false;
    }
  // This is special case #2 from above...
  } else if ((negative_args = *command->arguments().begin()) < 0) {
    args = Regex::split(input, -(negative_args+1));
    if ((signed)args.size() < (signed)-negative_args) {
      // This is for special case #3 above...
      if (command->arguments().find(args.size()) == command->arguments().end()) {
        creature()->send(command->printSyntax());
        return false;
      }
    }
  // The general case...
  } else {
    if (command->arguments().find(args.size()) == command->arguments().end()) {
      creature()->send(command->printSyntax());
      return false;
    }
  }

  // Reset the Avatar pointer after executing the command, just to be safe...
  status = command->execute(creature(), args);
  command->avatar(NULL);
  return status;
}

bool IOHandler::handle(void) {
  return handle(avatar()->getInput());
}

/************************************** LOGIN (BANNER) HANDLER **************************************/
void LoginBannerIOHandler::activate(void) {
  avatar()->send(prompt());
  avatar()->replaceIOHandler(new LoginNameIOHandler(avatar()));
  return;
}

void LoginBannerIOHandler::deactivate(void) {
  return;
}

bool LoginBannerIOHandler::handle(void) {
  return true;
}

std::string LoginBannerIOHandler::prompt(void) {
  return "\n\n\n\n\n{RS{Gy{Ym{Bp{Mh{Co{Rn{Gy{W!!{x\n\n\n\n\n";
}

/************************************** LOGIN (NAME) HANDLER **************************************/
void LoginNameIOHandler::activate(void) {
  return;
}

void LoginNameIOHandler::deactivate(void) {
  return;
}

bool LoginNameIOHandler::handle(void) {
  std::string input = ColorString(avatar()->getInput()).stripColor();
  input = Regex::trim(input.substr(0, input.find_first_of(' ')));
  Avatar* test = NULL;

  // Sanitize...
  Regex::replace("[^a-z]", "", input);
  if (input.size() > 12 || input.size() < 3) {
    avatar()->send("Sorry - names must be between 3 and 12 characters long (%s is %u).\n", input.c_str(), input.size());
    return false;
  }
  input[0] = toupper(input[0]);

  // Check to make sure that name isn't already signed in...
  if ((test = World::Instance().findAvatar(input)) != NULL) {
    if (test->identifiers().shortname() == input) {
      avatar()->send("Sorry - they're already signed in.\n");
      avatar()->disconnected(true);
      return false;
    }
  }

  // Figure out whether or not the given name already exists...
  avatar()->changeName(input);
  if (avatar()->load()) {
    avatar()->replaceIOHandler(new LoginPasswordIOHandler(avatar()));
    return true;
  } else {
    avatar()->send("A newcomer, eh?\n");
    avatar()->status().set(CREATING);
    avatar()->replaceIOHandler(new CreationNameIOHandler(avatar()));
    return true;
  }
}

std::string LoginNameIOHandler::prompt(void) {
  return "Name: ";
}

/************************************** LOGIN (PASSWORD) HANDLER **************************************/
void LoginPasswordIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void LoginPasswordIOHandler::deactivate(void) {
  return;
}

bool LoginPasswordIOHandler::handle(void) {
  std::string input = avatar()->getInput();
  estring fd = avatar()->socket()->getFd();
  input = Regex::trim(input.substr(0, input.find_first_of(' ')));

  if (avatar()->checkPassword(input)) {
    World::Instance().removeAvatar(fd);
    World::Instance().insert(avatar());
    if (avatar()->deleteMe()) {
      avatar()->status().set(DELETING);
      avatar()->replaceIOHandler(new LoginDeleteIOHandler(avatar()));
      return true;
    }
    World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_LOGINS, "%s has logged in from %s.", avatar()->identifiers().shortname().c_str(), avatar()->socket()->getIP().c_str());
    World::Instance().playerLog(World::LOG_LEVEL_NOTICE, World::LOG_TYPE_PLAYER, "%s (%lu) logged in from %s", avatar()->identifiers().shortname().c_str(), avatar()->ID(), avatar()->socket()->getIP().c_str());
    avatar()->status().set(CONNECTED);
    avatar()->replaceIOHandler(new InputIOHandler(avatar()));
    avatar()->send("\nWecome back!\n");
    avatar()->restoreRoom();
    return true;
  } else {
    avatar()->send("That password is invalid.\n");
    avatar()->deleteMe(false);
    avatar()->markForDeletion(0);
    avatar()->disconnected(true);
    return false;
  }
}

std::string LoginPasswordIOHandler::prompt(void) {
  return "Password: ";
}

/************************************** LOGIN (DELETE) HANDLER **************************************/
void LoginDeleteIOHandler::activate(void) {
  avatar()->deleteMe(false);
  avatar()->send("\n");
  return;
}

void LoginDeleteIOHandler::deactivate(void) {
  return;
}

bool LoginDeleteIOHandler::handle(void) {
  if (avatar()->getInput() == "delete") {
    avatar()->send("\n{RCharacter deleted.{x\n\nFarewell, traveler. May you find happiness in another realm.\n");
    avatar()->deleteMe(true);
    avatar()->disconnected(true);
  } else {
    avatar()->markForDeletion(0);
    avatar()->send("\nCharacter deletion {YCANCELLED{x!!  Whew - that was a close one.");
    World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_LOGINS, "%s has cancelled deletion and logged in from %s.", avatar()->identifiers().shortname().c_str(), avatar()->socket()->getIP().c_str());
    World::Instance().playerLog(World::LOG_LEVEL_NOTICE, World::LOG_TYPE_PLAYER, "%s (%lu) has cancelled deletion and logged in from %s", avatar()->identifiers().shortname().c_str(), avatar()->ID(), avatar()->socket()->getIP().c_str());
    avatar()->status().set(CONNECTED);
    avatar()->send("\nWecome back!\n");
    avatar()->restoreRoom();
  }
  avatar()->replaceIOHandler(new InputIOHandler(avatar()));
  return true;
}

std::string LoginDeleteIOHandler::prompt(void) {
  return "{RThis character has been marked for deletion!{x\n\
To delete this character, enter \"delete\" - any other input will cancel\n\
the process and log you into the game.\n\
Note: Character deletion IS permanent. All data will be instantly lost.";
}

/************************************** CREATION (NAME) HANDLER **************************************/
void CreationNameIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void CreationNameIOHandler::deactivate(void) {
  return;
}

bool CreationNameIOHandler::handle(void) {
  std::string input = ColorString(avatar()->getInput()).stripColor();
  input = Regex::trim(input.substr(0, input.find_first_of(' ')));
  if (Regex::strPrefix(input, "yes")) {
    avatar()->send("Sure it is!\n");
    avatar()->replaceIOHandler(new CreationPasswordIOHandler(avatar()));
    return false;
  } else {
    avatar()->send("My mistake!\n");
    avatar()->changeName(estring(avatar()->socket()->getFd()));
    avatar()->replaceIOHandler(new LoginBannerIOHandler(avatar()));
    return false;
  }
}

std::string CreationNameIOHandler::prompt(void) {
  std::string output;
  output.append("Your name is ").append(avatar()->identifiers().shortname()).append(", then? [yes/no] ");
  return output;
}

/************************************** CREATION (PASSWORD) HANDLER **************************************/
void CreationPasswordIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void CreationPasswordIOHandler::deactivate(void) {
  return;
}

bool CreationPasswordIOHandler::handle(void) {
  std::string input = avatar()->getInput();
  input = Regex::trim(input.substr(0, input.find_first_of(' ')));
  if (input.size() < 6) {
    avatar()->send("Sorry - it needs to be at least 6 characters long (no spaces). ");
    return false;
  } else {
    avatar()->password(input);
    avatar()->replaceIOHandler(new CreationPasswordConfirmIOHandler(avatar()));
    return true;
  }
}

std::string CreationPasswordIOHandler::prompt(void) {
  return "You'll need a password (no spaces): ";
}

/************************************** CREATION (PASSWORD CONFIRM) HANDLER **************************************/
void CreationPasswordConfirmIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void CreationPasswordConfirmIOHandler::deactivate(void) {
  return;
}

bool CreationPasswordConfirmIOHandler::handle(void) {
  std::string input = avatar()->getInput();
  input = Regex::trim(input.substr(0, input.find_first_of(' ')));
  if (input == avatar()->password()) {
    avatar()->send("Great - now on to more interesting matters.\n");
    avatar()->replaceIOHandler(new CreationGenderIOHandler(avatar()));
    return true;
  } else {
    avatar()->send("That doesn't match the other password. ");
    avatar()->replaceIOHandler(new CreationPasswordIOHandler(avatar()));
    return false;
  }
}

std::string CreationPasswordConfirmIOHandler::prompt(void) {
  return "One more time to confirm the password: ";
}

/************************************** CREATION (GENDER) HANDLER **************************************/
void CreationGenderIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void CreationGenderIOHandler::deactivate(void) {
  return;
}

bool CreationGenderIOHandler::handle(void) {
  std::string input = ColorString(avatar()->getInput()).stripColor();
  input = Regex::trim(input.substr(0, input.find_first_of(' ')));
  if (Regex::strPrefix(input, "male")) {
    avatar()->gender().set(MALE);
  } else if (Regex::strPrefix(input, "female")) {
    avatar()->gender().set(FEMALE);
  } else {
    avatar()->send("Sorry, I didn't understand that... ");
    return false;
  }
  avatar()->send("A %s indeed.\n", avatar()->gender().string().c_str());
  avatar()->replaceIOHandler(new CreationRaceIOHandler(avatar()));
  return true;
}

std::string CreationGenderIOHandler::prompt(void) {
  return "Are you male, or female? ";
}

/************************************** CREATION (RACE) HANDLER **************************************/
void CreationRaceIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void CreationRaceIOHandler::deactivate(void) {
  return;
}

bool CreationRaceIOHandler::handle(void) {
  std::string input = ColorString(avatar()->getInput()).stripColor();
  input = Regex::trim(input.substr(0, input.find_first_of(' ')));
  if (Regex::strPrefix(input, "human")) {
    avatar()->race().set(HUMAN);
    avatar()->send("A human it is!\n");
  } else if (Regex::strPrefix(input, "elf")) {
    avatar()->race().set(ELF);
    avatar()->send("A child of the forest you shall be!\n");
  } else if (Regex::strPrefix(input, "dwarf")) {
    avatar()->race().set(DWARF);
    avatar()->send("And a fine dwarf you'll make!\n");
  } else {
    avatar()->send("Sorry... I don't understand... ");
    return false;
  }
  avatar()->replaceIOHandler(new CreationClassIOHandler(avatar()));
  return true;
}

std::string CreationRaceIOHandler::prompt(void) {
  std::string output("The races of the land are:\n");
  output.append("\n  Human     The dominant race. Can be: mage, cleric, warrior, rogue");
  output.append("\n  Elf       Forest-dwelling humanoids. Can be: mage, cleric, warrior, rogue");
  output.append("\n  Dwarf     Mountain-dwelling humanoids. Can be: cleric, warrior");
  output.append("\n\nWhich do you choose? ");
  return output;
}

/************************************** CREATION (CLASS) HANDLER **************************************/
void CreationClassIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void CreationClassIOHandler::deactivate(void) {
  return;
}

bool CreationClassIOHandler::handle(void) {
  std::string input = ColorString(avatar()->getInput()).stripColor();
  input = Regex::trim(input.substr(0, input.find_first_of(' ')));
  if (Regex::strPrefix(input, "mage") && avatar()->race().number() != DWARF) {
    avatar()->pClass().set(MAGE);
    avatar()->send("You shall study the ways of arcane magic.\n");
  } else if (Regex::strPrefix(input, "cleric")) {
    avatar()->pClass().set(CLERIC);
    avatar()->send("You've chosen to follow the holy path.\n");
  } else if (Regex::strPrefix(input, "warrior")) {
    avatar()->pClass().set(WARRIOR);
    avatar()->send("Your physical prowess will be unmatched in combat.\n");
  } else if (Regex::strPrefix(input, "rogue") && avatar()->race().number() != DWARF) {
    avatar()->pClass().set(ROGUE);
    avatar()->send("In stealth you will excel.\n");
  } else {
    avatar()->send("Sorry... I don't understand... ");
    return false;
  }
  avatar()->replaceIOHandler(new CreationSummaryIOHandler(avatar()));
  return true;
}

std::string CreationClassIOHandler::prompt(void) {
  std::string output("As a ");
  output.append(avatar()->race().string()).append(" you may choose to be a:");
  output.append("\n  Cleric   Clerics are divine casters able to channel holy energy");
  output.append("\n  Warrior  Warriors are armsmen, great at dealing (and taking) massive damage");
  if (!avatar()->isDwarf()) {
    output.append("\n  Mage     Mages are arcane casters, champions of illusion and enchantment");
  }
  if (!avatar()->isDwarf()) {
    output.append("\n  Rogue    Rogues are the perfect assassins - stealth is their trademark");
  }
  output.append("\n\nWhich class suits you? ");
  return output;
}

/************************************** CREATION (SUMMARY) HANDLER **************************************/
void CreationSummaryIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void CreationSummaryIOHandler::deactivate(void) {
  return;
}

bool CreationSummaryIOHandler::handle(void) {
  std::string input = ColorString(avatar()->getInput()).stripColor();
  input = Regex::trim(input.substr(0, input.find_first_of(' ')));
  bool success = false;
  if (Regex::strPrefix(input, "accept")) {
    avatar()->naturalStatAdjustment();
    avatar()->status().set(CONNECTED);
    avatar()->room(World::Instance().findRoom(0));
    if (avatar()->create() && avatar()->save() && avatar()->load()) {
      avatar()->send("\n\n{WWelcome to the realm, young %s!{x\n", avatar()->identifiers().shortname().c_str());
      avatar()->restoreRoom();
      avatar()->save();
      success = true;
    } else {
      avatar()->send("Terribly sorry - there was a problem finalizing your account.");
      success = false;
    }
  } else {
    avatar()->send("\n{RCharacter cancelled.{x\n\nFarewell, traveler. May you find happiness in another realm.\n");
    success = false;
  }
  avatar()->replaceIOHandler(new InputIOHandler(avatar()));
  avatar()->disconnected(!success);
  return success;
}

std::string CreationSummaryIOHandler::prompt(void) {
  std::string output("A summary of your choices:");
  output.append("\n  Name:   {g").append(avatar()->identifiers().shortname()).append("{x");
  output.append("\n  Gender: {g").append(avatar()->gender().string()).append("{x");
  output.append("\n  Race:   {g").append(avatar()->race().string()).append("{x");
  output.append("\n  Class:  {g").append(avatar()->pClass().string()).append("{x");
  output.append("\n\nTo accept this configuration, type 'accept'.");
  output.append("\nAny other input will cancel the process.");
  return output;
}

/************************************** NOTE (SUBJECT) HANDLER **************************************/
void NoteGetSubjectIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void NoteGetSubjectIOHandler::deactivate(void) {
  return;
}

bool NoteGetSubjectIOHandler::handle(void) {
  std::string input = ColorString(avatar()->getInput()).stripColor();
  avatar()->note()->subject(input);
  avatar()->composing().set(COMP_NOTE);
  IOHandler* h = new TeditIOHandler(avatar());
  h->getState()["vector"] = (void*)(new std::vector<std::string>());
  h->getState()["name"] = (void*)(new std::string("Note Editor"));
  h->getState()["pointer"] = (void*)avatar()->note()->bodyp();
  avatar()->replaceIOHandler(h);
  avatar()->send("Type `cancel` to abandon your note or `send` to finish it.");
  return true;
}

std::string NoteGetSubjectIOHandler::prompt(void) {
  return "Subject: ";
}

/************************************** NOTE (EXIT) HANDLER **************************************/
void NoteExitIOHandler::activate(void) {
  avatar()->send("\n");
  return;
}

void NoteExitIOHandler::deactivate(void) {
  return;
}

bool NoteExitIOHandler::handle(void) {
  std::string input = ColorString(avatar()->getInput()).stripColor();
  if (Regex::strPrefix(input, "yes")) {
    avatar()->replaceIOHandler(new NoteSendIOHandler(avatar()));
  } else if (Regex::strPrefix(input, "no")) {
    avatar()->replaceIOHandler(new NoteCancelIOHandler(avatar()));
  } else {
    avatar()->send("Sorry...\n");
    return false;
  }
  return true;
}

std::string NoteExitIOHandler::prompt(void) {
  return "Would you like to send this note? (yes/no) ";
}

/************************************** NOTE (SEND) HANDLER **************************************/
void NoteSendIOHandler::activate(void) {
  avatar()->popIOHandler();
  return;
}

void NoteSendIOHandler::deactivate(void) {
  avatar()->composing().set(0);
  avatar()->note()->save();
  World::Instance().boards()[avatar()->board()]->notes().insert(std::make_pair(avatar()->note()->ID(), avatar()->note()));
  avatar()->note(NULL);
  avatar()->send("Your note has been sent.");
  return;
}

bool NoteSendIOHandler::handle(void) {
  return true;
}

std::string NoteSendIOHandler::prompt(void) {
  return "";
}

/************************************** NOTE (CANCEL) HANDLER **************************************/
void NoteCancelIOHandler::activate(void) {
  avatar()->popIOHandler();
  return;
}

void NoteCancelIOHandler::deactivate(void) {
  avatar()->composing().set(0);
  avatar()->note()->destroy();
  avatar()->note(NULL);
  avatar()->send("Your note has been trashed.");
  return;
}

bool NoteCancelIOHandler::handle(void) {
  return true;
}

std::string NoteCancelIOHandler::prompt(void) {
  return "";
}

/************************************** MAIN HANDLER **************************************/
void InputIOHandler::activate(void) {
  commandTable(&(Commands::Instance()));
  return;
}

void InputIOHandler::deactivate(void) {
  return;
}

std::string InputIOHandler::prompt(void) {
  char buffer[256];
  if (avatar()->whoFlags().test(WHO_AFK)) {
    return "\n\n{x[{YAFK{x] ";
  } else {
    sprintf(buffer,
      "\n\n{W[{G%d{x/{g%d{xhp {C%d{x/{c%d{xmana {M%d{xstamina {y%d{xtnl{W]{x ",
      avatar()->health(),
      avatar()->maxHealth(),
      avatar()->mana(),
      avatar()->maxMana(),
      avatar()->stamina(),
      avatar()->tnl());
    return buffer;
  }
}

/************************************** AEDIT HANDLER **************************************/
void AeditIOHandler::activate(void) {
  commandTable(&(AeditCommands::Instance()));
  avatar()->whoFlags().set(WHO_BUSY);
  avatar()->mode().set(MODE_AEDIT);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has entered area editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

void AeditIOHandler::deactivate(void) {
  avatar()->send("Goodbye!\n");
  avatar()->whoFlags().clear(WHO_BUSY);
  avatar()->mode().set(MODE_NONE);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has left area editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

std::string AeditIOHandler::prompt(void) {
  char buffer[Socket::MAX_BUFFER];
  sprintf(buffer, "\n\n{W[{cAedit {x- {G%s{W] ", avatar()->aedit()->name().c_str());
  return buffer;
}

/************************************** MEDIT HANDLER **************************************/
void MeditIOHandler::activate(void) {
  commandTable(&(MeditCommands::Instance()));
  avatar()->whoFlags().set(WHO_BUSY);
  avatar()->mode().set(MODE_MEDIT);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has entered mob editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

void MeditIOHandler::deactivate(void) {
  avatar()->send("Goodbye!\n");
  avatar()->whoFlags().clear(WHO_BUSY);
  avatar()->mode().set(MODE_NONE);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has left mob editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

std::string MeditIOHandler::prompt(void) {
  char buffer[Socket::MAX_BUFFER];
  sprintf(buffer, "\n\n{W[{cMedit {x- {G%lu{W] ", avatar()->medit()->vnum());
  return buffer;
}

/************************************** OEDIT HANDLER **************************************/
void OeditIOHandler::activate(void) {
  commandTable(&(OeditCommands::Instance()));
  avatar()->whoFlags().set(WHO_BUSY);
  avatar()->mode().set(MODE_OEDIT);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has entered object editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

void OeditIOHandler::deactivate(void) {
  avatar()->send("Goodbye!\n");
  avatar()->whoFlags().clear(WHO_BUSY);
  avatar()->mode().set(MODE_NONE);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has left object editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

std::string OeditIOHandler::prompt(void) {
  char buffer[Socket::MAX_BUFFER];
  sprintf(buffer, "\n\n{W[{cOedit {x- {G%lu{W] ", avatar()->oedit()->vnum());
  return buffer;
}

/************************************** PEDIT HANDLER **************************************/
void PeditIOHandler::activate(void) {
  commandTable(&(PeditCommands::Instance()));
  avatar()->whoFlags().set(WHO_BUSY);
  avatar()->mode().set(MODE_PEDIT);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has entered player editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

void PeditIOHandler::deactivate(void) {
  avatar()->send("Goodbye!\n");
  avatar()->whoFlags().clear(WHO_BUSY);
  avatar()->mode().set(MODE_NONE);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has left player editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

std::string PeditIOHandler::prompt(void) {
  char buffer[Socket::MAX_BUFFER];
  sprintf(buffer, "\n\n{W[{cPedit {x- {G%s{W] ", avatar()->pedit()->identifiers().shortname().c_str());
  return buffer;
}

/************************************** REDIT HANDLER **************************************/
void ReditIOHandler::activate(void) {
  commandTable(&(ReditCommands::Instance()));
  avatar()->whoFlags().set(WHO_BUSY);
  avatar()->mode().set(MODE_REDIT);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has entered room editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

void ReditIOHandler::deactivate(void) {
  avatar()->send("Goodbye!\n");
  avatar()->whoFlags().clear(WHO_BUSY);
  avatar()->mode().set(MODE_NONE);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has left room editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

std::string ReditIOHandler::prompt(void) {
  char buffer[Socket::MAX_BUFFER];
  sprintf(buffer, "\n\n{W[{cRedit {x- {G%lu{W] ", avatar()->room()->vnum());
  return buffer;
}

/************************************** SEDIT HANDLER **************************************/
void SeditIOHandler::activate(void) {
  commandTable(&(SeditCommands::Instance()));
  avatar()->whoFlags().set(WHO_BUSY);
  avatar()->mode().set(MODE_SEDIT);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has entered social editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

void SeditIOHandler::deactivate(void) {
  avatar()->send("Goodbye!\n");
  avatar()->whoFlags().clear(WHO_BUSY);
  avatar()->mode().set(MODE_NONE);
  World::Instance().bigBrother(avatar(), ADMIN_BIGBRO_MODES, "%s has left social editing mode.", avatar()->identifiers().shortname().c_str());
  return;
}

std::string SeditIOHandler::prompt(void) {
  char buffer[Socket::MAX_BUFFER];
  sprintf(buffer, "\n\n{W[{cSedit {x- {G%s{W] ", avatar()->sedit()->name().c_str());
  return buffer;
}

/************************************** TEDIT HANDLER **************************************/
void TeditIOHandler::activate(void) {
  commandTable(&(TeditCommands::Instance()));
  avatar()->send("Welcome to the Symphony Text Editor.\n");
  return;
}

void TeditIOHandler::deactivate(void) {
  std::vector<std::string>* v = (std::vector<std::string>*)getState()["vector"];
  std::string* s = (std::string*)getState()["name"];
  ((std::string*)getState()["pointer"])->assign(Regex::implode("\n", *v));
  delete v;
  delete s;
  getState().clear();
  avatar()->send("You've left the Symphony Text Editor.\n");
  return;
}

std::string TeditIOHandler::prompt(void) {
  char buffer[Socket::MAX_BUFFER];
  sprintf(buffer, "\n\n{W[{cCurrently Editing: {C%s{W] ", ((std::string*)getState()["name"])->c_str());
  return buffer;
}
