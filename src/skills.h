
#ifndef H_SYMPHONY_SKILLS
#define H_SYMPHONY_SKILLS

#include "ability.h"

#define SECOND_STRIKE "second strike"
#define THIRD_STRIKE  "third strike"
#define FOURTH_STRIKE "fourth strike"
#define BLOCK         "block"
#define PARRY         "parry"
#define RIPOSTE       "riposte"

DEF_SKILL(SECOND_STRIKE,  SecondStrikeSkill)
DEF_SKILL(THIRD_STRIKE,   ThirdStrikeSkill)
DEF_SKILL(FOURTH_STRIKE,  FourthStrikeSkill)
DEF_SKILL(BLOCK,          BlockSkill)
DEF_SKILL(PARRY,          ParrySkill)
DEF_SKILL(RIPOSTE,        RiposteSkill)

#endif // #ifndef H_SYMPHONY_SKILLS
