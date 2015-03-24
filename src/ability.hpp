
#ifndef H_SYMPHONY_ABILITY
#define H_SYMPHONY_ABILITY

#include <set>
#include <string>
#include "enum-table.h"

class Being;
class Item;

//////////////////////////////////////////// BASE CLASS ////////////////////////////////////////////
class Ability {
  public:

    static const unsigned DEFAULT_LEVEL      = 1;
    static const unsigned DEFAULT_TRAINS     = 1;
    static const unsigned DEFAULT_DIFFICULTY = 3;
    static const unsigned DEFAULT_STAMINA    = 0;
    static const unsigned DEFAULT_MANA       = 0;

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

    void                  type(AbilityType type)          { _type = type; }
    AbilityType           type(void) const                { return _type; }
    void                  name(std::string name)          { _name = name; }
    std::string           name(void) const                { return _name; }
    void                  level(unsigned level)           { _level = level; }
    unsigned              level(void) const               { return _level; }
    void                  trains(unsigned trains)         { _trains = trains; }
    unsigned              trains(void) const              { return _trains; }
    void                  difficulty(unsigned difficulty) { _difficulty = difficulty; }
    unsigned              difficulty(void) const          { return _difficulty; }
    void                  stamina(unsigned stamina)       { _stamina = stamina; }
    unsigned              stamina(void) const             { return _stamina; }
    void                  mana(unsigned mana)             { _mana = mana; }
    unsigned              mana(void) const                { return _mana; }

    bool                  is_skill(void) const;
    bool                  is_spell(void) const;

    bool                  invoke(Being* being);
    virtual bool          execute(Being* being) const = 0;
    bool                  execute(Being* being, Being* target, Item* item);
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
    std::set<Ability*>    _dependencies;
    std::set<Ability*>    _dependents;

  protected:
    // These will transiently hold references to some target object during execute()
    Being*                _target_being;
    Item*                 _target_item;

};

//////////////////////////////////////////// SKILL /////////////////////////////////////////////////
#define DEF_SKILL(NAME,CLASS)                         \
class CLASS: public Ability {                         \
  public:                                             \
  CLASS(                                              \
    unsigned level_      = DEFAULT_LEVEL,             \
    unsigned trains_     = DEFAULT_TRAINS,            \
    unsigned difficulty_ = DEFAULT_DIFFICULTY,        \
    unsigned stamina_    = DEFAULT_STAMINA,           \
    unsigned mana_       = DEFAULT_MANA)              \
    {                                                 \
      type(SKILL);                                    \
      name(NAME);                                     \
      level(level_);                                  \
      trains(trains_);                                \
      difficulty(difficulty_);                        \
      stamina(stamina_);                              \
      mana(mana_);                                    \
      return;                                         \
    }                                                 \
    virtual ~CLASS(void) { return; }                  \
    virtual bool execute(Being* being) const;         \
};                                                    \

//////////////////////////////////////////// SPELL /////////////////////////////////////////////////
#define DEF_SPELL(NAME,CLASS)                         \
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
      return;                                         \
    }                                                 \
    virtual ~CLASS(void) { return; }                  \
    virtual bool execute(Being* being) const;         \
};                                                    \

#endif // #ifndef H_SYMPHONY_ABILITY
