# Generic features, concepts, and pipe dreams

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
  * Security
    * login throttling
    * banning
    * password storage (will libsodium support PBKDF2?)
  * Performance
    * jobs per turn
    * turn sleep
    * stat tracking on
      * turn processing time
      * job processing time
      * job execution delay
  * Possible rename "Object" -> "Item"
  * More flexible logging system
    * Engine-level events (bugs, diagnostics)
    * World-level events (timers, resets)
    * Player-level events (logins, pk's, levels)
  * Account emails
    * Integrate with POP/SMTP for email validation?
    * Integrate board system with email?
  * Re-examine FlagBank: convert to uint64?

## General
  * hunger, thirst, drunk
    * food, drink
  * mob scavenging
  * weather
    * factor date, time
  * time
    * moon phases
  * recall
  * spell objects (wands, scrolls)

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
