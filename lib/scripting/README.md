# Scripting

## 1. Introduction
This module allows for Symphony to execute arbitrary scripts for the purpose
of allowing (predominantly world builders) to create a more dynamic, custom
storytelling experience.

The Script class is the primary consumer interface for the module. While it
attempts to abstract some of the lower level details, honestly I don't know
at the time of authorship that we will ever implement anything besides Lua.

## 2. Lua
Create a new Script object.

```ruby
script = Script.new
```

Then set the script code to be executed.

```ruby
script.script = "local x = 0;"
```

Then, in order for the script to actually interact with the game world, you
need to pass in named objects. This is done using the `set_context` method.
Within [Abstractions.rb](abstractions.rb) we map object classes (these may or
may not be subclasses of `Objekt`) to a set of method wrappers which allow us
to pass\* curated/protected versions of the native objects into the script. For
a given context name/object pair, the system will generate a set of global
functions concatenating the given name with the method signature provided in
the Abstraction configuration.

```ruby
# Assume we have a Realm::Avatar called avatar.
# Further assume that there exist Abstraction mappings for Realm::Avatar to
# a method labeled 'puts' and a method labeled 'level'.

script.set_context('hero', avatar)

# Within the Lua script, we'll then have the following functions available:
#   * hero_puts(:string) -> :void
#   * hero_level() -> :integer
```

Finally, you execute the script

```ruby
script.run
```

## 3. Errors

`Scripting::ParsingError` - Raised from `Script::script` if the user script
can't be parsed.

`Scripting::ArgumentError` - Raised from `Script::run` if the user script
calls a method with the wrong number or type of arugments.

`Scripting::ReturnTypeError` - Raised from `Script::run` if the user script
invokes a function with a defined return type, but the function doesn't return
the expected type.

`Scripting::RuntimeError` - Raised from `Script::run` if the user script
exceeds the maximum defined execution time (defined in wall time).

`Scripting::RuntimeError` - Raised from `Script::run` if there are any other
unhandled errors during script execution.

## 4. Notes
\* The system does not actually pass any objects into Lua. It uses the FFI
to create a C function that calls a Ruby method which invokes the Abstraction
labmda against the underlying object. It's a lot of context switching, but it
works!
