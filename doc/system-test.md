# Symphony System Tests
While Symphony uses RSpec for low-level unit testing, higher level system tests are also very useful in a complex system. Symphony comes with its own purpose-built system testing facility. At a high level, this allows for both isolated and related tests to be run completely from the perspective of a user connecting via telnet. These system tests synthesize end-user input against a full-featured server backend. As such, while they are high level and can be incredibly useful for emulating complex in-game situations, they are not as fast and efficient as the unit tests.

## Overview
Scenarios are the various system tests that are run. Each scenario is defined in arbitrary files under `test/scenarios`. Each scenario needs a unique name to identify itself. Each scenario (to be useful) must specify the name of an Avatar account to be used during the test. And each scenario defines a test block where input is sent through the specified account(s) and output can be examined.

### A trivial example
```ruby
module Test
  define :say_hello do
    with :batman
    test do
      batman 'say hello'
      expect /You.*say.*hello/
    end
  end
end
```

This example creates a system test scenario called `say_hello` which boots up a clean server environment from an empty data directory (`tmp/test-state/say_hello/data`). It creates a single client identified as `batman`. Once the server is running and the client is connected, the command `say hello` is sent through the `batman` client. As you might find in other popular test frameworks, we can use `expect` (and `n_expect` for "not/negative expect") and pass either a string or a regexp. Each call to `expect`/`n_expect` is counted as an assertion in the test.

There are actually six assertion methods available:
  * `expect_last` takes two parameters: a client name, and a string/regexp. If the string is found in (or the regexp matches) the last output message of the given client, then the assertion will pass.
  * `n_expect_last` works exactly the same, except the assertion will pass if the substring is NOT found, or the regexp does NOT match the last output message of the given client.
  * `expect_any` is similar, but looks for the string/regexp in the entire output log of the client since it's initial connection, not just whatever output was last sent to it.
  * `n_expect_any` works similarly, but negated.
  * `expect` is a shorcut method for `expect_last` that automatically uses the last client to have been given an input command as its first argument. In other words, given a scenario with only one client, calling `expect_last(client, 'mysubstr')` is exactly equivalent to calling `expect('mysubstr')`. Again, in a scenario with more than one client, `expect()` will examine the output of the last connection to have been issued a command.
  * Ditto `n_expect` being a shorthand for `n_expect_last` using the last active client.

In addition, any exceptions raised from within your tests will count as a failure. The output from our little example might look like this:

```text
$> bundle exec test/test.rb
Bootstrapping system test suite...
Seed 25418723
Loading complete in 0.046s

say_hello: pass (1 assertion in 1.357s)


Seed 25418723
System testing complete in 1.970s.
  1 tests
  1 passed
  0 failed
```

As you can see,
  1. The random seed for the run is displayed and can be set on future invocations by passing the `-s` / `--seed` flag on the command line.
  2. Each scenario is printed on a line noting the number of assertions executed, the overall status of the scenario, and time taken.
  3. Total number of tests with pass/fail breakdown and total time is shown at the end.

### A more complex example
```ruby
module Test
  define :say_hello do
    with :batman
    with :superman, { klass: 'princess', gender: 'female' }
    test do
      batman 'say hello'
      expect 'You say, "hello"'
      expect_last superman, 'batman says, "hello"'
    end
  end
end
```

Here, we define two clients, `batman` and `superman`. It's worth noting that since every scenario begins by default with a completely clean/fresh environment directory, there are no Zones, and no Avatars. Symphony dynamically generates one default Zone (Origin) with one Room (Void) by default in this case. But if you recall from [doc/install.md](doc/install.md), the first Avatar account created in the system is automatically promoted to system administrator. Thus, the first client you define your scenario (via `with`) will be promoted before the test executes. You may choose to develop the habit of defining the first client as `admin` to help keep things clear.

In this case, `batman` is the first client defined for this scenario, and thus is promoted. `superman`, being an all around lesser hero in nature and quality, is not. You can also see with `superman` something else new - overrides to creation sequence defaults. There are statically defined default attributes defined in the test code:
```ruby
    def default_creation_opts
      {
        race: 'human',
        gender: 'female',
        hand: 'right',
        height: 'average',
        weight: 'average',
        klass: 'warrior'
      }
    end
```

Because we are creating actual telnet connections, each connection needs to log into the game which means a username (whatever you passed to `with`), a statically defined password since it doesn't need to be secure, as well as these character creation sequence choices, which can be overridden by passing a `Hash` that will be merged over the defaults, as shown above (which serves the secondary objective of further illustrating my opinion of `superman`).

Back to the example above, you can see that `batman` says something, and we ensure that it is shown in his output buffer using the `expect` shortcut method. Then we validate that `superman` also saw the correct output but we need to use the `expect_last` method.

Finally, note that you can manually inspect outbut buffers yourself. In our example here, a `batman_client` and a `superman_client` object are dynamically created, and `_client` objects have a `buffer` property which is an `Array` of `String` holding all output messages the client has received since connection.

Putting it all together, note how the following snippets are all logically equivalent (that is, they cause a test failure if the word `'gotham'` does not appear in output after `batman` says it):

_Ex. A_
```ruby
batman 'say gotham'
expect /gotham/
```

_Ex. B_
```ruby
batman 'say gotham'
expect_last batman, /gotham/
```

_Ex. C_
```ruby
batman 'say gotham'
raise 'not found' unless batman_client.buffer[-1].match /gotham/
```

## Heirarchy
 