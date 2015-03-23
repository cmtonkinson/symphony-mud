
#include "command-def.h"

/* This is the default constructor for all CommandTables, therefore
 * any commands added here will be available throughout every game
 * mode (such as Zedit, et al.).  If you want a Command available
 * ONLY via the default handler, add it to Commands (which is at the
 * bottom of this file).
*/
CommandTable::CommandTable(void) {
  // I put these commands in front so that very common one letter shortucts work...
  add(new CmdKill());
  add(new CmdLook());
  add(new CmdMap());
  add(new CmdMoveNorth());
  add(new CmdMoveEast());
  add(new CmdMoveSouth());
  add(new CmdMoveWest());
  add(new CmdMoveUp());
  add(new CmdMoveDown());
  add(new CmdSummary());
  // Administrative commands...
  add(new CmdAdminNote());
  add(new CmdBigBrother());
  add(new CmdClone());
  add(new CmdConnections());
  add(new CmdCrash());
  add(new CmdDashboard());
  add(new CmdDemote());
  add(new CmdDisconnect());
  add(new CmdDunce());
  add(new CmdEat());
  add(new CmdForce());
  add(new CmdGecho());
  add(new CmdGoto());
  add(new CmdHeal());
  add(new CmdIdentify());
  add(new CmdIlist());
  add(new CmdIload());
  add(new CmdIncognito());
  add(new CmdInventory());
  add(new CmdPlacement());
  add(new CmdNlist());
  add(new CmdNload());
  add(new CmdPeace());
  add(new CmdPoofin());
  add(new CmdPoofout());
  add(new CmdPromote());
  add(new CmdPurge());
  add(new CmdRestring());
  add(new CmdRlist());
  add(new CmdSummon());
  add(new CmdTest());
  add(new CmdTimers());
  add(new CmdTutor());
  add(new CmdWorldSave());
  // Normal commands...
  add(new CmdAfk());
  add(new CmdZones());
  add(new CmdBoards());
  add(new CmdBored());
  add(new CmdBusy());
  add(new CmdCast());
  add(new CmdChannels());
  add(new CmdClose());
  add(new CmdColors());
  add(new CmdCommands());
  add(new CmdDrop());
  add(new CmdEmote());
  add(new CmdEquipment());
  add(new CmdGet());
  add(new CmdGive());
  add(new CmdGroup());
  add(new CmdHelp());
  add(new CmdLay());
  add(new CmdLearn());
  add(new CmdLock());
  add(new CmdNote());
  add(new CmdOoc());
  add(new CmdOpen());
  add(new CmdPut());
  add(new CmdRemove());
  add(new CmdReply());
  add(new CmdSave());
  add(new CmdSay());
  add(new CmdSit());
  //add(new CmdSleep());
  add(new CmdSocials());
  add(new CmdStand());
  add(new CmdSkills());
  add(new CmdSpells());
  add(new CmdTell());
  add(new CmdTime());
  add(new CmdTitle());
  add(new CmdTrain());
  add(new CmdUngroup());
  add(new CmdUnlock());
  add(new CmdWear());
  add(new CmdWho());
  add(new CmdWhoAmI());
  return;
}

/* This is the constructor for the "normal" CommandTable. Any Commands
 * added here will not be available outside of the default game mode.
 * If you want a Command to be visible throughout all modes, add it to
 * the base CommandTable constructor above.
 */
Commands::Commands(void) {
  // Administrative commands...
  add(new CmdAt());
  add(new CmdZedit());
  add(new CmdDisable());
  add(new CmdEnable());
  add(new CmdNedit());
  add(new CmdIedit());
  add(new CmdPedit());
  add(new CmdReboot());
  add(new CmdRedit());
  add(new CmdSedit());
  add(new CmdShutdown());
  // Normal commands...
  add(new CmdDelete());
  add(new CmdDescription());
  add(new CmdPassword());
  add(new CmdQuit());
  return;
}


///////////////////////////////////////////////////////////////////////////////
// ITEM EDITOR
///////////////////////////////////////////////////////////////////////////////
IeditCommands::IeditCommands(void) {
  name("Iedit");
  add(new CmdExit(), true);
  add(new ICmdAttribute(), true);
  add(new ICmdComposition(), true);
  add(new ICmdDelete(), true);
  add(new ICmdDescription(), true);
  add(new ICmdFlag(), true);
  add(new ICmdFurnitureCapacity(), true);
  add(new ICmdFurnitureLayOn(), true);
  add(new ICmdFurnitureSitAt(), true);
  add(new ICmdFurnitureSitOn(), true);
  add(new ICmdFurnitureStandOn(), true);
  add(new ICmdInformation(), true);
  add(new ICmdKeywords(), true);
  add(new ICmdLevel(), true);
  add(new ICmdLongname(), true);
  add(new ICmdShortname(), true);
  add(new ICmdType(), true);
  add(new ICmdValue(), true);
  add(new ICmdWeaponDamage(), true);
  add(new ICmdWeaponType(), true);
  add(new ICmdWeaponVerb(), true);
  add(new ICmdWearable(), true);
  return;
}

///////////////////////////////////////////////////////////////////////////////
// NPC EDITOR
///////////////////////////////////////////////////////////////////////////////
NeditCommands::NeditCommands(void) {
  name("Nedit");
  add(new CmdExit(), true);
  add(new NCmdAggressiveness(), true);
  add(new NCmdClass(), true);
  add(new NCmdDescription(), true);
  add(new NCmdGender(), true);
  add(new NCmdInformation(), true);
  add(new NCmdKeywords(), true);
  add(new NCmdLevel(), true);
  add(new NCmdLongname(), true);
  add(new NCmdMobility(), true);
  add(new NCmdRace(), true);
  add(new NCmdShortname(), true);
  return;
}

///////////////////////////////////////////////////////////////////////////////
// PLAYER EDITOR
///////////////////////////////////////////////////////////////////////////////
PeditCommands::PeditCommands(void) {
  name("Pedit");
  add(new CmdExit(), true);
  add(new PCmdColor(), true);
  add(new PCmdHeadBuilder(), true);
  return;
}

///////////////////////////////////////////////////////////////////////////////
// ROOM EDITOR
///////////////////////////////////////////////////////////////////////////////
ReditCommands::ReditCommands(void) {
  name("Redit");
  add(new CmdExit(), true);
  add(new RCmdCreate(), true);
  add(new RCmdDelete(), true);
  add(new RCmdDescription(), true);
  add(new RCmdDig(), true);
  add(new RCmdExitflag(), true);
  add(new RCmdFlag(), true);
  add(new RCmdInformation(), true);
  add(new RCmdKey(), true);
  add(new RCmdLink(), true);
  add(new RCmdName(), true);
  add(new RCmdSmell(), true);
  add(new RCmdSound(), true);
  add(new RCmdTerrain(), true);
  add(new RCmdUnlink(), true);
  return;
}

///////////////////////////////////////////////////////////////////////////////
// SOCIAL EDITOR
///////////////////////////////////////////////////////////////////////////////
SeditCommands::SeditCommands(void) {
  name("Social Editor");
  add(new CmdExit(), true);
  add(new SCmdAudible(), true);
  add(new SCmdDelete(), true);
  add(new SCmdInformation(), true);
  add(new SCmdTactile(), true);
  add(new SCmdTargetNone(), true);
  add(new SCmdTargetSelf(), true);
  add(new SCmdTargetVictim(), true);
  add(new SCmdNoneActor(), true);
  add(new SCmdNoneRoom(), true);
  add(new SCmdSelfActor(), true);
  add(new SCmdSelfRoom(), true);
  add(new SCmdVictimActor(), true);
  add(new SCmdVictimVictim(), true);
  add(new SCmdVictimRoom(), true);
  return;
}

///////////////////////////////////////////////////////////////////////////////
// TEXT EDITOR
///////////////////////////////////////////////////////////////////////////////
TeditCommands::TeditCommands(void) {
  name("Text Editor");
  add(new CmdExit(), true);
  add(new TCmdAppend(), true);
  add(new TCmdCancel(), true);
  add(new TCmdClear(), true);
  add(new TCmdDelete(), true);
  add(new TCmdInsert(), true);
  add(new TCmdReplace(), true);
  add(new TCmdSend(), true);
  add(new TCmdShow(), true);
  return;
}

///////////////////////////////////////////////////////////////////////////////
// ZONE EDITOR
///////////////////////////////////////////////////////////////////////////////
ZeditCommands::ZeditCommands(void) {
  name("Zedit");
  add(new CmdExit(), true);
  add(new ZCmdDelete(), true);
  add(new ZCmdInformation(), true);
  add(new ZCmdName(), true);
  add(new ZCmdPermission(), true);
  add(new ZCmdTerrain(), true);
  return;
}
