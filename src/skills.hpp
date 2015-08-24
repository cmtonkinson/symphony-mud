
#ifndef H_SYMPHONY_SKILLS
#define H_SYMPHONY_SKILLS

#include "ability.hpp"

///////////////////////////////////////////////////////////////////////////////
// ATTACKS
///////////////////////////////////////////////////////////////////////////////
#define SECOND_STRIKE   "second strike"
#define THIRD_STRIKE    "third strike"
#define FOURTH_STRIKE   "fourth strike"
#define DUAL_WIELD      "dual wield"
#define COUNTERATTACK   "counterattack"

DEF_SKILL(SECOND_STRIKE,  SecondStrikeSkill,  Ability::TARGET_BEING)
DEF_SKILL(THIRD_STRIKE,   ThirdStrikeSkill,   Ability::TARGET_BEING)
DEF_SKILL(FOURTH_STRIKE,  FourthStrikeSkill,  Ability::TARGET_BEING)
DEF_SKILL(DUAL_WIELD,     DualWieldSkill,     Ability::TARGET_BEING)
DEF_SKILL(COUNTERATTACK,  CounterattackSkill, Ability::TARGET_BEING)

///////////////////////////////////////////////////////////////////////////////
// AVOIDANCE
///////////////////////////////////////////////////////////////////////////////
#define BLOCK         "block"
#define DODGE         "dodge"
#define DUCK          "duck"
#define PARRY         "parry"
#define RIPOSTE       "riposte"
#define SHIELD_BLOCK  "shield block"

DEF_SKILL(BLOCK,          BlockSkill,         Ability::TARGET_BEING)
DEF_SKILL(DODGE,          DodgeSkill,         Ability::TARGET_BEING)
DEF_SKILL(DUCK,           DuckSkill,          Ability::TARGET_BEING)
DEF_SKILL(PARRY,          ParrySkill,         Ability::TARGET_BEING)
DEF_SKILL(RIPOSTE,        RiposteSkill,       Ability::TARGET_BEING | Ability::TARGET_ITEM)
DEF_SKILL(SHIELD_BLOCK,   ShieldBlockSkill,   Ability::TARGET_BEING)

#endif // #ifndef H_SYMPHONY_SKILLS
