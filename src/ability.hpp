
#ifndef H_SYMPHONY_ABILITY
#define H_SYMPHONY_ABILITY

#include <set>
#include <string>
#include "enum-table.hpp"

class Being;
class Item;

///////////////////////////////////////////////////////////////////////////////
// BASE CLASS
///////////////////////////////////////////////////////////////////////////////
class Ability {
  public:

    // Internal defaults.
    static const unsigned DEFAULT_LEVEL        = 1;
    static const unsigned DEFAULT_TRAINS       = 1;
    static const unsigned DEFAULT_DIFFICULTY   = 3;
    static const unsigned DEFAULT_STAMINA      = 1;
    static const unsigned DEFAULT_MANA         = 0;
    static const unsigned DEFAULT_SUCCESS_RATE = 100;

    // Spell categories.
    static const unsigned CHARM           = 1;
    static const unsigned ENCHANTMENT     = 2;
    static const unsigned HEX             = 3;
    static const unsigned CURSE           = 4;
    static const unsigned CURE            = 5;
    static const unsigned HARM            = 6;
    static const unsigned CONJURING       = 7;
    static const unsigned TRANSFIGURATION = 8;

    // Targets.
    // For spells, these are enforced in the `cast` command.
    // For skills, these are enforced in Ability::execute().
    static const unsigned TARGET_NONE  = 1 << 0;
    static const unsigned TARGET_ANY   = 1 << 1;
    static const unsigned TARGET_BEING = 1 << 2;
    static const unsigned TARGET_ITEM  = 1 << 3;

    // Used for setting default targets.
    static const unsigned NO_DEFAULT       = 0;
    static const unsigned DEFAULT_SELF     = 1;
    static const unsigned DEFAULT_OPPONENT = 2;

    enum AbilityType {
      SKILL,
      SPELL
    };

    Ability(void) { }
    virtual ~Ability(void) { }

    std::set<Ability*>&         dependencies(void)        { return _dependencies; }
    const std::set<Ability*>&   dependencies(void) const  { return _dependencies; }
    std::set<Ability*>&         dependents(void)          { return _dependents; }
    const std::set<Ability*>&   dependents(void) const    { return _dependents; }

    void                  add_dependency(Ability* ability);
    void                  add_dependent(Ability* ability);
    bool                  has_dependencies(void) const;
    bool                  has_dependents(void) const;

    std::set<std::string> dependency_names(void) const;

    bool                  is_root(void) const;
    bool                  is_leaf(void) const;

    void                  type(AbilityType type)              { _type = type; }
    AbilityType           type(void) const                    { return _type; }
    void                  name(std::string name)              { _name = name; }
    std::string           name(void) const                    { return _name; }
    void                  level(unsigned level)               { _level = level; }
    unsigned              level(void) const                   { return _level; }
    void                  trains(unsigned trains)             { _trains = trains; }
    unsigned              trains(void) const                  { return _trains; }
    void                  difficulty(unsigned difficulty)     { _difficulty = difficulty; }
    unsigned              difficulty(void) const              { return _difficulty; }
    void                  stamina(unsigned stamina)           { _stamina = stamina; }
    unsigned              stamina(void) const                 { return _stamina; }
    void                  mana(unsigned mana)                 { _mana = mana; }
    unsigned              mana(void) const                    { return _mana; }
    void                  successRate(unsigned success_rate)  { _success_rate = success_rate; }
    unsigned              successRate(void) const             { return _success_rate; }
    void                  category(unsigned category)         { _category = category; }
    unsigned              category(void) const                { return _category; }
    void                  targeting(unsigned targeting)       { _targeting = targeting; }
    unsigned              targeting(void) const               { return _targeting; }

    bool                  is_skill(void) const;
    bool                  is_spell(void) const;

    bool                  is_charm(void) const            { return _category == CHARM; }
    bool                  is_enchantment(void) const      { return _category == ENCHANTMENT; }
    bool                  is_hex(void) const              { return _category == HEX; }
    bool                  is_curse(void) const            { return _category == CURSE; }
    bool                  is_cure(void) const             { return _category == CURE; }
    bool                  is_harm(void) const             { return _category == HARM; }
    bool                  is_conjuring(void) const        { return _category == CONJURING; }
    bool                  is_transfiguration(void) const  { return _category == TRANSFIGURATION; }

    bool                  target_none(void) const         { return (_targeting & TARGET_NONE) == TARGET_NONE; }
    bool                  target_any(void) const          { return (_targeting & TARGET_ANY) == TARGET_ANY; }
    bool                  target_being(void) const        { return (_targeting & TARGET_BEING) == TARGET_BEING; }
    bool                  target_item(void) const         { return (_targeting & TARGET_ITEM) == TARGET_ITEM; }
    unsigned              default_target(void) const;

    // invoke() is a high-level wrapper around execute() which automatically
    // performs resource verification/deduction as well as improvement logic.
    bool                  invoke(Being* being, Being* target = nullptr, Item* item = nullptr);
    // execute() with target/item is a low-level wrapper around execute()
    // which exists to set the _target_being and _target_item members when
    // references to "external" objects is required for an Ability (such as
    // enchanting a weapon or backstabbing an NPC).
    bool                  execute(Being* being, Being* target, Item* item);
    // execute() is to be implemented by each concrete Ability and should
    // contain the unique business logic for same.
    virtual bool          execute(Being* being) const = 0;

    unsigned              trainingFactor(void) const;
    static std::string    masteryToString(unsigned mastery);

  private:

    AbilityType           _type;
    std::string           _name;
    unsigned              _level;         // minimum level at which Ability can be learned
    unsigned              _trains;        // number of training points required to learn
    unsigned              _difficulty;    // relative difficulty for increasing mastery (1-5)
    unsigned              _stamina;       // amount of stamina required for the Ability
    unsigned              _mana;          // amount of mana required for the Ability
    unsigned              _success_rate;  // how likely the Ability is to execute when invoked
    unsigned              _category;      // broadly speaking, the type of Ability
    unsigned              _targeting;     // what can be targeted with this Ability?
    std::set<Ability*>    _dependencies;
    std::set<Ability*>    _dependents;

  protected:
    // These will transiently hold references to some target object during execute()
    Being*                _target_being;
    Item*                 _target_item;

};

///////////////////////////////////////////////////////////////////////////////
// SKILL
///////////////////////////////////////////////////////////////////////////////
#define DEF_SKILL(NAME,CLASS,TARGETING)               \
class CLASS: public Ability {                         \
  public:                                             \
  CLASS(                                              \
    unsigned level_        = DEFAULT_LEVEL,           \
    unsigned trains_       = DEFAULT_TRAINS,          \
    unsigned difficulty_   = DEFAULT_DIFFICULTY,      \
    unsigned stamina_      = DEFAULT_STAMINA,         \
    unsigned success_rate_ = DEFAULT_SUCCESS_RATE,    \
    unsigned mana_         = DEFAULT_MANA)            \
    {                                                 \
      type(SKILL);                                    \
      name(NAME);                                     \
      targeting(TARGETING);                           \
      level(level_);                                  \
      trains(trains_);                                \
      difficulty(difficulty_);                        \
      stamina(stamina_);                              \
      mana(mana_);                                    \
      successRate(success_rate_);                     \
      category(0);                                    \
      return;                                         \
    }                                                 \
    virtual ~CLASS(void) { return; }                  \
    virtual bool execute(Being* being) const;         \
};                                                    \

///////////////////////////////////////////////////////////////////////////////
// SPELL
///////////////////////////////////////////////////////////////////////////////
#define DEF_SPELL(NAME,CLASS,CATEGORY,TARGETING)      \
class CLASS: public Ability {                         \
  public:                                             \
  CLASS(                                              \
    unsigned level_      = DEFAULT_LEVEL,             \
    unsigned trains_     = DEFAULT_TRAINS,            \
    unsigned difficulty_ = DEFAULT_DIFFICULTY,        \
    unsigned mana_       = DEFAULT_MANA,              \
    unsigned stamina_    = DEFAULT_STAMINA)           \
    {                                                 \
      type(SPELL);                                    \
      name(NAME);                                     \
      level(level_);                                  \
      trains(trains_);                                \
      difficulty(difficulty_);                        \
      stamina(stamina_);                              \
      mana(mana_);                                    \
      category(CATEGORY);                             \
      targeting(TARGETING);                           \
      return;                                         \
    }                                                 \
    virtual ~CLASS(void) { return; }                  \
    virtual bool execute(Being* being) const;         \
};                                                    \

#endif // #ifndef H_SYMPHONY_ABILITY
