# Symphony [![build status](https://gitlab.tonkinson.net/symphony/engine/badges/master/build.svg)](https://gitlab.tonkinson.net/symphony/engine/commits/master)
The harmonius MUD codebase

---
Symphony is a modern MUD codebase. The inspiration comes from the Rom-family of MUDs, and the user experience inherits from that style of play. The primary project goal is **extensibility** in both form and function. It should be simple and intuitive to customize the interface, extend and modify the feature set, and create new content. This is achieved by maintaining a minimally complex core and implementing as much of the game as possible through a robust module system. Some other engineering considerations include, in no particular order:

 * test coverage
 * security
 * code quality
 * performance
 * ease of maintenance
 * documentation
 * simplicity

### Performance
Symphony is tested to ensure it scales well. The most recent performance test shows it takes over 10k live NPCs simultaneously active in the system in order to induce meaningful TUI lag and get over 95% (single-core) CPU utilization on an Intel Core i5-4570S. At that point, Symphony utilized approximately 600MB of RAM.

### TL;DR
Hit the ground running with the following commands from the project root (assuming you haven't
overridden any default configurations):

 * `bundle install`
 * `bundle exec ruby symphony.rb`

You should see some informational output. Once you see "listening for connections" in another
terminal, telnet to `localhost:6500` and follow the instructions. If you're in a *nix environment
that should look something like:

 * `telnet localhost 6500`

### Detailed Installation

### Philosophy

### Architecture

### TODO

