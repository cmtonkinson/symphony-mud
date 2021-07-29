# Symphony  
The harmonius MUD codebase  
[![pipeline status](http://gitlab.tonkinson.net/symphony/engine/badges/master/pipeline.svg)](http://gitlab.tonkinson.net/symphony/engine/-/commits/master) [![coverage report](http://gitlab.tonkinson.net/symphony/engine/badges/master/coverage.svg)](http://gitlab.tonkinson.net/symphony/engine/-/commits/master)  


---
Symphony is a modern MUD codebase. The inspiration comes from the Rom-family of MUDs, and the user experience inherits from that style of play. The primary project goal is **extensibility** in both form and function. It should be simple and intuitive to customize the interface, extend and modify the feature set, and create new content. This is achieved by maintaining a minimally complex core and implementing as much of the game as possible through a robust module system. Some other engineering considerations include, in no particular order:

 * test coverage
 * security
 * code quality
 * performance
 * ease of maintenance
 * documentation
 * simplicity

## Performance
Symphony is tested to ensure it scales well. The most recent performance test shows it takes over 10k live NPCs simultaneously active in the system in order to induce meaningful TUI lag and get over 95% (single-core) CPU utilization on an Intel Core i5-4570S. At that point, Symphony utilized approximately 600MB of RAM.

## TL;DR
Hit the ground running with the following commands from the project root (assuming you haven't overridden any default configurations):

 * `bundle install`
 * `bundle exec ruby symphony.rb`

You should see some informational output. Once you see "listening for connections" in another terminal, telnet to `localhost:6500` and follow the instructions. If you're in a *nix environment that should look something like:

 * `telnet localhost 6500`

---
## Directory Structure
Understanding the order in which Symphony loads files during startup at a high-level helps make it clear what parts of the system come from where, and how the application engine code is meant to relate to your game and its extensions/customizations.

Translations get loaded first:
```
locales
game/locales
```

Then global constants and the main codebase:
```
core/konst.rb
game/konst.rb
core
game/lib
```

Next, command definitions:
```
commands
game/commands
```

And finally, any supplemental logic:
```
boot/setup
game/setup
```

There are of course other directories and files of importance as well:
  * `spec/` &mdash; RSpec unit tests for the core modules
  * `test/` &mdash; custom system test solution
  * `data/` &mdash; holds all server state (zones, avatars, etc)
  * `templates/`, `game/templates/`, `help/`, and `game/help` &mdash; contain stylized output templates and help files, respectively

## Detailed Installation

## Philosophy

## Architecture

## TODO

