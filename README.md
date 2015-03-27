# Symphony MUD
Symphony MUD is a modern, extensible, open source MUD server codebase intended as a replacement for the Diku/Merc/ROM line of games. See the [announcement][announcement] for details.

---
## Features

  * Hot-reboot ("copyover")
  * Global autosaving
  * Flexible character creation sequence
  * Modular and extensible API for defining new commands
  * Persistent world (with persistent items & equipment)
  * In-game editor suite for world building ("OLC" in ROM-speak)
    * Zones
    * NPCs
    * Items
    * Social commands
    * ... and more
  * In-game message board system
  * Level based permission system
  * Tree-based ability system
  * Semi-managed auto-combat system
  * Dynamic enabling/disabling of commands
  * A customizable character creation sequence
  * A generic framework for scheduled & recurring events (called Jobs)

---
## Getting Started
Symphony MUD is built, tested, and hacked in a [Vagrant][vagrant] virtual machine. If you're familiar with Vagrant, the instructions couldn't be easier:

```bash
# Clone the repository.
git clone https://github.com/cmtonkinson/symphonymud
cd symphonymud
# Bring in submodules.
git submodule init
git submodule update
# Log in to the virtual machine.
vagrant up
vagrant ssh
# Compile the source.
make
# Boot Symphony MUD.
bin/symphony
```

If you're not familiar with Vagrant, you should be (because it's a great tool). Worst case, if you have a Linux system you should be able to get Symphony MUD to compile and run as long as you meet the dependencies (check out [provision.sh](provision.sh) and the [makefile](makefile) for more details).

---
## Detailed Setup
Symphony MUD is developed with clang (using the C++11 standard) on Ubuntu 14.04 x64 with the 3.13 kernel. `libpcre` and `libsodium` are prerequisites. `libpcre` is used throughout the code for various string manipulation, while `libsodium` is at present only used for password storage.

PCRE is installed (as per [provision.sh](provision.sh)) via `apt`, while `libsodium` is included as a submodule which is built and installed using autotools.

`ccache` and `gdb` are used for ease of development and testing, but are not required. Settings for these can be found in the [makefile](makefile) as well as in the additions to `~/.bashrc` made by [provision.sh](provision.sh).

---
## Data
All data is persisted to disk in plain old human readable text format under `data/`. MySQL has been used previously, but turns out not to be a great fit for a MUD. Currently most data is stored in a custom serialization format due to trouble getting a YAML library (particularly, [yaml-cpp][yaml-cpp]) to compile cleanly. A pull request integrating a YAML-based replacement for the current `Storage` class would be accepted enthusiastically.

---
## Changelog
[Available in markdown format.](doc/CHANGELOG.md)

---
## Get Involved
#### Reporting Bugs
Bug reports should be submitted on [Github][bugs].

#### Contributing Code

  1. [Fork symphonymud on Github][fork]
  1. Create a feature branch (`git checkout -b my-feature-branch`)
  1. Make your changes and commit (`git commit -am "Add my new feature"`)
  1. Push your branch (`git push origin my-feature-branch`)
  1. Create a pull request

---
## Credits
### Author: [Chris Tonkinson][chris] (aka "Xantiloth")

### Contributors:
Original inspiration for Symphony MUD comes from the excellent (sadly now defunct) ROM-derived *Feudal Realms* by Charles Webber (aka "Turloch"). Some internals take initial inspiration from *Scratch*, a telnet chat server by Jared Devall (aka Rendelven).

---
## License
Copyright © Chris Tonkinson 2004-2015. Symphony MUD is published under the [MIT license](doc/LICENSE.md).



[vagrant]: https://www.vagrantup.com/
[bugs]: https://github.com/cmtonkinson/symphonymud/issues
[fork]: https://github.com/cmtonkinson/symphonymud/fork
[chris]: http://chris.tonkinson.com/
[announcement]: http://chris.tonkinson.com/2015/symphony-mud.html
