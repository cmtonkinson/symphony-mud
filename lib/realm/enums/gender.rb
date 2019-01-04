module Realm
  class Gender
    include Ruby::Enum

    define :MALE,   I18n.t('linguist.gender.male')
    define :FEMALE, I18n.t('linguist.gender.female')
    define :NEUTER, I18n.t('linguist.gender.neuter')

    @pronouns = {
      objective: {
        MALE: I18n.t('linguist.pronoun.objective.male'),
        FEMALE: I18n.t('linguist.pronoun.objective.female'),
        NEUTER: I18n.t('linguist.pronoun.objective.neuter'),
      },
      subjective: {
        MALE: I18n.t('linguist.pronoun.subjective.male'),
        FEMALE: I18n.t('linguist.pronoun.subjective.female'),
        NEUTER: I18n.t('linguist.pronoun.subjective.neuter'),
      },
      possessive: {
        MALE: I18n.t('linguist.pronoun.possessive.male'),
        FEMALE: I18n.t('linguist.pronoun.possessive.female'),
        NEUTER: I18n.t('linguist.pronoun.possessive.neuter'),
      },
      reflexive: {
        MALE: I18n.t('linguist.pronoun.reflexive.male'),
        FEMALE: I18n.t('linguist.pronoun.reflexive.female'),
        NEUTER: I18n.t('linguist.pronoun.reflexive.neuter'),
      },
    }

    @pronouns.each do |type, defns|
      self.class.send :define_method, "#{type}_pronoun" do |gender|
        defns[key gender]
      end
    end
  end
end
