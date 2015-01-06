/*
 * $Id: creature.h 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
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

#ifndef H_SYMPHONY_CREATURE
#define H_SYMPHONY_CREATURE

#include <list>
#include <map>
#include <vector>
#include "enumTable.h"
#include "equipmentContainer.h"
#include "estring.h"
#include "flagTable.h"
#include "group.h"
#include "identifiers.h"
#include "inventoryContainer.h"
#include "job.h"
#include "modifier.h"
#include "object.h"

// Wear Locations...
#define WEARLOC_ERROR        0
#define WEARLOC_HEAD         1
#define WEARLOC_EAR_L        2
#define WEARLOC_EAR_R        3
#define WEARLOC_FACE         4
#define WEARLOC_NECK         5
#define WEARLOC_SHOULDERS    6
#define WEARLOC_ARMS         7
#define WEARLOC_TORSO        8
#define WEARLOC_FOREARM_L    9
#define WEARLOC_FOREARM_R   10
#define WEARLOC_WRIST_L     11
#define WEARLOC_WRIST_R     12
#define WEARLOC_HANDS       13
#define WEARLOC_HOLD_L      14
#define WEARLOC_HOLD_R      15
#define WEARLOC_FINGER_L    16
#define WEARLOC_FINGER_R    17
#define WEARLOC_WAIST_1     18
#define WEARLOC_WAIST_2     19
#define WEARLOC_LEGS        20
#define WEARLOC_KNEE_L      21
#define WEARLOC_KNEE_R      22
#define WEARLOC_SHIN_L      23
#define WEARLOC_SHIN_R      24
#define WEARLOC_ANKLE_L     25
#define WEARLOC_ANKLE_R     26
#define WEARLOC_FEET        27
#define WEARLOC_END         28 // not a valid wearloc; just makes life easier

// Levels...
#define LEVEL_NEWB  10
#define LEVEL_HERO  100
#define NEWBIE      LEVEL_NEWB
#define HERO        LEVEL_HERO
#define LORD        (HERO+2)
#define DUKE        (HERO+5)
#define DEMIGOD     (HERO+15)
#define GOD         (HERO+20)
#define CREATOR     (HERO+25)

/* Map integers in a range to attributes
 * (indexing must be contiguous, starting from 1)
 */
#define ATTR_BEGIN     1
#define ATTR_MAX_HP    1
#define ATTR_MAX_MANA  2
#define ATTR_MAX_MOVE  3
#define ATTR_STR       4
#define ATTR_DEX       5
#define ATTR_CON       6
#define ATTR_INT       7
#define ATTR_WIS       8
#define ATTR_CHA       9
#define ATTR_HR       10
#define ATTR_DR       11
#define ATTR_SAVES    12
#define ATTR_AC       13
#define ATTR_BASH     14
#define ATTR_SLASH    15
#define ATTR_PIERCE   16
#define ATTR_EXOTIC   17
#define ATTR_END      18

// Visibility...
#define SEE_NOTHING         0
#define SEE_SOMETHING       1
#define SEE_NAME            2

// Actions...
#define ACT_NONE    0
#define ACT_SLEEP   1
#define ACT_FIGHT   2

class Area;
class IOHandler;
class ObjFurniture;
class Room;
class Mob;
class SocialCommand;
class Group;

class Creature {
  public:
    // constructors...
    Creature( void );
    Creature( const Creature& ref );
    virtual ~Creature( void );

    // public accessors...
    std::vector<IOHandler*>&    IOhandlers( void )                                                { return _IOhandlers; }
    void                        ID( const unsigned long& ID )                                     { _ID = ID; }
    unsigned long               ID( void ) const                                                  { return _ID; }
    virtual void                room( Room* room )                                                { _room = room; }
    virtual Room*               room( void ) const                                                { return _room; }
    emap&                       values( void )                                                    { return _values; }
    const emap&                 values( void ) const                                              { return _values; }
    /* wear() assesses weather or not the creature can don the particular equipment given.  If it can,
     * then the operation is performed and true is returned.  If the creature can't wear the item,
     * then message is appropriately set and false is returned.
     */
    bool                        wear( Object* article, std::string& message, Object*& removed );
    // `remove(Object*)` is already implemented via `Container`, so we call equipment removal `unwear()`
    bool                        unwear( Object* article, std::string& message, bool force = false );
    Object*                     worn( const int& location ) const;
    static bool                 isSingleWearLoc( const unsigned short& object_weartype );
    std::string                 applyExperience( long e );
    std::string                 gainLevel( void );
    Identifiers&                identifiers( void )                                               { return _identifiers; }
    const Identifiers&          identifiers( void ) const                                         { return _identifiers; }
    InventoryContainer&         inventory( void )                                                 { return _inventory; }
    const InventoryContainer&   inventory( void ) const                                           { return _inventory; }
    EquipmentContainer&         equipment( void )                                                 { return _equipment; }
    const EquipmentContainer&   equipment( void ) const                                           { return _equipment; }
    std::list<Modifier*>&       modifiers( void )                                                 { return _modifiers; }
    const std::list<Modifier*>& modifiers( void ) const                                           { return _modifiers; }
    void                        furniture( ObjFurniture* furniture )                              { _furniture = furniture; }
    ObjFurniture*               furniture( void ) const                                           { return _furniture; }

    Position&         position( void )        { return _position; }
    const Position&   position( void ) const  { return _position; }
    Action&           action( void )          { return _action; }
    const Action&     action( void ) const    { return _action; }

    void              group(Group* group)     { _group = group; }
    Group*            group(void)             { return _group; }

    Gender&           gender( void )          { return _gender; }
    const Gender&     gender( void ) const    { return _gender; }
    Race&             race( void )            { return _race; }
    const Race&       race( void ) const      { return _race; }
    PClass&           pClass( void )          { return _pClass; }
    const PClass&     pClass( void ) const    { return _pClass; }

    void                        level( const unsigned short& level )                              { _level = level; }
    unsigned short              level( void ) const                                               { return _level; }
    void                        exp( const unsigned long& exp )                                   { _exp = exp; }
    unsigned long               exp( void ) const                                                 { return _exp; }
    void                        tnl( const unsigned long& tnl )                                   { _tnl = tnl; }
    unsigned long               tnl( void ) const                                                 { return _tnl; }
    void                        hp( const unsigned short& hp )                                    { _hp = hp; }
    unsigned short              hp( void ) const                                                  { return _hp; }
    void                        maxHp( const unsigned short& maxHp )                              { _maxHp = maxHp; }
    unsigned short              maxHp( void ) const                                               { return _maxHp; }
    void                        mana( const unsigned short& mana )                                { _mana = mana; }
    unsigned short              mana( void ) const                                                { return _mana; }
    void                        maxMana( const unsigned short& maxMana )                          { _maxMana = maxMana; }
    unsigned short              maxMana( void ) const                                             { return _maxMana; }
    void                        movement( const unsigned short& movement )                        { _movement = movement; }
    unsigned short              movement( void ) const                                            { return _movement; }
    void                        maxMovement( const unsigned short& maxMovement )                  { _maxMovement = maxMovement; }
    unsigned short              maxMovement( void ) const                                         { return _maxMovement; }
    void                        strength( const unsigned short& strength )                        { _strength = strength; }
    unsigned short              strength( void ) const                                            { return _strength; }
    void                        maxStrength( const unsigned short& maxStrength )                  { _maxStrength = maxStrength; }
    unsigned short              maxStrength( void ) const                                         { return _maxStrength; }
    void                        dexterity( const unsigned short& dexterity )                      { _dexterity = dexterity; }
    unsigned short              dexterity( void ) const                                           { return _dexterity; }
    void                        maxDexterity( const unsigned short& maxDexterity )                { _maxDexterity = maxDexterity; }
    unsigned short              maxDexterity( void ) const                                        { return _maxDexterity; }
    void                        constitution( const unsigned short& constitution )                { _constitution = constitution; }
    unsigned short              constitution( void ) const                                        { return _constitution; }
    void                        maxConstitution( const unsigned short& maxConstitution )          { _maxConstitution = maxConstitution; }
    unsigned short              maxConstitution( void ) const                                     { return _maxConstitution; }
    void                        intelligence( const unsigned short& intelligence )                { _intelligence = intelligence; }
    unsigned short              intelligence( void ) const                                        { return _intelligence; }
    void                        maxIntelligence( const unsigned short& maxIntelligence )          { _maxIntelligence = maxIntelligence; }
    unsigned short              maxIntelligence( void ) const                                     { return _maxIntelligence; }
    void                        wisdom( const unsigned short& wisdom )                            { _wisdom = wisdom; }
    unsigned short              wisdom( void ) const                                              { return _wisdom; }
    void                        maxWisdom( const unsigned short& maxWisdom )                      { _maxWisdom = maxWisdom; }
    unsigned short              maxWisdom( void ) const                                           { return _maxWisdom; }
    void                        charisma( const unsigned short& charisma )                        { _charisma = charisma; }
    unsigned short              charisma( void ) const                                            { return _charisma; }
    void                        maxCharisma( const unsigned short& maxCharisma )                  { _maxCharisma = maxCharisma; }
    unsigned short              maxCharisma( void ) const                                         { return _maxCharisma; }
    void                        hitroll( const unsigned short& hitroll )                          { _hitroll = hitroll; }
    unsigned short              hitroll( void ) const                                             { return _hitroll; }
    void                        damroll( const unsigned short& damroll )                          { _damroll = damroll; }
    unsigned short              damroll( void ) const                                             { return _damroll; }
    void                        saves( const signed short& saves )                                { _saves = saves; }
    signed short                saves( void ) const                                               { return _saves; }
    void                        ac( const signed short& ac )                                      { _ac = ac; }
    signed short                ac( void ) const                                                  { return _ac; }
    void                        bash( const signed short& bash )                                  { _bash = bash; }
    signed short                bash( void ) const                                                { return _bash; }
    void                        slash( const signed short& slash )                                { _slash = slash; }
    signed short                slash( void ) const                                               { return _slash; }
    void                        pierce( const signed short& pierce )                              { _pierce = pierce; }
    signed short                pierce( void ) const                                              { return _pierce; }
    void                        exotic( const signed short& exotic )                              { _exotic = exotic; }
    signed short                exotic( void ) const                                              { return _exotic; }
    void                        gold( const unsigned& gold )                                      { _gold = gold; }
    unsigned                    gold( void ) const                                                { return _gold; }
    void                        silver( const unsigned& silver )                                  { _silver = silver; }
    unsigned                    silver( void ) const                                              { return _silver; }
    // combat
    time_t                      nextAttack(void) const                                            { return _next_attack; }

    // Public static methods...
    static unsigned short       stringToAttribute( const std::string& name );
    static const char*          attributeToString( const unsigned short& index );
    static std::string          listAttributes( void );

    // Public methods...
    virtual bool                isAvatar( void ) const                                            { return false; }
    virtual bool                isMob( void ) const                                               { return false; }
    bool                        isLaying( void ) const                                            { return position().number() == LAYING; }
    bool                        isSitting( void ) const                                           { return position().number() == SITTING; }
    bool                        isStanding( void ) const                                          { return position().number() == STANDING; }
    bool                        isMounted( void ) const                                           { return position().number() == MOUNTED; }
    bool                        isSleeping( void ) const                                          { return action().number() == SLEEPING; }
    bool                        isFighting( void ) const                                          { return action().number() == FIGHTING; }
    bool                        isDwarf( void ) const                                             { return race().number() == DWARF; }
    bool                        isElf( void ) const                                               { return race().number() == ELF; }
    bool                        isHuman( void ) const                                             { return race().number() == HUMAN; }
    bool                        isCleric( void ) const                                            { return pClass().number() == CLERIC; }
    bool                        isMage( void ) const                                              { return pClass().number() == MAGE; }
    bool                        isRogue( void ) const                                             { return pClass().number() == ROGUE; }
    bool                        isWarrior( void ) const                                           { return pClass().number() == WARRIOR; }
    bool                        lay( std::string& error, ObjFurniture* furniture = NULL );
    bool                        sit( std::string& error, ObjFurniture* furniture = NULL, bool on = false );
    bool                        stand( std::string& error );
    void                        setModifications( Object* object );
    void                        unsetModifications( Object* object );
    void                        modify( Modifier* modifier );
    void                        unmodify( Modifier* modifier );
    void                        doModification( const unsigned short& attribute, const int& magnitude );
    unsigned short              canSee( Creature* target );
    std::string                 seeName( Creature* target, bool capitalize = false );
    const char*                 seeReflexivePronoun( Creature* target, bool capitalize = false ); // e.g. "himself"
    const char*                 seeObjectPronoun( Creature* target, bool capitalize = false );    // e.g. "him"
    const char*                 seePosessivePronoun( Creature* target, bool capitalize = false ); // e.g. "his"
    unsigned short              canSee( Object* target );
    std::string                 seeName( Object* target, bool capitalize = false );
    bool                        canAlter( Creature* target );
    bool                        canMove( const unsigned short& direction, std::string& message );
    void                        move( const unsigned short& direction );
    IOHandler*                  IOhandler( void );
    void                        pushIOHandler( IOHandler* handler );
    void                        popIOHandler( void );
    void                        replaceIOHandler( IOHandler* handler );
    void                        handle( void );
    Creature*                   findCreature( const std::string& name );
    Object*                     findObject( const std::string& query );
    unsigned short              getWearloc( const Object::Wearable& wearable ) const;
    static const char*          wearLocName( const unsigned short& wearloc );
    Object*                     primary(void);
    Object*                     secondary(void);

    // Combat...
    bool        inCombat(void);
    bool        attack(Job* job);
    Creature*   aquireTarget(void);
    void        strike(Creature* target);
    void        escalate(Group* group);
    void        scheduleAttack(void);

    // Pure virtual public methods...
    virtual bool                save( void )                                                      = 0;
    virtual bool                load( void )                                                      { return false; }
    virtual bool                destroy( void )                                                   = 0;

    virtual void                send( const std::string& message )                                { }
    virtual void                send( const char* format, ... )                                   { }

  private:
    // basics...
    std::vector<IOHandler*>     _IOhandlers;
    unsigned long               _ID;
    Room*                       _room;
    emap                        _values;
    Identifiers                 _identifiers;
    InventoryContainer          _inventory;
    EquipmentContainer          _equipment;
    std::list<Modifier*>        _modifiers;
    ObjFurniture*               _furniture;
    Position                    _position;
    Action                      _action;
    Group*                      _group;
    // identity...
    Gender                      _gender;
    Race                        _race;
    PClass                      _pClass;
    // stats...
    unsigned short              _level;
    unsigned long               _exp;
    unsigned long               _tnl;
    unsigned short              _hp;
    unsigned short              _maxHp;
    unsigned short              _mana;
    unsigned short              _maxMana;
    unsigned short              _movement;
    unsigned short              _maxMovement;
    unsigned short              _strength;
    unsigned short              _maxStrength;
    unsigned short              _dexterity;
    unsigned short              _maxDexterity;
    unsigned short              _constitution;
    unsigned short              _maxConstitution;
    unsigned short              _intelligence;
    unsigned short              _maxIntelligence;
    unsigned short              _wisdom;
    unsigned short              _maxWisdom;
    unsigned short              _charisma;
    unsigned short              _maxCharisma;
    unsigned short              _hitroll;
    unsigned short              _damroll;
    signed short                _saves;
    signed short                _ac;
    signed short                _bash;
    signed short                _slash;
    signed short                _pierce;
    signed short                _exotic;
    unsigned                    _gold;
    unsigned                    _silver;
    // combat...
    time_t                      _next_attack;

};

#endif // #ifndef H_SYMPHONY_CREATURE
