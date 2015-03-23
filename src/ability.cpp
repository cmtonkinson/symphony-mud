
#include "ability.h"
#include "being.h"
#include "item.h"
#include "os.h"
#include "world.h"

void Ability::add_dependency(Ability* ability) {
  _dependencies.insert(ability);
  ability->dependents().insert(this);
  return;
}

void Ability::add_dependent(Ability* ability) {
  _dependents.insert(ability);
  ability->dependencies().insert(this);
  return;
}

bool Ability::has_dependencies(void) const {
  return !_dependencies.empty();
}

bool Ability::has_dependents(void) const {
  return !_dependents.empty();
}

std::set<std::string> Ability::dependency_names(void) const {
  std::set<Ability*>::const_iterator iter;
  std::set<std::string> prereqs;
  for (iter = dependencies().begin(); iter != dependencies().end(); ++iter) {
    prereqs.insert((*iter)->name());
  }
  return prereqs;
}

bool Ability::is_root(void) const {
  return !has_dependencies();
}

bool Ability::is_leaf(void) const {
  return !has_dependents();
}

bool Ability::is_skill(void) const {
  return _type == SKILL;
}

bool Ability::is_spell(void) const {
  return _type == SPELL;
}

bool Ability::invoke(Being* being) {
  bool status = false;
  // A positive value for mana or stamina indicates that the Ability requires a static amount and
  // that it can/should be deducted from the Beings resources automatically. A non-positive value
  // indicates that either no resource is required for the Ability, or that execute() is going to
  // perform its own checks and deductions dynamically - in either case, invoke() should not perform
  // any automatic checks or deductions.

  // Because some Abilities require both stamina and mana, we need to verify that both are in
  // sufficient quantity before we begin deductions.
  if (being->stamina() > 0 && !being->check_mana(mana())) return false;
  if (being->mana() > 0 && !being->check_stamina(stamina())) return false;

  if (stamina() > 0) being->deplete_stamina(stamina());
  if (mana() > 0) being->deplete_mana(mana());

  // There is a random chance any non-mastered ability will be a dud.
  if (Math::percent_chance(100 - being->mastery(this))) {
    being->send("You get confused and your '{m%s{x' fails.\n", name().c_str());
    // Randomly improve upon the ability. The more advanced you are, the more instructive failure is.
    if (Math::percent_chance(being->mastery(this))) {
      // The higher the difficulty, the less your chances of improving.
      if (rand() % difficulty() == 0) {
        being->improve(this, false);
      }
    }
    return false;
  }

  // Capture the return value of the Ability.
  status = execute(being);

  // Randomly improve upon the ability. The more novice you are, the more instructive success is.
  if (Math::percent_chance(100 - being->mastery(this))) {
    // The higher the difficulty, the less your chances of improving.
    if (rand() % difficulty() == 0) {
      being->improve(this, true);
    }
  }

  // Return the execution status of the Ability.
  return status;
}

bool Ability::execute(Being* being, Being* target, Item* item) {
  bool status = false;
  // Set the "parameters"
  _target_being = target;
  _target_item  = item;
  // execute the concrete Ability
  status = execute(being);
  // Clear the "parameters"
  _target_being = nullptr;
  _target_item  = nullptr;
  // Return execution status
  return status;
}

// This method is used in the "train" command to determine how much closer to mastery 1 training
// point will get your Ability.
unsigned Ability::trainingFactor(void) const {
  switch (difficulty()) {
    case 1: return 20;
    case 2: return 10;
    case 3: return 5;
    case 4: return 3;
    case 5: return 2;
    default:
      ERROR_(0, "invalid difficulty for %s", name().c_str())
      return 20;
  }
}

std::string Ability::masteryToString(unsigned mastery) {
  if (mastery == 100) return "{Wmasterful{x";
  else if (mastery > 95) return "{Gsuperb{x";
  else if (mastery > 85) return "{Cexcellent{x";
  else if (mastery > 75) return "{Mvery good{x";
  else if (mastery > 60) return "{ggood{x";
  else if (mastery > 41) return "{cokay{x";
  else if (mastery > 31) return "{minferior{x";
  else if (mastery > 15) return "{rpoor{x";
  else return "{wbad{x";
}
