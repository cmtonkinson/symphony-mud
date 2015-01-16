/*
 * $Id: commands-sedit.cpp 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

#include "commands-sedit.h"
#include "commandTable.h"
#include "commandTable-default.h"
#include "world.h"

/*
SCmd::SCmd(void) {
  name("");
  level(GOD);
  addSyntax(0, "");
  brief("");
  return;
}

bool SCmd::execute(Creature* creature, const std::vector<std::string>& args) {

  return true;
}
*/

SCmdAudible::SCmdAudible(void) {
  name("audible");
  level(GOD);
  addSyntax(0, "");
  brief("Toggles whether or not the social makes a sound when used.");
  return;
}

bool SCmdAudible::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->flags().toggle(COM_AUDIBLE);
  avatar()->send("Audible set to {y%s{x.", avatar()->sedit()->isAudible() ? "yes" : "no");
  return true;
}

SCmdDelete::SCmdDelete(void) {
  name("delete");
  level(GOD);
  addSyntax(1, "delete");
  brief("Erases the Social Command.");
  return;
}

bool SCmdDelete::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string name = avatar()->sedit()->name();
  CmdExit exit;
  std::vector<std::string> exit_args(1);

  if (args[0] != "delete") {
    avatar()->send(printSyntax());
    return false;
  }

  avatar()->send("Social command \"%s\" has been deleted.", name.c_str());
  for (std::vector<Command*>::iterator it = Commands::Instance().commands().begin(); it != Commands::Instance().commands().end(); ++it) {
    if ((*it)->name() == avatar()->sedit()->name()) {
      Commands::Instance().commands().erase(it);
      break;
    }
  }

  avatar()->sedit()->destroy();
  avatar()->sedit(NULL);
  avatar()->send("Social command \"%s\" deleted.\n", name.c_str());
  exit.avatar(avatar());
  exit.execute(creature, exit_args);
  return true;
}

SCmdInformation::SCmdInformation(void) {
  name("information");
  level(GOD);
  addSyntax(0, "");
  brief("Displays the current status of the Social Command.");
  return;
}

bool SCmdInformation::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->send("Command name: {Y%s{x\nCreated by: {y%s{x\nAudible: {y%s{x\nTactile: {y%s{x\n",
                  avatar()->sedit()->name().c_str(),
                  World::Instance().getAvatarNameByID(avatar()->sedit()->creator()).c_str(),
                  avatar()->sedit()->isAudible() ? "yes" : "no",
                  avatar()->sedit()->isTactile() ? "yes" : "no"
               );

  avatar()->send("\nCan the command be used without any arguments (\"{mtargetNone{x\")? {y%s{x\n", avatar()->sedit()->targetNone() ? "yes" : "no");
  if (avatar()->sedit()->targetNone()) {
    avatar()->send("  The player sees (\"{mnoneActor{x\"): {y%s{x\n", avatar()->sedit()->noneActor().c_str());
    avatar()->send("  Rest of the room sees (\"{mnoneRoom{x\"): {y%s{x\n", avatar()->sedit()->noneRoom().c_str());
  }
  avatar()->send("\nCan the command be used on oneself (\"{mtargetSelf{x\")? {y%s{x\n", avatar()->sedit()->targetSelf() ? "yes" : "no");
  if (avatar()->sedit()->targetSelf()) {
    avatar()->send("  The player sees (\"{mselfActor{x\"): {y%s{x\n", avatar()->sedit()->selfActor().c_str());
    avatar()->send("  Rest of the room sees (\"{mselfRoom{x\"): {y%s{x\n", avatar()->sedit()->selfRoom().c_str());
  }
  avatar()->send("\nCan the command be used on someone else (\"{mtargetVictim{x\")? {y%s{x\n", avatar()->sedit()->targetVictim() ? "yes" : "no");
  if (avatar()->sedit()->targetVictim()) {
    avatar()->send("  The player sees (\"{mvictimActor{x\"): {y%s{x\n", avatar()->sedit()->victimActor().c_str());
    avatar()->send("  The victim sees (\"{mvictimVictim{x\"): {y%s{x\n", avatar()->sedit()->victimVictim().c_str());
    avatar()->send("  Rest of the room sees (\"{mvictimRoom{x\"): {y%s{x\n", avatar()->sedit()->victimRoom().c_str());
  }
  avatar()->send("\nThe display engine will make the following replacements (where applicable):\n");
  avatar()->send("    {C$c{x will become the name of the actor\n");
  avatar()->send("    {C$C{x will become the name of the victim\n");
  avatar()->send("    {C$f{x will become himself/herself/itself with respect to the actor\n");
  avatar()->send("    {C$F{x will become himself/herself/itself with respect to the victim\n");
  avatar()->send("    {C$b{x will become him/her/it with respect to the actor\n");
  avatar()->send("    {C$B{x will become him/her/it with respect to the victim\n");
  avatar()->send("    {C$r{x will become his/her/its with respect to the actor\n");
  avatar()->send("    {C$R{x will become his/her/its with respect to the victim\n");
  return true;
}

SCmdTactile::SCmdTactile(void) {
  name("tactile");
  level(GOD);
  addSyntax(0, "");
  brief("Toggles whether or not the social makes contact with the target when used.");
  return;
}

bool SCmdTactile::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->flags().toggle(COM_TACTILE);
  avatar()->send("Tactile set to {y%s{x.", avatar()->sedit()->isTactile() ? "yes" : "no");
  return true;
}

SCmdTargetNone::SCmdTargetNone(void) {
  name("targetnone");
  level(GOD);
  addSyntax(0, "");
  brief("Toggles whether the Command can be used without a target.");
  return;
}

bool SCmdTargetNone::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->targetNone(!avatar()->sedit()->targetNone());
  if (avatar()->sedit()->targetNone()) {
    avatar()->send("{Y%s{x can now be used without any arguments.", avatar()->sedit()->name().c_str());
    avatar()->sedit()->arguments().insert(0);
  } else {
    avatar()->send("{Y%s{x can no longer be used without a valid target.", avatar()->sedit()->name().c_str());
    avatar()->sedit()->arguments().erase(0);
  }
  return true;
}

SCmdTargetSelf::SCmdTargetSelf(void) {
  name("targetself");
  level(GOD);
  addSyntax(0, "");
  brief("Toggles whether the Command can be used on oneself.");
  return;
}

bool SCmdTargetSelf::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->targetSelf(!avatar()->sedit()->targetSelf());
  if (avatar()->sedit()->targetSelf()) {
    avatar()->send("{Y%s{x can now be used on oneself.", avatar()->sedit()->name().c_str());
    avatar()->sedit()->arguments().insert(1);
  } else {
    avatar()->send("{Y%s{x can no longer be used on oneself.", avatar()->sedit()->name().c_str());
    avatar()->sedit()->arguments().erase(1);
  }
  return true;
}

SCmdTargetVictim::SCmdTargetVictim(void) {
  name("targetvictim");
  level(GOD);
  addSyntax(0, "");
  brief("Toggles whether the Command can be used on someone else.");
  return;
}

bool SCmdTargetVictim::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->targetVictim(!avatar()->sedit()->targetVictim());
  if (avatar()->sedit()->targetVictim()) {
    avatar()->send("{Y%s{x can now be used on someone else.", avatar()->sedit()->name().c_str());
    avatar()->sedit()->arguments().insert(1);
  } else {
    avatar()->send("{Y%s{x can no longer be used on someone else.", avatar()->sedit()->name().c_str());
    avatar()->sedit()->arguments().erase(1);
  }
  return true;
}

SCmdNoneActor::SCmdNoneActor(void) {
  name("noneactor");
  level(GOD);
  addSyntax(-1, "<string>");
  brief("Changes what the actor sees when using the Command without a target.");
  return;
}

bool SCmdNoneActor::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->noneActor(args[0]);
  avatar()->send("noneActor has been set to: {y%s{x", avatar()->sedit()->noneActor().c_str());
  return true;
}

SCmdNoneRoom::SCmdNoneRoom(void) {
  name("noneroom");
  level(GOD);
  addSyntax(-1, "<string>");
  brief("Changes what others see when the actor uses Command without a target.");
  return;
}

bool SCmdNoneRoom::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->noneRoom(args[0]);
  avatar()->send("noneRoom has been set to: {y%s{x", avatar()->sedit()->noneRoom().c_str());
  return true;
}

SCmdSelfActor::SCmdSelfActor(void) {
  name("selfactor");
  level(GOD);
  addSyntax(-1, "<string>");
  brief("Changes what the actor sees when using the Social Command on itself.");
  return;
}

bool SCmdSelfActor::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->selfActor(args[0]);
  avatar()->send("selfActor has been set to: {y%s{x", avatar()->sedit()->selfActor().c_str());
  return true;
}

SCmdSelfRoom::SCmdSelfRoom(void) {
  name("selfroom");
  level(GOD);
  addSyntax(-1, "<string>");
  brief("Changes what others see when the actor uses the Command on itself.");
  return;
}

bool SCmdSelfRoom::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->selfRoom(args[0]);
  avatar()->send("selfRoom has been set to: {y%s{x", avatar()->sedit()->selfRoom().c_str());
  return true;
}

SCmdVictimActor::SCmdVictimActor(void) {
  name("victimactor");
  level(GOD);
  addSyntax(-1, "<string>");
  brief("Changes what the actor sees when using the Command on another.");
  return;
}

bool SCmdVictimActor::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->victimActor(args[0]);
  avatar()->send("victimActor has been set to: {y%s{x", avatar()->sedit()->victimActor().c_str());
  return true;
}

SCmdVictimVictim::SCmdVictimVictim(void) {
  name("victimvictim");
  level(GOD);
  addSyntax(-1, "<string>");
  brief("Changes what the victim of the Command sees.");
  return;
}

bool SCmdVictimVictim::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->victimVictim(args[0]);
  avatar()->send("victimVictim has been set to: {y%s{x", avatar()->sedit()->victimVictim().c_str());
  return true;
}

SCmdVictimRoom::SCmdVictimRoom(void) {
  name("victimroom");
  level(GOD);
  addSyntax(-1, "<string>");
  brief("Changes what others see when an actor uses the Command on a victim.");
  return;
}

bool SCmdVictimRoom::execute(Creature* creature, const std::vector<std::string>& args) {
  avatar()->sedit()->victimRoom(args[0]);
  avatar()->send("victimRoom has been set to: {y%s{x", avatar()->sedit()->victimRoom().c_str());
  return true;
}
