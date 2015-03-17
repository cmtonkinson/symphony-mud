Symphony is an Open Source (GPLv3) MUD codebase, meant to replace the aging Diku/Merc/ROM line.  
Symphony is written in ANSI standard compliant C++ and was originally derived from the 
[http://www.dragonssquare.com/ Scratch MUD] codebase (link may or may not be up).

The Symphony codebase is comprised of entirely custom source (although certain components are 
derived almost directly from Scratch).

----

## Current state:
The /trunk has been cleaned out to prepare for the rewrite. If you were expecting code there, 
it has been moved to /tags/old_versions/version-2.0-final.
----
## Major/High-level features (thus far):

  * Hot-reboot ("copyover")
  * Global autosave
  * Persistent world (with persistent objects & equipment)
  * In-game editor suite for world building ("OLC" in Rom-speak)
  * In-game social command editor
  * In-game message board system
  * Level-based permission system
  * Dynamic enabling/disabling of commands
  * A customizable character creation sequence

## Planned features:

  * Tree-based ability acquisition system
  * Semi-managed auto-combat with formation-based grouping
  * muscl, a custom MUD "scripting" language

## Administrative junk:

  * Licensed under the GNU GPLv3
  * ANSI standard compliant C++
  * Some minimal C code; I wrapped all C libraries (think PCRE, MySQL, pthreads, etc.) in C++ 
classes.  These are wrappers I use frequently in all of my projects, so they're very stable.
  * MySQL (tested on 5.x) for persistent storage. We're moving to  
[YAML](http://code.google.com/p/yaml-cpp/) to reduce dependencies.
  * Tested on Linux (2.6 kernel) and GCC (4.4)
  * Requires PCRE and MySQL libraries
