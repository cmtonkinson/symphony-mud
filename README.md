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
Hit the ground running with the following commands from the project root:

 * `bundle install`
 * `cp -r sample game`
 * `bundle exec symphony.rb`

You should see some informational output. Once you see "listening for connections" in another terminal, telnet to `localhost:6500` and follow the instructions. If you're in a *nix environment that should look something like:

 * `telnet localhost 6500`

---
## Directory Structure

### Default directories
The Symphony directory heirarchy attemps to stay as flat as possible (within reason). Here is a brief overview of what you will find in each top-level directory:

| Directory name | Contents |
| --- | --- |
| `boot/` | application configuration & setup scripts |
| `commands/` | definition of available in-game commands |
| `data/` | storage for server & game state (zones, avatars, etc) |
| `game/` | does not exist (see below) |
| `help/` | content files for help output |
| `lib/` | primary software modules for the game engine |
| `locales/` | text language translations for i18n |
| `sample/` | basic example / starting point for `game/` (see below) |
| `scripts/` | utilities than can be run in game-world context |
| `spec/` | RSpec unit tests for the core modules in `lib/` |
| `templates/`  | output template files for stylized panels |
| `test/` | system tests (see [test/README.md](test/README.md) for more) |
| `tmp/` | temporary files produced & consumed by Symphony at runtime |

### The `game/` directory
The top-level `game/` directory does not exist in this repository and is excluded by `.gitignore`. That's because this is where your content belongs. The intent is that this is where you build your own customized game world, overlays, extensions, monkeypatches, and other creative outputs. However, you can find a basic working example to get started in the `sample/` directory &mdash; simply `cp -r sample game`. You are at liberty to use the actual content as a starting point as-is, or simply reference the code there for your own implementations.

In practice, you'll want to keep your game code both A) source controlled and B) separate from the Symphony engine. I recommend you keep your game code in a separate repsitory, and then symlink that from here. Here's an example of what that could look like:

```bash
# Initialize a top-level folder for your work.
mkdir your-mud
cd your-mud

# Get Symphony.
git clone https://gitlab.com/cmtonkinson/symphony.git symphony

# Create your game repository.
git clone https://gitlab.com/yourname/yourgame.git game

# Symlink your game directory from within the Symphony workspace.
ln -s game symphony/game
```

This allows you to track upstream changes to Symphony over time separately from your own code, without using a complicated submodule setup.

### Boot Order
Understanding the order in which Symphony loads files during startup (at a high-level &mdash; this is an overview, not a deep dive) helps make it clear what parts of the system come from where, and how the application engine is meant to relate to your game and its extensions/customizations.

Translations get loaded first:
```
locales/*
game/locales/*
```

Then constants:
```
lib/konst.rb
game/konst.rb
```

Then core logic:
```
lib/*
game/lib/*
```

Next, command definitions:
```
commands/*
game/commands/*
```

And finally, any supplemental logic:
```
boot/setup/*
game/setup/*
```

Generally speaking, the content in the top level `setup`, `locales`, `help`, `commands`, `lib`, and `templates` directories can be extended, supplemented, or overshadowed by creating a matching folder/file under `game`.

## Detailed Installation

## Philosophy

## Architecture

## TODO
