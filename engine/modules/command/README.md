# Engine Command Module

## Defining new commands

Symphony has a metaprogramming interface (commonly, wrongly, called a "DSL" in Ruby) to make
defining new commands as clean as possible. These can be defined in arbitrary files matching the
pattern `lib/commands/*.rb`.

Here's an example:

    module Command
      # The command name will be created by sending #to_s on this variable.
      define :wink do

        # Minimum required character level required to execute the command. (default: `1`)
        level 10

        # Command group - arbitrary symbol (useful for grouping related stuff, default: `:global`)
        group :social

        # Usages - ways to invoke the command
        usage # command can be invoked with no arguments
        usage :target # command can be issued at someone/something
        usage "at", :target # same as above, but explicitly aware of English preposition

        # The `exec` block is the actual code to be run when a user issues the command.
        # Certain methods are available by default within the `exec` block:
        #  user       - Base::User which invoked the command.
        #  user_input - String of the entire command line entry.
        exec do
          if target
            user.puts "You wink suggestively at #{target.name}."
          else
            user.puts "You wink suggestively."
          end
        end
      end
    end

If you want to do things the manual way, the same command would look something like this. Why would
you want to? No idea, but you can. Knowing this helps explain what the heck Command::define does
though, since that method is so meta.

    module Command
      Command_wink < Command
        def initialize
          setup
          @name  = "wink"
          @level = 10
          @group = :social
          @usages.push Argument.new(), Argument.new(:target), Argument.new("at", :target)
          @exec = Proc.new do
            if target
              user.puts "You wink suggestively at #{target.name}."
            else
              user.puts "You wink suggestively."
            end
          end
        end
      end
    end

## TODOs:

 * require preregistration of command sets, to enforce options such as modality
