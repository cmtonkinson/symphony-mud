/*
 * $Id: commandTable-default.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include "commandTable.h"
#include "commandTable-default.h"

/* This is the default constructor for all CommandTables, therefore
 * any commands added here will be available throughout every game
 * mode (such as Aedit, et al.).  If you want a Command available
 * ONLY via the default handler, add it to Commands (which is at the
 * bottom of this file).
*/
CommandTable::CommandTable( void ) {
  // I put these commands in front so that very common one letter shortucts work...
  add( new CmdLook() );
  add( new CmdMap() );
  add( new CmdMoveNorth() );
  add( new CmdMoveEast() );
  add( new CmdMoveSouth() );
  add( new CmdMoveWest() );
  add( new CmdMoveUp() );
  add( new CmdMoveDown() );
  add( new CmdSummary() );
  // Administrative commands...
  add( new CmdAdminNote() );
  add( new CmdBigBrother() );
  add( new CmdClone() );
  add( new CmdConnections() );
  add( new CmdCrash() );
  add( new CmdDashboard() );
  add( new CmdDemote() );
  add( new CmdDisconnect() );
  add( new CmdDunce() );
  add( new CmdEat() );
  add( new CmdForce() );
  add( new CmdGecho() );
  add( new CmdGoto() );
  add( new CmdIdentify() );
  add( new CmdIncognito() );
  add( new CmdInventory() );
  add( new CmdLoadRule() );
  add( new CmdMlist() );
  add( new CmdMload() );
  add( new CmdOlist() );
  add( new CmdOload() );
  add( new CmdPoofin() );
  add( new CmdPoofout() );
  add( new CmdPromote() );
  add( new CmdPurge() );
  add( new CmdRestring() );
  add( new CmdRlist() );
  add( new CmdSummon() );
  add( new CmdTimers() );
  add( new CmdTest() );
  add( new CmdWorldSave() );
  // Normal commands...
  add( new CmdAfk() );
  add( new CmdAreas() );
  add( new CmdBoards() );
  add( new CmdBored() );
  add( new CmdBusy() );
  add( new CmdChannels() );
  add( new CmdClose() );
  add( new CmdColors() );
  add( new CmdCommands() );
  add( new CmdDrop() );
  add( new CmdEmote() );
  add( new CmdEquipment() );
  add( new CmdGet() );
  add( new CmdGive() );
  add( new CmdGroup() );
  add( new CmdHelp() );
  add( new CmdLay() );
  add( new CmdLock() );
  add( new CmdNote() );
  add( new CmdOoc() );
  add( new CmdOpen() );
  add( new CmdPut() );
  add( new CmdRemove() );
  add( new CmdReply() );
  add( new CmdSave() );
  add( new CmdSay() );
  add( new CmdSit() );
  //add( new CmdSleep() );
  add( new CmdSocials() );
  add( new CmdStand() );
  add( new CmdTell() );
  add( new CmdTime() );
  add( new CmdTitle() );
  add( new CmdUngroup() );
  add( new CmdUnlock() );
  add( new CmdWear() );
  add( new CmdWho() );
  add( new CmdWhoAmI() );
  return;
}

/* This is the constructor for the "normal" CommandTable. Any Commands
 * added here will not be available outside of the default game mode.
 * If you want a Command to be visible throughout all modes, add it to
 * the base CommandTable constructor above.
 */
Commands::Commands( void ) {
  // Administrative commands...
  add( new CmdAt() );
  add( new CmdAedit() );
  add( new CmdDisable() );
  add( new CmdEnable() );
  add( new CmdMedit() );
  add( new CmdOedit() );
  add( new CmdPedit() );
  add( new CmdReboot() );
  add( new CmdRedit() );
  add( new CmdSedit() );
  add( new CmdShutdown() );
  // Normal commands...
  add( new CmdDelete() );
  add( new CmdDescription() );
  add( new CmdPassword() );
  add( new CmdQuit() );
  return;
}
