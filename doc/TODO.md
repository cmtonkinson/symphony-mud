# Generic features, concepts, and pipe dreams

## Combat, Abilities
  * offense/defense calculations
  * crafts
  * languages
  * regeneration

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
  * quote buffer

## Building
  * absolute map coordinates
  * placement enhancements

## Engine
  * Security
    * login throttling
    * banning
    * password storage (will libsodium support PBKDF2?)
  * Performance
    * stat tracking on
      * turn processing time
  * Account emails
    * Integrate with POP/SMTP for email validation?
    * Integrate board system with email?
  * Re-examine FlagBank: convert to uint64?

## General
  * hunger, thirst, drunk
    * food, drink
  * npc scavenging
  * weather
    * factor date, time
  * time
    * moon phases
  * recall
  * spell items (wands, scrolls)
  * currency
  * color toggle
  * md-based help docs
  * rb test harness

---

# Specific pending actionables from existing logic - alphabetical order

## Ability::invoke()

Factor difficulty into learning probabilities.

## Being::aquireTarget()

Aggro/threat analysis instead of just returning the first attackable opponent.

## Being::autoassist()

Make autoassist a togglable setting.

## Being::die()

Leave a corpse with loot.

## Being::scheduleAttack()

Stat-based attack timing.

## Being::strike()

Refactor damage calculation so it can be re-used elsewhere e.g. in RiposteSkill::execute()

## Being::targetTNL()

Is there anything worthwhile from the character creation sequence that can be used to make the TNL calculations dynamic?

## Npc::auto_move()

Implement implement some concept of "momentum" so that the same Npc doesn't keep flipping in and out of the same two or three Rooms over and over again in a huge Zone.

## OS::log()
Replace the `std::cout` with graduated outputs to stdout, stderr, files.
