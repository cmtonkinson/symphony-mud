# Symphony MUD

Symphony MUD is a modern, extensible, open source MUD server codebase intended as a replacement for the Diku/Merc/ROM line of games.

---
## Features

  * Hot-reboot ("copyover")
  * Global autosaving
  * Flexible character creation sequence
  * Modular and extensible API for defining new commands
  * Persistent world (with persistent items & equipment)
  * In-game editor suite for world building ("OLC" in Rom-speak)
  * In-game social command editor
  * In-game message board system
  * Level based permission system
  * Tree-based ability system
  * Semi-managed auto-combat system
  * Dynamic enabling/disabling of commands
  * A customizable character creation sequence

---
## Getting Started

Symphony MUD is built, tested, and hacked in a [Vagrant](https://www.vagrantup.com/) virtual machine. If you're familiar with Vagrant, the instructions couldn't be easier:

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

If you're not familiar with Vagrant, you should be. Worst case, if you have a Linux system you should be able to get Symphony MUD to compile and run as long as you meet the dependencies (check out [provision.sh](provision.sh) and the [makefile](makefile) for more details).

---
## Code

The code is moving toward C++11 (still with large sections of older C and C++) but compiles cleanly against clang on Linux. The following third party libraries are prerequisite:

  * PCRE (not included)
  * libsodium (included)

---
## Data

All data is persisted to disk in plain old text format. A conversion to YAML (instead of the custom serialization format) would be a very welcome pull request indeed.

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

### Author:

  * Xantiloth ([Chris Tonkinson][chris])

### Contributors (alphabetical):

  * Dromynus
  * Marek
  * Turin
  * Zantix

Original inpsiration for Symphony MUD comes from the excellent (sadly now defunct) ROM-derived *Feudal Realms* by Charles Webber (aka "Turloch"). Some internals take initial inspiration from *Scratch*, a telnet chat server by Jared Devall (aka Rendelven).

---
## License

Copyright © Chris Tonkinson 2004-2015. Symphony MUD is published under the [MIT license](doc/LICENSE.md).



[bugs]: https://github.com/cmtonkinson/symphonymud/issues
[fork]: https://github.com/cmtonkinson/symphonymud/fork
[chris]: http://chris.tonkinson.com/
