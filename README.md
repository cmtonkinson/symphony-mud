# README

## Symphony, the harmonius MUD codebase
Symphony is a modern MUD codebase. The inspiration comes from the Rom-family of MUDs, and the user
experience inherits from that style of play. The primary project goal is **extensibility** in both
form and function. It should be simple and intuitive to customize the interface, extend and modify
the feature set, and create new content. This is achieved by maintaining a minimally complex core
and implementing as much of the game as possible through a robust module system. Some other
engineering considerations include, in no particular order:

 * test coverage
 * security
 * code quality
 * performance
 * ease of maintenance
 * documentation
 * simplicity

### TL;DR
Hit the ground running with the following commands from the project root (assuming you haven't
overridden any default configurations):

 * `bundle install`
 * `bundle exec ruby symphony.rb`

You should see some informational output. Once you see "listening for connections" in another
terminal, telnet to `localhost:6501` and follow the instructions. If you're in a *nix environment
that should look something like:

 * `telnet localhost 6501`

### Detailed Installation

### Philosophy

### Architecture

### TODO

 * vagrant
