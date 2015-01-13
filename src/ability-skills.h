
#ifndef H_SYMPHONY_ABILITY_SKILLS
#define H_SYMPHONY_ABILITY_SKILLS

#include "ability.h"

#define SECOND_STRIKE "second strike"
#define THIRD_STRIKE  "third strike"
#define FOURTH_STRIKE "fourth strike"
#define BLOCK         "block"
#define PARRY         "parry"
#define RIPOSTE       "riposte"

DEF_ABILITY(SECOND_STRIKE,  SecondStrikeSkill)
DEF_ABILITY(THIRD_STRIKE,   ThirdStrikeSkill)
DEF_ABILITY(FOURTH_STRIKE,  FourthStrikeSkill)
DEF_ABILITY(BLOCK,          BlockSkill)
DEF_ABILITY(PARRY,          ParrySkill)
DEF_ABILITY(RIPOSTE,        RiposteSkill)

#endif // #ifndef H_SYMPHONY_ABILITY_SKILLS
