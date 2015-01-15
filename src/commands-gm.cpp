/*
 * $Id: commands-gm.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include <deque>
#include "area.h"
#include "avatar.h"
#include "commandTable.h"
#include "commandTable-default.h"
#include "display.h"
#include "flagTable.h"
#include "group.h"
#include "io-handler.h"
#include "loadRule.h"
#include "loadRuleMob.h"
#include "loadRuleObject.h"
#include "mob.h"
#include "object-container.h"
#include "room.h"
#include "world.h"

/*
Cmd::Cmd(void) {
  name("");
  level();
  notAllowedWhile(Creature::Position_);
  addSyntax(0, "");
  brief("");
  return;
}

bool Cmd::execute( Creature* creature, const std::vector<std::string>& args ) {
  return true;
}
*/

CmdGecho::CmdGecho( void ) {
  name( "gecho" );
  level( GOD );
  addSyntax( -1, "<message>" );
  brief( "Broadcasts a message to all connected players in." );
  return;
}

bool CmdGecho::execute( Creature* creature, const std::vector<std::string>& args ) {
  for ( std::map<std::string,Avatar*>::iterator it = World::Instance().getAvatars().begin(); it != World::Instance().getAvatars().end(); ++it ) {
    if ( it->second->isConnected() ) {
      it->second->send( "{%c%s{x", avatar()->gechoColor(), args[0].c_str() );
    }
  }
  return true;
}

CmdGet::CmdGet( void ) {
  name( "get" );
  addSyntax( 1, "<object>" );
  addSyntax( 2, "<object> <container>" );
  brief( "Picks up an object." );
  return;
}

bool CmdGet::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::list<Object*> objects;
  Object* container = NULL;

  if ( args.size() == 1 ) {
    /* look for items lying in the room */
    objects = creature->room()->inventory().searchObjects( args[0] );
    if ( objects.empty() ) {
      creature->send( "There aren't any of those here." );
      return false;
    }
    for ( std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it ) {
      if ( (*it)->flags().test( OBJECT_NOGET ) ) {
        creature->send( "You can't get %s{x.\n", (*it)->identifiers().shortname().c_str() );
      } else {
        creature->send( "You get %s.\n", (*it)->identifiers().shortname().c_str() );
        creature->room()->send_cond( "$p gets $o.\n", creature, *it );
        creature->room()->inventory().remove( *it );
        creature->inventory().add( *it );
      }
    }
  } else if ( args.size() == 2 ) {
    /* look for items in a container */
    if ( ( container = creature->findObject( args[1] ) ) == NULL ) {
      creature->send( "You can't find that." );
      return false;
    }
    if ( !container->isContainer() ) {
      creature->send( "That's not a container." );
      return false;
    }
    objects = container->container()->inventory().searchObjects( args[0] );
    if ( objects.empty() ) {
      creature->send( "There aren't any of those in %s{x.", container->identifiers().shortname().c_str() );
      return false;
    }
    for ( std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it ) {
      // transfer each object
      container->container()->inventory().remove( *it );
      creature->inventory().add( *it );
      creature->send( "You get %s{x from %s{x.\n", (*it)->identifiers().shortname().c_str(), container->identifiers().shortname().c_str() );
      creature->room()->send_cond( "$p gets $o{x from $O{x.\n", creature, *it, container );
    }
  }

  return true;
}

CmdGive::CmdGive( void ) {
  name( "give" );
  addSyntax( 2, "<object> <player>" );
  addSyntax( 2, "<object> <mob>" );
  brief( "Gives an object to the target." );
  return;
}

bool CmdGive::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::list<Object*> objects;
  Creature* target = NULL;
  Object* object = NULL;

  if ( ( target = creature->findCreature( args[1] ) ) == NULL ) {
    creature->send( "They're not around at the moment." );
    return false;
  }
  if ( ( objects = creature->inventory().searchObjects( args[0] ) ).empty() ) {
    creature->send( "You don't have that." );
    return false;
  }
  if ( objects.empty() ) {
    creature->send( "You don't have that." );
    return false;
  }
  for ( std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it ) {
    object = *it;
    if ( object->flags().test( OBJECT_NODROP ) ) {
      creature->send( "You can't let go of %s.\n", object->identifiers().shortname().c_str() );
    } else {
      creature->inventory().remove( object );
      target->inventory().add( object );
      creature->send( "You give %s to %s.\n", object->identifiers().shortname().c_str(), target->identifiers().shortname().c_str() );
      target->send( "%s gives you %s.\n", target->seeName( creature, true ).c_str(), object->identifiers().shortname().c_str() );
      creature->room()->send_cond( "$p gives $c $O.\n", creature, target, object, TO_NOTVICT );
    }
  }

  return true;
}

CmdGoto::CmdGoto( void ) {
  name( "goto" );
  level( DEMIGOD );
  addSyntax( 1, "<vnum>        (target room)" );
  addSyntax( 1, "<player>      (target player)" );
  addSyntax( 1, "<mob>         (target mob)" );
  brief( "Transports you to the target." );
  return;
}

bool CmdGoto::execute( Creature* creature, const std::vector<std::string>& args ) {
  Creature* target = NULL;
  Room* room = NULL;
  CmdLook look;
  std::vector<std::string> look_args( 1 );

  if ( Regex::match( "^[0-9]+$", args[0] ) ) {
    if ( ( room = World::Instance().findRoom( estring(args[0]) ) ) == NULL ) {
      avatar()->send( "That room doesn't exist." );
      return false;
    }
  } else {
    target = World::Instance().findCreature( args[0] );
    if ( !target || avatar()->canSee( target ) < SEE_NAME ) {
      avatar()->send( "They aren't around right now." );
      return false;
    }
    room = target->room();
  }

  if ( room == avatar()->room() ) {
    creature->send( "You're already there." );
    return false;
  }

  avatar()->room()->remove( avatar() );
  avatar()->room()->send_cond( avatar()->poofout(), avatar() );
  room->add( avatar() );
  room->send_cond( avatar()->poofin(), avatar() );
  avatar()->room( room );
  look.avatar( avatar() );
  look.execute( avatar(), look_args );
  return true;
}

CmdGroup::CmdGroup(void) {
  name("group");
  addSyntax(0, "");
  addSyntax(1, "<player>");
  brief("Manage group membership.");
  return;
}

bool CmdGroup::execute(Creature* creature, const std::vector<std::string>& args) {
  std::string name;
  Group* this_group = creature->group();
  Group* new_group  = NULL;
  Creature* target  = NULL;
  Creature* leader  = NULL;

  name = args[0];
  // Empty name - list the group stats.
  if (name.empty()) {
    creature->send("+----------------------------------------------------------------------------+\n");
    creature->send("| Group Leader: {B%-20s{x {YLEVEL   {GHEALTH      {CMANA       {MSTAMINA  {x|\n", this_group->leader()->identifiers().shortname().c_str());
    creature->send("+----------------------------------------------------------------------------+\n");
    for (std::set<Creature*>::iterator it = this_group->members().begin(); it != this_group->members().end(); ++it) {
      creature->send("| {W%-20s{x                {Y%3d{x  {G%4d{x/{g%-4u  {C%4u{x/{c%-4u       {M%3u{x    |\n",
        (*it)->identifiers().shortname().c_str(),
        (*it)->level(), (*it)->health(), (*it)->maxHealth(), (*it)->mana(), (*it)->maxMana(), (*it)->stamina()
      );
    }
    creature->send("+----------------------------------------------------------------------------+\n");
  }
  // Got a name - look for the player and join, if possible.
  else {
    // Find the target.
    if ((target = creature->findCreature(args[0])) == NULL) {
      creature->send("You don't see them here.");
      return false;
    }
    // Make sure you're following the leader of the group (for example, if Alice is following
    // Bob, and you follow Alice, you actually want to group with Bob directly).
    new_group = target->group();
    leader    = new_group->leader();
    // Can't follow yourself.
    if (target == creature) {
      creature->send("Follow yourself? How would that work?");
      return false;
    }
    // Same group, silly.
    if (leader == creature) {
      creature->send("You're already in the same group.");
      return false;
    }
    // Everyone in your group should follow the new leader, otherwise we have to either disband
    // your current group, or arbitrarily select a new leader. Both of those options seem suboptimal
    // so instead we merge the two groups under the new leader. This same logic also works for the
    // simplified case where you're solo.
    std::set<Creature*> current_members(this_group->members());
    for (std::set<Creature*>::iterator iter = current_members.begin(); iter != current_members.end(); ++iter) {
      this_group->remove_member(*iter);
      new_group->add_member(*iter);
      (*iter)->group(new_group);
      (*iter)->send("You are now grouped with %s.\n", leader->identifiers().shortname().c_str());
      leader->send("%s has joined your group.\n", (*iter)->identifiers().shortname().c_str());
      (*iter)->room()->send_cond("$p is now grouped with $c.\n", *iter, leader, NULL, TO_NOTVICT);
    }
  }
  return true;
}

CmdHeal::CmdHeal(void) {
  name("heal");
  playerOnly(true);
  level(DEMIGOD);
  addSyntax(1, "<target>");
  addSyntax(1, "all");
  brief("Restores hp/mp/mv to the target, or all players.");
  return;
}

bool CmdHeal::execute(Creature* creature, const std::vector<std::string>& args) {
  Creature* target = NULL;
  if (args[0] == "all") {
    for (std::map<std::string, Avatar*>::iterator iter = World::Instance().getAvatars().begin(); iter != World::Instance().getAvatars().end(); ++iter) {
      iter->second->heal();
      if (iter->second == creature) continue;
      iter->second->send(iter->second->seeName(creature, true));
      iter->second->send(" has healed you.\n");
    }
    creature->send("You have healed the world.\n");
  } else {
    if ((target = World::Instance().findCreature(args[0])) == NULL) {
      avatar()->send("They're not around at the moment.");
      return false;
    }
    target->heal();
    if (target == creature) {
      target->send("You heal yourself.\n");
    } else {
      target->send(target->seeName(creature, true));
      target->send(" has healed you.\n");
      creature->send("You have healed %s.\n", target->identifiers().shortname().c_str());
    }
  }
  return true;
}

CmdHelp::CmdHelp( void ) {
  name( "help" );
  playerOnly( true );
  addSyntax( 1, "<command>" );
  brief( "Provides information about commands." );
  return;
}

bool CmdHelp::execute( Creature* creature, const std::vector<std::string>& args ) {
  Command* command = NULL;
  if ( ( command = avatar()->IOhandler()->commandTable()->find( args[0] ) ) == NULL || command->level() > avatar()->level() ) {
    avatar()->send( "%s is not a valid command.", args[0].c_str() );
    return false;
  }
  avatar()->send( command->printSyntax() );
  return true;
}

CmdIdentify::CmdIdentify( void ) {
  name( "identify" );
  level( DEMIGOD );
  addSyntax( 1, "<object>" );
  addSyntax( 1, "<mob>" );
  brief( "Displays diagnostic information on the target." );
  return;
}

bool CmdIdentify::execute( Creature* creature, const std::vector<std::string>& args ) {
  Object* o = NULL;
  Creature* c = NULL;
  std::string output;
  if ( ( c = avatar()->findCreature( args[0] ) ) == NULL ) {
    // check for objects matching the arg
    if ( ( o = avatar()->findObject( args[0] ) ) == NULL ) {
      avatar()->send( "You don't see that here." );
      return false;
    } else {
      avatar()->send( o->printStatus() );
    }
  } else {
    if ( c->isMob() ) {
      avatar()->send( Mob::getInformation( (Mob*)c ) );
    } else {
      avatar()->send( "Sorry - 'identify' doesn't work on players yet" );
      return false;
    }
  }
  return true;
}

CmdIncognito::CmdIncognito( void ) {
  name( "incognito" );
  level( GOD );
  addSyntax( 0, "" );
  brief( "Makes an immortal undetectable." );
  return;
}

bool CmdIncognito::execute( Creature* creature, const std::vector<std::string>& args ) {
  avatar()->adminFlags().toggle( ADMIN_INCOGNITO );
  if ( avatar()->adminFlags().test( ADMIN_INCOGNITO ) ) {
    avatar()->send( "You disappear." );
  } else {
    avatar()->send( "You fade into existence." );
    avatar()->room()->send_cond( "$p fades into existence.", avatar() );
  }
  return true;
}

CmdInventory::CmdInventory( void ) {
  name( "inventory" );
  addSyntax( 0, "" );
  brief( "Displays the objects currently carried." );
  return;
}

bool CmdInventory::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string output;
  if ( creature->inventory().objectList().empty() ) {
    creature->send( "You aren't carrying anything at the moment." );
    return true;
  }
  creature->send( "You are carrying:\n" );
  creature->send( creature->inventory().listObjects().c_str() );
  return true;
}

CmdKill::CmdKill(void) {
  name("kill");
  addSyntax(1, "<target>");
  brief("Attack a player or mob, initiating combat.");
  return;
}

bool CmdKill::execute(Creature* creature, const std::vector<std::string>& args) {
  Creature* target = NULL;
  if ((target = creature->findCreature(args[0])) == NULL) {
    creature->send("But they're not even here right now!");
    return false;
  } else if (target == creature) {
    creature->send("Your sense of honor precludes suicide.");
    return false;
  } else if (!creature->deplete_stamina(1)) {
    return false;
  } else {
    creature->send("You attack %s!\n", target->name());
    target->send("%s attacks you!\n", creature->name());
    creature->room()->send_cond("$p attacks $c!\n", creature, target, NULL, TO_NOTVICT);
    creature->add_opponent(target);
  }
  return true;
}

CmdLay::CmdLay( void ) {
  name( "lay" );
  addSyntax( 0, "" );
  brief( "Lay down and rest." );
  return;
}

bool CmdLay::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string error;
  if ( !creature->lay( error ) ) {
    creature->send( error );
    return false;
  }
  creature->send( "You lay down." );
  creature->room()->send_cond( "$p lays down.", creature );
  return true;
}

CmdLearn::CmdLearn(void) {
  name("learn");
  playerOnly(true);
  addSyntax(-1, "<name of skill>");
  addSyntax(-1, "<name of spell>");
  seeAlso("skills");
  seeAlso("spells");
  brief("Use training points to learn an available ability.");
  return;
}

bool CmdLearn::execute(Creature* creature, const std::vector<std::string>& args) {
  if (args.empty()) {
    creature->send(printSyntax());
    return false;
  }

  AbilityMap::const_iterator iter = creature->klass()->abilities().abilitiesByName().find(args[0]);
  if (iter == creature->klass()->abilities().abilitiesByName().end()) {
    creature->send("Can't find that ability.");
    return false;
  }

  if (!creature->can_learn(iter->second)) {
    creature->send("You can't learn that ability just yet.");
    return false;
  }
  creature->learn(iter->second, 25);
  creature->trains(creature->trains() - iter->second->trains());
  creature->send("You spend {B%u{x training points learning the {M%s{x ability!\n", iter->second->trains(), iter->second->name().c_str());
  creature->send("You now have {B%u{x remaining training points.\n", creature->trains());
  return true;
}

CmdLoadRule::CmdLoadRule( void ) {
  name( "loadrule" );
  level( DEMIGOD );
  addSyntax( 1, "list" );
  addSyntax( 2, "delete <rule>" );
  addSyntax( 5, "add mob <vnum> <number> <max>" );
  addSyntax( 6, "add mob <vnum> <number> <max> <probability>" );
  addSyntax( 5, "add object <vnum> <number> <max>" );
  addSyntax( 6, "add object <vnum> <number> <max> <probability>" );
  addSyntax( 7, "add object <vnum> <number> <max> in <target>[#index]" );
  addSyntax( 8, "add object <vnum> <number> <max> in <target>[#index] <probability>" );
  addSyntax( 7, "add object <vnum> <number> <max> on <target>[#index]" );
  addSyntax( 8, "add object <vnum> <number> <max> on <target>[#index] <probability>" );
  addSyntax( 7, "add object <vnum> <number> <max> <target>[#index] carry" );
  addSyntax( 8, "add object <vnum> <number> <max> <target>[#index] carry <probability>" );
  addSyntax( 7, "add object <vnum> <number> <max> <target>[#index] wear" );
  addSyntax( 8, "add object <vnum> <number> <max> <target>[#index] wear <probability>" );
  brief( "Manipulates the reset rules for the room." );
  return;
}

/*"--------Description:\n\
1) View the Load Rules for the current room.\n\
2) Delete the given Load Rule.\n\
3) Create a Load Rule for mob <vnum>.\n\
4) Create a Load Rule for object <vnum>.\n\
5) Like #4, but this object should then be placed in the object specified by vnum <target>, which must be a container.\n\
6) Like #4, but this object should then be placed on the object specified by vnum <target>, which must be a piece of furniture.\n\
7) Like #4, but this object should then be placed in the inventory of the mob specified by vnum <target>.\n\
8) Like #4, but this object should then be worn by the mob specified by vnum <target>.\n\n\
--------Notes:\n\
Load Rules are the rules by which the server automatically loads mobs and\n\
objects into the game when areas are \"reset.\"  Every room has it's own list\n\
of Rules that get evaluated when the area resets. For all Load Rules, <number>\n\
specifies how many clones (objects or mobs) should be generated. No more of\n\
these clones will be created after there are <max> copies already anywhere in\n\
the area. By default the engine will create <number> clones on every single\n\
area reset until the <max> is reached, however by specifying the optional\n\
[probability] argument (a number between 1 and 99), you can control how often\n\
a clone is made.\n\n\
Additionally, for statements 5 through 8, you may optionally include an index\n\
argument.  If, for example, you are loading two identical pouches (say the\n\
pouch vnum is 934) into a room, and you want a potion to load into the second\n\
pouch, your \"<target>[.index]\" argument should look like \"934#2\"\n\n\
--------Examples:\n\
\"loadrule add mob 654 1 4\" would specify that 1 of mob 654 should be loaded on every reset, until there are 4 total in the area.\n\
\"loadrule add object 3967 5 5\" would specify a rule to load up to five instances of object 3967.\n\
\"loadrule add object 1234 1 1 5\" would create a rule to load object 1234 approximately 5% of the time.\n\
\"loadrule add object 4354 1 1 654 carry\" loads one instance of object 4354, and puts it in the inventory of mob 654."*/

bool CmdLoadRule::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::deque<std::string> pieces;
  std::string foo;
  std::vector<std::string> bar;
  Room* room = creature->room();
  std::list<LoadRule*>* rules = &(room->loadRules());
  std::list<LoadRule*>::iterator it;
  LoadRule* rule = NULL;
  LoadRuleMob* mobRule = NULL;
  LoadRuleObject* objectRule = NULL;
  CmdLoadRule cmdLoadRule;
  std::vector<std::string> load_rule_args;
  unsigned rule_number = 0;
  unsigned type = 0;
  unsigned target = 0;
  unsigned number = 0;
  unsigned max = 0;
  unsigned probability = 0;

  // Do we have any arguments?
  if ( args.empty() ) {
    avatar()->send( printSyntax() );
    return false;
  }

  pieces.insert( pieces.begin(), args.begin(), args.end() );

  // Check permissions...
  if ( !World::Instance().hasPermission( room->area(), avatar() ) ) {
    avatar()->send( "You don't have permissions to the LoadRules in this room." );
    return false;
  }

  // Provide a listing...
  if ( pieces[0] == "list" ) {
    if ( rules->empty() ) {
      avatar()->send( "There are no LoadRules defined for this room." );
      return true;
    } else {
      avatar()->send( "LoadRules for Room %lu (%s):\n\n", room->vnum(), room->name().c_str() );
      avatar()->send( "#  | type |  vnum | number | max | probability | notes\n------------------------------------------------------------------------------\n" );
      for ( it = rules->begin(), rule_number=1; it != rules->end(); ++it, ++rule_number ) {
        switch ( (*it)->type() ) {
          case LOADRULE_MOB:
            mobRule = (LoadRuleMob*)*it;
            avatar()->send( "%02u | %4s | %5u |   %2u   |  %2u |   %03u/100   | %s\n", rule_number, mobRule->strType().c_str(), mobRule->target(), mobRule->number(), mobRule->max(), mobRule->probability(), "N/A" );
            break;
          case LOADRULE_OBJECT:
            objectRule = (LoadRuleObject*)*it;
            avatar()->send( "%02u | %4s | %5u |   %2u   |  %2u |   %03u/100   | %s\n", rule_number, objectRule->strType().c_str(), objectRule->target(), objectRule->number(), objectRule->max(), objectRule->probability(), objectRule->notes().c_str() );
            break;
          default:
            break;
        }
      }
      return true;
    }
  }
  // Deleting a rule...
  else if ( pieces[0] == "delete" ) {
    if ( pieces.size() != 2 ) {
      avatar()->send( printSyntax() );
      return false;
    }
    rule_number = estring(pieces[1]);
    if ( !rule_number || rule_number > rules->size() ) {
      avatar()->send( "There is no rule #%u.", rule_number );
      return false;
    } else {
      room->removeLoadRule( rule_number );
      avatar()->send( "Rule #%u has been removed.", rule_number );
      return true;
    }
  }
  // Create a rule...
  else if ( pieces[0] == "add" ) {
    // Do we have required information?
    if ( pieces.size() < 3 ) {
      avatar()->send( printSyntax() );
      return false;
    }
    pieces.pop_front();

    // What kind of rule?
    if ( pieces[0] == "mob" ) {
      type = LOADRULE_MOB;
    } else if ( pieces[0] == "object" ) {
      type = LOADRULE_OBJECT;
    } else {
      avatar()->send( printSyntax() );
      return false;
    }
    pieces.pop_front();

    // Get the target vnum...
    target = estring(pieces[0]);
    pieces.pop_front();

    // How many should we load?
    number = estring(pieces[0]);
    number = number > 10 ? 10 : number;
    pieces.pop_front();

    // Max to load?
    max = estring(pieces[0]);
    max = max > 99 ? 99 : max;
    pieces.pop_front();

    // Construct the Rule...
    switch ( type ) {
      case LOADRULE_MOB:
        rule = new LoadRuleMob();
        if ( pieces.empty() ) {
          probability = 100;
        } else {
          probability = estring(pieces[0]);
        }
        break;
      case LOADRULE_OBJECT:
        rule = new LoadRuleObject();
        if ( pieces.empty() ) {
          probability = 100;
          break;
        } else if ( pieces.size() == 1 ) {
          probability = estring(pieces[0]);
          pieces.pop_front();
          break;
        } else if ( pieces.size() == 2 || pieces.size() == 3 ) {
          if ( pieces[0] == "in" ) {
            ((LoadRuleObject*)rule)->preposition( LOADRULE_IN );
            foo = estring(pieces[1]);
          } else if ( pieces[0] == "on" ) {
            ((LoadRuleObject*)rule)->preposition( LOADRULE_ON );
            foo = estring(pieces[1]);
          } else if ( pieces[1] == "carry" ) {
            ((LoadRuleObject*)rule)->preposition( LOADRULE_CARRY );
            foo = estring(pieces[0]);
          } else if ( pieces[1] == "wear" ) {
            ((LoadRuleObject*)rule)->preposition( LOADRULE_WEAR );
            foo = estring(pieces[0]);
          } else {
            avatar()->send( printSyntax() );
            return false;
          }
          pieces.pop_front();
          pieces.pop_front();
          bar = Regex::explode( "#", foo );
          switch ( bar.size() ) {
            case 1:
              ((LoadRuleObject*)rule)->indirectObject( estring(bar[0]) );
              ((LoadRuleObject*)rule)->indirectObjectIndex( 1 );
              break;
            case 2:
              ((LoadRuleObject*)rule)->indirectObject( estring(bar[0]) );
              ((LoadRuleObject*)rule)->indirectObjectIndex( estring(bar[1]) );
              break;
            default:
              avatar()->send( printSyntax() );
              return false;
          }
          if ( pieces.empty() ) {
            probability = 100;
          } else {
            probability = estring(pieces[0]);
            pieces.pop_front();
          }
        } else {
          avatar()->send( printSyntax() );
        }
        break;
      default:
        avatar()->send( printSyntax() );
        return false;
    }

    // Basic settings...
    rule->vnum( room->vnum() );
    rule->room( room );
    rule->type( type );
    rule->target( target );
    rule->number( number );
    rule->max( max );
    rule->probability( probability > 100 ? 100 : probability );
    rule->commit();
    rules->push_back( rule );
    avatar()->send( "Rule created: " );
    load_rule_args.push_back( "list" );
    cmdLoadRule.avatar( avatar() );
    cmdLoadRule.execute( creature, load_rule_args );
    return true;
  } else {
    avatar()->send( printSyntax() );
    return false;
  }
}

CmdLock::CmdLock( void ) {
  name( "lock" );
  addSyntax( 1, "<direction>" );
  brief( "Locks a door." );
  return;
}

bool CmdLock::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::list<Object*> foo;
  Exit* exit = NULL;
  Object* key = NULL;

  if ( ( exit = creature->room()->exit( args[0] ) ) == NULL ) {
    creature->send( "There is no door in that direction." );
    return false;
  }
  if ( !exit->flag( EXIT_CLOSED ) ) {
    creature->send( "It's open." );
    return false;
  }
  if ( exit->flag( EXIT_LOCKED ) ) {
    creature->send( "It's already locked." );
    return false;
  }
  if ( !exit->flag( EXIT_LOCKABLE ) ) {
    creature->send( "You can't lock that." );
    return false;
  }

  foo = creature->inventory().searchObjects( exit->key() );
  if ( !foo.empty() ) {
    key = foo.front();
  }

  if ( key == NULL || !key->isKey() ) {
    creature->send( "You don't have the key." );
    return false;
  }

  exit->flag( EXIT_LOCKED, true );
  creature->send( "You lock the door." );
  creature->room()->send_cond( "$p locked the way $e.", creature, exit );
  return true;
}

CmdLook::CmdLook( void ) {
  name( "look" );
  shortcut( "l" );
  addSyntax( 0, "                  (look around you)" );
  addSyntax( 1, "<object>          (examine an object)" );
  addSyntax( 1, "<player>          (examine a player)" );
  addSyntax( 1, "<mob>             (examine a mob)" );
  addSyntax( 2, "in <container>    (examine the contents)" );
  brief( "Examines the room or target object, player, or mob." );
  return;
}

bool CmdLook::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string output;
  char buffer[MAX_BUFFER];
  bool has_exits = false;
  Object* container = NULL;
  Creature* ctarget = NULL;
  Object* otarget = NULL;

  if ( args[0].empty() ) {
    /*************** looking at the room */
    if ( creature->level() >= DEMIGOD ) {
      sprintf( buffer, "\n\n[{g%lu{x] {%c%s{x\n", creature->room()->vnum(), creature->room()->terrain()->title(), creature->room()->name().c_str() );
    } else {
      sprintf( buffer, "\n\n{%c%s{x\n", creature->room()->terrain()->title(), creature->room()->name().c_str() );
    }
    output.append( buffer );
    // Room description...
    sprintf( buffer, "{%c%s{x\n\n", creature->room()->terrain()->description(), creature->room()->description().c_str() );
    output.append( buffer );
    // Exits...
    memset( buffer, 0, MAX_BUFFER );
    for ( unsigned u = NORTH; u <= DOWN; ++u ) {
      if ( creature->room()->exit( u ) != NULL ) {
        // Bail if we're not supposed to see the exit...
        if ( creature->room()->exit( u )->flag( EXIT_HIDDEN ) && creature->level() < DEMIGOD ) {
          continue;
        }
        has_exits = true;
        // Open brackets, parentheses, etc...
        if ( creature->room()->exit( u )->flag( EXIT_HIDDEN ) ) {
          strcat( buffer, "{W<" );
        }
        if ( creature->room()->exit( u )->flag( EXIT_CLOSED ) ) {
          strcat( buffer, "{R[" );
        }
        // Exit name...
        strcat( buffer, "{C" );
        strcat( buffer, Exit::name( u ) );
        // Close brackets, parentheses, etc...
        if ( creature->room()->exit( u )->flag( EXIT_CLOSED ) ) {
          strcat( buffer, "{R]" );
        }
        if ( creature->room()->exit( u )->flag( EXIT_HIDDEN ) ) {
          strcat( buffer, "{W>" );
        }
        strcat( buffer, " " );
      }
    }
    if ( has_exits ) {
      output.append( "[ {WExits{x: {C" ).append( buffer ).append( "{x]" );
    }
    // Objects...
    output.append( 1, '\n' ).append( creature->room()->inventory().listObjects() );
    // Creatures...
    for ( std::list<Creature*>::iterator it = creature->room()->creatures().begin(); it != creature->room()->creatures().end(); ++it ) {
      if ( *it != creature && creature->canSee( *it ) == SEE_NAME ) {
        output.append( "{x\n " );
        if ( creature->isAvatar() ) {
          if ( ((Avatar*)creature)->whoFlags().test( WHO_AFK ) ) {
            output.append( "[{YAFK{x] " );
          }
        }
        output.append( (*it)->identifiers().shortname() ).append( "{x");
        output.append(", a ").append((*it)->gender().string()).append(" ").append((*it)->race().string()).append(",");
        output.append(" is ").append((*it)->position().string()).append(" here.");
      }
    }
    creature->send( output );
    return true;
  } else if ( args.size() == 1 ) {
    if ( ( ctarget = creature->findCreature( args[0] ) ) != NULL ) {
      // looking at a creature
      creature->send( "You look at %s:\n%s{x\n\n", ctarget->identifiers().shortname().c_str(), ctarget->identifiers().description().c_str() );
      for ( unsigned u = 1; u < WEARLOC_END; ++u ) {
        if ( ctarget->worn( u ) ) {
          creature->send( "%s%s{x\n", Creature::wearLocName( u ), ctarget->worn( u )->decorativeShortname().c_str() );
        }
      }
      if ( ctarget->canSee( creature ) > SEE_NOTHING ) {
        ctarget->send( "%s looks at you.\n", ctarget->seeName( creature ).c_str() );
      }
      creature->room()->send_cond( "$p looks at $c.\n", creature, ctarget, NULL, TO_NOTVICT );
      return true;
    } else if ( ( otarget = creature->room()->inventory().searchSingleObject( args[0] ) ) != NULL ) {
      // looking at an object
      creature->send( "You look at %s:\n%s{x\n", otarget->identifiers().shortname().c_str(), otarget->identifiers().description().c_str() );
      creature->room()->send_cond( "$p looks at $o.\n", creature, otarget );
      return true;
    } else {
      creature->send( "You don't see that here." );
      return false;
    }
  } else if ( args.size() == 2 ) {
    /*************** looking in something */
    if ( args[0] == "i" || args[0] == "in" ) {
      if ( ( container = creature->findObject( args[1] ) ) == NULL ) {
        creature->send( "You don't see that here." );
        return false;
      }
      if ( !container->isContainer() ) {
        creature->send( "That's not a container." );
        return false;
      }
      if ( container->container()->inventory().objectList().empty() ) {
        creature->send( "%s{x is empty.", container->identifiers().shortname().c_str() );
        return true;
      }
      creature->send( "You look in %s:\n", container->identifiers().shortname().c_str() );
      creature->send( container->container()->inventory().listObjects() );
      return true;
    } else {
      creature->send( printSyntax() );
      return false;
    }
  } else {
    creature->send( printSyntax() );
    return false;
  }
}

CmdMap::CmdMap( void ) {
  name( "map" );
  shortcut( "m" );
  addSyntax( 0, "" );
  brief( "Displays a map of your surroundings." );
  return;
}

bool CmdMap::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string output;
  unsigned xsize = 37;
  unsigned ysize = 9;
  unsigned x = 0;
  unsigned y = 0;
  bool success = false;

  // Allocate a 2D array of Room pointers and initialize to NULL...
  Room*** map = new Room**[xsize];
  for ( y = 0; y < ysize; ++y ) {
    map[y] = new Room*[xsize];
    for ( x = 0; x < xsize; ++x ) {
      map[y][x] = NULL;
    }
  }

  // Allocate a 2D array of characters and initialize to spaces...
  std::string** display = new std::string*[ysize*2];
  for ( y = 0; y < ysize*2; ++y ) {
    display[y] = new std::string[xsize*2];
    for ( x = 0; x < xsize*2; ++x ) {
      display[y][x] = " ";
    }
  }

  // Start a depth-first search of the map, beginning in the current room...
  success = World::search_map( creature, map, ysize/2, xsize/2, ysize/2, xsize/2, creature->room(), display );

  // Print out our map...
  output.append( "{x" ).append( xsize*2+2, '-' ).append( 1, '\n' );
  for ( y = ysize*2-1; y > 0; --y ) {
    output.append( "{x|" );
    for ( x=0; x < xsize*2; ++x ) {
      output.append( display[y][x] );
    }
    output.append( "{x|\n" );
  }
  output.append( "{x" ).append( xsize*2+2, '-' ).append( 1, '\n' );

  if ( !success ) {
    output.append( "There was an error generating this map because the area isn't square.\nThe output may be garbled.\nPlease contact the Head Builder." );
  }

  creature->send( output );
  return true;
}

CmdMedit::CmdMedit( void ) {
  name( "medit" );
  level( DEMIGOD );
  addSyntax( 1, "<vnum>" );
  addSyntax( 2, "create <vnum>" );
  brief( "Launches the Mob Editor." );
  return;
}

bool CmdMedit::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::map<unsigned long,Mob*>::iterator it;
  Area* area = NULL;
  Mob* mob = NULL;
  unsigned long vnum = 0;

  if ( args.size() == 1 ) {
    vnum = estring(args[0]);
    // Get the area...
    if ( ( area = World::Instance().lookup( vnum ) ) == NULL ) {
      avatar()->send( "That vnum doesn't exist." );
      return false;
    }
    // Check permissions...
    if ( !World::Instance().hasPermission( area, avatar() ) ) {
      avatar()->send( "You don't have permissions to that object." );
      return false;
    }
    // Make sure the Object exists...
    if ( ( it = area->mobs().find( vnum ) ) == area->mobs().end() ) {
      avatar()->send( "That mob doesn't exist." );
      return false;
    }
    // Make sure no one else is editing the object...
    for ( std::map<std::string,Avatar*>::iterator a_it = World::Instance().getAvatars().begin(); a_it != World::Instance().getAvatars().end(); ++a_it ) {
      if ( a_it->second->mode().number() == MODE_MEDIT && a_it->second->medit() == it->second ) {
        avatar()->send( "Sorry, %s is currently editing %s (mob %lu).", avatar()->seeName( ((Creature*)a_it->second) ).c_str(), it->second->identifiers().shortname().c_str(), it->second->vnum() );
        return false;
      }
    }
    // All looks well; send them to medit...
    avatar()->medit( it->second );
    avatar()->pushIOHandler( new MeditIOHandler( avatar() ) );
    avatar()->send( "You're editing mob %lu.", avatar()->medit()->vnum() );
    return true;
  } else if ( args.size() == 2 ) {
    if ( !Regex::strPrefix( args[0], "create" ) ) {
      avatar()->send( printSyntax() );
      return false;
    }
    vnum = estring(args[1]);
    // Get the area...
    if ( ( area = World::Instance().lookup( vnum ) ) == NULL ) {
      avatar()->send( "That vnum doesn't exist." );
      return false;
    }
    // Check permissions...
    if ( !World::Instance().hasPermission( area, avatar() ) ) {
      avatar()->send( "You don't have permissions to that mob." );
      return false;
    }
    // Make sure the Mob doesn't already exist...
    if ( ( it = area->mobs().find( vnum ) ) != area->mobs().end() ) {
      avatar()->send( "That mob already exists." );
      return false;
    }
    // Everything checks out; let's make us a new Mob!
    if ( ( mob = Mob::create( area, vnum ) ) == NULL ) {
      avatar()->send( "There was an error creating the mob." );
      return false;
    }
    avatar()->send( "Mob %lu created successfully.", mob->vnum() );
    avatar()->mode().set( MODE_MEDIT );
    avatar()->medit( mob );
    avatar()->pushIOHandler( new MeditIOHandler( avatar() ) );
    return true;
  }

  return false;
}

CmdMlist::CmdMlist( void ) {
  name( "mlist" );
  level( DEMIGOD );
  addSyntax( 1, "<areaID>                       (list all Mobs in the area)" );
  addSyntax( 2, "<first vnum> <last vnum>       (list all Mobs in the vnum range)" );
  addSyntax( 1, "<keyword>                      (list all Mobs by keyword)" );
  addSyntax( 1, "/<regex>                       (list all Mobs matching the PCRE)" );
  return;
}

bool CmdMlist::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::vector<std::string> mutable_args( args );
  std::vector<Mob*> mobs;
  Area* area = NULL;
  unsigned long low = 0;
  unsigned long high = 0;
  std::string search;
  std::string output;
  char buffer[MAX_BUFFER];

  if ( mutable_args.size() == 1 ) {
    if ( Regex::match( "^[0-9]+$", mutable_args[0] ) ) {
      // We got an areaID...
      if ( ( area = World::Instance().findArea( estring(mutable_args[0]) ) ) == NULL ) {
        creature->send( "That area couldn't be found." );
        return false;
      }
      for ( std::map<unsigned long,Mob*>::iterator m_it = area->mobs().begin(); m_it != area->mobs().end(); ++m_it ) {
        mobs.push_back( m_it->second );
      }
    } else {
      if ( mutable_args[0][0] == '/' ) {
        mutable_args[0].erase( 0, 1 );
        // This search is a regex...
        for ( std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it ) {
          for ( std::map<unsigned long,Mob*>::iterator m_it = (*a_it)->mobs().begin(); m_it != (*a_it)->mobs().end(); ++m_it ) {
            if ( m_it->second->identifiers().matchesKeyword( mutable_args[0] ) ) {
              mobs.push_back( m_it->second );
            }
          }
        }
      } else {
        search = Regex::lower(mutable_args[0]);
        // We got a search string...
        for ( std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it ) {
          for ( std::map<unsigned long,Mob*>::iterator m_it = (*a_it)->mobs().begin(); m_it != (*a_it)->mobs().end(); ++m_it ) {
            if ( m_it->second->identifiers().matchesKeyword( search ) ) {
              mobs.push_back( m_it->second );
            }
          }
        }
      }
    }
  } else if ( mutable_args.size() == 2 ) {
    /* We're looking for a vnum range here */
    // Grab our range values...
    low = estring(mutable_args[0]);
    high = estring(mutable_args[1]);
    // Check our range...
    if ( !high || low >= high ) {
      creature->send( "Invalid vnum range." );
      return false;
    }
    if ( low+400 < high ) {
      creature->send( "The maximum vnum range is 400." );
      return false;
    }
    // Grab the mobs...
    for ( std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it ) {
      for ( std::map<unsigned long,Mob*>::iterator m_it = (*a_it)->mobs().begin(); m_it != (*a_it)->mobs().end(); ++m_it ) {
        if ( m_it->second->vnum() >= low && m_it->second->vnum() <= high ) {
          mobs.push_back( m_it->second );
        }
      }
    }
  }

  if ( mobs.empty() ) {
    creature->send( "No matches for \"%s\"", mutable_args[0].c_str() );
    return false;
  }

  output.append( " [{y vnum{x] {gname{x\n -------------------\n" );
  for ( std::vector<Mob*>::iterator it = mobs.begin(); it != mobs.end(); ++it ) {
    sprintf( buffer, " [{y%5lu{x] %s{x\n", (*it)->vnum(), (*it)->identifiers().shortname().c_str() );
    output.append( buffer );
  }

  creature->send( output );
  return true;
}

CmdMload::CmdMload( void ) {
  name( "mload" );
  level( DEMIGOD );
  addSyntax( 1, "<vnum>" );
  brief( "Incarnates a Mob." );
}

bool CmdMload::execute( Creature* creature, const std::vector<std::string>& args ) {
  unsigned long vnum = estring(args[0]);
  std::map<unsigned long,Mob*>::iterator m_it;
  Mob* mob = NULL;

  for ( std::set<Area*,area_comp>::iterator a_it = World::Instance().getAreas().begin(); a_it != World::Instance().getAreas().end(); ++a_it ) {
    if ( ( m_it = (*a_it)->mobs().find( vnum ) ) != (*a_it)->mobs().end() ) {
      mob = Mob::create( m_it->second, creature->room() );
      if ( mob == NULL ) {
        creature->send( "Mload failed. Don't you feel stupid?" );
        return false;
      }
      if ( mob->identifiers().shortname().empty() || mob->identifiers().longname().empty() || mob->identifiers().getKeywords().empty() ) {
        avatar()->send( "Sorry; that mob isn't complete yet." );
        return false;
      }
      World::Instance().insert( mob );
      creature->room()->add( mob );
      mob->room( creature->room() );
      creature->send( "You load %s.", mob->identifiers().shortname().c_str() );
      creature->room()->send_cond( "$p has created $c.", creature, mob );
      return true;
    }
  }

  creature->send( "There is no mob of that vnum." );
  return false;
}

CmdMoveNorth::CmdMoveNorth( void ) {
  name( "north" );
  shortcut( "n" );
  allowedWhile( STANDING | MOUNTED );
  addSyntax( 0, "" );
  brief( "Move to the North." );
  return;
}

bool CmdMoveNorth::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string message;
  if ( creature->canMove( NORTH, message ) ) {
    creature->move( NORTH );
    return true;
  } else {
    creature->send( message );
    return false;
  }
}

CmdMoveEast::CmdMoveEast( void ) {
  name( "east" );
  shortcut( "e" );
  allowedWhile( STANDING | MOUNTED );
  addSyntax( 0, "" );
  brief( "Move to the East." );
  return;
}

bool CmdMoveEast::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string message;
  if ( creature->canMove( EAST, message ) ) {
    creature->move( EAST );
    return true;
  } else {
    creature->send( message );
    return false;
  }
}

CmdMoveSouth::CmdMoveSouth( void ) {
  name( "south" );
  shortcut( "s" );
  allowedWhile( STANDING | MOUNTED );
  addSyntax( 0, "" );
  brief( "Move to the South." );
  return;
}

bool CmdMoveSouth::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string message;
  if ( creature->canMove( SOUTH, message ) ) {
    creature->move( SOUTH );
    return true;
  } else {
    creature->send( message );
    return false;
  }
}

CmdMoveWest::CmdMoveWest( void ) {
  name( "west" );
  shortcut( "w" );
  allowedWhile( STANDING | MOUNTED );
  addSyntax( 0, "" );
  brief( "Move to the West." );
  return;
}

bool CmdMoveWest::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string message;
  if ( creature->canMove( WEST, message ) ) {
    creature->move( WEST );
    return true;
  } else {
    creature->send( message );
    return false;
  }
}

CmdMoveUp::CmdMoveUp( void ) {
  name( "up" );
  shortcut( "u" );
  allowedWhile( STANDING | MOUNTED );
  addSyntax( 0, "" );
  brief( "Move up." );
  return;
}

bool CmdMoveUp::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string message;
  if ( creature->canMove( UP, message ) ) {
    creature->move( UP );
    return true;
  } else {
    creature->send( message );
    return false;
  }
}

CmdMoveDown::CmdMoveDown( void ) {
  name( "down" );
  shortcut( "d" );
  allowedWhile( STANDING | MOUNTED );
  addSyntax( 0, "" );
  brief( "Move Dowm." );
  return;
}

bool CmdMoveDown::execute( Creature* creature, const std::vector<std::string>& args ) {
  std::string message;
  if ( creature->canMove( DOWN, message ) ) {
    creature->move( DOWN );
    return true;
  } else {
    creature->send( message );
    return false;
  }
}
