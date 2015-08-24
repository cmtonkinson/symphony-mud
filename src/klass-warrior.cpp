
#include "skills.hpp"
#include "klass.hpp"

Warrior::Warrior(void) {
  _name = "warrior";

  // Construct skills for the klass.                level   trains  difficulty   stamina  successRate
  Ability* second_strike = new SecondStrikeSkill(    25,     2,        3,          1,         80);
  Ability* third_strike  = new ThirdStrikeSkill(     50,     3,        3,          2,         80);
  Ability* fourth_strike = new FourthStrikeSkill(    75,     4,        3,          3,         80);
  Ability* dual_wield    = new DualWieldSkill(       45,     2,        3,          2,         55);

  Ability* block         = new BlockSkill(           15,     1,        3,          2,         25);
  Ability* shield_block  = new ShieldBlockSkill(     40,     2,        4,          1,         40);
  Ability* parry         = new ParrySkill(           45,     1,        3,          2,         40);
  Ability* riposte       = new RiposteSkill(         65,     2,        3,          2,         50);
  Ability* counterattack = new CounterattackSkill(   85,     3,        4,          3,         20);

  // Setup dependency graph.
  third_strike->add_dependency(second_strike);
  fourth_strike->add_dependency(third_strike);
  dual_wield->add_dependency(second_strike);
  shield_block->add_dependency(block);
  parry->add_dependency(block);
  riposte->add_dependency(parry);
  riposte->add_dependency(second_strike);

  // Register all the abilities to the klass.
  _abilities.insert(second_strike);
  _abilities.insert(third_strike);
  _abilities.insert(fourth_strike);
  _abilities.insert(dual_wield);
  _abilities.insert(block);
  _abilities.insert(shield_block);
  _abilities.insert(parry);
  _abilities.insert(riposte);
  _abilities.insert(counterattack);
  return;
}
