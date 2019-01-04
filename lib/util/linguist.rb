module Util
  class Linguist
    ###########################################################################
    def self.interpret(format, *args)
      message = format.gsub /\$[a-z]+/i do |interpreter|
        if args.empty?
          Symphony.logger.error "Room#interpret with format '#{format}' - no args left to interpret '#{interpreter}'"
          next Realm.configuration.error_string
        end

        arg = args.shift

        begin
          send 'interpret_' + interpreter[1..-1], arg
        rescue NoMethodError
          Symphony.logger.error "Room#interpret with format '#{format}' - no interpreter for '#{interpreter}' (arg is a #{arg.class.name})"
          Realm.configuration.error_string
        end
      end

      unless args.empty?
        Symphony.logger.error "Room#interpret with format '#{format}' - args leftover after interpreters exhausted (remaining: [#{args.join ', '}])"
      end

      message
    end

    ###########################################################################
    def self.interpret_string(arg)
      arg
    end

    ###########################################################################
    def self.interpret_rid(arg)
      arg.rid
    end

    ###########################################################################
    def self.interpret_being(arg)
      prepend_article arg
    end

    ###########################################################################
    def self.interpret_room(arg)
      prepend_article arg
    end

    ###########################################################################
    def self.interpret_zone(arg)
      arg.name
    end

    ###########################################################################
    def self.interpret_item(arg)
      prepend_article arg
    end

    ###########################################################################
    def self.interpret_exit(arg)
      I18n.t "movement.exit_from.#{arg}"
    end

    ###########################################################################
    def self.interpret_opposite(arg)
      opposite = Realm::Exit::DIRECTIONS[arg][:opposite]
      I18n.t "movement.exit_from.#{opposite}"
    end

    ###########################################################################
    def self.interpret_strike(arg)
      arg.action
    end

    ###########################################################################
    def self.interpret_ability(arg)
      arg.name
    end

    ###########################################################################
    def self.interpret_subjective(arg)
      Realm::Gender.subjective_pronoun arg.gender
    end

    ###########################################################################
    def self.interpret_objective(arg)
      Realm::Gender.objective_pronoun arg.gender
    end

    ###########################################################################
    def self.interpret_possessive(arg)
      Realm::Gender.possessive_pronoun arg.gender
    end

    ###########################################################################
    def self.interpret_reflexive(arg)
      Realm::Gender.reflexive_pronoun arg.gender
    end

    ###########################################################################
    def self.prepend_article(arg)
      name       = arg.name.clone
      first_char = name[0]

      # Don't modify Avatar names.
      return name if arg.is_a? Realm::Avatar

      # It's some other kind of proper noun, so we leave it be.
      return name if first_char == first_char.upcase

      # Starts with a vowel, so we add "an"
      return name.prepend (I18n.t('linguist.article.indefinite.vowel') + ' ') if first_char =~ /[aeiou]/i

      # Just a regular name, so we add "a"
      return name.prepend (I18n.t('linguist.article.indefinite.consonant') + ' ')
    end

    ###########################################################################
    # Truncate the middle of a `source` string, replacing enough characters
    # with an elipses so that the result is no longer than `final_length`.
    # From http://stackoverflow.com/a/12203172/77047
    def self.truncate_middle(source, final_length)
      half = (final_length - 3) / 2
      splits = source.scan(/(.{0,#{half}})(.*?)(.{0,#{half}}$)/)[0]
      splits[1] = '...' if splits[1].length > 3
      splits.join
    end
  end
end
