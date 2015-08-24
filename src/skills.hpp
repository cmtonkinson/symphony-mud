
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

DEF_SKILL(SECOND_STRIKE,  SecondStrikeSkill)
DEF_SKILL(THIRD_STRIKE,   ThirdStrikeSkill)
DEF_SKILL(FOURTH_STRIKE,  FourthStrikeSkill)
DEF_SKILL(DUAL_WIELD,     DualWieldSkill)
DEF_SKILL(COUNTERATTACK,  CounterattackSkill)

///////////////////////////////////////////////////////////////////////////////
// AVOIDANCE
///////////////////////////////////////////////////////////////////////////////
#define BLOCK         "block"
#define DODGE         "dodge"
#define DUCK          "duck"
#define PARRY         "parry"
#define RIPOSTE       "riposte"
#define SHIELD_BLOCK  "shield block"

DEF_SKILL(BLOCK,          BlockSkill)
DEF_SKILL(DODGE,          DodgeSkill)
DEF_SKILL(DUCK,           DuckSkill)
DEF_SKILL(PARRY,          ParrySkill)
DEF_SKILL(RIPOSTE,        RiposteSkill)
DEF_SKILL(SHIELD_BLOCK,   ShieldBlockSkill)

#endif // #ifndef H_SYMPHONY_SKILLS
