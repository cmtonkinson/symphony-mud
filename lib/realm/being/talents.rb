module Realm
  class Being
    attr_accessor :abilities

    define_hooks :before_ability_invocation, :after_ability_invocation

    ###########################################################################
    after_initialize -> {
      @abilities = Array.new
    }

    ###########################################################################
    on_hashify -> {
      hash = Hash.new

      abilities.select(&:learned?).map { |a| hash[a.name] = a.mastery }

      { abilities: hash }
    }

    ###########################################################################
    on_dehashify ->(hash) {
      return unless hash.has_key? :abilities

      hash = hash[:abilities]

      hash.each do |name, mastery|
        a = get_ability name
        next if a.nil?
        a.learn mastery
      end
    }

    ###########################################################################
    def add_enabler(enabler)
      @enablers.push enabler
      enabler.abilities.each { |a| add_ability a }
    end

    ###########################################################################
    def add_ability(ability)
      @abilities.push ability
      ability.added_to self
      ability
    end

    ###########################################################################
    def get_ability(name)
      abilities.find { |a| a.name == name.to_sym }
    end

    ###########################################################################
    # Either learn the Ability successfully, or return a String indicating why
    # that couldn't be done.
    def learn(name)
      ability = get_ability name

      return I18n.t('being.talents.err_not_found') if ability.nil?
      return I18n.t('being.talents.err_learned') if ability.learned?
      return I18n.t('being.talents.err_cannot_learn') unless ability.can_learn?
      return I18n.t('being.talents.err_trains') if ability.trains > training_points

      @training_points -= ability.trains
      ability.learn
      puts I18n.t('being.talents.learned'), ability.trains, ability.name_s

      nil
    end

    ###########################################################################
    def practice(name)
      Symphony.logger.error name
      ability = get_ability name
      cost = 1

      return I18n.t('being.talents.err_not_found') if ability.nil?
      return I18n.t('being.talents.err_not_learned') unless ability.learned?
      return I18n.t('being.talents.err_trains') if training_points < 1
      return I18n.t('being.talents.err_max') if ability.mastery > 69

      @training_points -= cost
      ability.practice
      puts I18n.t('being.talents.practiced'), cost, ability.name_s
    end

    ###########################################################################
    def puts_abilities_list(type = nil, learned = nil)
      abilities
        .select { |a| type.nil? ? true : a.category == type }
        .select { |a| learned.nil? ? true : learned == a.learned? }
        .sort_by(&:level)
        .each do |a|
          a_level = '%3d' % a.level
          a_name  = '%-15s' % a.name_s

          if a.learned?
            puts I18n.t('being.talents.list_learned'), a_level, a_name, Display::Format.mastery(a.mastery)
          else
            deps = a.prerequisites.empty? ? '' : (' requires ' + a.prereq_string)
            puts I18n.t('being.talents.list_' + (a.can_learn? ? 'can_learn' : 'cannot_learn')), a_level, a_name, deps
          end
        end
    end
  end
end
