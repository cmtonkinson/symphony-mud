
#ifndef H_SYMPHONY_COMMAND_LIST
#define H_SYMPHONY_COMMAND_LIST

#include "command-table.hpp"
#include "command.hpp"

DEF_COMMANDTABLE(Commands)      // Standard (and admin)0 Commands
DEF_COMMANDTABLE(IeditCommands) // Item edit Commands
DEF_COMMANDTABLE(NeditCommands) // NPC edit Commands
DEF_COMMANDTABLE(PeditCommands) // Player edit Commands
DEF_COMMANDTABLE(ReditCommands) // Room edit Commands
DEF_COMMANDTABLE(SeditCommands) // Social command edit Commands
DEF_COMMANDTABLE(TeditCommands) // Text edit Commands
DEF_COMMANDTABLE(ZeditCommands) // Zone edit Commands

///////////////////////////////////////////////////////////////////////////////
// ADMINISTRATIVE COMMANDS
///////////////////////////////////////////////////////////////////////////////
DEF_COMMAND(CmdAdminNote)
DEF_COMMAND(CmdAt)
DEF_COMMAND(CmdBigBrother)
DEF_COMMAND(CmdClone)
DEF_COMMAND(CmdConnections)
DEF_COMMAND(CmdCrash)
DEF_COMMAND(CmdDashboard)
DEF_COMMAND(CmdDemote)
DEF_COMMAND(CmdDisable)
DEF_COMMAND(CmdDisconnect)
DEF_COMMAND(CmdEat)
DEF_COMMAND(CmdEnable)
DEF_COMMAND(CmdExit)
DEF_COMMAND(CmdForce)
DEF_COMMAND(CmdGecho)
DEF_COMMAND(CmdGoto)
DEF_COMMAND(CmdHeal)
DEF_COMMAND(CmdIdentify)
DEF_COMMAND(CmdIedit)
DEF_COMMAND(CmdIlist)
DEF_COMMAND(CmdIload)
DEF_COMMAND(CmdIncognito)
DEF_COMMAND(CmdJobs)
DEF_COMMAND(CmdNedit)
DEF_COMMAND(CmdNlist)
DEF_COMMAND(CmdNload)
DEF_COMMAND(CmdPeace)
DEF_COMMAND(CmdPedit)
DEF_COMMAND(CmdPlacement)
DEF_COMMAND(CmdPoofin)
DEF_COMMAND(CmdPoofout)
DEF_COMMAND(CmdPromote)
DEF_COMMAND(CmdPurge)
DEF_COMMAND(CmdQuit)
DEF_COMMAND(CmdReboot)
DEF_COMMAND(CmdRedit)
DEF_COMMAND(CmdRestring)
DEF_COMMAND(CmdRlist)
DEF_COMMAND(CmdSedit)
DEF_COMMAND(CmdShutdown)
DEF_COMMAND(CmdSummon)
DEF_COMMAND(CmdTest)
DEF_COMMAND(CmdTimers)
DEF_COMMAND(CmdTutor)
DEF_COMMAND(CmdWorldSave)
DEF_COMMAND(CmdZedit)

///////////////////////////////////////////////////////////////////////////////
// "STANDARD" COMMANDS
///////////////////////////////////////////////////////////////////////////////
DEF_COMMAND(CmdAffinity)
DEF_COMMAND(CmdAfk)
DEF_COMMAND(CmdBoards)
DEF_COMMAND(CmdBored)
DEF_COMMAND(CmdBusy)
DEF_COMMAND(CmdCast)
DEF_COMMAND(CmdChannels)
DEF_COMMAND(CmdClose)
DEF_COMMAND(CmdColors)
DEF_COMMAND(CmdCommands)
DEF_COMMAND(CmdDelete)
DEF_COMMAND(CmdDescription)
DEF_COMMAND(CmdDrop)
DEF_COMMAND(CmdDunce)
DEF_COMMAND(CmdEmote)
DEF_COMMAND(CmdEquipment)
DEF_COMMAND(CmdGet)
DEF_COMMAND(CmdGive)
DEF_COMMAND(CmdGroup)
DEF_COMMAND(CmdHelp)
DEF_COMMAND(CmdInventory)
DEF_COMMAND(CmdKill)
DEF_COMMAND(CmdLay)
DEF_COMMAND(CmdLearn)
DEF_COMMAND(CmdLock)
DEF_COMMAND(CmdLook)
DEF_COMMAND(CmdMap)
DEF_COMMAND(CmdMoveDown)
DEF_COMMAND(CmdMoveEast)
DEF_COMMAND(CmdMoveNorth)
DEF_COMMAND(CmdMoveSouth)
DEF_COMMAND(CmdMoveUp)
DEF_COMMAND(CmdMoveWest)
DEF_COMMAND(CmdNote)
DEF_COMMAND(CmdOoc)
DEF_COMMAND(CmdOpen)
DEF_COMMAND(CmdPassword)
DEF_COMMAND(CmdPut)
DEF_COMMAND(CmdRemove)
DEF_COMMAND(CmdReply)
DEF_COMMAND(CmdSave)
DEF_COMMAND(CmdSay)
DEF_COMMAND(CmdSit)
DEF_COMMAND(CmdSkills)
DEF_COMMAND(CmdSleep)
DEF_COMMAND(CmdSocials)
DEF_COMMAND(CmdSpells)
DEF_COMMAND(CmdStand)
DEF_COMMAND(CmdSummary)
DEF_COMMAND(CmdTell)
DEF_COMMAND(CmdTime)
DEF_COMMAND(CmdTitle)
DEF_COMMAND(CmdTrain)
DEF_COMMAND(CmdUngroup)
DEF_COMMAND(CmdUnlock)
DEF_COMMAND(CmdWear)
DEF_COMMAND(CmdWho)
DEF_COMMAND(CmdWhoAmI)
DEF_COMMAND(CmdZones)

///////////////////////////////////////////////////////////////////////////////
// ITEM EDITOR
///////////////////////////////////////////////////////////////////////////////
DEF_COMMAND(ICmdArmorType)
DEF_COMMAND(ICmdAttribute)
DEF_COMMAND(ICmdComposition)
DEF_COMMAND(ICmdDelete)
DEF_COMMAND(ICmdDescription)
DEF_COMMAND(ICmdFlag)
DEF_COMMAND(ICmdFurnitureCapacity)
DEF_COMMAND(ICmdFurnitureLayOn)
DEF_COMMAND(ICmdFurnitureSitAt)
DEF_COMMAND(ICmdFurnitureSitOn)
DEF_COMMAND(ICmdFurnitureStandOn)
DEF_COMMAND(ICmdInformation)
DEF_COMMAND(ICmdKeywords)
DEF_COMMAND(ICmdLevel)
DEF_COMMAND(ICmdLongname)
DEF_COMMAND(ICmdShortname)
DEF_COMMAND(ICmdType)
DEF_COMMAND(ICmdValue)
DEF_COMMAND(ICmdWeaponDamage)
DEF_COMMAND(ICmdWeaponStat)
DEF_COMMAND(ICmdWeaponType)
DEF_COMMAND(ICmdWeaponVerb)
DEF_COMMAND(ICmdWearable)

///////////////////////////////////////////////////////////////////////////////
// NPC EDITOR
///////////////////////////////////////////////////////////////////////////////
DEF_COMMAND(NCmdAggressiveness)
DEF_COMMAND(NCmdClass)
DEF_COMMAND(NCmdDescription)
DEF_COMMAND(NCmdGender)
DEF_COMMAND(NCmdInformation)
DEF_COMMAND(NCmdKeywords)
DEF_COMMAND(NCmdLevel)
DEF_COMMAND(NCmdLongname)
DEF_COMMAND(NCmdMobility)
DEF_COMMAND(NCmdRace)
DEF_COMMAND(NCmdShortname)

///////////////////////////////////////////////////////////////////////////////
// PLAYER EDITOR
///////////////////////////////////////////////////////////////////////////////
DEF_COMMAND(PCmdColor)
DEF_COMMAND(PCmdHeadBuilder)

///////////////////////////////////////////////////////////////////////////////
// ROOM EDITOR
///////////////////////////////////////////////////////////////////////////////
DEF_COMMAND(RCmdCreate)
DEF_COMMAND(RCmdDelete)
DEF_COMMAND(RCmdDescription)
DEF_COMMAND(RCmdDig)
DEF_COMMAND(RCmdExitflag)
DEF_COMMAND(RCmdFlag)
DEF_COMMAND(RCmdInformation)
DEF_COMMAND(RCmdKey)
DEF_COMMAND(RCmdLink)
DEF_COMMAND(RCmdName)
DEF_COMMAND(RCmdSmell)
DEF_COMMAND(RCmdSound)
DEF_COMMAND(RCmdTerrain)
DEF_COMMAND(RCmdUnlink)

///////////////////////////////////////////////////////////////////////////////
// SOCIAL EDITOR
///////////////////////////////////////////////////////////////////////////////
DEF_COMMAND(SCmdAudible)
DEF_COMMAND(SCmdDelete)
DEF_COMMAND(SCmdInformation)
DEF_COMMAND(SCmdTactile)

DEF_COMMAND(SCmdTargetNone)
DEF_COMMAND(SCmdTargetSelf)
DEF_COMMAND(SCmdTargetVictim)

DEF_COMMAND(SCmdNoneActor)
DEF_COMMAND(SCmdNoneRoom)

DEF_COMMAND(SCmdSelfActor)
DEF_COMMAND(SCmdSelfRoom)

DEF_COMMAND(SCmdVictimActor)
DEF_COMMAND(SCmdVictimVictim)
DEF_COMMAND(SCmdVictimRoom)

///////////////////////////////////////////////////////////////////////////////
// TEXT EDITOR
///////////////////////////////////////////////////////////////////////////////
DEF_COMMAND(TCmdAppend)
DEF_COMMAND(TCmdCancel)
DEF_COMMAND(TCmdClear)
DEF_COMMAND(TCmdDelete)
DEF_COMMAND(TCmdInsert)
DEF_COMMAND(TCmdReplace)
DEF_COMMAND(TCmdSend)
DEF_COMMAND(TCmdShow)

///////////////////////////////////////////////////////////////////////////////
// ZONE EDITOR
///////////////////////////////////////////////////////////////////////////////
DEF_COMMAND(ZCmdDelete)
DEF_COMMAND(ZCmdInformation)
DEF_COMMAND(ZCmdName)
DEF_COMMAND(ZCmdPermission)
DEF_COMMAND(ZCmdTerrain)

#endif // #ifndef H_SYMPHONY_COMMAND_LIST
