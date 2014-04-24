# Engine Command Module

## Defining new commands

Symphony has a metaprogramming interface (commonly, wrongly, called a "DSL" in Ruby) to make
defining new commands as clean as possible. These can be defined in arbitrary files matching  the
pattern `lib/commands/*.rb`.

Here's an example:

    module Command
      # The command name will be created by sending #to_s on this variable.
      define :wink do
        # Minimum required character level required to execute the command. (default 1)
        level 10
        # Command group - arbitrary (useful for grouping related commands, default :global)
        group :social
        # The block here is the actual command code to be executed when a user issues the command.
        # Certain instance variables are available within command exec code:
        #  @user         - Base::User which invoked the command.
        #  @full_command - String of the entire command line entry.
        #  @arg_string   - String of the command line entry, minus the command name itself.
        exec do
          @user.puts "You wink suggestively."
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
          @name = "wink"
          @level = 10
          @group = :social
          @exec = Proc.new do
            @user.puts "You wink suggestively."
          end
        end
      end
    end

## TODOs:

 * require preregistration of command sets, to enforce options such as modality
