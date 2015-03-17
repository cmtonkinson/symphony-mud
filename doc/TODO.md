# Generic features and concepts

## Combat, Abilities
  * offense/defense calculations
  * Log kills, deaths
  * crafts
  * languages

## UI
  * say
  * tell/reply
  * custom promtps
  * aliases
  * macros?
  * triggers?
    * autoassist
    * autoexit
    * autoloot
    * autogold

## Building
  * absolute map coordinates
  * loadrule enhancements

## Engine
  * security: login throttling, password storage
  * performance: jobs per turn, turn sleep
  * performance: stat tracking on turn processing time, job processing time, job execution delay, etc
  * Log logins, logouts, d/c
  * Account emails
  * FlagBank to long int
  * Logging bugs (switch defaults, bad indices, etc)

## General
  * hunger, thirst, drunk
    * food, drink
  * mob scavenging
  * weather
    * factor date, time
  * time
    * moon phases
  * recall
  * exit flags not saving

## MySQL -> text

[X] area
[X]   room
[X]     exit
[X]     load_rule
[X]   object
[X]   mob
[X] avatar
[X]   inventory
[X]   equipment
[X]   abilities
[ ]   pets
[ ] area permissions
[ ] disabled_commands
[X] socials
[X] boards
[X]   notes

---

# Specific pending actionables from existing logic - alphabetical order

## Ability::invoke()

Factor difficulty into learning probabilities.

## Creature::aquireTarget()

Aggro/threat analysis instead of just returning the first attackable opponent.

## Creature::autoassist()

Make autoassist a togglable setting.

## Creature::die()

Leave a corpse with loot.

## Creature::scheduleAttack()

Stat-based attack timing.

## Creature::targetTNL()

Is there anything worthwhile from the character creation sequence that can be used to make the TNL calculations dynamic?

## Mob::auto_move()

Implement implement some concept of "momentum" so that the same Mob doesn't keep flipping in and out of the same two or three Rooms over and over again in a huge Area.
