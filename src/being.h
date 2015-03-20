
#ifndef H_SYMPHONY_BEING
#define H_SYMPHONY_BEING

#include <list>
#include <map>
#include <set>
#include <vector>
#include "enumTable.h"
#include "equipmentContainer.h"
#include "estring.h"
#include "flagTable.h"
#include "group.h"
#include "identifiers.h"
#include "inventoryContainer.h"
#include "job.h"
#include "klass.h"
#include "modifier.h"
#include "item.h"

class Zone;
class Ability;
class IOHandler;
class ObjFurniture;
class Room;
class Mob;
class SocialCommand;
class Group;

class Being {
  public:

    // Wear Locations...
    static const unsigned WEARLOC_ERROR     =  0;
    static const unsigned WEARLOC_HEAD      =  1;
    static const unsigned WEARLOC_EAR_L     =  2;
    static const unsigned WEARLOC_EAR_R     =  3;
    static const unsigned WEARLOC_FACE      =  4;
    static const unsigned WEARLOC_NECK      =  5;
    static const unsigned WEARLOC_SHOULDERS =  6;
    static const unsigned WEARLOC_ARMS      =  7;
    static const unsigned WEARLOC_TORSO     =  8;
    static const unsigned WEARLOC_FOREARM_L =  9;
    static const unsigned WEARLOC_FOREARM_R = 10;
    static const unsigned WEARLOC_WRIST_L   = 11;
    static const unsigned WEARLOC_WRIST_R   = 12;
    static const unsigned WEARLOC_HANDS     = 13;
    static const unsigned WEARLOC_HOLD_L    = 14;
    static const unsigned WEARLOC_HOLD_R    = 15;
    static const unsigned WEARLOC_FINGER_L  = 16;
    static const unsigned WEARLOC_FINGER_R  = 17;
    static const unsigned WEARLOC_WAIST_1   = 18;
    static const unsigned WEARLOC_WAIST_2   = 19;
    static const unsigned WEARLOC_LEGS      = 20;
    static const unsigned WEARLOC_KNEE_L    = 21;
    static const unsigned WEARLOC_KNEE_R    = 22;
    static const unsigned WEARLOC_SHIN_L    = 23;
    static const unsigned WEARLOC_SHIN_R    = 24;
    static const unsigned WEARLOC_ANKLE_L   = 25;
    static const unsigned WEARLOC_ANKLE_R   = 26;
    static const unsigned WEARLOC_FEET      = 27;
    static const unsigned WEARLOC_END       = 28; // not a valid wearloc; just makes life easier

    // Levels...
    static const unsigned NEWBIE     = 10;
    static const unsigned HERO       = 100;
    static const unsigned LORD       = (HERO+2);
    static const unsigned DUKE       = (HERO+5);
    static const unsigned DEMIGOD    = (HERO+15);
    static const unsigned GOD        = (HERO+20);
    static const unsigned CREATOR    = (HERO+25);

    /* Map integers in a range to attributes
     * (indexing must be contiguous, starting from 1)
     */
    static const unsigned ATTR_BEGIN      =  1;
    static const unsigned ATTR_MAX_HEALTH =  1;
    static const unsigned ATTR_MAX_MANA   =  2;
    static const unsigned ATTR_MAX_MOVE   =  3;
    static const unsigned ATTR_STR        =  4;
    static const unsigned ATTR_DEX        =  5;
    static const unsigned ATTR_CON        =  6;
    static const unsigned ATTR_INT        =  7;
    static const unsigned ATTR_FOC        =  8;
    static const unsigned ATTR_CRE        =  9;
    static const unsigned ATTR_CHA        = 10;
    static const unsigned ATTR_LUC        = 11;
    static const unsigned ATTR_ARMOR      = 12;
    static const unsigned ATTR_BASH       = 13;
    static const unsigned ATTR_SLASH      = 14;
    static const unsigned ATTR_PIERCE     = 15;
    static const unsigned ATTR_EXOTIC     = 16;
    static const unsigned ATTR_END        = 17;

    // Visibility...
    static const unsigned SEE_NOTHING   = 0;
    static const unsigned SEE_SOMETHING = 1;
    static const unsigned SEE_NAME      = 2;

    // Actions...
    static const unsigned ACT_NONE  = 0;
    static const unsigned ACT_SLEEP = 1;
    static const unsigned ACT_FIGHT = 2;

    // Leveling...
    static const unsigned BASE_EXP    = 0;
    static const unsigned BASE_TNL    = 100;
    static const unsigned TARGET_TNL  = 100000;
    static const unsigned BASE_TRAINS = 5;

    static const unsigned BASE_HEALTH = 100;
    static const unsigned BASE_MANA   = 100;
    static const unsigned MAX_STAMINA = 100;

    static const unsigned MIN_HEALTH_GAIN = 3;
    static const unsigned MIN_MANA_GAIN   = 3;

    static const unsigned ALTERABILITY_LEVEL_DIFFERENCE = 5;

    // constructors...
    Being(void);
    Being(const Being& ref);
    virtual ~Being(void);

    // public accessors...
    std::vector<IOHandler*>&    IOhandlers(void)                                                { return _IOhandlers; }
    void                        ID(const unsigned long& ID)                                     { _ID = ID; }
    unsigned long               ID(void) const                                                  { return _ID; }
    virtual void                room(Room* room)                                                { _room = room; }
    virtual Room*               room(void) const                                                { return _room; }
    emap&                       values(void)                                                    { return _values; }
    const emap&                 values(void) const                                              { return _values; }
    /* wear() assesses weather or not the being can don the particular equipment given.  If it can,
     * then the operation is performed and true is returned.  If the being can't wear the item,
     * then message is appropriately set and false is returned.
     */
    bool                        wear(Item* article, std::string& message, Item*& removed);
    // `remove(Item*)` is already implemented via `Container`, so we call equipment removal `unwear()`
    bool                        unwear(Item* article, std::string& message, bool force = false);
    Item*                     worn(const int& location) const;
    static bool                 isSingleWearLoc(const unsigned short& item_weartype);
    Identifiers&                identifiers(void)                                               { return _identifiers; }
    const Identifiers&          identifiers(void) const                                         { return _identifiers; }
    InventoryContainer&         inventory(void)                                                 { return _inventory; }
    const InventoryContainer&   inventory(void) const                                           { return _inventory; }
    EquipmentContainer&         equipment(void)                                                 { return _equipment; }
    const EquipmentContainer&   equipment(void) const                                           { return _equipment; }
    std::list<Modifier*>&       modifiers(void)                                                 { return _modifiers; }
    const std::list<Modifier*>& modifiers(void) const                                           { return _modifiers; }
    void                        furniture(ObjFurniture* furniture)                              { _furniture = furniture; }
    ObjFurniture*               furniture(void) const                                           { return _furniture; }

    Position&         position(void)        { return _position; }
    const Position&   position(void) const  { return _position; }
    Action&           action(void)          { return _action; }
    const Action&     action(void) const    { return _action; }

    void              group(Group* group)     { _group = group; }
    Group*            group(void)             { return _group; }

    Gender&           gender(void)          { return _gender; }
    const Gender&     gender(void) const    { return _gender; }
    Race&             race(void)            { return _race; }
    const Race&       race(void) const      { return _race; }
    PClass&           pClass(void)          { return _pClass; }
    const PClass&     pClass(void) const    { return _pClass; }
    Klass*            klass(void) const;

    // Level
    void                level(unsigned level)                           { _level = level; }
    unsigned            level(void) const                               { return _level; }
    void                exp(unsigned exp)                               { _exp = exp; }
    unsigned            exp(void) const                                 { return _exp; }
    void                tnl(unsigned tnl)                               { _tnl = tnl; }
    unsigned            tnl(void) const                                 { return _tnl; }
    // Health
    void                health(int health)                              { _health = ((health <= maxHealth()) ? health : maxHealth()); }
    int                 health(void) const                              { return _health; }
    void                maxHealth(int maxHealth)                        { _maxHealth = maxHealth; }
    int                 maxHealth(void) const                           { return _maxHealth; }
    void                mana(int mana)                                  { _mana = ((mana <= maxMana()) ? mana : maxMana()); }
    int                 mana(void) const                                { return _mana; }
    void                maxMana(int maxMana)                            { _maxMana = maxMana; }
    int                 maxMana(void) const                             { return _maxMana; }
    void                stamina(unsigned stamina)                       { _stamina = (stamina <= MAX_STAMINA ? stamina : MAX_STAMINA); }
    unsigned            stamina(void) const                             { return _stamina; }
    // Stats
    void                strength(unsigned short strength)               { _strength = ((strength <= maxStrength()) ? strength : maxStrength()); }
    unsigned short      strength(void) const                            { return _strength; }
    void                maxStrength(unsigned short maxStrength)         { _maxStrength = maxStrength; }
    unsigned short      maxStrength(void) const                         { return _maxStrength; }
    void                dexterity(unsigned short dexterity)             { _dexterity = ((dexterity <= maxDexterity()) ? dexterity : maxDexterity()); }
    unsigned short      dexterity(void) const                           { return _dexterity; }
    void                maxDexterity(unsigned short maxDexterity)       { _maxDexterity = maxDexterity; }
    unsigned short      maxDexterity(void) const                        { return _maxDexterity; }
    void                constitution(unsigned short constitution)       { _constitution = ((constitution <= maxConstitution()) ? constitution : maxConstitution()); }
    unsigned short      constitution(void) const                        { return _constitution; }
    void                maxConstitution(unsigned short maxConstitution) { _maxConstitution = maxConstitution; }
    unsigned short      maxConstitution(void) const                     { return _maxConstitution; }
    void                intelligence(unsigned short intelligence)       { _intelligence = ((intelligence <= maxIntelligence()) ? intelligence : maxIntelligence()); }
    unsigned short      intelligence(void) const                        { return _intelligence;}
    void                maxIntelligence(unsigned short maxIntelligence) { _maxIntelligence = maxIntelligence; }
    unsigned short      maxIntelligence(void) const                     { return _maxIntelligence;}
    void                focus(unsigned short focus)                     { _focus = ((focus <= maxFocus()) ? focus : maxFocus()); }
    unsigned short      focus(void) const                               { return _focus;}
    void                maxFocus(unsigned short maxFocus)               { _maxFocus = maxFocus; }
    unsigned short      maxFocus(void) const                            { return _maxFocus;}
    void                creativity(unsigned short creativity)           { _creativity = ((creativity <= maxCreativity()) ? creativity : maxCreativity()); }
    unsigned short      creativity(void) const                          { return _creativity;}
    void                maxCreativity(unsigned short maxCreativity)     { _maxCreativity = maxCreativity; }
    unsigned short      maxCreativity(void) const                       { return _maxCreativity;}
    void                charisma(unsigned short charisma)               { _charisma = ((charisma <= maxCharisma()) ? charisma : maxCharisma()); }
    unsigned short      charisma(void) const                            { return _charisma;}
    void                maxCharisma(unsigned short maxCharisma)         { _maxCharisma = maxCharisma; }
    unsigned short      maxCharisma(void) const                         { return _maxCharisma;}
    void                luck(unsigned short luck)                       { _luck = ((luck <= maxLuck()) ? luck : maxLuck()); }
    unsigned short      luck(void) const                                { return _luck;}
    void                maxLuck(unsigned short maxLuck)                 { _maxLuck = maxLuck; }
    unsigned short      maxLuck(void) const                             { return _maxLuck;}
    // Armor
    void                armor(int armor)                                { _armor = armor; }
    int                 armor(void) const                               { return _armor; }
    void                bash(int bash)                                  { _bash = bash; }
    int                 bash(void) const                                { return _bash; }
    void                slash(int slash)                                { _slash = slash; }
    int                 slash(void) const                               { return _slash; }
    void                pierce(int pierce)                              { _pierce = pierce; }
    int                 pierce(void) const                              { return _pierce; }
    void                exotic(int exotic)                              { _exotic = exotic; }
    int                 exotic(void) const                              { return _exotic; }
    // Misc
    void                trains(unsigned trains)                         { _trains = trains; }
    unsigned            trains(void) const                              { return _trains; }
    void                gold(unsigned gold)                             { _gold = gold; }
    unsigned            gold(void) const                                { return _gold; }
    void                silver(unsigned silver)                         { _silver = silver; }
    unsigned            silver(void) const                              { return _silver; }
    // combat
    std::set<Being*>&  opponents(void)                               { return _opponents; }

    // Public static methods...
    static unsigned short       stringToAttribute(const std::string& name);
    static const char*          attributeToString(const unsigned short& index);
    static std::string          listAttributes(void);

    // Public methods...
    const char*                 name(void) const                                                { return identifiers().shortname().c_str(); }
    virtual bool                isAvatar(void) const                                            { return false; }
    virtual bool                isMob(void) const                                               { return false; }
    bool                        isLaying(void) const                                            { return position().number() == LAYING; }
    bool                        isSitting(void) const                                           { return position().number() == SITTING; }
    bool                        isStanding(void) const                                          { return position().number() == STANDING; }
    bool                        isMounted(void) const                                           { return position().number() == MOUNTED; }
    bool                        isDead(void) const                                              { return action().number() == DEAD; }
    bool                        isSleeping(void) const                                          { return action().number() == SLEEPING; }
    bool                        isFighting(void) const                                          { return action().number() == FIGHTING; }
    bool                        isDwarf(void) const                                             { return race().number() == DWARF; }
    bool                        isElf(void) const                                               { return race().number() == ELF; }
    bool                        isHuman(void) const                                             { return race().number() == HUMAN; }
    bool                        isCleric(void) const                                            { return pClass().number() == CLERIC; }
    bool                        isMage(void) const                                              { return pClass().number() == MAGE; }
    bool                        isRogue(void) const                                             { return pClass().number() == ROGUE; }
    bool                        isWarrior(void) const                                           { return pClass().number() == WARRIOR; }
    unsigned short              hand(void) const                                                { return WEARLOC_HOLD_R; }
    unsigned short              off_hand(void) const                                            { return WEARLOC_HOLD_L; }
    bool                        lay(std::string& error, ObjFurniture* furniture = NULL);
    bool                        sit(std::string& error, ObjFurniture* furniture = NULL, bool on = false);
    bool                        stand(std::string& error);
    void                        setModifications(Item* item);
    void                        unsetModifications(Item* item);
    void                        modify(Modifier* modifier);
    void                        unmodify(Modifier* modifier);
    void                        doModification(const unsigned short& attribute, const int& magnitude);
    unsigned short              canSee(Being* target);
    std::string                 seeName(Being* target, bool capitalize = false);
    const char*                 seeReflexivePronoun(Being* target, bool capitalize = false); // e.g. "himself"
    const char*                 seeItemPronoun(Being* target, bool capitalize = false);    // e.g. "him"
    const char*                 seePosessivePronoun(Being* target, bool capitalize = false); // e.g. "his"
    unsigned short              canSee(Item* target);
    std::string                 seeName(Item* target, bool capitalize = false);
    bool                        canAlter(Being* target);
    bool                        canMove(const unsigned short& direction, std::string& message);
    bool                        move(const unsigned short& direction);
    IOHandler*                  IOhandler(void);
    void                        pushIOHandler(IOHandler* handler);
    void                        popIOHandler(void);
    void                        replaceIOHandler(IOHandler* handler);
    void                        handle(void);
    Being*                   findBeing(const std::string& name);
    Item*                     findItem(const std::string& query);
    unsigned short              getWearloc(const Item::Wearable& wearable) const;
    static const char*          wearLocName(const unsigned short& wearloc);
    Item*                     primary(void);
    Item*                     secondary(void);
    void                        naturalStatAdjustment(void);
    std::string                 serializeAbilities(void);
    void                        unserializeAbilities(std::string ser);

    // Combat...
    void          formGroup(void);
    void          ungroup(void);
    void          add_opponent(Being* opponent, bool reciprocal = true);
    void          remove_opponent(Being* opponent, bool reciprocal = true);
    void          scheduleAttack(void);
    bool          attack(Job* job);
    void          acquireTarget(void);
    void          strike(void);
    void          takeDamage(int damage, Being* damager = NULL);
    void          die(Being* killer = NULL);
    void          peace(void);
    void          awardExperience(unsigned experience);
    void          gainLevel(void);
    bool          autoassist(void) const;
    void          heal(void);
    virtual void  whatHappensWhenIDie(void) = 0;

    // Leveling, Gains, Abilities...
    void          resetStats(void);
    unsigned      targetHealth(void) const;
    unsigned      targetMana(void) const;
    unsigned      targetMovement(void) const;
    unsigned      targetTNL(void) const;

    AbilityTable&                 learned(void)                 { return _learned; }
    const AbilityTable&           learned(void) const           { return _learned; }
    std::map<Ability*,unsigned>&  abilityMastery(void)          { return _ability_mastery; }
    unsigned                      mastery(Ability* ability);
    void                          learn(Ability* ability, unsigned mastery);
    void                          improve(Ability* ability, bool success = true);
    std::set<Ability*>            available_abilities(void) const;
    bool                          can_learn(Ability* ability) const;
    Ability*                      find_spell(std::string name) const;
    bool                          intone(Ability* spell);
    bool                          deplete_mana(unsigned mana_, bool message = true);
    bool                          check_mana(unsigned mana_, bool message = true);
    bool                          exhausted(void) const;
    bool                          deplete_stamina(unsigned stamina_, bool message = true);
    bool                          check_stamina(unsigned stamina_, bool message = true);

    virtual void                send(std::string message)       { return; }
    virtual void                send(const char* format, ...)   { return; }

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
    // Level
    unsigned                    _level;
    unsigned                    _exp;
    unsigned                    _tnl;
    // Health
    int                         _health;
    int                         _maxHealth;
    int                         _mana;
    int                         _maxMana;
    int                         _stamina;
    // Stats
    unsigned short              _strength;
    unsigned short              _maxStrength;
    unsigned short              _dexterity;
    unsigned short              _maxDexterity;
    unsigned short              _constitution;
    unsigned short              _maxConstitution;
    unsigned short              _intelligence;
    unsigned short              _maxIntelligence;
    unsigned short              _focus;
    unsigned short              _maxFocus;
    unsigned short              _creativity;
    unsigned short              _maxCreativity;
    unsigned short              _charisma;
    unsigned short              _maxCharisma;
    unsigned short              _luck;
    unsigned short              _maxLuck;
    // Armor
    int                         _armor;
    int                         _bash;
    int                         _slash;
    int                         _pierce;
    int                         _exotic;
    // Misc
    unsigned                    _trains;
    unsigned                    _gold;
    unsigned                    _silver;
    // Combat
    std::set<Being*>         _opponents;
    Being*                   _target;
    Job*                        _next_attack;
    AbilityTable                _learned;
    std::map<Ability*,unsigned> _ability_mastery;

};

#endif // #ifndef H_SYMPHONY_BEING
