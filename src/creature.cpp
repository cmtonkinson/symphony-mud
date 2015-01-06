/*
 * $Id: creature.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#include "area.h"
#include "commandTable-default.h"
#include "creature.h"
#include "display.h"
#include "exit.h"
#include "group.h"
#include "identifiers.h"
#include "io-handler.h"
#include "job.h"
#include "object-furniture.h"
#include "object-weapon.h"
#include "room.h"
#include "world.h"

Creature::Creature( void ):
    _inventory( &Identifiers::shortname ) {
  ID(0);
  room( NULL );
  position().set( STANDING );
  action().set(0);
  // Default group...
  group(new Group());
  group()->add_member(this);
  group()->leader(this);
  // identity...
  gender().set( NEUTRAL );
  race().set(0);
  pClass().set(0);
  // stats...
  furniture( NULL );
  // Level
  level(1);
  exp(0);
  tnl(0);
  // Health
  health(100);
  maxHealth(100);
  mana(100);
  maxMana(100);
  movement(100);
  maxMovement(100);
  // Stats
  strength(15);
  maxStrength(20);
  dexterity(15);
  maxDexterity(20);
  constitution(15);
  maxConstitution(20);
  intelligence(15);
  maxIntelligence(20);
  focus(15);
  maxFocus(20);
  creativity(15);
  maxCreativity(20);
  charisma(15);
  maxCharisma(20);
  luck(15);
  maxLuck(20);
  // Armor
  armor(100);
  bash(0);
  slash(0);
  pierce(0);
  exotic(0);
  // Misc
  gold(0);
  silver(0);
  // combat
  _next_attack = 0;
  return;
}

Creature::Creature( const Creature& ref ):
    _identifiers( ref.identifiers() ),
    _inventory( ref.inventory() ),
    _equipment( ref.equipment() ),
    _position( ref.position() ),
    _action( ref.action() ),
    // identity...
    _gender( ref.gender() ),
    _race( ref.race() ),
    _pClass( ref.pClass() ) {
  ID( ref.ID() );
  level( ref.level() );
  room( NULL );
  for ( std::list<Modifier*>::const_iterator it = ref.modifiers().begin(); it != ref.modifiers().end(); ++it ) {
    modifiers().push_back( new Modifier( **it ) );
  }
  furniture( ref.furniture() );
  // stats...
  level( ref.level() );
  exp( ref.exp() );
  tnl( ref.tnl() );
  health( ref.health() );
  maxHealth( ref.maxHealth() );
  mana( ref.mana() );
  maxMana( ref.maxMana() );
  movement( ref.movement() );
  maxMovement( ref.maxMovement() );
  strength( ref.strength() );
  maxStrength( ref.maxStrength() );
  dexterity( ref.dexterity() );
  maxDexterity( ref.maxDexterity() );
  constitution( ref.constitution() );
  maxConstitution( ref.maxConstitution() );
  intelligence( ref.intelligence() );
  maxIntelligence( ref.maxIntelligence() );
  focus( ref.focus() );
  maxFocus( ref.maxFocus() );
  creativity( ref.creativity() );
  maxCreativity( ref.maxCreativity() );
  charisma( ref.charisma() );
  maxCharisma( ref.maxCharisma() );
  luck( ref.luck() );
  maxLuck( ref.maxLuck() );
  armor( ref.armor() );
  bash( ref.bash() );
  slash( ref.slash() );
  pierce( ref.pierce() );
  exotic( ref.exotic() );
  return;
}

Creature::~Creature( void ) {
  for ( std::vector<IOHandler*>::iterator it = IOhandlers().begin(); IOhandlers().size(); ) {
    delete *it;
    it = IOhandlers().erase( it );
  }
  if (group()) {
    group()->remove_member(this);
  }
  return;
}

IOHandler* Creature::IOhandler( void ) {
  return IOhandlers().back();
}

void Creature::pushIOHandler( IOHandler* handler ) {
  IOhandlers().push_back( handler );
  handler->activate();
  return;
}

void Creature::popIOHandler( void ) {
  IOhandlers().back()->deactivate();
  IOhandlers().pop_back();
  return;
}

void Creature::replaceIOHandler( IOHandler* handler ) {
  popIOHandler();
  pushIOHandler( handler );
  return;
}

void Creature::handle( void ) {
  IOhandlers().back()->handle();
  return;
}

void Creature::naturalStatAdjustment( void ) {
  unsigned short Str = 0;
  unsigned short Dex = 0;
  unsigned short Con = 0;
  unsigned short Int = 0;
  unsigned short Foc = 0;
  unsigned short Cre = 0;
  unsigned short Cha = 0;
  unsigned short Luc = 0;
  switch ( race().number() ) {
    case ELF:
      Dex += 2;
      Int += 1;
      Cha += 2;
      Con -= 1;
      break;
    case DWARF:
      Str += 2;
      Con += 2;
      Luc += 3;
      Dex -= 1;
      Cha -= 1;
      break;
    case HUMAN:
      break;
  }
  switch ( pClass().number() ) {
    case CLERIC:
      Foc += 2;
      Cre -= 1;
      Str -= 1;
      break;
    case MAGE:
      Int += 2;
      Foc += 2;
      Cre += 2;
      Str -= 2;
      Con -= 2;
      break;
    case ROGUE:
      Dex += 2;
      Int += 1;
      Cre += 1;
      Con -= 1;
      Cha -= 2;
      break;
    case WARRIOR:
      Str += 2;
      Dex += 1;
      Con += 2;
      Int -= 2;
      Foc -= 2;
      break;
  }
  strength(strength() + Str);
  maxStrength(maxStrength() + Str);
  dexterity(dexterity() + Dex);
  maxDexterity(maxDexterity() + Dex);
  constitution(constitution() + Con);
  maxConstitution(maxConstitution() + Con);
  intelligence(intelligence() + Int);
  maxIntelligence(maxIntelligence() + Int);
  focus(focus() + Foc);
  maxFocus(maxFocus() + Foc);
  creativity(creativity() + Cre);
  maxCreativity(maxCreativity() + Cre);
  charisma(charisma() + Cha);
  maxCharisma(maxCharisma() + Cha);
  luck(luck() + Luc);
  maxLuck(maxLuck() + Luc);
  return;
}

Creature* Creature::findCreature( const std::string& name ) {
  if ( name == "self" ) {
    return this;
  }
  for ( std::list<Creature*>::iterator it = room()->creatures().begin(); it != room()->creatures().end(); ++it ) {
    if ( (*it)->identifiers().matchesKeyword( name ) && canSee( *it ) == SEE_NAME ) {
      return *it;
    }
  }
  return NULL;
}

Object* Creature::findObject( const std::string& query ) {
  Object* o = NULL;
  if ( ( o = inventory().searchSingleObject( query ) ) == NULL ) {
    if ( ( o = equipment().searchSingleObject( query ) ) == NULL ) {
      if ( ( o = room()->inventory().searchSingleObject( query ) ) == NULL ) {
        return NULL;
      }
    }
  }
  return o;
}

unsigned short Creature::getWearloc( const Object::Wearable& wearable ) const {
  switch ( wearable ) {
    case Object::Wearable_Head:       return WEARLOC_HEAD;
    case Object::Wearable_Ear:        return WEARLOC_EAR_L;
    case Object::Wearable_Face:       return WEARLOC_FACE;
    case Object::Wearable_Neck:       return WEARLOC_NECK;
    case Object::Wearable_Shoulders:  return WEARLOC_SHOULDERS;
    case Object::Wearable_Arms:       return WEARLOC_ARMS;
    case Object::Wearable_Torso:      return WEARLOC_TORSO;
    case Object::Wearable_Forearm:    return WEARLOC_FOREARM_L;
    case Object::Wearable_Wrist:      return WEARLOC_WRIST_L;
    case Object::Wearable_Hands:      return WEARLOC_HANDS;
    case Object::Wearable_Hold:       return WEARLOC_HOLD_L;
    case Object::Wearable_Finger:     return WEARLOC_FINGER_L;
    case Object::Wearable_Waist:      return WEARLOC_WAIST_1;
    case Object::Wearable_Legs:       return WEARLOC_LEGS;
    case Object::Wearable_Knee:       return WEARLOC_KNEE_L;
    case Object::Wearable_Shin:       return WEARLOC_SHIN_L;
    case Object::Wearable_Ankle:      return WEARLOC_ANKLE_L;
    case Object::Wearable_Feet:       return WEARLOC_FEET;
    default:                          return WEARLOC_END;
  }
}

const char* Creature::wearLocName( const unsigned short& wearloc ) {
  switch ( wearloc ) {
    case WEARLOC_HEAD:      return "<{ghead{x>          ";
    case WEARLOC_EAR_L:     return "<{gleft ear{x>      ";
    case WEARLOC_EAR_R:     return "<{gright ear{x>     ";
    case WEARLOC_FACE:      return "<{gface{x>          ";
    case WEARLOC_NECK:      return "<{gneck{x>          ";
    case WEARLOC_SHOULDERS: return "<{gshoulders{x>     ";
    case WEARLOC_ARMS:      return "<{garms{x>          ";
    case WEARLOC_TORSO:     return "<{gtorso{x>         ";
    case WEARLOC_FOREARM_L: return "<{gleft forearm{x>  ";
    case WEARLOC_FOREARM_R: return "<{gright forearm{x> ";
    case WEARLOC_WRIST_L:   return "<{gleft wrist{x>    ";
    case WEARLOC_WRIST_R:   return "<{gright wrist{x>   ";
    case WEARLOC_HANDS:     return "<{ghands{x>         ";
    case WEARLOC_HOLD_L:    return "<{gleft hand{x>     ";
    case WEARLOC_HOLD_R:    return "<{gright hand{x>    ";
    case WEARLOC_FINGER_L:  return "<{gleft finger{x>   ";
    case WEARLOC_FINGER_R:  return "<{gright finger{x>  ";
    case WEARLOC_WAIST_1:   return "<{gwaist{x>         ";
    case WEARLOC_WAIST_2:   return "<{gwaist{x>         ";
    case WEARLOC_LEGS:      return "<{glegs{x>          ";
    case WEARLOC_KNEE_L:    return "<{gleft knee{x>     ";
    case WEARLOC_KNEE_R:    return "<{gright knee{x>    ";
    case WEARLOC_SHIN_L:    return "<{gleft shin{x>     ";
    case WEARLOC_SHIN_R:    return "<{gright shin{x>    ";
    case WEARLOC_ANKLE_L:   return "<{gleft ankle{x>    ";
    case WEARLOC_ANKLE_R:   return "<{gright ankle{x>   ";
    case WEARLOC_FEET:      return "<{gfeet{x>          ";
    default:                return "[{Rerror{x]       ";
  }
}

bool Creature::wear( Object* article, std::string& message, Object*& removed ) {
  int location = getWearloc( article->wearable() );

  // make sure we can wear it...
  if ( location == WEARLOC_ERROR ) {
    message.assign( "You can't wear " ).append( article->identifiers().shortname() ).append( "{x." );
    return false;
  } else if ( article->level() > level() ) {
    message.assign( "You're not powerful enough to wear " ).append( article->identifiers().shortname() ).append( "{x yet." );
    return false;
  }

  // for single-slot items (head, float, etc)
  if ( isSingleWearLoc( article->wearable() ) ) {
    if ( worn( location ) ) {
      if ( !unwear( worn( location ), message ) ) {
        return false;
      }
    }
  // edge case to ensure that weapons are held in the primary hand by default
  // duplicate of the other "double-slot" items below
  } else if (location == WEARLOC_HOLD_L || location == WEARLOC_HOLD_R) {
    if (worn(hand()) == NULL) {
      location = hand();
    } else if (worn(off_hand()) == NULL) {
      location = off_hand();
    } else if ((unwear(removed = worn(hand()), message))) {
      location = hand();
    } else if ((unwear(removed = worn(off_hand()), message))) {
      location = off_hand();
    }
  // for other double-slot items (like ears, fingers, etc)
  } else {
    // check the first slot
    if ( worn( location ) == NULL ) { }
    // check the second slot
    else if ( worn( location+1 ) == NULL ) {
      ++location;
    }
    // try to force the first slot
    else if ( ( unwear( removed = worn( location ), message ) ) ) { }
    // try to force the second slot
    else if ( ( unwear( removed = worn( location+1 ), message ) ) ) {
      ++location;
    }
    // both slots are taken and can't be reclaimed at this time
    else {
      return false;
    }
  }

  // don the article...
  inventory().remove( article );
  equipment().add( article, location );
  setModifications( article );
  return true;
}

bool Creature::unwear( Object* article, std::string& message, bool force ) {
  if ( !force ) {
    if ( article->flags().test( OBJECT_NOREMOVE ) ) {
      message.assign( "You can't remove " ).append( article->identifiers().shortname().c_str() ).append( "{x." );
      return false;
    }
  }
  for ( std::map<int,Object*>::iterator it = equipment().objectMap().begin(); it != equipment().objectMap().end(); ++it ) {
    if ( it->second == article ) {
      equipment().remove( article );
      inventory().add( article );
      unsetModifications( article );
      return true;
    }
  }
  message.assign( "can't find it" );
  return false;
}

Object* Creature::worn( const int& location ) const {
  std::map<int,Object*>::const_iterator it;
  if ( ( it = equipment().objectMap().find( location ) ) != equipment().objectMap().end() ) {
    return it->second;
  }
  return NULL;
}

bool Creature::isSingleWearLoc( const unsigned short& object_weartype ) {
  switch ( object_weartype ) {
    case Object::Wearable_Head:
    case Object::Wearable_Face:
    case Object::Wearable_Neck:
    case Object::Wearable_Shoulders:
    case Object::Wearable_Arms:
    case Object::Wearable_Torso:
    case Object::Wearable_Hands:
    case Object::Wearable_Legs:
    case Object::Wearable_Feet:
      return true;
    default:
      return false;
  }
}

Object* Creature::primary(void) {
  return equipment().at(hand());
}

Object* Creature::secondary(void) {
  return equipment().at(off_hand());
}

std::string Creature::applyExperience( long e ) {
  std::string output;
  long gained = 0;
  if ( e > 0 ) {
    while ( e > 0 ) {
      gained = ( e >= (long)_tnl ) ? _tnl : e ;
      e -= gained;
      _tnl -= gained;
      _exp += gained;
      if ( _tnl == 0 ) {
        output.append( gainLevel() );
      }
    }
  } else {
    _exp += e;
    _tnl -= e;
  }
  return output;
}

std::string Creature::gainLevel( void ) {
  std::string output;
  _level += 1;
  _tnl = 1000;
  send("You gain a level!\n");
  return output;
}

bool Creature::lay( std::string& error, ObjFurniture* furniture ) {
  if ( isLaying() ) {
    error.assign( "You're already laying down." );
    return false;
  }
  if ( !isSitting() && !isStanding() ) {
    error.assign( "You can't lie down while " ).append( position().string() ).append( "." );
    return false;
  }
  if ( furniture ) {
    if ( furniture->add( this, FURN_LAY_ON ) ) {
      this->furniture( furniture );
      position().set( LAYING );
      return true;
    } else {
      error.assign( "You can't lay on that." );
      return false;
    }
  }
  position().set( LAYING );
  return true;
}

bool Creature::sit( std::string& error, ObjFurniture* furniture, bool on ) {
  EnumInt pos = on? FURN_SIT_ON : FURN_SIT_AT;
  if ( isSitting() ) {
    error.assign( "You're already sitting." );
    return false;
  }
  if ( !isLaying() && !isStanding() ) {
    error.assign( "You can't sit while " ).append( position().string() ).append( "." );
    return false;
  }
  if ( furniture ) {
    if ( furniture->add( this, pos ) ) {
      this->furniture( furniture );
      position().set( SITTING );
      return true;
    } else {
      if ( on ) {
        error.assign( "You can't sit on that." );
      } else {
        error.assign( "You can't sit there." );
      }
      return false;
    }
  }
  position().set( SITTING );
  return true;
}

bool Creature::stand( std::string& error ) {
  if ( isStanding() ) {
    error.assign( "You're already standing." );
    return false;
  }
  if ( isMounted() ) {
    error.assign( "You can't do that while " ).append( position().string() ).append( "." );
    return false;
  }
  if ( furniture() ) {
    furniture()->remove( this );
    furniture( NULL );
  }
  position().set( STANDING );
  return true;
}

unsigned short Creature::stringToAttribute( const std::string& name ) {
  if ( Regex::strPrefix( name, "health" ) ) {
    return ATTR_MAX_HEALTH;
  } else if ( Regex::strPrefix( name, "mana" ) ) {
    return ATTR_MAX_MANA;
  } else if ( Regex::strPrefix( name, "move" ) ) {
    return ATTR_MAX_MOVE;
  } else if ( Regex::strPrefix( name, "str" ) ) {
    return ATTR_STR;
  } else if ( Regex::strPrefix( name, "dex" ) ) {
    return ATTR_DEX;
  } else if ( Regex::strPrefix( name, "con" ) ) {
    return ATTR_CON;
  } else if ( Regex::strPrefix( name, "int" ) ) {
    return ATTR_INT;
  } else if ( Regex::strPrefix( name, "foc" ) ) {
    return ATTR_FOC;
  } else if ( Regex::strPrefix( name, "cre" ) ) {
    return ATTR_CRE;
  } else if ( Regex::strPrefix( name, "cha" ) ) {
    return ATTR_CHA;
  } else if ( Regex::strPrefix( name, "luc" ) ) {
    return ATTR_LUC;
  } else if ( Regex::strPrefix( name, "armor" ) ) {
    return ATTR_ARMOR;
  } else if ( Regex::strPrefix( name, "bash" ) ) {
    return ATTR_BASH;
  } else if ( Regex::strPrefix( name, "slash" ) ) {
    return ATTR_SLASH;
  } else if ( Regex::strPrefix( name, "pierce" ) ) {
    return ATTR_PIERCE;
  } else if ( Regex::strPrefix( name, "exotic" ) ) {
    return ATTR_EXOTIC;
  } else {
    return ATTR_END;
  }
}

const char* Creature::attributeToString( const unsigned short& index ) {
  switch ( index ) {
    case ATTR_MAX_HEALTH: return "health";
    case ATTR_MAX_MANA:   return "mana";
    case ATTR_MAX_MOVE:   return "move";
    case ATTR_STR:        return "str";
    case ATTR_DEX:        return "dex";
    case ATTR_CON:        return "con";
    case ATTR_INT:        return "int";
    case ATTR_FOC:        return "foc";
    case ATTR_CRE:        return "cre";
    case ATTR_CHA:        return "cha";
    case ATTR_LUC:        return "luc";
    case ATTR_ARMOR:      return "armor";
    case ATTR_BASH:       return "bash";
    case ATTR_SLASH:      return "slash";
    case ATTR_PIERCE:     return "pierce";
    case ATTR_EXOTIC:     return "exotic";
    default:              return "[error]";
  }
}

std::string Creature::listAttributes( void ) {
  std::string output;
  for ( unsigned i = ATTR_BEGIN; i < ATTR_END; ++i ) {
    output.append( attributeToString( i ) ).append( 1, ' ' );
  }
  if ( !output.empty() ) {
    output.resize( output.size()-1 );
  }
  return output;
}

void Creature::setModifications( Object* object ) {
  for ( std::list<Modifier*>::const_iterator it = object->modifiers().begin(); it != object->modifiers().end(); ++it ) {
    modify( *it );
  }
  return;
}

void Creature::unsetModifications( Object* object ) {
  for ( std::list<Modifier*>::const_iterator it = object->modifiers().begin(); it != object->modifiers().end(); ++it ) {
    unmodify( *it );
  }
  return;
}

void Creature::modify( Modifier* modifier ) {
  doModification( modifier->attribute(), modifier->magnitude() );
  modifiers().push_back( modifier );
  return;
}

void Creature::unmodify( Modifier* modifier ) {
  doModification( modifier->attribute(), -(modifier->magnitude()) );
  modifiers().remove( modifier );
  return;
}

void Creature::doModification( const unsigned short& attribute, const int& magnitude ) {
  switch ( attribute ) {
    case ATTR_MAX_HEALTH: _maxHealth    += magnitude; break;
    case ATTR_MAX_MANA:   _maxMana      += magnitude; break;
    case ATTR_MAX_MOVE:   _maxMovement  += magnitude; break;
    case ATTR_STR:        _strength     += magnitude; break;
    case ATTR_DEX:        _dexterity    += magnitude; break;
    case ATTR_CON:        _constitution += magnitude; break;
    case ATTR_INT:        _intelligence += magnitude; break;
    case ATTR_FOC:        _focus        += magnitude; break;
    case ATTR_CRE:        _creativity   += magnitude; break;
    case ATTR_CHA:        _charisma     += magnitude; break;
    case ATTR_LUC:        _luck         += magnitude; break;
    case ATTR_ARMOR:      _armor        += magnitude; break;
    case ATTR_BASH:       _bash         += magnitude; break;
    case ATTR_SLASH:      _slash        += magnitude; break;
    case ATTR_PIERCE:     _pierce       += magnitude; break;
    case ATTR_EXOTIC:     _exotic       += magnitude; break;
    default: break;
  }
  return;
}

unsigned short Creature::canSee( Creature* target ) {
  // Check for immortal invisibility...
  if ( target->isAvatar() ) {
    if ( ((Avatar*)target)->adminFlags().test( ADMIN_INCOGNITO ) && target->level() > level() && ((Avatar*)target)->isConnected() ) {
      return SEE_NOTHING;
    }
  }
  return SEE_NAME;
}

std::string Creature::seeName( Creature* target, bool capitalize ) {
  if ( canSee( target ) == SEE_NAME ) {
    return target->identifiers().shortname();
  } else {
    return capitalize ? "Someone" : "someone";
  }
}

const char* Creature::seeReflexivePronoun( Creature* target, bool capitalize ) {
  switch ( canSee( target ) ) {
    case SEE_NAME:
      switch ( target->gender().number() ) {
        case MALE:    return capitalize ? "Himself" : "himself";
        case FEMALE:  return capitalize ? "Herself" : "herself";
        default:      return capitalize ? "Itself"  : "itself";
      }
    default:
      return capitalize ? "Themself" : "themself";
  }
}

const char* Creature::seeObjectPronoun( Creature* target, bool capitalize ) {
  switch ( canSee( target ) ) {
    case SEE_NAME:
      switch ( target->gender().number() ) {
        case MALE:    return capitalize ? "Him" : "him";
        case FEMALE:  return capitalize ? "Her" : "her";
        default:      return capitalize ? "It"  : "it";
      }
    default:
      return capitalize ? "Someone" : "someone";
  }
}

const char* Creature::seePosessivePronoun( Creature* target, bool capitalize ) {
  switch ( canSee( target ) ) {
    case SEE_NAME:
      switch ( target->gender().number() ) {
        case MALE:    return capitalize ? "His" : "his";
        case FEMALE:  return capitalize ? "Her" : "her";
        default:      return capitalize ? "Its" : "its";
      }
    default:
      return capitalize ? "Their" : "their";
  }
}

unsigned short Creature::canSee( Object* target ) {
  return SEE_NAME;
}

std::string Creature::seeName( Object* target, bool capitalize ) {
  if ( canSee( target ) == SEE_NAME ) {
    return target->identifiers().shortname();
  } else {
    return capitalize ? "Something" : "something";
  }
}

bool Creature::canAlter( Creature* target ) {
  return ( target->level() < level()-2 );
}

bool Creature::canMove( const unsigned short& direction, std::string& message ) {
  Exit* exit = NULL;

  if ( ( exit = room()->exit( direction ) ) == NULL ) {
    message.append( "You cannot go that way." );
    return false;
  }

  if ( exit->flags().test( EXIT_CLOSED ) ) {
    message.append( "That door is closed." );
    return false;
  }

  return true;
}

void Creature::move( const unsigned short& direction ) {
  // For standard movement...
  Exit* exit = room()->exit(direction);
  Room* from = room();
  Room* to = exit->target();
  CmdLook look;
  std::vector<std::string> look_args(1);
  // For group movement...
  Creature* member = NULL;
  std::string message;

  // Make the switch...
  from->remove(this);
  to->add(this);
  room(to);

  // Send some output...
  send("You leave %s.\n", Exit::name(direction));
  switch (direction) {
    case NORTH:
    case EAST:
    case SOUTH:
    case WEST:
      from->send_cond("$p leaves to the $e.", this, exit);
      to->send_cond("$p has arrived from the $n.", this, exit);
      break;
    case UP:
      from->send_cond("$p leaves up.", this, exit);
      to->send_cond("$p has arrived from below.", this, exit);
      break;
    case DOWN:
      from->send_cond("$p leaves down.", this, exit);
      to->send_cond("$p has arrived from above.", this, exit);
      break;
    default:
      break;
  }
  look.execute( this, look_args );

  // If this is the leader of a non-empty Group, invoke movement in the other
  // members as well.
  if (group()->leader() == this && group()->members().size() > 1) {
    for (std::set<Creature*>::iterator it = group()->members().begin(); it != group()->members().end(); ++it) {
      member = *it;
      if (member == this) {
        continue;
      }
      if (member->canMove(direction, message)) {
        member->move(direction);
      } else {
        send("%s could not follow you here.\n", member->identifiers().shortname().c_str());
        member->send("You cannot follow %s %s.\n", identifiers().shortname().c_str(), Exit::name(direction));
        member->send(message);
      }
    }
  }

  return;
}

bool Creature::inCombat(void) {
  return group()->in_combat();
}

bool Creature::attack(Job* job) {
  Creature* target = NULL;
  // No-op if not in combat.
  if (!inCombat()) return false;
  // Aquire a target.
  target = aquireTarget();
  // Make the strike.
  strike(target);
  // Schedule next attack.
  scheduleAttack();
  // Fully involve both groups.
  escalate(target->group());
  // Must return bool per the Job interface.
  return true;
}

Creature* Creature::aquireTarget(void) {
  return *(group()->opponents().begin());
}

void Creature::strike(Creature* target) {
  unsigned int damage = 0;
  std::string weapon_noun;
  std::string damage_verb;
  Object* object = NULL;

  // Initial damage calculation (based on the attackers offense).
  damage = rand() % 250;
  // Adjust damage (based on the targets defense).
  damage -= rand() % (damage - 1);

  // Tell the world.
  weapon_noun = "punch";
  object = primary();
  if (object && object->isWeapon()) weapon_noun = object->weapon()->verb().string();
  damage_verb = Display::formatDamage(damage);
  send("Your %s %s %s!\n", weapon_noun.c_str(), damage_verb.c_str(), target->identifiers().shortname().c_str());
  target->send("%s's %s %s you!\n", identifiers().shortname().c_str(), weapon_noun.c_str(), damage_verb.c_str());
  room()->send_cond("$p's punch $s $C!", this, (void*)damage_verb.c_str(), target, TO_NOTVICT, true);

  // Deal the pain (performed last to ensure that DEAD notification appear in the proper order.
  target->takeDamage(damage);

  return;
}

void Creature::assessCombat(void) {

  return;
}

/*
 * Ensure that each member of the target Group is tracking this Creature as an opponent. Any new
 * adversaries should schedule their first attack.
 */
void Creature::escalate(Group* group) {
  group->add_opponent(this);
  for (std::set<Creature*>::iterator iter = group->members().begin(); iter != group->members().end(); ++iter) {
    if (!(*iter)->nextAttack()) (*iter)->scheduleAttack();
  }
  return;
}

void Creature::scheduleAttack(void) {
  _next_attack = time(NULL) + rand() % 5;
  World::Instance().schedule()->add(new Job(_next_attack, this, &Creature::attack));
  return;
}

void Creature::takeDamage(int damage) {
  health(health() - damage);
  if (health() < 2) {
    health(1);
    send("\n\nYou are {RDEAD{x!!!\n\n");
    room()->send_cond("\n\n$p is {RDEAD{x!!!\n\n", this, NULL, NULL, TO_NOTVICT);
  }
  return;
}
